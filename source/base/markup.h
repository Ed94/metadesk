#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "linkage.h"
#	include "strings.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

MD_API void md_set_thread_name(MD_String8 string);

inline void
md_set_thread_namef(char *fmt, ...)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);

	va_list args;
	va_start(args, fmt);
	MD_String8 string = md_str8fv(scratch.arena, fmt, args);
	md_set_thread_name(string);
	va_end(args);

	scratch_end(scratch);
}

#define md_thread_namef(...) (md_set_thread_namef(__VA_ARGS__))
#define md_thread_name(str)  (md_set_thread_name(str))
