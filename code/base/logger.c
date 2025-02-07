#ifdef INTELLISENSE_DIRECTIVES
#	include "logger.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Globals/Thread-Locals

MD_API_C thread_static Log *log_active;
#if !BUILD_SUPPLEMENTARY_UNIT
MD_API_C thread_static Log *log_active = 0;
#endif

////////////////////////////////
//~ rjf: Log Creation/Selection

void
log_select(Log* log) {
	log_active = log;
}

////////////////////////////////
//~ rjf: Log Scopes

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
