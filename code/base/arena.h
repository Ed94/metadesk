#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "macros.h"
#	include "base_types.h"
#	include "memory_substrate.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Constants

#define ARENA_HEADER_SIZE 64

////////////////////////////////
//~ rjf: Types

typedef U32 ArenaFlags;
enum
{
	// Don't chain this arena
	ArenaFlag_NoChain      = (1 << 0), 
	// Only works if backing is virtual memory, will allocate a new backing VArena when the current block exhausts
	// Otherwise will assume backing can chain multiple block_size arenas until failure.
	ArenaFlag_ChainVirtual = (1 << 1),
};

typedef struct ArenaParams ArenaParams;
struct ArenaParams
{
	AllocatorInfo backing;
	ArenaFlags    flags;
	U64           block_size; // If chaining VArenas set this to the reserve size
};

/* NOTE(Ed): This is a combination of several concepts into a single interface:
	* A arena 'block' of memory with segmented chaining of the blocks
	* An OS virtual memory allocation scheme
	* A push/pop stack allocation interface for the arena

	TODO(Ed): We need to lift the virtual memory tracking to its own data structure 
	and virtual memory interface utilizing memory_substrate.h
*/

typedef struct Arena Arena;
struct Arena
{
	Arena*        prev;    // previous arena in chain
	Arena*        current; // current arena in chain
	AllocatorInfo backing;
	U64           pos;
	U64           block_size;
	ArenaFlags    flags;
};
static_assert(size_of(Arena) <= ARENA_HEADER_SIZE, "sizeof(Arena) <= ARENA_HEADER_SIZE");

typedef struct TempArena TempArena;
struct TempArena
{
	Arena* arena;
	U64    pos;
};

////////////////////////////////
//~ rjf: Arena Functions

MD_API void* arena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);

//- rjf: arena creation/destruction

MD_API Arena* arena_alloc_(ArenaParams *params);
#define       arena_alloc(...) arena_alloc_( & ( ArenaParams) { .backing = {}, .reserve_size = MB(64), .commit_size = KB(64), __VA_ARGS__ } )

void arena_release(Arena *arena);

//- rjf: arena push/pop/pos core functions

internal void *arena_push  (Arena *arena, U64 size, U64 align);
internal U64   arena_pos   (Arena *arena);
internal void  arena_pop_to(Arena *arena, U64 pos);

//- rjf: arena push/pop helpers

internal void arena_clear(Arena *arena);
internal void arena_pop  (Arena *arena, U64 amt);

//- rjf: temporary arena scopes

internal TempArena temp_arena_begin(Arena *arena);
internal void      temp_arena_end(TempArena temp);

//- rjf: push helper macros

#ifndef push_array
#define push_array_no_zero_aligned(a, T, c, align) (T *)arena_push((a), sizeof(T) * (c), (align))
#define push_array_aligned(a, T, c, align)         (T *)memory_zero(push_array_no_zero_aligned(a, T, c, align), sizeof(T) * (c))
#define push_array_no_zero(a, T, c)                     push_array_no_zero_aligned(a, T, c, max(8, align_of(T)))
#define push_array(a, T, c)                             push_array_aligned        (a, T, c, max(8, align_of(T)))
#endif

// Inlines

inline void
arena_release(Arena* arena)
{
	for (Arena* n = arena->current, *prev = 0; n != 0; n = prev)
	{
		prev = n->prev;
		alloc_free(arena->backing, n);
	}
}
