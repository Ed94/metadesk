#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/context_cracking.h"
#endif

#if !defined(OS_FEATURE_GRAPHICAL)
#	define OS_FEATURE_GRAPHICAL 0
#endif

#if !defined(OS_GFX_STUB)
#	define OS_GFX_STUB 0
#endif

#if OS_WINDOWS
#	include "core/win32/os_core_win32.h"
#elif OS_LINUX
#	include "core/linux/os_core_linux.h"
#else
#	error OS core layer not implemented for this operating system.
#endif

#include "core/os_core.h"
