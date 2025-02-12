#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/debug.h"
#	include "base/strings.h"
#	include "base/thread_context.h"
#	include "os/os.h"
#	include "os/linux/os_linux_includes.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

int pthread_setname_np(pthread_t thread, const char* name);
int pthread_getname_np(pthread_t thread, char*       name, size_t size);

typedef struct tm       tm;
typedef struct timespec timespec;

////////////////////////////////
//~ rjf: File Iterator

typedef struct MD_OS_LNX_FileIter MD_OS_LNX_FileIter;
struct MD_OS_LNX_FileIter
{
	DIR*           dir;
	struct dirent* dp;
	MD_String8        path;
};
md_assert(sizeof(Member(MD_OS_FileIter, memory)) >= sizeof(MD_OS_LNX_FileIter), md_os_lnx_file_iter_size_check);

////////////////////////////////
//~ rjf: Safe Call Handler Chain

typedef struct MD_OS_LNX_SafeCallChain MD_OS_LNX_SafeCallChain;
struct MD_OS_LNX_SafeCallChain
{
	MD_OS_LNX_SafeCallChain*  next;
	MD_OS_ThreadFunctionType* fail_handler;
	void *ptr;
};

////////////////////////////////
//~ rjf: Entities

typedef enum MD_OS_LNX_EntityKind MD_OS_LNX_EntityKind
enum MD_OS_LNX_EntityKind
{
	MD_OS_LNX_EntityKind_Thread,
	MD_OS_LNX_EntityKind_Mutex,
	MD_OS_LNX_EntityKind_RWMutex,
	MD_OS_LNX_EntityKind_ConditionVariable,
};

typedef struct MD_OS_LNX_EntityThread MD_OS_LNX_EntityThread;
struct MD_OS_LNX_EntityThread
{
	pthread_t              handle;
	MD_OS_ThreadFunctionType* func;
	void*                  ptr;
};

typedef struct MD_OS_LNX_Entity MD_OS_LNX_Entity;
struct MD_OS_LNX_Entity
{
	MD_OS_LNX_Entity*    next;
	MD_OS_LNX_EntityKind kind;
	union
	{
		MD_OS_LNX_EntityThread thread;
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

typedef struct MD_OS_LNX_State MD_OS_LNX_State;
struct MD_OS_LNX_State
{
	MD_Arena*          arena;
	MD_OS_SystemInfo   system_info;
	MD_OS_ProcessInfo  process_info;
	pthread_mutex_t entity_mutex;
	MD_Arena*          entity_arena;
	MD_OS_LNX_Entity*  entity_free;
};


////////////////////////////////
//~ rjf: Helpers

MD_DateTime       md_os_lnx_date_time_from_tm        (tm in, MD_U32 msec);
tm             md_os_lnx_tm_from_date_time        (MD_DateTime dt);
timespec       md_os_lnx_timespec_from_date_time  (MD_DateTime dt);
MD_DenseTime      md_os_lnx_dense_time_from_timespec (timespec in);
MD_FileProperties md_os_lnx_file_properties_from_stat(struct stat* s);
void           md_os_lnx_safe_call_sig_handler    (int x);

inline MD_DateTime
md_os_lnx_date_time_from_tm(tm in, MD_U32 msec) {
	MD_DateTime dt = {0};
	dt.sec  = in.tm_sec;
	dt.md_min  = in.tm_min;
	dt.hour = in.tm_hour;

	dt.day  = in.tm_mday-1;
	dt.mon  = in.tm_mon;
	dt.year = in.tm_year+1900;
	dt.msec = msec;
	return dt;
}

inline tm
md_os_lnx_tm_from_date_time(MD_DateTime dt) {
  tm result = {0};
  result.tm_sec = dt.sec;
  result.tm_min = dt.md_min;
  result.tm_hour= dt.hour;

  result.tm_mday= dt.day+1;
  result.tm_mon = dt.mon;
  result.tm_year= dt.year-1900;
  return result;
}

inline timespec
md_os_lnx_timespec_from_date_time(MD_DateTime dt) {
  tm tm_val = md_os_lnx_tm_from_date_time(dt);
  time_t seconds = timegm(&tm_val);
  timespec result = {0};
  result.tv_sec = seconds;
  return result;
}

inline MD_DenseTime
md_os_lnx_dense_time_from_timespec(timespec in) {
  MD_DenseTime result = 0; {
    struct tm tm_time = {0};
    gmtime_r(&in.tv_sec, &tm_time);
    MD_DateTime date_time = md_os_lnx_date_time_from_tm(tm_time, in.tv_nsec/Million(1));
    result = md_dense_time_from_date_time(date_time);
  }
  return result;
}

inline MD_FileProperties
md_os_lnx_file_properties_from_stat(struct stat* s) {
	MD_FileProperties props = {0};
	props.size     = s->st_size;
	props.created  = md_os_lnx_dense_time_from_timespec(s->st_ctim);
	props.modified = md_os_lnx_dense_time_from_timespec(s->st_mtim);
	if (s->st_mode & S_IFDIR) {
		props.flags |= MD_FilePropertyFlag_IsFolder;
	}
	return props;
}

inline void
md_os_lnx_safe_call_sig_handler(int x) {
	MD_OS_LNX_SafeCallChain* chain = md_os_lnx_safe_call_chain;
	if (chain != 0 && chain->fail_handler != 0) {
		chain->fail_handler(chain->ptr);
	}
	abort();
}

////////////////////////////////
//~ rjf: Entities

MD_API MD_OS_LNX_Entity* md_os_lnx_entity_alloc  (MD_OS_LNX_EntityKind kind);
MD_API void           md_os_lnx_entity_release(MD_OS_LNX_Entity*    entity);

////////////////////////////////
//~ rjf: Thread Entry Point

MD_API void* md_os_lnx_thread_entry_point(void* ptr);

////////////////////////////////
//~ rjf: @md_os_hooks System/Process Info (Implemented Per-OS)

inline MD_String8
md_os_get_current_path__ainfo(MD_AllocatorInfo ainfo) {
	char*   cwdir  = getcwd(0, 0);
	MD_String8 string = md_str8_copy(ainfo, md_str8_cstring(cwdir));
	return  string;
}

////////////////////////////////
//~ rjf: @md_os_hooks Memory Allocation (Implemented Per-OS)

//- rjf: basic

inline void* md_os_reserve (           MD_U64 size) { void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); return result; }
inline MD_B32   md_os_commit  (void *ptr, MD_U64 size) { mprotect(ptr, size, PROT_READ | PROT_WRITE);                                 return 1; }
inline void  md_os_decommit(void *ptr, MD_U64 size) { madvise(ptr, size, MADV_DONTNEED); mprotect(ptr, size, PROT_NONE); }
inline void  md_os_release (void *ptr, MD_U64 size) { munmap(ptr, size); } 

//- rjf: large pages

inline void* md_os_reserve_large(           MD_U64 size) { void* result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0); return result; }
inline MD_B32   md_os_commit_large (void *ptr, MD_U64 size) { mprotect(ptr, size, PROT_READ | PROT_WRITE);                                               return 1; }

////////////////////////////////
//~ rjf: @md_os_hooks Thread Info (Implemented Per-OS)

inline MD_U32
md_os_tid(void) {
	MD_U32 result = 0;
#if defined(SYS_gettid)
	result = syscall(SYS_gettid);
#else
	result = gettid();
#endif
	return result;
}
