#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#endif

#ifndef MD_API
#if MD_COMPILER_MSVC
#	if MD_BUILD_DYANMIC
#		if MD_BUILD_API_EXPORT
#			define MD_API __declspec(dllexport)
#		else
#			define MD_API __declspec(dllimport)
#		endif
#	else
#		define MD_API  // Empty for static builds
#	endif
#else
#	ifdef MD_BUILD_DYANMIC
#		define MD_API __attribute__((visibility("default")))
#	else
#		define MD_API  // Empty for static builds
#	endif
#endif
#endif // GEN_API

#ifndef MD_API_C_BEGIN
#	if MD_LANG_C
#		define MD_API_C_BEGIN
#		define MD_API_C_END
#		define MD_API_C
#	else
#		define MD_API_C_BEGIN extern "C" {
#		define MD_API_C_END   }
#		define MD_API_C_END   extern "C"
#	endif
#endif

#ifndef md_global // Global variables
#	if MD_BUILD_API_EXPORT || MD_BUILD_STATIC
#		define md_global         
#	else
#		define md_global static
#	endif
#endif

// Internal Linkage
#ifndef md_internal
#define md_internal static
#endif
