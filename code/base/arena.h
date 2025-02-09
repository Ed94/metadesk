#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "macros.h"
#	include "base_types.h"
#	include "memory_substrate.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

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

#define ARENA_DEFAULT_BLOCK_SIZE VARENA_DEFAULT_RESERVE - align_pow2(size_of(VArena), MD_DEFAULT_MEMORY_ALIGNMENT)

/* NOTE(Ed): The original metadesk arena is a combination of several concepts into a single interface
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
	SSIZE         base_pos;
	SSIZE         pos;
	SSIZE         block_size;
	ArenaFlags    flags;
};
// static_assert(size_of(Arena) <= ARENA_HEADER_SIZE, "sizeof(Arena) <= ARENA_HEADER_SIZE");

typedef struct TempArena TempArena;
struct TempArena
{
	Arena* arena;
	SSIZE  pos;
};

////////////////////////////////
//~ rjf: Arena Functions

MD_API void* arena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);

force_inline AllocatorInfo arena_allocator(Arena* arena) { AllocatorInfo info = { arena_allocator_proc, arena}; return info; }

// Useful for providing an arena to scratch_begin_alloc
force_inline Arena*
extract_arena(AllocatorInfo ainfo) {
	if (allocator_type(ainfo) == AllocatorType_Arena) {
		return (Arena*) ainfo.data;
	}
	return nullptr;
}

//- rjf: arena creation/destruction

MD_API Arena* arena__alloc(ArenaParams params);
#define       arena_alloc(...) arena__alloc( (ArenaParams){ __VA_ARGS__ } )

void arena_release(Arena *arena);

//- rjf: arena push/pop/pos core functions

MD_API void* arena_push  (Arena* arena, SSIZE size, SSIZE align);
       U64   arena_pos   (Arena* arena);
MD_API void  arena_pop_to(Arena* arena, SSIZE pos);

//- rjf: arena push/pop helpers

void arena_clear(Arena* arena);
void arena_pop  (Arena* arena, SSIZE amt);

//- rjf: temporary arena scopes

TempArena temp_arena_begin(Arena* arena);
void      temp_arena_end(TempArena temp);

//- rjf: push helper macros

#ifndef push_array
#define push_array_no_zero_aligned(a, T, c, align) (T *)arena_push((a), sizeof(T) * (c), (align))
#define push_array_aligned(a, T, c, align)         (T *)memory_zero(push_array_no_zero_aligned(a, T, c, align), sizeof(T) * (c))
#define push_array_no_zero(a, T, c)                     push_array_no_zero_aligned(a, T, c, max(8, align_of(T)))
#define push_array(a, T, c)                             push_array_aligned        (a, T, c, max(8, align_of(T)))
#endif

// Inlines

inline void
arena_release(Arena* arena) {
	for (Arena* n = arena->current, *prev = 0; n != 0; n = prev) {
		prev = n->prev;
		alloc_free(arena->backing, n);
	}
}

inline U64
arena_pos(Arena *arena) {
	Arena* current = arena->current;
	U64    pos     = current->base_pos + current->pos;
	return pos;
}

//- rjf: arena push/pop helpers

force_inline void arena_clear(Arena* arena) { arena_pop_to(arena, 0); }

inline void
arena_pop(Arena* arena, SSIZE amt) {
	SSIZE pos_old = arena_pos(arena);
	SSIZE pos_new = pos_old;
	if (amt < pos_old)
	{
		pos_new = pos_old - amt;
	}
	arena_pop_to(arena, pos_new);
}

//- rjf: temporary arena scopes

inline TempArena
temp_begin(Arena* arena) {
	U64       pos  = arena_pos(arena);
	TempArena temp = {arena, pos};
	return temp;
}

force_inline void temp_end(TempArena temp) { arena_pop_to(temp.arena, temp.pos); }

// ======================================== DEFAULT_ALLOCATOR =====================================================

#ifndef MD_OVERRIDE_DEFAULT_ALLOCATOR
// The default allocator for this base module is the Arena allocator with a VArena backing
// NOTE(Ed): In order for this to work, either the os entry_point must have been utilized or os_init needs to be called.
inline AllocatorInfo
default_allocator()
{
	local_persist thread_local Arena* arena = nullptr;
	if (arena == nullptr) {
		VArena* backing_vmem = varena_alloc(.flags = 0, .base_addr = 0x0, .reserve_size = VARENA_DEFAULT_RESERVE, .commit_size = VARENA_DEFAULT_COMMIT);
		        arena        = arena_alloc(.backing = varena_allocator(backing_vmem), .block_size = VARENA_DEFAULT_RESERVE);
	}
	AllocatorInfo info = { arena_allocator_proc, arena };
	return info;
}
#endif
