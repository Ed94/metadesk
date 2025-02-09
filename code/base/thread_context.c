#ifdef INTELLISENSE_DIRECTIVES
#	include "arena.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(allen): Thread Context Functions

MD_API_C thread_static TCTX* tctx_thread_local;

#if ! MD_BUILD_SUPPLEMENTARY_UNIT
MD_API_C thread_static TCTX* tctx_thread_local = 0;
#endif

void
tctx_init_and_equip(TCTX* tctx)
{
	// memory_zero_struct(tctx);

	Arena** arena_ptr = tctx->arenas;
	for (U64 i = 0; i < array_count(tctx->arenas); i += 1, arena_ptr += 1)
	{
		if (*arena_ptr == nullptr)
		{
			VArena* vm = varena_alloc(.reserve_size = VARENA_DEFAULT_RESERVE, .commit_size = VARENA_DEFAULT_COMMIT);
			*arena_ptr = arena_alloc(.backing = varena_allocator(vm));
		}
	}
	tctx_thread_local = tctx;
}

void
tctx_init_and_equip_alloc(TCTX* tctx, AllocatorInfo ainfo)
{
	memory_zero_struct(tctx);

	Arena** arena_ptr = tctx->arenas;
	for (U64 i = 0; i < array_count(tctx->arenas); i += 1, arena_ptr += 1)
	{
		if (*arena_ptr == nullptr)
		{
			*arena_ptr = arena_alloc(.backing = ainfo);
		}
	}
	tctx_thread_local = tctx;
}

void
tctx_release(void) {
  for (U64 i = 0; i < array_count(tctx_thread_local->arenas); i += 1) {
    arena_release(tctx_thread_local->arenas[i]);
  }
}

TCTX*
tctx_get_equipped(void){
  return(tctx_thread_local);
}

Arena*
tctx_get_scratch(Arena** conflicts, U64 count)
{
	TCTX* tctx = tctx_get_equipped();

	Arena*  result    = 0;
	Arena** arena_ptr = tctx->arenas;
	for (U64 i = 0; i < array_count(tctx->arenas); i += 1, arena_ptr += 1)
	{
		Arena** conflict_ptr = conflicts;
		B32     has_conflict = 0;
		for (U64 j = 0; j < count; j += 1, conflict_ptr += 1)
		{
			if (*arena_ptr == *conflict_ptr) {
				has_conflict = 1;
				break;
			}
		}
		if ( ! has_conflict){
			result = *arena_ptr;
			break;
		}
	}
	return(result);
}
