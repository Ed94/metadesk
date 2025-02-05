#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "macros.h"
#endif

////////////////////////////////
//~ rjf: Asserts

#ifndef trap
#	if COMPILER_MSVC
#		define trap() __debugbreak()
#	elif COMPILER_CLANG || COMPILER_GCC
#		define trap() __builtin_trap()
#	else
#		error Unknown trap intrinsic for this compiler.
#	endif
#endif

#ifndef assert_always
#define assert_always(x) do { if ( !(x) ) { trap(); } } while(0)
#endif

#ifndef assert
#	if BUILD_DEBUG
#		define assert(x) assert_always(x)
#	else
#		define assert(x) (void)(x)
#	endif
#endif

#ifndef invalid_path
#define invalid_path            assert( ! "Invalid Path!")
#endif
#ifndef not_implemented
#define not_implemented         assert( ! "Not Implemented!")
#endif
#ifndef no_op
#define no_op                   ((void)0)
#endif
#ifndef md_static_assert
#define md_static_assert(C, ID) global U8 glue(ID, __LINE__)[ (C) ? 1 : -1 ]
#endif
