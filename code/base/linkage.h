#ifdef MD_INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
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

#ifndef global // Global variables
#	ifdef MD_DYN_EXPORT
#		define global         
#	else
#		define global static
#	endif
#endif

// Internal Linkage
#ifndef internal
#define internal static
#endif
