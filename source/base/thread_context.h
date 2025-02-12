#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "arena.h"
#	include "string.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(allen): Thread MD_Context

typedef struct MD_TCTX MD_TCTX;
struct MD_TCTX
{
	MD_Arena* arenas[2];
	
	MD_U8  md_thread_name[32];
	MD_U64 thread_name_size;
	
	char* file_name;
	MD_U64   line_number;
};

////////////////////////////////
// NOTE(allen): Thread MD_Context Functions

MD_API void  md_tctx_init_and_equip      (MD_TCTX* md_tctx);
MD_API void  md_tctx_init_and_equip_alloc(MD_TCTX* md_tctx, MD_AllocatorInfo ainfo);
MD_API void  md_tctx_release             (void);
MD_API MD_TCTX* md_tctx_get_equipped        (void);

MD_API MD_Arena* md_tctx_get_scratch(MD_Arena** conflicts, MD_U64 count);

void    md_tctx_set_thread_name(MD_String8 name);
MD_String8 md_tctx_get_thread_name(void);

void    md_tctx_write_srcloc(char*  file_name, MD_U64  line_number);
void    md_tctx_read_srcloc (char** file_name, MD_U64* line_number);
#define md_tctx_write_this_srcloc() md_tctx_write_srcloc(__FILE__, __LINE__)

typedef struct { MD_U64 count; } Opt_ScratchBegin;

inline MD_TempArena
md_scratch_begin__ainfo(MD_AllocatorInfo ainfo, Opt_ScratchBegin opt) {
	MD_Arena*    arena   = md_extract_arena(ainfo);
	MD_TempArena scratch = md_temp_begin(md_tctx_get_scratch(&arena, arena != md_nullptr));
	return scratch;
}

md_force_inline MD_TempArena md_scratch_begin__arena(MD_Arena** arena, Opt_ScratchBegin opt) { MD_TempArena scratch = md_temp_begin(md_tctx_get_scratch(arena, opt.count)); return scratch; }

#define md_scratch_begin(conflicts, ...)        \
_Generic(conflicts,                          \
	int          : md_scratch_begin__arena,     \
	MD_Arena**      : md_scratch_begin__arena,     \
	MD_AllocatorInfo: md_scratch_begin__ainfo,     \
	default      : md_assert_generic_sel_fail \
) md_generic_call(conflicts, (Opt_ScratchBegin){__VA_ARGS__})

#define scratch_end(scratch) md_temp_end(scratch)

inline void
md_tctx_set_thread_name(MD_String8 string){
  MD_TCTX* md_tctx = md_tctx_get_equipped();
  MD_U64   size = md_clamp_top(string.size, sizeof(md_tctx->md_thread_name));
  md_memory_copy(md_tctx->md_thread_name, string.str, size);
  md_tctx->thread_name_size = size;
}

inline MD_String8
md_tctx_get_thread_name(void) {
  MD_TCTX*   md_tctx   = md_tctx_get_equipped();
  MD_String8 result = md_str8(md_tctx->md_thread_name, md_tctx->thread_name_size);
  return(result);
}

inline void
md_tctx_write_srcloc(char* file_name, MD_U64 line_number){
  MD_TCTX *md_tctx = md_tctx_get_equipped();
  md_tctx->file_name = file_name;
  md_tctx->line_number = line_number;
}

inline void
md_tctx_read_srcloc(char** file_name, MD_U64* line_number){
	MD_TCTX* md_tctx   = md_tctx_get_equipped();
	*file_name   = md_tctx->file_name;
	*line_number = md_tctx->line_number;
}
