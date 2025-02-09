#ifdef INTELLISENSE_DIRECTIVES
#	include "entry_point.h"
#	include "profiling.h"
#	include "arena.h"
#	include "thread_context.h"
#	include "os/os.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

void main_thread_base_entry_point(MainThread_EntryPointProc* entry_point, char** arguments, U64 arguments_count)
{
#if PROFILE_TELEMETRY
	local_persist U8 tm_data[MB(64)];
	tmLoadLibrary(TM_RELEASE);
	tmSetMaxThreadCount(256);
	tmInitialize(sizeof(tm_data), (char *)tm_data);
#endif
	thread_namef("[main thread]");

	// TODO(Ed): Review?
	TempArena   scratch = scratch_begin(0, 0);

	String8List command_line_argument_strings = os_string_list_from_argcv(scratch.arena, (int)arguments_count, arguments);
	CmdLine     cmdline                       = cmd_line_from_string_list(scratch.arena, command_line_argument_strings);
	B32         capture                       = cmd_line_has_flag(&cmdline, str8_lit("capture"));
	if (capture) {
		prof_begin_capture(arguments[0]);
	}

	entry_point(&cmdline);

	if (capture) {
		prof_end_capture();
	}

	scratch_end(scratch);
}

void 
supplement_thread_base_entry_point(SupplementThread_EntryPointProc* entry_point, void*  params)
{
	// TODO(Ed): Review?
	TCTX tctx;
	tctx_init_and_equip(&tctx);
	entry_point(params);
	tctx_release();
}
