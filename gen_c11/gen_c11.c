#pragma once

#include "gen_common.h"

#ifndef GENERATE_SEGREGATED
#define GENERATE_SEGEREGATED 1
#endif

#ifndef GENERATE_SINGLEHEADER
#define GENERATE_SINGLEHEADER 1
#endif

#define path_refactor_script path_gen_c11 "c11.refactor"

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

	gen_Code banner_base    = gen_scan_file(path_gen_c11 "banner_base.h");
	gen_Code banner_os      = gen_scan_file(path_gen_c11 "banner_os.h");
	gen_Code banner_mdesk   = gen_scan_file(path_gen_c11 "banner_mdesk.h");
	gen_Code banner_md_impl = gen_scan_file(path_gen_c11 "banner_md_impl.h");

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

	gen_Code os_h                = gen_scan_file(path_os       "os.h");
	gen_Code os_win32_includes_h = gen_scan_file(path_os_win32 "os_win32_includes.h");
	gen_Code os_win32_h          = gen_scan_file(path_os_win32 "os_win32.h");
	gen_Code os_linux_includes_h = gen_scan_file(path_os_linux "os_linux_includes.h");
	gen_Code os_linux_h          = gen_scan_file(path_os_linux "os_linux.h");

	gen_Code mdesk_h = gen_scan_file(path_mdesk "mdesk.h");

	gen_Code base_platfom_c          = gen_scan_file(path_base "platform.c");
	gen_Code base_debug_c            = gen_scan_file(path_base "debug.c");
	gen_Code base_memory_substrate_c = gen_scan_file(path_base "memory_substrate.c");
	gen_Code base_arena_c            = gen_scan_file(path_base "arena.c");
	gen_Code base_strings_c          = gen_scan_file(path_base "strings.c");
	gen_Code base_text_c             = gen_scan_file(path_base "text.c");
	gen_Code base_thread_context_c   = gen_scan_file(path_base "thread_context.c");
	gen_Code base_markup_c           = gen_scan_file(path_base "markup.c");
	gen_Code base_command_line_c     = gen_scan_file(path_base "command_line.c");
	gen_Code base_logger_c           = gen_scan_file(path_base "logger.c");
	gen_Code base_entry_point_c      = gen_scan_file(path_base "entry_point.c");
	gen_Code base_time_c             = gen_scan_file(path_base "time.c");

	gen_Code os_win32_c = gen_scan_file(path_os_win32 "os_win32.c");
	gen_Code os_linux_c = gen_scan_file(path_os_linux "os_linux.c");
	gen_Code os_os_c    = gen_scan_file(path_os       "os.c");

	gen_Code mdesk_c = gen_scan_file(path_mdesk "mdesk.c");

	#pragma region Refactored / Formatted
	gen_Code r_tp_stb_sprintf_h = refactor(tp_stb_sprintf_h);

	gen_Code r_base_context_cracking_h = refactor(base_context_cracking_h);
	gen_Code r_base_platform_h         = refactor(base_platform_h);
	gen_Code r_base_linkage_h          = refactor(base_linkage_h);
	gen_Code r_base_macros_h           = refactor(base_macros_h);
	gen_Code r_base_generic_macros_h   = refactor(base_generic_macros_h);
	gen_Code r_base_profiling_h        = refactor(base_profiling_h);
	gen_Code r_base_base_types_h       = refactor(base_base_types_h);
	gen_Code r_base_ring_h             = refactor(base_ring_h);
	gen_Code r_base_debug_h            = refactor(base_debug_h);
	gen_Code r_base_memory_h           = refactor(base_memory_h);
	gen_Code r_base_memory_substrate_h = refactor(base_memory_substrate_h);
	gen_Code r_base_arena_h            = refactor(base_arena_h);
	gen_Code r_base_space_h            = refactor(base_space_h);
	gen_Code r_base_math_h             = refactor_and_format(base_math_h);
	gen_Code r_base_sort_h             = refactor(base_sort_h);
	gen_Code r_base_toolchain_h        = refactor(base_toolchain_h);
	gen_Code r_base_time_h             = refactor(base_time_h);
	gen_Code r_base_strings_h          = refactor_and_format(base_strings_h);
	gen_Code r_base_text_h             = refactor(base_text_h);
	gen_Code r_base_thread_context_h   = refactor(base_thread_context_h);
	gen_Code r_base_command_line_h     = refactor(base_command_line_h);
	gen_Code r_base_markup_h           = refactor(base_markup_h);
	gen_Code r_base_logger_h           = refactor(base_logger_h);
	gen_Code r_base_entry_point_h      = refactor(base_entry_point_h);
	gen_Code r_base_file_h             = refactor(base_file_h);

	gen_Code r_os_h                = refactor(os_h);
	gen_Code r_os_win32_includes_h = refactor(os_win32_includes_h);
	gen_Code r_os_win32_h          = refactor(os_win32_h);
	gen_Code r_os_linux_includes_h = refactor(os_linux_includes_h);
	gen_Code r_os_linux_h          = refactor(os_linux_h);

	gen_Code r_mdesk_h = refactor(mdesk_h);

	gen_Code r_base_platform_c         = refactor(base_platfom_c);
	gen_Code r_base_debug_c            = refactor(base_debug_c);
	gen_Code r_base_memory_substrate_c = refactor(base_memory_substrate_c);
	gen_Code r_base_arena_c            = refactor(base_arena_c);
	gen_Code r_base_strings_c          = refactor(base_strings_c);
	gen_Code r_base_text_c             = refactor(base_text_c);
	gen_Code r_base_thread_context_c   = refactor(base_thread_context_c);
	gen_Code r_base_markup_c           = refactor(base_markup_c);
	gen_Code r_base_command_line_c     = refactor(base_command_line_c);
	gen_Code r_base_logger_c           = refactor(base_logger_c);
	gen_Code r_base_entry_point_c      = refactor(base_entry_point_c);
	gen_Code r_base_time_c             = refactor(base_time_c);

	gen_Code r_os_win32_c = refactor(os_win32_c);
	gen_Code r_os_linux_c = refactor(os_linux_c);
	gen_Code r_os_c       = refactor(os_os_c);

	gen_Code r_mdesk_c = refactor(mdesk_c);
	#pragma endregion Refactored / Formatted

	if (GENERATE_SINGLEHEADER)
	{
		#define builder header
		gen_Builder header_ = gen_builder_open(path_gen "metadesk_singleheader.h");
		gen_Builder* header = & header_;

		gen_Str implementation_guard_start = lit(
			// "#pragma region METADESK IMPLEMENTATION GUARD\n"
			"#if defined(MD_IMPLEMENTATION) && ! defined(MD_IMPLEMENTED)\n"
			// "#define MD_IMPLEMENTED\n"
		);
		gen_Str implementation_guard_end = lit(
			"// END: MD_IMPLEMENTATION\n"
			"#endif\n"
			// "#pragma endregion METADESK IMPLEMENTATION GUARD\n"
		);

		// Header files

		preprocess_ifndef("METADESK_HEADER");
		define(lit("METADESK_HEADER"), MT_Statement);
		new_line();

		print_fmt("%S", generation_notice);

		print(gen_scan_file(path_gen_c11 "banner_singleheader.h"));
		new_line();

		pragma_region("Base");
		print(banner_base);
		new_line();

		print_section(r_base_context_cracking_h, lit("Context Cracking"));
		print_section(r_base_platform_h,         lit("platform"));
		print_section(r_base_linkage_h,          lit("Linkage"));
		print_section(r_base_macros_h,           lit("Macros"));
		print_section(r_base_generic_macros_h,   lit("_Generic Macros"));
		print_section(r_base_profiling_h,        lit("Profiling"));

		print_fmt("%S", implementation_guard_start);
		new_line();

		define(lit("STB_SPRINTF_IMPLEMENTATION"), MT_Statement);
		define(lit("STB_SPRINTF_DECORATE(name)"), MT_Expression, .content = lit("md_##name"), .flags = MF_Functional);
		// print();
		preprocess_if("MD_BUILD_STATIC"); {
			define(lit("STB_BUILD_STATIC"), MT_Statement);
		}
		preprocess_endif();
		new_line();

		print_section(r_tp_stb_sprintf_h, lit("STB snprintf Header"));
		print_fmt("%S", implementation_guard_end);
		new_line();

		print_section(r_base_base_types_h,       lit("Types"));
		print_section(r_base_ring_h,             lit("Ring"));
		print_section(r_base_debug_h,            lit("Debug"));
		print_section(r_base_memory_h,           lit("Memory"));
		print_section(r_base_memory_substrate_h, lit("Memory Substrate"));
		print_section(r_base_arena_h,            lit("Arena"));
		print_section(r_base_space_h,            lit("Space"));
		print_section(r_base_math_h,             lit("Math"));
		print_section(r_base_sort_h,             lit("Sort"));
		print_section(r_base_toolchain_h,        lit("Toolchain"));
		print_section(r_base_time_h,             lit("Time"));
		print_section(r_base_strings_h,          lit("strings"));
		print_section(r_base_text_h,             lit("Text"));
		print_section(r_base_thread_context_h,   lit("Thread Context"));
		print_section(r_base_command_line_h,     lit("Command Line"));
		print_section(r_base_markup_h,           lit("Markup"));
		print_section(r_base_logger_h,           lit("Logger"));
		print_section(r_base_entry_point_h,      lit("Entry Point"));
		print_section(r_base_file_h,             lit("File"));

		pragma_endregion("Base");
		new_line();

		pragma_region("Operating System");
		print(banner_os);
		new_line();

		print_section(r_os_h, lit("OS"));
		
		preprocess_if("!defined(MD_OS_FEATURE_GRAPHICAL)"); {
			define(lit("MD_OS_FEATURE_GRAPHICAL"), MT_Expression, .content = lit("0") );
		}
		preprocess_endif();
		new_line();

		preprocess_if("!defined(MD_OS_GFX_STUB)"); {
			define(lit("MD_OS_GFX_STUB"), MT_Expression, .content = lit("0") );
		}
		preprocess_endif();
		new_line();

		preprocess_if("MD_OS_WINDOWS"); {
			print_section(r_os_win32_includes_h, lit("Win32 Includes"));
			print_section(r_os_win32_h, lit("OS Win32"));
		}
		comment("#if MD_OS_WINDOWS"); new_line();
		preprocess_elif("MD_OS_LINUX"); {
			print_section(r_os_linux_includes_h, lit("Linux Includes"));
			print_section(r_os_linux_h, lit("OS Linux"));
		}
		comment("#elif OS_LINUX");
		preprocess_endif();

		pragma_endregion("Operting System")
		new_line();

		pragma_region("MDesk")
		print(banner_mdesk);
		new_line();

		print(r_mdesk_h);

		pragma_endregion("MDesk")
		new_line();

		comment("END: METADESK_HEADER")
		preprocess_endif();

		// Source files

		print(banner_md_impl);
		print_fmt("%S", implementation_guard_start);
		define(lit("MD_IMPLEMENED"), MT_Statement);
		new_line();

		pragma_region("Base");
		print(banner_base);
		new_line();

		print_section(r_base_platform_c,         lit("Platform"));
		print_section(r_base_memory_substrate_c, lit("Memory Substrate"));
		print_section(r_base_arena_c,            lit("Arena"));
		print_section(r_base_strings_c,          lit("Strings"));
		print_section(r_base_text_c,             lit("Text"));
		print_section(r_base_thread_context_c,   lit("Thread Context"));
		print_section(r_base_markup_c,           lit("Markup"));
		print_section(r_base_command_line_c,     lit("Command Line"));
		print_section(r_base_logger_c,           lit("Logger"));
		print_section(r_base_entry_point_c,      lit("Entry Point"));
		print_section(r_base_time_c,             lit("Time"));

		pragma_endregion("Base");
		new_line();

		pragma_region("Operating System");
		print(banner_os);
		new_line();

		preprocess_if("MD_OS_WINDOWS"); {
			print(r_os_win32_c)
		}
		preprocess_elif("MD_OS_LINUX"); {
			print(r_os_linux_c);
		}
		preprocess_endif();

		print(r_os_c);;

		pragma_endregion("Operating System");
		new_line();

		pragma_region("MDesk");
		print(banner_mdesk);
		new_line();

		print(r_mdesk_c);

		pragma_endregion("MDesk");
		new_line();

		print_fmt("%S", implementation_guard_end);
		new_line();

		gen_builder_write(header);
		#undef builder
	}

	if (GENERATE_SEGEREGATED)
	{

	}

	// gen_deinit(& ctx);
	return 0;
}
