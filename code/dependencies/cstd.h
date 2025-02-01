#if MD_INTELLISENSE_DIRECTIVES
#include "arch.h"
#include "os.h"
#include "compiler.h"
#endif

#pragma region Mandatory Includes

#	include <stdarg.h>
#	include <stddef.h>

#	if defined( MD_SYSTEM_WINDOWS )
#		include <intrin.h>
#	endif

#if MD_COMPILER_C
#include <assert.h>
#include <stdbool.h>
#endif

#pragma endregion Mandatory Includes
