#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#endif

////////////////////////////////
//~ rjf: Branch Predictor Hints

#if MD_COMPILER_CLANG
# define md_expect(expr, val) __builtin_expect((expr), (val))
#else
# define md_expect(expr, val) (expr)
#endif

#define md_likely(expr)   md_expect(expr, 1)
#define md_unlikely(expr) md_expect(expr, 0)

////////////////////////////////
//~ erg: type casting

#if MD_LANG_CPP
#	ifndef md_ccast
#	define md_ccast( type, value ) ( const_cast< type >( (value) ) )
#	endif
#	ifndef md_pcast
#	define md_pcast( type, value ) ( * reinterpret_cast< type* >( & ( value ) ) )
#	endif
#	ifndef md_rcast
#	define md_rcast( type, value ) reinterpret_cast< type >( value )
#	endif
#	ifndef md_scast
#	define md_scast( type, value ) static_cast< type >( value )
#	endif
#else
#	ifndef md_ccast
#	define md_ccast( type, value ) ( (type)(value) )
#	endif
#	ifndef md_pcast
#	define md_pcast( type, value ) ( * (type*)(& value) )
#	endif
#	ifndef md_rcast
#	define md_rcast( type, value ) ( (type)(value) )
#	endif
#	ifndef md_scast
#	define md_scast( type, value ) ( (type)(value) )
#	endif
#endif

#if ! defined(typeof) && ( ! MD_LANG_C || __STDC_VERSION__ < 202311L)
#	if ! MD_LANG_C
#		define typeof decltype
#	elif defined(_MSC_VER)
#		define typeof __typeof__
#	elif defined(__GNUC__) || defined(__clang__)
#		define typeof __typeof__
#	else
#		error "MD_Compiler not supported"
#	endif
#endif

#if MD_LANG_C
#	if __STDC_VERSION__ >= 202311L
#		define md_enum_underlying(type) : type
#	else
#		define md_enum_underlying(type)
#   endif
#else
#	define md_enum_underlying(type) : type
#endif

#if MD_LANG_C
#	ifndef md_nullptr
#		define md_nullptr NULL
#	endif

#	ifndef MD_REMOVE_PTR
#		define MD_REMOVE_PTR(type) typeof(* ( (type) NULL) )
#	endif
#endif

#if ! defined(MD_PARAM_DEFAULT) && MD_LANG_CPP
#	define MD_PARAM_DEFAULT = {}
#else
#	define MD_PARAM_DEFAULT
#endif

#if MD_LANG_C
#	ifndef static_assert
#	undef  static_assert
#		if __STDC_VERSION__ >= 201112L
#			define static_assert(condition, message) _Static_assert(condition, message)
#		else
#			define static_assert(condition, message) typedef char static_assertion_##__LINE__[(condition)?1:-1]
#		endif
#	endif
#endif

#ifndef        md_force_inline
#	if MD_COMPILER_MSVC
#		define md_force_inline __forceinline
#	elif MD_COMPILER_GCC
#		define md_force_inline inline __attribute__((__always_inline__))
#	elif MD_COMPILER_CLANG
#		if __has_attribute(__always_inline__)
#			define md_force_inline inline __attribute__((__always_inline__))
#		else
#			define md_force_inline
#		endif
#	else
#		define md_force_inline
#	endif
#endif

#ifndef        md_never_inline
#	if MD_COMPILER_MSVC
#		define md_never_inline __declspec( noinline )
#	elif MD_COMPILER_GCC
#		define md_never_inline __attribute__( ( __noinline__ ) )
#	elif MD_COMPILER_CLANG
#		if __has_attribute(__always_inline__)
#			define md_never_inline __attribute__( ( __noinline__ ) )
#		else
#			define md_never_inline
#		endif
#	else
#		define md_never_inline
#	endif
#endif

////////////////////////////////
//~ rjf: For-Loop Construct Macros

#ifndef md_defer_loop
#define md_defer_loop(begin, end)           for (int _i_ = ((begin), 0); ! _i_; _i_ += 1, (end))
#endif
#ifndef md_defer_loop_checked
#define md_defer_loop_checked(begin, end)   for (int _i_ = 2 * ! (begin); (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))
#endif

#ifndef md_each_enum_val
#define md_each_enum_val(type, it)          type it = (type) 0; it < type ## _COUNT; it = (type)( it + 1 )
#endif
#ifndef md_each_non_zero_enum_val
#define md_each_non_zero_enum_val(type, it) type it = (type) 1; it < type ## _COUNT; it = (type)( it + 1 )
#endif

#ifndef md_stringify
#define md_stringify_(S) #S
#define md_stringify(S)  md_stringify_(S)
#endif

#ifndef md_glue
#define md_glue_(A, B) A ## B
#define md_glue(A, B)  md_glue_(A,B)
#endif

#define md_src_line_str md_stringify(__LINE__)

#ifndef md_do_once
#define md_do_once()                                                                               \
	md_local_persist int __do_once_counter_##md_src_line_str  = 0;                                 \
    for(;      __do_once_counter_##md_src_line_str != 1; __do_once_counter_##md_src_line_str = 1 ) \

#define md_do_once_defer( expression )                                                                    \
    md_local_persist int __do_once_counter_##md_src_line_str  = 0;                                        \
    for(;__do_once_counter_##md_src_line_str != 1; __do_once_counter_##md_src_line_str = 1, (expression)) \

#define md_do_once_start   \
	do                     \
	{                      \
		md_local_persist   \
		bool done = false; \
		if ( done )        \
			break;         \
		done = true;

#define md_do_once_end \
	}                  \
	while(0);
#endif

#define ct_if(expr, then, else) _Generic( \
(&(char[1 + !!(expr)]){0}), \
	char (*)[2]: (then),    \
	char (*)[1]: (else)     \
)
