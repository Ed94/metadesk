#if MD_INTELLISENSE_DIRECTIVES
#pragma once
#endif

#pragma region Platform OS

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

#pragma endregion Platform OS
