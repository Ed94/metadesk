#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "linkage.h"
#	include "strings.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

MD_API void set_thread_name(String8 string);

inline void
set_thread_namef(char *fmt, ...)
{
	TempArena scratch = scratch_begin(0, 0);

	va_list args;
	va_start(args, fmt);
	String8 string = push_str8fv(scratch.arena, fmt, args);
	set_thread_name(string);
	va_end(args);

	scratch_end(scratch);
}

#define thread_namef(...) (set_thread_namef(__VA_ARGS__))
#define thraed_name(str)  (set_thread_name(str))
