#pragma once

#include "gen_common.h"

#define path_refactor_script "./c11.refactor"

gen_Code refactor( gen_Code code ) {
	return code_refactor_and_format(code, path_scratch_file, path_refactor_script, nullptr );
}
gen_Code refactor_and_format( gen_Code code ) {
	return code_refactor_and_format(code, path_scratch_file, path_refactor_script, path_format_style );
}

int main()
{
	gen_Context ctx = {0};
	gen_init(& ctx);

	register_library_macros();

	gen_Str generation_notice = lit(
		"// This file was generated automatially by metadesk's gen_c11.c  "
		"(See: https://github.com/Ed94/metadesk/tree/master)\n\n"
	);

	gen_Str roll_own_dependencies_guard_start = lit(
		"\n"
		"//! If its desired to roll your own dependencies, define MD_ROLL_OWN_DEPENDENCIES before including this file.\n"
		"#ifndef MD_ROLL_OWN_DEPENDENCIES\n"
		"\n"
	);
	
	gen_Str roll_own_dependencies_guard_end = lit(
		"\n"
		"// MD_ROLL_OWN_DEPENDENCIES\n"
		"#endif\n"
		"\n"
	);

	gen_Code tp_stb_sprintf_h = gen_scan_file(path_third_party "stb/stb_sprintf.h");

	gen_Code base_context_cracking_h = gen_scan_file(path_base "context_cracking.h");
	gen_Code base_platform_h         = gen_scan_file(path_base "platform.h");
	gen_Code base_linkage_h          = gen_scan_file(path_base "linkage.h");
	gen_Code base_macros_h           = gen_scan_file(path_base "macros.h");
	gen_Code base_generic_macros_h   = gen_scan_file(path_base "generic_macros.h");
	gen_Code base_profiling_h        = gen_scan_file(path_base "profiling.h");
	gen_Code base_base_types_h       = gen_scan_file(path_base "base_types.h");
	gen_Code base_ring_h             = gen_scan_file(path_base "ring.h");
	gen_Code base_debug_h            = gen_scan_file(path_base "debug.h");
	gen_Code base_memory_h           = gen_scan_file(path_base "memory.h");
	gen_Code base_memory_substrate_h = gen_scan_file(path_base "memory_substrate.h");
	gen_Code base_arena_h            = gen_scan_file(path_base "arena.h");
	gen_Code base_space_h            = gen_scan_file(path_base "space.h");
	gen_Code base_math_h             = gen_scan_file(path_base "math.h");
	gen_Code base_sort_h             = gen_scan_file(path_base "sort.h");
	gen_Code base_toolchain_h        = gen_scan_file(path_base "toolchain.h");
	gen_Code base_time_h             = gen_scan_file(path_base "time.h");
	gen_Code base_strings_h          = gen_scan_file(path_base "strings.h");
	gen_Code base_text_h             = gen_scan_file(path_base "text.h");
	gen_Code base_thread_context_h   = gen_scan_file(path_base "thread_context.h");
	gen_Code base_command_line_h     = gen_scan_file(path_base "command_line.h");
	gen_Code base_markup_h           = gen_scan_file(path_base "markup.h");
	gen_Code base_logger_h           = gen_scan_file(path_base "logger.h");
	gen_Code base_entry_point_h      = gen_scan_file(path_base "entry_point.h");
	gen_Code base_file_h             = gen_scan_file(path_base "file.h");

	gen_Code os_h                    = gen_scan_file(path_os       "os.h");
	gen_Code os_win32_includes_h     = gen_scan_file(path_os_win32 "os_win32_includes.h");
	gen_Code os_win32_h              = gen_scan_file(path_os_win32 "os_win32.h");
	gen_Code os_linux_includes_h     = gen_scan_file(path_os_linux "os_linux_includes.h");
	gen_Code os_linux_h              = gen_scan_file(path_os_linux "os_linux.h");

	// Singleheader
	{
		gen_Builder header_ = gen_builder_open(path_gen "metadesk_singleheader.h");
		gen_Builder* header = & header_;

		gen_Str implementation_guard_start = lit(
			"\n"
			"#pragma region METADESK IMPLEMENTATION GUARD\n"
			"#if defined(MD_IMPLEMENTATION) && ! defined(MD_IMPLEMENTED)\n"
			"#define MD_IMPLEMENTED\n"
			"\n"
		);
		gen_Str implementation_guard_end = lit(
			"\n"
			"// END: MD_IMPLEMENTATION\n"
			"#endif\n"
			"#pragma endregion METADESK IMPLEMENTATION GUARD\n"
			"\n"
		);

		// Header files

		gen_builder_print_fmt(header, "%S", generation_notice);

		gen_builder_write(header);
	}

	// Segregated
	{

	}

	// gen_deinit(& ctx);
	return 0;
}
