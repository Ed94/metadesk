#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/strings.h"
#	include "base/thread_context.h"
#	include "os.h"
#	include "os_win32_includes.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: File Iterator Types

typedef struct MD_OS_W32_FileIter MD_OS_W32_FileIter;
struct MD_OS_W32_FileIter
{
	HANDLE           handle;
	WIN32_FIND_DATAW find_data;
	MD_B32              is_volume_iter;
	MD_String8Array     drive_strings;
	MD_U64              drive_strings_iter_idx;
};
md_static_assert(sizeof(md_member(MD_OS_FileIter, memory)) >= sizeof(MD_OS_W32_FileIter), file_iter_memory_size);

////////////////////////////////
//~ rjf: Entity Types

typedef enum MD_OS_W32_EntityKind MD_OS_W32_EntityKind;
enum MD_OS_W32_EntityKind
{
	MD_OS_W32_EntityKind_Null,
	MD_OS_W32_EntityKind_Thread,
	MD_OS_W32_EntityKind_Mutex,
	MD_OS_W32_EntityKind_RWMutex,
	MD_OS_W32_EntityKind_ConditionVariable,
};

typedef struct MD_OS_W32_EntityThread MD_OS_W32_EntityThread;
struct MD_OS_W32_EntityThread
{
	MD_OS_ThreadFunctionType* func;
	void*                  ptr;
	HANDLE                 handle;
	DWORD                  tid;
};

typedef struct MD_OS_W32_Entity MD_OS_W32_Entity;
struct MD_OS_W32_Entity
{
	MD_OS_W32_Entity*    next;
	MD_OS_W32_EntityKind kind;
	union
	{
		MD_OS_W32_EntityThread thread;
		CRITICAL_SECTION    mutex;
		SRWLOCK             rw_mutex;
		CONDITION_VARIABLE  cv;
	};
};

////////////////////////////////
//~ rjf: State

typedef struct MD_OS_W32_State MD_OS_W32_State;
struct MD_OS_W32_State
{
	MD_Arena* arena;
	
	// rjf: info
	MD_OS_SystemInfo  system_info;
	MD_OS_ProcessInfo process_info;
	MD_U64 microsecond_resolution;
	
	// rjf: entity storage
	CRITICAL_SECTION entity_mutex;
	MD_Arena*           entity_arena;
	MD_OS_W32_Entity*   entity_free;
};

////////////////////////////////
//~ rjf: Globals

MD_API extern MD_OS_W32_State md_os_w32_state;


////////////////////////////////
//~ rjf: Time Conversion Helpers

       void md_os_w32_date_time_from_system_time(MD_DateTime*   out, SYSTEMTIME* in);
       void md_os_w32_system_time_from_date_time(SYSTEMTIME* out, MD_DateTime*   in);
       void md_os_w32_dense_time_from_file_time (MD_DenseTime*  out, FILETIME*   in);
MD_API MD_U32  md_os_w32_sleep_ms_from_endt_us(MD_U64 endt_us);

inline void
md_os_w32_date_time_from_system_time(MD_DateTime* out, SYSTEMTIME* in)
{
	out->year    = in->wYear;
	out->mon     = in->wMonth - 1;
	out->wday    = in->wDayOfWeek;
	out->day     = in->wDay;

	out->hour    = in->wHour;
	out->md_min     = in->wMinute;
	out->sec     = in->wSecond;
	out->msec    = in->wMilliseconds;
}

inline void
md_os_w32_system_time_from_date_time(SYSTEMTIME* out, MD_DateTime* in)
{
	out->wYear         = (WORD)(in->year);
	out->wMonth        = in->mon + 1;
	out->wDay          = in->day;

	out->wHour         = in->hour;
	out->wMinute       = in->md_min;
	out->wSecond       = in->sec;
	out->wMilliseconds = in->msec;
}

inline void
md_os_w32_dense_time_from_file_time(MD_DenseTime* out, FILETIME* in) {
	SYSTEMTIME systime   = {0};
	MD_DateTime   date_time = {0};
	FileTimeToSystemTime(in, &systime);
	md_os_w32_date_time_from_system_time(&date_time, &systime);
	*out = md_dense_time_from_date_time(date_time);
}

////////////////////////////////
//~ rjf: File Info Conversion Helpers

inline MD_FilePropertyFlags
md_os_w32_file_property_flags_from_dwFileAttributes(DWORD dwFileAttributes)
{
	MD_FilePropertyFlags flags = 0;
	if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { 
		flags |= MD_FilePropertyFlag_IsFolder;
	}
	return flags;
}

inline void
md_os_w32_file_properties_from_attribute_data(MD_FileProperties* properties, WIN32_FILE_ATTRIBUTE_DATA* attributes) {
	properties->size = md_compose_64bit(attributes->nFileSizeHigh, attributes->nFileSizeLow);
	md_os_w32_dense_time_from_file_time(&properties->created, &attributes->ftCreationTime);
	md_os_w32_dense_time_from_file_time(&properties->modified, &attributes->ftLastWriteTime);
	properties->flags = md_os_w32_file_property_flags_from_dwFileAttributes(attributes->dwFileAttributes);
}

////////////////////////////////
//~ rjf: Entity Functions

MD_API MD_OS_W32_Entity* md_os_w32_entity_alloc  (MD_OS_W32_EntityKind kind);
MD_API void           md_os_w32_entity_release(MD_OS_W32_Entity*    entity);

////////////////////////////////
//~ rjf: Thread Entry Point

MD_API DWORD md_os_w32_thread_entry_point(void* ptr);

////////////////////////////////
//~ rjf: @md_os_hooks System/Process Info (Implemented Per-OS)

inline MD_String8
md_os_get_current_path__ainfo(MD_AllocatorInfo ainfo) {
	MD_String8 name;
	MD_TempArena scratch = md_scratch_begin(ainfo);
	{
		DWORD   length  = GetCurrentDirectoryW(0, 0);
		MD_U16*    memory  = md_push_array__no_zero(scratch.arena, MD_U16, length + 1);
				length  = GetCurrentDirectoryW(length + 1, (WCHAR*)memory);
		        name    = md_str8_from(ainfo, md_str16(memory, length));
	}
	scratch_end(scratch);
	return name;
}

////////////////////////////////
//~ rjf: @md_os_hooks Memory Allocation (Implemented Per-OS)

//- rjf: basic

inline void* md_os_reserve (           MD_U64 size) { void* result =  VirtualAlloc(  0, size, MEM_RESERVE, PAGE_READWRITE);       return result; }
inline MD_B32   md_os_commit  (void* ptr, MD_U64 size) { MD_B32   result = (VirtualAlloc(ptr, size, MEM_COMMIT,  PAGE_READWRITE) != 0); return result; }
inline void  md_os_decommit(void* ptr, MD_U64 size) {                 VirtualFree (ptr, size, MEM_DECOMMIT); }

inline void
md_os_release(void* ptr, MD_U64 size) { 
	// NOTE(rjf): size not used - not necessary on Windows, but necessary for other OSes.
	VirtualFree(ptr, 0, MEM_RELEASE);
}

//- rjf: large pages

inline void*
md_os_reserve_large(MD_U64 size) { 
	// we commit on reserve because windows
	void* result = VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT|MEM_LARGE_PAGES, PAGE_READWRITE);
	return result;
}

inline MD_B32 md_os_commit_large(void* ptr, MD_U64 size) { return 1; } 

////////////////////////////////
//~ rjf: @md_os_hooks Thread Info (Implemented Per-OS)

inline MD_U32 md_os_tid(void) { DWORD id = GetCurrentThreadId(); return (MD_U32)id; }
