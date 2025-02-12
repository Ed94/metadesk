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

typedef MD_U32 MD_ArenaFlags;
enum
{
	// Don't chain this arena
	MD_ArenaFlag_NoChain        = (1 << 0), 
	// Only relevant if backing is virtual memory, will prevent allocating a new backing MD_VArena when the current block exhausts
	// Will assume backing can chain multiple block_size arenas however. If there is an allocation failure it will md_assert.
	MD_ArenaFlag_NoChainVirtual = (1 << 1),
	// Backing allocator identified as MD_VArena during initialization
	MD_ArenaFlag_Virtual        = (1 << 2),
};

typedef struct MD_ArenaParams MD_ArenaParams;
struct MD_ArenaParams
{
	MD_AllocatorInfo backing;
	MD_ArenaFlags    flags;
	MD_U64           block_size; // If chaining VArenas set this to the reserve size
};

#define MD_ARENA_DEFAULT_BLOCK_SIZE MD_VARENA_DEFAULT_RESERVE - md_align_pow2(size_of(MD_VArena), MD_DEFAULT_MEMORY_ALIGNMENT)

/* NOTE(Ed): The original metadesk arena is a combination of several concepts into a single interface
	* An OS virtual memory allocation scheme
	* A arena 'block' of memory with segmented chaining of the blocks
	* A push/pop stack allocation interface for the arena

	The virtual memory has been abstracted into a backing allocator,
	and chaining still supports reserving new virtual address regions .
	(can be disabled with MD_ArenaFlag_NoChainVirtual)

	If large pages are desired, see MD_VArena.
*/

typedef struct MD_Arena MD_Arena;
struct MD_Arena
{
	MD_Arena*        prev;     // Previous arena in chain
	MD_Arena*        current;  // Current arena in chain
	MD_AllocatorInfo backing;
	MD_SSIZE         base_pos; // Tracks how main arenas have been chained
	MD_SSIZE         pos;
	MD_SSIZE         block_size;
	MD_ArenaFlags    flags;
};
// static_assert(size_of(MD_Arena) <= ARENA_HEADER_SIZE, "sizeof(MD_Arena) <= ARENA_HEADER_SIZE");

typedef struct MD_TempArena MD_TempArena;
struct MD_TempArena
{
	MD_Arena* arena;
	MD_SSIZE  pos;
};

////////////////////////////////
//~ rjf: MD_Arena Functions

MD_API void* md_arena_allocator_proc(void* allocator_data, MD_AllocatorMode mode, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags);

md_force_inline MD_AllocatorInfo md_arena_allocator(MD_Arena* arena) { MD_AllocatorInfo info = { md_arena_allocator_proc, arena}; return info; }

// Useful for providing an arena to md_scratch_begin_alloc
md_force_inline MD_Arena*
md_extract_arena(MD_AllocatorInfo ainfo) {
	if (md_allocator_type(ainfo) == MD_AllocatorType_Arena) {
		return (MD_Arena*) ainfo.data;
	}
	return md_nullptr;
}

//- rjf: arena creation/destruction

MD_API MD_Arena* md_arena__alloc(MD_ArenaParams* params);
#define       md_arena_alloc(...) md_arena__alloc( &(MD_ArenaParams){ __VA_ARGS__ } )

void md_arena_release(MD_Arena *arena);

//- rjf: arena push/pop/pos core functions

MD_API void* md_arena_push  (MD_Arena* arena, MD_SSIZE size, MD_SSIZE align);
       MD_U64   md_arena_pos   (MD_Arena* arena);
MD_API void  md_arena_pop_to(MD_Arena* arena, MD_SSIZE pos);

//- rjf: arena push/pop helpers

void md_arena_clear(MD_Arena* arena);
void md_arena_pop  (MD_Arena* arena, MD_SSIZE amt);

//- rjf: temporary arena scopes

MD_TempArena temp_arena_begin(MD_Arena* arena);
void      temp_arena_end(MD_TempArena temp);

//- rjf: push helper macros

#ifndef md_push_array_
#define md_push_array__no_zero_aligned(a, T, c, align) (T *)md_arena_push((a), sizeof(T) * (c), (align))
#define md_push_array__aligned(a, T, c, align)         (T *)md_memory_zero(md_push_array__no_zero_aligned(a, T, c, align), sizeof(T) * (c))
#define md_push_array__no_zero(a, T, c)                     md_push_array__no_zero_aligned(a, T, c, md_max(8, md_align_of(T)))
#define md_push_array_(a, T, c)                             md_push_array__aligned        (a, T, c, md_max(8, md_align_of(T)))
#endif

// Inlines

inline void
md_arena_release(MD_Arena* arena) {
	for (MD_Arena* n = arena->current, *prev = 0; n != 0; n = prev) {
		prev = n->prev;
		md_alloc_free(arena->backing, n);
	}
}

inline MD_U64
md_arena_pos(MD_Arena *arena) {
	MD_Arena* current = arena->current;
	MD_U64    pos     = current->base_pos + current->pos;
	return pos;
}

//- rjf: arena push/pop helpers

md_force_inline void md_arena_clear(MD_Arena* arena) { md_arena_pop_to(arena, 0); }

inline void
md_arena_pop(MD_Arena* arena, MD_SSIZE amt) {
	MD_SSIZE pos_old = md_arena_pos(arena);
	MD_SSIZE pos_new = pos_old;
	if (amt < pos_old)
	{
		pos_new = pos_old - amt;
	}
	md_arena_pop_to(arena, pos_new);
}

//- rjf: temporary arena scopes

inline MD_TempArena
md_temp_begin(MD_Arena* arena) {
	MD_U64       pos  = md_arena_pos(arena);
	MD_TempArena temp = {arena, pos};
	return temp;
}

md_force_inline void md_temp_end(MD_TempArena temp) { md_arena_pop_to(temp.arena, temp.pos); }

// ======================================== DEFAULT_ALLOCATOR =====================================================

#ifndef MD_OVERRIDE_DEFAULT_ALLOCATOR
// The default allocator for this base module is the MD_Arena allocator with a MD_VArena backing
// NOTE(Ed): In order for this to work, either the os md_entry_point must have been utilized or md_os_init needs to be called.
inline MD_AllocatorInfo
md_default_allocator()
{
	md_local_persist md_thread_local MD_Arena* arena = md_nullptr;
	if (arena == md_nullptr) {
		MD_VArena* backing_vmem = md_varena_alloc(.flags = 0, .base_addr = 0x0, .reserve_size = MD_VARENA_DEFAULT_RESERVE, .commit_size = MD_VARENA_DEFAULT_COMMIT);
		        arena        = md_arena_alloc(.backing = md_varena_allocator(backing_vmem), .block_size = MD_VARENA_DEFAULT_RESERVE);
	}
	MD_AllocatorInfo info = { md_arena_allocator_proc, arena };
	return info;
}
#endif
