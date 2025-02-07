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
//~ rjf: Log Types

typedef enum LogMsgKind LogMsgKind;
enum LogMsgKind
{
	LogMsgKind_Info,
	LogMsgKind_UserError,
	LogMsgKind_COUNT
};

typedef struct LogScope LogScope;
struct LogScope
{
	LogScope*   next;
	U64         pos;
	String8List strings[LogMsgKind_COUNT];
};

typedef struct LogScopeResult LogScopeResult;
struct LogScopeResult
{
	String8 strings[LogMsgKind_COUNT];
};

typedef struct Log Log;
struct Log
{
	Arena*    arena;
	LogScope* top_scope;
};

#ifndef LOG_DEFAULT_ARENA_BLOCK_SIZE
#define LOG_DEFAULT_ARENA_BLOCK_SIZE VARENA_DEFAULT_RESERVE
#endif

////////////////////////////////
//~ rjf: Log Creation/Selection

       Log* log_alloc(AllocatorInfo ainfo);
       void log_release(Log* log);
MD_API void log_select (Log* log);

inline Log*
log_alloc(AllocatorInfo ainfo, U64 arena_block_size) {
	if (arena_block_size == 0) {
		arena_block_size = LOG_DEFAULT_ARENA_BLOCK_SIZE;
	}
	Arena* arena = arena_alloc(.backing = ainfo, .block_size = arena_block_size);
	Log*   log   = push_array(arena, Log, 1);
	log->arena   = arena;
  return log;
}

inline void log_release(Log* log) { arena_release(log->arena); }

////////////////////////////////
//~ rjf: Log Building

MD_API void log_msg (LogMsgKind kind, String8 string);
MD_API void log_msgf(LogMsgKind kind, char* fmt, ...);

#define log_info(s)               log_msg (LogMsgKind_Info,      (s))
#define log_infof(fmt, ...)       log_msgf(LogMsgKind_Info,      (fmt), __VA_ARGS__)
#define log_user_error(s)         log_msg (LogMsgKind_UserError, (s))
#define log_user_errorf(fmt, ...) log_msgf(LogMsgKind_UserError, (fmt), __VA_ARGS__)

#define log_info_named_block(s)         defer_loop(log_infof("%S:\n{\n", (s)), log_infof("}\n"))
#define log_info_named_blockf(fmt, ...) defer_loop((log_infof(fmt, __VA_ARGS__), log_infof(":\n{\n")), log_infof("}\n"))

////////////////////////////////
//~ rjf: Log Scopes

       void           log_scope_begin(void);
MD_API LogScopeResult log_scope_end(Arena* arena);

inline void
log_scope_begin(void) {
	if (log_active != 0) {
		U64       pos   = arena_pos(log_active->arena);
		LogScope* scope = push_array(log_active->arena, LogScope, 1);
		scope->pos = pos;
		sll_stack_push(log_active->top_scope, scope);
	}
}
