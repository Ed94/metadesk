#pragma once

#include "gen_common.h"

#ifndef PRINT_SECTION_REGION_PRAGMAS
#define PRINT_SECTION_REGION_PRAGMAS 1
#endif

#define path_refactor_script path_gen_c11 "c11.refactor"

gen_Code refactor( gen_Code code ) {
	return code_refactor_and_format(code, path_scratch_file, path_refactor_script, nullptr );
}
gen_Code refactor_and_format( gen_Code code ) {
	return code_refactor_and_format(code, path_scratch_file, path_refactor_script, path_format_style );
}

#define str_fmt(fmt, ...) gen_strbuilder_to_str(gen_strbuilder_fmt_buf(gen_get_context()->Allocator_Temp, fmt, __VA_ARGS__))

void print_section(gen_Builder* builder, gen_Code code, gen_Str label) {
	if (PRINT_SECTION_REGION_PRAGMAS) gen_builder_print(builder, gen_def_pragma(str_fmt("region %S", label)) );
	gen_builder_print(builder, code);
	if (PRINT_SECTION_REGION_PRAGMAS) gen_builder_print(builder, gen_def_pragma(str_fmt("endregion %S", label)) );
	gen_builder_print(builder, gen_fmt_newline);
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

	gen_Code os_h                = gen_scan_file(path_os       "os.h");
	gen_Code os_win32_includes_h = gen_scan_file(path_os_win32 "os_win32_includes.h");
	gen_Code os_win32_h          = gen_scan_file(path_os_win32 "os_win32.h");
	gen_Code os_linux_includes_h = gen_scan_file(path_os_linux "os_linux_includes.h");
	gen_Code os_linux_h          = gen_scan_file(path_os_linux "os_linux.h");

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
	#pragma endregion Refactored / Formatted

	// Singleheader
	{
		gen_Builder header_ = gen_builder_open(path_gen "metadesk_singleheader.h");
		gen_Builder* header = & header_;

		#define print(code)                 gen_builder_print(header, code);
		#define print_fmt(fmt, ...)         gen_builder_print_fmt(header, fmt, __VA_ARGS__)
		#define new_line()                  gen_builder_print(header, gen_fmt_newline)
		#define pragma_region(label)        print(gen_def_pragma(str_fmt("region %S", lit(label))))
		#define pragma_endregion(label)     print(gen_def_pragma(str_fmt("endregion %S", lit(label))))
		#define define                      gen_def_define
		#define preprocess_cond(type, expr) gen_def_preprocess_cond(type, expr)

		gen_Str implementation_guard_start = lit(
			// "#pragma region METADESK IMPLEMENTATION GUARD\n"
			"#if defined(MD_IMPLEMENTATION) && ! defined(MD_IMPLEMENTED)\n"
			"#define MD_IMPLEMENTED\n"
		);
		gen_Str implementation_guard_end = lit(
			"// END: MD_IMPLEMENTATION\n"
			"#endif\n"
			// "#pragma endregion METADESK IMPLEMENTATION GUARD\n"
		);

		// Header files

		print_fmt("%S", generation_notice);
		print_fmt("%S", lit("#pragma once\n\n"));

		print(gen_scan_file(path_gen_c11 "header_start.h"));
		new_line();

		pragma_region("Base");
		print(gen_scan_file(path_gen_c11 "base_banner.h"));
		new_line();
		new_line();

		print_section(header, r_base_context_cracking_h, lit("Context Cracking"));
		print_section(header, r_base_platform_h,         lit("platform"));
		print_section(header, r_base_linkage_h,          lit("Linkage"));
		print_section(header, r_base_macros_h,           lit("Macros"));
		print_section(header, r_base_generic_macros_h,   lit("_Generic Macros"));
		print_section(header, r_base_profiling_h,        lit("Profiling"));

		define(lit("STB_SPRINTF_IMPLEMENTATION"), MT_Statement);
		print_fmt("%S", implementation_guard_start);
		print(gen_def_define(lit("STB_SPRINTF_DECORATE(name)"), MT_Expression, .content = lit("md_##name")));
		// print();
		print(preprocess_cond(PreprocessCond_If, lit("MD_BUILD_STATIC"))); new_line(); {
			define(lit("STB_BUILD_STATIC"), MT_Statement);
		}
		print(gen_preprocess_endif);
		new_line();

		print_section(header, r_tp_stb_sprintf_h, lit("STB snprintf Header"));
		print_fmt("%S", implementation_guard_end);
		new_line();

		print_section(header, r_base_base_types_h, lit("Types"));
		print_section(header, r_base_ring_h, lit("Ring"));
		print_section(header, r_base_debug_h, lit("Debug"));
		print_section(header, r_base_memory_h, lit("Memory"));
		print_section(header, r_base_memory_substrate_h, lit("Memory Substrate"));
		print_section(header, r_base_arena_h, lit("Arena"));
		print_section(header, r_base_space_h, lit("Space"));
		print_section(header, r_base_math_h, lit("Math"));
		print_section(header, r_base_sort_h, lit("Sort"));
		print_section(header, r_base_toolchain_h, lit("Toolchain"));
		print_section(header, r_base_time_h, lit("Time"));
		print_section(header, r_base_strings_h, lit("strings"));
		print_section(header, r_base_text_h, lit("Text"));
		print_section(header, r_base_thread_context_h, lit("Thread Context"));
		print_section(header, r_base_command_line_h, lit("Command Line"));
		print_section(header, r_base_markup_h, lit("Markup"));
		print_section(header, r_base_logger_h, lit("Logger"));
		print_section(header, r_base_entry_point_h, lit("Entry Point"));
		print_section(header, r_base_file_h, lit("File"));

		pragma_endregion("Base");

		

		gen_builder_write(header);
	}

	// Segregated
	{

	}

	// gen_deinit(& ctx);
	return 0;
}
