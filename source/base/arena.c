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
arena__alloc(ArenaParams* optional_params)
{
	ArenaParams params = optional_params ? *optional_params : (ArenaParams){0};

	SPTR const varena_header_size = align_pow2(size_of(VArena), MD_DEFAULT_MEMORY_ALIGNMENT);
	SPTR const header_size        = align_pow2(size_of(Arena),  MD_DEFAULT_MEMORY_ALIGNMENT);

	U64 const varena_reserve_size = VARENA_DEFAULT_RESERVE;

	B32 is_virtual = allocator_type(params.backing) & AllocatorType_VArena;
	params.flags  |= ArenaFlag_Virtual * is_virtual;


	if (params.backing.proc == nullptr) params.backing    = default_allocator();
	if (params.block_size   == 0      ) params.block_size = ARENA_DEFAULT_BLOCK_SIZE;

	SSIZE alloc_size = is_virtual ? header_size : params.block_size;

	void* base = alloc(params.backing, alloc_size);
	// rjf: extract arena header & fill
	Arena* arena      = (Arena*) base;
	arena->prev       = nullptr;
	arena->current    = arena;
	arena->backing    = params.backing;
	arena->base_pos   = 0;
	arena->pos        = header_size;
	arena->block_size = params.block_size;
	arena->flags      = params.flags;
	asan_unpoison_memory_region(base, sizeof(Arena));
	return arena;
}

//- rjf: arena push/pop core functions

void*
arena_push(Arena* arena, SSIZE size, SSIZE align)
{
	SPTR const header_size = align_pow2(size_of(Arena), MD_DEFAULT_MEMORY_ALIGNMENT);

	Arena* current   = arena->current;
	SPTR   curr_sptr = scast(SPTR, current);

	SSIZE aligned_size = align_pow2(size, align);

	SPTR pos_pre = current->pos;
	SPTR pos_pst = pos_pre + aligned_size;

	B32 is_virtual = arena->flags & ArenaFlag_Virtual;
  
	// rjf: chain, if needed
	if ( current->block_size < pos_pst && ! (arena->flags & ArenaFlag_NoChain) )
	{
		Arena* new_block = nullptr;

		B32 vmem_chain  = is_virtual && (arena->flags & ArenaFlag_NoChainVirtual);
		if (vmem_chain) {
			SPTR const varena_header_size = align_pow2(size_of(VArena), MD_DEFAULT_MEMORY_ALIGNMENT);
			SPTR const arena_block_size   = VARENA_DEFAULT_RESERVE - varena_header_size;

			VArena* vcurrent = rcast(VArena*, arena->backing.data);

			VArena* new_vm    = varena_alloc(.reserve_size = vcurrent->reserve, .commit_size = vcurrent->commit_size);
			        new_block = arena_alloc(.backing = varena_allocator(new_vm), .block_size = arena_block_size);
		}
		else {
			SPTR const arena_block_size = arena->block_size + header_size;
			new_block = arena_alloc(.backing = arena->backing, .block_size = arena_block_size);
		}
		new_block->base_pos = current->base_pos + current->block_size;

		sll_stack_push_n(arena->current, new_block, prev);
		
		current = new_block;
		pos_pre = current->pos;
		pos_pst = pos_pre + aligned_size;
	}

	// rjf: push onto current block
	void* result = 0;
	{
		result       = scast(void*, curr_sptr + pos_pre);
		current->pos = pos_pst;
		asan_unpoison_memory_region(result, size);
	}

	if (is_virtual) {
		// Sync virtual arena
		void* vresult = alloc_align(arena->backing, size, align);
		assert(vresult == result);
	}
	
	// rjf: panic on failure
	#if OS_FEATURE_GRAPHICAL
	if(unlikely(result == 0))
	{
		os_graphical_message(1, str8_lit("Fatal Allocation Failure"), str8_lit("Unexpected memory allocation failure."));
		os_abort(1);
	}
	#endif
	
	return result;
}

void
arena_pop_to(Arena *arena, SSIZE pos)
{
	SPTR const header_size = align_pow2(size_of(Arena), MD_DEFAULT_MEMORY_ALIGNMENT);

	Arena*        current    = arena->current;
	AllocatorInfo backing    = current->backing;
	B32           is_virtual = allocator_type(backing) & AllocatorType_VArena;

	SSIZE  big_pos = clamp_bot(header_size, pos);
	// If base position is larger than the position to pop to:
	//	We are in a previous arena and msut free the current
	for(Arena* prev = 0; current->base_pos >= big_pos; current = prev)
	{
		prev = current->prev;

		if (is_virtual) {
			varena_release(rcast(VArena*, current->backing.data));
		}
		else if (allocator_query_support(backing) & AllocatorQuery_Free) {
			alloc_free(current->backing, current);
		}
	}
	arena->current = current;
	SSIZE new_pos  = big_pos - current->base_pos;
	assert_always(new_pos <= current->pos);
	asan_poison_memory_region((U8*)current + new_pos, (current->pos - new_pos));
	current->pos = new_pos;
	if (is_virtual) {
		varena_rewind(rcast(VArena*, current->backing.data), current->pos);
	}
}

void* arena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags)
{
	Arena* arena = rcast(Arena*, allocator_data);

	void* allocated_ptr = nullptr;
	switch (mode)
	{
		case AllocatorMode_Alloc:
		{
			allocated_ptr = arena_push(arena, size, alignment);
		}
		break;

		case AllocatorMode_Free:
		{
		}
		break;

		case AllocatorMode_FreeAll:
		{
			arena_release(arena);
		}
		break;

		case AllocatorMode_Resize:
		{
			assert(old_memory != nullptr);
			assert(old_size > 0);
			assert_msg(old_size == size, "Requested resize when none needed");

			size     = align_pow2(size, alignment);
			old_size = align_pow2(size, alignment);

			SPTR old_memory_offset = scast(SPTR, old_memory) + old_size;
			SPTR current_offset    = arena->pos;

			assert_msg(old_memory_offset == current_offset, "Cannot resize existing allocation in VArena unless it was the last allocated");

			B32 requested_shrink = size >= old_size;
			if (requested_shrink) {
				arena->pos     -= size;
				allocated_ptr   = old_memory;
				break;
			}

			allocated_ptr = old_memory;
			arena->pos   += size;
		}
		break;

		case AllocatorMode_QueryType:
		{
			return (void*) AllocatorType_Arena;
		}
		break;

		case AllocatorMode_QuerySupport:
		{
			return (void*) (AllocatorQuery_Alloc | AllocatorQuery_Resize | AllocatorQuery_FreeAll);
		}
		break;
	}

	return allocated_ptr;
}
