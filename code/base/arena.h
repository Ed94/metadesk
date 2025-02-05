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
	ArenaFlag_NoChain        = (1 << 0), 
	// Only relevant if backing is virtual memory, will prevent allocating a new backing VArena when the current block exhausts
	// Will assume backing can chain multiple block_size arenas however. If there is an allocation failure it will assert.
	ArenaFlag_NoChainVirtual = (1 << 1),
};

typedef struct ArenaParams ArenaParams;
struct ArenaParams
{
	AllocatorInfo backing;
	ArenaFlags    flags;
	U64           block_size; // If chaining VArenas set this to the reserve size
};

/* NOTE(Ed): The original metadesk arena is a combination of several concepts into a single interface:
	* An OS virtual memory allocation scheme
	* A arena 'block' of memory with segmented chaining of the blocks
	* A push/pop stack allocation interface for the arena

	The virtual memory has been abstracted into a backing allocator,
	and chaining still supports reserving new virtual address regions .
	(can be disabled with ArenaFlag_NoChainVirtual)

	If large pages are desired, see VArena.
*/

typedef struct Arena Arena;
struct Arena
{
	Arena*        prev;    // previous arena in chain
	Arena*        current; // current arena in chain
	AllocatorInfo backing;
	SSIZE         pos;
	SSIZE         block_size;
	ArenaFlags    flags;
};
static_assert(size_of(Arena) <= ARENA_HEADER_SIZE, "sizeof(Arena) <= ARENA_HEADER_SIZE");

typedef struct TempArena TempArena;
struct TempArena
{
	Arena* arena;
	SSIZE  pos;
};

////////////////////////////////
//~ rjf: Arena Functions

MD_API void* arena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);

inline
AllocatorInfo arena_allocator(Arena* arena) {
	AllocatorInfo info = { arena_allocator_proc, arena};
	return info;
}

//- rjf: arena creation/destruction

MD_API Arena* arena_alloc_(ArenaParams* params);
#define       arena_alloc(...) arena_alloc_( &(ArenaParams){ __VA_ARGS__ } )

void arena_release(Arena *arena);

//- rjf: arena push/pop/pos core functions

internal void *arena_push  (Arena *arena, SSIZE size, SSIZE align);
internal U64   arena_pos   (Arena *arena);
internal void  arena_pop_to(Arena *arena, SSIZE pos);

//- rjf: arena push/pop helpers

internal void arena_clear(Arena *arena);
internal void arena_pop  (Arena *arena, SSIZE amt);

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

// DEFAULT_ALLOCATOR
#ifndef MD_OVERRIDE_DEFAULT_ALLOCATOR
// The default allocator for this base module is the Arena allocator with a VArena backing
inline
AllocatorInfo default_allocator()
{
	local_persist thread_local VArena* backing_vmem = nullptr;
	local_persist thread_local Arena*  arena        = nullptr;
	if (arena == nullptr) {
		backing_vmem = varena_alloc(.flags = 0, .base_addr = 0x0, .reserve_size = VARENA_DEFAULT_RESERVE, .commit_size = VARENA_DEFAULT_COMMIT);
		arena        = arena_alloc(.backing = varena_allocator(backing_vmem), .block_size = VARENA_DEFAULT_RESERVE);
	}
	AllocatorInfo info = { arena_allocator_proc, arena };
	return info;
}
#endif
