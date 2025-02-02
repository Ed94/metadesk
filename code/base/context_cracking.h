#if MD_INTELLISENSE_DIRECTIVES
#	pragma once
#endif

#pragma region Compiler Vendor

#if defined( _MSC_VER )
#	pragma message("Detected MSVC")
// #	define MD_COMPILER_CLANG 0
#	define MD_COMPILER_MSVC  1
// #	define MD_COMPILER_GCC   0
#	if _MSC_VER >= 1920
#		define MD_COMPILER_MSVC_YEAR 2019
#	elif _MSC_VER >= 1910
#		define MD_COMPILER_MSVC_YEAR 2017
#	elif _MSC_VER >= 1900
#		define MD_COMPILER_MSVC_YEAR 2015
#	elif _MSC_VER >= 1800
#		define MD_COMPILER_MSVC_YEAR 2013
#	elif _MSC_VER >= 1700
#		define MD_COMPILER_MSVC_YEAR 2012
#	elif _MSC_VER >= 1600
#		define MD_COMPILER_MSVC_YEAR 2010
#	elif _MSC_VER >= 1500
#		define M_DCOMPILER_MSVC_YEAR 2008
#	elif _MSC_VER >= 1400
#		define MD_COMPILER_MSVC_YEAR 2005
#	else
#		define MD_COMPILER_MSVC_YEAR 0
#	endif

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

#pragma endregion Compiler Vendor

#pragma endregion Language

#if ! defined(MD_LANG_C)
#	ifdef __cplusplus
#		define MD_LANG_C   0
#		define MD_LANG_CPP 1
#	else
#		if defined(__STDC__)
#			define MD_LANG_C   1
#		    define MD_LANG_CPP 0
#		else
            // Fallback for very old C compilers
#			define MD_LANG_C   1
#		    define MD_LANG_CPP 0
#		endif
#   endif
#endif

#if MD_LANG_C
#	pragma message("MD: Detected C")
#endif

#if MD_LANG_CPP
#	pragma message("MD: Detected CPP")
#endif

#pragma endregion Langauge

#pragma region Hardware Architecture

#if MD_COMPILER_CLANG
#	if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#		define MD_ARCH_X64 1
#	elif defined(i386) || defined(__i386) || defined(__i386__)
#		define MD_ARCH_X86 1
#	elif defined(__aarch64__)
#		define MD_ARCH_ARM64 1
#	elif defined(__arm__)
#		define MD_ARCH_ARM32 1
#	else
#		error Architecture not supported.
#	endif
#endif // MD_COMPILER_CLANG

#if MD_COMPILER_MSVC
#	if defined(_M_AMD64)
#		define MD_ARCH_X64 1
#	elif defined(_M_IX86)
#		define MD_ARCH_X86 1
#	elif defined(_M_ARM64)
#		define MD_ARCH_ARM64 1
#	elif defined(_M_ARM)
#		define MD_ARCH_ARM32 1
#	else
#		error Architecture not supported.
#	endif
#endif // MD_COMPILER_MSVC

#if MD_COMPILER_GCC
#	if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#		define MD_ARCH_X64 1
#	elif defined(i386) || defined(__i386) || defined(__i386__)
#		define MD_ARCH_X86 1
#	elif defined(__aarch64__)
#		define MD_ARCH_ARM64 1
#	elif defined(__arm__)
#		define MD_ARCH_ARM32 1
#	else
#		error Architecture not supported.
#	endif
#endif // MD_COMPILER_GCC

#if defined(MD_ARCH_X64)
#	define MD_ARCH_64BIT 1
#elif defined(MD_ARCH_X86)
#	define MD_ARCH_32BIT 1
#endif

#if MD_ARCH_ARM32 || MD_ARCH_ARM64 || MD_ARCH_X64 || MD_ARCH_X86
#	define MD_ARCH_LITTLE_ENDIAN 1
#else
#	error Endianness of this architecture not understood by context cracker.
#endif

#pragma endregion Hardware Architecture

#pragma region Operating System

#if defined( _WIN32 ) || defined( _WIN64 )
#	ifndef     MD_OS_WINDOWS
#		define MD_OS_WINDOWS 1
#	endif
#elif defined( __APPLE__ ) && defined( __MACH__ )
#	ifndef     MD_OS_OSX
#		define MD_OS_OSX 1
#	endif
#	ifndef     MD_OS_MACOS
#		define MD_OS_MACOS 1
#	endif
#elif defined( __unix__ )
#	ifndef     MD_OS_UNIX
#		define MD_OS_UNIX 1
#	endif
#	if defined( ANDROID ) || defined( __ANDROID__ )
#		ifndef     MD_OS_ANDROID
#	    	define MD_OS_ANDROID 1
#		endif
#		ifndef     MD_OS_LINUX
#			define MD_OS_LINUX 1
#		endif
#	elif defined( __linux__ )
#		ifndef     MD_OS_LINUX
#			define MD_OS_LINUX 1
#		endif
#	elif defined( __FreeBSD__ ) || defined( __FreeBSD_kernel__ )
#		ifndef     MD_OS_FREEBSD
#			define MD_OS_FREEBSD 1
#		endif
#	elif defined( __OpenBSD__ )
#		ifndef     MD_OS_OPENBSD
#			define MD_OS_OPENBSD 1
#		endif
#	elif defined( __EMSCRIPTEN__ )
#		ifndef     MD_OS_EMSCRIPTEN
#			define MD_OS_EMSCRIPTEN 1
#		endif
#	elif defined( __CYGWIN__ )
#		ifndef     MD_OS_CYGWIN
#			define MD_OS_CYGWIN 1
#		endif
#	else
#		error This UNIX operating system is not supported
#	endif
#else
#	error This operating system is not supported
#endif

#pragma endregion Operating System

#pragma region Language
#pragma endregion Langage

////////////////////////////////
//~ rjf: Zero All Undefined Options

#if !defined(MD_ARCH_32BIT)
# define MD_ARCH_32BIT 0
#endif
#if !defined(MD_ARCH_64BIT)
# define MD_ARCH_64BIT 0
#endif
#if !defined(MD_ARCH_X64)
# define MD_ARCH_X64 0
#endif
#if !defined(MD_ARCH_X86)
# define MD_ARCH_X86 0
#endif
#if !defined(MD_ARCH_ARM64)
# define MD_ARCH_ARM64 0
#endif
#if !defined(MD_ARCH_ARM32)
# define MD_ARCH_ARM32 0
#endif
#if !defined(MD_COMPILER_MSVC)
# define MD_COMPILER_MSVC 0
#endif
#if !defined(MD-COMPILER_GCC)
# define MD_COMPILER_GCC 0
#endif
#if !defined(MD_COMPILER_CLANG)
# define MD_COMPILER_CLANG 0
#endif
#if !defined(MD_OS_WINDOWS)
# define MD_OS_WINDOWS 0
#endif
#if !defined(MD_OS_LINUX)
# define MD_OS_LINUX 0
#endif
#if !defined(MD_OS_MAC)
# define MD_OS_MAC 0
#endif
#if !defined(MD_LANG_CPP)
# define MD_LANG_CPP 0
#endif
#if !defined(MD_LANG_C)
# define MD_LANG_C 0
#endif

////////////////////////////////
//~ rjf: Unsupported Errors

#if MD_ARCH_X86
#	error You tried to build in x86 (32 bit) mode, but currently, only building in x64 (64 bit) mode is supported.
#endif
#if ! MD_ARCH_X64
#	error You tried to build with an unsupported architecture. Currently, only building in x64 mode is supported.
#endif
