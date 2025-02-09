#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#endif

////////////////////////////////
//~ rjf: Build Options

#if ! defined(BUILD_DEBUG)
#	  define  BUILD_DEBUG 1
#endif

#if ! defined(BUILD_STATIC)
#	define BUILD_STATIC 0
#endif

#if ! defined(BUILD_DYANMIC)
#	define BUILD_DYANMIC 0
#endif

#if ! defined(BUILD_API_EXPORT)
#	define BUILD_API_EXPORT 0
#endif

#if !defined(BUILD_ENTRY_DEFINING_UNIT)
#	define BUILD_ENTRY_DEFINING_UNIT 0
#endif

#if !defined(BUILD_CONSOLE_INTERFACE)
#	define BUILD_CONSOLE_INTERFACE 0
#endif

#if !defined(BUILD_VERSION_MAJOR)
#	define BUILD_VERSION_MAJOR 0
#endif

#if !defined(BUILD_VERSION_MINOR)
#	define BUILD_VERSION_MINOR 1
#endif

#if !defined(BUILD_VERSION_PATCH)
#	define BUILD_VERSION_PATCH 0
#endif

#define BUILD_VERSION_STRING_LITERAL stringify(BUILD_VERSION_MAJOR) "." stringify(BUILD_VERSION_MINOR) "." stringify(BUILD_VERSION_PATCH)

#if BUILD_DEBUG
#	define BUILD_MODE_STRING_LITERAL_APPEND " [Debug]"
#else
#	define BUILD_MODE_STRING_LITERAL_APPEND ""
#endif
#if defined(BUILD_GIT_HASH)
#	define BUILD_GIT_HASH_STRING_LITERAL_APPEND " [" BUILD_GIT_HASH "]"
#else
#	define BUILD_GIT_HASH_STRING_LITERAL_APPEND ""
#endif

#if !defined(BUILD_TITLE)
#	define BUILD_TITLE "Untitled"
#endif

#if !defined(BUILD_RELEASE_PHASE_STRING_LITERAL)
#	define BUILD_RELEASE_PHASE_STRING_LITERAL "ALPHA"
#endif

#if !defined(BUILD_ISSUES_LINK_STRING_LITERAL)
# define BUILD_ISSUES_LINK_STRING_LITERAL "ADD_BUILD_ISSUES_LINK"
#endif

#define BUILD_TITLE_STRING_LITERAL BUILD_TITLE " (" BUILD_VERSION_STRING_LITERAL " " BUILD_RELEASE_PHASE_STRING_LITERAL ") - " __DATE__ "" BUILD_GIT_HASH_STRING_LITERAL_APPEND BUILD_MODE_STRING_LITERAL_APPEND

#pragma region Compiler Vendor

#if defined( _MSC_VER )
#	pragma message("Detected MSVC")
// #	define COMPILER_CLANG 0
#	define COMPILER_MSVC  1
// #	define COMPILER_GCC   0
#	if _MSC_VER >= 1920
#		define COMPILER_MSVC_YEAR 2019
#	elif _MSC_VER >= 1910
#		define COMPILER_MSVC_YEAR 2017
#	elif _MSC_VER >= 1900
#		define COMPILER_MSVC_YEAR 2015
#	elif _MSC_VER >= 1800
#		define COMPILER_MSVC_YEAR 2013
#	elif _MSC_VER >= 1700
#		define COMPILER_MSVC_YEAR 2012
#	elif _MSC_VER >= 1600
#		define COMPILER_MSVC_YEAR 2010
#	elif _MSC_VER >= 1500
#		define M_DCOMPILER_MSVC_YEAR 2008
#	elif _MSC_VER >= 1400
#		define COMPILER_MSVC_YEAR 2005
#	else
#		define COMPILER_MSVC_YEAR 0
#	endif

#elif defined( __GNUC__ )
#	pragma message("Detected GCC")
// #	define COMPILER_CLANG 0
// #	define COMPILER_MSVC  0
#	define COMPILER_GCC   1
#elif defined( __clang__ )
#	pragma message("Detected CLANG")
#	define COMPILER_CLANG 1
// #	define COMPILER_MSVC  0
// #	define COMPILER_GCC   0
#else
#	error Unknown compiler
#endif

#if defined( __has_attribute )
#	define HAS_ATTRIBUTE( attribute ) __has_attribute( attribute )
#else
#	define HAS_ATTRIBUTE( attribute ) ( 0 )
#endif

#if defined(GCC_VERSION_CHECK)
#	undef GCC_VERSION_CHECK
#endif
#if defined(GEN_GCC_VERSION)
#	define GCC_VERSION_CHECK(major,minor,patch) (GEN_GCC_VERSION >= GEN_VERSION_ENCODE(major, minor, patch))
#else
#	define GCC_VERSION_CHECK(major,minor,patch) (0)
#endif

#pragma endregion Compiler Vendor

#pragma endregion Language

#if ! defined(LANG_C)
#	ifdef __cplusplus
#		define LANG_C   0
#		define LANG_CPP 1
#	else
#		if defined(__STDC__)
#			define LANG_C   1
#		    define LANG_CPP 0
#		else
            // Fallback for very old C compilers
#			define LANG_C   1
#		    define LANG_CPP 0
#		endif
#   endif
#endif

#if LANG_C
#	pragma message("MD: Detected C")
#endif

#if LANG_CPP
#	pragma message("MD: Detected CPP")
#endif

#pragma endregion Langauge

#pragma region Hardware Architecture

#if COMPILER_CLANG
#	if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#		define ARCH_X64 1
#	elif defined(i386) || defined(__i386) || defined(__i386__)
#		define ARCH_X86 1
#	elif defined(__aarch64__)
#		define ARCH_ARM64 1
#	elif defined(__arm__)
#		define ARCH_ARM32 1
#	else
#		error Architecture not supported.
#	endif
#endif // COMPILER_CLANG

#if COMPILER_MSVC
#	if defined(_M_AMD64)
#		define ARCH_X64 1
#	elif defined(_M_IX86)
#		define ARCH_X86 1
#	elif defined(_M_ARM64)
#		define ARCH_ARM64 1
#	elif defined(_M_ARM)
#		define ARCH_ARM32 1
#	else
#		error Architecture not supported.
#	endif
#endif // COMPILER_MSVC

#if COMPILER_GCC
#	if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#		define ARCH_X64 1
#	elif defined(i386) || defined(__i386) || defined(__i386__)
#		define ARCH_X86 1
#	elif defined(__aarch64__)
#		define ARCH_ARM64 1
#	elif defined(__arm__)
#		define ARCH_ARM32 1
#	else
#		error Architecture not supported.
#	endif
#endif // COMPILER_GCC

#if defined(ARCH_X64)
#	define ARCH_64BIT 1
#elif defined(ARCH_X86)
#	define ARCH_32BIT 1
#endif

#if ARCH_ARM32 || ARCH_ARM64 || ARCH_X64 || ARCH_X86
#	define ARCH_LITTLE_ENDIAN 1
#else
#	error Endianness of this architecture not understood by context cracker.
#endif

#pragma endregion Hardware Architecture

#pragma region Operating System

#if defined( _WIN32 ) || defined( _WIN64 )
#	ifndef     OS_WINDOWS
#		define OS_WINDOWS 1
#	endif
#elif defined( __APPLE__ ) && defined( __MACH__ )
#	ifndef     OS_OSX
#		define OS_OSX 1
#	endif
#	ifndef     OS_MACOS
#		define OS_MACOS 1
#	endif
#elif defined( __unix__ )
#	ifndef     OS_UNIX
#		define OS_UNIX 1
#	endif
#	if defined( ANDROID ) || defined( __ANDROID__ )
#		ifndef     OS_ANDROID
#	    	define OS_ANDROID 1
#		endif
#		ifndef     OS_LINUX
#			define OS_LINUX 1
#		endif
#	elif defined( __linux__ )
#		ifndef     OS_LINUX
#			define OS_LINUX 1
#		endif
#	elif defined( __FreeBSD__ ) || defined( __FreeBSD_kernel__ )
#		ifndef     OS_FREEBSD
#			define OS_FREEBSD 1
#		endif
#	elif defined( __OpenBSD__ )
#		ifndef     OS_OPENBSD
#			define OS_OPENBSD 1
#		endif
#	elif defined( __EMSCRIPTEN__ )
#		ifndef     OS_EMSCRIPTEN
#			define OS_EMSCRIPTEN 1
#		endif
#	elif defined( __CYGWIN__ )
#		ifndef     OS_CYGWIN
#			define OS_CYGWIN 1
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

#if !defined(ARCH_32BIT)
#	define ARCH_32BIT 0
#endif
#if !defined(ARCH_64BIT)
#	define ARCH_64BIT 0
#endif
#if !defined(ARCH_X64)
#	define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
#	define ARCH_X86 0
#endif
#if !defined(ARCH_ARM64)
#	define ARCH_ARM64 0
#endif
#if !defined(ARCH_ARM32)
#	define ARCH_ARM32 0
#endif
#if !defined(COMPILER_MSVC)
#	define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
#	define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
#	define COMPILER_CLANG 0
#endif
#if !defined(OS_WINDOWS)
#	define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
#	define OS_LINUX 0
#endif
#if !defined(OS_MAC)
#	define OS_MAC 0
#endif
#if !defined(LANG_CPP)
#	define LANG_CPP 0
#endif
#if !defined(LANG_C)
#	define LANG_C 0
#endif

////////////////////////////////
//~ rjf: Unsupported Errors

#if ARCH_X86
#	error You tried to build in x86 (32 bit) mode, but currently, only building in x64 (64 bit) mode is supported.
#endif
#if ! ARCH_X64
#	error You tried to build with an unsupported architecture. Currently, only building in x64 mode is supported.
#endif
