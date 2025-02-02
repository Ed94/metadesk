#ifdef MD_INTELLISENSE_DIRECTIVES
#	pragma once
#	include "linkage.h"
#	include "strings.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

internal void set_thread_name(String8 string);
internal void set_thread_namef(char *fmt, ...);
#define ThreadNameF(...) (set_thread_namef(__VA_ARGS__))
#define ThreadName(str)  (set_thread_name(str))
