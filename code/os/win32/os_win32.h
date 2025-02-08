#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/base.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Includes / Libraries

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#define WIN32_MEAN_AND_LEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <windowsx.h>
#include <timeapi.h>
#include <tlhelp32.h>
#include <Shlobj.h>
#include <processthreadsapi.h>
#pragma comment(lib, "user32")
#pragma comment(lib, "winmm")
#pragma comment(lib, "shell32")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "rpcrt4")
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "comctl32")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") // this is required for loading correct comctl32 dll file
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN
#undef WIN32_MEAN_AND_LEAN
#undef VC_EXTRALEAN

////////////////////////////////
//~ rjf: File Iterator Types

typedef struct OS_W32_FileIter OS_W32_FileIter;
struct OS_W32_FileIter
{
	HANDLE           handle;
	WIN32_FIND_DATAW find_data;
	B32              is_volume_iter;
	String8Array     drive_strings;
	U64              drive_strings_iter_idx;
};
md_static_assert(sizeof(member(OS_FileIter, memory)) >= sizeof(OS_W32_FileIter), file_iter_memory_size);

////////////////////////////////
//~ rjf: Entity Types

typedef enum OS_W32_EntityKind OS_W32_EntityKind;
enum OS_W32_EntityKind
{
	OS_W32_EntityKind_Null,
	OS_W32_EntityKind_Thread,
	OS_W32_EntityKind_Mutex,
	OS_W32_EntityKind_RWMutex,
	OS_W32_EntityKind_ConditionVariable,
};

typedef struct OS_W32_EntityThread OS_W32_EntityThread;
struct OS_W32_EntityThread
{
	OS_ThreadFunctionType* func;
	void*                  ptr;
	HANDLE                 handle;
	DWORD                  tid;
};

typedef struct OS_W32_Entity OS_W32_Entity;
struct OS_W32_Entity
{
	OS_W32_Entity*    next;
	OS_W32_EntityKind kind;
	union
	{
		OS_W32_EntityThread thread;
		CRITICAL_SECTION    mutex;
		SRWLOCK             rw_mutex;
		CONDITION_VARIABLE  cv;
	};
};

////////////////////////////////
//~ rjf: State

typedef struct OS_W32_State OS_W32_State;
struct OS_W32_State
{
	Arena* arena;
	
	// rjf: info
	OS_SystemInfo  system_info;
	OS_ProcessInfo process_info;
	U64 microsecond_resolution;
	
	// rjf: entity storage
	CRITICAL_SECTION entity_mutex;
	Arena*           entity_arena;
	OS_W32_Entity*   entity_free;
};

////////////////////////////////
//~ rjf: Globals

MD_API extern OS_W32_State os_w32_state = {0};

////////////////////////////////
//~ rjf: File Info Conversion Helpers

inline FilePropertyFlags
os_w32_file_property_flags_from_dwFileAttributes(DWORD dwFileAttributes)
{
	FilePropertyFlags flags = 0;
	if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { 
		flags |= FilePropertyFlag_IsFolder;
	}
	return flags;
}

inline void
os_w32_file_properties_from_attribute_data(FileProperties* properties, WIN32_FILE_ATTRIBUTE_DATA* attributes) {
	properties->size = compose_64bit(attributes->nFileSizeHigh, attributes->nFileSizeLow);
	os_w32_dense_time_from_file_time(&properties->created, &attributes->ftCreationTime);
	os_w32_dense_time_from_file_time(&properties->modified, &attributes->ftLastWriteTime);
	properties->flags = os_w32_file_property_flags_from_dwFileAttributes(attributes->dwFileAttributes);
}

////////////////////////////////
//~ rjf: Time Conversion Helpers

       void os_w32_date_time_from_system_time(DateTime*   out, SYSTEMTIME* in);
       void os_w32_system_time_from_date_time(SYSTEMTIME* out, DateTime*   in);
       void os_w32_dense_time_from_file_time (DenseTime*  out, FILETIME*   in);
MD_API U32  os_w32_sleep_ms_from_endt_us(U64 endt_us);

inline void
os_w32_date_time_from_system_time(DateTime* out, SYSTEMTIME* in)
{
	out->year    = in->wYear;
	out->mon     = in->wMonth - 1;
	out->wday    = in->wDayOfWeek;
	out->day     = in->wDay;

	out->hour    = in->wHour;
	out->min     = in->wMinute;
	out->sec     = in->wSecond;
	out->msec    = in->wMilliseconds;
}

inline void
os_w32_system_time_from_date_time(SYSTEMTIME* out, DateTime* in)
{
	out->wYear         = (WORD)(in->year);
	out->wMonth        = in->mon + 1;
	out->wDay          = in->day;

	out->wHour         = in->hour;
	out->wMinute       = in->min;
	out->wSecond       = in->sec;
	out->wMilliseconds = in->msec;
}

inline void
os_w32_dense_time_from_file_time(DenseTime* out, FILETIME* in) {
	SYSTEMTIME systime   = {0};
	DateTime   date_time = {0};
	FileTimeToSystemTime(in, &systime);
	os_w32_date_time_from_system_time(&date_time, &systime);
	*out = dense_time_from_date_time(date_time);
}

////////////////////////////////
//~ rjf: Entity Functions

MD_API OS_W32_Entity* os_w32_entity_alloc  (OS_W32_EntityKind kind);
MD_API void           os_w32_entity_release(OS_W32_Entity*    entity);

////////////////////////////////
//~ rjf: Thread Entry Point

MD_API DWORD os_w32_thread_entry_point(void* ptr);

////////////////////////////////
//~ rjf: @os_hooks System/Process Info (Implemented Per-OS)

inline String8
os_get_current_path(Arena* arena) {
	String8 name;
	TempArena scratch = scratch_begin(&arena, 1);
	{
		DWORD   length  = GetCurrentDirectoryW(0, 0);
		U16*    memory  = push_array_no_zero(scratch.arena, U16, length + 1);
				length  = GetCurrentDirectoryW(length + 1, (WCHAR*)memory);
		        name    = str8_from_16(arena, str16(memory, length));
	}
	scratch_end(scratch);
	return name;
}

inline String8
os_get_current_path(AllocatorInfo ainfo) {
	String8 name;
	// TODO(Ed): Review
	TempArena scratch = scratch_begin(0, 0);
	{
		DWORD   length  = GetCurrentDirectoryW(0, 0);
		U16*    memory  = push_array_no_zero(scratch.arena, U16, length + 1);
				length  = GetCurrentDirectoryW(length + 1, (WCHAR*)memory);
		        name    = str8_from_16_alloc(ainfo, str16(memory, length));
	}
	scratch_end(scratch);
	return name;
}

////////////////////////////////
//~ rjf: @os_hooks Memory Allocation (Implemented Per-OS)

//- rjf: basic

inline void* os_reserve (           U64 size) { void* result =  VirtualAlloc(  0, size, MEM_RESERVE, PAGE_READWRITE);       return result; }
inline B32   os_commit  (void* ptr, U64 size) { B32   result = (VirtualAlloc(ptr, size, MEM_COMMIT,  PAGE_READWRITE) != 0); return result; }
inline void  os_decommit(void* ptr, U64 size) {                 VirtualFree (ptr, size, MEM_DECOMMIT); }

inline void
os_release(void* ptr, U64 size) { 
	// NOTE(rjf): size not used - not necessary on Windows, but necessary for other OSes.
	VirtualFree(ptr, 0, MEM_RELEASE);
}

//- rjf: large pages

inline void*
os_reserve_large(U64 size) { 
	// we commit on reserve because windows
	void* result = VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT|MEM_LARGE_PAGES, PAGE_READWRITE);
	return result;
}

inline B32 os_commit_large(void *ptr, U64 size) { return 1; } 

////////////////////////////////
//~ rjf: @os_hooks Thread Info (Implemented Per-OS)

inline U32 os_tid(void) { DWORD id = GetCurrentThreadId(); return (U32)id; }
