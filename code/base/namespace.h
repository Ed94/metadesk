#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#endif

// C++ namespace support
#if defined(MD_DONT_USE_NAMESPACE) || LANG_C
#	if LANG_C
#		define MD_NS
#		define MD_NS_BEGIN
#		define MD_NS_END
#	else
#		define MD_NS       ::
#		define MD_NS_BEGIN
#		define MD_NS_END
#	endif
#else
	namespace MD {}
	namespace md = MD;

#	define MD_NS       MD::
#	define MD_NS_BEGIN namespace MD {
#	define MD_NS_END   }
#endif
