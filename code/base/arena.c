#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "arena.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Arena Functions

//- rjf: arena creation/destruction

Arena*
arena_alloc_(ArenaParams* params)
{
	void* base = alloc(params->backing, params->block_size);
	// rjf: extract arena header & fill
	Arena* arena      = (Arena*) base;
	arena->prev       = nullptr;
	arena->current    = arena;
	arena->backing    = params->backing;
	arena->pos        = size_of(Arena);
	arena->block_size = params->block_size;
	arena->flags      = params->flags;
	asan_unpoison_memory_region(base, sizeof(Arena));
	return arena;
}

//- rjf: arena push/pop core functions

internal void *
arena_push(Arena *arena, U64 size, U64 align)
{
	SPTR header_size = size_of(Arena);

	Arena *current = arena->current;
	SPTR pos_pre = AlignPow2(current->pos, align);
	SPTR pos_pst = pos_pre + size;
  
	// rjf: chain, if needed
	if ( current->block_size < pos_pst && !(arena->flags & ArenaFlag_NoChain) )
	{
		SSIZE res_size = current->block_size;

		if(size +  > res_size)
		{
			res_size = size + ARENA_HEADER_SIZE;
		}

		Arena *new_block = arena_alloc(
			.reserve_size = res_size,
			.commit_size = cmt_size,
			.flags = current->flags
		);
		
		new_block->base_pos = current->base_pos + current->res;

		SLLStackPush_N(arena->current, new_block, prev);
		
		current = new_block;
		pos_pre = AlignPow2(current->pos, align);
		pos_pst = pos_pre + size;
	}
  
	// rjf: commit new pages, if needed
	if(current->cmt < pos_pst && !(current->flags & ArenaFlag_LargePages))
	{
		U64 cmt_pst_aligned = AlignPow2(pos_pst, current->cmt_size);
		U64 cmt_pst_clamped = ClampTop(cmt_pst_aligned, current->res);
		U64 cmt_size = cmt_pst_clamped - current->cmt;
		os_commit((U8 *)current + current->cmt, cmt_size);
		current->cmt = cmt_pst_clamped;
	}
  
	// rjf: push onto current block
	void *result = 0;
	if(current->cmt >= pos_pst)
	{
		result = (U8 *)current+pos_pre;
		current->pos = pos_pst;
		AsanUnpoisonMemoryRegion(result, size);
	}
	
	// rjf: panic on failure
	#if OS_FEATURE_GRAPHICAL
	if(Unlikely(result == 0))
	{
		os_graphical_message(1, str8_lit("Fatal Allocation Failure"), str8_lit("Unexpected memory allocation failure."));
		os_abort(1);
	}
	#endif
	
	return result;
}

internal U64
arena_pos(Arena *arena)
{
  Arena *current = arena->current;
  U64 pos = current->base_pos + current->pos;
  return pos;
}

internal void
arena_pop_to(Arena *arena, U64 pos)
{
  U64 big_pos = ClampBot(ARENA_HEADER_SIZE, pos);
  Arena *current = arena->current;
  for(Arena *prev = 0; current->base_pos >= big_pos; current = prev)
  {
    prev = current->prev;
    os_release(current, current->res);
  }
  arena->current = current;
  U64 new_pos = big_pos - current->base_pos;
  AssertAlways(new_pos <= current->pos);
  AsanPoisonMemoryRegion((U8*)current + new_pos, (current->pos - new_pos));
  current->pos = new_pos;
}

//- rjf: arena push/pop helpers

internal void
arena_clear(Arena *arena)
{
  arena_pop_to(arena, 0);
}

internal void
arena_pop(Arena *arena, U64 amt)
{
  U64 pos_old = arena_pos(arena);
  U64 pos_new = pos_old;
  if(amt < pos_old)
  {
    pos_new = pos_old - amt;
  }
  arena_pop_to(arena, pos_new);
}

//- rjf: temporary arena scopes

internal TempArena
temp_begin(Arena *arena)
{
  U64       pos  = arena_pos(arena);
  TempArena temp = {arena, pos};
  return temp;
}

internal void
temp_end(TempArena temp)
{
  arena_pop_to(temp.arena, temp.pos);
}
