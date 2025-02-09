#ifdef INTELLISENSE_DIRECTIVES
#	include "metadesk.h"
#endif

#include "base/base.c"

#if OS_WINDOWS
#	include "os/win32/os_win32.c"
#elif OS_LINUX
#	include "os/linux/os_linux.c"
#else
#	error OS core layer not implemented for this operating system.
#endif

#include "os/os.c"

#include "mdesk/mdesk.c"
