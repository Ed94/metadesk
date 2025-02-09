#include "metadesk.h"

#undef  MARKUP_LAYER_COLOR
#define MARKUP_LAYER_COLOR 0.20f, 0.60f, 0.80f

#include "base/platform.c"

MD_NS_BEGIN

#include "base/debug.c"
#include "base/memory_substrate.c"
#include "base/arena.c"
#include "base/strings.c"
#include "base/text.c"
#include "base/thread_context.c"
#include "base/command_line.c"
#include "base/logger.c"
#include "base/entry_point.c"
#include "base/time.c"

#if OS_WINDOWS
#	include "os/win32/os_win32.c"
#elif OS_LINUX
#	include "os/win32/os_win32.c"
#endif

#include "os/os.c"

#include "mdesk/mdesk.c"

MD_NS_END
