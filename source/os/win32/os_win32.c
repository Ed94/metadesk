
// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

MD_API md_global MD_OS_W32_State md_os_w32_state = {0};

////////////////////////////////
//~ rjf: Modern Windows SDK Functions
//
// (We must dynamically link to them, since they can be missing in older SDKs)

typedef HRESULT W32_SetThreadDescription_Type(HANDLE hThread, PCWSTR lpThreadDescription);
MD_API md_global W32_SetThreadDescription_Type* w32_SetThreadDescription_func = 0;

////////////////////////////////
//~ rjf: Time Conversion Helpers

MD_U32
md_os_w32_sleep_ms_from_endt_us(MD_U64 endt_us)
{
	MD_U32 sleep_ms = 0;
	if (endt_us == MD_MAX_U64) {
		sleep_ms = INFINITE;
	}
	else
	{
		MD_U64 begint = md_os_now_microseconds();
		if (begint < endt_us) {
			MD_U64 sleep_us = endt_us - begint;
			    sleep_ms = (MD_U32)((sleep_us + 999)/1000);
		}
	}
	return sleep_ms;
}

////////////////////////////////
//~ rjf: Entity Functions

MD_OS_W32_Entity*
md_os_w32_entity_alloc(MD_OS_W32_EntityKind kind)
{
	MD_OS_W32_Entity *result = 0;
	EnterCriticalSection(&md_os_w32_state.entity_mutex);
	{
		result = md_os_w32_state.entity_free;
		if(result)
		{
			md_sll_stack_pop(md_os_w32_state.entity_free);
		}
		else
		{
			result = md_push_array__no_zero(md_os_w32_state.entity_arena, MD_OS_W32_Entity, 1);
		}
		md_memory_zero_struct(result);
	}
	LeaveCriticalSection(&md_os_w32_state.entity_mutex);
	result->kind = kind;
	return result;
}

void
md_os_w32_entity_release(MD_OS_W32_Entity *entity)
{
  entity->kind = MD_OS_W32_EntityKind_Null;
  EnterCriticalSection(&md_os_w32_state.entity_mutex);
  md_sll_stack_push(md_os_w32_state.entity_free, entity);
  LeaveCriticalSection(&md_os_w32_state.entity_mutex);
}

////////////////////////////////
//~ rjf: Thread Entry Point

DWORD
md_os_w32_thread_entry_point(void *ptr)
{
	MD_OS_W32_Entity*         entity     = (MD_OS_W32_Entity*)ptr;
	MD_OS_ThreadFunctionType* func       = entity->thread.func;
	void*                  thread_ptr = entity->thread.ptr;

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
	return &md_os_w32_state.system_info;
}

MD_OS_ProcessInfo*
md_os_get_process_info(void) {
  return &md_os_w32_state.process_info;
}

////////////////////////////////
//~ rjf: @md_os_hooks Thread Info (Implemented Per-OS)

void
md_os_set_thread_name(MD_String8 name)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
  
	// rjf: windows 10 style
	if (w32_SetThreadDescription_func)
	{
		MD_String16 name16 = md_str16_from(scratch.arena, name);
		HRESULT hr = w32_SetThreadDescription_func(GetCurrentThread(), (WCHAR*)name16.str);
	}
  
	// rjf: raise-exception style
	{
		MD_String8 name_copy = md_str8_copy(scratch.arena, name);

	#pragma pack(push,8)
		typedef struct THREADNAME_INFO THREADNAME_INFO;
		struct THREADNAME_INFO
		{
			MD_U32   dwType;     // Must be 0x1000.
			char* szName;   // Pointer to name (in user addr space).
			MD_U32   dwThreadID; // Thread ID (-1=caller thread).
			MD_U32   dwFlags;    // Reserved for future use, must be zero.
		};
	#pragma pack(pop)

		THREADNAME_INFO info;
		info.dwType     = 0x1000;
		info.szName     = (char *)name_copy.str;
		info.dwThreadID = md_os_tid();
		info.dwFlags    = 0;

	#pragma warning(push)
	#pragma warning(disable: 6320 6322)
		__try
		{
			RaiseException(0x406D1388, 0, sizeof(info) / sizeof(void *), (const ULONG_PTR *)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	#pragma warning(pop)
	}
  
	scratch_end(scratch);
}

////////////////////////////////
//~ rjf: @md_os_hooks Aborting (Implemented Per-OS)

void md_os_abort(MD_S32 exit_code) { ExitProcess(exit_code); }

////////////////////////////////
//~ rjf: @md_os_hooks File System (Implemented Per-OS)

//- rjf: files

MD_OS_Handle
md_os_file_open(MD_OS_AccessFlags flags, MD_String8 path)
{
	MD_OS_Handle result  = {0};
	MD_TempArena scratch = md_scratch_begin(0, 0);
	{
		MD_String16  path16 = md_str16_from(scratch.arena, path);

		DWORD access_flags         = 0;
		DWORD share_mode           = 0;
		DWORD creation_disposition = OPEN_EXISTING;

		if (flags & MD_OS_AccessFlag_Read)       { access_flags        |= GENERIC_READ; }
		if (flags & MD_OS_AccessFlag_Write)      { access_flags        |= GENERIC_WRITE; }
		if (flags & MD_OS_AccessFlag_Execute)    { access_flags        |= GENERIC_EXECUTE; }
		if (flags & MD_OS_AccessFlag_ShareRead)  { share_mode          |= FILE_SHARE_READ; }
		if (flags & MD_OS_AccessFlag_ShareWrite) { share_mode          |= FILE_SHARE_WRITE | FILE_SHARE_DELETE; }
		if (flags & MD_OS_AccessFlag_Write)      { creation_disposition = CREATE_ALWAYS; }
		if (flags & MD_OS_AccessFlag_Append)     { creation_disposition = OPEN_ALWAYS; }

		HANDLE file = CreateFileW((WCHAR*)path16.str, access_flags, share_mode, 0, creation_disposition, FILE_ATTRIBUTE_NORMAL, 0);
		if (file != INVALID_HANDLE_VALUE) {
			result.u64[0] = (MD_U64)file;
		}
	}
	scratch_end(scratch);
	return result;
}

void
md_os_file_close(MD_OS_Handle file) {
	if (md_os_handle_match(file, md_os_handle_zero())) { return; }
	HANDLE handle = (HANDLE)file.u64[0];
	BOOL   result = CloseHandle(handle);
	(void)result;
}

MD_U64
md_os_file_read(MD_OS_Handle file, MD_Rng1U64 rng, void *out_data)
{
	if (md_os_handle_match(file, md_os_handle_zero())) { return 0; }
	HANDLE handle = (HANDLE)file.u64[0];
  
	// rjf: md_clamp range by file size
	MD_U64 size = 0;
	GetFileSizeEx(handle, (LARGE_INTEGER *)&size);
	MD_Rng1U64 rng_clamped     = md_r1u64(md_clamp_top(rng.md_min, size), md_clamp_top(rng.md_max, size));
	MD_U64     total_read_size = 0;
  
	// rjf: read loop
	{
		MD_U64 to_read = md_dim_1u64(rng_clamped);
		for(MD_U64 off = rng.md_min; total_read_size < to_read;)
		{
			MD_U64        amt64      = to_read - total_read_size;
			MD_U32        amt32      = md_u32_from_u64_saturate(amt64);
			DWORD      read_size  = 0;

			OVERLAPPED overlapped = {0};
			overlapped.Offset     = (off&0x00000000ffffffffull);
			overlapped.OffsetHigh = (off&0xffffffff00000000ull) >> 32;

			ReadFile(handle, (MD_U8 *)out_data + total_read_size, amt32, &read_size, &overlapped);

			off             += read_size;
			total_read_size += read_size;
			if (read_size != amt32) {
				break;
			}
		}
	}
  
	return total_read_size;
}

MD_U64
md_os_file_write(MD_OS_Handle file, MD_Rng1U64 rng, void *data)
{
	if(md_os_handle_match(file, md_os_handle_zero())) { return 0; }
	HANDLE win_handle = (HANDLE)file.u64[0];

	MD_U64 src_off              = 0;
	MD_U64 dst_off              = rng.md_min;
	MD_U64 bytes_to_write_total = rng.md_max-rng.md_min;
	MD_U64 total_bytes_written  = 0;
	for  (;src_off < bytes_to_write_total;)
	{
		void* bytes_src         = (void*)((MD_U8*)data + src_off);
		MD_U64   bytes_to_write_64 = (bytes_to_write_total-src_off);
		MD_U32   bytes_to_write_32 = md_u32_from_u64_saturate(bytes_to_write_64);
		MD_U32   bytes_written     = 0;

		OVERLAPPED overlapped = {0};
		overlapped.Offset     = (dst_off&0x00000000ffffffffull);
		overlapped.OffsetHigh = (dst_off&0xffffffff00000000ull) >> 32;

		BOOL success = WriteFile(win_handle, bytes_src, bytes_to_write_32, (DWORD *)&bytes_written, &overlapped);
		if (success == 0) {
			break;
		}
		src_off             += bytes_written;
		dst_off             += bytes_written;
		total_bytes_written += bytes_written;
	}

	return total_bytes_written;
}

MD_B32
md_os_file_set_times(MD_OS_Handle file, MD_DateTime time)
{
	if(md_os_handle_match(file, md_os_handle_zero())) { return 0; }
	MD_B32 result = 0;
	{
		HANDLE     handle      = (HANDLE)file.u64[0];
		SYSTEMTIME system_time = {0};
		md_os_w32_system_time_from_date_time(&system_time, &time);

		FILETIME file_time = {0};
		result = (SystemTimeToFileTime(&system_time, &file_time) && SetFileTime(handle, &file_time, &file_time, &file_time));
	}
	return result;
}

MD_FileProperties
md_os_properties_from_file(MD_OS_Handle file)
{
	if (md_os_handle_match(file, md_os_handle_zero())) { MD_FileProperties r = {0}; return r; }
	MD_FileProperties props = {0};

	HANDLE                     handle = (HANDLE)file.u64[0];
	BY_HANDLE_FILE_INFORMATION info;
	BOOL info_good = GetFileInformationByHandle(handle, &info);
	if (info_good)
	{
		MD_U32 size_lo = info.nFileSizeLow;
		MD_U32 size_hi = info.nFileSizeHigh;
		props.size  = (MD_U64)size_lo | (((MD_U64)size_hi)<<32);
		md_os_w32_dense_time_from_file_time(&props.modified, &info.ftLastWriteTime);
		md_os_w32_dense_time_from_file_time(&props.created, &info.ftCreationTime);
		props.flags = md_os_w32_file_property_flags_from_dwFileAttributes(info.dwFileAttributes);
	}
	return props;
}

MD_OS_FileID
md_os_id_from_file(MD_OS_Handle file)
{
  if(md_os_handle_match(file, md_os_handle_zero())) { MD_OS_FileID r = {0}; return r; }
  MD_OS_FileID result = {0};

	HANDLE                     handle = (HANDLE)file.u64[0];
	BY_HANDLE_FILE_INFORMATION info;
	BOOL is_ok = GetFileInformationByHandle(handle, &info);
	if (is_ok)
	{
		result.v[0] = info.dwVolumeSerialNumber;
		result.v[1] = info.nFileIndexLow;
		result.v[2] = info.nFileIndexHigh;
	}
	return result;
}

MD_B32
md_os_delete_file_at_path(MD_String8 path)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String16  path16  = md_str16_from(scratch.arena, path);
	MD_B32       result  = DeleteFileW((WCHAR*)path16.str);
	scratch_end(scratch);
	return result;
}

MD_B32
md_os_copy_file_path(MD_String8 dst, MD_String8 src)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String16  dst16   = md_str16_from(scratch.arena, dst);
	MD_String16  src16   = md_str16_from(scratch.arena, src);
	MD_B32 result = CopyFileW((WCHAR*)src16.str, (WCHAR*)dst16.str, 0);
	scratch_end(scratch);
	return result;
}

MD_String8
md_os_full_path_from_path__ainfo(MD_AllocatorInfo ainfo, MD_String8 path)
{
	MD_TempArena scratch = md_scratch_begin(ainfo);
	DWORD     buffer_size = MAX_PATH + 1;
	MD_U16*      buffer      = md_push_array__no_zero(scratch.arena, MD_U16, buffer_size);
	MD_String16  path16      = md_str16_from(scratch.arena, path);
	DWORD     path16_size = GetFullPathNameW((WCHAR*)path16.str, buffer_size, (WCHAR*)buffer, NULL);
	MD_String8   full_path   = md_str8_from(ainfo, md_str16(buffer, path16_size));
	scratch_end(scratch);
	return full_path;
}

MD_B32
md_os_file_path_exists(MD_String8 path)
{
	MD_TempArena scratch    = md_scratch_begin(0,0);
	MD_String16  path16     = md_str16_from(scratch.arena, path);
	DWORD     attributes = GetFileAttributesW((WCHAR *)path16.str);
	MD_B32       exists     = (attributes != INVALID_FILE_ATTRIBUTES) && !!(~attributes & FILE_ATTRIBUTE_DIRECTORY);
	scratch_end(scratch);
	return exists;
}

MD_FileProperties
md_os_properties_from_file_path(MD_String8 path)
{
	MD_FileProperties props = {0};
	WIN32_FIND_DATAW find_data = {0};
	MD_TempArena        scratch   = md_scratch_begin(0, 0);
	MD_String16         path16    = md_str16_from(scratch.arena, path);
	HANDLE           handle    = FindFirstFileW((WCHAR *)path16.str, &find_data);
	if (handle != INVALID_HANDLE_VALUE)
	{
		props.size = md_compose_64bit(find_data.nFileSizeHigh, find_data.nFileSizeLow);
		md_os_w32_dense_time_from_file_time(&props.created, &find_data.ftCreationTime);
		md_os_w32_dense_time_from_file_time(&props.modified, &find_data.ftLastWriteTime);
		props.flags = md_os_w32_file_property_flags_from_dwFileAttributes(find_data.dwFileAttributes);
	}
	FindClose(handle);
	scratch_end(scratch);
	return props;
}

//- rjf: file maps

MD_OS_Handle
md_os_file_map_open(MD_OS_AccessFlags flags, MD_OS_Handle file)
{
	MD_OS_Handle map = {0};
	{
		HANDLE file_handle = (HANDLE)file.u64[0];
		DWORD protect_flags = 0;
		{
			switch(flags)
			{
				default: {} break;

				case MD_OS_AccessFlag_Read:
				{ protect_flags = PAGE_READONLY; }
				break;

				case MD_OS_AccessFlag_Write:
				case MD_OS_AccessFlag_Read|MD_OS_AccessFlag_Write:
				{ protect_flags = PAGE_READWRITE; }
				break;

				case MD_OS_AccessFlag_Execute:
				case MD_OS_AccessFlag_Read|MD_OS_AccessFlag_Execute:
				{ protect_flags = PAGE_EXECUTE_READ; }
				break;

				case MD_OS_AccessFlag_Execute|MD_OS_AccessFlag_Write|MD_OS_AccessFlag_Read:
				case MD_OS_AccessFlag_Execute|MD_OS_AccessFlag_Write:
				{ protect_flags = PAGE_EXECUTE_READWRITE; }
				break;
			}
		}
		HANDLE map_handle = CreateFileMappingA(file_handle, 0, protect_flags, 0, 0, 0);
		map.u64[0] = (MD_U64)map_handle;
	}
	return map;
}

void
md_os_file_map_close(MD_OS_Handle map)
{
	HANDLE handle = (HANDLE)map.u64[0];
	BOOL   result = CloseHandle(handle);
	(void)result;
}

void*
md_os_file_map_view_open(MD_OS_Handle map, MD_OS_AccessFlags flags, MD_Rng1U64 range)
{
	HANDLE handle       = (HANDLE)map.u64[0];
	MD_U32    off_lo       = (MD_U32)((range.md_min&0x00000000ffffffffull)>>0);
	MD_U32    off_hi       = (MD_U32)((range.md_min&0xffffffff00000000ull)>>32);
	MD_U64    size         = md_dim_1u64(range);
	DWORD  access_flags = 0;
	{
		switch(flags)
		{
			default:{}break;
			case MD_OS_AccessFlag_Read:                       { access_flags = FILE_MAP_READ; }       break;
			case MD_OS_AccessFlag_Write:                      { access_flags = FILE_MAP_WRITE; }      break; 
			case MD_OS_AccessFlag_Read | MD_OS_AccessFlag_Write: { access_flags = FILE_MAP_ALL_ACCESS; } break; 

			case MD_OS_AccessFlag_Execute:
			case MD_OS_AccessFlag_Read | MD_OS_AccessFlag_Execute:
			case MD_OS_AccessFlag_Write| MD_OS_AccessFlag_Execute:
			case MD_OS_AccessFlag_Read | MD_OS_AccessFlag_Write | MD_OS_AccessFlag_Execute:
			{
				access_flags = FILE_MAP_ALL_ACCESS|FILE_MAP_EXECUTE;
			}
			break;
		}
	}
	void* result = MapViewOfFile(handle, access_flags, off_hi, off_lo, size);
	return result;
}

void
md_os_file_map_view_close(MD_OS_Handle map, void* ptr, MD_Rng1U64 range) {
	BOOL result = UnmapViewOfFile(ptr);
	(void)result;
}

//- rjf: directory iteration

MD_OS_FileIter*
md_os_file_iter_begin__ainfo(MD_AllocatorInfo ainfo, MD_String8 path, MD_OS_FileIterFlags flags)
{
	MD_TempArena scratch = md_scratch_begin(ainfo);

	MD_String8   path_with_wildcard = md_str8_cat(scratch.arena, path, md_str8_lit("\\*"));
	MD_String16  path16             = md_str16_from(scratch.arena, path_with_wildcard);

	MD_OS_FileIter* 
	iter        = md_alloc_array(ainfo, MD_OS_FileIter, 1); 
	iter->flags = flags;

	MD_OS_W32_FileIter* w32_iter = (MD_OS_W32_FileIter*)iter->memory;
	if (path.size == 0)
	{
		w32_iter->is_volume_iter = 1;

		WCHAR buffer[512] = {0};
		DWORD length      = GetLogicalDriveStringsW(sizeof(buffer), buffer);

		MD_String8List drive_strings = {0};
		for(MD_U64 off = 0; off < (MD_U64)length;)
		{
			MD_String16 next_drive_string_16  = md_str16_cstring((MD_U16*)buffer + off);
			         off                  += next_drive_string_16.size + 1;
			MD_String8  next_drive_string     = md_str8_from(ainfo, next_drive_string_16);
			         next_drive_string     = md_str8_chop_last_slash(next_drive_string);
			md_str8_list_push(scratch.arena, &drive_strings, next_drive_string);
		}
		w32_iter->drive_strings          = md_str8_array_from_list(ainfo, &drive_strings);
		w32_iter->drive_strings_iter_idx = 0;
	}
	else
	{
		w32_iter->handle = FindFirstFileW((WCHAR*)path16.str, &w32_iter->find_data);
	}
	scratch_end(scratch);
	return iter;
}

MD_B32
md_os_file_iter_next__ainfo(MD_AllocatorInfo ainfo, MD_OS_FileIter* iter, MD_OS_FileInfo* info_out)
{
	MD_B32 result = 0;

	MD_OS_FileIterFlags flags    = iter->flags;
	MD_OS_W32_FileIter* w32_iter = (MD_OS_W32_FileIter*)iter->memory;
	switch (w32_iter->is_volume_iter)
	{
		//- rjf: file iteration
		default:

		case 0:
		{
			if (!(flags & MD_OS_FileIterFlag_Done) && w32_iter->handle != INVALID_HANDLE_VALUE)
			{
				do
				{
					// check is usable
					MD_B32 usable_file = 1;
					
					WCHAR* file_name  = w32_iter->find_data.cFileName;
					DWORD  attributes = w32_iter->find_data.dwFileAttributes;
					if (file_name[0] == '.')
					{
						if (flags & MD_OS_FileIterFlag_SkipHiddenFiles) {
							usable_file = 0;
						}
						else if (file_name[1] == 0){
							usable_file = 0;
						}
						else if (file_name[1] == '.' && file_name[2] == 0) {
							usable_file = 0;
						}
					}
					if (attributes & FILE_ATTRIBUTE_DIRECTORY) 
					{
						if (flags & MD_OS_FileIterFlag_SkipFolders) {
							usable_file = 0;
						}
					}
					else
					{
						if (flags & MD_OS_FileIterFlag_SkipFiles) {
							usable_file = 0;
						}
					}
				
					// emit if usable
					if (usable_file)
					{
						info_out->name       = md_str8_from(ainfo, md_str16_cstring((MD_U16*)file_name));
						info_out->props.size = (MD_U64)w32_iter->find_data.nFileSizeLow | (((MD_U64)w32_iter->find_data.nFileSizeHigh) << 32);
						md_os_w32_dense_time_from_file_time(&info_out->props.created,  &w32_iter->find_data.ftCreationTime);
						md_os_w32_dense_time_from_file_time(&info_out->props.modified, &w32_iter->find_data.ftLastWriteTime);
						info_out->props.flags = md_os_w32_file_property_flags_from_dwFileAttributes(attributes);

						result = 1;
						if (!FindNextFileW(w32_iter->handle, &w32_iter->find_data)) {
							iter->flags |= MD_OS_FileIterFlag_Done;
						}

						break;
					}
				}
				while (FindNextFileW(w32_iter->handle, &w32_iter->find_data));
			}
		}
		break;
		
		//- rjf: volume iteration
		case 1:
		{
			result = w32_iter->drive_strings_iter_idx < w32_iter->drive_strings.count;
			if (result != 0)
			{
				md_memory_zero_struct(info_out);
				info_out->name                    = w32_iter->drive_strings.v[w32_iter->drive_strings_iter_idx];
				info_out->props.flags            |= MD_FilePropertyFlag_IsFolder;
				w32_iter->drive_strings_iter_idx += 1;
			}
		}
		break;
	}
	if (!result)
	{
		iter->flags |= MD_OS_FileIterFlag_Done;
	}
	return result;
}

void
md_os_file_iter_end(MD_OS_FileIter* iter) {
	MD_OS_W32_FileIter* w32_iter = (MD_OS_W32_FileIter*)iter->memory;
	FindClose(w32_iter->handle);
}

//- rjf: directory creation

MD_B32
md_os_make_directory(MD_String8 path)
{
	MD_B32 result = 0;
	MD_TempArena scratch = md_scratch_begin(0, 0);
	{
		MD_String16                  name16     = md_str16_from(scratch.arena, path);
		WIN32_FILE_ATTRIBUTE_DATA attributes = {0};
		GetFileAttributesExW((WCHAR*)name16.str, GetFileExInfoStandard, &attributes);
		if (attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			result = 1;
		}
		else if (CreateDirectoryW((WCHAR*)name16.str, 0)) {
			result = 1;
		}
	}
	scratch_end(scratch);
	return(result);
}

////////////////////////////////
//~ rjf: @md_os_hooks Shared Memory (Implemented Per-OS)

MD_OS_Handle
md_os_shared_memory_alloc(MD_U64 size, MD_String8 name)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String16 name16 = md_str16_from(scratch.arena, name);
	HANDLE   file   = CreateFileMappingW(INVALID_HANDLE_VALUE,
		0,
		PAGE_READWRITE,
		(MD_U32)((size & 0xffffffff00000000) >> 32),
		(MD_U32)((size & 0x00000000ffffffff)),
		(WCHAR *)name16.str
	);
	scratch_end(scratch);
	MD_OS_Handle result = {(MD_U64)file};
	return result;
}

MD_OS_Handle
md_os_shared_memory_open(MD_String8 name)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String16 name16 = md_str16_from(scratch.arena, name);
	HANDLE   file   = OpenFileMappingW(FILE_MAP_ALL_ACCESS, 0, (WCHAR *)name16.str);
	scratch_end(scratch);
	MD_OS_Handle result = {(MD_U64)file};
	return result;
}

void
md_os_shared_memory_close(MD_OS_Handle handle) {
	HANDLE file = (HANDLE)(handle.u64[0]);
	CloseHandle(file);
}

void*
md_os_shared_memory_view_open(MD_OS_Handle handle, MD_Rng1U64 range)
{
	HANDLE file = (HANDLE)(handle.u64[0]);
	MD_U64   offset = range.md_min;
	MD_U64   size   = range.md_max-range.md_min;
	void* ptr    = MapViewOfFile(file, FILE_MAP_ALL_ACCESS,
		(MD_U32)((offset & 0xffffffff00000000) >> 32),
		(MD_U32)((offset & 0x00000000ffffffff)),
		size
	);
	return ptr;
}

void
md_os_shared_memory_view_close(MD_OS_Handle handle, void *ptr, MD_Rng1U64 range) {
	UnmapViewOfFile(ptr);
}

////////////////////////////////
//~ rjf: @md_os_hooks Time (Implemented Per-OS)

MD_U64
md_os_now_microseconds(void) {
	MD_U64 result = 0;
	LARGE_INTEGER large_int_counter;
	if (QueryPerformanceCounter(&large_int_counter)) {
		result = (large_int_counter.QuadPart * md_million(1)) / md_os_w32_state.microsecond_resolution;
	}
	return result;
}

MD_U32
md_os_now_unix(void) {
	FILETIME file_time;
	GetSystemTimeAsFileTime(&file_time);
	MD_U64 win32_time  = ((MD_U64)file_time.dwHighDateTime << 32) | file_time.dwLowDateTime;
	MD_U64 unix_time64 = ((win32_time - 0x19DB1DED53E8000ULL) / 10000000);
	MD_U32 unix_time32 = (MD_U32)unix_time64;
	return unix_time32;
}

MD_DateTime
md_os_now_universal_time(void) {
	SYSTEMTIME systime = {0};
	GetSystemTime(&systime);
	MD_DateTime result = {0};
	md_os_w32_date_time_from_system_time(&result, &systime);
	return result;
}

MD_DateTime
md_os_universal_time_from_local(MD_DateTime* date_time)
{
	SYSTEMTIME systime = {0};
	md_os_w32_system_time_from_date_time(&systime, date_time);
	FILETIME ftime = {0};
	SystemTimeToFileTime(&systime, &ftime);
	FILETIME ftime_local = {0};
	LocalFileTimeToFileTime(&ftime, &ftime_local);
	FileTimeToSystemTime(&ftime_local, &systime);
	MD_DateTime result = {0};
	md_os_w32_date_time_from_system_time(&result, &systime);
	return result;
}

MD_DateTime
md_os_local_time_from_universal(MD_DateTime* date_time)
{
	SYSTEMTIME systime = {0};
	md_os_w32_system_time_from_date_time(&systime, date_time);
	FILETIME ftime = {0};
	SystemTimeToFileTime(&systime, &ftime);
	FILETIME ftime_local = {0};
	FileTimeToLocalFileTime(&ftime, &ftime_local);
	FileTimeToSystemTime(&ftime_local, &systime);
	MD_DateTime result = {0};
	md_os_w32_date_time_from_system_time(&result, &systime);
	return result;
}

void
md_os_sleep_milliseconds(MD_U32 msec) {
	Sleep(msec);
}

////////////////////////////////
//~ rjf: @md_os_hooks Child Processes (Implemented Per-OS)

MD_OS_Handle
md_os_process_launch(MD_OS_ProcessLaunchParams* params)
{
	MD_OS_Handle result = {0};
	MD_TempArena scratch = md_scratch_begin(0, 0);
  
	//- rjf: form full command string
	MD_String8 cmd = {0};
	{
		MD_StringJoin 
		join_params = {0};
		join_params.pre  = md_str8_lit("\"");
		join_params.sep  = md_str8_lit("\" \"");
		join_params.post = md_str8_lit("\"");
		cmd = md_str8_list_join(scratch.arena, &params->cmd_line, &join_params);
	}
  
	//- rjf: form environment
	MD_B32 use_null_env_arg = 0;
	MD_String8 env = {0};
	{
		MD_StringJoin 
		join_params2 = {0};
		join_params2.sep  = md_str8_lit("\0");
		join_params2.post = md_str8_lit("\0");

		MD_String8List all_opts = params->env;
		if (params->inherit_env != 0)
		{
			if (all_opts.md_node_count != 0)
			{
				md_memory_zero_struct(&all_opts);
				for (MD_String8Node *n = params->env.first; n != 0; n = n->next) {
					md_str8_list_push(scratch.arena, &all_opts, n->string);
				}
				for (MD_String8Node *n = md_os_w32_state.process_info.environment.first; n != 0; n = n->next) {
					md_str8_list_push(scratch.arena, &all_opts, n->string);
				}
			}
			else {
				use_null_env_arg = 1;
			}
		}
		if (use_null_env_arg == 0) {
			env = md_str8_list_join(scratch.arena, &all_opts, &join_params2);
		}
	}
  
	//- rjf: utf-8 -> utf-16
	MD_String16 cmd16 = md_str16_from(scratch.arena, cmd);
	MD_String16 dir16 = md_str16_from(scratch.arena, params->path);
	MD_String16 env16 = {0};
	if (use_null_env_arg == 0) {
		env16 = md_str16_from(scratch.arena, env);
	}
  
	//- rjf: determine creation flags
	DWORD creation_flags = CREATE_UNICODE_ENVIRONMENT;
	if (params->consoleless) {
		creation_flags |= CREATE_NO_WINDOW;
	}
  
	//- rjf: launch
	STARTUPINFOW        startup_info = {sizeof(startup_info)};
	PROCESS_INFORMATION process_info = {0};
	if (CreateProcessW(0, (WCHAR*)cmd16.str, 0, 0, 0, creation_flags, use_null_env_arg ? 0 : (WCHAR*)env16.str, (WCHAR*)dir16.str, &startup_info, &process_info))
	{
		result.u64[0] = (MD_U64)process_info.hProcess;
		CloseHandle(process_info.hThread);
	}
  
	scratch_end(scratch);
	return result;
}

MD_B32
md_os_process_join(MD_OS_Handle handle, MD_U64 endt_us) {
  HANDLE process = (HANDLE)(handle.u64[0]);
  DWORD sleep_ms = md_os_w32_sleep_ms_from_endt_us(endt_us);
  DWORD result = WaitForSingleObject(process, sleep_ms);
  return (result == WAIT_OBJECT_0);
}

void
md_os_process_detach(MD_OS_Handle handle) {
  HANDLE process = (HANDLE)(handle.u64[0]);
  CloseHandle(process);
}

////////////////////////////////
//~ rjf: @md_os_hooks Threads (Implemented Per-OS)

MD_OS_Handle
md_os_thread_launch(MD_OS_ThreadFunctionType* func, void* ptr, void* params) {
	MD_OS_W32_Entity* entity = md_os_w32_entity_alloc(MD_OS_W32_EntityKind_Thread);
	entity->thread.func   = func;
	entity->thread.ptr    = ptr;
	entity->thread.handle = CreateThread(0, 0, md_os_w32_thread_entry_point, entity, 0, &entity->thread.tid);
	MD_OS_Handle result = {md_int_from_ptr(entity)};
	return result;
}

MD_B32
md_os_thread_join(MD_OS_Handle handle, MD_U64 endt_us)
{
	DWORD          sleep_ms    = md_os_w32_sleep_ms_from_endt_us(endt_us);
	MD_OS_W32_Entity* entity      = (MD_OS_W32_Entity*)md_ptr_from_int(handle.u64[0]);
	DWORD          wait_result = WAIT_OBJECT_0;
	if (entity != 0) {
		wait_result = WaitForSingleObject(entity->thread.handle, sleep_ms);
	}
	md_os_w32_entity_release(entity);
	return (wait_result == WAIT_OBJECT_0);
}

void
md_os_thread_detach(MD_OS_Handle thread) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(thread.u64[0]);
	md_os_w32_entity_release(entity);
}

////////////////////////////////
//~ rjf: @md_os_hooks Synchronization Primitives (Implemented Per-OS)

//- rjf: mutexes

MD_OS_Handle
md_os_mutex_alloc(void) {
	MD_OS_W32_Entity* entity = md_os_w32_entity_alloc(MD_OS_W32_EntityKind_Mutex);
	InitializeCriticalSection(&entity->mutex);
	MD_OS_Handle result = {md_int_from_ptr(entity)};
	return result;
}

void
md_os_mutex_release(MD_OS_Handle mutex) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(mutex.u64[0]);
	md_os_w32_entity_release(entity);
}

void
md_os_mutex_take(MD_OS_Handle mutex) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(mutex.u64[0]);
	EnterCriticalSection(&entity->mutex);
}

void
md_os_mutex_drop(MD_OS_Handle mutex)
{
  MD_OS_W32_Entity *entity = (MD_OS_W32_Entity*)md_ptr_from_int(mutex.u64[0]);
  LeaveCriticalSection(&entity->mutex);
}

//- rjf: reader/writer mutexes

MD_OS_Handle
md_os_rw_mutex_alloc(void) {
	MD_OS_W32_Entity* entity = md_os_w32_entity_alloc(MD_OS_W32_EntityKind_RWMutex);
	InitializeSRWLock(&entity->rw_mutex);
	MD_OS_Handle result = {md_int_from_ptr(entity)};
	return    result;
}

void
md_os_rw_mutex_release(MD_OS_Handle rw_mutex) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(rw_mutex.u64[0]);
	md_os_w32_entity_release(entity);
}

void
md_os_rw_mutex_take_r(MD_OS_Handle rw_mutex) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(rw_mutex.u64[0]);
	AcquireSRWLockShared(&entity->rw_mutex);
}

void
md_os_rw_mutex_drop_r(MD_OS_Handle rw_mutex) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(rw_mutex.u64[0]);
	ReleaseSRWLockShared(&entity->rw_mutex);
}

void
md_os_rw_mutex_take_w(MD_OS_Handle rw_mutex) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(rw_mutex.u64[0]);
	AcquireSRWLockExclusive(&entity->rw_mutex);
}

void
md_os_rw_mutex_drop_w(MD_OS_Handle rw_mutex) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(rw_mutex.u64[0]);
	ReleaseSRWLockExclusive(&entity->rw_mutex);
}

//- rjf: condition variables

MD_OS_Handle
md_os_condition_variable_alloc(void) {
	MD_OS_W32_Entity* entity = md_os_w32_entity_alloc(MD_OS_W32_EntityKind_ConditionVariable);
	InitializeConditionVariable(&entity->cv);
	MD_OS_Handle result = {md_int_from_ptr(entity)};
	return    result;
}

void
md_os_condition_variable_release(MD_OS_Handle cv) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(cv.u64[0]);
	md_os_w32_entity_release(entity);
}

MD_B32
md_os_condition_variable_wait(MD_OS_Handle cv, MD_OS_Handle mutex, MD_U64 endt_us) {
	MD_U32  sleep_ms = md_os_w32_sleep_ms_from_endt_us(endt_us);
	BOOL result   = 0;
	if (sleep_ms > 0) {
		MD_OS_W32_Entity* entity       = (MD_OS_W32_Entity*)md_ptr_from_int(cv.u64[0]);
		MD_OS_W32_Entity* mutex_entity = (MD_OS_W32_Entity*)md_ptr_from_int(mutex.u64[0]);
		result = SleepConditionVariableCS(&entity->cv, &mutex_entity->mutex, sleep_ms);
	}
	return result;
}

MD_B32
md_os_condition_variable_wait_rw_r(MD_OS_Handle cv, MD_OS_Handle mutex_rw, MD_U64 endt_us) {
	MD_U32  sleep_ms = md_os_w32_sleep_ms_from_endt_us(endt_us);
	BOOL result   = 0;
	if (sleep_ms > 0) {
		MD_OS_W32_Entity* entity       = (MD_OS_W32_Entity*)md_ptr_from_int(cv.u64[0]);
		MD_OS_W32_Entity* mutex_entity = (MD_OS_W32_Entity*)md_ptr_from_int(mutex_rw.u64[0]);
		result = SleepConditionVariableSRW(&entity->cv, &mutex_entity->rw_mutex, sleep_ms, CONDITION_VARIABLE_LOCKMODE_SHARED);
	}
	return result;
}

MD_B32
md_os_condition_variable_wait_rw_w(MD_OS_Handle cv, MD_OS_Handle mutex_rw, MD_U64 endt_us) {
	MD_U32  sleep_ms = md_os_w32_sleep_ms_from_endt_us(endt_us);
	BOOL result   = 0;
	if(sleep_ms > 0)
	{
		MD_OS_W32_Entity* entity       = (MD_OS_W32_Entity*)md_ptr_from_int(cv.u64[0]);
		MD_OS_W32_Entity* mutex_entity = (MD_OS_W32_Entity*)md_ptr_from_int(mutex_rw.u64[0]);
		result = SleepConditionVariableSRW(&entity->cv, &mutex_entity->rw_mutex, sleep_ms, 0);
	}
	return result;
}

void
md_os_condition_variable_signal(MD_OS_Handle cv) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(cv.u64[0]);
	WakeConditionVariable(&entity->cv);
}

void
md_os_condition_variable_broadcast(MD_OS_Handle cv) {
	MD_OS_W32_Entity* entity = (MD_OS_W32_Entity*)md_ptr_from_int(cv.u64[0]);
	WakeAllConditionVariable(&entity->cv);
}

//- rjf: cross-process semaphores

MD_OS_Handle
md_os_semaphore_alloc(MD_U32 initial_count, MD_U32 max_count, MD_String8 name) {
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String16  name16  = md_str16_from(scratch.arena, name);
	HANDLE    handle  = CreateSemaphoreW(0, initial_count, max_count, (WCHAR *)name16.str);
	MD_OS_Handle result  = {(MD_U64)handle};
	scratch_end(scratch);
	return result;
}

void
md_os_semaphore_release(MD_OS_Handle semaphore) {
	HANDLE handle = (HANDLE)semaphore.u64[0];
	CloseHandle(handle);
}

MD_OS_Handle
md_os_semaphore_open(MD_String8 name) {
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String16  name16  = md_str16_from(scratch.arena, name);
	HANDLE    handle  = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS , 0, (WCHAR *)name16.str);
	MD_OS_Handle result  = {(MD_U64)handle};
	scratch_end(scratch);
	return result;
}

void
md_os_semaphore_close(MD_OS_Handle semaphore) {
	HANDLE handle = (HANDLE)semaphore.u64[0];
	CloseHandle(handle);
}

MD_B32
md_os_semaphore_take(MD_OS_Handle semaphore, MD_U64 endt_us) {
	MD_U32    sleep_ms    = md_os_w32_sleep_ms_from_endt_us(endt_us);
	HANDLE handle      = (HANDLE)semaphore.u64[0];
	DWORD  wait_result = WaitForSingleObject(handle, sleep_ms);
	MD_B32    result      = (wait_result == WAIT_OBJECT_0);
	return result;
}

void
md_os_semaphore_drop(MD_OS_Handle semaphore) {
	HANDLE handle = (HANDLE)semaphore.u64[0];
	ReleaseSemaphore(handle, 1, 0);
}

////////////////////////////////
//~ rjf: @md_os_hooks Dynamically-Loaded Libraries (Implemented Per-OS)

MD_OS_Handle
md_os_library_open(MD_String8 path)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String16  path16  = md_str16_from(scratch.arena, path);
	HMODULE   mod     = LoadLibraryW((LPCWSTR)path16.str);
	MD_OS_Handle result  = { (MD_U64)mod };
	scratch_end(scratch);
	return result;
}

MD_VoidProc*
md_os_library_load_proc(MD_OS_Handle lib, MD_String8 name)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	HMODULE   mod     = (HMODULE)lib.u64[0];
	          name    = md_str8_copy(scratch.arena, name);
	MD_VoidProc* result  = (MD_VoidProc*)GetProcAddress(mod, (LPCSTR)name.str);
	scratch_end(scratch);
	return result;
}

void
md_os_library_close(MD_OS_Handle lib) {
	HMODULE mod = (HMODULE)lib.u64[0];
	FreeLibrary(mod);
}

////////////////////////////////
//~ rjf: @md_os_hooks Safe Calls (Implemented Per-OS)

void
md_os_safe_call(MD_OS_ThreadFunctionType *func, MD_OS_ThreadFunctionType *fail_handler, void *ptr)
{
	__try {
		func(ptr);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		if (fail_handler != 0)
		{
			fail_handler(ptr);
		}
		ExitProcess(1);
	}
}

////////////////////////////////
//~ rjf: @md_os_hooks GUIDs (Implemented Per-OS)

MD_OS_Guid
md_os_make_guid(void)
{
	MD_OS_Guid    result; md_memory_zero_struct(&result);
	UUID       uuid;
	RPC_STATUS rpc_status = UuidCreate(&uuid);
	if (rpc_status == RPC_S_OK)
	{
		result.data1 = uuid.Data1;
		result.data2 = uuid.Data2;
		result.data3 = uuid.Data3;
		md_memory_copy_array(result.data4, uuid.Data4);
	}
	return result;
}

////////////////////////////////
//~ rjf: @md_os_hooks Entry Points (Implemented Per-OS)

#if MD_BUILD_ENTRY_DEFINING_UNIT

#include <dbghelp.h>
#undef MD_OS_WINDOWS // shlwapi uses its own MD_OS_WINDOWS include inside
#include <shlwapi.h>

MD_B32 win32_g_is_quiet = 0;

HRESULT WINAPI
win32_dialog_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, LONG_PTR data)
{
	if (msg == TDN_HYPERLINK_CLICKED)
	{
		ShellExecuteW(NULL, L"open", (LPWSTR)lparam, NULL, NULL, SW_SHOWNORMAL);
	}
	return S_OK;
}

LONG WINAPI
win32_exception_filter(EXCEPTION_POINTERS* exception_ptrs)
{
	if (win32_g_is_quiet)
	{
		ExitProcess(1);
	}
  
	static volatile LONG first = 0;
	if (InterlockedCompareExchange(&first, 1, 0) != 0)
	{
		// prevent failures in other threads to popup same message box
		// this handler just shows first thread that crashes
		// we are terminating afterwards anyway
		for (;;) Sleep(1000);
	}
  
	WCHAR buffer[4096] = {0};
	int buflen = 0;
  
	DWORD exception_code = exception_ptrs->ExceptionRecord->ExceptionCode;
	buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L"A fatal exception (code 0x%x) occurred. The process is terminating.\n", exception_code);
  
	// load dbghelp dynamically just in case if it is missing
	HMODULE dbghelp = LoadLibraryA("dbghelp.dll");
	if(dbghelp)
	{
		DWORD (WINAPI * dbg_SymSetOptions )(DWORD  SymOptions);
		BOOL  (WINAPI * dbg_SymInitializeW)(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess);
		BOOL  (WINAPI * dbg_StackWalk64)(
			DWORD                            MachineType, 
			HANDLE                           hProcess, 
			HANDLE                           hThread,
			LPSTACKFRAME64                   StackFrame, 
			PVOID                            ContextRecord,
			PREAD_PROCESS_MEMORY_ROUTINE64   ReadMemoryRoutine,
			PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
			PGET_MODULE_BASE_ROUTINE64       GetModuleBaseRoutine,
			PTRANSLATE_ADDRESS_ROUTINE64     TranslateAddress
		);
		PVOID   (WINAPI * dbg_SymFunctionTableAccess64)(HANDLE hProcess, DWORD64 AddrBase);
		DWORD64 (WINAPI * dbg_SymGetModuleBase64      )(HANDLE hProcess, DWORD64 qwAddr);
		BOOL    (WINAPI * dbg_SymFromAddrW            )(HANDLE hProcess, DWORD64 Address, PDWORD64    Displacement, PSYMBOL_INFOW     Symbol);
		BOOL    (WINAPI * dbg_SymGetLineFromAddrW64   )(HANDLE hProcess, DWORD64 dwAddr,  PDWORD   pdwDisplacement, PIMAGEHLP_LINEW64 Line);
		BOOL    (WINAPI * dbg_SymGetModuleInfoW64     )(HANDLE hProcess, DWORD64 qwAddr,  PIMAGEHLP_MODULEW64 ModuleInfo);
    
		* (FARPROC*) &dbg_SymSetOptions            = GetProcAddress(dbghelp, "SymSetOptions");
		* (FARPROC*) &dbg_SymInitializeW           = GetProcAddress(dbghelp, "SymInitializeW");
		* (FARPROC*) &dbg_StackWalk64              = GetProcAddress(dbghelp, "StackWalk64");
		* (FARPROC*) &dbg_SymFunctionTableAccess64 = GetProcAddress(dbghelp, "SymFunctionTableAccess64");
		* (FARPROC*) &dbg_SymGetModuleBase64       = GetProcAddress(dbghelp, "SymGetModuleBase64");
		* (FARPROC*) &dbg_SymFromAddrW             = GetProcAddress(dbghelp, "SymFromAddrW");
		* (FARPROC*) &dbg_SymGetLineFromAddrW64    = GetProcAddress(dbghelp, "SymGetLineFromAddrW64");
		* (FARPROC*) &dbg_SymGetModuleInfoW64      = GetProcAddress(dbghelp, "SymGetModuleInfoW64");
    
		if(dbg_SymSetOptions && dbg_SymInitializeW && dbg_StackWalk64 && dbg_SymFunctionTableAccess64 && dbg_SymGetModuleBase64 && dbg_SymFromAddrW && dbg_SymGetLineFromAddrW64 && dbg_SymGetModuleInfoW64)
		{
			HANDLE   process = GetCurrentProcess();
			HANDLE   thread  = GetCurrentThread();
			CONTEXT* context = exception_ptrs->ContextRecord;
      
			dbg_SymSetOptions(SYMOPT_EXACT_SYMBOLS | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
			if(dbg_SymInitializeW(process, L"", TRUE))
			{
				// check that raddbg.pdb file is good
				MD_B32 raddbg_pdb_valid = 0;
				{
					IMAGEHLP_MODULEW64
					module              = {0};
					module.SizeOfStruct = sizeof(module);
					if(dbg_SymGetModuleInfoW64(process, (DWORD64)&win32_exception_filter, &module)) {
						raddbg_pdb_valid = (module.SymType == SymPdb);
					}
				}
        
				if(!raddbg_pdb_valid)
				{
					buflen += wnsprintfW(buffer + buflen, sizeof(buffer) - buflen,
						L"\nThe PDB debug information file for this executable is not valid or was not found. Please rebuild binary to get the call stack.\n"
					);
				}
				else
				{
					STACKFRAME64 frame = {0};
					DWORD image_type;
				#if defined(_M_AMD64)
					image_type = IMAGE_FILE_MACHINE_AMD64;
					frame.AddrPC.Offset = context->Rip;
					frame.AddrPC.Mode = AddrModeFlat;
					frame.AddrFrame.Offset = context->Rbp;
					frame.AddrFrame.Mode = AddrModeFlat;
					frame.AddrStack.Offset = context->Rsp;
					frame.AddrStack.Mode = AddrModeFlat;
				#elif defined(_M_ARM64)
					image_type = IMAGE_FILE_MACHINE_ARM64;
					frame.AddrPC.Offset = context->Pc;
					frame.AddrPC.Mode = AddrModeFlat;
					frame.AddrFrame.Offset = context->Fp;
					frame.AddrFrame.Mode = AddrModeFlat;
					frame.AddrStack.Offset = context->Sp;
					frame.AddrStack.Mode = AddrModeFlat;
				#else
				#	error Architecture not supported!
				#endif
          
					for(MD_U32 idx=0; ;idx++)
					{
						const MD_U32 max_frames = 32;
						if(idx == max_frames)
						{
							buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L"...");
							break;
						}
					
						if(!dbg_StackWalk64(image_type, process, thread, &frame, context, 0, dbg_SymFunctionTableAccess64, dbg_SymGetModuleBase64, 0))
						{
							break;
						}
					
						MD_U64 address = frame.AddrPC.Offset;
						if(address == 0)
						{
							break;
						}
					
						if(idx==0)
						{
						#if MD_BUILD_CONSOLE_INTERFACE
							buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L"\nCreate a new issue with this report at %S.\n\n", MD_BUILD_ISSUES_LINK_STRING_LITERAL);
						#else
							buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen,
								L"\nPress Ctrl+C to copy this text to clipboard, then create a new issue at\n"
								L"<a href=\"%S\">%S</a>\n\n", MD_BUILD_ISSUES_LINK_STRING_LITERAL, MD_BUILD_ISSUES_LINK_STRING_LITERAL);
						#endif
							buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L"Call stack:\n");
						}
					
						buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L"%u. [0x%I64x]", idx + 1, address);
						
						struct 
						{
							SYMBOL_INFOW info;
							WCHAR        name[MAX_SYM_NAME];
						} 
						symbol = {0};
						symbol.info.SizeOfStruct = sizeof(symbol.info);
						symbol.info.MaxNameLen   = MAX_SYM_NAME;
					
						DWORD64 displacement = 0;
						if (dbg_SymFromAddrW(process, address, &displacement, &symbol.info))
						{
							buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L" %s +%u", symbol.info.Name, (DWORD)displacement);
						
							IMAGEHLP_LINEW64 
							line = {0};
							line.SizeOfStruct = sizeof(line);
						
							DWORD line_displacement = 0;
							if(dbg_SymGetLineFromAddrW64(process, address, &line_displacement, &line)) {
								buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L", %s line %u", PathFindFileNameW(line.FileName), line.LineNumber);
							}
						}
						else
						{
							IMAGEHLP_MODULEW64 
							module = {0};
							module.SizeOfStruct = sizeof(module);
							if (dbg_SymGetModuleInfoW64(process, address, &module)) {
								buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L" %s", module.ModuleName);
							}
						}
					
						buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L"\n");
					}
				}
			}
		}
	}
  
	buflen += wnsprintfW(buffer + buflen, md_array_count(buffer) - buflen, L"\nVersion: %S%S", MD_BUILD_VERSION_STRING_LITERAL, MD_BUILD_GIT_HASH_STRING_LITERAL_APPEND);
  
	#if MD_BUILD_CONSOLE_INTERFACE
		fwprintf(stderr, L"\n--- Fatal Exception ---\n");
		fwprintf(stderr, L"%s\n\n", buffer);
	#else
		TASKDIALOGCONFIG dialog = {0};
		dialog.cbSize          = sizeof(dialog);
		dialog.dwFlags         = TDF_SIZE_TO_CONTENT | TDF_ENABLE_HYPERLINKS | TDF_ALLOW_DIALOG_CANCELLATION;
		dialog.pszMainIcon     = TD_ERROR_ICON;
		dialog.dwCommonButtons = TDCBF_CLOSE_BUTTON;
		dialog.pszWindowTitle  = L"Fatal Exception";
		dialog.pszContent      = buffer;
		dialog.pfCallback      = &win32_dialog_callback;
		TaskDialogIndirect(&dialog, 0, 0, 0);
	#endif
	
	ExitProcess(1);
}

#undef MD_OS_WINDOWS // shlwapi uses its own MD_OS_WINDOWS include inside
#define MD_OS_WINDOWS 1

void
w32_entry_point_caller(int argc, WCHAR** wargv)
{
	SetUnhandledExceptionFilter(&win32_exception_filter);
	//- rjf: do OS layer initialization
	{
		// rjf: dynamically load windows functions which are not guaranteed
		// in all SDKs
		{
			HMODULE module = LoadLibraryA("kernel32.dll");
			w32_SetThreadDescription_func = (W32_SetThreadDescription_Type *)GetProcAddress(module, "SetThreadDescription");
			FreeLibrary(module);
		}
    
		// rjf: try to enable large pages if we can
		{
			HANDLE token;
			if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
			{
				LUID luid;
				if (LookupPrivilegeValue(0, SE_LOCK_MEMORY_NAME, &luid))
				{
					TOKEN_PRIVILEGES priv;
					priv.PrivilegeCount           = 1;
					priv.Privileges[0].Luid       = luid;
					priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					AdjustTokenPrivileges(token, 0, &priv, sizeof(priv), 0, 0);
				}
				CloseHandle(token);
			}
		}
    
		// rjf: get system info
		SYSTEM_INFO sysinfo = {0};
		GetSystemInfo(&sysinfo);
    
		// rjf: set up non-dynamically-md_alloc'd state
		//
		// (we need to set up some basics before this layer can supply
		// memory allocation primitives)
		{
			md_os_w32_state.microsecond_resolution  = 1;
			LARGE_INTEGER large_int_resolution;
			if (QueryPerformanceFrequency(&large_int_resolution))
			{
				md_os_w32_state.microsecond_resolution = large_int_resolution.QuadPart;
			}
		}
		{
			MD_OS_SystemInfo* info = &md_os_w32_state.system_info;
			info->logical_processor_count = (MD_U64)sysinfo.dwNumberOfProcessors;
			info->page_size               = sysinfo.dwPageSize;
			info->large_page_size         = GetLargePageMinimum();
			info->allocation_granularity  = sysinfo.dwAllocationGranularity;
		}
		{
			MD_OS_ProcessInfo* info = &md_os_w32_state.process_info;
			info->pid = GetCurrentProcessId();
		}
    
		// rjf: set up thread context
		md_local_persist MD_TCTX md_tctx;
		md_tctx_init_and_equip(&md_tctx);
    
		// rjf: set up dynamically-md_alloc'd state
		MD_Arena* arena = md_arena_alloc(.backing = md_varena_alloc(0));
		{
			md_os_w32_state.arena = arena;
			{
				MD_OS_SystemInfo* info = &md_os_w32_state.system_info;
				MD_U8    buffer[MAX_COMPUTERNAME_LENGTH + 1] = {0};
				DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
				if(GetComputerNameA((char*)buffer, &size))
				{
					info->machine_name = md_push_str8_copy(arena, md_str8(buffer, size));
				}
			}
		}
		{
			MD_OS_ProcessInfo* info = &md_os_w32_state.process_info;
			{
				MD_TempArena scratch = md_scratch_begin(0, 0);
				DWORD   size         = MD_KB(32);
				MD_U16*    buffer       = md_push_array__no_zero(scratch.arena, MD_U16, size);
				DWORD   length       = GetModuleFileNameW(0, (WCHAR*)buffer, size);
				MD_String8 name8        = md_str8_from_16(scratch.arena, md_str16(buffer, length));
				MD_String8 name_chopped = md_str8_chop_last_slash(name8);
				info->binary_path    = md_push_str8_copy(arena, name_chopped);
				scratch_end(scratch);
			}

			info->initial_path = md_os_get_current_path(arena);
			{
				MD_TempArena scratch = md_scratch_begin(0, 0);
				MD_U64  size   = MD_KB(32);
				MD_U16* buffer = md_push_array__no_zero(scratch.arena, MD_U16, size);
				if (SUCCEEDED(SHGetFolderPathW(0, CSIDL_APPDATA, 0, 0, (WCHAR*)buffer))) {
					info->user_program_data_path = md_str8_from_16(arena, md_str16_cstring(buffer));
				}
				scratch_end(scratch);
			}
			{
				WCHAR* this_proc_env = GetEnvironmentStringsW();
				MD_U64    start_idx     = 0;
				for (MD_U64 idx = 0;; idx += 1)
				{
					if (this_proc_env[idx] == 0)
					{
						if (start_idx == idx)
						{
							break;
						}
						else
						{
							MD_String16 string16 = md_str16((MD_U16 *)this_proc_env + start_idx, idx - start_idx);
							MD_String8  string   = md_str8_from_16(arena, string16);
							md_str8_list_push(arena, &info->environment, string);
							start_idx = idx+1;
						}
					}
				}
			}
		}
    
		// rjf: set up entity storage
		InitializeCriticalSection(&md_os_w32_state.entity_mutex);
		md_os_w32_state.entity_arena = md_arena_alloc(.backing = md_varena_alloc(0));
	}
  
	//- rjf: extract arguments
	MD_Arena*  args_arena = md_arena_alloc(.backing = md_varena_alloc(.reserve_size = MD_MB(1), .commit_size = MD_KB(32)));
	char**  argv       = md_push_array_(args_arena, char*, argc);
	for (int i = 0; i < argc; i += 1)
	{
		MD_String16 arg16 = md_str16_cstring((MD_U16 *)wargv[i]);
		MD_String8  arg8  = md_str8_from_16(args_arena, arg16);
		if (md_str8_match(arg8, md_str8_lit("--quiet"), MD_StringMatchFlag_CaseInsensitive))
		{
			win32_g_is_quiet = 1;
		}
		argv[i] = (char *)arg8.str;
	}
  
	//- rjf: call into "real" entry point
	md_main_thread_base_entry_point(md_entry_point, argv, (MD_U64)argc);
}

#if MD_BUILD_CONSOLE_INTERFACE
int wmain(int argc, WCHAR** argv)
{
	w32_entry_point_caller(argc, argv);
	return 0;
}
#else
int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	w32_entry_point_caller(__argc, __wargv);
	return 0;
}
#endif

// MD_BUILD_ENTRY_POINT
#endif

////////////////////////////////
//~ Ed: Manual OS Bootstrap

void md_os_init(MD_OS_Context* ctx, MD_TCTX* thread_ctx)
{
	// rjf: dynamically load windows functions which are not guaranteed
	// in all SDKs
	{
		HMODULE module = LoadLibraryA("kernel32.dll");
		w32_SetThreadDescription_func = (W32_SetThreadDescription_Type *)GetProcAddress(module, "SetThreadDescription");
		FreeLibrary(module);
	}

	// rjf: try to enable large pages if we can
	if (ctx->enable_large_pages)
	{
		HANDLE token;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
		{
			LUID luid;
			if (LookupPrivilegeValue(0, SE_LOCK_MEMORY_NAME, &luid))
			{
				TOKEN_PRIVILEGES priv;
				priv.PrivilegeCount           = 1;
				priv.Privileges[0].Luid       = luid;
				priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				AdjustTokenPrivileges(token, 0, &priv, sizeof(priv), 0, 0);
			}
			CloseHandle(token);
		}
	}

	SYSTEM_INFO sysinfo = {0};
	GetSystemInfo(&sysinfo);

	// rjf: set up non-dynamically-md_alloc'd state
	//
	// (we need to set up some basics before this layer can supply
	// memory allocation primitives)
	{
		md_os_w32_state.microsecond_resolution  = 1;
		LARGE_INTEGER large_int_resolution;
		if (QueryPerformanceFrequency(&large_int_resolution))
		{
			md_os_w32_state.microsecond_resolution = large_int_resolution.QuadPart;
		}
	}
	{
		MD_OS_SystemInfo* info = &md_os_w32_state.system_info;
		info->logical_processor_count = (MD_U64)sysinfo.dwNumberOfProcessors;
		info->page_size               = sysinfo.dwPageSize;
		info->large_page_size         = GetLargePageMinimum();
		info->allocation_granularity  = sysinfo.dwAllocationGranularity;
	}
	{
		MD_OS_ProcessInfo* info = &md_os_w32_state.process_info;
		info->pid = GetCurrentProcessId();
	}

	// rjf: set up thread context
	md_tctx_init_and_equip(thread_ctx);

	// rjf: set up dynamically-md_alloc'd state
	MD_Arena* state_arena = ctx->entity_arena;
	if (state_arena == md_nullptr) {
		state_arena      = md_arena_alloc(.backing = md_varena_allocator(md_varena_alloc(0)));
		ctx->state_arena = state_arena;
	}
	{
		md_os_w32_state.arena = state_arena;
		{
			MD_OS_SystemInfo* info = &md_os_w32_state.system_info;
			MD_U8    buffer[MAX_COMPUTERNAME_LENGTH + 1] = {0};
			DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
			if(GetComputerNameA((char*)buffer, &size))
			{
				info->machine_name = md_str8_copy(state_arena, md_str8(buffer, size));
			}
		}
	}
	{
		MD_OS_ProcessInfo* info = &md_os_w32_state.process_info;
		{
			MD_TempArena scratch = md_scratch_begin(0, 0);
			DWORD   size         = MD_KB(32);
			MD_U16*    buffer       = md_push_array__no_zero(scratch.arena, MD_U16, size);
			DWORD   length       = GetModuleFileNameW(0, (WCHAR*)buffer, size);
			MD_String8 name8        = md_str8_from(scratch.arena, md_str16(buffer, length));
			MD_String8 name_chopped = md_str8_chop_last_slash(name8);
			info->binary_path    = md_str8_copy(state_arena, name_chopped);
			scratch_end(scratch);
		}

		info->initial_path = md_os_get_current_path(state_arena);
		{
			MD_TempArena scratch = md_scratch_begin(0, 0);
			MD_U64  size   = MD_KB(32);
			MD_U16* buffer = md_push_array__no_zero(scratch.arena, MD_U16, size);
			if (SUCCEEDED(SHGetFolderPathW(0, CSIDL_APPDATA, 0, 0, (WCHAR*)buffer))) {
				info->user_program_data_path = md_str8_from(state_arena, md_str16_cstring(buffer));
			}
			scratch_end(scratch);
		}
		{
			WCHAR* this_proc_env = GetEnvironmentStringsW();
			MD_U64    start_idx     = 0;
			for (MD_U64 idx = 0;; idx += 1)
			{
				if (this_proc_env[idx] == 0)
				{
					if (start_idx == idx)
					{
						break;
					}
					else
					{
						MD_String16 string16 = md_str16((MD_U16 *)this_proc_env + start_idx, idx - start_idx);
						MD_String8  string   = md_str8_from(state_arena, string16);
						md_str8_list_push(state_arena, &info->environment, string);
						start_idx = idx+1;
					}
				}
			}
		}
	}

	// rjf: set up entity storage
	InitializeCriticalSection(&md_os_w32_state.entity_mutex);
	if (ctx->entity_arena == md_nullptr) {
		ctx->entity_arena         =  md_arena_alloc(.backing = md_varena_allocator(md_varena_alloc(0)));
		md_os_w32_state.entity_arena = ctx->entity_arena;
	}
}
