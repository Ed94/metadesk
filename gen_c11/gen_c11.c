#pragma once

#include "gen_common.h"

#define path_refactor_script "./c11.refactor"

gen_Code refactor( gen_Code code ) {
	return code_refactor_and_format(code, scratch_file, path_refactor_script, nullptr );
}
gen_Code refactor_and_format( gen_Code code ) {
	return code_refactor_and_format(code, scratch_file, path_refactor_script, path_format_style );
}

int main()
{
	gen_Context ctx = {0};
	gen_init(& ctx);

	register_library_macros();

	

	// gen_deinit(& ctx);
	return 0;
}
