#ifdef INTELLISENSE_DIRECTIVES
#	include "logger.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Globals/Thread-Locals

MD_API_C md_thread_static MD_Log* md_log_active;
#if !MD_BUILD_SUPPLEMENTARY_UNIT
MD_API_C md_thread_static MD_Log* md_log_active = 0;
#endif

////////////////////////////////
//~ rjf: MD_Log Creation/Selection

void
md_log_select(MD_Log* log) {
	md_log_active = log;
}

////////////////////////////////
//~ rjf: MD_Log Building

void
md_log_msg(MD_LogMsgKind kind, MD_String8 string) {
	if(md_log_active != 0 && md_log_active->top_scope != 0) {
		MD_String8 md_string_copy = md_str8_copy(md_log_active->arena, string);
		md_str8_list_push(md_log_active->arena, &md_log_active->top_scope->strings[kind], md_string_copy);
	}
}

void
md_log_msgf(MD_LogMsgKind kind, char *fmt, ...) {
	if(md_log_active != 0) 
	{
		MD_TempArena scratch = md_scratch_begin(0, 0);

		va_list args;
		va_start(args, fmt);
		MD_String8 string = md_str8fv(scratch.arena, fmt, args);
		md_log_msg(kind, string);
		va_end(args);

		scratch_end(scratch);
	}
}

////////////////////////////////
//~ rjf: MD_Log Scopes

void
md_log_scope_begin(void) {
	if (md_log_active != 0) {
		MD_U64       pos   = md_arena_pos(md_log_active->arena);
		MD_LogScope* scope = md_push_array_(md_log_active->arena, MD_LogScope, 1);
		scope->pos = pos;
		md_sll_stack_push(md_log_active->top_scope, scope);
	}
}

MD_LogScopeResult
md_log_scope_end(MD_Arena *arena)
{
	MD_LogScopeResult result = {0};
	if(md_log_active != 0)
	{
		MD_LogScope* scope = md_log_active->top_scope;
		if(scope != 0)
		{
			md_sll_stack_pop(md_log_active->top_scope);
			if(arena != 0)
			{
				for (md_each_enum_val(MD_LogMsgKind, kind)) {
					MD_TempArena scratch = md_scratch_begin(&arena, 1);
					MD_String8   
					result_unindented    = md_str8_list_join(scratch.arena, &scope->strings[kind], 0);
					result.strings[kind] = md_indented_from_string(arena, result_unindented);
					scratch_end(scratch);
				}
			}
			md_arena_pop_to(md_log_active->arena, scope->pos);
		}
	}
	return result;
}
