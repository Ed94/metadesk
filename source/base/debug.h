#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "base_types.h"
#endif

// TODO(Ed): Review this

////////////////////////////////
//~ rjf: Asserts

#ifndef md_trap
#	if MD_COMPILER_MSVC
#		if _MSC_VER < 1300
#			define md_trap() __asm int 3 /* Trap to debugger! */
#		else
#			define md_trap() __debugbreak()
#		endif
#	elif MD_COMPILER_CLANG || MD_COMPILER_GCC
#		define md_trap() __builtin_trap()
#	else
#		error Unknown md_trap intrinsic for this compiler.
#	endif
#endif

#define md_assert_msg( cond, msg, ... )                                                                 \
	do                                                                                               \
	{                                                                                                \
		if ( ! ( cond ) )                                                                            \
		{                                                                                            \
			md_assert_handler( #cond, __FILE__, __func__, md_scast( MD_S64, __LINE__ ), msg, ##__VA_ARGS__ ); \
			md_trap();                                                                         \
		}                                                                                            \
	} while ( 0 )

#ifndef md_assert_always
#define md_assert_always(x) do { if ( !(x) ) { md_trap(); } } while(0)
#endif

#ifndef md_assert
#	if MD_BUILD_DEBUG
#		define md_assert(x) md_assert_always(x)
#	else
#		define md_assert(x) (void)(x)
#	endif
#endif

#ifndef md_invalid_path
#define md_invalid_path            md_assert( ! "Invalid Path!")
#endif
#ifndef md_not_implemented
#define md_not_implemented         md_assert( ! "Not Implemented!")
#endif
#ifndef md_no_op
#define md_no_op                   ((void)0)
#endif
#ifndef md_static_assert
#define md_static_assert(C, ID) md_global MD_U8 md_glue(ID, __LINE__)[ (C) ? 1 : -1 ]
#endif

MD_API void md_assert_handler( char const* condition, char const* file, char const* function, MD_S32 line, char const* msg, ... );
