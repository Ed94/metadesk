#if MD_INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#endif

#include <stdarg.h>
#include <stddef.h>

#if defined( MD_OS_WINDOWS )
#	include <intrin.h>
#endif

#if MD_LANG_C
#	include <assert.h>
#	include <stdbool.h>
#endif
