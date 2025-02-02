#if MD_INTELLISENSE_DIRECTIVES
#pragma once
#endif

#if defined( _MSC_VER )
#	pragma message("Detected MSVC")
// #	define MD_COMPILER_CLANG 0
#	define MD_COMPILER_MSVC  1
// #	define MD_COMPILER_GCC   0
#elif defined( __GNUC__ )
#	pragma message("Detected GCC")
// #	define MD_COMPILER_CLANG 0
// #	define MD_COMPILER_MSVC  0
#	define MD_COMPILER_GCC   1
#elif defined( __clang__ )
#	pragma message("Detected CLANG")
#	define MD_COMPILER_CLANG 1
// #	define MD_COMPILER_MSVC  0
// #	define MD_COMPILER_GCC   0
#else
#	error Unknown compiler
#endif

#if defined( __has_attribute )
#	define MD_HAS_ATTRIBUTE( attribute ) __has_attribute( attribute )
#else
#	define MD_HAS_ATTRIBUTE( attribute ) ( 0 )
#endif

#if defined(MD_GCC_VERSION_CHECK)
#  undef MD_GCC_VERSION_CHECK
#endif
#if defined(GEN_GCC_VERSION)
#  define MD_GCC_VERSION_CHECK(major,minor,patch) (GEN_GCC_VERSION >= GEN_VERSION_ENCODE(major, minor, patch))
#else
#  define MD_GCC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if ! defined(MD_COMPILER_C)
#	ifdef __cplusplus
#		define MD_COMPILER_C   0
#		define MD_COMPILER_CPP 1
#	else
#		if defined(__STDC__)
#			define MD_COMPILER_C   1
#		    define MD_COMPILER_CPP 0
#		else
            // Fallback for very old C compilers
#			define MD_COMPILER_C   1
#		    define MD_COMPILER_CPP 0
#		endif
#   endif
#endif

#if MD_COMPILER_C
#pragma message("MD: Detected C")
#endif

#if MD_COMPILER_CPP
#pragma message("MD: Detected CPP")
#endif
