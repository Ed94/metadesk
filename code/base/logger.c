#ifdef INTELLISENSE_DIRECTIVES
#	include "logger.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Globals/Thread-Locals

MD_API_C thread_static Log* log_active;
#if !BUILD_SUPPLEMENTARY_UNIT
MD_API_C thread_static Log* log_active = 0;
#endif

////////////////////////////////
//~ rjf: Log Creation/Selection

void
log_select(Log* log) {
	log_active = log;
}

////////////////////////////////
//~ rjf: Log Building

void
log_msg(LogMsgKind kind, String8 string) {
	if(log_active != 0 && log_active->top_scope != 0) {
		String8 string_copy = push_str8_copy(log_active->arena, string);
		str8_list_push(log_active->arena, &log_active->top_scope->strings[kind], string_copy);
	}
}

void
log_msgf(LogMsgKind kind, char *fmt, ...) {
	if(log_active != 0) 
	{
		// TODO(Ed): Review
		TempArena scratch = scratch_begin(0, 0);

		va_list args;
		va_start(args, fmt);
		String8 string = push_str8fv(scratch.arena, fmt, args);
		log_msg(kind, string);
		va_end(args);

		scratch_end(scratch);
	}
}


////////////////////////////////
//~ rjf: Log Scopes

void
log_scope_begin(void) {
	if (log_active != 0) {
		U64       pos   = arena_pos(log_active->arena);
		LogScope* scope = push_array(log_active->arena, LogScope, 1);
		scope->pos = pos;
		sll_stack_push(log_active->top_scope, scope);
	}
}

LogScopeResult
log_scope_end(Arena *arena)
{
	LogScopeResult result = {0};
	if(log_active != 0)
	{
		LogScope* scope = log_active->top_scope;
		if(scope != 0)
		{
			sll_stack_pop(log_active->top_scope);
			if(arena != 0)
			{
				for (each_enum_val(LogMsgKind, kind)) {
					// TODO(Ed): Review
					TempArena scratch = scratch_begin(&arena, 1);
					String8   
					result_unindented    = str8_list_join(scratch.arena, &scope->strings[kind], 0);
					result.strings[kind] = indented_from_string(arena, result_unindented);
					scratch_end(scratch);
				}
			}
			arena_pop_to(log_active->arena, scope->pos);
		}
	}
	return result;
}
