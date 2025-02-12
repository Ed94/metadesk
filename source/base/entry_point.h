#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "command_line.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

typedef void (MD_MainThread_EntryPointProc)      (MD_CmdLine* cmd_line);
typedef void (MD_SupplementThread_EntryPointProc)(void*    params);

MD_API void md_main_thread_base_entry_point      (MD_MainThread_EntryPointProc*       md_entry_point, char** arguments, MD_U64 arguments_count);
MD_API void md_supplement_thread_base_entry_point(MD_SupplementThread_EntryPointProc* md_entry_point, void*  params);
