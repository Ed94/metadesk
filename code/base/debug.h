#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Asserts

#ifndef trap
#	if COMPILER_MSVC
#		if _MSC_VER < 1300
#			define GEN_DEBUG_TRAP() __asm int 3 /* Trap to debugger! */
#		else
#			define GEN_DEBUG_TRAP() __debugbreak()
#		endif
#	elif COMPILER_CLANG || COMPILER_GCC
#		define trap() __builtin_trap()
#	else
#		error Unknown trap intrinsic for this compiler.
#	endif
#endif

#define assert_msg( cond, msg, ... )                                                                  \
	do                                                                                                \
	{                                                                                                 \
		if ( ! ( cond ) )                                                                             \
		{                                                                                             \
			assert_handler( #cond, __FILE__, __func__, scast( s64, __LINE__ ), msg, ##__VA_ARGS__ );  \
			GEN_DEBUG_TRAP();                                                                         \
		}                                                                                             \
	} while ( 0 )

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

MD_API void assert_handler( char const* condition, char const* file, char const* function, S32 line, char const* msg, ... );
