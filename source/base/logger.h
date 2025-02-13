#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#	include "strings.h"
#	include "arena.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: MD_Log Types

typedef enum MD_LogMsgKind MD_LogMsgKind;
enum MD_LogMsgKind
{
	MD_LogMsgKind_Info,
	MD_LogMsgKind_UserError,
	MD_LogMsgKind_COUNT
};

typedef struct MD_LogScope MD_LogScope;
struct MD_LogScope
{
	MD_LogScope*   next;
	MD_U64         pos;
	MD_String8List strings[MD_LogMsgKind_COUNT];
};

typedef struct MD_LogScopeResult MD_LogScopeResult;
struct MD_LogScopeResult
{
	MD_String8 strings[MD_LogMsgKind_COUNT];
};

typedef struct MD_Log MD_Log;
struct MD_Log
{
	MD_Arena*    arena;
	MD_LogScope* top_scope;
};

#ifndef MD_LOG_DEFAULT_ARENA_BLOCK_SIZE
#define MD_LOG_DEFAULT_ARENA_BLOCK_SIZE MD_VARENA_DEFAULT_RESERVE
#endif

////////////////////////////////
//~ rjf: MD_Log Creation/Selection

		MD_Log* md_log_alloc(MD_AllocatorInfo ainfo, MD_U64 md_arena_block_size);
       void     md_log_release(MD_Log* log);
MD_API void     md_log_select (MD_Log* log);

inline MD_Log*
md_log_alloc(MD_AllocatorInfo ainfo, MD_U64 md_arena_block_size) {
	if (md_arena_block_size == 0) {
		md_arena_block_size = MD_LOG_DEFAULT_ARENA_BLOCK_SIZE;
	}
	MD_Arena* arena = md_arena_alloc(.backing = ainfo, .block_size = md_arena_block_size);
	MD_Log*   log   = md_push_array(arena, MD_Log, 1);
	log->arena   = arena;
  return log;
}

inline void md_log_release(MD_Log* log) { md_arena_release(log->arena); }

////////////////////////////////
//~ rjf: MD_Log Building

MD_API void md_log_msg (MD_LogMsgKind kind, MD_String8 string);
MD_API void md_log_msgf(MD_LogMsgKind kind, char* fmt, ...);

#define md_log_info(s)               md_log_msg (MD_LogMsgKind_Info,      (s))
#define md_log_infof(fmt, ...)       md_log_msgf(MD_LogMsgKind_Info,      (fmt), __VA_ARGS__)
#define md_log_user_error(s)         md_log_msg (MD_LogMsgKind_UserError, (s))
#define md_log_user_errorf(fmt, ...) md_log_msgf(MD_LogMsgKind_UserError, (fmt), __VA_ARGS__)

#define md_log_info_named_block(s)         md_defer_loop(md_log_infof("%S:\n{\n", (s)), md_log_infof("}\n"))
#define md_log_info_named_blockf(fmt, ...) md_defer_loop((md_log_infof(fmt, __VA_ARGS__), md_log_infof(":\n{\n")), md_log_infof("}\n"))

////////////////////////////////
//~ rjf: MD_Log Scopes

       void           md_log_scope_begin(void);
MD_API MD_LogScopeResult md_log_scope_end(MD_Arena* arena);
