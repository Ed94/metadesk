#ifdef INTELLISENSE_DIRECTIVES
#	include "base.h"
#endif

#undef  MARKUP_LAYER_COLOR
#define MARKUP_LAYER_COLOR 0.20f, 0.60f, 0.80f

#include "platform.c"

MD_NS_BEGIN

#include "debug.c"
#include "memory_substrate.c"
#include "arena.c"
#include "strings.c"
#include "text.c"
#include "thread_context.c"
#include "command_line.c"
#include "logger.c"
#include "entry_point.c"
#include "time.c"

MD_NS_END
