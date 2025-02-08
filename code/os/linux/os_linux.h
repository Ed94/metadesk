#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/base.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Includes

#define _GNU_SOURCE
#include <features.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <time.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <signal.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/sysinfo.h>
#include <sys/random.h>

int pthread_setname_np(pthread_t thread, const char* name);
int pthread_getname_np(pthread_t thread, char*       name, size_t size);

typedef struct tm       tm;
typedef struct timespec timespec;

////////////////////////////////
//~ rjf: File Iterator

typedef struct OS_LNX_FileIter OS_LNX_FileIter;
struct OS_LNX_FileIter
{
	DIR*           dir;
	struct dirent* dp;
	String8        path;
};
md_assert(sizeof(Member(OS_FileIter, memory)) >= sizeof(OS_LNX_FileIter), os_lnx_file_iter_size_check);

////////////////////////////////
//~ rjf: Safe Call Handler Chain

typedef struct OS_LNX_SafeCallChain OS_LNX_SafeCallChain;
struct OS_LNX_SafeCallChain
{
	OS_LNX_SafeCallChain*  next;
	OS_ThreadFunctionType* fail_handler;
	void *ptr;
};

////////////////////////////////
//~ rjf: Entities

typedef enum OS_LNX_EntityKind OS_LNX_EntityKind
enum OS_LNX_EntityKind
{
	OS_LNX_EntityKind_Thread,
	OS_LNX_EntityKind_Mutex,
	OS_LNX_EntityKind_RWMutex,
	OS_LNX_EntityKind_ConditionVariable,
};

typedef struct OS_LNX_EntityThread OS_LNX_EntityThread;
struct OS_LNX_EntityThread
{
	pthread_t              handle;
	OS_ThreadFunctionType* func;
	void*                  ptr;
};

typedef struct OS_LNX_Entity OS_LNX_Entity;
struct OS_LNX_Entity
{
	OS_LNX_Entity*    next;
	OS_LNX_EntityKind kind;
	union
	{
		OS_LNX_EntityThread thread;
		pthread_mutex_t  mutex_handle;
		pthread_rwlock_t rwmutex_handle;
		struct {
			pthread_cond_t  cond_handle;
			pthread_mutex_t rwlock_mutex_handle;
		} cv;
	};
};

////////////////////////////////
//~ rjf: State

typedef struct OS_LNX_State OS_LNX_State;
struct OS_LNX_State
{
	Arena*          arena;
	OS_SystemInfo   system_info;
	OS_ProcessInfo  process_info;
	pthread_mutex_t entity_mutex;
	Arena*          entity_arena;
	OS_LNX_Entity*  entity_free;
};


////////////////////////////////
//~ rjf: Helpers

DateTime       os_lnx_date_time_from_tm        (tm in, U32 msec);
tm             os_lnx_tm_from_date_time        (DateTime dt);
timespec       os_lnx_timespec_from_date_time  (DateTime dt);
DenseTime      os_lnx_dense_time_from_timespec (timespec in);
FileProperties os_lnx_file_properties_from_stat(struct stat* s);
void           os_lnx_safe_call_sig_handler    (int x);

inline DateTime
os_lnx_date_time_from_tm(tm in, U32 msec) {
	DateTime dt = {0};
	dt.sec  = in.tm_sec;
	dt.min  = in.tm_min;
	dt.hour = in.tm_hour;

	dt.day  = in.tm_mday-1;
	dt.mon  = in.tm_mon;
	dt.year = in.tm_year+1900;
	dt.msec = msec;
	return dt;
}

inline tm
os_lnx_tm_from_date_time(DateTime dt) {
  tm result = {0};
  result.tm_sec = dt.sec;
  result.tm_min = dt.min;
  result.tm_hour= dt.hour;

  result.tm_mday= dt.day+1;
  result.tm_mon = dt.mon;
  result.tm_year= dt.year-1900;
  return result;
}

inline timespec
os_lnx_timespec_from_date_time(DateTime dt) {
  tm tm_val = os_lnx_tm_from_date_time(dt);
  time_t seconds = timegm(&tm_val);
  timespec result = {0};
  result.tv_sec = seconds;
  return result;
}

inline DenseTime
os_lnx_dense_time_from_timespec(timespec in) {
  DenseTime result = 0; {
    struct tm tm_time = {0};
    gmtime_r(&in.tv_sec, &tm_time);
    DateTime date_time = os_lnx_date_time_from_tm(tm_time, in.tv_nsec/Million(1));
    result = dense_time_from_date_time(date_time);
  }
  return result;
}

inline FileProperties
os_lnx_file_properties_from_stat(struct stat* s) {
	FileProperties props = {0};
	props.size     = s->st_size;
	props.created  = os_lnx_dense_time_from_timespec(s->st_ctim);
	props.modified = os_lnx_dense_time_from_timespec(s->st_mtim);
	if (s->st_mode & S_IFDIR) {
		props.flags |= FilePropertyFlag_IsFolder;
	}
	return props;
}

inline void
os_lnx_safe_call_sig_handler(int x) {
	OS_LNX_SafeCallChain* chain = os_lnx_safe_call_chain;
	if (chain != 0 && chain->fail_handler != 0) {
		chain->fail_handler(chain->ptr);
	}
	abort();
}

////////////////////////////////
//~ rjf: Entities

MD_API OS_LNX_Entity* os_lnx_entity_alloc  (OS_LNX_EntityKind kind);
MD_API void           os_lnx_entity_release(OS_LNX_Entity* entity);

////////////////////////////////
//~ rjf: Thread Entry Point

void* os_lnx_thread_entry_point(void* ptr);

////////////////////////////////
//~ rjf: @os_hooks System/Process Info (Implemented Per-OS)

String8
os_get_current_path(Arena* arena) {
	char*   cwdir  = getcwd(0, 0);
	String8 string = push_str8_copy(arena, str8_cstring(cwdir));
	return  string;
}

////////////////////////////////
//~ rjf: @os_hooks Memory Allocation (Implemented Per-OS)

//- rjf: basic

inline void* os_reserve (           U64 size) { void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); return result; }
inline B32   os_commit  (void *ptr, U64 size) { mprotect(ptr, size, PROT_READ | PROT_WRITE);                                 return 1; }
inline void  os_decommit(void *ptr, U64 size) { madvise(ptr, size, MADV_DONTNEED); mprotect(ptr, size, PROT_NONE); }
inline void  os_release (void *ptr, U64 size) { munmap(ptr, size); } 

//- rjf: large pages

inline void* os_reserve_large(           U64 size) { void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0); return result; }
inline B32   os_commit_large (void *ptr, U64 size) { mprotect(ptr, size, PROT_READ | PROT_WRITE);                                               return 1; }

////////////////////////////////
//~ rjf: @os_hooks Thread Info (Implemented Per-OS)

inline U32
os_tid(void) {
	U32 result = 0;
#if defined(SYS_gettid)
	result = syscall(SYS_gettid);
#else
	result = gettid();
#endif
	return result;
}

////////////////////////////////
//~ rjf: @os_hooks Aborting (Implemented Per-OS)

inline void os_abort(S32 exit_code) { exit(exit_code); }


