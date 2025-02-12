#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#endif

#include <stdarg.h>
#include <stddef.h>

#if defined( MD_OS_WINDOWS )
#	include <intrin.h>
#	include <tmmintrin.h>
#	include <wmmintrin.h>
#endif

#if MD_LANG_C
#	include <assert.h>
#	include <stdbool.h>
#endif
