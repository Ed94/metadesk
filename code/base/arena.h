#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "macros.h"
#	include "base_types.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Constants

#define MD_ARENA_HEADER_SIZE 64

////////////////////////////////
//~ rjf: Types

typedef U32 ArenaFlags;
enum
{
	ArenaFlag_NoChain    = ( 1 << 0),
	ArenaFlag_LargePages = ( 1 << 1),
};

typedef struct ArenaParams ArenaParams;
struct ArenaParams
{
	ArenaFlags flags;
	U64        reserve_size;
	U64        commit_size;
	void*      optional_backing_buffer;
};

typedef struct Arena Arena;
struct Arena
{
	Arena*     prev;    // previous arena in chain
	Arena*     current; // current arena in chain
	ArenaFlags flags;
	U32        cmt_size;
	U64        res_size;
	U64        base_pos;
	U64        pos;
	U64        cmt;
	U64        res;
};
static_assert(sizeof(Arena) <= MD_ARENA_HEADER_SIZE, "sizeof(Arena) <= MD_ARENA_HEADER_SIZE");

typedef struct TempArena TempArena;
struct TempArena
{
	Arena* arena;
	U64    pos;
};

////////////////////////////////
//~ rjf: Arena Functions

//- rjf: arena creation/destruction
internal Arena* arena_alloc_(ArenaParams *params);
#define         arena_alloc(...) arena_alloc_( & ( ArenaParams) { .reserve_size = MB(64), .commit_size = KB(64), __VA_ARGS__ } )

internal void arena_release(Arena *arena);

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
#define push_array_no_zero_aligned(a, T, c, align) (T *)arena_push((a), sizeof(T) * (c), (align))
#define push_array_aligned(a, T, c, align)         (T *)memory_zero(push_array_no_zero_aligned(a, T, c, align), sizeof(T) * (c))
#define push_array_no_zero(a, T, c)                     push_array_no_zero_aligned(a, T, c, max(8, align_of(T)))
#define push_array(a, T, c)                             push_array_aligned        (a, T, c, max(8, align_of(T)))
