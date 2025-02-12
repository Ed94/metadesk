#ifdef INTELLISENSE_DIRECTIVES
#	include "arena.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(allen): Thread MD_Context Functions

MD_API_C md_thread_static MD_TCTX* md_tctx_thread_local;

#if ! MD_BUILD_SUPPLEMENTARY_UNIT
MD_API_C md_thread_static MD_TCTX* md_tctx_thread_local = 0;
#endif

void
md_tctx_init_and_equip(MD_TCTX* md_tctx)
{
	// md_memory_zero_struct(md_tctx);

	MD_Arena** md_arena_ptr = md_tctx->arenas;
	for (MD_U64 i = 0; i < md_array_count(md_tctx->arenas); i += 1, md_arena_ptr += 1)
	{
		if (*md_arena_ptr == md_nullptr)
		{
			MD_VArena* vm = md_varena_alloc(.reserve_size = MD_VARENA_DEFAULT_RESERVE, .commit_size = MD_VARENA_DEFAULT_COMMIT);
			*md_arena_ptr = md_arena_alloc(.backing = md_varena_allocator(vm));
		}
	}
	md_tctx_thread_local = md_tctx;
}

void
md_tctx_init_and_equip_alloc(MD_TCTX* md_tctx, MD_AllocatorInfo ainfo)
{
	md_memory_zero_struct(md_tctx);

	MD_Arena** md_arena_ptr = md_tctx->arenas;
	for (MD_U64 i = 0; i < md_array_count(md_tctx->arenas); i += 1, md_arena_ptr += 1)
	{
		if (*md_arena_ptr == md_nullptr)
		{
			*md_arena_ptr = md_arena_alloc(.backing = ainfo);
		}
	}
	md_tctx_thread_local = md_tctx;
}

void
md_tctx_release(void) {
  for (MD_U64 i = 0; i < md_array_count(md_tctx_thread_local->arenas); i += 1) {
    md_arena_release(md_tctx_thread_local->arenas[i]);
  }
}

MD_TCTX*
md_tctx_get_equipped(void){
  return(md_tctx_thread_local);
}

MD_Arena*
md_tctx_get_scratch(MD_Arena** conflicts, MD_U64 count)
{
	MD_TCTX* md_tctx = md_tctx_get_equipped();

	MD_Arena*  result    = 0;
	MD_Arena** md_arena_ptr = md_tctx->arenas;
	for (MD_U64 i = 0; i < md_array_count(md_tctx->arenas); i += 1, md_arena_ptr += 1)
	{
		MD_Arena** conflict_ptr = conflicts;
		MD_B32     has_conflict = 0;
		for (MD_U64 j = 0; j < count; j += 1, conflict_ptr += 1)
		{
			if (*md_arena_ptr == *conflict_ptr) {
				has_conflict = 1;
				break;
			}
		}
		if ( ! has_conflict){
			result = *md_arena_ptr;
			break;
		}
	}
	return(result);
}
