#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#endif

#include <stdarg.h>
#include <stddef.h>

#if defined( OS_WINDOWS )
#	include <intrin.h>
#	include <tmmintrin.h>
#	include <wmmintrin.h>
#	ifndef WIN32_LEAN_AND_MEAN
#		ifndef NOMINMAX
#			define NOMINMAX
#		endif
#
#		define WIN32_LEAN_AND_MEAN
#		define WIN32_MEAN_AND_LEAN
#		define VC_EXTRALEAN
#	include <Windows.h>
#	endif
#	undef NOMINMAX
#	undef WIN32_LEAN_AND_MEAN
#	undef WIN32_MEAN_AND_LEAN
#	undef VC_EXTRALEAN
#endif

#if LANG_C
#	include <assert.h>
#	include <stdbool.h>
#endif
