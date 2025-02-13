#ifdef INTELLISENSE_DIRECTIVES
#	include "os_linux.h"
#	include "os/os.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Globals

MD_API_C md_global        MD_OS_LNX_State          md_os_lnx_state           = {0};
MD_API_C md_thread_static MD_OS_LNX_SafeCallChain* md_os_lnx_safe_call_chain = 0;

////////////////////////////////
//~ rjf: Entities

MD_OS_LNX_Entity*
md_os_lnx_entity_alloc(MD_OS_LNX_EntityKind kind)
{
	MD_OS_LNX_Entity* entity = 0;
	md_defer_loop(pthread_mutex_lock  (&md_os_lnx_state.entity_mutex), pthread_mutex_unlock(&md_os_lnx_state.entity_mutex))
	{
		entity = md_os_lnx_state.entity_free;
		if (entity) {
			md_sll_stack_pop(md_os_lnx_state.entity_free);
		}
		else {
			entity = md_push_array__no_zero(md_os_lnx_state.entity_arena, MD_OS_LNX_Entity, 1);
		}
	}
	md_memory_zero_struct(entity);
	entity->kind = kind;
	return entity;
}

void
md_os_lnx_entity_release(MD_OS_LNX_Entity *entity) {
	md_defer_loop(pthread_mutex_lock(&md_os_lnx_state.entity_mutex), pthread_mutex_unlock(&md_os_lnx_state.entity_mutex))
	{
		md_sll_stack_push(md_os_lnx_state.entity_free, entity);
	}
}

////////////////////////////////
//~ rjf: Thread Entry Point

void*
md_os_lnx_thread_entry_point(void *ptr)
{
	MD_OS_LNX_Entity*         entity     = (MD_OS_LNX_Entity *)ptr;
	MD_OS_ThreadFunctionType* func       = entity->thread.func;
	void*                     thread_ptr = entity->thread.ptr;

	MD_TCTX md_tctx_;
	md_tctx_init_and_equip(&md_tctx_);
	func(thread_ptr);
	md_tctx_release();
	return 0;
}

////////////////////////////////
//~ rjf: @md_os_hooks System/Process Info (Implemented Per-OS)

MD_OS_SystemInfo*
md_os_get_system_info(void) {
	return &md_os_lnx_state.system_info;
}

MD_OS_ProcessInfo*
md_os_get_process_info(void) {
  return &md_os_lnx_state.process_info;
}

////////////////////////////////
//~ rjf: @md_os_hooks Thread Info (Implemented Per-OS)

void
md_os_set_thread_name(MD_String8 name)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	{
		MD_String8 name_copy      = md_push_str8_copy(scratch.arena, name);
		pthread_t  current_thread = pthread_self();
		pthread_setname_np(current_thread, (char *)name_copy.str);
	}
	scratch_end(scratch);
}

////////////////////////////////
//~ rjf: @md_os_hooks Aborting (Implemented Per-OS)

void md_os_abort(MD_S32 exit_code) { exit(exit_code); }

////////////////////////////////
//~ rjf: @md_os_hooks File System (Implemented Per-OS)

//- rjf: files

MD_OS_Handle
md_os_file_open(MD_OS_AccessFlags flags, MD_String8 path)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	{
		MD_String8 path_copy = md_push_str8_copy(scratch.arena, path);
		int        lnx_flags = 0;
		if      (flags & (MD_OS_AccessFlag_Read | MD_OS_AccessFlag_Write)) { lnx_flags  = O_RDWR; }
		else if (flags & MD_OS_AccessFlag_Write)                           { lnx_flags  = O_WRONLY; }
		else if (flags & MD_OS_AccessFlag_Read)                            { lnx_flags  = O_RDONLY; }
		if      (flags & MD_OS_AccessFlag_Append)                          { lnx_flags |= O_APPEND; }

		int          fd     = open((char *)path_copy.str, lnx_flags);
		MD_OS_Handle handle = {0};
		if (fd != -1) {
			handle.u64[0] = fd;
		}
	}
	scratch_end(scratch);
	return handle;
}

void
md_os_file_close(MD_OS_Handle file) {
	if (md_os_handle_match(file, md_os_handle_zero())) { return; }
	int fd = (int)file.u64[0];
	close(fd);
}

MD_U64
md_os_file_read(MD_OS_Handle file, MD_Rng1U64 rng, void* out_data)
{
	if (md_os_handle_match(file, md_os_handle_zero())) { return 0; }

	int fd = (int)file.u64[0];
	if (rng.md_min != 0) {
		lseek(fd, rng.md_min, SEEK_SET);
	}

	MD_U64 total_num_bytes_to_read      = md_dim_1u64(rng);
	MD_U64 total_num_bytes_read         = 0;
	MD_U64 total_num_bytes_left_to_read = total_num_bytes_to_read;
	for (;total_num_bytes_left_to_read > 0;)
{
		int read_result = read(fd, (MD_U8 *)out_data + total_num_bytes_read, total_num_bytes_left_to_read);
		if (read_result >= 0) {
			total_num_bytes_read         += read_result;
			total_num_bytes_left_to_read -= read_result;
		}
		else if (errno != EINTR) {
			break;
		}
	}
	return total_num_bytes_read;
}

MD_U64
md_os_file_write(MD_OS_Handle file, MD_Rng1U64 rng, void *data)
{
	if (md_os_handle_match(file, md_os_handle_zero())) { return 0; }
 
	int fd = (int)file.u64[0];
	if (rng.md_min != 0) {
		lseek(fd, rng.md_min, SEEK_SET);
	}

	MD_U64 total_num_bytes_to_write      = md_dim_1u64(rng);
	MD_U64 total_num_bytes_written       = 0;
	MD_U64 total_num_bytes_left_to_write = total_num_bytes_to_write;
	for (;total_num_bytes_left_to_write > 0;)
	{
		int write_result = write(fd, (MD_U8*)data + total_num_bytes_written, total_num_bytes_left_to_write);
		if (write_result >= 0) {
			total_num_bytes_written       += write_result;
			total_num_bytes_left_to_write -= write_result;
		}
		else if (errno != EINTR) {
			break;
		}
	}
	return total_num_bytes_written;
}

MD_B32
md_os_file_set_times(MD_OS_Handle file, MD_DateTime date_time)
{
	if (md_os_handle_match(file, md_os_handle_zero())) { return 0; }
	int fd = (int)file.u64[0];

	timespec time            = md_os_lnx_timespec_from_date_time(date_time);
	timespec times[2]        = {time, time};
	int      futimens_result = futimens(fd, times);
	MD_B32      good         = (futimens_result != -1);
	return good;
}

MD_FileProperties
md_os_properties_from_file(MD_OS_Handle file)
{
	if (md_os_handle_match(file, md_os_handle_zero())) { return (MD_FileProperties){0}; }

	int         fd           = (int)file.u64[0];
	struct stat fd_stat      = {0};
	int         fstat_result = fstat(fd, &fd_stat);
	MD_FileProperties props = {0};
	if (fstat_result != -1) {
		props = md_os_lnx_file_properties_from_stat(&fd_stat);
	}
	return props;
}

MD_OS_FileID
md_os_id_from_file(MD_OS_Handle file)
{
	if (md_os_handle_match(file, md_os_handle_zero())) { return (MD_OS_FileID){0}; }
	int         fd           = (int)file.u64[0];
	struct stat fd_stat      = {0};
	int         fstat_result = fstat(fd, &fd_stat);
	MD_OS_FileID id = {0};
	if (fstat_result != -1) {
		id.v[0] = fd_stat.st_dev;
		id.v[1] = fd_stat.st_ino;
	}
	return id;
}

MD_B32
md_os_delete_file_at_path(MD_String8 path)
{
	MD_TempArena scratch   = md_scratch_begin(0, 0);
	MD_B32       result    = 0;
	MD_String8   path_copy = md_push_str8_copy(scratch.arena, path);
	if (remove((char*)path_copy.str) != -1) {
		result = 1;
	}
	scratch_end(scratch);
	return result;
}

MD_B32
md_os_copy_file_path(MD_String8 dst, MD_String8 src)
{
	MD_B32 result = 0;
	MD_OS_Handle src_h = md_os_file_open(MD_OS_AccessFlag_Read, src);
	MD_OS_Handle dst_h = md_os_file_open(MD_OS_AccessFlag_Write, dst);
	if ( !md_os_handle_match(src_h, md_os_handle_zero()) &&
		 !md_os_handle_match(dst_h, md_os_handle_zero())   )
	{
		MD_FileProperties src_props = md_os_properties_from_file(src_h);

		MD_U64 size               = src_props.size;
		MD_U64 total_bytes_copied = 0;
		MD_U64 bytes_left_to_copy = size;
		for (;bytes_left_to_copy > 0;)
		{
			MD_TempArena scratch = md_scratch_begin(0, 0);

			MD_U64 buffer_size     = Min(bytes_left_to_copy, MD_MB(8));
			MD_U8 *buffer          = md_push_array__no_zero(scratch.arena, MD_U8, buffer_size);
			MD_U64 bytes_read      = md_os_file_read (src_h, md_r1u64(total_bytes_copied, total_bytes_copied+buffer_size), buffer);
			MD_U64 bytes_written   = md_os_file_write(dst_h, md_r1u64(total_bytes_copied, total_bytes_copied+bytes_read),  buffer);
			MD_U64 bytes_copied    = Min(bytes_read, bytes_written);
			bytes_left_to_copy -= bytes_copied;
			total_bytes_copied += bytes_copied;

			scratch_end(scratch);
			if(bytes_copied == 0) {
				break;
			}
		}
	}
	md_os_file_close(src_h);
	md_os_file_close(dst_h);
	return result;
}

MD_String8
md_os_full_path_from_path__ainfo(MD_AllocatorInfo ainfo, MD_String8 path)
{
	char buffer[PATH_MAX] = {0};
	MD_TempArena scratch = md_scratch_begin(ainfo); {
		MD_String8 path_copy = md_push_str8_copy(scratch.arena, path);
		realpath((char *)path_copy.str, buffer);
	}
	scratch_end(scratch);
	MD_String8 result = md_alloc_str8_copy(ainfo, md_str8_cstring(buffer));
	return  result;
}

MD_B32
md_os_file_path_exists(MD_String8 path)
{
	MD_B32 result = 0;
	MD_TempArena scratch       = md_scratch_begin(0, 0);
	MD_String8   path_copy     = md_push_str8_copy(scratch.arena, path);
	int       access_result = access((char *)path_copy.str, F_OK);
	if (access_result == 0) {
		result = 1;
	}
	scratch_end(scratch);
	return result;
}

md_internal MD_FileProperties
md_os_properties_from_file_path(MD_String8 path)
{
	MD_FileProperties props = {0};
	MD_TempArena scratch   = md_scratch_begin(0, 0);
	MD_String8   path_copy = md_push_str8_copy(scratch.arena, path);
	struct stat f_stat    = {0};
	int stat_result       = stat((char *)path_copy.str, &f_stat);
	if (stat_result != -1) {
		props = md_os_lnx_file_properties_from_stat(&f_stat);
	}
	scratch_end(scratch);
	return props;
}

//- rjf: file maps

MD_OS_Handle
md_os_file_map_open(MD_OS_AccessFlags flags, MD_OS_Handle file) {
	MD_OS_Handle map = file;
	return    map;
}

void
md_os_file_map_close(MD_OS_Handle map) {
	// NOTE(rjf): nothing to do; `map` handles are the same as `file` handles in
	// the linux implementation (on Windows they require separate handles)
}

void*
md_os_file_map_view_open(MD_OS_Handle map, MD_OS_AccessFlags flags, MD_Rng1U64 range)
{
	if (md_os_handle_match(map, md_os_handle_zero())) { return 0; }
	int prot_flags = 0;
	if (flags & MD_OS_AccessFlag_Write) { prot_flags |= PROT_WRITE; }
	if (flags & MD_OS_AccessFlag_Read)  { prot_flags |= PROT_READ; }
	int    fd        = (int)map.u64[0];
	int    map_flags = MAP_PRIVATE;
	void*  base      = mmap(0, md_dim_1u64(range), prot_flags, map_flags, fd, range.md_min);
	return base;
}

void
md_os_file_map_view_close(MD_OS_Handle map, void* ptr, MD_Rng1U64 range) {
	munmap(ptr, md_dim_1u64(range));
}

//- rjf: directory iteration

MD_OS_FileIter*
md_os_file_iter_begin__ainfo(MD_AllocatorInfo ainfo, MD_String8 path, MD_OS_FileIterFlags flags)
{
	MD_OS_FileIter* 
	base_iter        = md_alloc_array(ainfo, MD_OS_FileIter, 1);
	base_iter->flags = flags;

	MD_OS_LNX_FileIter* iter = (MD_OS_LNX_FileIter*)base_iter->memory;
	{
		MD_String8 path_copy = md_str8_copy(arena, path);
		iter->dir  = opendir((char*)path_copy.str);
		iter->path = path_copy;
	}
	return base_iter;
}

MD_B32
md_os_file_iter_next(MD_Arena* arena, MD_OS_FileIter* iter, MD_OS_FileInfo* info_out)
{
	MD_B32 good = 0;
	MD_OS_LNX_FileIter* lnx_iter = (MD_OS_LNX_FileIter*)iter->memory;
	for(;;)
	{
		// rjf: get next entry
		lnx_iter->dp = readdir(lnx_iter->dir);
		good         = (lnx_iter->dp != 0);
    
		// rjf: unpack entry info
		struct stat st = {0};
		int stat_result = 0;
		if(good)
		{
			MD_TempArena scratch     = md_scratch_begin(&arena, 1);
			MD_String8   full_path   = md_str8f(scratch.arena, "%S/%s", lnx_iter->path, lnx_iter->dp->d_name);
			          stat_result = stat((char *)full_path.str, &st);
			scratch_end(scratch);
		}
    
		// rjf: determine if filtered
		MD_B32 filtered = 0;
		if(good)
		{
			filtered = ((st.st_mode == S_IFDIR && iter->flags & MD_OS_FileIterFlag_SkipFolders) ||
						(st.st_mode == S_IFREG && iter->flags & MD_OS_FileIterFlag_SkipFiles) ||
						(lnx_iter->dp->d_name[0] == '.' && lnx_iter->dp->d_name[1] == 0) ||
						(lnx_iter->dp->d_name[0] == '.' && lnx_iter->dp->d_name[1] == '.' && lnx_iter->dp->d_name[2] == 0));
		}
    
		// rjf: output & exit, if good & unfiltered
		if (good && !filtered)
		{
			info_out->name = md_str8_copy(arena, md_str8_cstring(lnx_iter->dp->d_name));
			if (stat_result != -1) {
				info_out->props = md_os_lnx_file_properties_from_stat(&st);
			}
			break;
		}
    
		// rjf: exit if not good
		if (!good)
		{
			break;
		}
	}
	return good;
}

void
md_os_file_iter_end(MD_OS_FileIter* iter) {
	MD_OS_LNX_FileIter* lnx_iter = (MD_OS_LNX_FileIter*)iter->memory;
	closedir(lnx_iter->dir);
}

//- rjf: directory creation

MD_B32
md_os_make_directory(MD_String8 path)
{
	MD_B32 result = 0;
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String8 path_copy = md_push_str8_copy(scratch.arena, path);
	if (mkdir((char*)path_copy.str, 0777) != -1) {
		result = 1;
	}
	scratch_end(scratch);
	return result;
}

////////////////////////////////
//~ rjf: @md_os_hooks Shared Memory (Implemented Per-OS)

MD_OS_Handle
md_os_shared_memory_alloc(MD_U64 size, MD_String8 name)
{
	MD_TempArena scratch   = md_scratch_begin(0, 0);
	MD_String8   name_copy = md_push_str8_copy(scratch.arena, name);
	int          id        = shm_open((char *)name_copy.str, O_RDWR, 0);
	ftruncate(id, size);
	scratch_end(scratch);
	MD_OS_Handle result = {(MD_U64)id};
	return result;
}

MD_OS_Handle
md_os_shared_memory_open(MD_String8 name)
{
	MD_TempArena scratch   = md_scratch_begin(0, 0);
	MD_String8   name_copy = md_push_str8_copy(scratch.arena, name);
	int          id        = shm_open((char *)name_copy.str, O_RDWR, 0);
	scratch_end(scratch);
	MD_OS_Handle result = {(MD_U64)id};
	return result;
}

void
md_os_shared_memory_close(MD_OS_Handle handle) {
  if (md_os_handle_match(handle, md_os_handle_zero())) { return; }
  int id = (int)handle.u64[0];
  close(id);
}

void*
md_os_shared_memory_view_open(MD_OS_Handle handle, MD_Rng1U64 range) {
	if (md_os_handle_match(handle, md_os_handle_zero())) { return 0; }
	int    id   = (int)handle.u64[0];
	void*  base = mmap(0, md_dim_1u64(range), PROT_READ|PROT_WRITE, MAP_SHARED, id, range.md_min);
	return base;
}

void
md_os_shared_memory_view_close(MD_OS_Handle handle, void* ptr, MD_Rng1U64 range) {
	if (md_os_handle_match(handle, md_os_handle_zero())) { return; }
	munmap(ptr, md_dim_1u64(range));
}

////////////////////////////////
//~ rjf: @md_os_hooks Time (Implemented Per-OS)

MD_U64
md_os_now_microseconds(void) {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	MD_U64 result = t.tv_sec * md_million(1) + (t.tv_nsec / md_thousand(1));
	return result;
}

MD_U32
md_os_now_unix(void) {
	time_t t = time(0);
	return (MD_U32)t;
}

MD_DateTime
md_os_now_universal_time(void)
{
	time_t t = 0;
	time(&t);
	struct tm universal_tm = {0};
	gmtime_r(&t, &universal_tm);
	MD_DateTime result = md_os_lnx_date_time_from_tm(universal_tm, 0);
	return result;
}

MD_DateTime
md_os_universal_time_from_local(MD_DateTime* date_time)
{
	// rjf: local MD_DateTime -> universal time_t
	tm     local_tm          = md_os_lnx_tm_from_date_time(*date_time);
	       local_tm.tm_isdst = -1;
	time_t universal_t       = mktime(&local_tm);
  
	// rjf: universal time_t -> MD_DateTime
	tm universal_tm = {0};
	gmtime_r(&universal_t, &universal_tm);
	MD_DateTime result = md_os_lnx_date_time_from_tm(universal_tm, 0);
	return result;
}

MD_DateTime
md_os_local_time_from_universal(MD_DateTime* date_time)
{
	// rjf: universal MD_DateTime -> local time_t
	tm     universal_tm          = md_os_lnx_tm_from_date_time(*date_time);
	       universal_tm.tm_isdst = -1;
	time_t universal_t           = timegm(&universal_tm);
	tm     local_tm = {0};
	localtime_r(&universal_t, &local_tm);
	
	// rjf: local tm -> MD_DateTime
	MD_DateTime result = md_os_lnx_date_time_from_tm(local_tm, 0);
	return   result;
}

void
md_os_sleep_milliseconds(MD_U32 msec) {
	usleep(msec * md_thousand(1));
}

////////////////////////////////
//~ rjf: @md_os_hooks Child Processes (Implemented Per-OS)

MD_OS_Handle
md_os_process_launch(MD_OS_ProcessLaunchParams* params)
{
	NotImplemented;
}

MD_B32
md_os_process_join(MD_OS_Handle handle, MD_U64 endt_us)
{
	NotImplemented;
}

void
md_os_process_detach(MD_OS_Handle handle)
{
	NotImplemented;
}

////////////////////////////////
//~ rjf: @md_os_hooks Threads (Implemented Per-OS)

MD_OS_Handle
md_os_thread_launch(MD_OS_ThreadFunctionType* func, void* ptr, void* params)
{
	MD_OS_LNX_Entity* entity = md_os_lnx_entity_alloc(MD_OS_LNX_EntityKind_Thread);
	entity->thread.func = func;
	entity->thread.ptr  = ptr;
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);

		int pthread_result = pthread_create(&entity->thread.handle, &attr, md_os_lnx_thread_entry_point, entity);
		pthread_attr_destroy(&attr);
		if (pthread_result == -1)
		{
			md_os_lnx_entity_release(entity);
			entity = 0;
		}
	}
	MD_OS_Handle handle = {(MD_U64)entity};
	return handle;
}

MD_B32
md_os_thread_join(MD_OS_Handle handle, MD_U64 endt_us)
{
	if (md_os_handle_match(handle, md_os_handle_zero())) { return 0; }
	MD_OS_LNX_Entity* entity      = (MD_OS_LNX_Entity*)handle.u64[0];
	int               join_result = pthread_join(entity->thread.handle, 0);
	MD_B32            result      = (join_result == 0);
	md_os_lnx_entity_release(entity);
	return result;
}

void
md_os_thread_detach(MD_OS_Handle handle) {
	if(md_os_handle_match(handle, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity*)handle.u64[0];
	md_os_lnx_entity_release(entity);
}

////////////////////////////////
//~ rjf: @md_os_hooks Synchronization Primitives (Implemented Per-OS)

//- rjf: mutexes

MD_OS_Handle
md_os_mutex_alloc(void)
{
	MD_OS_LNX_Entity *entity = md_os_lnx_entity_alloc(MD_OS_LNX_EntityKind_Mutex);

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

	int init_result = pthread_mutex_init(&entity->mutex_handle, &attr);
	pthread_mutexattr_destroy(&attr);

	if (init_result == -1) {
		md_os_lnx_entity_release(entity);
		entity = 0;
	}
	MD_OS_Handle handle = {(MD_U64)entity};
	return    handle;
}

void
md_os_mutex_release(MD_OS_Handle mutex) {
	if (md_os_handle_match(mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity *)mutex.u64[0];
	pthread_mutex_destroy(&entity->mutex_handle);
	md_os_lnx_entity_release(entity);
}

void
md_os_mutex_take(MD_OS_Handle mutex) {
	if (md_os_handle_match(mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity *)mutex.u64[0];
	pthread_mutex_lock(&entity->mutex_handle);
}

void
md_os_mutex_drop(MD_OS_Handle mutex) {
	if (md_os_handle_match(mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity *)mutex.u64[0];
	pthread_mutex_unlock(&entity->mutex_handle);
}

//- rjf: reader/writer mutexes

MD_OS_Handle
md_os_rw_mutex_alloc(void)
{
	MD_OS_LNX_Entity* entity = md_os_lnx_entity_alloc(MD_OS_LNX_EntityKind_RWMutex);
	int init_result = pthread_rwlock_init(&entity->rwmutex_handle, 0);
	if (init_result == -1) {
		md_os_lnx_entity_release(entity);
		entity = 0;
	}
	MD_OS_Handle handle = {(MD_U64)entity};
	return    handle;
}

void
md_os_rw_mutex_release(MD_OS_Handle rw_mutex) {
	if (md_os_handle_match(rw_mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity*)rw_mutex.u64[0];
	pthread_rwlock_destroy(&entity->rwmutex_handle);
	md_os_lnx_entity_release(entity);
}

void
md_os_rw_mutex_take_r(MD_OS_Handle rw_mutex) {
	if (md_os_handle_match(rw_mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity*)rw_mutex.u64[0];
	pthread_rwlock_rdlock(&entity->rwmutex_handle);
}

void
md_os_rw_mutex_drop_r(MD_OS_Handle rw_mutex) {
	if (md_os_handle_match(rw_mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity*)rw_mutex.u64[0];
	pthread_rwlock_unlock(&entity->rwmutex_handle);
}

void
md_os_rw_mutex_take_w(MD_OS_Handle rw_mutex) {
	if (md_os_handle_match(rw_mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity*)rw_mutex.u64[0];
	pthread_rwlock_wrlock(&entity->rwmutex_handle);
}

void
md_os_rw_mutex_drop_w(MD_OS_Handle rw_mutex) {
	if (md_os_handle_match(rw_mutex, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity*)rw_mutex.u64[0];
	pthread_rwlock_unlock(&entity->rwmutex_handle);
}

//- rjf: condition variables

MD_OS_Handle
md_os_condition_variable_alloc(void)
{
	MD_OS_LNX_Entity* entity = md_os_lnx_entity_alloc(MD_OS_LNX_EntityKind_ConditionVariable);
	int init_result = pthread_cond_init(&entity->cv.cond_handle, 0);
	if (init_result == -1) {
		md_os_lnx_entity_release(entity);
		entity = 0;
	}
	int init2_result = 0;
	if (entity) {
		init2_result = pthread_mutex_init(&entity->cv.rwlock_mutex_handle, 0);
	}
	if (init2_result == -1) {
		pthread_cond_destroy(&entity->cv.cond_handle);
		md_os_lnx_entity_release(entity);
		entity = 0;
	}
	MD_OS_Handle handle = {(MD_U64)entity};
	return handle;
}

void
md_os_condition_variable_release(MD_OS_Handle cv) {
	if (md_os_handle_match(cv, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* entity = (MD_OS_LNX_Entity *)cv.u64[0];
	pthread_cond_destroy(&entity->cv.cond_handle);
	pthread_mutex_destroy(&entity->cv.rwlock_mutex_handle);
	md_os_lnx_entity_release(entity);
}

MD_B32
md_os_condition_variable_wait(MD_OS_Handle cv, MD_OS_Handle mutex, MD_U64 endt_us)
{
	if (md_os_handle_match(cv,    md_os_handle_zero())) { return 0; }
	if (md_os_handle_match(mutex, md_os_handle_zero())) { return 0; }

	MD_OS_LNX_Entity* cv_entity    = (MD_OS_LNX_Entity*)cv.u64[0];
	MD_OS_LNX_Entity* mutex_entity = (MD_OS_LNX_Entity*)mutex.u64[0];

	struct timespec endt_timespec;
	endt_timespec.tv_sec  = endt_us / md_million(1);
	endt_timespec.tv_nsec = md_thousand(1) * (endt_us - (endt_us / md_million(1)) * md_million(1));

	int    wait_result = pthread_cond_timedwait(&cv_entity->cv.cond_handle, &mutex_entity->mutex_handle, &endt_timespec);
	MD_B32 result      = (wait_result != ETIMEDOUT);
	return result;
}

MD_B32
md_os_condition_variable_wait_rw_r(MD_OS_Handle cv, MD_OS_Handle mutex_rw, MD_U64 endt_us)
{
	// TODO(rjf): because pthread does not supply cv/rw natively, I had to hack
	// this together, but this would probably just be a lot better if we just
	// implemented the primitives ourselves with e.g. futexes
	//
	if (md_os_handle_match(cv, md_os_handle_zero()))       { return 0; }
	if (md_os_handle_match(mutex_rw, md_os_handle_zero())) { return 0; }

	MD_OS_LNX_Entity* cv_entity       = (MD_OS_LNX_Entity*)cv.u64[0];
	MD_OS_LNX_Entity* rw_mutex_entity = (MD_OS_LNX_Entity*)mutex_rw.u64[0];

	struct timespec endt_timespec;
	endt_timespec.tv_sec  = endt_us / md_million(1);
	endt_timespec.tv_nsec = md_thousand(1) * (endt_us - (endt_us / md_million(1)) * md_million(1));

	MD_B32 result = 0;
	for(;;)
	{
		pthread_mutex_lock(&cv_entity->cv.rwlock_mutex_handle);
		int wait_result = pthread_cond_timedwait(&cv_entity->cv.cond_handle, &cv_entity->cv.rwlock_mutex_handle, &endt_timespec);
		if (wait_result != ETIMEDOUT)
		{
			pthread_rwlock_rdlock(&rw_mutex_entity->rwmutex_handle);
			pthread_mutex_unlock(&cv_entity->cv.rwlock_mutex_handle);
			result = 1;
			break;
		}

		pthread_mutex_unlock(&cv_entity->cv.rwlock_mutex_handle);
		if (wait_result == ETIMEDOUT)
		{
			break;
		}
	}
	return result;
}

MD_B32
md_os_condition_variable_wait_rw_w(MD_OS_Handle cv, MD_OS_Handle mutex_rw, MD_U64 endt_us)
{
	// TODO(rjf): because pthread does not supply cv/rw natively, I had to hack
	// this together, but this would probably just be a lot better if we just
	// implemented the primitives ourselves with e.g. futexes
	//
	if (md_os_handle_match(cv,       md_os_handle_zero())) { return 0; }
	if (md_os_handle_match(mutex_rw, md_os_handle_zero())) { return 0; }

	MD_OS_LNX_Entity* cv_entity       = (MD_OS_LNX_Entity*)cv.u64[0];
	MD_OS_LNX_Entity* rw_mutex_entity = (MD_OS_LNX_Entity*)mutex_rw.u64[0];

	struct timespec endt_timespec;
	endt_timespec.tv_sec  = endt_us / md_million(1);
	endt_timespec.tv_nsec = md_thousand(1) * (endt_us - (endt_us / md_million(1)) * md_million(1));

	MD_B32 result = 0;
	for(;;)
	{
		pthread_mutex_lock(&cv_entity->cv.rwlock_mutex_handle);
		int wait_result = pthread_cond_timedwait(&cv_entity->cv.cond_handle, &cv_entity->cv.rwlock_mutex_handle, &endt_timespec);
		if (wait_result != ETIMEDOUT)
		{
			pthread_rwlock_wrlock(&rw_mutex_entity->rwmutex_handle);
			pthread_mutex_unlock(&cv_entity->cv.rwlock_mutex_handle);
			result = 1;
			break;
		}

		pthread_mutex_unlock(&cv_entity->cv.rwlock_mutex_handle);
		if (wait_result == ETIMEDOUT)
		{
			break;
		}
	}
	return result;
}

void
md_os_condition_variable_signal(MD_OS_Handle cv) {
	if (md_os_handle_match(cv, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* cv_entity = (MD_OS_LNX_Entity *)cv.u64[0];
	pthread_cond_signal(&cv_entity->cv.cond_handle);
}

void
md_os_condition_variable_broadcast(MD_OS_Handle cv) {
	if (md_os_handle_match(cv, md_os_handle_zero())) { return; }
	MD_OS_LNX_Entity* cv_entity = (MD_OS_LNX_Entity *)cv.u64[0];
	pthread_cond_broadcast(&cv_entity->cv.cond_handle);
}

//- rjf: cross-process semaphores

MD_OS_Handle
md_os_semaphore_alloc(MD_U32 initial_count, MD_U32 max_count, MD_String8 name)
{
	NotImplemented;
}

void
md_os_semaphore_release(MD_OS_Handle semaphore)
{
	NotImplemented;
}

MD_OS_Handle
md_os_semaphore_open(MD_String8 name)
{
	NotImplemented;
}

void
md_os_semaphore_close(MD_OS_Handle semaphore)
{
	NotImplemented;
}

MD_B32
md_os_semaphore_take(MD_OS_Handle semaphore, MD_U64 endt_us)
{
	NotImplemented;
}

void
md_os_semaphore_drop(MD_OS_Handle semaphore)
{
	NotImplemented;
}

////////////////////////////////
//~ rjf: @md_os_hooks Dynamically-Loaded Libraries (Implemented Per-OS)

MD_OS_Handle
md_os_library_open(MD_String8 path) {
	MD_TempArena scratch = md_scratch_begin(0, 0);
	char*        path_cstr = (char *)md_push_str8_copy(scratch.arena, path).str;
	void*        so        = dlopen(path_cstr, RTLD_LAZY);
	MD_OS_Handle lib       = { (MD_U64)so };
	scratch_end(scratch);
	return lib;
}

MD_VoidProc*
md_os_library_load_proc(MD_OS_Handle lib, MD_String8 name) {
	MD_TempArena scratch = md_scratch_begin(0, 0);
	void*        so        = (void*)lib.u64;
	char*        name_cstr = (char*)md_push_str8_copy(scratch.arena, name).str;
	MD_VoidProc* proc      = (MD_VoidProc*)dlsym(so, name_cstr);
	scratch_end(scratch);
	return proc;
}

void
md_os_library_close(MD_OS_Handle lib) {
	void* so = (void*)lib.u64;
	dlclose(so);
}

////////////////////////////////
//~ rjf: @md_os_hooks Safe Calls (Implemented Per-OS)

void
md_os_safe_call(MD_OS_ThreadFunctionType* func, MD_OS_ThreadFunctionType* fail_handler, void* ptr)
{
	// rjf: push handler to chain
	MD_OS_LNX_SafeCallChain chain = {0};
	sll_stack_push(md_os_lnx_safe_call_chain, &chain);
	chain.fail_handler = fail_handler;
	chain.ptr          = ptr;
  
	// rjf: set up sig handler info
	struct sigaction new_act = {0};
	new_act.sa_handler       = md_os_lnx_safe_call_sig_handler;
	int signals_to_handle[]  = {
		SIGILL, SIGFPE, SIGSEGV, SIGBUS, SIGTRAP,
	};
	struct sigaction og_act[md_array_count(signals_to_handle)] = {0};
  
	// rjf: attach handler info for all signals
	for(MD_U32 i = 0; i < md_array_count(signals_to_handle); i += 1) {
		sigaction(signals_to_handle[i], &new_act, &og_act[i]);
	}
  
	// rjf: call function
	func(ptr);
  
	// rjf: reset handler info for all signals
	for (MD_U32 i = 0; i < md_array_count(signals_to_handle); i += 1) {
		sigaction(signals_to_handle[i], &og_act[i], 0);
	}
}

////////////////////////////////
//~ rjf: @md_os_hooks GUIDs (Implemented Per-OS)

MD_OS_Guid
md_os_make_guid(void)
{
	MD_U8 random_bytes[16] = {0};
	md_static_assert(sizeof(random_bytes) == sizeof(MD_OS_Guid), md_os_lnx_guid_size_check);
	getrandom(random_bytes, sizeof(random_bytes), 0);

	MD_OS_Guid guid = {0};
	md_memory_copy(&guid, random_bytes, sizeof(random_bytes));
	guid.data3    &= 0x0fff;
	guid.data3    |= (4 << 12);
	guid.data4[0] &= 0x3f;
	guid.data4[0] |= 0x80;
	return guid;
}

////////////////////////////////
//~ rjf: @md_os_hooks Entry Points (Implemented Per-OS)

#if MD_BUILD_ENTRY_DEFINING_UNIT || 1

int
main(int argc, char **argv)
{
	//- rjf: set up OS layer
	{
		//- rjf: get statically-allocated system/process info
		{
			MD_OS_SystemInfo *info = &md_os_lnx_state.system_info;
			info->logical_processor_count = (MD_U32)get_nprocs();
			info->page_size               = (MD_U64)getpagesize();
			info->large_page_size         = MD_MB(2);
			info->allocation_granularity  = info->page_size;
		}
		{
			MD_OS_ProcessInfo *info = &md_os_lnx_state.process_info;
			info->pid = (MD_U32)getpid();
		}
    
		//- rjf: set up thread context
		md_local_persist MD_TCTX md_tctx;
		md_tctx_init_and_equip(&md_tctx);
    
		//- rjf: set up dynamically allocated state
		md_os_lnx_state.arena        = md_arena_alloc();
		md_os_lnx_state.entity_arena = md_arena_alloc();
		pthread_mutex_init(&md_os_lnx_state.entity_mutex, 0);
    
		//- rjf: grab dynamically allocated system info
		{
			MD_TempArena      scratch = md_scratch_begin(0, 0);
			MD_OS_SystemInfo* info    = &md_os_lnx_state.system_info;
      
			// rjf: get machine name
			MD_B32 got_final_result = 0;
			MD_U8* buffer           = 0;
			int size                = 0;
			for (MD_S64 cap = 4096, r = 0; r < 4; cap *= 2, r += 1)
			{
				scratch_end(scratch);
				buffer = md_push_array__no_zero(scratch.arena, MD_U8, cap);
				size   = gethostname((char*)buffer, cap);
				if (size < cap)
				{
					got_final_result = 1;
					break;
				}
			}
      
			// rjf: save name to info
			if (got_final_result && size > 0)
			{
				info->machine_name.size = size;
				info->machine_name.str  = md_push_array__no_zero(md_os_lnx_state.arena, MD_U8, info->machine_name.size + 1);
				memory_copy(info->machine_name.str, buffer, info->machine_name.size);
				info->machine_name.str[info->machine_name.size] = 0;
			}
      
			scratch_end(scratch);
		}
    
		//- rjf: grab dynamically allocated process info
		{
			MD_TempArena       scratch = md_scratch_begin(0, 0);
			MD_OS_ProcessInfo* info    = &md_os_lnx_state.process_info;
      
			// rjf: grab binary path
			{
				// rjf: get self string
				MD_B32 got_final_result = 0;
				MD_U8* buffer           = 0;
				int size             = 0;
				for (MD_S64 cap = PATH_MAX, r = 0; r < 4; cap *= 2, r += 1)
				{
					scratch_end(scratch);
					buffer = md_push_array__no_zero(scratch.arena, MD_U8, cap);
					size   = readlink("/proc/self/exe", (char*)buffer, cap);
					if (size < cap)
					{
						got_final_result = 1;
						break;
					}
				}
				
				// rjf: save
				if (got_final_result && size > 0)
				{
					MD_String8 full_name    = md_str8(buffer, size);
					MD_String8 name_chopped = md_str8_chop_last_slash(full_name);
					info->binary_path       = md_push_str8_copy(md_os_lnx_state.arena, name_chopped);
				}
			}
      
			// rjf: grab initial directory
			{
				info->initial_path = md_os_get_current_path(md_os_lnx_state.arena);
			}
      
			// rjf: grab home directory
			{
				char *home = getenv("HOME");
				info->user_program_data_path = md_str8_cstring(home);
			}
      
			scratch_end(scratch);
		}
	}
  
	//- rjf: call into "real" entry point
	md_main_thread_base_entry_point(md_entry_point, argv, (MD_U64)argc);
}

// MD_BUILD_ENTRY_POINT
#endif
