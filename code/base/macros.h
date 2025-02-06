#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#endif

////////////////////////////////
//~ rjf: Branch Predictor Hints

#if COMPILER_CLANG
# define expect(expr, val) __builtin_expect((expr), (val))
#else
# define expect(expr, val) (expr)
#endif

#define likely(expr)   expect(expr, 1)
#define unlikely(expr) expect(expr, 0)

////////////////////////////////
//~ erg: type casting

#if LANG_CPP
#	ifndef ccast
#	define ccast( type, value ) ( const_cast< type >( (value) ) )
#	endif
#	ifndef pcast
#	define pcast( type, value ) ( * reinterpret_cast< type* >( & ( value ) ) )
#	endif
#	ifndef rcast
#	define rcast( type, value ) reinterpret_cast< type >( value )
#	endif
#	ifndef scast
#	define scast( type, value ) static_cast< type >( value )
#	endif
#else
#	ifndef ccast
#	define ccast( type, value ) ( (type)(value) )
#	endif
#	ifndef pcast
#	define pcast( type, value ) ( * (type*)(& value) )
#	endif
#	ifndef rcast
#	define rcast( type, value ) ( (type)(value) )
#	endif
#	ifndef scast
#	define scast( type, value ) ( (type)(value) )
#	endif
#endif

#if ! defined(typeof) && ( ! LANG_C || __STDC_VERSION__ < 202311L)
#	if ! LANG_C
#		define typeof decltype
#	elif defined(_MSC_VER)
#		define typeof __typeof__
#	elif defined(__GNUC__) || defined(__clang__)
#		define typeof __typeof__
#	else
#		error "Compiler not supported"
#	endif
#endif

#if LANG_C
#	if __STDC_VERSION__ >= 202311L
#		define enum_underlying(type) : type
#	else
#		define enum_underlying(type)
#   endif
#else
#	define enum_underlying(type) : type
#endif

#if LANG_C
#	ifndef nullptr
#		define nullptr NULL
#	endif

#	ifndef MD_REMOVE_PTR
#		define MD_REMOVE_PTR(type) typeof(* ( (type) NULL) )
#	endif
#endif

#if ! defined(PARAM_DEFAULT) && LANG_CPP
#	define PARAM_DEFAULT = {}
#else
#	define PARAM_DEFAULT
#endif

#if LANG_C
#	ifndef static_assert
#	undef  static_assert
#		if __STDC_VERSION__ >= 201112L
#			define static_assert(condition, message) _Static_assert(condition, message)
#		else
#			define static_assert(condition, message) typedef char static_assertion_##__LINE__[(condition)?1:-1]
#		endif
#	endif
#endif

////////////////////////////////
//~ rjf: For-Loop Construct Macros

#ifndef defer_loop
#define defer_loop(begin, end)           for (int _i_ =       ((begin), 0); ! _i_;                           _i_ += 1, (end))
#endif
#ifndef defer_loop_checked
#define defer_loop_checked(begin, end)   for (int _i_ = 2 * ! (begin);       (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))
#endif

#ifndef each_enum_val
#define each_enum_val(type, it)          type it = (type) 0; it < type ## _COUNT; it = (type)( it + 1 )
#endif
#ifndef each_non_zero_enum_val
#define each_non_zero_enum_val(type, it) type it = (type) 1; it < type ## _COUNT; it = (type)( it + 1 )
#endif
