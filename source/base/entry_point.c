#ifdef INTELLISENSE_DIRECTIVES
#	include "entry_point.h"
#	include "profiling.h"
#	include "arena.h"
#	include "thread_context.h"
#	include "os/os.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

void md_main_thread_base_entry_point(MD_MainThread_EntryPointProc* md_entry_point, char** arguments, MD_U64 arguments_count)
{
#if MD_PROFILE_TELEMETRY
	md_local_persist MD_U8 tm_data[MD_MB(64)];
	tmLoadLibrary(TM_RELEASE);
	tmSetMaxThreadCount(256);
	tmInitialize(sizeof(tm_data), (char *)tm_data);
#endif
	md_thread_namef("[main thread]");

	MD_TempArena   scratch = md_scratch_begin(0, 0);

	MD_String8List command_line_argument_strings = md_os_string_list_from_argcv(scratch.arena, (int)arguments_count, arguments);
	MD_CmdLine     cmdline                       = md_cmd_line_from_string_list(scratch.arena, command_line_argument_strings);
	MD_B32         capture                       = md_cmd_line_has_flag(&cmdline, md_str8_lit("capture"));
	if (capture) {
		md_prof_begin_capture(arguments[0]);
	}

	md_entry_point(&cmdline);

	if (capture) {
		md_prof_end_capture();
	}

	scratch_end(scratch);
}

void 
md_supplement_thread_base_entry_point(MD_SupplementThread_EntryPointProc* md_entry_point, void*  params)
{
	MD_TCTX md_tctx;
	md_tctx_init_and_equip(&md_tctx);
	md_entry_point(params);
	md_tctx_release();
}
