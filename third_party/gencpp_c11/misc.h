#pragma once

// #define 
#define GEN_EXPOSE_BACKEND
#include "gencpp_c11.h"

// Will format a file with the given style at the provided path.
// Assumes clang-format is defined in an user-exposed or system enviornment PATH.
void clang_format_file( char const* path, char const* style_path )
{
	GEN_ASSERT_NOT_NULL(path);
	gen_StrBuilder resolved_path = gen_strbuilder_make_str(gen_get_context()->Allocator_Temp, gen_to_str_from_c_str(path));
	gen_StrBuilder style_arg;
	if (style_path) {
		style_arg = gen_strbuilder_make_str(gen_get_context()->Allocator_Temp, gen_txt("-style=file:"));
		gen_strbuilder_append_fmt( & style_arg, "%s ", style_path );
	}

	gen_Str clang_format      = gen_txt("clang-format ");
	gen_Str cf_format_inplace = gen_txt("-i ");
	gen_Str cf_verbose        = gen_txt("-verbose ");

	gen_StrBuilder command = gen_strbuilder_make_str( gen_get_context()->Allocator_Temp, clang_format );
	gen_strbuilder_append_str   ( & command, cf_format_inplace );
	gen_strbuilder_append_str   ( & command, cf_verbose );
	gen_strbuilder_append_string( & command, style_arg );
	gen_strbuilder_append_string( & command, resolved_path );
	system( command );
}

// Will refactor a file with the given script at the provided path.
// Assumes refactor is defined in an user-exposed or system enviornment PATH.
// (See: ./gencpp/scripts/build.ci.ps1 for how)
void refactor_file( char const* path, char const* refactor_script )
{
	GEN_ASSERT_NOT_NULL(path);
	GEN_ASSERT_NOT_NULL(refactor_script);

	gen_StrBuilder command = gen_strbuilder_make_str(gen_get_context()->Allocator_Temp, gen_txt("refactor "));
	// strbuilder_append_str( & command, gen_txt("-debug ") );
	gen_strbuilder_append_str( & command, gen_txt("-num=1 ") );
	gen_strbuilder_append_fmt( & command, "-src=%s ", path );
	gen_strbuilder_append_fmt( & command,"-spec=%s ", refactor_script );
	system(command);
	gen_log_fmt("\n");
}

// Does either of the above or both to the provided code.
// Code returned will be untyped content (its be serialized)
gen_Code code_refactor_and_format( gen_Code code, char const* scratch_path, char const* refactor_script, char const* clang_format_sytle_path )
{
	GEN_ASSERT(code);
	GEN_ASSERT_NOT_NULL(scratch_path);

	gen_Builder scratch_file = gen_builder_open( scratch_path );
	gen_builder_print( & scratch_file, code);
	gen_builder_write(& scratch_file);

	if (refactor_script) {
		refactor_file(scratch_path, refactor_script);
	}
	if ( clang_format_sytle_path ) {
		clang_format_file(scratch_path, clang_format_sytle_path);
	}

	gen_Code result = gen_scan_file( scratch_path );
	remove(scratch_path);
	return result;
}
