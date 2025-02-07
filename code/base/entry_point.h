#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "command_line.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

typedef void (MainThread_EntryPointProc)      (CmdLine* cmd_line);
typedef void (SupplementThread_EntryPointProc)(void*    params);

MD_API void main_thread_base_entry_point      (MainThread_EntryPointProc*       entry_point, char** arguments, U64 arguments_count);
MD_API void supplement_thread_base_entry_point(SupplementThread_EntryPointProc* entry_point, void*  params);
