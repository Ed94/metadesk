#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/debug.h"
#	include "base/strings.h"
#	include "base/thread_context.h"
#	include "base/file.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: System Info

typedef struct MD_OS_SystemInfo MD_OS_SystemInfo;
struct MD_OS_SystemInfo
{
	MD_U32     logical_processor_count;
	MD_U64     page_size;
	MD_U64     large_page_size;
	MD_U64     allocation_granularity;
	MD_String8 machine_name;
};

////////////////////////////////
//~ rjf: Process Info

typedef struct MD_OS_ProcessInfo MD_OS_ProcessInfo;
struct MD_OS_ProcessInfo
{
	MD_U32         pid;
	MD_String8     binary_path;
	MD_String8     initial_path;
	MD_String8     user_program_data_path;
	MD_String8List module_load_paths;
	MD_String8List environment;
};

////////////////////////////////
//~ rjf: Access Flags

typedef MD_U32 MD_OS_AccessFlags;
enum
{
	MD_OS_AccessFlag_Read       = (1 << 0),
	MD_OS_AccessFlag_Write      = (1 << 1),
	MD_OS_AccessFlag_Execute    = (1 << 2),
	MD_OS_AccessFlag_Append     = (1 << 3),
	MD_OS_AccessFlag_ShareRead  = (1 << 4),
	MD_OS_AccessFlag_ShareWrite = (1 << 5),
};

////////////////////////////////
//~ rjf: Files

typedef MD_U32 MD_OS_FileIterFlags;
enum
{
	MD_OS_FileIterFlag_SkipFolders     = (1 << 0),
	MD_OS_FileIterFlag_SkipFiles       = (1 << 1),
	MD_OS_FileIterFlag_SkipHiddenFiles = (1 << 2),
	MD_OS_FileIterFlag_Done            = (1 << 31),
};

typedef struct MD_OS_FileIter MD_OS_FileIter;
struct MD_OS_FileIter
{
	MD_OS_FileIterFlags flags;
	MD_U8               memory[800];
};

typedef struct MD_OS_FileInfo MD_OS_FileInfo;
struct MD_OS_FileInfo
{
	MD_String8        name;
	MD_FileProperties props;
};

// nick: on-disk file identifier
typedef struct MD_OS_FileID MD_OS_FileID;
struct MD_OS_FileID
{
	MD_U64 v[3];
};

////////////////////////////////
//~ rjf: Process Launch Parameters

typedef struct MD_OS_ProcessLaunchParams MD_OS_ProcessLaunchParams;
struct MD_OS_ProcessLaunchParams
{
	MD_String8List cmd_line;
	MD_String8     path;
	MD_String8List env;
	MD_B32         inherit_env;
	MD_B32         consoleless;
};

////////////////////////////////
//~ rjf: Handle Type

typedef struct MD_OS_Handle MD_OS_Handle;
struct MD_OS_Handle
{
	MD_U64 u64[1];
};

typedef struct MD_OS_HandleNode MD_OS_HandleNode;
struct MD_OS_HandleNode
{
	MD_OS_HandleNode* next;
	MD_OS_Handle      v;
};

typedef struct MD_OS_HandleList MD_OS_HandleList;
struct MD_OS_HandleList
{
	MD_OS_HandleNode* first;
	MD_OS_HandleNode* last;
	MD_U64            count;
};

typedef struct MD_OS_HandleArray MD_OS_HandleArray;
struct MD_OS_HandleArray
{
	MD_OS_Handle* v;
	MD_U64        count;
};

////////////////////////////////
//~ rjf: Globally Unique IDs

typedef struct MD_OS_Guid MD_OS_Guid;
struct MD_OS_Guid
{
	MD_U32 data1;
	MD_U16 data2;
	MD_U16 data3;
	MD_U8  data4[8];
};
md_static_assert(size_of(MD_OS_Guid) == 16, md_os_guid_check);

////////////////////////////////
//~ rjf: Thread Types

typedef void MD_OS_ThreadFunctionType(void *ptr);

////////////////////////////////
//~ rjf: Handle Type Functions (Helpers, Implemented Once)

md_force_inline MD_OS_Handle md_os_handle_zero (void)                     { MD_OS_Handle handle = {0}; return handle; }
md_force_inline MD_B32       md_os_handle_match(MD_OS_Handle a, MD_OS_Handle b) { return a.u64[0] == b.u64[0]; }

void           md_os_handle_list_push__arena      (MD_Arena*        arena, MD_OS_HandleList* handles, MD_OS_Handle handle);
void           md_os_handle_list_push__ainfo      (MD_AllocatorInfo ainfo, MD_OS_HandleList* handles, MD_OS_Handle handle);
MD_OS_HandleArray md_os_handle_array_from_list__arena(MD_Arena*        arena, MD_OS_HandleList* list);
MD_OS_HandleArray md_os_handle_array_from_list__ainfo(MD_AllocatorInfo ainfo, MD_OS_HandleList* list);

#define md_os_handle_ist_push(arena, handles, handle) _Generic(allocator, MD_Arena*: md_os_handle_list_push__arena,       MD_AllocatorInfo: md_os_handle_list_push__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, handles, handle)
#define md_os_handle_array_from_list(allocator, list) _Generic(allocator, MD_Arena*: md_os_handle_array_from_list__arena, MD_AllocatorInfo: md_os_handle_list_push__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, list)

md_force_inline void           md_os_handle_list_push__arena      (MD_Arena* arena, MD_OS_HandleList* handles, MD_OS_Handle handle) {        md_os_handle_list__push_ainfo      (md_arena_allocator(arena), handles, handle); }
md_force_inline MD_OS_HandleArray md_os_handle_array_from_list__arena(MD_Arena* arena, MD_OS_HandleList* list)                      { return md_os_handle_array_from_list__ainfo(md_arena_allocator(arena), list); }

inline void
md_os_handle_list_alloc(MD_AllocatorInfo ainfo, MD_OS_HandleList* handles, MD_OS_Handle handle) {
	MD_OS_HandleNode* n = md_alloc_array(ainfo, MD_OS_HandleNode, 1);
	n->v = handle;
	md_sll_queue_push(handles->first, handles->last, n);
	handles->count += 1;
}

inline MD_OS_HandleArray
md_os_handle_array_from_list_alloc(MD_AllocatorInfo ainfo, MD_OS_HandleList* list) {
	MD_OS_HandleArray result = {0};
	result.count = list->count;
	result.v     = md_alloc_array_no_zero(ainfo, MD_OS_Handle, result.count);
	MD_U64 idx = 0;
	for(MD_OS_HandleNode* n = list->first; n != 0; n = n->next, idx += 1) {
		result.v[idx] = n->v;
	}
	return result;
}

////////////////////////////////
//~ rjf: Command Line Argc/Argv Helper (Helper, Implemented Once)

MD_String8List md_os_string_list_from_argcv__arena(MD_Arena*        arena, int argc, char** argv);
MD_String8List md_os_string_list_from_argcv__ainfo(MD_AllocatorInfo ainfo, int argc, char** argv);

inline MD_String8List
md_os_string_list_from_argcv__ainfo(MD_AllocatorInfo ainfo, int argc, char** argv) {
	MD_String8List result = {0};
	for(int i = 0; i < argc; i += 1)
	{
		MD_String8 str = md_str8_cstring(argv[i]);
		md_str8_list_push(ainfo, &result, str);
	}
	return result;
}

#define md_os_string_list_from_argcv(allocator, argc, argv) _Generic(allocator, MD_Arena*: md_os_string_list_from_argcv__arena, MD_AllocatorInfo: md_os_string_list_from_argcv__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, argc, argv)

md_force_inline MD_String8List md_os_string_list_from_argcv__arena(MD_Arena* arena, int argc, char** argv) { return md_os_string_list_from_argcv__ainfo(md_arena_allocator(arena), argc, argv); }

////////////////////////////////
//~ rjf: @md_os_hooks File System (Implemented Per-OS)

//- rjf: files
MD_API MD_OS_Handle      md_os_file_open                 (MD_OS_AccessFlags flags, MD_String8 path);
MD_API void              md_os_file_close                (MD_OS_Handle file);
MD_API MD_U64            md_os_file_read                 (MD_OS_Handle file, MD_Rng1U64 rng, void *out_data);
MD_API MD_U64            md_os_file_write                (MD_OS_Handle file, MD_Rng1U64 rng, void *data);
MD_API MD_B32            md_os_file_set_times            (MD_OS_Handle file, MD_DateTime time);
MD_API MD_FileProperties md_os_properties_from_file      (MD_OS_Handle file);
MD_API MD_OS_FileID      md_os_id_from_file              (MD_OS_Handle file);
MD_API MD_B32            md_os_delete_file_at_path       (MD_String8   path);
MD_API MD_B32            md_os_copy_file_path            (MD_String8   dst,   MD_String8 src);
MD_API MD_B32            md_os_file_path_exists          (MD_String8   path);
MD_API MD_FileProperties md_os_properties_from_file_path (MD_String8   path);
MD_API MD_String8        md_os_full_path_from_path__arena(MD_Arena*        arena, MD_String8 path);
MD_API MD_String8        md_os_full_path_from_path__ainfo(MD_AllocatorInfo arena, MD_String8 path);

#define md_os_full_path_from_path(allocator, path) _Generic(allocator, MD_Arena*: md_os_full_path_from_path__arena, MD_AllocatorInfo: md_os_full_path_from_path__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, path)

md_force_inline MD_String8 md_os_full_path_from_path__arena(MD_Arena* arena, MD_String8 path) { return md_os_full_path_from_path__ainfo(md_arena_allocator(arena), path); }

//- rjf: file maps
MD_API MD_OS_Handle md_os_file_map_open      (MD_OS_AccessFlags flags, MD_OS_Handle file);
MD_API void         md_os_file_map_close     (MD_OS_Handle map);
MD_API void*        md_os_file_map_view_open (MD_OS_Handle map, MD_OS_AccessFlags flags, MD_Rng1U64 range);
MD_API void         md_os_file_map_view_close(MD_OS_Handle map, void* ptr, MD_Rng1U64 range);
   
//- rjf: directory iteration
       MD_OS_FileIter* md_os_file_iter_begin__arena(MD_Arena*        arena, MD_String8      path, MD_OS_FileIterFlags flags);
MD_API MD_OS_FileIter* md_os_file_iter_begin__ainfo(MD_AllocatorInfo ainfo, MD_String8      path, MD_OS_FileIterFlags flags);
       MD_B32          md_os_file_iter_next__arena (MD_Arena*        arena, MD_OS_FileIter* iter, MD_OS_FileInfo*     info_out);
MD_API MD_B32          md_os_file_iter_next__ainfo (MD_AllocatorInfo arena, MD_OS_FileIter* iter, MD_OS_FileInfo*     info_out);
MD_API void         md_os_file_iter_end         (                     MD_OS_FileIter* iter);

#define md_os_file_iter_begin(allocator, path, flags)   _Generic(allocator, MD_Arena*: md_os_file_iter_begin__arena, MD_AllocatorInfo: md_os_file_iter_begin__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, path)
#define md_os_file_iter_next(allocator, iter, info_out) _Generic(allocator, MD_Arena*: md_os_file_iter_next__arena,  MD_AllocatorInfo: md_os_file_iter_next__ainfo,  default: md_assert_generic_sel_fail) md_generic_call(allocator, path)

md_force_inline MD_OS_FileIter* md_os_file_iter_begin__arena(MD_Arena* arena, MD_String8      path, MD_OS_FileIterFlags flags)    { reutrn (); }
md_force_inline MD_B32          md_os_file_iter_next__arena (MD_Arena* arena, MD_OS_FileIter* iter, MD_OS_FileInfo*     info_out) { reutrn (); }

//- rjf: directory creation
MD_API MD_B32 md_os_make_directory(MD_String8 path);

////////////////////////////////
//~ rjf: Filesystem Helpers (Helpers, Implemented Once)

MD_API MD_B32       md_os_write_data_to_file_path     (MD_String8 path, MD_String8     data);
MD_API MD_B32       md_os_write_data_list_to_file_path(MD_String8 path, MD_String8List list);
MD_API MD_B32       md_os_append_data_to_file_path    (MD_String8 path, MD_String8     data);
       MD_OS_FileID md_os_id_from_file_path           (MD_String8 path);
       MD_S64       md_os_file_id_compare             (MD_OS_FileID a, MD_OS_FileID b);

       MD_String8 md_os_data_from_file_path__arena   (MD_Arena*        arena, MD_String8 path);
       MD_String8 md_os_data_from_file_path__ainfo   (MD_AllocatorInfo ainfo, MD_String8 path);
MD_API MD_String8 md_os_string_from_file_range__arena(MD_Arena*        arena, MD_OS_Handle file, MD_Rng1U64 range);
MD_API MD_String8 md_os_string_from_file_range__ainfo(MD_AllocatorInfo ainfo, MD_OS_Handle file, MD_Rng1U64 range);

#define md_os_data_from_file_path(allocator, path)           _Generic(allocator, MD_Arena*: md_os_data_from_file_path__arena,    MD_AllocatorInfo: md_os_data_from_file_path__ainfo,    default: md_assert_generic_sel_fail) md_generic_call(allocator, path)
#define md_os_string_from_file_range(allocator, file, range) _Generic(allocator, MD_Arena*: md_os_string_from_file_range__arena, MD_AllocatorInfo: md_os_string_from_file_range__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, file, range)

md_force_inline MD_String8 md_os_data_from_file_path__arena(MD_Arena* arena, MD_String8 path) { return md_os_data_from_file_path__ainfo(md_arena_allocator(arena), path); }

inline MD_String8
md_os_data_from_file_path__ainfo(MD_AllocatorInfo ainfo, MD_String8 path)
{
	MD_OS_Handle      file  = md_os_file_open(MD_OS_AccessFlag_Read | MD_OS_AccessFlag_ShareRead, path);
	MD_FileProperties props = md_os_properties_from_file(file);
	MD_String8        data  = md_os_string_from_file_range(ainfo, file, md_r1u64(0, props.size));
	md_os_file_close(file);
	return data;
}

inline MD_OS_FileID
md_os_id_from_file_path(MD_String8 path) {
	MD_OS_Handle file = md_os_file_open(MD_OS_AccessFlag_Read | MD_OS_AccessFlag_ShareRead, path);
	MD_OS_FileID id   = md_os_id_from_file(file);
	md_os_file_close(file);
	return id;
}

inline MD_S64 md_os_file_id_compare(MD_OS_FileID a, MD_OS_FileID b) { MD_S64 cmp = md_memory_compare((void*)&a.v[0], (void*)&b.v[0], sizeof(a.v)); return cmp; }

////////////////////////////////
//~ rjf: GUID Helpers (Helpers, Implemented Once)

MD_String8 md_os_string_from_guid__arena(MD_Arena*        arena, MD_OS_Guid guid);
MD_String8 md_os_string_from_guid__ainfo(MD_AllocatorInfo ainfo, MD_OS_Guid guid);

#define md_os_string_from_guid(allocator, guid) _Generic(allocator, MD_Arena*: md_os_string_from_guid__arena, MD_AllocatorInfo: md_os_string_from_guid__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, guid)

md_force_inline MD_String8 md_os_string_from_guid__arena(MD_Arena* arena, MD_OS_Guid guid) { md_os_string_from_guid__ainfo(md_arena_allocator(arena), guid); }

inline MD_String8
md_os_string_from_guid_alloc(MD_AllocatorInfo ainfo, MD_OS_Guid guid) {

	MD_String8 result = md_str8f(ainfo, 
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
//~ rjf: @md_os_hooks System/Process Info (Implemented Per-OS)

MD_API MD_OS_SystemInfo*  md_os_get_system_info (void);
MD_API MD_OS_ProcessInfo* md_os_get_process_info(void);

MD_String8 md_os_get_current_path__arena(MD_Arena*        arena);
MD_String8 md_os_get_current_path__ainfo(MD_AllocatorInfo arena);

#define md_os_get_current_path(allocator) _Generic(allocator, MD_Arena*: md_os_get_current_path__arena, MD_AllocatorInfo: md_os_get_current_path__ainfo) md_generic_call(allocator)

md_force_inline MD_String8 md_os_get_current_path__arena(MD_Arena* arena) { return md_os_get_current_path__ainfo(md_arena_allocator(arena)); }

////////////////////////////////
//~ rjf: @md_os_hooks Memory Allocation (Implemented Per-OS)

//- rjf: basic
void* md_os_reserve (           MD_U64 size);
MD_B32   md_os_commit  (void* ptr, MD_U64 size);
void  md_os_decommit(void* ptr, MD_U64 size);
void  md_os_release (void* ptr, MD_U64 size);

//- rjf: large pages
void* md_os_reserve_large(           MD_U64 size);
MD_B32   md_os_commit_large (void* ptr, MD_U64 size);

////////////////////////////////
//~ rjf: @md_os_hooks Thread Info (Implemented Per-OS)

MD_U32 md_os_tid(void);

MD_API void md_os_set_thread_name(MD_String8 string);

////////////////////////////////
//~ rjf: @md_os_hooks Aborting (Implemented Per-OS)

MD_API void md_os_abort(MD_S32 exit_code);

////////////////////////////////
//~ rjf: @md_os_hooks Shared Memory (Implemented Per-OS)

MD_API MD_OS_Handle md_os_shared_memory_alloc     (MD_U64       size, MD_String8 name);
MD_API MD_OS_Handle md_os_shared_memory_open      (MD_String8   name);
MD_API void      md_os_shared_memory_close     (MD_OS_Handle handle);
MD_API void*     md_os_shared_memory_view_open (MD_OS_Handle handle, MD_Rng1U64 range);
MD_API void      md_os_shared_memory_view_close(MD_OS_Handle handle, void* ptr, MD_Rng1U64 range);

////////////////////////////////
//~ rjf: @md_os_hooks Time (Implemented Per-OS)

MD_API MD_U64         md_os_now_microseconds         (void);
MD_API MD_U32         md_os_now_unix                 (void);
MD_API MD_DateTime    md_os_now_universal_time       (void);
MD_API MD_DateTime    md_os_universal_time_from_local(MD_DateTime* local_time);
MD_API MD_DateTime    md_os_local_time_from_universal(MD_DateTime* universal_time);
MD_API void        md_os_sleep_milliseconds       (MD_U32       msec);

////////////////////////////////
//~ rjf: @md_os_hooks Child Processes (Implemented Per-OS)

MD_API MD_OS_Handle md_os_process_launch(MD_OS_ProcessLaunchParams* params);
MD_API MD_B32       md_os_process_join  (MD_OS_Handle handle, MD_U64 endt_us);
MD_API void      md_os_process_detach(MD_OS_Handle handle);

////////////////////////////////
//~ rjf: @md_os_hooks Threads (Implemented Per-OS)

MD_API MD_OS_Handle md_os_thread_launch(MD_OS_ThreadFunctionType* func, void* ptr, void* params);
MD_API MD_B32       md_os_thread_join  (MD_OS_Handle handle, MD_U64 endt_us);
MD_API void      md_os_thread_detach(MD_OS_Handle handle);

////////////////////////////////
//~ rjf: @md_os_hooks Synchronization Primitives (Implemented Per-OS)

//- rjf: recursive mutexes
MD_API MD_OS_Handle md_os_mutex_alloc  (void);
MD_API void      md_os_mutex_release(MD_OS_Handle mutex);
MD_API void      md_os_mutex_take   (MD_OS_Handle mutex);
MD_API void      md_os_mutex_drop   (MD_OS_Handle mutex);

//- rjf: reader/writer mutexes
MD_API MD_OS_Handle md_os_rw_mutex_alloc  (void);
MD_API void      md_os_rw_mutex_release(MD_OS_Handle rw_mutex);
MD_API void      md_os_rw_mutex_take_r (MD_OS_Handle mutex);
MD_API void      md_os_rw_mutex_drop_r (MD_OS_Handle mutex);
MD_API void      md_os_rw_mutex_take_w (MD_OS_Handle mutex);
MD_API void      md_os_rw_mutex_drop_w (MD_OS_Handle mutex);

//- rjf: condition variables
MD_API MD_OS_Handle md_os_condition_variable_alloc  (void);
MD_API void      md_os_condition_variable_release(MD_OS_Handle cv);
// returns false on timeout, true on signal, (max_wait_ms = max_U64) -> no timeout
MD_API MD_B32       md_os_condition_variable_wait     (MD_OS_Handle cv, MD_OS_Handle mutex, MD_U64 endt_us);
MD_API MD_B32       md_os_condition_variable_wait_rw_r(MD_OS_Handle cv, MD_OS_Handle mutex_rw, MD_U64 endt_us);
MD_API MD_B32       md_os_condition_variable_wait_rw_w(MD_OS_Handle cv, MD_OS_Handle mutex_rw, MD_U64 endt_us);
MD_API void      md_os_condition_variable_signal   (MD_OS_Handle cv);
MD_API void      md_os_condition_variable_broadcast(MD_OS_Handle cv);

//- rjf: cross-process semaphores
MD_API MD_OS_Handle md_os_semaphore_alloc  (MD_U32       initial_count, MD_U32 max_count, MD_String8 name);
MD_API void      md_os_semaphore_release(MD_OS_Handle semaphore);
MD_API MD_OS_Handle md_os_semaphore_open   (MD_String8   name);
MD_API void      md_os_semaphore_close  (MD_OS_Handle semaphore);
MD_API MD_B32       md_os_semaphore_take   (MD_OS_Handle semaphore, MD_U64 endt_us);
MD_API void      md_os_semaphore_drop   (MD_OS_Handle semaphore);

//- rjf: scope macros
#define md_os_mutex_scope(mutex)            md_defer_loop( md_os_mutex_take     (mutex), md_os_mutex_drop     (mutex))
#define md_os_mutex_scope_r(mutex)          md_defer_loop( md_os_rw_mutex_take_r(mutex), md_os_rw_mutex_drop_r(mutex))
#define md_os_mutex_scope_W(mutex)          md_defer_loop( md_os_rw_mutex_take_w(mutex), md_os_rw_mutex_drop_w(mutex))
#define md_os_mutex_scope_rw_promote(mutex) md_defer_loop((md_os_rw_mutex_drop_r(mutex), md_os_rw_mutex_take_w(mutex)), (md_os_rw_mutex_drop_w(mutex), md_os_rw_mutex_take_r(mutex)))

////////////////////////////////
//~ rjf: @md_os_hooks Dynamically-Loaded Libraries (Implemented Per-OS)

MD_API MD_OS_Handle md_os_library_open     (MD_String8   path);
MD_API void      md_os_library_close    (MD_OS_Handle lib);
MD_API MD_VoidProc* md_os_library_load_proc(MD_OS_Handle lib, MD_String8 name);

////////////////////////////////
//~ rjf: @md_os_hooks Safe Calls (Implemented Per-OS)

MD_API void md_os_safe_call(MD_OS_ThreadFunctionType* func, MD_OS_ThreadFunctionType* fail_handler, void* ptr);

////////////////////////////////
//~ rjf: @md_os_hooks GUIDs (Implemented Per-OS)

MD_API MD_OS_Guid md_os_make_guid(void);

////////////////////////////////
//~ rjf: @md_os_hooks Entry Points (Implemented Per-OS)

// NOTE(rjf): The implementation of `md_os_core` will define low-level entry
// points if MD_BUILD_ENTRY_DEFINING_UNIT is defined to 1. These will call
// into the standard codebase program entry points, named "md_entry_point".

#if MD_BUILD_ENTRY_DEFINING_UNIT
void md_entry_point(MD_CmdLine* cmdline);
#endif

////////////////////////////////
//~ Ed: Manual OS Bootstrap (Implemented Per-OS)

typedef struct MD_OS_Context MD_OS_Context;
struct MD_OS_Context
{
	MD_Arena* state_arena;
	MD_Arena* entity_arena;
	MD_B32    enable_large_pages;
};

// OS layer initialization
MD_API void md_os_init(MD_OS_Context* ctx, MD_TCTX* thread_ctx);
