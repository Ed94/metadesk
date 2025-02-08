#pragma once

#include "third_party/gencpp_c11/push_ignores.inline.h"

#define GEN_DEFINE_LIBRARY_CODE_CONSTANTS
#define GEN_ENFORCE_STRONG_CODE_TYPES
#define GEN_IMPLEMENTATION
#include "third_party/gencpp_c11/gencpp_c11.h"

#include "third_party/gencpp_c11/misc.h"

int main()
{
	gen_Context ctx = {0};
	gen_init(& ctx);

	
	return 0;
}
