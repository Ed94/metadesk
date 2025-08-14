#ifdef INTELLISENSE_DIRECTIVES
#	include "base/context_cracking.h"
#	include "base/namespace.h"
#endif

#if !defined(MD_OS_FEATURE_GRAPHICAL)
#	define MD_OS_FEATURE_GRAPHICAL 0
#endif

#if !defined(MD_OS_GFX_STUB)
#	define MD_OS_GFX_STUB 0
#endif

#if MD_OS_WINDOWS
#	include "os/win32/os_win32_includes.h"

	MD_NS_BEGIN
#	include "os/win32/os_win32.h"
	MD_NS_END

#elif MD_OS_LINUX
#	include "os/linux/os_linux_includes.h"

	MD_NS_BEGIN
#	include "os/linux/os_linux.h"
	MD_NS_END
#	error OS core layer not implemented for this operating system.
#endif
