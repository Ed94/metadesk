#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "arena.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: MD_Arena Functions

//- rjf: arena creation/destruction

MD_Arena*
md_arena__alloc(MD_ArenaParams* optional_params)
{
	MD_ArenaParams params = optional_params ? *optional_params : (MD_ArenaParams){0};

	MD_SPTR const varena_header_size = md_align_pow2(size_of(MD_VArena), MD_DEFAULT_MEMORY_ALIGNMENT);
	MD_SPTR const header_size        = md_align_pow2(size_of(MD_Arena),  MD_DEFAULT_MEMORY_ALIGNMENT);

	MD_U64 const varena_reserve_size = MD_VARENA_DEFAULT_RESERVE;

	MD_B32 is_virtual = md_allocator_type(params.backing) & MD_AllocatorType_VArena;
	params.flags  |= MD_ArenaFlag_Virtual * is_virtual;


	if (params.backing.proc == md_nullptr) params.backing    = md_default_allocator();
	if (params.block_size   == 0      ) params.block_size = MD_ARENA_DEFAULT_BLOCK_SIZE;

	MD_SSIZE md_alloc_size = is_virtual ? header_size : params.block_size;

	void* base = md_alloc(params.backing, md_alloc_size);
	// rjf: extract arena header & fill
	MD_Arena* arena      = (MD_Arena*) base;
	arena->prev       = md_nullptr;
	arena->current    = arena;
	arena->backing    = params.backing;
	arena->base_pos   = 0;
	arena->pos        = header_size;
	arena->block_size = params.block_size;
	arena->flags      = params.flags;
	md_asan_unpoison_memory_region(base, sizeof(MD_Arena));
	return arena;
}

//- rjf: arena push/pop core functions

void*
md_arena_push(MD_Arena* arena, MD_SSIZE size, MD_SSIZE align)
{
	MD_SPTR const header_size = md_align_pow2(size_of(MD_Arena), MD_DEFAULT_MEMORY_ALIGNMENT);

	MD_Arena* current   = arena->current;
	MD_SPTR   curr_sptr = md_scast(MD_SPTR, current);

	MD_SSIZE aligned_size = md_align_pow2(size, align);

	MD_SPTR pos_pre = current->pos;
	MD_SPTR pos_pst = pos_pre + aligned_size;

	MD_B32 is_virtual = arena->flags & MD_ArenaFlag_Virtual;
  
	// rjf: chain, if needed
	if ( current->block_size < pos_pst && ! (arena->flags & MD_ArenaFlag_NoChain) )
	{
		MD_Arena* new_block = md_nullptr;

		MD_B32 vmem_chain  = is_virtual && (arena->flags & MD_ArenaFlag_NoChainVirtual);
		if (vmem_chain) {
			MD_SPTR const varena_header_size  = md_align_pow2(size_of(MD_VArena), MD_DEFAULT_MEMORY_ALIGNMENT);
			MD_SPTR const md_arena_block_size = MD_VARENA_DEFAULT_RESERVE - varena_header_size;

			MD_VArena* vcurrent = md_rcast(MD_VArena*, arena->backing.data);

			MD_VArena* new_vm = md_varena_alloc(.reserve_size = vcurrent->reserve, .commit_size = vcurrent->commit_size);
			        new_block = md_arena_alloc(.backing = md_varena_allocator(new_vm), .block_size = md_arena_block_size);
		}
		else {
			MD_SPTR const md_arena_block_size = arena->block_size + header_size;
			new_block = md_arena_alloc(.backing = arena->backing, .block_size = md_arena_block_size);
		}
		new_block->base_pos = current->base_pos + current->block_size;

		md_sll_stack_push_n(arena->current, new_block, prev);
		
		current = new_block;
		pos_pre = current->pos;
		pos_pst = pos_pre + aligned_size;
	}

	// rjf: push onto current block
	void* result = 0;
	{
		result       = md_scast(void*, curr_sptr + pos_pre);
		current->pos = pos_pst;
		md_asan_unpoison_memory_region(result, size);
	}

	if (is_virtual) {
		// Sync virtual arena
		void* vresult = md_alloc_align(arena->backing, size, align);
		md_assert(vresult == result);
	}
	
	// rjf: panic on failure
	#if MD_OS_FEATURE_GRAPHICAL
	if(md_unlikely(result == 0))
	{
		md_os_graphical_message(1, md_str8_lit("Fatal Allocation Failure"), md_str8_lit("Unexpected memory allocation failure."));
		md_os_abort(1);
	}
	#endif
	
	return result;
}

void
md_arena_pop_to(MD_Arena *arena, MD_SSIZE pos)
{
	MD_SPTR const header_size = md_align_pow2(size_of(MD_Arena), MD_DEFAULT_MEMORY_ALIGNMENT);

	MD_Arena*        current    = arena->current;
	MD_AllocatorInfo backing    = current->backing;
	MD_B32           is_virtual = md_allocator_type(backing) & MD_AllocatorType_VArena;

	MD_SSIZE  big_pos = md_clamp_bot(header_size, pos);
	// If base position is larger than the position to pop to:
	//	We are in a previous arena and msut free the current
	for(MD_Arena* prev = 0; current->base_pos >= big_pos; current = prev)
	{
		prev = current->prev;

		if (is_virtual) {
			md_varena_release(md_rcast(MD_VArena*, current->backing.data));
		}
		else if (md_allocator_query_support(backing) & MD_AllocatorQuery_Free) {
			md_alloc_free(current->backing, current);
		}
	}
	arena->current = current;
	MD_SSIZE new_pos  = big_pos - current->base_pos;
	md_assert_always(new_pos <= current->pos);
	md_asan_poison_memory_region((MD_U8*)current + new_pos, (current->pos - new_pos));
	current->pos = new_pos;
	if (is_virtual) {
		varena_rewind(md_rcast(MD_VArena*, current->backing.data), current->pos);
	}
}

void* md_arena_allocator_proc(void* allocator_data, MD_AllocatorMode mode, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags)
{
	MD_Arena* arena = md_rcast(MD_Arena*, allocator_data);

	void* allocated_ptr = md_nullptr;
	switch (mode)
	{
		case MD_AllocatorMode_Alloc:
		{
			allocated_ptr = md_arena_push(arena, size, alignment);
		}
		break;

		case MD_AllocatorMode_Free:
		{
		}
		break;

		case MD_AllocatorMode_FreeAll:
		{
			md_arena_release(arena);
		}
		break;

		case MD_AllocatorMode_Resize:
		{
			md_assert(old_memory != md_nullptr);
			md_assert(old_size > 0);
			md_assert_msg(old_size == size, "Requested md_resize when none needed");

			size     = md_align_pow2(size, alignment);
			old_size = md_align_pow2(size, alignment);

			MD_SPTR old_memory_offset = md_scast(MD_SPTR, old_memory) + old_size;
			MD_SPTR current_offset    = arena->pos;

			md_assert_msg(old_memory_offset == current_offset, "Cannot md_resize existing allocation in MD_VArena unless it was the last allocated");

			MD_B32 requested_shrink = size >= old_size;
			if (requested_shrink) {
				arena->pos     -= size;
				allocated_ptr   = old_memory;
				break;
			}

			allocated_ptr = old_memory;
			arena->pos   += size;
		}
		break;

		case MD_AllocatorMode_QueryType:
		{
			return (void*) MD_AllocatorType_Arena;
		}
		break;

		case MD_AllocatorMode_QuerySupport:
		{
			return (void*) (MD_AllocatorQuery_Alloc | MD_AllocatorQuery_Resize | MD_AllocatorQuery_FreeAll);
		}
		break;
	}

	return allocated_ptr;
}
