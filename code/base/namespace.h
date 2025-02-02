#ifdef MD_INTELLISENSE_DIRECTIVES
#pragma once
#include "cracking_arch.h"
#endif

// C++ namespace support
#if MD_DONT_USE_NAMESPACE || MD_COMPILER_C
#	if MD_COMPILER_C
#		define MD_NS
#		define MD_NS_BEGIN
#		define MD_NS_END
#	else
#		define MD_NS              ::
#		define MD_NS_BEGIN
#		define MD_NS_END
#	endif
#else
	namespace MD {}
	namespace md = MD;
#	define MD_NS              MD::
#	define MD_NS_BEGIN        namespace MD {
#	define MD_NS_END          }
#endif
