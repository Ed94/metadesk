#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/file.h"
#	include "base/debug.h"
#	include "os_resolve.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: System Info

typedef struct OS_SystemInfo OS_SystemInfo;
struct OS_SystemInfo
{
	U32     logical_processor_count;
	U64     page_size;
	U64     large_page_size;
	U64     allocation_granularity;
	String8 machine_name;
};

////////////////////////////////
//~ rjf: Process Info

typedef struct OS_ProcessInfo OS_ProcessInfo;
struct OS_ProcessInfo
{
	U32         pid;
	String8     binary_path;
	String8     initial_path;
	String8     user_program_data_path;
	String8List module_load_paths;
	String8List environment;
};

////////////////////////////////
//~ rjf: Access Flags

typedef U32 OS_AccessFlags;
enum
{
	OS_AccessFlag_Read       = (1 << 0),
	OS_AccessFlag_Write      = (1 << 1),
	OS_AccessFlag_Execute    = (1 << 2),
	OS_AccessFlag_Append     = (1 << 3),
	OS_AccessFlag_ShareRead  = (1 << 4),
	OS_AccessFlag_ShareWrite = (1 << 5),
};

////////////////////////////////
//~ rjf: Files

typedef U32 OS_FileIterFlags;
enum
{
	OS_FileIterFlag_SkipFolders     = (1 << 0),
	OS_FileIterFlag_SkipFiles       = (1 << 1),
	OS_FileIterFlag_SkipHiddenFiles = (1 << 2),
	OS_FileIterFlag_Done            = (1 << 31),
};

typedef struct OS_FileIter OS_FileIter;
struct OS_FileIter
{
	OS_FileIterFlags flags;
	U8               memory[800];
};

typedef struct OS_FileInfo OS_FileInfo;
struct OS_FileInfo
{
	String8        name;
	FileProperties props;
};

// nick: on-disk file identifier
typedef struct OS_FileID OS_FileID;
struct OS_FileID
{
	U64 v[3];
};

////////////////////////////////
//~ rjf: Process Launch Parameters

typedef struct OS_ProcessLaunchParams OS_ProcessLaunchParams;
struct OS_ProcessLaunchParams
{
	String8List cmd_line;
	String8     path;
	String8List env;
	B32         inherit_env;
	B32         consoleless;
};

////////////////////////////////
//~ rjf: Handle Type

typedef struct OS_Handle OS_Handle;
struct OS_Handle
{
	U64 u64[1];
};

typedef struct OS_HandleNode OS_HandleNode;
struct OS_HandleNode
{
	OS_HandleNode* next;
	OS_Handle      v;
};

typedef struct OS_HandleList OS_HandleList;
struct OS_HandleList
{
	OS_HandleNode* first;
	OS_HandleNode* last;
	U64            count;
};

typedef struct OS_HandleArray OS_HandleArray;
struct OS_HandleArray
{
	OS_Handle* v;
	U64        count;
};

////////////////////////////////
//~ rjf: Globally Unique IDs

typedef struct OS_Guid OS_Guid;
struct OS_Guid
{
	U32 data1;
	U16 data2;
	U16 data3;
	U8  data4[8];
};
md_static_assert(size_of(OS_Guid) == 16, os_guid_check);

////////////////////////////////
//~ rjf: Thread Types

typedef void OS_ThreadFunctionType(void *ptr);

////////////////////////////////
//~ rjf: Handle Type Functions (Helpers, Implemented Once)

force_inline OS_Handle os_handle_zero (void)                     { OS_Handle handle = {0}; return handle; }
force_inline B32       os_handle_match(OS_Handle a, OS_Handle b) { return a.u64[0] == b.u64[0]; }

void           os_handle_list_push            (Arena*        arena, OS_HandleList* handles, OS_Handle handle);
void           os_handle_list_alloc           (AllocatorInfo ainfo, OS_HandleList* handles, OS_Handle handle);
OS_HandleArray os_handle_array_from_list      (Arena*        arena, OS_HandleList* list);
OS_HandleArray os_handle_array_from_list_alloc(AllocatorInfo ainfo, OS_HandleList* list);

inline void
os_handle_list_push(Arena* arena, OS_HandleList* handles, OS_Handle handle) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	OS_HandleNode* n = push_array(arena, OS_HandleNode, 1);
	n->v = handle;
	sll_queue_push(handles->first, handles->last, n);
	handles->count += 1;
#else
	os_handle_list_alloc(arena_allocator(arena), handles, handle);
#endif
}

inline OS_HandleArray
os_handle_array_from_list(Arena* arena, OS_HandleList* list) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	OS_HandleArray result = {0};
	result.count = list->count;
	result.v     = push_array_no_zero(arena, OS_Handle, result.count);
	U64 idx = 0;
	for(OS_HandleNode* n = list->first; n != 0; n = n->next, idx += 1) {
		result.v[idx] = n->v;
	}
	return result;
#else
	return os_handle_array_from_list_alloc(arena_allocator(arena), list);
#endif
}

inline void
os_handle_list_alloc(AllocatorInfo ainfo, OS_HandleList* handles, OS_Handle handle) {
	OS_HandleNode* n = alloc_array(ainfo, OS_HandleNode, 1);
	n->v = handle;
	sll_queue_push(handles->first, handles->last, n);
	handles->count += 1;
}

inline OS_HandleArray
os_handle_array_from_list_alloc(AllocatorInfo ainfo, OS_HandleList* list) {
	OS_HandleArray result = {0};
	result.count = list->count;
	result.v     = alloc_array_no_zero(ainfo, OS_Handle, result.count);
	U64 idx = 0;
	for(OS_HandleNode* n = list->first; n != 0; n = n->next, idx += 1) {
		result.v[idx] = n->v;
	}
	return result;
}

////////////////////////////////
//~ rjf: Command Line Argc/Argv Helper (Helper, Implemented Once)

inline String8List
os_string_list_from_argcv_alloc(AllocatorInfo ainfo, int argc, char** argv) {
	String8List result = {0};
	for(int i = 0; i < argc; i += 1)
	{
		String8 str = str8_cstring(argv[i]);
		str8_list_alloc(ainfo, &result, str);
	}
	return result;
}

inline String8List
os_string_list_from_argcv(Arena* arena, int argc, char** argv) {
	String8List result = {0};
	for(int i = 0; i < argc; i += 1)
	{
		String8 str = str8_cstring(argv[i]);
		str8_list_push(arena, &result, str);
	}
	return result;
}

////////////////////////////////
//~ rjf: @os_hooks File System (Implemented Per-OS)

//- rjf: files
MD_API OS_Handle      os_file_open                (OS_AccessFlags flags, String8 path);
MD_API void           os_file_close               (OS_Handle file);
MD_API U64            os_file_read                (OS_Handle file, Rng1U64 rng, void *out_data);
MD_API U64            os_file_write               (OS_Handle file, Rng1U64 rng, void *data);
MD_API B32            os_file_set_times           (OS_Handle file, DateTime time);
MD_API FileProperties os_properties_from_file     (OS_Handle file);
MD_API OS_FileID      os_id_from_file             (OS_Handle file);
MD_API B32            os_delete_file_at_path      (String8   path);
MD_API B32            os_copy_file_path           (String8   dst,   String8 src);
MD_API String8        os_full_path_from_path      (Arena*    arena, String8 path);
MD_API B32            os_file_path_exists         (String8   path);
MD_API FileProperties os_properties_from_file_path(String8   path);

//- rjf: file maps
MD_API OS_Handle os_file_map_open      (OS_AccessFlags flags, OS_Handle file);
MD_API void      os_file_map_close     (OS_Handle map);
MD_API void*     os_file_map_view_open (OS_Handle map, OS_AccessFlags flags, Rng1U64 range);
MD_API void      os_file_map_view_close(OS_Handle map, void* ptr, Rng1U64 range);

//- rjf: directory iteration
MD_API OS_FileIter* os_file_iter_begin(Arena* arena, String8      path, OS_FileIterFlags flags);
MD_API B32          os_file_iter_next (Arena* arena, OS_FileIter* iter, OS_FileInfo*     info_out);
MD_API void         os_file_iter_end  (              OS_FileIter* iter);

//- rjf: directory creation
MD_API B32 os_make_directory(String8 path);

////////////////////////////////
//~ rjf: Filesystem Helpers (Helpers, Implemented Once)

MD_API B32       os_write_data_to_file_path     (String8 path, String8     data);
MD_API B32       os_write_data_list_to_file_path(String8 path, String8List list);
MD_API B32       os_append_data_to_file_path    (String8 path, String8     data);
       OS_FileID os_id_from_file_path           (String8 path);
       S64       os_file_id_compare             (OS_FileID a, OS_FileID b);

       String8 os_data_from_file_path         (Arena*        arena, String8 path);
       String8 os_data_from_file_path_alloc   (AllocatorInfo ainfo, String8 path);
MD_API String8 os_string_from_file_range      (Arena*        arena, OS_Handle file, Rng1U64 range);
MD_API String8 os_string_from_file_range_alloc(AllocatorInfo ainfo, OS_Handle file, Rng1U64 range);

inline String8
os_data_from_file_path(Arena* arena, String8 path)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	OS_Handle      file  = os_file_open(OS_AccessFlag_Read | OS_AccessFlag_ShareRead, path);
	FileProperties props = os_properties_from_file(file);
	String8        data  = os_string_from_file_range(arena, file, r1u64(0, props.size));
	os_file_close(file);
	return data;
#else
	return os_data_from_file_path_alloc(arena_allocator(arena), path);
#endif
}

inline String8
os_data_from_file_path_alloc(AllocatorInfo ainfo, String8 path)
{
	OS_Handle      file  = os_file_open(OS_AccessFlag_Read|OS_AccessFlag_ShareRead, path);
	FileProperties props = os_properties_from_file(file);
	String8        data  = os_string_from_file_range_alloc(ainfo, file, r1u64(0, props.size));
	os_file_close(file);
	return data;
}


inline OS_FileID
os_id_from_file_path(String8 path) {
	OS_Handle file = os_file_open(OS_AccessFlag_Read | OS_AccessFlag_ShareRead, path);
	OS_FileID id   = os_id_from_file(file);
	os_file_close(file);
	return id;
}

inline S64 os_file_id_compare(OS_FileID a, OS_FileID b) { S64 cmp = memory_compare((void*)&a.v[0], (void*)&b.v[0], sizeof(a.v)); return cmp; }

////////////////////////////////
//~ rjf: GUID Helpers (Helpers, Implemented Once)

inline String8
os_string_from_guid_alloc(AllocatorInfo ainfo, OS_Guid guid) {
	String8 result = alloc_str8f(ainfo, 
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.data1,
		guid.data2,
		guid.data3,
		guid.data4[0],
		guid.data4[1],
		guid.data4[2],
		guid.data4[3],
		guid.data4[4],
		guid.data4[5],
		guid.data4[6],
		guid.data4[7]
	);
	return result;
}

inline String8
os_string_from_guid(Arena* arena, OS_Guid guid) {
	String8 result = push_str8f(arena, 
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.data1,
		guid.data2,
		guid.data3,
		guid.data4[0],
		guid.data4[1],
		guid.data4[2],
		guid.data4[3],
		guid.data4[4],
		guid.data4[5],
		guid.data4[6],
		guid.data4[7]
	);
	return result;
}

////////////////////////////////
//~ rjf: @os_hooks System/Process Info (Implemented Per-OS)

MD_API OS_SystemInfo*  os_get_system_info (void);
MD_API OS_ProcessInfo* os_get_process_info(void);

String8 os_get_current_path(Arena* arena);

////////////////////////////////
//~ rjf: @os_hooks Memory Allocation (Implemented Per-OS)

//- rjf: basic
void* os_reserve (           U64 size);
B32   os_commit  (void* ptr, U64 size);
void  os_decommit(void* ptr, U64 size);
void  os_release (void* ptr, U64 size);

//- rjf: large pages
void* os_reserve_large(           U64 size);
B32   os_commit_large (void* ptr, U64 size);

////////////////////////////////
//~ rjf: @os_hooks Thread Info (Implemented Per-OS)

       U32  os_tid            (void);
MD_API void os_set_thread_name(String8 string);

////////////////////////////////
//~ rjf: @os_hooks Aborting (Implemented Per-OS)

MD_API void os_abort(S32 exit_code);

////////////////////////////////
//~ rjf: @os_hooks Shared Memory (Implemented Per-OS)

MD_API OS_Handle os_shared_memory_alloc     (U64       size, String8 name);
MD_API OS_Handle os_shared_memory_open      (String8   name);
MD_API void      os_shared_memory_close     (OS_Handle handle);
MD_API void*     os_shared_memory_view_open (OS_Handle handle, Rng1U64 range);
MD_API void      os_shared_memory_view_close(OS_Handle handle, void* ptr, Rng1U64 range);

////////////////////////////////
//~ rjf: @os_hooks Time (Implemented Per-OS)

MD_API U64         os_now_microseconds         (void);
MD_API U32         os_now_unix                 (void);
MD_API DateTime    os_now_universal_time       (void);
MD_API DateTime    os_universal_time_from_local(DateTime* local_time);
MD_API DateTime    os_local_time_from_universal(DateTime* universal_time);
MD_API void        os_sleep_milliseconds       (U32       msec);

////////////////////////////////
//~ rjf: @os_hooks Child Processes (Implemented Per-OS)

MD_API OS_Handle os_process_launch(OS_ProcessLaunchParams* params);
MD_API B32       os_process_join  (OS_Handle handle, U64 endt_us);
MD_API void      os_process_detach(OS_Handle handle);

////////////////////////////////
//~ rjf: @os_hooks Threads (Implemented Per-OS)

MD_API OS_Handle os_thread_launch(OS_ThreadFunctionType* func, void* ptr, void* params);
MD_API B32       os_thread_join  (OS_Handle handle, U64 endt_us);
MD_API void      os_thread_detach(OS_Handle handle);

////////////////////////////////
//~ rjf: @os_hooks Synchronization Primitives (Implemented Per-OS)

//- rjf: recursive mutexes
MD_API OS_Handle os_mutex_alloc  (void);
MD_API void      os_mutex_release(OS_Handle mutex);
MD_API void      os_mutex_take   (OS_Handle mutex);
MD_API void      os_mutex_drop   (OS_Handle mutex);

//- rjf: reader/writer mutexes
MD_API OS_Handle os_rw_mutex_alloc(void);
MD_API void      os_rw_mutex_release(OS_Handle rw_mutex);
MD_API void      os_rw_mutex_take_r(OS_Handle mutex);
MD_API void      os_rw_mutex_drop_r(OS_Handle mutex);
MD_API void      os_rw_mutex_take_w(OS_Handle mutex);
MD_API void      os_rw_mutex_drop_w(OS_Handle mutex);

//- rjf: condition variables
MD_API OS_Handle os_condition_variable_alloc  (void);
MD_API void      os_condition_variable_release(OS_Handle cv);
// returns false on timeout, true on signal, (max_wait_ms = max_U64) -> no timeout
MD_API B32       os_condition_variable_wait     (OS_Handle cv, OS_Handle mutex, U64 endt_us);
MD_API B32       os_condition_variable_wait_rw_r(OS_Handle cv, OS_Handle mutex_rw, U64 endt_us);
MD_API B32       os_condition_variable_wait_rw_w(OS_Handle cv, OS_Handle mutex_rw, U64 endt_us);
MD_API void      os_condition_variable_signal   (OS_Handle cv);
MD_API void      os_condition_variable_broadcast(OS_Handle cv);

//- rjf: cross-process semaphores
MD_API OS_Handle os_semaphore_alloc  (U32       initial_count, U32 max_count, String8 name);
MD_API void      os_semaphore_release(OS_Handle semaphore);
MD_API OS_Handle os_semaphore_open   (String8   name);
MD_API void      os_semaphore_close  (OS_Handle semaphore);
MD_API B32       os_semaphore_take   (OS_Handle semaphore, U64 endt_us);
MD_API void      os_semaphore_drop   (OS_Handle semaphore);

//- rjf: scope macros
#define OS_MutexScope(mutex)          defer_loop( os_mutex_take     (mutex), os_mutex_drop     (mutex))
#define OS_MutexScopeR(mutex)         defer_loop( os_rw_mutex_take_r(mutex), os_rw_mutex_drop_r(mutex))
#define OS_MutexScopeW(mutex)         defer_loop( os_rw_mutex_take_w(mutex), os_rw_mutex_drop_w(mutex))
#define OS_MutexScopeRWPromote(mutex) defer_loop((os_rw_mutex_drop_r(mutex), os_rw_mutex_take_w(mutex)), (os_rw_mutex_drop_w(mutex), os_rw_mutex_take_r(mutex)))

////////////////////////////////
//~ rjf: @os_hooks Dynamically-Loaded Libraries (Implemented Per-OS)

MD_API OS_Handle os_library_open     (String8   path);
MD_API void      os_library_close    (OS_Handle lib);
MD_API VoidProc* os_library_load_proc(OS_Handle lib, String8 name);

////////////////////////////////
//~ rjf: @os_hooks Safe Calls (Implemented Per-OS)

MD_API void os_safe_call(OS_ThreadFunctionType* func, OS_ThreadFunctionType* fail_handler, void* ptr);

////////////////////////////////
//~ rjf: @os_hooks GUIDs (Implemented Per-OS)

MD_API OS_Guid os_make_guid(void);

////////////////////////////////
//~ rjf: @os_hooks Entry Points (Implemented Per-OS)

// NOTE(rjf): The implementation of `os_core` will define low-level entry
// points if BUILD_ENTRY_DEFINING_UNIT is defined to 1. These will call
// into the standard codebase program entry points, named "entry_point".

#if BUILD_ENTRY_DEFINING_UNIT
void entry_point(CmdLine* cmdline);
#endif

////////////////////////////////
//~ Ed: Manual OS Bootstrap (Implemented Per-OS)

typedef struct OS_Context OS_Context;
struct OS_Context
{
	Arena* state_arena;
	Arena* entity_arena;
	B32    enable_large_pages;
};

// OS layer initialization
MD_API void os_init(OS_Context* ctx, TCTX* thread_ctx);
