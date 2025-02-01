#if MD_INTELLISENSE_DIRECTIVES
#include "cstd.h"
#endif

#ifndef MD_API
#if MD_COMPILER_MSVC
#	ifdef MD_DYN_LINK
#		ifdef MD_DYN_EXPORT
#			define MD_API __declspec(dllexport)
#		else
#			define MD_API __declspec(dllimport)
#		endif
#	else
#		define MD_API  // Empty for static builds
#	endif
#else
#	ifdef MD_DYN_LINK
#		define MD_API __attribute__((visibility("default")))
#	else
#		define MD_API  // Empty for static builds
#	endif
#endif
#endif // GEN_API

#ifndef internal
#define internal static
#endif

#ifndef local_persist
#define local_persist static
#endif

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
