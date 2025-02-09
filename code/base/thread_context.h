#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "arena.h"
#	include "string.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// NOTE(allen): Thread Context

typedef struct TCTX TCTX;
struct TCTX
{
	Arena* arenas[2];
	
	U8  thread_name[32];
	U64 thread_name_size;
	
	char* file_name;
	U64   line_number;
};

////////////////////////////////
// NOTE(allen): Thread Context Functions

MD_API void  tctx_init_and_equip      (TCTX* tctx);
MD_API void  tctx_init_and_equip_alloc(TCTX* tctx, AllocatorInfo ainfo);
MD_API void  tctx_release             (void);
MD_API TCTX* tctx_get_equipped        (void);

MD_API Arena* tctx_get_scratch(Arena** conflicts, U64 count);

void    tctx_set_thread_name(String8 name);
String8 tctx_get_thread_name(void);

void    tctx_write_srcloc(char*  file_name, U64  line_number);
void    tctx_read_srcloc (char** file_name, U64* line_number);
#define tctx_write_this_srcloc() tctx_write_srcloc(__FILE__, __LINE__)

inline TempArena
scratch__begin_alloc(AllocatorInfo ainfo) {
	Arena*    arena   = extract_arena(ainfo);
	TempArena scratch =  temp_begin(tctx_get_scratch(&arena, arena != nullptr));
	return scratch;
}

#define scratch_begin_alloc(ainfo)      scratch__begin_alloc(ainfo)
#define scratch_begin(conflicts, count) temp_begin(tctx_get_scratch((conflicts), (count)))
#define scratch_end(scratch)            temp_end(scratch)

inline void
tctx_set_thread_name(String8 string){
  TCTX* tctx = tctx_get_equipped();
  U64   size = clamp_top(string.size, sizeof(tctx->thread_name));
  memory_copy(tctx->thread_name, string.str, size);
  tctx->thread_name_size = size;
}

inline String8
tctx_get_thread_name(void) {
  TCTX*   tctx   = tctx_get_equipped();
  String8 result = str8(tctx->thread_name, tctx->thread_name_size);
  return(result);
}

inline void
tctx_write_srcloc(char* file_name, U64 line_number){
  TCTX *tctx = tctx_get_equipped();
  tctx->file_name = file_name;
  tctx->line_number = line_number;
}

inline void
tctx_read_srcloc(char** file_name, U64* line_number){
	TCTX* tctx   = tctx_get_equipped();
	*file_name   = tctx->file_name;
	*line_number = tctx->line_number;
}
