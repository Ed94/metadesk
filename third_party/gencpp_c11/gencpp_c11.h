// This file was generated automatially by gencpp's c_library.cpp  (See: https://github.com/Ed94/gencpp)

#pragma once

#ifdef __clang__
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wunused-const-variable"
#	pragma clang diagnostic ignored "-Wunused-but-set-variable"
#	pragma clang diagnostic ignored "-Wswitch"
#	pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wunknown-pragmas"
#	pragma clang diagnostic ignored "-Wvarargs"
#	pragma clang diagnostic ignored "-Wunused-function"
#	pragma clang diagnostic ignored "-Wbraced-scalar-init"
#   pragma clang diagnostic ignored "-W#pragma-messages"
#	pragma clang diagnostic ignored "-Wstatic-in-inline"
#endif

#ifdef __GNUC__
#	pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunknown-pragmas"
#	pragma GCC diagnostic ignored "-Wcomment"
#	pragma GCC diagnostic ignored "-Wswitch"
#	pragma GCC diagnostic ignored "-Wunused-variable"
#endif
/*
	gencpp: An attempt at "simple" staged metaprogramming for c/c++.

	See Readme.md for more information from the project repository.

	Public Address:
	https://github.com/Ed94/gencpp  ---------------------------------------------------------------.
	|   _____                               _____ _                       _      ___   __     __    |
	|  / ____)                             / ____} |                     | |   / ,__} /  |   /  |   |
	| | / ___  ___ _ __   ___ _ __  _ __  | {___ | l_ __ _  __ _, ___  __| |  | |    '-l |  '-l |   |
	| | |{_  \/ __\ '_ \ / __} '_ l| '_ l  \___ \| __/ _` |/ _` |/ __\/ _` |  | |      | |    | |   |
	| | l__j | ___/ | | | {__; ;_l } ;_l } ____} | l| (_} | {_| | ___j {_; |  | l___  _J l_  _J l_  |
	|  \_____|\___}_l |_|\___} .__/| .__/ {_____/ \__\__/_l\__. |\___/\__,_l   \____}{_____}{_____} |
	|                        | |   | |                      __} |                                   |
	|                        l_l   l_l                     {___/                                    |
	! ----------------------------------------------------------------------- VERSION: v0.25-Alpha  |
	! ============================================================================================= |
	! WARNING: THIS IS AN ALPHA VERSION OF THE LIBRARY, USE AT YOUR OWN DISCRETION                  |
	! NEVER DO CODE GENERATION WITHOUT AT LEAST HAVING CONTENT IN A CODEBASE UNDER VERSION CONTROL  |
	! ============================================================================================= /
*/
#if ! defined(GEN_DONT_ENFORCE_GEN_TIME_GUARD) && ! defined(GEN_TIME)
#	error Gen.hpp : GEN_TIME not defined
#endif

//! If its desired to roll your own dependencies, define GEN_ROLL_OWN_DEPENDENCIES before including this file.
// Dependencies are derived from the c-zpl library: https://github.com/zpl-c/zpl
#ifndef GEN_ROLL_OWN_DEPENDENCIES

#pragma region Platform Detection

/* Platform architecture */

#if defined( _WIN64 ) || defined( __x86_64__ ) || defined( _M_X64 ) || defined( __64BIT__ ) || defined( __powerpc64__ ) || defined( __ppc64__ ) || defined( __aarch64__ )
#	ifndef GEN_ARCH_64_BIT
#		define GEN_ARCH_64_BIT 1
#	endif
#else
#	ifndef GEN_ARCH_32_BItxt_StrCaT
#		define GEN_ARCH_32_BIT 1
#	endif
#endif

/* Platform OS */

#if defined( _WIN32 ) || defined( _WIN64 )
#	ifndef GEN_SYSTEM_WINDOWS
#		define GEN_SYSTEM_WINDOWS 1
#	endif
#elif defined( __APPLE__ ) && defined( __MACH__ )
#	ifndef GEN_SYSTEM_OSX
#		define GEN_SYSTEM_OSX 1
#	endif
#	ifndef GEN_SYSTEM_MACOS
#		define GEN_SYSTEM_MACOS 1
#	endif
#elif defined( __unix__ )
#	ifndef GEN_SYSTEM_UNIX
#		define GEN_SYSTEM_UNIX 1
#	endif
#	if defined( ANDROID ) || defined( __ANDROID__ )
#		ifndef GEN_SYSTEM_ANDROID
#			define GEN_SYSTEM_ANDROID 1
#		endif
#		ifndef GEN_SYSTEM_LINUX
#			define GEN_SYSTEM_LINUX 1
#		endif
#	elif defined( __linux__ )
#		ifndef GEN_SYSTEM_LINUX
#			define GEN_SYSTEM_LINUX 1
#		endif
#	elif defined( __FreeBSD__ ) || defined( __FreeBSD_kernel__ )
#		ifndef GEN_SYSTEM_FREEBSD
#			define GEN_SYSTEM_FREEBSD 1
#		endif
#	elif defined( __OpenBSD__ )
#		ifndef GEN_SYSTEM_OPENBSD
#			define GEN_SYSTEM_OPENBSD 1
#		endif
#	elif defined( __EMSCRIPTEN__ )
#		ifndef GEN_SYSTEM_EMSCRIPTEN
#			define GEN_SYSTEM_EMSCRIPTEN 1
#		endif
#	elif defined( __CYGWIN__ )
#		ifndef GEN_SYSTEM_CYGWIN
#			define GEN_SYSTEM_CYGWIN 1
#		endif
#	else
#		error This UNIX operating system is not supported
#	endif
#else
#	error This operating system is not supported
#endif

/* Platform compiler */

#if defined( _MSC_VER )
#	pragma message("Detected MSVC")
// #	define GEN_COMPILER_CLANG 0
#	define GEN_COMPILER_MSVC  1
// #	define GEN_COMPILER_GCC   0
#elif defined( __GNUC__ )
#	pragma message("Detected GCC")
// #	define GEN_COMPILER_CLANG 0
// #	define GEN_COMPILER_MSVC  0
#	define GEN_COMPILER_GCC   1
#elif defined( __clang__ )
#	pragma message("Detected CLANG")
#	define GEN_COMPILER_CLANG 1
// #	define GEN_COMPILER_MSVC  0
// #	define GEN_COMPILER_GCC   0
#else
#	error Unknown compiler
#endif

#if defined( __has_attribute )
#	define GEN_HAS_ATTRIBUTE( attribute ) __has_attribute( attribute )
#else
#	define GEN_HAS_ATTRIBUTE( attribute ) ( 0 )
#endif

#if defined(GEN_GCC_VERSION_CHECK)
#  undef GEN_GCC_VERSION_CHECK
#endif
#if defined(GEN_GCC_VERSION)
#  define GEN_GCC_VERSION_CHECK(major,minor,patch) (GEN_GCC_VERSION >= GEN_VERSION_ENCODE(major, minor, patch))
#else
#  define GEN_GCC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if !defined(GEN_COMPILER_C)
#	ifdef __cplusplus
#		define GEN_COMPILER_C   0
#		define GEN_COMPILER_CPP 1
#	else
#		if defined(__STDC__)
#			define GEN_COMPILER_C   1
#		    define GEN_COMPILER_CPP 0
#		else
            // Fallback for very old C compilers
#			define GEN_COMPILER_C   1
#		    define GEN_COMPILER_CPP 0
#		endif
#   endif
#endif

#if GEN_COMPILER_C
#pragma message("GENCPP: Detected C")
#endif

#pragma endregion Platform Detection

#pragma region Mandatory Includes

#	include <stdarg.h>
#	include <stddef.h>

#	if defined( GEN_SYSTEM_WINDOWS )
#		include <intrin.h>
#	endif

#if GEN_COMPILER_C
#include <assert.h>
#include <stdbool.h>
#endif

#pragma endregion Mandatory Includes

#if GEN_DONT_USE_NAMESPACE || GEN_COMPILER_C
#	if GEN_COMPILER_C
#		define GEN_NS
#		define GEN_NS_BEGIN
#		define GEN_NS_END
#	else
#		define GEN_NS              ::
#		define GEN_NS_BEGIN
#		define GEN_NS_END
#	endif
#else
#	define GEN_NS              gen::
#	define GEN_NS_BEGIN        namespace gen {
#	define GEN_NS_END          }
#endif

GEN_NS_BEGIN

#pragma region Macros

#ifndef GEN_API
#if GEN_COMPILER_MSVC
    #ifdef GEN_DYN_LINK
        #ifdef GEN_DYN_EXPORT
            #define GEN_API __declspec(dllexport)
        #else
            #define GEN_API __declspec(dllimport)
        #endif
    #else
        #define GEN_API  // Empty for static builds
    #endif
#else
    #ifdef GEN_DYN_LINK
        #define GEN_API __attribute__((visibility("default")))
    #else
        #define GEN_API  // Empty for static builds
    #endif
#endif
#endif // GEN_API

#ifndef gen_global // Global variables
#	if defined(GEN_STATIC_LINK) || defined(GEN_DYN_LINK)
#		define gen_global         
#	else
#		define gen_global static
#	endif
#endif
#ifndef gen_internal
#define gen_internal      static    // Internal linkage
#endif
#ifndef gen_local_persist
#define gen_local_persist static    // Local Persisting variables
#endif

#ifndef gen_bit
#define gen_bit( Value )                         ( 1 << Value )
#endif

#ifndef gen_bitfield_is_set
#define gen_bitfield_is_set( Type, Field, Mask ) ( (gen_scast(Type, Mask) & gen_scast(Type, Field)) == gen_scast(Type, Mask) )
#endif

// Mainly intended for forcing the base library to utilize only C-valid constructs or type coercion
#ifndef GEN_C_LIKE_CPP
#define GEN_C_LIKE_CPP 0
#endif

#if GEN_COMPILER_CPP
#	ifndef gen_cast
#	define gen_cast( type, value ) (tmpl_cast<type>( value ))
#	endif
#else
#	ifndef gen_cast
#	define gen_cast( type, value )  ( (type)(value) )
#	endif
#endif

#if GEN_COMPILER_CPP
#	ifndef gen_ccast
#	define gen_ccast( type, value ) ( const_cast< type >( (value) ) )
#	endif
#	ifndef gen_pcast
#	define gen_pcast( type, value ) ( * reinterpret_cast< type* >( & ( value ) ) )
#	endif
#	ifndef gen_rcast
#	define gen_rcast( type, value ) reinterpret_cast< type >( value )
#	endif
#	ifndef gen_scast
#	define gen_scast( type, value ) static_cast< type >( value )
#	endif
#else
#	ifndef gen_ccast
#	define gen_ccast( type, value ) ( (type)(value) )
#	endif
#	ifndef gen_pcast
#	define gen_pcast( type, value ) ( * (type*)(& value) )
#	endif
#	ifndef gen_rcast
#	define gen_rcast( type, value ) ( (type)(value) )
#	endif
#	ifndef gen_scast
#	define gen_scast( type, value ) ( (type)(value) )
#	endif
#endif

#ifndef gen_stringize
#define gen_stringize_va( ... ) #__VA_ARGS__
#define gen_stringize( ... )    gen_stringize_va( __VA_ARGS__ )
#endif

#define src_line_str gen_stringize(__LINE__)

#ifndef gen_do_once
#define gen_do_once()                                                                            \
	gen_local_persist int __do_once_counter_##src_line_str  = 0;                                 \
    for(;      __do_once_counter_##src_line_str != 1; __do_once_counter_##src_line_str = 1 ) \

#define do_once_defer( expression )                                                                 \
    gen_local_persist int __do_once_counter_##src_line_str  = 0;                                        \
    for(;__do_once_counter_##src_line_str != 1; __do_once_counter_##src_line_str = 1, (expression)) \

#define gen_do_once_start      \
	do                     \
	{                      \
		gen_local_persist      \
		bool done = false; \
		if ( done )        \
			break;         \
		done = true;

#define gen_do_once_end        \
	}                      \
	while(0);
#endif

#ifndef gen_labeled_scope_start
#define gen_labeled_scope_start if ( false ) {
#define gen_labeled_scope_end   }
#endif

#ifndef         gen_compiler_decorated_func_name
#   ifdef COMPILER_CLANG
#       define  gen_compiler_decorated_func_name __PRETTY_NAME__
#   elif defined(COMPILER_MSVC)
#   	define gen_compiler_decorated_func_name __FUNCDNAME__
#   endif
#endif

#ifndef gen_num_args_impl

// This is essentially an arg couneter version of GEN_SELECT_ARG macros
// See section : _Generic function overloading for that usage (explains this heavier case)

#define gen_num_args_impl( _0,                                 \
		_1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10,   \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
		_21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
		_31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
		_41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
		_51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
		_61, _62, _63, _64, _65, _66, _67, _68, _69, _70,  \
		_71, _72, _73, _74, _75, _76, _77, _78, _79, _80,  \
		_81, _82, _83, _84, _85, _86, _87, _88, _89, _90,  \
		_91, _92, _93, _94, _95, _96, _97, _98, _99, _100, \
		N, ...                                             \
	) N

// ## deletes preceding comma if _VA_ARGS__ is empty (GCC, Clang)
#define gen_num_args(...)                            \
	gen_num_args_impl(_, ## __VA_ARGS__,             \
		100, 99, 98, 97, 96, 95, 94, 93, 92, 91, \
		 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, \
		 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, \
		 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, \
		 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, \
		 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
		 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, \
		 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
		 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
		 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, \
		 0                                       \
	)
#endif

#ifndef gen_clamp
#define gen_clamp( x, lower, upper )      gen_min( gen_max( ( x ), ( lower ) ), ( upper ) )
#endif
#ifndef gen_count_of
#define gen_count_of( x )                 ( ( gen_size_of( x ) / gen_size_of( 0 [ x ] ) ) / ( ( gen_ssize )( ! ( gen_size_of( x ) % gen_size_of( 0 [ x ] ) ) ) ) )
#endif
#ifndef gen_is_between
#define gen_is_between( x, lower, upper ) ( ( ( lower ) <= ( x ) ) && ( ( x ) <= ( upper ) ) )
#endif
#ifndef gen_size_of
#define gen_size_of( x )                  ( gen_ssize )( sizeof( x ) )
#endif

#ifndef gen_max
#define gen_max( a, b ) ( (a > b) ? (a) : (b) )
#endif
#ifndef gen_min
#define gen_min( a, b ) ( (a < b) ? (a) : (b) )
#endif

#if GEN_COMPILER_MSVC || GEN_COMPILER_TINYC
#	define gen_offset_of( Type, element ) ( ( GEN_NS( gen_ssize ) ) & ( ( ( Type* )0 )->element ) )
#else
#	define gen_offset_of( Type, element ) __builtin_offsetof( Type, element )
#endif

#ifndef        gen_forceinline
#	if GEN_COMPILER_MSVC
#		define gen_forceinline __forceinline
#	elif GEN_COMPILER_GCC
#		define gen_forceinline inline __attribute__((__always_inline__))
#	elif GEN_COMPILER_CLANG
#	if __has_attribute(__always_inline__)
#		define gen_forceinline inline __attribute__((__always_inline__))
#	else
#		define gen_forceinline
#	endif
#	else
#		define gen_forceinline
#	endif
#endif

#ifndef        gen_neverinline
#	if GEN_COMPILER_MSVC
#		define gen_neverinline __declspec( noinline )
#	elif GEN_COMPILER_GCC
#		define gen_neverinline __attribute__( ( __noinline__ ) )
#	elif GEN_COMPILER_CLANG
#	if __has_attribute(__always_inline__)
#		define gen_neverinline __attribute__( ( __noinline__ ) )
#	else
#		define gen_neverinline
#	endif
#	else
#		define gen_neverinline
#	endif
#endif

#if GEN_COMPILER_C
#ifndef gen_static_assert
#undef  gen_static_assert
    #if GEN_COMPILER_C && __STDC_VERSION__ >= 201112L
        #define gen_static_assert(condition, message) _Static_assert(condition, message)
    #else
        #define gen_static_assert(condition, message) typedef char static_assertion_##__LINE__[(condition)?1:-1]
	#endif
#endif
#endif

#if GEN_COMPILER_CPP
// Already Defined
#elif GEN_COMPILER_C && __STDC_VERSION__ >= 201112L
#	define gen_thread_local _Thread_local
#elif GEN_COMPILER_MSVC
#	define gen_thread_local __declspec(thread)
#elif GEN_COMPILER_CLANG
#	define gen_thread_local __thread
#else
#	error "No thread local support"
#endif

#if ! defined(gen_typeof) && (!GEN_COMPILER_C || __STDC_VERSION__ < 202311L)
#	if ! GEN_COMPILER_C
#		define gen_typeof decltype
#	elif defined(_MSC_VER)
#		define gen_typeof __typeof__
#	elif defined(__GNUC__) || defined(__clang__)
#		define gen_typeof __typeof__
#	else
#		error "Compiler not supported"
#	endif
#endif

#ifndef GEN_API_C_BEGIN
#	if GEN_COMPILER_C
#		define GEN_API_C_BEGIN
#		define GEN_API_C_END
#	else
#		define GEN_API_C_BEGIN extern "C" {
#		define GEN_API_C_END   }
#	endif
#endif

#if GEN_COMPILER_C
#	if __STDC_VERSION__ >= 202311L
#		define gen_enum_underlying(type) : type
#	else
#		define gen_enum_underlying(type)
#   endif
#else
#	define gen_enum_underlying(type) : type
#endif

#if GEN_COMPILER_C
#	ifndef gen_nullptr
#		define gen_nullptr NULL
#	endif

#	ifndef GEN_REMOVE_PTR
#		define GEN_REMOVE_PTR(type) gen_typeof(* ( (type) NULL) )
#	endif
#endif

#if ! defined(GEN_PARAM_DEFAULT) && GEN_COMPILER_CPP
#	define GEN_PARAM_DEFAULT = {}
#else
#	define GEN_PARAM_DEFAULT
#endif

#if GEN_COMPILER_CPP
    #define gen_struct_init(type, value) {value}
#else
    #define gen_struct_init(type, value) {value}
#endif

#if 0
#ifndef GEN_OPTIMIZE_MAPPINGS_BEGIN
#	define GEN_OPTIMIZE_MAPPINGS_BEGIN _pragma(optimize("gt", on))
#	define GEN_OPITMIZE_MAPPINGS_END   _pragma(optimize("", on))
#endif
#else
#	define GEN_OPTIMIZE_MAPPINGS_BEGIN
#	define GEN_OPITMIZE_MAPPINGS_END
#endif

#ifndef get_optional
#	if GEN_COMPILER_C
#		define get_optional(opt) opt ? *opt : (gen_typeof(*opt)){0}
#	else
#		define get_optional(opt) opt
#	endif
#endif

#pragma endregion Macros

#pragma region _Generic Macros
//   ____                       _        ______                _   _                ____                  _                 __ _
//  / ___}                     (_)      |  ____}              | | (_)              / __ \                | |               | |(_)
// | | ___  ___ _ __   ___ _ __ _  ___  | |__ _   _ _ __   ___| |_ _  ___  _ __   | |  | |_   _____ _ __ | | ___   __ _  __| | _ _ __   __ _
// | |{__ |/ _ \ '_ \ / _ \ '__} |/ __| |  __} | | | '_ \ / __} __} |/ _ \| '_ \  | |  | \ \ / / _ \ '_ }| |/ _ \ / _` |/ _` || | '_ \ / _` |
// | |__j |  __/ | | |  __/ |  | | (__  | |  | |_| | | | | (__| l_| | (_) | | | | | l__| |\ V /  __/ |   | | (_) | (_| | (_| || | | | | (_| |
//  \____/ \___}_l l_l\___}_l  l_l\___| l_l   \__,_l_l l_l\___}\__}_l\___/l_l l_l  \____/  \_/ \___}_l   l_l\___/ \__,_l\__,_l|_|_| |_|\__, |
// This implemnents macros for utilizing "The Naive Extendible _Generic Macro" explained in:                                            __| |
// https://github.com/JacksonAllan/CC/blob/main/articles/Better_C_Generics_Part_1_The_Extendible_Generic.md                            {___/
// Since gencpp is used to generate the c-library, it was choosen over the more novel implementations to keep the macros as easy to understand and unobfuscated as possible.

#define GEN_COMMA_OPERATOR , // The comma operator is used by preprocessor macros to delimit arguments, so we have to represent it via a macro to prevent parsing incorrectly.

// Helper macros for argument selection
#define GEN_SELECT_ARG_1( _1, ... ) _1 // <-- Of all th args passed pick _1.
#define GEN_SELECT_ARG_2( _1, _2, ... ) _2 // <-- Of all the args passed pick _2.
#define GEN_SELECT_ARG_3( _1, _2, _3, ... ) _3 // etc..

#define GEN_GENERIC_SEL_ENTRY_TYPE             GEN_SELECT_ARG_1 // Use the arg expansion macro to select arg 1 which should have the type.
#define GEN_GENERIC_SEL_ENTRY_FUNCTION         GEN_SELECT_ARG_2 // Use the arg expansion macro to select arg 2 which should have the function.
#define GEN_GENERIC_SEL_ENTRY_COMMA_DELIMITER  GEN_SELECT_ARG_3 // Use the arg expansion macro to select arg 3 which should have the comma delimiter ','.

#define GEN_RESOLVED_FUNCTION_CALL // Just used to indicate where the call "occurs"

// ----------------------------------------------------------------------------------------------------------------------------------
// GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( macro ) includes a _Generic slot only if the specified macro is defined (as type, function_name).
// It takes advantage of the fact that if the macro is defined, then the expanded text will contain a comma.
// Expands to ',' if it can find (type): (function) <comma_operator: ',' >
// Where GEN_GENERIC_SEL_ENTRY_COMMA_DELIMITER is specifically looking for that <comma> ,
#define GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( slot_exp ) GEN_GENERIC_SEL_ENTRY_COMMA_DELIMITER( slot_exp, GEN_GENERIC_SEL_ENTRY_TYPE( slot_exp, ): GEN_GENERIC_SEL_ENTRY_FUNCTION( slot_exp, ) GEN_COMMA_OPERATOR, , )
//                                                          ^ Selects the comma                              ^ is the type                             ^ is the function                             ^ Insert a comma
// The slot won't exist if that comma is not found.

// For the occastion where an expression didn't resolve to a selection option the "default: <value>" will be set to:
typedef struct GENCPP_NO_RESOLVED_GENERIC_SELECTION GENCPP_NO_RESOLVED_GENERIC_SELECTION;
struct GENCPP_NO_RESOLVED_GENERIC_SELECTION {
	void* _THE_VOID_SLOT_;
};
GENCPP_NO_RESOLVED_GENERIC_SELECTION const gen_generic_selection_fail = {0};
// Which will provide the message:  error: called object type 'struct NO_RESOLVED_GENERIC_SELECTION' is not a function or function pointer
// ----------------------------------------------------------------------------------------------------------------------------------

// Below are generated on demand for an overlaod depdendent on a type:
// ----------------------------------------------------------------------------------------------------------------------------------
#define GEN_FUNCTION_GENERIC_EXAMPLE( selector_arg ) _Generic(             \
(selector_arg), /* Select Via Expression*/                                 \
  /* Extendibility slots: */                                               \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1__function_sig ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2__function_sig ) \
	default: gen_generic_selection_fail                                    \
) GEN_RESOLVED_FUNCTION_CALL( selector_arg )
// ----------------------------------------------------------------------------------------------------------------------------------

// Then each definiton of a function has an associated define:
#// #define GENERIC_SLOT_<#>_<generic identifier> <typename>, <function_to_resolve>

// Then somehwere later on
// <etc> <return_type> <function_id> ( <arguments> ) { <implementation> }

// Concrete example:

// To add support for long:
#define GENERIC_SLOT_1_gen_example_hash long, gen_example_hash__P_long
size_t gen_example_hash__P_long( long val ) { return val * 2654435761ull; }

// To add support for long long:
#define GENERIC_SLOT_2_gen_example_hash  long long, gen_example_hash__P_long_long
size_t gen_example_hash__P_long_long( long long val ) { return val * 2654435761ull; }

// If using an Editor with support for syntax hightlighting macros:
// GENERIC_SLOT_1_gen_example_hash and GENERIC_SLOT_2_gen_example_hash should show color highlighting indicating the slot is enabled,
// or, "defined" for usage during the compilation pass that handles the _Generic instrinsic.
#define gen_hash_example( function_arguments ) _Generic(                      \
(function_arguments), /* Select Via Expression*/                              \
  /* Extendibility slots: */                                                  \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1_gen_example_hash ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2_gen_example_hash ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_3_gen_example_hash ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_4_gen_example_hash ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_5_gen_example_hash ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_6_gen_example_hash ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_7_gen_example_hash ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_8_gen_example_hash ) \
	default: gen_generic_selection_fail                                       \
) GEN_RESOLVED_FUNCTION_CALL( function_arguments )

// Additional Variations:

// If the function takes more than one argument the following is used:
#define GEN_FUNCTION_GENERIC_EXAMPLE_VARADIC( selector_arg, ... ) _Generic( \
(selector_arg),                                                             \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1__function_sig )  \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2__function_sig )  \
	/* ... */                                                               \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GENERIC_SLOT_N__function_sig )   \
	default: gen_generic_selection_fail                                     \
) GEN_RESOLVED_FUNCTION_CALL( selector_arg, __VA_ARG__ )

// If the function does not take the arugment as a parameter:
#define GEN_FUNCTION_GENERIC_EXAMPLE_DIRECT_TYPE( selector_arg ) _Generic( \
( GEN_TYPE_TO_EXP(selector_arg) ),                                         \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1__function_sig ) \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2__function_sig ) \
	/* ... */                                                              \
	GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GENERIC_SLOT_N__function_sig )  \
	default: gen_generic_selection_fail                                    \
) GEN_RESOLVED_FUNCTION_CALL()

// Used to keep the _Generic keyword happy as bare types are not considered "expressions"
#define GEN_TYPE_TO_EXP(type) (* (type*)NULL)
// Instead of using this macro, you'll see it directly expanded by the code generation.

// typedef void* GEN_GenericExampleType;
// GEN_FUNCTION_GENERIC_EXAMPLE_DIRECT_TYPE( GEN_GenericExampleType );
#pragma endregion _Generic Macros

GEN_API_C_BEGIN

#pragma region Basic Types

#define GEN_U8_MIN 0u
#define GEN_U8_MAX 0xffu
#define GEN_I8_MIN ( -0x7f - 1 )
#define GEN_I8_MAX 0x7f

#define GEN_U16_MIN 0u
#define GEN_U16_MAX 0xffffu
#define GEN_I16_MIN ( -0x7fff - 1 )
#define GEN_I16_MAX 0x7fff

#define GEN_U32_MIN 0u
#define GEN_U32_MAX 0xffffffffu
#define GEN_I32_MIN ( -0x7fffffff - 1 )
#define GEN_I32_MAX 0x7fffffff

#define GEN_U64_MIN 0ull
#define GEN_U64_MAX 0xffffffffffffffffull
#define GEN_I64_MIN ( -0x7fffffffffffffffll - 1 )
#define GEN_I64_MAX 0x7fffffffffffffffll

#if defined( GEN_ARCH_32_BIT )
#	define GEN_USIZE_MIN GEN_U32_MIN
#	define GEN_USIZE_MAX GEN_U32_MAX
#	define GEN_ISIZE_MIN GEN_S32_MIN
#	define GEN_ISIZE_MAX GEN_S32_MAX
#elif defined( GEN_ARCH_64_BIT )
#	define GEN_USIZE_MIN GEN_U64_MIN
#	define GEN_USIZE_MAX GEN_U64_MAX
#	define GEN_ISIZE_MIN GEN_I64_MIN
#	define GEN_ISIZE_MAX GEN_I64_MAX
#else
#	error Unknown architecture size. This library only supports 32 gen_bit and 64 gen_bit architectures.
#endif

#define GEN_F32_MIN 1.17549435e-38f
#define GEN_F32_MAX 3.40282347e+38f
#define GEN_F64_MIN 2.2250738585072014e-308
#define GEN_F64_MAX 1.7976931348623157e+308

#if defined( GEN_COMPILER_MSVC )
#	if _MSC_VER < 1300
typedef unsigned char  gen_u8;
typedef signed   char  gen_s8;
typedef unsigned short gen_u16;
typedef signed   short gen_s16;
typedef unsigned int   gen_u32;
typedef signed   int   gen_s32;
#    else
typedef unsigned __int8  gen_u8;
typedef signed   __int8  gen_s8;
typedef unsigned __int16 gen_u16;
typedef signed   __int16 gen_s16;
typedef unsigned __int32 gen_u32;
typedef signed   __int32 gen_s32;
#    endif
typedef unsigned __int64 gen_u64;
typedef signed   __int64 gen_s64;
#else
#	include <stdint.h>

typedef uint8_t  gen_u8;
typedef int8_t   gen_s8;
typedef uint16_t gen_u16;
typedef int16_t  gen_s16;
typedef uint32_t gen_u32;
typedef int32_t  gen_s32;
typedef uint64_t gen_u64;
typedef int64_t  gen_s64;
#endif

gen_static_assert( sizeof( gen_u8 )  == sizeof( gen_s8 ),  "sizeof(gen_u8) != sizeof(gen_s8)" );
gen_static_assert( sizeof( gen_u16 ) == sizeof( gen_s16 ), "sizeof(gen_u16) != sizeof(gen_s16)" );
gen_static_assert( sizeof( gen_u32 ) == sizeof( gen_s32 ), "sizeof(gen_u32) != sizeof(gen_s32)" );
gen_static_assert( sizeof( gen_u64 ) == sizeof( gen_s64 ), "sizeof(gen_u64) != sizeof(gen_s64)" );

gen_static_assert( sizeof( gen_u8 )  == 1, "sizeof(gen_u8) != 1" );
gen_static_assert( sizeof( gen_u16 ) == 2, "sizeof(gen_u16) != 2" );
gen_static_assert( sizeof( gen_u32 ) == 4, "sizeof(gen_u32) != 4" );
gen_static_assert( sizeof( gen_u64 ) == 8, "sizeof(gen_u64) != 8" );

typedef size_t    gen_usize;
typedef ptrdiff_t gen_ssize;

gen_static_assert( sizeof( gen_usize ) == sizeof( gen_ssize ), "sizeof(gen_usize) != sizeof(gen_ssize)" );

// NOTE: (u)zpl_intptr is only here for semantic reasons really as this library will only support 32/64 gen_bit OSes.
#if defined( _WIN64 )
typedef signed __int64   gen_sptr;
typedef unsigned __int64 gen_uptr;
#elif defined( _WIN32 )
// NOTE; To mark types changing their size, e.g. zpl_intptr
#	ifndef _W64
#		if ! defined( __midl ) && ( defined( _X86_ ) || defined( _M_IX86 ) ) && _MSC_VER >= 1300
#			define _W64 __w64
#		else
#			define _W64
#		endif
#	endif
typedef _W64 signed int   gen_sptr;
typedef _W64 unsigned int gen_uptr;
#else
typedef uintptr_t gen_uptr;
typedef intptr_t  gen_sptr;
#endif

gen_static_assert( sizeof( gen_uptr ) == sizeof( gen_sptr ), "sizeof(gen_uptr) != sizeof(gen_sptr)" );

typedef float  gen_f32;
typedef double gen_f64;

gen_static_assert( sizeof( gen_f32 ) == 4, "sizeof(gen_f32) != 4" );
gen_static_assert( sizeof( gen_f64 ) == 8, "sizeof(gen_f64) != 8" );

typedef gen_s8  gen_b8;
typedef gen_s16 gen_b16;
typedef gen_s32 gen_b32;

typedef void*       gen_mem_ptr;
typedef void const* gen_mem_ptr_const ;

#if GEN_COMPILER_CPP
template<typename Type> gen_uptr gen_to_uptr( Type* ptr ) { return (gen_uptr)ptr; }
template<typename Type> gen_sptr gen_to_sptr( Type* ptr ) { return (gen_sptr)ptr; }

template<typename Type> gen_mem_ptr       gen_to_mem_ptr      ( Type ptr ) { return (gen_mem_ptr)      ptr; }
template<typename Type> gen_mem_ptr_const gen_to_mem_ptr_const( Type ptr ) { return (gen_mem_ptr_const)ptr; }
#else
#define gen_to_uptr( ptr ) ((gen_uptr)(ptr))
#define gen_to_sptr( ptr ) ((gen_sptr)(ptr))

#define gen_to_mem_ptr( ptr)       ((gen_mem_ptr)ptr)
#define gen_to_mem_ptr_const( ptr) ((gen_mem_ptr)ptr)
#endif

#pragma endregion Basic Types

#pragma region Debug

#if GEN_BUILD_DEBUG
#	if defined( GEN_COMPILER_MSVC )
#		if _MSC_VER < 1300
// #pragma message("GEN_BUILD_DEBUG: __asm int 3")
#			define GEN_DEBUG_TRAP() __asm int 3 /* Trap to debugger! */
#		else
// #pragma message("GEN_BUILD_DEBUG: __debugbreak()")
#			define GEN_DEBUG_TRAP() __debugbreak()
#		endif
#	elif defined( GEN_COMPILER_TINYC )
#		define GEN_DEBUG_TRAP() gen_process_exit( 1 )
#	else
#		define GEN_DEBUG_TRAP() __builtin_trap()
#	endif
#else
// #pragma message("GEN_BUILD_DEBUG: omitted")
#	define GEN_DEBUG_TRAP()
#endif

#define GEN_ASSERT( cond ) GEN_ASSERT_MSG( cond, NULL )

#define GEN_ASSERT_MSG( cond, msg, ... )                                                              \
	do                                                                                                \
	{                                                                                                 \
		if ( ! ( cond ) )                                                                             \
		{                                                                                             \
			gen_assert_handler( #cond, __FILE__, __func__, gen_scast( gen_s64, __LINE__ ), msg, ##__VA_ARGS__ );  \
			GEN_DEBUG_TRAP();                                                                         \
		}                                                                                             \
	} while ( 0 )

#define GEN_ASSERT_NOT_NULL( ptr ) GEN_ASSERT_MSG( ( ptr ) != NULL, #ptr " must not be NULL" )

// NOTE: Things that shouldn't happen with a message!
#define GEN_PANIC( msg, ... ) GEN_ASSERT_MSG( 0, msg, ##__VA_ARGS__ )

#if GEN_BUILD_DEBUG
	#define GEN_FATAL( ... )                               \
	do                                                     \
	{                                                      \
		gen_local_persist gen_thread_local                         \
		char buf[GEN_PRINTF_MAXLEN] = { 0 };               \
		                                                   \
		gen_c_str_fmt(buf, GEN_PRINTF_MAXLEN, __VA_ARGS__);    \
		GEN_PANIC(buf);                                    \
	}                                                      \
	while (0)
#else

#	define GEN_FATAL( ... )                  \
	do                                       \
	{                                        \
		gen_c_str_fmt_out_err( __VA_ARGS__ );    \
		GEN_DEBUG_TRAP();                    \
		gen_process_exit(1);                     \
	}                                        \
	while (0)
#endif

GEN_API void gen_assert_handler( char const* condition, char const* file, char const* function, gen_s32 line, char const* msg, ... );
GEN_API gen_s32  gen_assert_crash( char const* condition );
GEN_API void gen_process_exit( gen_u32 code );

#pragma endregion Debug

#pragma region Memory


#define gen_kilobytes(x) ((x) * (gen_s64)(1024))
#define gen_megabytes(x) (gen_kilobytes(x) * (gen_s64)(1024))
#define gen_gigabytes(x) (gen_megabytes(x) * (gen_s64)(1024))
#define gen_terabytes(x) (gen_gigabytes(x) * (gen_s64)(1024))

#define GEN__ONES        (gen_scast(GEN_NS gen_usize, -1) / GEN_U8_MAX)
#define GEN__HIGHS       (GEN__ONES * (GEN_U8_MAX / 2 + 1))
#define GEN__HAS_ZERO(x) (((x) - GEN__ONES) & ~(x) & GEN__HIGHS)

#define gen_swap(a, b)             \
	do                             \
	{                              \
		gen_typeof(a) temp = (a);  \
		(a)                = (b);  \
		(b)                = temp; \
	} while (0)
//! Checks if value is power of 2.
gen_b32 gen_is_power_of_two(gen_ssize x);

//! Aligns address to specified alignment.
void* gen_align_forward(void* ptr, gen_ssize alignment);

//! Aligns value to a specified alignment.
gen_s64 gen_align_forward_by_value(gen_s64 value, gen_ssize alignment);

//! Moves pointer forward by bytes.
void* gen_pointer_add(void* ptr, gen_ssize bytes);

//! Moves pointer forward by bytes.
void const* gen_pointer_add_const(void const* ptr, gen_ssize bytes);

//! Calculates difference between two addresses.
gen_ssize gen_pointer_diff(void const* begin, void const* end);

//! Copy non-overlapping memory from source to destination.
GEN_API void* gen_mem_copy(void* dest, void const* source, gen_ssize size);

//! Search for a constant value within the size limit at memory location.
GEN_API void const* gen_mem_find(void const* data, gen_u8 byte_value, gen_ssize size);

//! Copy memory from source to destination.
void* gen_mem_move(void* dest, void const* source, gen_ssize size);

//! Set constant value at memory location with specified size.
void* gen_mem_set(void* data, gen_u8 byte_value, gen_ssize size);

//! @param ptr Memory location to clear up.
//! @param size The size to clear up with.
void gen_zero_size(void* ptr, gen_ssize size);

//! Clears up an item.
#define gen_zero_item(t) gen_zero_size((t), gen_size_of(*(t)))    // NOTE: Pass pointer of struct

//! Clears up an array.
#define gen_zero_array(a, count) gen_zero_size((a), gen_size_of(*(a)) * count)

enum gen_AllocType gen_enum_underlying(gen_u8)
{
	EAllocation_ALLOC,
	EAllocation_FREE,
	EAllocation_FREE_ALL,
	EAllocation_RESIZE,
};

typedef gen_u8 gen_AllocType;

typedef void*(gen_AllocatorProc)(void*         allocator_data,
                                 gen_AllocType type,
                                 gen_ssize     size,
                                 gen_ssize     alignment,
                                 void*         old_memory,
                                 gen_ssize     old_size,
                                 gen_u64       flags);

struct gen_AllocatorInfo
{
	gen_AllocatorProc* Proc;
	void*              Data;
};
typedef struct gen_AllocatorInfo gen_AllocatorInfo;

enum gen_AllocFlag
{
	ALLOCATOR_FLAG_CLEAR_TO_ZERO = gen_bit(0),
};
typedef enum gen_AllocFlag gen_AllocFlag;

#ifndef GEN_DEFAULT_MEMORY_ALIGNMENT
#define GEN_DEFAULT_MEMORY_ALIGNMENT (2 * gen_size_of(void*))
#endif

#ifndef GEN_DEFAULT_ALLOCATOR_FLAGS
#define GEN_DEFAULT_ALLOCATOR_FLAGS (ALLOCATOR_FLAG_CLEAR_TO_ZERO)
#endif

//! Allocate memory with default alignment.
void* gen_alloc(gen_AllocatorInfo a, gen_ssize size);

//! Allocate memory with specified alignment.
void* gen_alloc_align(gen_AllocatorInfo a, gen_ssize size, gen_ssize alignment);

//! Free allocated memory.
void gen_allocator_free(gen_AllocatorInfo a, void* ptr);

//! Free all memory allocated by an allocator.
void gen_free_all(gen_AllocatorInfo a);

//! Resize an allocated memory.
void* gen_resize(gen_AllocatorInfo a, void* ptr, gen_ssize old_size, gen_ssize new_size);

//! Resize an allocated memory with specified alignment.
void* gen_resize_align(gen_AllocatorInfo a, void* ptr, gen_ssize old_size, gen_ssize new_size, gen_ssize alignment);

//! Allocate memory for an item.
#define gen_alloc_item(allocator_, Type) (Type*)gen_alloc(allocator_, gen_size_of(Type))

//! Allocate memory for an array of items.
#define gen_alloc_array(allocator_, Type, count) (Type*)gen_alloc(allocator_, gen_size_of(Type) * (count))

/* gen_heap memory analysis tools */
/* define GEN_HEAP_ANALYSIS to enable this feature */
/* call zpl_heap_stats_init at the beginning of the entry point */
/* you can call zpl_heap_stats_check near the end of the execution to validate any possible leaks */
GEN_API void      gen_heap_stats_init(void);
GEN_API gen_ssize gen_heap_stats_used_memory(void);
GEN_API gen_ssize gen_heap_stats_alloc_count(void);
GEN_API void      gen_heap_stats_check(void);

//! Allocate/Resize memory using default options.

//! Use this if you don't need a "fancy" gen_resize allocation
void* gen_default_resize_align(gen_AllocatorInfo a, void* ptr, gen_ssize old_size, gen_ssize new_size, gen_ssize alignment);

GEN_API void*
    gen_heap_allocator_proc(void* allocator_data, gen_AllocType type, gen_ssize size, gen_ssize alignment, void* old_memory, gen_ssize old_size, gen_u64 flags);

//! The gen_heap allocator backed by operating system's memory manager.
#define gen_heap()                           \
	(gen_AllocatorInfo)                      \
	{                                        \
		gen_heap_allocator_proc, gen_nullptr \
	}
//! Helper to allocate memory using gen_heap allocator.
#define gen_malloc(sz) gen_alloc(gen_heap(), sz)

//! Helper to free memory allocated by gen_heap allocator.
#define gen_mfree(ptr) gen_allocator_free(gen_heap(), ptr)

struct gen_VirtualMemory
{
	void*     data;
	gen_ssize size;
};
typedef struct gen_VirtualMemory gen_VirtualMemory;

//! Initialize virtual memory from existing data.
GEN_API gen_VirtualMemory gen_vm_from_memory(void* data, gen_ssize size);

//! Allocate virtual memory at address with size.

//! @param addr The starting address of the region to reserve. If NULL, it lets operating system to decide where to allocate it.
//! @param size The size to serve.
GEN_API gen_VirtualMemory gen_vm_alloc(void* addr, gen_ssize size);

//! Release the virtual memory.
GEN_API gen_b32 gen_vm_free(gen_VirtualMemory vm);

//! Trim virtual memory.
GEN_API gen_VirtualMemory gen_vm_trim(gen_VirtualMemory vm, gen_ssize lead_size, gen_ssize size);

//! Purge virtual memory.
GEN_API gen_b32 gen_vm_purge(gen_VirtualMemory vm);

//! Retrieve VM's page size and alignment.
GEN_API gen_ssize gen_virtual_memory_page_size(gen_ssize* alignment_out);

#pragma region gen_Arena

struct gen_Arena;
typedef struct gen_Arena gen_Arena;

gen_AllocatorInfo gen_arena_allocator_info(gen_Arena* arena);

// Remove static keyword and rename allocator_proc
GEN_API void* gen_arena_allocator_proc(
    void*         allocator_data,
    gen_AllocType type,
    gen_ssize     size,
    gen_ssize     alignment,
    void*         old_memory,
    gen_ssize     old_size,
    gen_u64       flags
);

// Add these declarations after the gen_Arena struct
gen_Arena gen_arena_init_from_allocator(gen_AllocatorInfo backing, gen_ssize size);
gen_Arena gen_arena_init_from_memory(void* start, gen_ssize size);

gen_Arena gen_arena_init_sub(gen_Arena* parent, gen_ssize size);
gen_ssize gen_arena_alignment_of(gen_Arena* arena, gen_ssize alignment);
void      gen_arena_check(gen_Arena* arena);
void      gen_arena_free(gen_Arena* arena);
gen_ssize gen_arena_size_remaining(gen_Arena* arena, gen_ssize alignment);

struct gen_Arena
{
	gen_AllocatorInfo Backing;
	void*             PhysicalStart;
	gen_ssize         TotalSize;
	gen_ssize         TotalUsed;
	gen_ssize         TempCount;
};
typedef struct gen_Arena gen_Arena;

inline gen_AllocatorInfo gen_arena_allocator_info(gen_Arena* arena)
{
	GEN_ASSERT(arena != gen_nullptr);
	gen_AllocatorInfo info = { gen_arena_allocator_proc, arena };
	return info;
}

inline gen_Arena gen_arena_init_from_memory(void* start, gen_ssize size)
{
	gen_Arena arena = {
		{ gen_nullptr, gen_nullptr },
		start,
		size,
		0,
		0
	};
	return arena;
}

inline gen_Arena gen_arena_init_from_allocator(gen_AllocatorInfo backing, gen_ssize size)
{
	gen_Arena result = { backing, gen_alloc(backing, size), size, 0, 0 };
	return result;
}

inline gen_Arena gen_arena_init_sub(gen_Arena* parent, gen_ssize size)
{
	GEN_ASSERT(parent != gen_nullptr);
	return gen_arena_init_from_allocator(parent->Backing, size);
}

inline gen_ssize gen_arena_alignment_of(gen_Arena* arena, gen_ssize alignment)
{
	GEN_ASSERT(arena != gen_nullptr);
	gen_ssize alignment_offset, result_pointer, mask;
	GEN_ASSERT(gen_is_power_of_two(alignment));

	alignment_offset = 0;
	result_pointer   = (gen_ssize)arena->PhysicalStart + arena->TotalUsed;
	mask             = alignment - 1;

	if (result_pointer & mask)
		alignment_offset = alignment - (result_pointer & mask);

	return alignment_offset;
}

inline void gen_arena_check(gen_Arena* arena)
{
	GEN_ASSERT(arena != gen_nullptr);
	GEN_ASSERT(arena->TempCount == 0);
}

inline void gen_arena_free(gen_Arena* arena)
{
	GEN_ASSERT(arena != gen_nullptr);
	if (arena->Backing.Proc)
	{
		gen_allocator_free(arena->Backing, arena->PhysicalStart);
		arena->PhysicalStart = gen_nullptr;
	}
}

inline gen_ssize gen_arena_size_remaining(gen_Arena* arena, gen_ssize alignment)
{
	GEN_ASSERT(arena != gen_nullptr);
	gen_ssize result = arena->TotalSize - (arena->TotalUsed + gen_arena_alignment_of(arena, alignment));
	return result;
}

#pragma endregion gen_Arena


#pragma region FixedArena

struct gen_FixedArena_1KB
{
	char      memory[gen_kilobytes(1)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_1KB gen_FixedArena_1KB;

struct gen_FixedArena_4KB
{
	char      memory[gen_kilobytes(4)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_4KB gen_FixedArena_4KB;

struct gen_FixedArena_8KB
{
	char      memory[gen_kilobytes(8)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_8KB gen_FixedArena_8KB;

struct gen_FixedArena_16KB
{
	char      memory[gen_kilobytes(16)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_16KB gen_FixedArena_16KB;

struct gen_FixedArena_32KB
{
	char      memory[gen_kilobytes(32)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_32KB gen_FixedArena_32KB;

struct gen_FixedArena_64KB
{
	char      memory[gen_kilobytes(64)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_64KB gen_FixedArena_64KB;

struct gen_FixedArena_128KB
{
	char      memory[gen_kilobytes(128)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_128KB gen_FixedArena_128KB;

struct gen_FixedArena_256KB
{
	char      memory[gen_kilobytes(256)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_256KB gen_FixedArena_256KB;

struct gen_FixedArena_512KB
{
	char      memory[gen_kilobytes(512)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_512KB gen_FixedArena_512KB;

struct gen_FixedArena_1MB
{
	char      memory[gen_megabytes(1)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_1MB gen_FixedArena_1MB;

struct gen_FixedArena_2MB
{
	char      memory[gen_megabytes(2)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_2MB gen_FixedArena_2MB;

struct gen_FixedArena_4MB
{
	char      memory[gen_megabytes(4)];
	gen_Arena arena;
};
typedef struct gen_FixedArena_4MB gen_FixedArena_4MB;

inline void gen_fixed_arena_init_1KB(gen_FixedArena_1KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(1));
}

inline gen_ssize gen_fixed_arena_size_remaining_1KB(gen_FixedArena_1KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_1KB(gen_FixedArena_1KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_4KB(gen_FixedArena_4KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(4));
}

inline gen_ssize gen_fixed_arena_size_remaining_4KB(gen_FixedArena_4KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_4KB(gen_FixedArena_4KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_8KB(gen_FixedArena_8KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(8));
}

inline gen_ssize gen_fixed_arena_size_remaining_8KB(gen_FixedArena_8KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_8KB(gen_FixedArena_8KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_16KB(gen_FixedArena_16KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(16));
}

inline gen_ssize gen_fixed_arena_size_remaining_16KB(gen_FixedArena_16KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_16KB(gen_FixedArena_16KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_32KB(gen_FixedArena_32KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(32));
}

inline gen_ssize gen_fixed_arena_size_remaining_32KB(gen_FixedArena_32KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_32KB(gen_FixedArena_32KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_64KB(gen_FixedArena_64KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(64));
}

inline gen_ssize gen_fixed_arena_size_remaining_64KB(gen_FixedArena_64KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_64KB(gen_FixedArena_64KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_128KB(gen_FixedArena_128KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(128));
}

inline gen_ssize gen_fixed_arena_size_remaining_128KB(gen_FixedArena_128KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_128KB(gen_FixedArena_128KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_256KB(gen_FixedArena_256KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(256));
}

inline gen_ssize gen_fixed_arena_size_remaining_256KB(gen_FixedArena_256KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_256KB(gen_FixedArena_256KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_512KB(gen_FixedArena_512KB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_kilobytes(512));
}

inline gen_ssize gen_fixed_arena_size_remaining_512KB(gen_FixedArena_512KB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_512KB(gen_FixedArena_512KB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_1MB(gen_FixedArena_1MB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_megabytes(1));
}

inline gen_ssize gen_fixed_arena_size_remaining_1MB(gen_FixedArena_1MB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_1MB(gen_FixedArena_1MB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_2MB(gen_FixedArena_2MB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_megabytes(2));
}

inline gen_ssize gen_fixed_arena_size_remaining_2MB(gen_FixedArena_2MB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_2MB(gen_FixedArena_2MB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

inline void gen_fixed_arena_init_4MB(gen_FixedArena_4MB* result)
{
	result->arena = gen_arena_init_from_memory(&result->memory[0], gen_megabytes(4));
}

inline gen_ssize gen_fixed_arena_size_remaining_4MB(gen_FixedArena_4MB* fixed_arena, gen_ssize alignment)
{
	return gen_arena_size_remaining(&fixed_arena->arena, alignment);
}

inline void gen_fixed_arena_free_4MB(gen_FixedArena_4MB* fixed_arena)
{
	gen_arena_free(&fixed_arena->arena);
}

#define gen_fixed_arena_allocator_info(fixed_arena) ((gen_AllocatorInfo) { gen_arena_allocator_proc, &(fixed_arena)->arena })
#define gen_fixed_arena_init(expr)                          \
	_Generic(                                               \
	    (expr),                                             \
	    gen_FixedArena_1KB *: gen_fixed_arena_init_1KB,     \
	    gen_FixedArena_4KB *: gen_fixed_arena_init_4KB,     \
	    gen_FixedArena_8KB *: gen_fixed_arena_init_8KB,     \
	    gen_FixedArena_16KB *: gen_fixed_arena_init_16KB,   \
	    gen_FixedArena_32KB *: gen_fixed_arena_init_32KB,   \
	    gen_FixedArena_64KB *: gen_fixed_arena_init_64KB,   \
	    gen_FixedArena_128KB *: gen_fixed_arena_init_128KB, \
	    gen_FixedArena_256KB *: gen_fixed_arena_init_256KB, \
	    gen_FixedArena_512KB *: gen_fixed_arena_init_512KB, \
	    gen_FixedArena_1MB *: gen_fixed_arena_init_1MB,     \
	    gen_FixedArena_2MB *: gen_fixed_arena_init_2MB,     \
	    gen_FixedArena_4MB *: gen_fixed_arena_init_4MB,     \
	    default: gen_generic_selection_fail                 \
	) GEN_RESOLVED_FUNCTION_CALL(expr)

#define gen_fixed_arena_free(expr)                          \
	_Generic(                                               \
	    (expr),                                             \
	    gen_FixedArena_1KB *: gen_fixed_arena_free_1KB,     \
	    gen_FixedArena_4KB *: gen_fixed_arena_free_4KB,     \
	    gen_FixedArena_8KB *: gen_fixed_arena_free_8KB,     \
	    gen_FixedArena_16KB *: gen_fixed_arena_free_16KB,   \
	    gen_FixedArena_32KB *: gen_fixed_arena_free_32KB,   \
	    gen_FixedArena_64KB *: gen_fixed_arena_free_64KB,   \
	    gen_FixedArena_128KB *: gen_fixed_arena_free_128KB, \
	    gen_FixedArena_256KB *: gen_fixed_arena_free_256KB, \
	    gen_FixedArena_512KB *: gen_fixed_arena_free_512KB, \
	    gen_FixedArena_1MB *: gen_fixed_arena_free_1MB,     \
	    gen_FixedArena_2MB *: gen_fixed_arena_free_2MB,     \
	    gen_FixedArena_4MB *: gen_fixed_arena_free_4MB,     \
	    default: gen_generic_selection_fail                 \
	) GEN_RESOLVED_FUNCTION_CALL(expr)

#define gen_fixed_arena_size_remaining(expr, alignment)               \
	_Generic(                                                         \
	    (expr),                                                       \
	    gen_FixedArena_1KB *: gen_fixed_arena_size_remaining_1KB,     \
	    gen_FixedArena_4KB *: gen_fixed_arena_size_remaining_4KB,     \
	    gen_FixedArena_8KB *: gen_fixed_arena_size_remaining_8KB,     \
	    gen_FixedArena_16KB *: gen_fixed_arena_size_remaining_16KB,   \
	    gen_FixedArena_32KB *: gen_fixed_arena_size_remaining_32KB,   \
	    gen_FixedArena_64KB *: gen_fixed_arena_size_remaining_64KB,   \
	    gen_FixedArena_128KB *: gen_fixed_arena_size_remaining_128KB, \
	    gen_FixedArena_256KB *: gen_fixed_arena_size_remaining_256KB, \
	    gen_FixedArena_512KB *: gen_fixed_arena_size_remaining_512KB, \
	    gen_FixedArena_1MB *: gen_fixed_arena_size_remaining_1MB,     \
	    gen_FixedArena_2MB *: gen_fixed_arena_size_remaining_2MB,     \
	    gen_FixedArena_4MB *: gen_fixed_arena_size_remaining_4MB,     \
	    default: gen_generic_selection_fail                           \
	) GEN_RESOLVED_FUNCTION_CALL(expr, alignment)

#pragma endregion FixedArena


#pragma region gen_Pool

struct gen_Pool;
typedef struct gen_Pool gen_Pool;

GEN_API void*
    gen_pool_allocator_proc(void* allocator_data, gen_AllocType type, gen_ssize size, gen_ssize alignment, void* old_memory, gen_ssize old_size, gen_u64 flags);

gen_Pool          gen_pool_init(gen_AllocatorInfo backing, gen_ssize num_blocks, gen_ssize block_size);
gen_Pool          gen_pool_init_align(gen_AllocatorInfo backing, gen_ssize num_blocks, gen_ssize block_size, gen_ssize block_align);
gen_AllocatorInfo gen_pool_allocator_info(gen_Pool* pool);
GEN_API void      gen_pool_clear(gen_Pool* pool);
void              gen_pool_free(gen_Pool* pool);

struct gen_Pool
{
	gen_AllocatorInfo Backing;
	void*             PhysicalStart;
	void*             FreeList;
	gen_ssize         BlockSize;
	gen_ssize         BlockAlign;
	gen_ssize         TotalSize;
	gen_ssize         NumBlocks;
};
typedef struct gen_Pool gen_Pool;

inline gen_AllocatorInfo gen_pool_allocator_info(gen_Pool* pool)
{
	gen_AllocatorInfo info = { gen_pool_allocator_proc, pool };
	return info;
}

inline gen_Pool gen_pool_init(gen_AllocatorInfo backing, gen_ssize num_blocks, gen_ssize block_size)
{
	return gen_pool_init_align(backing, num_blocks, block_size, GEN_DEFAULT_MEMORY_ALIGNMENT);
}

inline void gen_pool_free(gen_Pool* pool)
{
	if (pool->Backing.Proc)
	{
		gen_allocator_free(pool->Backing, pool->PhysicalStart);
	}
}

#pragma endregion gen_Pool

inline gen_b32 gen_is_power_of_two(gen_ssize x)
{
	if (x <= 0)
		return false;
	return ! (x & (x - 1));
}

inline gen_mem_ptr gen_align_forward(void* ptr, gen_ssize alignment)
{
	GEN_ASSERT(gen_is_power_of_two(alignment));
	gen_uptr p       = gen_to_uptr(ptr);
	gen_uptr forward = (p + (alignment - 1)) & ~(alignment - 1);

	return gen_to_mem_ptr(forward);
}

inline gen_s64 align_forward_s64(gen_s64 value, gen_ssize alignment)
{
	return value + (alignment - value % alignment) % alignment;
}

inline void* gen_pointer_add(void* ptr, gen_ssize bytes)
{
	return gen_rcast(void*, gen_rcast(gen_u8*, ptr) + bytes);
}

inline void const* gen_pointer_add_const(void const* ptr, gen_ssize bytes)
{
	return gen_rcast(void const*, gen_rcast(gen_u8 const*, ptr) + bytes);
}

inline gen_sptr gen_pointer_diff(gen_mem_ptr_const begin, gen_mem_ptr_const end)
{
	return gen_scast(gen_ssize, gen_rcast(gen_u8 const*, end) - gen_rcast(gen_u8 const*, begin));
}

inline void* gen_mem_move(void* destination, void const* source, gen_ssize byte_count)
{
	if (destination == NULL)
	{
		return NULL;
	}

	gen_u8*       dest_ptr = gen_rcast(gen_u8*, destination);
	gen_u8 const* src_ptr  = gen_rcast(gen_u8 const*, source);

	if (dest_ptr == src_ptr)
		return dest_ptr;

	if (src_ptr + byte_count <= dest_ptr || dest_ptr + byte_count <= src_ptr)    // NOTE: Non-overlapping
		return gen_mem_copy(dest_ptr, src_ptr, byte_count);

	if (dest_ptr < src_ptr)
	{
		if (gen_to_uptr(src_ptr) % gen_size_of(gen_ssize) == gen_to_uptr(dest_ptr) % gen_size_of(gen_ssize))
		{
			while (gen_pcast(gen_uptr, dest_ptr) % gen_size_of(gen_ssize))
			{
				if (! byte_count--)
					return destination;

				*dest_ptr++ = *src_ptr++;
			}
			while (byte_count >= gen_size_of(gen_ssize))
			{
				*gen_rcast(gen_ssize*, dest_ptr)  = *gen_rcast(gen_ssize const*, src_ptr);
				byte_count                       -= gen_size_of(gen_ssize);
				dest_ptr                         += gen_size_of(gen_ssize);
				src_ptr                          += gen_size_of(gen_ssize);
			}
		}
		for (; byte_count; byte_count--)
			*dest_ptr++ = *src_ptr++;
	}
	else
	{
		if ((gen_to_uptr(src_ptr) % gen_size_of(gen_ssize)) == (gen_to_uptr(dest_ptr) % gen_size_of(gen_ssize)))
		{
			while (gen_to_uptr(dest_ptr + byte_count) % gen_size_of(gen_ssize))
			{
				if (! byte_count--)
					return destination;

				dest_ptr[byte_count] = src_ptr[byte_count];
			}
			while (byte_count >= gen_size_of(gen_ssize))
			{
				byte_count                                    -= gen_size_of(gen_ssize);
				*gen_rcast(gen_ssize*, dest_ptr + byte_count)  = *gen_rcast(gen_ssize const*, src_ptr + byte_count);
			}
		}
		while (byte_count)
			byte_count--, dest_ptr[byte_count] = src_ptr[byte_count];
	}

	return destination;
}

inline void* gen_mem_set(void* destination, gen_u8 fill_byte, gen_ssize byte_count)
{
	if (destination == NULL)
	{
		return NULL;
	}

	gen_ssize align_offset;
	gen_u8*   dest_ptr  = gen_rcast(gen_u8*, destination);
	gen_u32   fill_word = ((gen_u32)-1) / 255 * fill_byte;

	if (byte_count == 0)
		return destination;

	dest_ptr[0] = dest_ptr[byte_count - 1] = fill_byte;
	if (byte_count < 3)
		return destination;

	dest_ptr[1] = dest_ptr[byte_count - 2] = fill_byte;
	dest_ptr[2] = dest_ptr[byte_count - 3] = fill_byte;
	if (byte_count < 7)
		return destination;

	dest_ptr[3] = dest_ptr[byte_count - 4] = fill_byte;
	if (byte_count < 9)
		return destination;

	align_offset                                       = -gen_to_sptr(dest_ptr) & 3;
	dest_ptr                                          += align_offset;
	byte_count                                        -= align_offset;
	byte_count                                        &= -4;

	*gen_rcast(gen_u32*, (dest_ptr + 0))               = fill_word;
	*gen_rcast(gen_u32*, (dest_ptr + byte_count - 4))  = fill_word;
	if (byte_count < 9)
		return destination;

	*gen_rcast(gen_u32*, dest_ptr + 4)               = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + 8)               = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + byte_count - 12) = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + byte_count - 8)  = fill_word;
	if (byte_count < 25)
		return destination;

	*gen_rcast(gen_u32*, dest_ptr + 12)               = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + 16)               = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + 20)               = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + 24)               = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + byte_count - 28)  = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + byte_count - 24)  = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + byte_count - 20)  = fill_word;
	*gen_rcast(gen_u32*, dest_ptr + byte_count - 16)  = fill_word;

	align_offset                                      = 24 + gen_to_uptr(dest_ptr) & 4;
	dest_ptr                                         += align_offset;
	byte_count                                       -= align_offset;

	{
		gen_u64 fill_doubleword = (gen_scast(gen_u64, fill_word) << 32) | fill_word;
		while (byte_count > 31)
		{
			*gen_rcast(gen_u64*, dest_ptr + 0)   = fill_doubleword;
			*gen_rcast(gen_u64*, dest_ptr + 8)   = fill_doubleword;
			*gen_rcast(gen_u64*, dest_ptr + 16)  = fill_doubleword;
			*gen_rcast(gen_u64*, dest_ptr + 24)  = fill_doubleword;

			byte_count                          -= 32;
			dest_ptr                            += 32;
		}
	}

	return destination;
}

inline void* gen_alloc_align(gen_AllocatorInfo a, gen_ssize size, gen_ssize alignment)
{
	return a.Proc(a.Data, EAllocation_ALLOC, size, alignment, gen_nullptr, 0, GEN_DEFAULT_ALLOCATOR_FLAGS);
}

inline void* gen_alloc(gen_AllocatorInfo a, gen_ssize size)
{
	return gen_alloc_align(a, size, GEN_DEFAULT_MEMORY_ALIGNMENT);
}

inline void gen_allocator_free(gen_AllocatorInfo a, void* ptr)
{
	if (ptr != gen_nullptr)
		a.Proc(a.Data, EAllocation_FREE, 0, 0, ptr, 0, GEN_DEFAULT_ALLOCATOR_FLAGS);
}

inline void gen_free_all(gen_AllocatorInfo a)
{
	a.Proc(a.Data, EAllocation_FREE_ALL, 0, 0, gen_nullptr, 0, GEN_DEFAULT_ALLOCATOR_FLAGS);
}

inline void* gen_resize(gen_AllocatorInfo a, void* ptr, gen_ssize old_size, gen_ssize new_size)
{
	return gen_resize_align(a, ptr, old_size, new_size, GEN_DEFAULT_MEMORY_ALIGNMENT);
}

inline void* gen_resize_align(gen_AllocatorInfo a, void* ptr, gen_ssize old_size, gen_ssize new_size, gen_ssize alignment)
{
	return a.Proc(a.Data, EAllocation_RESIZE, new_size, alignment, ptr, old_size, GEN_DEFAULT_ALLOCATOR_FLAGS);
}

inline void* gen_default_resize_align(gen_AllocatorInfo a, void* old_memory, gen_ssize old_size, gen_ssize new_size, gen_ssize alignment)
{
	if (! old_memory)
		return gen_alloc_align(a, new_size, alignment);

	if (new_size == 0)
	{
		gen_allocator_free(a, old_memory);
		return gen_nullptr;
	}

	if (new_size < old_size)
		new_size = old_size;

	if (old_size == new_size)
	{
		return old_memory;
	}
	else
	{
		void* new_memory = gen_alloc_align(a, new_size, alignment);
		if (! new_memory)
			return gen_nullptr;

		gen_mem_move(new_memory, old_memory, gen_min(new_size, old_size));
		gen_allocator_free(a, old_memory);
		return new_memory;
	}
}

inline void gen_zero_size(void* ptr, gen_ssize size)
{
	gen_mem_set(ptr, 0, size);
}

#pragma endregion Memory

#pragma region Printing


typedef struct gen_FileInfo gen_FileInfo;

#ifndef GEN_PRINTF_MAXLEN
#define GEN_PRINTF_MAXLEN gen_kilobytes(128)
#endif
typedef char gen_PrintF_Buffer[GEN_PRINTF_MAXLEN];

// NOTE: A locally persisting buffer is used internally
GEN_API char*     gen_c_str_fmt_buf(char const* fmt, ...);
GEN_API char*     gen_c_str_fmt_buf_va(char const* fmt, va_list va);
GEN_API gen_ssize gen_c_str_fmt(char* str, gen_ssize n, char const* fmt, ...);
GEN_API gen_ssize gen_c_str_fmt_va(char* str, gen_ssize n, char const* fmt, va_list va);
GEN_API gen_ssize gen_c_str_fmt_out_va(char const* fmt, va_list va);
GEN_API gen_ssize gen_c_str_fmt_out_err(char const* fmt, ...);
GEN_API gen_ssize gen_c_str_fmt_out_err_va(char const* fmt, va_list va);
GEN_API gen_ssize gen_c_str_fmt_file(gen_FileInfo* f, char const* fmt, ...);
GEN_API gen_ssize gen_c_str_fmt_file_va(gen_FileInfo* f, char const* fmt, va_list va);

#define gen_Msg_Invalid_Value "INVALID VALUE PROVIDED"

inline gen_ssize gen_log_fmt(char const* fmt, ...)
{
	gen_ssize res;
	va_list   va;

	va_start(va, fmt);
	res = gen_c_str_fmt_out_va(fmt, va);
	va_end(va);

	return res;
}

#pragma endregion Printing

#pragma region String Ops

const char* gen_char_first_occurence( const char* str, char c );

gen_b32   gen_char_is_alpha( char c );
gen_b32   gen_char_is_alphanumeric( char c );
gen_b32   gen_char_is_digit( char c );
gen_b32   gen_char_is_hex_digit( char c );
gen_b32   gen_char_is_space( char c );
char  gen_char_to_lower( char c );
char  gen_char_to_upper( char c );

gen_s32  gen_digit_to_int( char c );
gen_s32  hex_digit_to_int( char c );

gen_s32         gen_c_str_compare( const char* s1, const char* s2 );
gen_s32         gen_c_str_compare_len( const char* s1, const char* s2, gen_ssize len );
char*       gen_c_str_copy( char* dest, const char* source, gen_ssize len );
gen_ssize       gen_c_str_copy_nulpad( char* dest, const char* source, gen_ssize len );
gen_ssize       gen_c_str_len( const char* str );
gen_ssize       gen_c_str_len_capped( const char* str, gen_ssize max_len );
char*       gen_c_str_reverse( char* str );    // NOTE: ASCII only
char const* gen_c_str_skip( char const* str, char c );
char const* gen_c_str_skip_any( char const* str, char const* gen_char_list );
char const* gen_c_str_trim( char const* str, gen_b32 catch_newline );

// NOTE: ASCII only
void gen_c_str_to_lower( char* str );
void gen_c_str_to_upper( char* str );

GEN_API gen_s64  gen_c_str_to_i64( const char* str, char** gen_end_ptr, gen_s32 base );
GEN_API void gen_i64_to_str( gen_s64 value, char* string, gen_s32 base );
GEN_API void gen_u64_to_str( gen_u64 value, char* string, gen_s32 base );
GEN_API gen_f64  gen_c_str_to_f64( const char* str, char** gen_end_ptr );

inline
const char* gen_char_first_occurence( const char* s, char c )
{
	char ch = c;
	for ( ; *s != ch; s++ )
	{
		if ( *s == '\0' )
			return NULL;
	}
	return s;
}

inline
gen_b32 gen_char_is_alpha( char c )
{
	if ( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) )
		return true;
	return false;
}

inline
gen_b32 gen_char_is_alphanumeric( char c )
{
	return gen_char_is_alpha( c ) || gen_char_is_digit( c );
}

inline
gen_b32 gen_char_is_digit( char c )
{
	if ( c >= '0' && c <= '9' )
		return true;
	return false;
}

inline
gen_b32 gen_char_is_hex_digit( char c )
{
	if ( gen_char_is_digit( c ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) )
		return true;
	return false;
}

inline
gen_b32 gen_char_is_space( char c )
{
	if ( c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v' )
		return true;
	return false;
}

inline
char gen_char_to_lower( char c )
{
	if ( c >= 'A' && c <= 'Z' )
		return 'a' + ( c - 'A' );
	return c;
}

inline char gen_char_to_upper( char c )
{
	if ( c >= 'a' && c <= 'z' )
		return 'A' + ( c - 'a' );
	return c;
}

inline
gen_s32 gen_digit_to_int( char c )
{
	return gen_char_is_digit( c ) ? c - '0' : c - 'W';
}

inline
gen_s32 hex_digit_to_int( char c )
{
	if ( gen_char_is_digit( c ) )
		return gen_digit_to_int( c );
	else if ( gen_is_between( c, 'a', 'f' ) )
		return c - 'a' + 10;
	else if ( gen_is_between( c, 'A', 'F' ) )
		return c - 'A' + 10;
	return -1;
}

inline
gen_s32 gen_c_str_compare( const char* s1, const char* s2 )
{
	while ( *s1 && ( *s1 == *s2 ) )
	{
		s1++, s2++;
	}
	return *( gen_u8* )s1 - *( gen_u8* )s2;
}

inline
gen_s32 gen_c_str_compare_len( const char* s1, const char* s2, gen_ssize len )
{
	for ( ; len > 0; s1++, s2++, len-- )
	{
		if ( *s1 != *s2 )
			return ( ( s1 < s2 ) ? -1 : +1 );
		else if ( *s1 == '\0' )
			return 0;
	}
	return 0;
}

inline
char* gen_c_str_copy( char* dest, const char* source, gen_ssize len )
{
	GEN_ASSERT_NOT_NULL( dest );
	if ( source )
	{
		char* str = dest;
		while ( len > 0 && *source )
		{
			*str++ = *source++;
			len--;
		}
		while ( len > 0 )
		{
			*str++ = '\0';
			len--;
		}
	}
	return dest;
}

inline
gen_ssize gen_c_str_copy_nulpad( char* dest, const char* source, gen_ssize len )
{
	gen_ssize result = 0;
	GEN_ASSERT_NOT_NULL( dest );
	if ( source )
	{
		const char* source_start = source;
		char*       str          = dest;
		while ( len > 0 && *source )
		{
			*str++ = *source++;
			len--;
		}
		while ( len > 0 )
		{
			*str++ = '\0';
			len--;
		}

		result = source - source_start;
	}
	return result;
}

inline
gen_ssize gen_c_str_len( const char* str )
{
	if ( str == NULL )
	{
		return 0;
	}
	const char* p = str;
	while ( *str )
		str++;
	return str - p;
}

inline
gen_ssize gen_c_str_len_capped( const char* str, gen_ssize max_len )
{
	const char* end = gen_rcast(const char*, gen_mem_find( str, 0, max_len ));
	if ( end )
		return end - str;
	return max_len;
}

inline
char* gen_c_str_reverse( char* str )
{
	gen_ssize    len  = gen_c_str_len( str );
	char* a    = str + 0;
	char* b    = str + len - 1;
	len       /= 2;
	while ( len-- )
	{
		gen_swap( *a, *b );
		a++, b--;
	}
	return str;
}

inline
char const* gen_c_str_skip( char const* str, char c )
{
	while ( *str && *str != c )
	{
		++str;
	}
	return str;
}

inline
char const* gen_c_str_skip_any( char const* str, char const* gen_char_list )
{
	char const* closest_ptr     = gen_rcast( char const*, gen_pointer_add_const( gen_rcast(gen_mem_ptr_const, str), gen_c_str_len( str ) ));
	gen_ssize       gen_char_list_count = gen_c_str_len( gen_char_list );
	for ( gen_ssize i = 0; i < gen_char_list_count; i++ )
	{
		char const* p = gen_c_str_skip( str, gen_char_list[ i ] );
		closest_ptr   = gen_min( closest_ptr, p );
	}
	return closest_ptr;
}

inline
char const* gen_c_str_trim( char const* str, gen_b32 catch_newline )
{
	while ( *str && gen_char_is_space( *str ) && ( ! catch_newline || ( catch_newline && *str != '\n' ) ) )
	{
		++str;
	}
	return str;
}

inline
void gen_c_str_to_lower( char* str )
{
	if ( ! str )
		return;
	while ( *str )
	{
		*str = gen_char_to_lower( *str );
		str++;
	}
}

inline
void gen_c_str_to_upper( char* str )
{
	if ( ! str )
		return;
	while ( *str )
	{
		*str = gen_char_to_upper( *str );
		str++;
	}
}

#pragma endregion String Ops

#pragma region Containers

typedef struct gen_ArrayHeader gen_ArrayHeader;

struct gen_ArrayHeader
{
	gen_AllocatorInfo Allocator;
	gen_usize         Capacity;
	gen_usize         Num;
};

#define gen_Array(Type)               gen_Array_##Type
#define gen_array_grow_formula(value) (2 * value + 8)
#define gen_array_get_header(self)    ((gen_ArrayHeader*)(self) - 1)
#define gen_array_begin(array)        (array)
#define gen_array_end(array)          (array + gen_array_get_header(array)->Num)
#define gen_array_next(array, entry)  (entry + 1)

#define gen_array_init(selector_arg, ...)                                                                                                                     \
	_Generic(                                                                                                                                                 \
	    (*(selector_arg*)NULL),                                                                                                                               \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_init) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_init          \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_init) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_init)       \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_init) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_init)     \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_init) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_init) \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_init)                                                                    \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_init) default: gen_generic_selection_fail                           \
	) GEN_RESOLVED_FUNCTION_CALL(__VA_ARGS__)

#define gen_array_init_reserve(selector_arg, ...)                                                                                                           \
	_Generic(                                                                                                                                               \
	    (*(selector_arg*)NULL),                                                                                                                             \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_init_reserve                                                                       \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_init_reserve)                                                                    \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_init_reserve)                                                                  \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_init_reserve)                                                              \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_init_reserve)                                                          \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_init_reserve)                                                      \
	                        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_init_reserve)                                                  \
	                            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_init_reserve)                                              \
	                                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_init_reserve)                                          \
	                                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_init_reserve) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(__VA_ARGS__)

#define gen_array_append(selector_arg, ...)                                                                                                                   \
	_Generic(                                                                                                                                                 \
	    (selector_arg),                                                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_append) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_append      \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_append) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_append    \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_append) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_append)   \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_append) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_append) \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_append)                                                                      \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_append) default: gen_generic_selection_fail                             \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg, __VA_ARGS__)

#define gen_array_append_at(selector_arg, ...)                                                                                                                 \
	_Generic(                                                                                                                                                  \
	    (selector_arg),                                                                                                                                        \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_append_at) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_append_at \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_append_at)                                                                          \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_append_at)                                                                        \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_append_at)                                                                    \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_append_at)                                                                \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_append_at)                                                            \
	                        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_append_at)                                                        \
	                            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_append_at)                                                    \
	                                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_append_at) default: gen_generic_selection_fail           \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg, __VA_ARGS__)

#define gen_array_append_items(selector_arg, ...)                                                                                                           \
	_Generic(                                                                                                                                               \
	    (selector_arg),                                                                                                                                     \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_append_items                                                                       \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_append_items)                                                                    \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_append_items)                                                                  \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_append_items)                                                              \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_append_items)                                                          \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_append_items)                                                      \
	                        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_append_items)                                                  \
	                            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_append_items)                                              \
	                                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_append_items)                                          \
	                                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_append_items) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg, __VA_ARGS__)

#define gen_array_append_items_at(selector_arg, ...)                                                                                                           \
	_Generic(                                                                                                                                                  \
	    (selector_arg),                                                                                                                                        \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_append_items_at                                                                       \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_append_items_at)                                                                    \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_append_items_at)                                                                  \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_append_items_at)                                                              \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_append_items_at)                                                          \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_append_items_at)                                                      \
	                        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_append_items_at)                                                  \
	                            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_append_items_at)                                              \
	                                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_append_items_at)                                          \
	                                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_append_items_at) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg, __VA_ARGS__)

#define gen_array_back(selector_arg, ...)                                                                                                                     \
	_Generic(                                                                                                                                                 \
	    (selector_arg),                                                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_back) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_back          \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_back) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_back)       \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_back) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_back)     \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_back) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_back) \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_back)                                                                    \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_back) default: gen_generic_selection_fail                           \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg)

#define gen_array_clear(selector_arg, ...)                                                                                                                  \
	_Generic(                                                                                                                                               \
	    (selector_arg),                                                                                                                                     \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_clear) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_clear      \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_clear) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_clear    \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_clear) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_clear)   \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_clear) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_clear) \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_clear)                                                                     \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_clear) default: gen_generic_selection_fail                            \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg)

#define gen_array_fill(selector_arg, ...)                                                                                                                     \
	_Generic(                                                                                                                                                 \
	    (selector_arg),                                                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_fill) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_fill          \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_fill) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_fill)       \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_fill) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_fill)     \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_fill) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_fill) \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_fill)                                                                    \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_fill) default: gen_generic_selection_fail                           \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#define gen_array_free(selector_arg, ...)                                                                                                                     \
	_Generic(                                                                                                                                                 \
	    (selector_arg),                                                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_free) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_free          \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_free) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_free)       \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_free) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_free)     \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_free) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_free) \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_free)                                                                    \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_free) default: gen_generic_selection_fail                           \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg)

#define gen_array_grow(selector_arg, ...)                                                                                                                     \
	_Generic(                                                                                                                                                 \
	    (selector_arg),                                                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_grow) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_grow          \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_grow) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_grow)       \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_grow) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_grow)     \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_grow) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_grow) \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_grow)                                                                    \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_grow) default: gen_generic_selection_fail                           \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#define gen_array_num(selector_arg, ...)                                                                                                                    \
	_Generic(                                                                                                                                               \
	    (selector_arg),                                                                                                                                     \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_num) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_num          \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_num) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_num)       \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_num) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_num)     \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_num) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_num) \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_num)                                                                   \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_num) default: gen_generic_selection_fail                          \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg)

#define gen_array_pop(selector_arg, ...)                                                                                                                    \
	_Generic(                                                                                                                                               \
	    (selector_arg),                                                                                                                                     \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_pop) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_pop          \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_pop) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_pop)       \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_pop) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_pop)     \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_pop) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_pop) \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_pop)                                                                   \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_pop) default: gen_generic_selection_fail                          \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg)

#define gen_array_remove_at(selector_arg, ...)                                                                                                                 \
	_Generic(                                                                                                                                                  \
	    (selector_arg),                                                                                                                                        \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_remove_at) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_remove_at \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_remove_at)                                                                          \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_remove_at)                                                                        \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_remove_at)                                                                    \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_remove_at)                                                                \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_remove_at)                                                            \
	                        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_remove_at)                                                        \
	                            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_remove_at)                                                    \
	                                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_remove_at) default: gen_generic_selection_fail           \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#define gen_array_reserve(selector_arg, ...)                                                                                                                  \
	_Generic(                                                                                                                                                 \
	    (selector_arg),                                                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_reserve) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_reserve    \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_reserve) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_reserve  \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_reserve) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_reserve  \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_reserve) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_reserve) \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_reserve)                                                                         \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_reserve) default: gen_generic_selection_fail                                \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg, __VA_ARGS__)

#define gen_array_resize(selector_arg, ...)                                                                                                                   \
	_Generic(                                                                                                                                                 \
	    (selector_arg),                                                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_resize) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_resize      \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_resize) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_resize    \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_resize) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_resize)   \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_resize) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_resize) \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_resize)                                                                      \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_resize) default: gen_generic_selection_fail                             \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg, __VA_ARGS__)

#define gen_array_set_capacity(selector_arg, ...)                                                                                                           \
	_Generic(                                                                                                                                               \
	    (selector_arg),                                                                                                                                     \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__array_set_capacity                                                                       \
	    ) GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__array_set_capacity)                                                                    \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_3__array_set_capacity)                                                                  \
	            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_4__array_set_capacity)                                                              \
	                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_5__array_set_capacity)                                                          \
	                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_6__array_set_capacity)                                                      \
	                        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_7__array_set_capacity)                                                  \
	                            GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_8__array_set_capacity)                                              \
	                                GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_9__array_set_capacity)                                          \
	                                    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_10__array_set_capacity) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

typedef struct gen_HT_FindResult_Def gen_HT_FindResult;

struct gen_HT_FindResult_Def
{
	gen_ssize HashIndex;
	gen_ssize PrevIndex;
	gen_ssize EntryIndex;
};

#define gen_HashTable(_type)            struct gen_HashTable_##_type
#define gen_HashTable_CriticalLoadScale 0.7f

#define gen_hashtable_init(selector_arg, ...)                                                                              \
	_Generic(                                                                                                              \
	    (*(selector_arg*)NULL),                                                                                            \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_init)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_init) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(__VA_ARGS__)

#define gen_hashtable_init_reserve(selector_arg, ...)                                                                              \
	_Generic(                                                                                                                      \
	    (*(selector_arg*)NULL),                                                                                                    \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_init_reserve)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_init_reserve) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(__VA_ARGS__)

#define gen_hashtable_clear(selector_arg, ...)                                                                              \
	_Generic(                                                                                                               \
	    (selector_arg),                                                                                                     \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_clear)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_clear) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg)

#define gen_hashtable_destroy(selector_arg, ...)                                                                              \
	_Generic(                                                                                                                 \
	    (selector_arg),                                                                                                       \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_destroy)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_destroy) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg)

#define gen_hashtable_get(selector_arg, ...)                                                                              \
	_Generic(                                                                                                             \
	    (selector_arg),                                                                                                   \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_get)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_get) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#define gen_hashtable_grow(selector_arg, ...)                                                                              \
	_Generic(                                                                                                              \
	    (selector_arg),                                                                                                    \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_grow)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_grow) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg)

#define gen_hashtable_rehash(selector_arg, ...)                                                                              \
	_Generic(                                                                                                                \
	    (selector_arg),                                                                                                      \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_rehash)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_rehash) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#define gen_hashtable_rehash_fast(selector_arg, ...)                                                                              \
	_Generic(                                                                                                                     \
	    (selector_arg),                                                                                                           \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_rehash_fast)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_rehash_fast) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg)

#define gen_hashtable_remove(selector_arg, ...)                                                                              \
	_Generic(                                                                                                                \
	    (selector_arg),                                                                                                      \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_remove)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_remove) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#define gen_hashtable_remove_entry(selector_arg, ...)                                                                              \
	_Generic(                                                                                                                      \
	    (selector_arg),                                                                                                            \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_remove_entry)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_remove_entry) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#define gen_hashtable_set(selector_arg, ...)                                                                              \
	_Generic(                                                                                                             \
	    (selector_arg),                                                                                                   \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_set)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_set) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(&selector_arg, __VA_ARGS__)

#define gen_hashtable_slot(selector_arg, ...)                                                                              \
	_Generic(                                                                                                              \
	    (selector_arg),                                                                                                    \
	    GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_1__hashtable_slot)                                         \
	        GEN_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GEN_GENERIC_SLOT_2__hashtable_slot) default: gen_generic_selection_fail \
	) GEN_RESOLVED_FUNCTION_CALL(selector_arg, __VA_ARGS__)

#pragma region gen_Array_gen_ssize

#define GEN_GENERIC_SLOT_1__array_init            gen_ssize, gen_Array_gen_ssize_init
#define GEN_GENERIC_SLOT_1__array_init_reserve    gen_ssize, gen_Array_gen_ssize_init_reserve
#define GEN_GENERIC_SLOT_1__array_append          gen_Array_gen_ssize, gen_Array_gen_ssize_append
#define GEN_GENERIC_SLOT_1__array_append_items    gen_Array_gen_ssize, gen_Array_gen_ssize_append_items
#define GEN_GENERIC_SLOT_1__array_append_at       gen_Array_gen_ssize, gen_Array_gen_ssize_append_at
#define GEN_GENERIC_SLOT_1__array_append_items_at gen_Array_gen_ssize, gen_Array_gen_ssize_append_items_at
#define GEN_GENERIC_SLOT_1__array_back            gen_Array_gen_ssize, gen_Array_gen_ssize_back
#define GEN_GENERIC_SLOT_1__array_clear           gen_Array_gen_ssize, gen_Array_gen_ssize_clear
#define GEN_GENERIC_SLOT_1__array_fill            gen_Array_gen_ssize, gen_Array_gen_ssize_fill
#define GEN_GENERIC_SLOT_1__array_free            gen_Array_gen_ssize, gen_Array_gen_ssize_free
#define GEN_GENERIC_SLOT_1__array_grow            gen_Array_gen_ssize*, gen_Array_gen_ssize_grow
#define GEN_GENERIC_SLOT_1__array_num             gen_Array_gen_ssize, gen_Array_gen_ssize_num
#define GEN_GENERIC_SLOT_1__array_pop             gen_Array_gen_ssize, gen_Array_gen_ssize_pop
#define GEN_GENERIC_SLOT_1__array_remove_at       gen_Array_gen_ssize, gen_Array_gen_ssize_remove_at
#define GEN_GENERIC_SLOT_1__array_reserve         gen_Array_gen_ssize, gen_Array_gen_ssize_reserve
#define GEN_GENERIC_SLOT_1__array_resize          gen_Array_gen_ssize, gen_Array_gen_ssize_resize
#define GEN_GENERIC_SLOT_1__array_set_capacity    gen_Array_gen_ssize*, gen_Array_gen_ssize_set_capacity

typedef gen_ssize*  gen_Array_gen_ssize;
gen_Array_gen_ssize gen_Array_gen_ssize_init(gen_AllocatorInfo allocator);
gen_Array_gen_ssize gen_Array_gen_ssize_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                gen_Array_gen_ssize_append_array(gen_Array_gen_ssize* self, gen_Array_gen_ssize other);
bool                gen_Array_gen_ssize_append(gen_Array_gen_ssize* self, gen_ssize value);
bool                gen_Array_gen_ssize_append_items(gen_Array_gen_ssize* self, gen_ssize* items, gen_usize item_num);
bool                gen_Array_gen_ssize_append_at(gen_Array_gen_ssize* self, gen_ssize item, gen_usize idx);
bool                gen_Array_gen_ssize_append_items_at(gen_Array_gen_ssize* self, gen_ssize* items, gen_usize item_num, gen_usize idx);
gen_ssize*          gen_Array_gen_ssize_back(gen_Array_gen_ssize self);
void                gen_Array_gen_ssize_clear(gen_Array_gen_ssize self);
bool                gen_Array_gen_ssize_fill(gen_Array_gen_ssize self, gen_usize begin, gen_usize end, gen_ssize value);
void                gen_Array_gen_ssize_free(gen_Array_gen_ssize* self);
bool                gen_Array_gen_ssize_grow(gen_Array_gen_ssize* self, gen_usize min_capacity);
gen_usize           gen_Array_gen_ssize_num(gen_Array_gen_ssize self);
gen_ssize           gen_Array_gen_ssize_pop(gen_Array_gen_ssize self);
void                gen_Array_gen_ssize_remove_at(gen_Array_gen_ssize self, gen_usize idx);
bool                gen_Array_gen_ssize_reserve(gen_Array_gen_ssize* self, gen_usize new_capacity);
bool                gen_Array_gen_ssize_resize(gen_Array_gen_ssize* self, gen_usize num);
bool                gen_Array_gen_ssize_set_capacity(gen_Array_gen_ssize* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_ssize gen_Array_gen_ssize_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_ssize, allocator, initial_size);
}

inline gen_Array_gen_ssize gen_Array_gen_ssize_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_ssize) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_ssize*, header + 1);
}

gen_forceinline bool gen_Array_gen_ssize_append_array(gen_Array_gen_ssize* self, gen_Array_gen_ssize other)
{
	return gen_array_append_items(*self, (gen_Array_gen_ssize)other, gen_Array_gen_ssize_num(other));
}

inline bool gen_Array_gen_ssize_append(gen_Array_gen_ssize* self, gen_ssize value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_ssize_append_items(gen_Array_gen_ssize* self, gen_ssize* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_ssize) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_ssize_append_at(gen_Array_gen_ssize* self, gen_ssize item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_ssize target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_ssize));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_ssize_append_items_at(gen_Array_gen_ssize* self, gen_ssize* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_ssize* target = (*self) + idx + item_num;
	gen_ssize* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_ssize));
	gen_mem_copy(src, items, item_num * sizeof(gen_ssize));
	header->Num += item_num;
	return true;
}

inline gen_ssize* gen_Array_gen_ssize_back(gen_Array_gen_ssize self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_ssize_clear(gen_Array_gen_ssize self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_ssize_fill(gen_Array_gen_ssize self, gen_usize begin, gen_usize end, gen_ssize value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_ssize_free(gen_Array_gen_ssize* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_ssize_grow(gen_Array_gen_ssize* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_ssize_num(gen_Array_gen_ssize self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_ssize gen_Array_gen_ssize_pop(gen_Array_gen_ssize self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_ssize result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_ssize_remove_at(gen_Array_gen_ssize self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_ssize) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_ssize_reserve(gen_Array_gen_ssize* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_ssize_resize(gen_Array_gen_ssize* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_ssize_set_capacity(gen_Array_gen_ssize* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_ssize) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_ssize) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_ssize*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_ssize

#pragma region gen_Array_gen_u8

#define GEN_GENERIC_SLOT_4__array_init            gen_u8, gen_Array_gen_u8_init
#define GEN_GENERIC_SLOT_4__array_init_reserve    gen_u8, gen_Array_gen_u8_init_reserve
#define GEN_GENERIC_SLOT_4__array_append          gen_Array_gen_u8, gen_Array_gen_u8_append
#define GEN_GENERIC_SLOT_4__array_append_items    gen_Array_gen_u8, gen_Array_gen_u8_append_items
#define GEN_GENERIC_SLOT_4__array_append_at       gen_Array_gen_u8, gen_Array_gen_u8_append_at
#define GEN_GENERIC_SLOT_4__array_append_items_at gen_Array_gen_u8, gen_Array_gen_u8_append_items_at
#define GEN_GENERIC_SLOT_4__array_back            gen_Array_gen_u8, gen_Array_gen_u8_back
#define GEN_GENERIC_SLOT_4__array_clear           gen_Array_gen_u8, gen_Array_gen_u8_clear
#define GEN_GENERIC_SLOT_4__array_fill            gen_Array_gen_u8, gen_Array_gen_u8_fill
#define GEN_GENERIC_SLOT_4__array_free            gen_Array_gen_u8, gen_Array_gen_u8_free
#define GEN_GENERIC_SLOT_4__array_grow            gen_Array_gen_u8*, gen_Array_gen_u8_grow
#define GEN_GENERIC_SLOT_4__array_num             gen_Array_gen_u8, gen_Array_gen_u8_num
#define GEN_GENERIC_SLOT_4__array_pop             gen_Array_gen_u8, gen_Array_gen_u8_pop
#define GEN_GENERIC_SLOT_4__array_remove_at       gen_Array_gen_u8, gen_Array_gen_u8_remove_at
#define GEN_GENERIC_SLOT_4__array_reserve         gen_Array_gen_u8, gen_Array_gen_u8_reserve
#define GEN_GENERIC_SLOT_4__array_resize          gen_Array_gen_u8, gen_Array_gen_u8_resize
#define GEN_GENERIC_SLOT_4__array_set_capacity    gen_Array_gen_u8*, gen_Array_gen_u8_set_capacity

typedef gen_u8*  gen_Array_gen_u8;
gen_Array_gen_u8 gen_Array_gen_u8_init(gen_AllocatorInfo allocator);
gen_Array_gen_u8 gen_Array_gen_u8_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool             gen_Array_gen_u8_append_array(gen_Array_gen_u8* self, gen_Array_gen_u8 other);
bool             gen_Array_gen_u8_append(gen_Array_gen_u8* self, gen_u8 value);
bool             gen_Array_gen_u8_append_items(gen_Array_gen_u8* self, gen_u8* items, gen_usize item_num);
bool             gen_Array_gen_u8_append_at(gen_Array_gen_u8* self, gen_u8 item, gen_usize idx);
bool             gen_Array_gen_u8_append_items_at(gen_Array_gen_u8* self, gen_u8* items, gen_usize item_num, gen_usize idx);
gen_u8*          gen_Array_gen_u8_back(gen_Array_gen_u8 self);
void             gen_Array_gen_u8_clear(gen_Array_gen_u8 self);
bool             gen_Array_gen_u8_fill(gen_Array_gen_u8 self, gen_usize begin, gen_usize end, gen_u8 value);
void             gen_Array_gen_u8_free(gen_Array_gen_u8* self);
bool             gen_Array_gen_u8_grow(gen_Array_gen_u8* self, gen_usize min_capacity);
gen_usize        gen_Array_gen_u8_num(gen_Array_gen_u8 self);
gen_u8           gen_Array_gen_u8_pop(gen_Array_gen_u8 self);
void             gen_Array_gen_u8_remove_at(gen_Array_gen_u8 self, gen_usize idx);
bool             gen_Array_gen_u8_reserve(gen_Array_gen_u8* self, gen_usize new_capacity);
bool             gen_Array_gen_u8_resize(gen_Array_gen_u8* self, gen_usize num);
bool             gen_Array_gen_u8_set_capacity(gen_Array_gen_u8* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_u8 gen_Array_gen_u8_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_u8, allocator, initial_size);
}

inline gen_Array_gen_u8 gen_Array_gen_u8_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_u8) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_u8*, header + 1);
}

gen_forceinline bool gen_Array_gen_u8_append_array(gen_Array_gen_u8* self, gen_Array_gen_u8 other)
{
	return gen_array_append_items(*self, (gen_Array_gen_u8)other, gen_Array_gen_u8_num(other));
}

inline bool gen_Array_gen_u8_append(gen_Array_gen_u8* self, gen_u8 value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_u8_append_items(gen_Array_gen_u8* self, gen_u8* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_u8) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_u8_append_at(gen_Array_gen_u8* self, gen_u8 item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_u8 target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_u8));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_u8_append_items_at(gen_Array_gen_u8* self, gen_u8* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_u8* target = (*self) + idx + item_num;
	gen_u8* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_u8));
	gen_mem_copy(src, items, item_num * sizeof(gen_u8));
	header->Num += item_num;
	return true;
}

inline gen_u8* gen_Array_gen_u8_back(gen_Array_gen_u8 self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_u8_clear(gen_Array_gen_u8 self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_u8_fill(gen_Array_gen_u8 self, gen_usize begin, gen_usize end, gen_u8 value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_u8_free(gen_Array_gen_u8* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_u8_grow(gen_Array_gen_u8* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_u8_num(gen_Array_gen_u8 self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_u8 gen_Array_gen_u8_pop(gen_Array_gen_u8 self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_u8 result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_u8_remove_at(gen_Array_gen_u8 self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_u8) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_u8_reserve(gen_Array_gen_u8* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_u8_resize(gen_Array_gen_u8* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_u8_set_capacity(gen_Array_gen_u8* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_u8) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_u8) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_u8*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_u8

#pragma endregion Containers

#pragma region Hashing

GEN_API gen_u32 gen_crc32( void const* data, gen_ssize len );
GEN_API gen_u64 gen_crc64( void const* data, gen_ssize len );

#pragma endregion Hashing

#pragma region Strings


typedef struct gen_Str gen_Str;
struct gen_Str;

gen_Str     gen_to_str_from_c_str(char const* bad_string);
bool        gen_str_are_equal(gen_Str lhs, gen_Str rhs);
char const* gen_str_back(gen_Str str);
bool        gen_str_contains(gen_Str str, gen_Str substring);
gen_Str     gen_str_duplicate(gen_Str str, gen_AllocatorInfo allocator);
gen_b32     gen_str_starts_with(gen_Str str, gen_Str substring);
gen_Str     gen_str_visualize_whitespace(gen_Str str, gen_AllocatorInfo allocator);

// Constant string with length.
struct gen_Str
{
	char const* Ptr;
	gen_ssize   Len;
};

#define gen_cast_to_str(str) *gen_rcast(gen_Str*, (str) - sizeof(gen_ssize))

#ifndef gen_txt
#define gen_txt(text)            \
	(GEN_NS gen_Str)             \
	{                            \
		(text), sizeof(text) - 1 \
	}
#endif

GEN_API_C_BEGIN

gen_forceinline char const* gen_str_begin(gen_Str str)
{
	return str.Ptr;
}

gen_forceinline char const* gen_str_end(gen_Str str)
{
	return str.Ptr + str.Len;
}

gen_forceinline char const* gen_str_next(gen_Str str, char const* iter)
{
	return iter + 1;
}

GEN_API_C_END

inline bool gen_str_are_equal(gen_Str lhs, gen_Str rhs)
{
	if (lhs.Len != rhs.Len)
		return false;

	for (gen_ssize idx = 0; idx < lhs.Len; ++idx)
		if (lhs.Ptr[idx] != rhs.Ptr[idx])
			return false;

	return true;
}

inline char const* gen_str_back(gen_Str str)
{
	return &str.Ptr[str.Len - 1];
}

inline bool gen_str_contains(gen_Str str, gen_Str substring)
{
	if (substring.Len > str.Len)
		return false;

	gen_ssize main_len = str.Len;
	gen_ssize sub_len  = substring.Len;
	for (gen_ssize idx = 0; idx <= main_len - sub_len; ++idx)
	{
		if (gen_c_str_compare_len(str.Ptr + idx, substring.Ptr, sub_len) == 0)
			return true;
	}
	return false;
}

inline gen_b32 gen_str_starts_with(gen_Str str, gen_Str substring)
{
	if (substring.Len > str.Len)
		return false;

	gen_b32 result = gen_c_str_compare_len(str.Ptr, substring.Ptr, substring.Len) == 0;
	return result;
}

inline gen_Str gen_to_str_from_c_str(char const* bad_str)
{
	gen_Str result = { bad_str, gen_c_str_len(bad_str) };
	return result;
}

// Dynamic gen_StrBuilder
// This is directly based off the ZPL string api.
// They used a header pattern
// I kept it for simplicty of porting but its not necessary to keep it that way.
#pragma region gen_StrBuilder

typedef struct gen_StrBuilderHeader gen_StrBuilderHeader;
struct gen_StrBuilderHeader;


typedef char* gen_StrBuilder;

gen_forceinline gen_usize gen_strbuilder_grow_formula(gen_usize value);

GEN_API gen_StrBuilder gen_strbuilder_make_reserve(gen_AllocatorInfo allocator, gen_ssize capacity);
GEN_API gen_StrBuilder gen_strbuilder_make_length(gen_AllocatorInfo allocator, char const* str, gen_ssize length);
GEN_API bool           gen_strbuilder_make_space_for(gen_StrBuilder* str, char const* to_append, gen_ssize add_len);
GEN_API bool           gen_strbuilder_append_c_str_len(gen_StrBuilder* str, char const* gen_c_str_to_append, gen_ssize length);
GEN_API void           gen_strbuilder_trim(gen_StrBuilder str, char const* cut_set);
GEN_API gen_StrBuilder gen_strbuilder_visualize_whitespace(gen_StrBuilder const str);

gen_StrBuilder        gen_strbuilder_make_c_str(gen_AllocatorInfo allocator, char const* str);
gen_StrBuilder        gen_strbuilder_make_str(gen_AllocatorInfo allocator, gen_Str str);
gen_StrBuilder        gen_strbuilder_fmt(gen_AllocatorInfo allocator, char* buf, gen_ssize buf_size, char const* fmt, ...);
gen_StrBuilder        gen_strbuilder_fmt_buf(gen_AllocatorInfo allocator, char const* fmt, ...);
gen_StrBuilder        gen_strbuilder_join(gen_AllocatorInfo allocator, char const** parts, gen_ssize num_parts, char const* glue);
bool                  gen_strbuilder_are_equal(gen_StrBuilder const lhs, gen_StrBuilder const rhs);
bool                  gen_strbuilder_are_equal_str(gen_StrBuilder const lhs, gen_Str rhs);
bool                  gen_strbuilder_append_char(gen_StrBuilder* str, char c);
bool                  gen_strbuilder_append_c_str(gen_StrBuilder* str, char const* gen_c_str_to_append);
bool                  gen_strbuilder_append_str(gen_StrBuilder* str, gen_Str gen_c_str_to_append);
bool                  gen_strbuilder_append_string(gen_StrBuilder* str, gen_StrBuilder const other);
bool                  gen_strbuilder_append_fmt(gen_StrBuilder* str, char const* fmt, ...);
gen_ssize             gen_strbuilder_avail_space(gen_StrBuilder const str);
char*                 gen_strbuilder_back(gen_StrBuilder str);
bool                  gen_strbuilder_contains_str(gen_StrBuilder const str, gen_Str substring);
bool                  gen_strbuilder_contains_string(gen_StrBuilder const str, gen_StrBuilder const substring);
gen_ssize             gen_strbuilder_capacity(gen_StrBuilder const str);
void                  gen_strbuilder_clear(gen_StrBuilder str);
gen_StrBuilder        gen_strbuilder_duplicate(gen_StrBuilder const str, gen_AllocatorInfo allocator);
void                  gen_strbuilder_free(gen_StrBuilder* str);
gen_StrBuilderHeader* gen_strbuilder_get_header(gen_StrBuilder str);
gen_ssize             gen_strbuilder_length(gen_StrBuilder const str);
gen_b32               gen_strbuilder_starts_with_str(gen_StrBuilder const str, gen_Str substring);
gen_b32               gen_strbuilder_starts_with_string(gen_StrBuilder const str, gen_StrBuilder substring);
void                  gen_strbuilder_skip_line(gen_StrBuilder str);
void                  gen_strbuilder_strip_space(gen_StrBuilder str);
gen_Str               gen_strbuilder_to_str(gen_StrBuilder str);
void                  gen_strbuilder_trim_space(gen_StrBuilder str);

struct gen_StrBuilderHeader
{
	gen_AllocatorInfo Allocator;
	gen_ssize         Capacity;
	gen_ssize         Length;
};

gen_forceinline char* gen_strbuilder_begin(gen_StrBuilder str)
{
	return ((char*)str);
}

gen_forceinline char* gen_strbuilder_end(gen_StrBuilder str)
{
	return ((char*)str + gen_strbuilder_length(str));
}

gen_forceinline char* gen_strbuilder_next(gen_StrBuilder str, char const* iter)
{
	return ((char*)iter + 1);
}

gen_forceinline gen_usize gen_strbuilder_grow_formula(gen_usize value)
{
	// Using a very aggressive growth formula to reduce time mem_copying with recursive calls to append in this library.
	return 4 * value + 8;
}

gen_forceinline gen_StrBuilder gen_strbuilder_make_c_str(gen_AllocatorInfo allocator, char const* str)
{
	gen_ssize length = str ? gen_c_str_len(str) : 0;
	return gen_strbuilder_make_length(allocator, str, length);
}

gen_forceinline gen_StrBuilder gen_strbuilder_make_str(gen_AllocatorInfo allocator, gen_Str str)
{
	return gen_strbuilder_make_length(allocator, str.Ptr, str.Len);
}

inline gen_StrBuilder gen_strbuilder_fmt(gen_AllocatorInfo allocator, char* buf, gen_ssize buf_size, char const* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	gen_ssize res = gen_c_str_fmt_va(buf, buf_size, fmt, va) - 1;
	va_end(va);

	return gen_strbuilder_make_length(allocator, buf, res);
}

inline gen_StrBuilder gen_strbuilder_fmt_buf(gen_AllocatorInfo allocator, char const* fmt, ...)
{
	gen_local_persist gen_thread_local gen_PrintF_Buffer buf = gen_struct_init(gen_PrintF_Buffer, { 0 });

	va_list va;
	va_start(va, fmt);
	gen_ssize res = gen_c_str_fmt_va(buf, GEN_PRINTF_MAXLEN, fmt, va) - 1;
	va_end(va);

	return gen_strbuilder_make_length(allocator, buf, res);
}

inline gen_StrBuilder gen_strbuilder_join(gen_AllocatorInfo allocator, char const** parts, gen_ssize num_parts, char const* glue)
{
	gen_StrBuilder result = gen_strbuilder_make_c_str(allocator, "");

	for (gen_ssize idx = 0; idx < num_parts; ++idx)
	{
		gen_strbuilder_append_c_str(&result, parts[idx]);

		if (idx < num_parts - 1)
			gen_strbuilder_append_c_str(&result, glue);
	}

	return result;
}

gen_forceinline bool gen_strbuilder_append_char(gen_StrBuilder* str, char c)
{
	GEN_ASSERT(str != gen_nullptr);
	return gen_strbuilder_append_c_str_len(str, (char const*)&c, (gen_ssize)1);
}

gen_forceinline bool gen_strbuilder_append_c_str(gen_StrBuilder* str, char const* gen_c_str_to_append)
{
	GEN_ASSERT(str != gen_nullptr);
	return gen_strbuilder_append_c_str_len(str, gen_c_str_to_append, gen_c_str_len(gen_c_str_to_append));
}

gen_forceinline bool gen_strbuilder_append_str(gen_StrBuilder* str, gen_Str gen_c_str_to_append)
{
	GEN_ASSERT(str != gen_nullptr);
	return gen_strbuilder_append_c_str_len(str, gen_c_str_to_append.Ptr, gen_c_str_to_append.Len);
}

gen_forceinline bool gen_strbuilder_append_string(gen_StrBuilder* str, gen_StrBuilder const other)
{
	GEN_ASSERT(str != gen_nullptr);
	return gen_strbuilder_append_c_str_len(str, (char const*)other, gen_strbuilder_length(other));
}

inline bool gen_strbuilder_append_fmt(gen_StrBuilder* str, char const* fmt, ...)
{
	GEN_ASSERT(str != gen_nullptr);
	gen_ssize res;
	char      buf[GEN_PRINTF_MAXLEN] = { 0 };

	va_list va;
	va_start(va, fmt);
	res = gen_c_str_fmt_va(buf, gen_count_of(buf) - 1, fmt, va) - 1;
	va_end(va);

	return gen_strbuilder_append_c_str_len(str, (char const*)buf, res);
}

inline bool gen_strbuilder_are_equal_string(gen_StrBuilder const lhs, gen_StrBuilder const rhs)
{
	if (gen_strbuilder_length(lhs) != gen_strbuilder_length(rhs))
		return false;

	for (gen_ssize idx = 0; idx < gen_strbuilder_length(lhs); ++idx)
		if (lhs[idx] != rhs[idx])
			return false;

	return true;
}

inline bool gen_strbuilder_are_equal_str(gen_StrBuilder const lhs, gen_Str rhs)
{
	if (gen_strbuilder_length(lhs) != (rhs.Len))
		return false;

	for (gen_ssize idx = 0; idx < gen_strbuilder_length(lhs); ++idx)
		if (lhs[idx] != rhs.Ptr[idx])
			return false;

	return true;
}

gen_forceinline gen_ssize gen_strbuilder_avail_space(gen_StrBuilder const str)
{
	gen_StrBuilderHeader const* header = gen_rcast(gen_StrBuilderHeader const*, gen_scast(char const*, str) - sizeof(gen_StrBuilderHeader));
	return header->Capacity - header->Length;
}

gen_forceinline char* gen_strbuilder_back(gen_StrBuilder str)
{
	return &(str)[gen_strbuilder_length(str) - 1];
}

inline bool gen_strbuilder_contains_StrC(gen_StrBuilder const str, gen_Str substring)
{
	gen_StrBuilderHeader const* header = gen_rcast(gen_StrBuilderHeader const*, gen_scast(char const*, str) - sizeof(gen_StrBuilderHeader));

	if (substring.Len > header->Length)
		return false;

	gen_ssize main_len = header->Length;
	gen_ssize sub_len  = substring.Len;

	for (gen_ssize idx = 0; idx <= main_len - sub_len; ++idx)
	{
		if (gen_c_str_compare_len(str + idx, substring.Ptr, sub_len) == 0)
			return true;
	}

	return false;
}

inline bool gen_strbuilder_contains_string(gen_StrBuilder const str, gen_StrBuilder const substring)
{
	gen_StrBuilderHeader const* header = gen_rcast(gen_StrBuilderHeader const*, gen_scast(char const*, str) - sizeof(gen_StrBuilderHeader));

	if (gen_strbuilder_length(substring) > header->Length)
		return false;

	gen_ssize main_len = header->Length;
	gen_ssize sub_len  = gen_strbuilder_length(substring);

	for (gen_ssize idx = 0; idx <= main_len - sub_len; ++idx)
	{
		if (gen_c_str_compare_len(str + idx, substring, sub_len) == 0)
			return true;
	}

	return false;
}

gen_forceinline gen_ssize gen_strbuilder_capacity(gen_StrBuilder const str)
{
	gen_StrBuilderHeader const* header = gen_rcast(gen_StrBuilderHeader const*, gen_scast(char const*, str) - sizeof(gen_StrBuilderHeader));
	return header->Capacity;
}

gen_forceinline void gen_strbuilder_clear(gen_StrBuilder str)
{
	gen_strbuilder_get_header(str)->Length = 0;
}

gen_forceinline gen_StrBuilder gen_strbuilder_duplicate(gen_StrBuilder const str, gen_AllocatorInfo allocator)
{
	return gen_strbuilder_make_length(allocator, str, gen_strbuilder_length(str));
}

gen_forceinline void gen_strbuilder_free(gen_StrBuilder* str)
{
	GEN_ASSERT(str != gen_nullptr);
	if (! (*str))
		return;

	gen_StrBuilderHeader* header = gen_strbuilder_get_header(*str);
	gen_allocator_free(header->Allocator, header);
}

gen_forceinline gen_StrBuilderHeader* gen_strbuilder_get_header(gen_StrBuilder str)
{
	return (gen_StrBuilderHeader*)(gen_scast(char*, str) - sizeof(gen_StrBuilderHeader));
}

gen_forceinline gen_ssize gen_strbuilder_length(gen_StrBuilder const str)
{
	gen_StrBuilderHeader const* header = gen_rcast(gen_StrBuilderHeader const*, gen_scast(char const*, str) - sizeof(gen_StrBuilderHeader));
	return header->Length;
}

gen_forceinline gen_b32 gen_strbuilder_starts_with_str(gen_StrBuilder const str, gen_Str substring)
{
	if (substring.Len > gen_strbuilder_length(str))
		return false;

	gen_b32 result = gen_c_str_compare_len(str, substring.Ptr, substring.Len) == 0;
	return result;
}

gen_forceinline gen_b32 gen_strbuilder_starts_with_string(gen_StrBuilder const str, gen_StrBuilder substring)
{
	if (gen_strbuilder_length(substring) > gen_strbuilder_length(str))
		return false;

	gen_b32 result = gen_c_str_compare_len(str, substring, gen_strbuilder_length(substring) - 1) == 0;
	return result;
}

inline void gen_strbuilder_skip_line(gen_StrBuilder str)
{
#define current (*scanner)
	char* scanner = str;
	while (current != '\r' && current != '\n')
	{
		++scanner;
	}

	gen_s32 new_length = scanner - str;

	if (current == '\r')
	{
		new_length += 1;
	}

	gen_mem_move((char*)str, scanner, new_length);

	gen_StrBuilderHeader* header = gen_strbuilder_get_header(str);
	header->Length               = new_length;
#undef current
}

inline void gen_strbuilder_strip_space(gen_StrBuilder str)
{
	char* write_pos = str;
	char* read_pos  = str;

	while (*read_pos)
	{
		if (! gen_char_is_space(*read_pos))
		{
			*write_pos = *read_pos;
			write_pos++;
		}
		read_pos++;
	}
	write_pos[0] = '\0';    // Null-terminate the modified string

	// Update the length if needed
	gen_strbuilder_get_header(str)->Length = write_pos - str;
}

gen_forceinline gen_Str gen_strbuilder_to_str(gen_StrBuilder str)
{
	gen_Str result = { (char const*)str, gen_strbuilder_length(str) };
	return result;
}

gen_forceinline void gen_strbuilder_trim_space(gen_StrBuilder str)
{
	gen_strbuilder_trim(str, " \t\r\n\v\f");
}

#pragma endregion gen_StrBuilder

gen_forceinline gen_Str gen_str_duplicate(gen_Str str, gen_AllocatorInfo allocator)
{
	gen_Str result = gen_strbuilder_to_str(gen_strbuilder_make_length(allocator, str.Ptr, str.Len));
	return result;
}

inline gen_Str gen_str_visualize_whitespace(gen_Str str, gen_AllocatorInfo allocator)
{
	gen_StrBuilder result = gen_strbuilder_make_reserve(allocator, str.Len * 2);    // Assume worst case for space requirements.
	for (char const* c = gen_str_begin(str); c != gen_str_end(str); c = gen_str_next(str, c))
		switch (*c)
		{
			case ' ':
				gen_strbuilder_append_str(&result, gen_txt("·"));
				break;
			case '\t':
				gen_strbuilder_append_str(&result, gen_txt("→"));
				break;
			case '\n':
				gen_strbuilder_append_str(&result, gen_txt("↵"));
				break;
			case '\r':
				gen_strbuilder_append_str(&result, gen_txt("⏎"));
				break;
			case '\v':
				gen_strbuilder_append_str(&result, gen_txt("⇕"));
				break;
			case '\f':
				gen_strbuilder_append_str(&result, gen_txt("⌂"));
				break;
			default:
				gen_strbuilder_append_char(&result, *c);
				break;
		}
	return gen_strbuilder_to_str(result);
}

// Represents strings cached with the string table.
// Should never be modified, if changed string is desired, cache_string( str ) another.
typedef gen_Str gen_StrCached;

// Implements basic string interning. Data structure is based off the ZPL Hashtable.
#pragma region gen_StringTable

#define GEN_GENERIC_SLOT_1__hashtable_init         gen_Str, gen_StringTable_init
#define GEN_GENERIC_SLOT_1__hashtable_init_reserve gen_Str, gen_StringTable_init_reserve
#define GEN_GENERIC_SLOT_1__hashtable_clear        gen_StringTable, gen_StringTable_clear
#define GEN_GENERIC_SLOT_1__hashtable_destroy      gen_StringTable, gen_StringTable_destroy
#define GEN_GENERIC_SLOT_1__hashtable_get          gen_StringTable, gen_StringTable_get
#define GEN_GENERIC_SLOT_1__hashtable_map          gen_StringTable, gen_StringTable_map
#define GEN_GENERIC_SLOT_1__hashtable_map_mut      gen_StringTable, gen_StringTable_map_mut
#define GEN_GENERIC_SLOT_1__hashtable_grow         gen_StringTable*, gen_StringTable_grow
#define GEN_GENERIC_SLOT_1__hashtable_rehash       gen_StringTable*, gen_StringTable_rehash
#define GEN_GENERIC_SLOT_1__hashtable_rehash_fast  gen_StringTable, gen_StringTable_rehash_fast
#define GEN_GENERIC_SLOT_1__hashtable_remove_entry gen_StringTable, gen_StringTable_remove_entry
#define GEN_GENERIC_SLOT_1__hashtable_set          gen_StringTable, gen_StringTable_set
#define GEN_GENERIC_SLOT_1__hashtable_slot         gen_StringTable, gen_StringTable_slot

#define GEN_GENERIC_SLOT_1__hashtable__add_entry   gen_StringTable*, gen_StringTable__add_entry
#define GEN_GENERIC_SLOT_1__hashtable__find        gen_StringTable, gen_StringTable__find
#define GEN_GENERIC_SLOT_1__hashtable__full        gen_StringTable, gen_StringTable__full

typedef struct gen_HashTable_gen_Str gen_StringTable;
typedef struct gen_HTE_StringTable   gen_HTE_StringTable;

struct gen_HTE_StringTable
{
	gen_u64   Key;
	gen_ssize Next;
	gen_Str   Value;
};

typedef void (*gen_StringTable_MapProc)(gen_StringTable self, gen_u64 key, gen_Str value);
typedef void (*gen_StringTable_MapMutProc)(gen_StringTable self, gen_u64 key, gen_Str* value);

#pragma region gen_Arr_HTE_StringTable

#define GEN_GENERIC_SLOT_3__array_init            gen_HTE_StringTable, gen_Arr_HTE_StringTable_init
#define GEN_GENERIC_SLOT_3__array_init_reserve    gen_HTE_StringTable, gen_Arr_HTE_StringTable_init_reserve
#define GEN_GENERIC_SLOT_3__array_append          gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_append
#define GEN_GENERIC_SLOT_3__array_append_items    gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_append_items
#define GEN_GENERIC_SLOT_3__array_append_at       gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_append_at
#define GEN_GENERIC_SLOT_3__array_append_items_at gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_append_items_at
#define GEN_GENERIC_SLOT_3__array_back            gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_back
#define GEN_GENERIC_SLOT_3__array_clear           gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_clear
#define GEN_GENERIC_SLOT_3__array_fill            gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_fill
#define GEN_GENERIC_SLOT_3__array_free            gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_free
#define GEN_GENERIC_SLOT_3__array_grow            gen_Arr_HTE_StringTable*, gen_Arr_HTE_StringTable_grow
#define GEN_GENERIC_SLOT_3__array_num             gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_num
#define GEN_GENERIC_SLOT_3__array_pop             gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_pop
#define GEN_GENERIC_SLOT_3__array_remove_at       gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_remove_at
#define GEN_GENERIC_SLOT_3__array_reserve         gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_reserve
#define GEN_GENERIC_SLOT_3__array_resize          gen_Arr_HTE_StringTable, gen_Arr_HTE_StringTable_resize
#define GEN_GENERIC_SLOT_3__array_set_capacity    gen_Arr_HTE_StringTable*, gen_Arr_HTE_StringTable_set_capacity

typedef gen_HTE_StringTable* gen_Arr_HTE_StringTable;
gen_Arr_HTE_StringTable      gen_Arr_HTE_StringTable_init(gen_AllocatorInfo allocator);
gen_Arr_HTE_StringTable      gen_Arr_HTE_StringTable_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                         gen_Arr_HTE_StringTable_append_array(gen_Arr_HTE_StringTable* self, gen_Arr_HTE_StringTable other);
bool                         gen_Arr_HTE_StringTable_append(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable value);
bool                         gen_Arr_HTE_StringTable_append_items(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable* items, gen_usize item_num);
bool                         gen_Arr_HTE_StringTable_append_at(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable item, gen_usize idx);
bool                 gen_Arr_HTE_StringTable_append_items_at(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable* items, gen_usize item_num, gen_usize idx);
gen_HTE_StringTable* gen_Arr_HTE_StringTable_back(gen_Arr_HTE_StringTable self);
void                 gen_Arr_HTE_StringTable_clear(gen_Arr_HTE_StringTable self);
bool                 gen_Arr_HTE_StringTable_fill(gen_Arr_HTE_StringTable self, gen_usize begin, gen_usize end, gen_HTE_StringTable value);
void                 gen_Arr_HTE_StringTable_free(gen_Arr_HTE_StringTable* self);
bool                 gen_Arr_HTE_StringTable_grow(gen_Arr_HTE_StringTable* self, gen_usize min_capacity);
gen_usize            gen_Arr_HTE_StringTable_num(gen_Arr_HTE_StringTable self);
gen_HTE_StringTable  gen_Arr_HTE_StringTable_pop(gen_Arr_HTE_StringTable self);
void                 gen_Arr_HTE_StringTable_remove_at(gen_Arr_HTE_StringTable self, gen_usize idx);
bool                 gen_Arr_HTE_StringTable_reserve(gen_Arr_HTE_StringTable* self, gen_usize new_capacity);
bool                 gen_Arr_HTE_StringTable_resize(gen_Arr_HTE_StringTable* self, gen_usize num);
bool                 gen_Arr_HTE_StringTable_set_capacity(gen_Arr_HTE_StringTable* self, gen_usize new_capacity);

gen_forceinline gen_Arr_HTE_StringTable gen_Arr_HTE_StringTable_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_HTE_StringTable, allocator, initial_size);
}

inline gen_Arr_HTE_StringTable gen_Arr_HTE_StringTable_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_HTE_StringTable) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_HTE_StringTable*, header + 1);
}

gen_forceinline bool gen_Arr_HTE_StringTable_append_array(gen_Arr_HTE_StringTable* self, gen_Arr_HTE_StringTable other)
{
	return gen_array_append_items(*self, (gen_Arr_HTE_StringTable)other, gen_Arr_HTE_StringTable_num(other));
}

inline bool gen_Arr_HTE_StringTable_append(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Arr_HTE_StringTable_append_items(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_HTE_StringTable) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Arr_HTE_StringTable_append_at(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Arr_HTE_StringTable target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_HTE_StringTable));
	header->Num++;
	return true;
}

inline bool gen_Arr_HTE_StringTable_append_items_at(gen_Arr_HTE_StringTable* self, gen_HTE_StringTable* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_HTE_StringTable* target = (*self) + idx + item_num;
	gen_HTE_StringTable* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_HTE_StringTable));
	gen_mem_copy(src, items, item_num * sizeof(gen_HTE_StringTable));
	header->Num += item_num;
	return true;
}

inline gen_HTE_StringTable* gen_Arr_HTE_StringTable_back(gen_Arr_HTE_StringTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Arr_HTE_StringTable_clear(gen_Arr_HTE_StringTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Arr_HTE_StringTable_fill(gen_Arr_HTE_StringTable self, gen_usize begin, gen_usize end, gen_HTE_StringTable value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Arr_HTE_StringTable_free(gen_Arr_HTE_StringTable* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Arr_HTE_StringTable_grow(gen_Arr_HTE_StringTable* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Arr_HTE_StringTable_num(gen_Arr_HTE_StringTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_HTE_StringTable gen_Arr_HTE_StringTable_pop(gen_Arr_HTE_StringTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_HTE_StringTable result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Arr_HTE_StringTable_remove_at(gen_Arr_HTE_StringTable self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_HTE_StringTable) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Arr_HTE_StringTable_reserve(gen_Arr_HTE_StringTable* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Arr_HTE_StringTable_resize(gen_Arr_HTE_StringTable* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Arr_HTE_StringTable_set_capacity(gen_Arr_HTE_StringTable* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_HTE_StringTable) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_HTE_StringTable) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_HTE_StringTable*, new_header + 1);
	return true;
}

#pragma endregion gen_Arr_HTE_StringTable

struct gen_HashTable_gen_Str
{
	gen_Array_gen_ssize     Hashes;
	gen_Arr_HTE_StringTable Entries;
};

gen_StringTable   gen_StringTable_init(gen_AllocatorInfo allocator);
gen_StringTable   gen_StringTable_init_reserve(gen_AllocatorInfo allocator, gen_ssize num);
void              gen_StringTable_clear(gen_StringTable self);
void              gen_StringTable_destroy(gen_StringTable* self);
gen_Str*          gen_StringTable_get(gen_StringTable self, gen_u64 key);
void              gen_StringTable_map(gen_StringTable self, gen_StringTable_MapProc map_proc);
void              gen_StringTable_map_mut(gen_StringTable self, gen_StringTable_MapMutProc map_proc);
void              gen_StringTable_grow(gen_StringTable* self);
void              gen_StringTable_rehash(gen_StringTable* self, gen_ssize new_num);
void              gen_StringTable_rehash_fast(gen_StringTable self);
void              gen_StringTable_remove(gen_StringTable self, gen_u64 key);
void              gen_StringTable_remove_entry(gen_StringTable self, gen_ssize idx);
void              gen_StringTable_set(gen_StringTable* self, gen_u64 key, gen_Str value);
gen_ssize         gen_StringTable_slot(gen_StringTable self, gen_u64 key);
gen_ssize         gen_StringTable__add_entry(gen_StringTable* self, gen_u64 key);
gen_HT_FindResult gen_StringTable__find(gen_StringTable self, gen_u64 key);
gen_b32           gen_StringTable__full(gen_StringTable self);

gen_StringTable gen_StringTable_init(gen_AllocatorInfo allocator)
{
	gen_StringTable result = gen_hashtable_init_reserve(gen_Str, allocator, 8);
	return result;
}

gen_StringTable gen_StringTable_init_reserve(gen_AllocatorInfo allocator, gen_ssize num)
{
	gen_StringTable result                   = { 0, 0 };
	result.Hashes                            = gen_array_init_reserve(gen_ssize, allocator, num);
	gen_array_get_header(result.Hashes)->Num = num;
	gen_array_resize(result.Hashes, num);
	gen_array_fill(result.Hashes, 0, num, -1);
	result.Entries = gen_array_init_reserve(gen_HTE_StringTable, allocator, num);
	return result;
}

void gen_StringTable_clear(gen_StringTable self)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_array_clear(self.Entries);
	gen_s32 what = gen_array_num(self.Hashes);
	gen_array_fill(self.Hashes, 0, what, (gen_ssize)-1);
}

void gen_StringTable_destroy(gen_StringTable* self)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	if (self->Hashes && gen_array_get_header(self->Hashes)->Capacity)
	{
		gen_array_free(self->Hashes);
		gen_array_free(self->Entries);
	}
}

gen_Str* gen_StringTable_get(gen_StringTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_ssize idx = gen_StringTable__find(self, key).EntryIndex;
	if (idx > 0)
		return &self.Entries[idx].Value;
	return gen_nullptr;
}

void gen_StringTable_map(gen_StringTable self, gen_StringTable_MapProc map_proc)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	GEN_ASSERT_NOT_NULL(map_proc);
	for (gen_ssize idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
	{
		map_proc(self, self.Entries[idx].Key, self.Entries[idx].Value);
	}
}

void gen_StringTable_map_mut(gen_StringTable self, gen_StringTable_MapMutProc map_proc)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	GEN_ASSERT_NOT_NULL(map_proc);
	for (gen_ssize idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
	{
		map_proc(self, self.Entries[idx].Key, &self.Entries[idx].Value);
	}
}

void gen_StringTable_grow(gen_StringTable* self)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	gen_ssize new_num = gen_array_grow_formula(gen_array_get_header(self->Entries)->Num);
	gen_hashtable_rehash(self, new_num);
}

void gen_StringTable_rehash(gen_StringTable* self, gen_ssize new_num)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	GEN_ASSERT(new_num > 0);
	gen_ssize        idx;
	gen_ssize        last_added_index;
	gen_ArrayHeader* old_hash_header    = gen_array_get_header(self->Hashes);
	gen_ArrayHeader* old_entries_header = gen_array_get_header(self->Entries);
	gen_StringTable  new_tbl            = gen_hashtable_init_reserve(gen_Str, old_hash_header->Allocator, old_hash_header->Num);
	gen_ArrayHeader* new_hash_header    = gen_array_get_header(new_tbl.Hashes);
	for (gen_ssize idx = 0; idx < gen_cast(gen_ssize, old_hash_header->Num); ++idx)
	{
		gen_HTE_StringTable* entry = &self->Entries[idx];
		gen_HT_FindResult    find_result;
		find_result      = gen_StringTable__find(new_tbl, entry->Key);
		last_added_index = gen_StringTable__add_entry(&new_tbl, entry->Key);
		if (find_result.PrevIndex < 0)
			new_tbl.Hashes[find_result.HashIndex] = last_added_index;
		else
			new_tbl.Entries[find_result.PrevIndex].Next = last_added_index;
		new_tbl.Entries[last_added_index].Next  = find_result.EntryIndex;
		new_tbl.Entries[last_added_index].Value = entry->Value;
	}
	gen_StringTable_destroy(self);
	*self = new_tbl;
}

void gen_StringTable_rehash_fast(gen_StringTable self)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_ssize idx;
	for (idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
		self.Entries[idx].Next = -1;
	for (idx = 0; idx < gen_array_get_header(self.Hashes)->Num; idx++)
		self.Hashes[idx] = -1;
	for (idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
	{
		gen_HTE_StringTable* entry;
		gen_HT_FindResult    find_result;
		entry       = &self.Entries[idx];
		find_result = gen_StringTable__find(self, entry->Key);
		if (find_result.PrevIndex < 0)
			self.Hashes[find_result.HashIndex] = idx;
		else
			self.Entries[find_result.PrevIndex].Next = idx;
	}
}

void gen_StringTable_remove(gen_StringTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_HT_FindResult find_result = gen_StringTable__find(self, key);
	if (find_result.EntryIndex >= 0)
	{
		gen_array_remove_at(self.Entries, find_result.EntryIndex);
		gen_hashtable_rehash_fast(self);
	}
}

void gen_StringTable_remove_entry(gen_StringTable self, gen_ssize idx)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_array_remove_at(self.Entries, idx);
}

void gen_StringTable_set(gen_StringTable* self, gen_u64 key, gen_Str value)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	gen_ssize         idx;
	gen_HT_FindResult find_result;
	if (gen_array_get_header(self->Hashes)->Num == 0)
		gen_hashtable_grow(self);
	find_result = gen_StringTable__find(*self, key);
	if (find_result.EntryIndex >= 0)
	{
		idx = find_result.EntryIndex;
	}
	else
	{
		idx = gen_StringTable__add_entry(self, key);
		if (find_result.PrevIndex >= 0)
		{
			self->Entries[find_result.PrevIndex].Next = idx;
		}
		else
		{
			self->Hashes[find_result.HashIndex] = idx;
		}
	}
	self->Entries[idx].Value = value;
	if (gen_StringTable__full(*self))
		gen_hashtable_grow(self);
}

gen_ssize gen_StringTable_slot(gen_StringTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	for (gen_ssize idx = 0; idx < gen_array_get_header(self.Hashes)->Num; ++idx)
		if (self.Hashes[idx] == key)
			return idx;
	return -1;
}

gen_ssize gen_StringTable__add_entry(gen_StringTable* self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	gen_ssize           idx;
	gen_HTE_StringTable entry = { key, -1 };
	idx                       = gen_array_get_header(self->Entries)->Num;
	gen_array_append(self->Entries, entry);
	return idx;
}

gen_HT_FindResult gen_StringTable__find(gen_StringTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_HT_FindResult result      = { -1, -1, -1 };
	gen_ArrayHeader*  hash_header = gen_array_get_header(self.Hashes);
	if (hash_header->Num > 0)
	{
		result.HashIndex  = key % hash_header->Num;
		result.EntryIndex = self.Hashes[result.HashIndex];
		while (result.EntryIndex >= 0)
		{
			if (self.Entries[result.EntryIndex].Key == key)
				break;
			result.PrevIndex  = result.EntryIndex;
			result.EntryIndex = self.Entries[result.EntryIndex].Next;
		}
	}
	return result;
}

gen_b32 gen_StringTable__full(gen_StringTable self)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_ArrayHeader* hash_header    = gen_array_get_header(self.Hashes);
	gen_ArrayHeader* entries_header = gen_array_get_header(self.Entries);
	gen_usize        critical_load  = gen_cast(gen_usize, gen_HashTable_CriticalLoadScale * gen_cast(gen_f32, hash_header->Num));
	gen_b32          result         = entries_header->Num > critical_load;
	return result;
}

#pragma endregion gen_StringTable

#pragma endregion Strings

#pragma region File Handling

enum gen_FileModeFlag
{
	EFileMode_READ   = gen_bit(0),
	EFileMode_WRITE  = gen_bit(1),
	EFileMode_APPEND = gen_bit(2),
	EFileMode_RW     = gen_bit(3),
	GEN_FILE_MODES   = EFileMode_READ | EFileMode_WRITE | EFileMode_APPEND | EFileMode_RW,
};
typedef enum gen_FileModeFlag gen_FileModeFlag;

// NOTE: Only used internally and for the file operations
enum gen_SeekWhenceType
{
	ESeekWhence_BEGIN   = 0,
	ESeekWhence_CURRENT = 1,
	ESeekWhence_END     = 2,
};
typedef enum gen_SeekWhenceType gen_SeekWhenceType;

enum gen_FileError
{
	EFileError_NONE,
	EFileError_INVALID,
	EFileError_INVALID_FILENAME,
	EFileError_EXISTS,
	EFileError_NOT_EXISTS,
	EFileError_PERMISSION,
	EFileError_TRUNCATION_FAILURE,
	EFileError_NOT_EMPTY,
	EFileError_NAME_TOO_LONG,
	EFileError_UNKNOWN,
};
typedef enum gen_FileError gen_FileError;

union gen_FileDescriptor
{
	void*    p;
	gen_sptr i;
	gen_uptr u;
};
typedef union gen_FileDescriptor gen_FileDescriptor;

typedef gen_u32                   gen_FileMode;
typedef struct gen_FileOperations gen_FileOperations;

#define GEN_FILE_OPEN_PROC(name) gen_FileError name(gen_FileDescriptor* fd, gen_FileOperations* ops, gen_FileMode mode, char const* filename)
#define GEN_FILE_READ_AT_PROC(name) \
	gen_b32 name(gen_FileDescriptor fd, void* buffer, gen_ssize size, gen_s64 offset, gen_ssize* bytes_read, gen_b32 stop_at_newline)
#define GEN_FILE_WRITE_AT_PROC(name) gen_b32 name(gen_FileDescriptor fd, gen_mem_ptr_const buffer, gen_ssize size, gen_s64 offset, gen_ssize* bytes_written)
#define GEN_FILE_SEEK_PROC(name)     gen_b32 name(gen_FileDescriptor fd, gen_s64 offset, gen_SeekWhenceType whence, gen_s64* new_offset)
#define GEN_FILE_CLOSE_PROC(name)    void name(gen_FileDescriptor fd)

typedef GEN_FILE_OPEN_PROC(gen_file_open_proc);
typedef GEN_FILE_READ_AT_PROC(FileReadProc);
typedef GEN_FILE_WRITE_AT_PROC(FileWriteProc);
typedef GEN_FILE_SEEK_PROC(FileSeekProc);
typedef GEN_FILE_CLOSE_PROC(FileCloseProc);

struct gen_FileOperations
{
	FileReadProc*  read_at;
	FileWriteProc* write_at;
	FileSeekProc*  seek;
	FileCloseProc* close;
};
typedef struct gen_FileOperations gen_FileOperations;

extern gen_FileOperations const default_file_operations;

typedef gen_u64 word;

enum gen_DirType
{
	GEN_DIR_TYPE_FILE,
	GEN_DIR_TYPE_FOLDER,
	GEN_DIR_TYPE_UNKNOWN,
};
typedef enum gen_DirType gen_DirType;

struct gen_DirInfo;
typedef struct gen_DirInfo gen_DirInfo;

struct gen_DirEntry
{
	char const*  filename;
	gen_DirInfo* dir_info;
	gen_u8       type;
};
typedef struct gen_DirEntry gen_DirEntry;

struct gen_DirInfo
{
	char const*   fullpath;
	gen_DirEntry* entries;    // zpl_array

	// Internals
	char**         filenames;    // zpl_array
	gen_StrBuilder buf;
};
typedef struct gen_DirInfo gen_DirInfo;

struct gen_FileInfo
{
	gen_FileOperations ops;
	gen_FileDescriptor fd;
	gen_b32            is_temp;

	char const*   filename;
	word          last_write_time;
	gen_DirEntry* dir;
};
typedef struct gen_FileInfo gen_FileInfo;

enum gen_FileStandardType
{
	EFileStandard_INPUT,
	EFileStandard_OUTPUT,
	EFileStandard_ERROR,
	EFileStandard_COUNT,
};
typedef enum gen_FileStandardType gen_FileStandardType;

/**
	* Get standard file I/O.
	* @param  std Check zpl_file_standard_type
	* @return     File handle to standard I/O
	*/
GEN_API gen_FileInfo* gen_file_get_standard(gen_FileStandardType std);

/**
	* Closes the file
	* @param  file
	*/
GEN_API gen_FileError gen_file_close(gen_FileInfo* file);

/**
	* Returns the currently opened file's name
	* @param  file
	*/
inline char const* gen_file_name(gen_FileInfo* file)
{
	return file->filename ? file->filename : "";
}

/**
	* Opens a file
	* @param  file
	* @param  filename
	*/
GEN_API gen_FileError gen_file_open(gen_FileInfo* file, char const* filename);

/**
	* Opens a file using a specified mode
	* @param  file
	* @param  mode     Access mode to use
	* @param  filename
	*/
GEN_API gen_FileError gen_file_open_mode(gen_FileInfo* file, gen_FileMode mode, char const* filename);

/**
	* Reads from a file
	* @param  file
	* @param  buffer Buffer to read to
	* @param  size   Size to read
	*/
gen_b32 gen_file_read(gen_FileInfo* file, void* buffer, gen_ssize size);

/**
	* Reads file at a specific offset
	* @param  file
	* @param  buffer     Buffer to read to
	* @param  size       Size to read
	* @param  offset     Offset to read from
	* @param  bytes_read How much data we've actually read
	*/
gen_b32 gen_file_read_at(gen_FileInfo* file, void* buffer, gen_ssize size, gen_s64 offset);

/**
	* Reads file safely
	* @param  file
	* @param  buffer     Buffer to read to
	* @param  size       Size to read
	* @param  offset     Offset to read from
	* @param  bytes_read How much data we've actually read
	*/
gen_b32 gen_file_read_at_check(gen_FileInfo* file, void* buffer, gen_ssize size, gen_s64 offset, gen_ssize* bytes_read);

typedef struct FileContents FileContents;

struct FileContents
{
	gen_AllocatorInfo allocator;
	void*             data;
	gen_ssize         size;
};
typedef struct FileContents FileContents;

#define gen_file_zero_terminate    true
#define gen_file_no_zero_terminate false

/**
	* Reads the whole file contents
	* @param  a              Allocator to use
	* @param  zero_terminate End the read data with null terminator
	* @param  filepath       Path to the file
	* @return                File contents data
	*/
GEN_API FileContents gen_file_read_contents(gen_AllocatorInfo a, gen_b32 zero_terminate, char const* filepath);

/**
	* Returns a size of the file
	* @param  file
	* @return      File size
	*/
GEN_API gen_s64 gen_file_size(gen_FileInfo* file);

/**
	* Seeks the file cursor from the beginning of file to a specific position
	* @param  file
	* @param  offset Offset to seek to
	*/
gen_s64 gen_file_seek(gen_FileInfo* file, gen_s64 offset);

/**
	* Seeks the file cursor to the end of the file
	* @param  file
	*/
gen_s64 gen_file_seek_to_end(gen_FileInfo* file);

/**
	* Returns the length from the beginning of the file we've read so far
	* @param  file
	* @return      Our current position in file
	*/
gen_s64 gen_file_tell(gen_FileInfo* file);

/**
	* Writes to a file
	* @param  file
	* @param  buffer Buffer to read from
	* @param  size   Size to read
	*/
gen_b32 gen_file_write(gen_FileInfo* file, void const* buffer, gen_ssize size);

/**
	* Writes to file at a specific offset
	* @param  file
	* @param  buffer        Buffer to read from
	* @param  size          Size to write
	* @param  offset        Offset to write to
	* @param  bytes_written How much data we've actually written
	*/
gen_b32 gen_file_write_at(gen_FileInfo* file, void const* buffer, gen_ssize size, gen_s64 offset);

/**
	* Writes to file safely
	* @param  file
	* @param  buffer        Buffer to read from
	* @param  size          Size to write
	* @param  offset        Offset to write to
	* @param  bytes_written How much data we've actually written
	*/
gen_b32 gen_file_write_at_check(gen_FileInfo* file, void const* buffer, gen_ssize size, gen_s64 offset, gen_ssize* bytes_written);

enum FileStreamFlags gen_enum_underlying(gen_u32)
{
	/* Allows us to write to the buffer directly. Beware: you can not append a new data! */
	EFileStream_WRITABLE = gen_bit(0), /* Clones the input buffer so you can write (zpl_file_write*) data into it. */
	/* Since we work with a clone, the buffer size can dynamically grow as well. */
	EFileStream_CLONE_WRITABLE = gen_bit(1),
	EFileStream_UNDERLYING     = GEN_U32_MAX,
};

typedef gen_u32 FileStreamFlags;

/**
	* Opens a new memory stream
	* @param file
	* @param allocator
	*/
GEN_API gen_b8 gen_file_stream_new(gen_FileInfo* file, gen_AllocatorInfo allocator);

/**
	* Opens a memory stream over an existing buffer
	* @param  file
	* @param  allocator
	* @param  buffer   Memory to create stream from
	* @param  size     Buffer's size
	* @param  flags
	*/
GEN_API gen_b8 gen_file_stream_open(gen_FileInfo* file, gen_AllocatorInfo allocator, gen_u8* buffer, gen_ssize size, FileStreamFlags flags);

/**
	* Retrieves the stream's underlying buffer and buffer size.
	* @param file memory stream
	* @param size (Optional) buffer size
	*/
GEN_API gen_u8* gen_file_stream_buf(gen_FileInfo* file, gen_ssize* size);

extern gen_FileOperations const memory_file_operations;

inline gen_s64 gen_file_seek(gen_FileInfo* f, gen_s64 offset)
{
	gen_s64 new_offset = 0;

	if (! f->ops.read_at)
		f->ops = default_file_operations;

	f->ops.seek(f->fd, offset, ESeekWhence_BEGIN, &new_offset);

	return new_offset;
}

inline gen_s64 gen_file_seek_to_end(gen_FileInfo* f)
{
	gen_s64 new_offset = 0;

	if (! f->ops.read_at)
		f->ops = default_file_operations;

	f->ops.seek(f->fd, 0, ESeekWhence_END, &new_offset);

	return new_offset;
}

inline gen_s64 gen_file_tell(gen_FileInfo* f)
{
	gen_s64 new_offset = 0;

	if (! f->ops.read_at)
		f->ops = default_file_operations;

	f->ops.seek(f->fd, 0, ESeekWhence_CURRENT, &new_offset);

	return new_offset;
}

inline gen_b32 gen_file_read(gen_FileInfo* f, void* buffer, gen_ssize size)
{
	gen_s64 cur_offset = gen_file_tell(f);
	gen_b32 result     = gen_file_read_at(f, buffer, size, gen_file_tell(f));
	gen_file_seek(f, cur_offset + size);
	return result;
}

inline gen_b32 gen_file_read_at(gen_FileInfo* f, void* buffer, gen_ssize size, gen_s64 offset)
{
	return gen_file_read_at_check(f, buffer, size, offset, NULL);
}

inline gen_b32 gen_file_read_at_check(gen_FileInfo* f, void* buffer, gen_ssize size, gen_s64 offset, gen_ssize* bytes_read)
{
	if (! f->ops.read_at)
		f->ops = default_file_operations;
	return f->ops.read_at(f->fd, buffer, size, offset, bytes_read, false);
}

inline gen_b32 gen_file_write(gen_FileInfo* f, void const* buffer, gen_ssize size)
{
	gen_s64 cur_offset = gen_file_tell(f);
	gen_b32 result     = gen_file_write_at(f, buffer, size, gen_file_tell(f));

	gen_file_seek(f, cur_offset + size);

	return result;
}

inline gen_b32 gen_file_write_at(gen_FileInfo* f, void const* buffer, gen_ssize size, gen_s64 offset)
{
	return gen_file_write_at_check(f, buffer, size, offset, NULL);
}

inline gen_b32 gen_file_write_at_check(gen_FileInfo* f, void const* buffer, gen_ssize size, gen_s64 offset, gen_ssize* bytes_written)
{
	if (! f->ops.read_at)
		f->ops = default_file_operations;

	return f->ops.write_at(f->fd, buffer, size, offset, bytes_written);
}

#pragma endregion File Handling

#pragma region Timing

#ifdef GEN_BENCHMARK
//! Return CPU timestamp.
GEN_API gen_u64 gen_read_cpu_time_stamp_counter( void );

//! Return relative time (in seconds) since the application start.
GEN_API gen_f64 gen_time_rel( void );

//! Return relative time since the application start.
GEN_API gen_u64 gen_time_rel_ms( void );
#endif

#pragma endregion Timing

#pragma region ADT

struct gen_ADT_Node;
typedef struct gen_ADT_Node gen_ADT_Node;
typedef gen_ADT_Node*       gen_Array_gen_ADT_Node;

enum gen_ADT_Type gen_enum_underlying(gen_u32)
{
	EADT_TYPE_UNINITIALISED, /* node was not initialised, this is a programming error! */
	EADT_TYPE_ARRAY,
	EADT_TYPE_OBJECT,
	EADT_TYPE_STRING,
	EADT_TYPE_MULTISTRING,
	EADT_TYPE_INTEGER,
	EADT_TYPE_REAL,
};

typedef gen_u32 gen_ADT_Type;

enum gen_ADT_Props gen_enum_underlying(gen_u32)
{
	EADT_PROPS_NONE,
	EADT_PROPS_NAN,
	EADT_PROPS_NAN_NEG,
	EADT_PROPS_INFINITY,
	EADT_PROPS_INFINITY_NEG,
	EADT_PROPS_FALSE,
	EADT_PROPS_TRUE,
	EADT_PROPS_NULL,
	EADT_PROPS_IS_EXP,
	EADT_PROPS_IS_HEX,    // Used internally so that people can fill in real numbers they plan to write.
	EADT_PROPS_IS_PARSED_REAL,
};

typedef gen_u32 gen_ADT_Props;

enum gen_ADT_NamingStyle gen_enum_underlying(gen_u32)
{
	EADT_NAME_STYLE_DOUBLE_QUOTE,
	EADT_NAME_STYLE_SINGLE_QUOTE,
	EADT_NAME_STYLE_NO_QUOTES,
};

typedef gen_u32 gen_ADT_NamingStyle;

enum gen_ADT_AssignStyle gen_enum_underlying(gen_u32)
{
	EADT_ASSIGN_STYLE_COLON,
	EADT_ASSIGN_STYLE_EQUALS,
	EADT_ASSIGN_STYLE_LINE,
};

typedef gen_u32 gen_ADT_AssignStyle;

enum gen_ADT_DelimStyle gen_enum_underlying(gen_u32)
{
	EADT_DELIM_STYLE_COMMA,
	EADT_DELIM_STYLE_LINE,
	EADT_DELIM_STYLE_NEWLINE,
};

typedef gen_u32 gen_ADT_DelimStyle;

enum gen_ADT_Error gen_enum_underlying(gen_u32)
{
	EADT_ERROR_NONE,
	EADT_ERROR_INTERNAL,
	EADT_ERROR_ALREADY_CONVERTED,
	EADT_ERROR_INVALID_TYPE,
	EADT_ERROR_OUT_OF_MEMORY,
};

typedef gen_u32 gen_ADT_Error;

struct gen_ADT_Node
{
	char const*          name;
	struct gen_ADT_Node* parent;

	/* properties */
	gen_ADT_Type type  : 4;
	gen_u8       props : 4;
#ifndef GEN_PARSER_DISABLE_ANALYSIS
	gen_u8 cfg_mode          : 1;
	gen_u8 name_style        : 2;
	gen_u8 assign_style      : 2;
	gen_u8 delim_style       : 2;
	gen_u8 delim_line_width  : 4;
	gen_u8 assign_line_width : 4;
#endif

	/* adt data */
	union
	{
		char const* string;
		gen_Array(gen_ADT_Node) nodes;    ///< zpl_array

		struct
		{
			union
			{
				gen_f64 real;
				gen_s64 integer;
			};

#ifndef GEN_PARSER_DISABLE_ANALYSIS
			/* number analysis */
			gen_s32 base;
			gen_s32 base2;
			gen_u8  base2_offset : 4;
			gen_s8  exp          : 4;
			gen_u8  neg_zero     : 1;
			gen_u8  lead_digit   : 1;
#endif
		};
	};
};

#pragma region gen_Array_gen_ADT_Node

#define GEN_GENERIC_SLOT_5__array_init            gen_ADT_Node, gen_Array_gen_ADT_Node_init
#define GEN_GENERIC_SLOT_5__array_init_reserve    gen_ADT_Node, gen_Array_gen_ADT_Node_init_reserve
#define GEN_GENERIC_SLOT_5__array_append          gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_append
#define GEN_GENERIC_SLOT_5__array_append_items    gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_append_items
#define GEN_GENERIC_SLOT_5__array_append_at       gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_append_at
#define GEN_GENERIC_SLOT_5__array_append_items_at gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_append_items_at
#define GEN_GENERIC_SLOT_5__array_back            gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_back
#define GEN_GENERIC_SLOT_5__array_clear           gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_clear
#define GEN_GENERIC_SLOT_5__array_fill            gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_fill
#define GEN_GENERIC_SLOT_5__array_free            gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_free
#define GEN_GENERIC_SLOT_5__array_grow            gen_Array_gen_ADT_Node*, gen_Array_gen_ADT_Node_grow
#define GEN_GENERIC_SLOT_5__array_num             gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_num
#define GEN_GENERIC_SLOT_5__array_pop             gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_pop
#define GEN_GENERIC_SLOT_5__array_remove_at       gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_remove_at
#define GEN_GENERIC_SLOT_5__array_reserve         gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_reserve
#define GEN_GENERIC_SLOT_5__array_resize          gen_Array_gen_ADT_Node, gen_Array_gen_ADT_Node_resize
#define GEN_GENERIC_SLOT_5__array_set_capacity    gen_Array_gen_ADT_Node*, gen_Array_gen_ADT_Node_set_capacity

gen_Array_gen_ADT_Node gen_Array_gen_ADT_Node_init(gen_AllocatorInfo allocator);
gen_Array_gen_ADT_Node gen_Array_gen_ADT_Node_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                   gen_Array_gen_ADT_Node_append_array(gen_Array_gen_ADT_Node* self, gen_Array_gen_ADT_Node other);
bool                   gen_Array_gen_ADT_Node_append(gen_Array_gen_ADT_Node* self, gen_ADT_Node value);
bool                   gen_Array_gen_ADT_Node_append_items(gen_Array_gen_ADT_Node* self, gen_ADT_Node* items, gen_usize item_num);
bool                   gen_Array_gen_ADT_Node_append_at(gen_Array_gen_ADT_Node* self, gen_ADT_Node item, gen_usize idx);
bool                   gen_Array_gen_ADT_Node_append_items_at(gen_Array_gen_ADT_Node* self, gen_ADT_Node* items, gen_usize item_num, gen_usize idx);
gen_ADT_Node*          gen_Array_gen_ADT_Node_back(gen_Array_gen_ADT_Node self);
void                   gen_Array_gen_ADT_Node_clear(gen_Array_gen_ADT_Node self);
bool                   gen_Array_gen_ADT_Node_fill(gen_Array_gen_ADT_Node self, gen_usize begin, gen_usize end, gen_ADT_Node value);
void                   gen_Array_gen_ADT_Node_free(gen_Array_gen_ADT_Node* self);
bool                   gen_Array_gen_ADT_Node_grow(gen_Array_gen_ADT_Node* self, gen_usize min_capacity);
gen_usize              gen_Array_gen_ADT_Node_num(gen_Array_gen_ADT_Node self);
gen_ADT_Node           gen_Array_gen_ADT_Node_pop(gen_Array_gen_ADT_Node self);
void                   gen_Array_gen_ADT_Node_remove_at(gen_Array_gen_ADT_Node self, gen_usize idx);
bool                   gen_Array_gen_ADT_Node_reserve(gen_Array_gen_ADT_Node* self, gen_usize new_capacity);
bool                   gen_Array_gen_ADT_Node_resize(gen_Array_gen_ADT_Node* self, gen_usize num);
bool                   gen_Array_gen_ADT_Node_set_capacity(gen_Array_gen_ADT_Node* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_ADT_Node gen_Array_gen_ADT_Node_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_ADT_Node, allocator, initial_size);
}

inline gen_Array_gen_ADT_Node gen_Array_gen_ADT_Node_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_ADT_Node) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_ADT_Node*, header + 1);
}

gen_forceinline bool gen_Array_gen_ADT_Node_append_array(gen_Array_gen_ADT_Node* self, gen_Array_gen_ADT_Node other)
{
	return gen_array_append_items(*self, (gen_Array_gen_ADT_Node)other, gen_Array_gen_ADT_Node_num(other));
}

inline bool gen_Array_gen_ADT_Node_append(gen_Array_gen_ADT_Node* self, gen_ADT_Node value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_ADT_Node_append_items(gen_Array_gen_ADT_Node* self, gen_ADT_Node* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_ADT_Node) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_ADT_Node_append_at(gen_Array_gen_ADT_Node* self, gen_ADT_Node item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_ADT_Node target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_ADT_Node));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_ADT_Node_append_items_at(gen_Array_gen_ADT_Node* self, gen_ADT_Node* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_ADT_Node* target = (*self) + idx + item_num;
	gen_ADT_Node* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_ADT_Node));
	gen_mem_copy(src, items, item_num * sizeof(gen_ADT_Node));
	header->Num += item_num;
	return true;
}

inline gen_ADT_Node* gen_Array_gen_ADT_Node_back(gen_Array_gen_ADT_Node self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_ADT_Node_clear(gen_Array_gen_ADT_Node self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_ADT_Node_fill(gen_Array_gen_ADT_Node self, gen_usize begin, gen_usize end, gen_ADT_Node value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_ADT_Node_free(gen_Array_gen_ADT_Node* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_ADT_Node_grow(gen_Array_gen_ADT_Node* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_ADT_Node_num(gen_Array_gen_ADT_Node self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_ADT_Node gen_Array_gen_ADT_Node_pop(gen_Array_gen_ADT_Node self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_ADT_Node result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_ADT_Node_remove_at(gen_Array_gen_ADT_Node self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_ADT_Node) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_ADT_Node_reserve(gen_Array_gen_ADT_Node* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_ADT_Node_resize(gen_Array_gen_ADT_Node* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_ADT_Node_set_capacity(gen_Array_gen_ADT_Node* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_ADT_Node) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_ADT_Node) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_ADT_Node*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_ADT_Node



/* ADT NODE LIMITS
	* delimiter and assignment segment width is limited to 128 whitespace symbols each.
	* real number limits decimal position to 128 places.
	* real number exponent is limited to 64 digits.
	*/

/**
	* @brief Initialise an ADT object or array
	*
	* @param node
	* @param backing Memory allocator used for descendants
	* @param name Node's name
	* @param is_array
	* @return error code
	*/
GEN_API gen_u8 gen_adt_make_branch(gen_ADT_Node* node, gen_AllocatorInfo backing, char const* name, gen_b32 is_array);

/**
	* @brief Destroy an ADT branch and its descendants
	*
	* @param node
	* @return error code
	*/
GEN_API gen_u8 gen_adt_destroy_branch(gen_ADT_Node* node);

/**
	* @brief Initialise an ADT leaf
	*
	* @param node
	* @param name Node's name
	* @param type Node's type (use zpl_adt_make_branch for container nodes)
	* @return error code
	*/
GEN_API gen_u8 gen_adt_make_leaf(gen_ADT_Node* node, char const* name, gen_ADT_Type type);


/**
	* @brief Fetch a node using provided URI string.
	*
	* This method uses a basic syntax to fetch a node from the ADT. The following features are available
	* to retrieve the data:
	*
	* - "a/b/c" navigates through objects "a" and "b" to get to "c"
	* - "arr/[foo=123]/bar" iterates over "arr" to find any object with param "foo" that matches the value "123", then gets its field called "bar"
	* - "arr/3" retrieves the 4th element in "arr"
	* - "arr/[apple]" retrieves the first element of value "apple" in "arr"
	*
	* @param node ADT node
	* @param uri Locator string as described above
	* @return zpl_adt_node*
	*
	* @see code/apps/examples/json_get.c
	*/
GEN_API gen_ADT_Node* gen_adt_query(gen_ADT_Node* node, char const* uri);

/**
	* @brief Find a field node within an object by the given name.
	*
	* @param node
	* @param name
	* @param deep_search Perform search recursively
	* @return zpl_adt_node * node
	*/
GEN_API gen_ADT_Node* gen_adt_find(gen_ADT_Node* node, char const* name, gen_b32 deep_search);

/**
	* @brief Allocate an unitialised node within a container at a specified index.
	*
	* @param parent
	* @param index
	* @return zpl_adt_node * node
	*/
GEN_API gen_ADT_Node* gen_adt_alloc_at(gen_ADT_Node* parent, gen_ssize index);

/**
	* @brief Allocate an unitialised node within a container.
	*
	* @param parent
	* @return zpl_adt_node * node
	*/
GEN_API gen_ADT_Node* gen_adt_alloc(gen_ADT_Node* parent);

/**
	* @brief Move an existing node to a new container at a specified index.
	*
	* @param node
	* @param new_parent
	* @param index
	* @return zpl_adt_node * node
	*/
GEN_API gen_ADT_Node* gen_adt_move_node_at(gen_ADT_Node* node, gen_ADT_Node* new_parent, gen_ssize index);

/**
	* @brief Move an existing node to a new container.
	*
	* @param node
	* @param new_parent
	* @return zpl_adt_node * node
	*/
GEN_API gen_ADT_Node* gen_adt_move_node(gen_ADT_Node* node, gen_ADT_Node* new_parent);

/**
	* @brief Swap two nodes.
	*
	* @param node
	* @param other_node
	* @return
	*/
GEN_API void gen_adt_swap_nodes(gen_ADT_Node* node, gen_ADT_Node* other_node);

/**
	* @brief Remove node from container.
	*
	* @param node
	* @return
	*/
GEN_API void gen_adt_remove_node(gen_ADT_Node* node);

/**
	* @brief Initialise a node as an object
	*
	* @param obj
	* @param name
	* @param backing
	* @return
	*/
GEN_API gen_b8 gen_adt_set_obj(gen_ADT_Node* obj, char const* name, gen_AllocatorInfo backing);

/**
	* @brief Initialise a node as an array
	*
	* @param obj
	* @param name
	* @param backing
	* @return
	*/
GEN_API gen_b8 gen_adt_set_arr(gen_ADT_Node* obj, char const* name, gen_AllocatorInfo backing);

/**
	* @brief Initialise a node as a string
	*
	* @param obj
	* @param name
	* @param value
	* @return
	*/
GEN_API gen_b8 gen_adt_set_str(gen_ADT_Node* obj, char const* name, char const* value);

/**
	* @brief Initialise a node as a float
	*
	* @param obj
	* @param name
	* @param value
	* @return
	*/
GEN_API gen_b8 gen_adt_set_flt(gen_ADT_Node* obj, char const* name, gen_f64 value);

/**
	* @brief Initialise a node as a signed integer
	*
	* @param obj
	* @param name
	* @param value
	* @return
	*/
GEN_API gen_b8 gen_adt_set_int(gen_ADT_Node* obj, char const* name, gen_s64 value);

/**
	* @brief Append a new node to a container as an object
	*
	* @param parent
	* @param name
	* @return*
	*/
GEN_API gen_ADT_Node* gen_adt_append_obj(gen_ADT_Node* parent, char const* name);

/**
	* @brief Append a new node to a container as an array
	*
	* @param parent
	* @param name
	* @return*
	*/
GEN_API gen_ADT_Node* gen_adt_append_arr(gen_ADT_Node* parent, char const* name);

/**
	* @brief Append a new node to a container as a string
	*
	* @param parent
	* @param name
	* @param value
	* @return*
	*/
GEN_API gen_ADT_Node* gen_adt_append_str(gen_ADT_Node* parent, char const* name, char const* value);

/**
	* @brief Append a new node to a container as a float
	*
	* @param parent
	* @param name
	* @param value
	* @return*
	*/
GEN_API gen_ADT_Node* gen_adt_append_flt(gen_ADT_Node* parent, char const* name, gen_f64 value);

/**
	* @brief Append a new node to a container as a signed integer
	*
	* @param parent
	* @param name
	* @param value
	* @return*
	*/
GEN_API gen_ADT_Node* gen_adt_append_int(gen_ADT_Node* parent, char const* name, gen_s64 value);

/* parser helpers */

/**
	* @brief Parses a text and stores the result into an unitialised node.
	*
	* @param node
	* @param base
	* @return*
	*/
GEN_API char* gen_adt_parse_number(gen_ADT_Node* node, char* base);

/**
	* @brief Parses a text and stores the result into an unitialised node.
	* This function expects the entire input to be a number.
	*
	* @param node
	* @param base
	* @return*
	*/
GEN_API char* gen_adt_parse_number_strict(gen_ADT_Node* node, char* base_str);

/**
	* @brief Parses and converts an existing string node into a number.
	*
	* @param node
	* @return
	*/
GEN_API gen_ADT_Error gen_adt_c_str_to_number(gen_ADT_Node* node);

/**
	* @brief Parses and converts an existing string node into a number.
	* This function expects the entire input to be a number.
	*
	* @param node
	* @return
	*/
GEN_API gen_ADT_Error gen_adt_c_str_to_number_strict(gen_ADT_Node* node);

/**
	* @brief Prints a number into a file stream.
	*
	* The provided file handle can also be a memory mapped stream.
	*
	* @see zpl_file_stream_new
	* @param file
	* @param node
	* @return
	*/
GEN_API gen_ADT_Error gen_adt_print_number(gen_FileInfo* file, gen_ADT_Node* node);

/**
	* @brief Prints a string into a file stream.
	*
	* The provided file handle can also be a memory mapped stream.
	*
	* @see zpl_file_stream_new
	* @param file
	* @param node
	* @param escaped_chars
	* @param escape_symbol
	* @return
	*/
GEN_API gen_ADT_Error gen_adt_print_string(gen_FileInfo* file, gen_ADT_Node* node, char const* escaped_chars, char const* escape_symbol);

#pragma endregion ADT

struct gen_ADT_Node;
typedef struct gen_ADT_Node gen_ADT_Node;

enum gen_CSV_Error gen_enum_underlying(gen_u32)
{
	ECSV_Error__NONE,
	ECSV_Error__INTERNAL,
	ECSV_Error__UNEXPECTED_END_OF_INPUT,
	ECSV_Error__MISMATCHED_ROWS,
};

typedef gen_u32 gen_CSV_Error;

typedef gen_ADT_Node gen_CSV_Object;

gen_u8         gen_csv_parse(gen_CSV_Object* root, char* text, gen_AllocatorInfo allocator, gen_b32 has_header);
GEN_API gen_u8 gen_csv_parse_delimiter(gen_CSV_Object* root, char* text, gen_AllocatorInfo allocator, gen_b32 has_header, char delim);
void           gen_csv_free(gen_CSV_Object* obj);

void                   gen_csv_write(gen_FileInfo* file, gen_CSV_Object* obj);
gen_StrBuilder         gen_csv_write_string(gen_AllocatorInfo a, gen_CSV_Object* obj);
GEN_API void           gen_csv_write_delimiter(gen_FileInfo* file, gen_CSV_Object* obj, char delim);
GEN_API gen_StrBuilder gen_csv_write_strbuilder_delimiter(gen_AllocatorInfo a, gen_CSV_Object* obj, char delim);

/* inline */

inline gen_u8 gen_csv_parse(gen_CSV_Object* root, char* text, gen_AllocatorInfo allocator, gen_b32 has_header)
{
	return gen_csv_parse_delimiter(root, text, allocator, has_header, ',');
}

inline void gen_csv_write(gen_FileInfo* file, gen_CSV_Object* obj)
{
	gen_csv_write_delimiter(file, obj, ',');
}

inline gen_StrBuilder gen_csv_write_string(gen_AllocatorInfo a, gen_CSV_Object* obj)
{
	return gen_csv_write_strbuilder_delimiter(a, obj, ',');
}

GEN_API_C_END
GEN_NS_END

// GEN_ROLL_OWN_DEPENDENCIES
#endif

GEN_NS_BEGIN
GEN_API_C_BEGIN

#pragma region Types

/*
 ________                                              __    __      ________
|        \                                            |  \  |  \    |        \
| ▓▓▓▓▓▓▓▓_______  __    __ ______ ____   _______     | ▓▓\ | ▓▓     \▓▓▓▓▓▓▓▓__    __  ______   ______   _______
| ▓▓__   |       \|  \  |  \      \    \ /       \    | ▓▓▓\| ▓▓       | ▓▓  |  \  |  \/      \ /      \ /       \
| ▓▓  \  | ▓▓▓▓▓▓▓\ ▓▓  | ▓▓ ▓▓▓▓▓▓\▓▓▓▓\  ▓▓▓▓▓▓▓    | ▓▓▓▓\ ▓▓       | ▓▓  | ▓▓  | ▓▓  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓
| ▓▓▓▓▓  | ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓ | ▓▓ | ▓▓\▓▓    \     | ▓▓\▓▓ ▓▓       | ▓▓  | ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓\▓▓    \
| ▓▓_____| ▓▓  | ▓▓ ▓▓__/ ▓▓ ▓▓ | ▓▓ | ▓▓_\▓▓▓▓▓▓\    | ▓▓ \▓▓▓▓       | ▓▓  | ▓▓__/ ▓▓ ▓▓__/ ▓▓ ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\
| ▓▓     \ ▓▓  | ▓▓\▓▓    ▓▓ ▓▓ | ▓▓ | ▓▓       ▓▓    | ▓▓  \▓▓▓       | ▓▓   \▓▓    ▓▓ ▓▓    ▓▓\▓▓     \       ▓▓
 \▓▓▓▓▓▓▓▓\▓▓   \▓▓ \▓▓▓▓▓▓ \▓▓  \▓▓  \▓▓\▓▓▓▓▓▓▓      \▓▓   \▓▓        \▓▓   _\▓▓▓▓▓▓▓ ▓▓▓▓▓▓▓  \▓▓▓▓▓▓▓\▓▓▓▓▓▓▓
                                                                             |  \__| ▓▓ ▓▓
                                                                              \▓▓    ▓▓ ▓▓
                                                                               \▓▓▓▓▓▓ \▓▓

*/

typedef gen_ssize (*LogFailType)(char const*, ...);

// By default this library will either crash or exit if an error is detected while generating codes.
// Even if set to not use GEN_FATAL, GEN_FATAL will still be used for memory failures as the library is unusable when they occur.
#ifdef GEN_DONT_USE_FATAL

#define gen_log_failure gen_log_fmt
#else
#define gen_log_failure GEN_FATAL
#endif

enum gen_AccessSpec gen_enum_underlying(gen_u32)
{
	AccessSpec_Default,
	AccessSpec_Private,
	AccessSpec_Protected,
	AccessSpec_Public,
	AccessSpec_Num_AccessSpec,
	AccessSpec_Invalid,
	AccessSpec_SizeDef = GEN_U32_MAX,
};

typedef gen_u32 gen_AccessSpec;
gen_static_assert(gen_size_of(gen_AccessSpec) == gen_size_of(gen_u32), "gen_AccessSpec not gen_u32 size");

inline gen_Str gen_access_spec_to_str(gen_AccessSpec type)
{
	gen_local_persist gen_Str lookup[(gen_u32)AccessSpec_Num_AccessSpec] = {
		{ "",        sizeof("") - 1          },
		{ "private", sizeof("prviate") - 1   },
		{ "private", sizeof("protected") - 1 },
		{ "public",  sizeof("public") - 1    },
	};

	gen_Str invalid = { "Invalid", sizeof("Invalid") - 1 };
	if (type > AccessSpec_Public)
		return invalid;

	return lookup[(gen_u32)type];
}

enum gen_CodeFlag gen_enum_underlying(gen_u32)
{
	CodeFlag_None          = 0,
	CodeFlag_FunctionType  = gen_bit(0),
	CodeFlag_ParamPack     = gen_bit(1),
	CodeFlag_Module_Export = gen_bit(2),
	CodeFlag_Module_Import = gen_bit(3),
	CodeFlag_SizeDef       = GEN_U32_MAX,
};

typedef gen_u32 gen_CodeFlag;
gen_static_assert(gen_size_of(gen_CodeFlag) == gen_size_of(gen_u32), "gen_CodeFlag not gen_u32 size");

// Used to indicate if enum definitoin is an enum class or regular enum.
enum gen_EnumDecl gen_enum_underlying(gen_u8)
{
	EnumDecl_Regular,
	EnumDecl_Class,
	EnumT_SizeDef = GEN_U8_MAX,
};

typedef gen_u8 gen_EnumDecl;
typedef gen_u8 EnumT;

enum gen_ModuleFlag gen_enum_underlying(gen_u32)
{
	ModuleFlag_None   = 0,
	ModuleFlag_Export = gen_bit(0),
	ModuleFlag_Import = gen_bit(1),
	Num_ModuleFlags,
	ModuleFlag_Invalid,
	ModuleFlag_SizeDef = GEN_U32_MAX,
};

typedef gen_u32 gen_ModuleFlag;
gen_static_assert(gen_size_of(gen_ModuleFlag) == gen_size_of(gen_u32), "gen_ModuleFlag not gen_u32 size");

inline gen_Str gen_module_flag_to_str(gen_ModuleFlag flag)
{
	gen_local_persist gen_Str lookup[(gen_u32)Num_ModuleFlags] = {
		{ "__none__", sizeof("__none__") - 1 },
		{ "export",   sizeof("export") - 1   },
		{ "import",   sizeof("import") - 1   },
	};

	gen_local_persist gen_Str invalid_flag = { "invalid", sizeof("invalid") };
	if (flag > ModuleFlag_Import)
		return invalid_flag;

	return lookup[(gen_u32)flag];
}

enum gen_EPreprocessCOnd gen_enum_underlying(gen_u32)
{
	PreprocessCond_If,
	PreprocessCond_IfDef,
	PreprocessCond_IfNotDef,
	PreprocessCond_ElIf,
	EPreprocessCond_SizeDef = GEN_U32_MAX,
};

typedef gen_u32 gen_EPreprocessCOnd;
gen_static_assert(gen_size_of(gen_EPreprocessCOnd) == gen_size_of(gen_u32), "gen_EPreprocessCOnd not gen_u32 size");

enum gen_ETypenameTag gen_enum_underlying(gen_u16)
{
	Tag_None,
	Tag_Class,
	Tag_Enum,
	Tag_Struct,
	Tag_Union,
	Tag_UnderlyingType = GEN_U16_MAX,
};

typedef gen_u16 gen_ETypenameTag;
gen_static_assert(gen_size_of(gen_ETypenameTag) == gen_size_of(gen_u16), "gen_ETypenameTag is not gen_u16 size");

enum gen_CodeType gen_enum_underlying(gen_u32)
{
	CT_Invalid,
	CT_Untyped,
	CT_NewLine,
	CT_Comment,
	CT_Access_Private,
	CT_Access_Protected,
	CT_Access_Public,
	CT_PlatformAttributes,
	CT_Class,
	CT_Class_Fwd,
	CT_Class_Body,
	CT_Constructor,
	CT_Constructor_Fwd,
	CT_Destructor,
	CT_Destructor_Fwd,
	CT_Enum,
	CT_Enum_Fwd,
	CT_Enum_Body,
	CT_Enum_Class,
	CT_Enum_Class_Fwd,
	CT_Execution,
	CT_Export_Body,
	CT_Extern_Linkage,
	CT_Extern_Linkage_Body,
	CT_Friend,
	CT_Function,
	CT_Function_Fwd,
	CT_Function_Body,
	CT_Global_Body,
	CT_Module,
	CT_Namespace,
	CT_Namespace_Body,
	CT_Operator,
	CT_Operator_Fwd,
	CT_Operator_Member,
	CT_Operator_Member_Fwd,
	CT_Operator_Cast,
	CT_Operator_Cast_Fwd,
	CT_Parameters,
	CT_Parameters_Define,
	CT_Preprocess_Define,
	CT_Preprocess_Include,
	CT_Preprocess_If,
	CT_Preprocess_IfDef,
	CT_Preprocess_IfNotDef,
	CT_Preprocess_ElIf,
	CT_Preprocess_Else,
	CT_Preprocess_EndIf,
	CT_Preprocess_Pragma,
	CT_Specifiers,
	CT_Struct,
	CT_Struct_Fwd,
	CT_Struct_Body,
	CT_Template,
	CT_Typedef,
	CT_Typename,
	CT_Union,
	CT_Union_Fwd,
	CT_Union_Body,
	CT_Using,
	CT_Using_Namespace,
	CT_Variable,
	CT_NumTypes,
	CT_UnderlyingType = GEN_U32_MAX
};
typedef enum gen_CodeType gen_CodeType;

inline gen_Str gen_codetype_to_str(gen_CodeType type)
{
	gen_local_persist gen_Str lookup[] = {
		{ "Invalid",             sizeof("Invalid") - 1             },
		{ "Untyped",             sizeof("Untyped") - 1             },
		{ "NewLine",             sizeof("NewLine") - 1             },
		{ "Comment",             sizeof("Comment") - 1             },
		{ "Access_Private",      sizeof("Access_Private") - 1      },
		{ "Access_Protected",    sizeof("Access_Protected") - 1    },
		{ "Access_Public",       sizeof("Access_Public") - 1       },
		{ "PlatformAttributes",  sizeof("PlatformAttributes") - 1  },
		{ "Class",               sizeof("Class") - 1               },
		{ "Class_Fwd",           sizeof("Class_Fwd") - 1           },
		{ "Class_Body",          sizeof("Class_Body") - 1          },
		{ "Constructor",         sizeof("Constructor") - 1         },
		{ "Constructor_Fwd",     sizeof("Constructor_Fwd") - 1     },
		{ "Destructor",          sizeof("Destructor") - 1          },
		{ "Destructor_Fwd",      sizeof("Destructor_Fwd") - 1      },
		{ "Enum",                sizeof("Enum") - 1                },
		{ "Enum_Fwd",            sizeof("Enum_Fwd") - 1            },
		{ "Enum_Body",           sizeof("Enum_Body") - 1           },
		{ "Enum_Class",          sizeof("Enum_Class") - 1          },
		{ "Enum_Class_Fwd",      sizeof("Enum_Class_Fwd") - 1      },
		{ "Execution",           sizeof("Execution") - 1           },
		{ "Export_Body",         sizeof("Export_Body") - 1         },
		{ "Extern_Linkage",      sizeof("Extern_Linkage") - 1      },
		{ "Extern_Linkage_Body", sizeof("Extern_Linkage_Body") - 1 },
		{ "Friend",              sizeof("Friend") - 1              },
		{ "Function",            sizeof("Function") - 1            },
		{ "Function_Fwd",        sizeof("Function_Fwd") - 1        },
		{ "Function_Body",       sizeof("Function_Body") - 1       },
		{ "Global_Body",         sizeof("Global_Body") - 1         },
		{ "Module",              sizeof("Module") - 1              },
		{ "Namespace",           sizeof("Namespace") - 1           },
		{ "Namespace_Body",      sizeof("Namespace_Body") - 1      },
		{ "gen_Operator",        sizeof("gen_Operator") - 1        },
		{ "Operator_Fwd",        sizeof("Operator_Fwd") - 1        },
		{ "Operator_Member",     sizeof("Operator_Member") - 1     },
		{ "Operator_Member_Fwd", sizeof("Operator_Member_Fwd") - 1 },
		{ "Operator_Cast",       sizeof("Operator_Cast") - 1       },
		{ "Operator_Cast_Fwd",   sizeof("Operator_Cast_Fwd") - 1   },
		{ "Parameters",          sizeof("Parameters") - 1          },
		{ "Parameters_Define",   sizeof("Parameters_Define") - 1   },
		{ "Preprocess_Define",   sizeof("Preprocess_Define") - 1   },
		{ "Preprocess_Include",  sizeof("Preprocess_Include") - 1  },
		{ "Preprocess_If",       sizeof("Preprocess_If") - 1       },
		{ "Preprocess_IfDef",    sizeof("Preprocess_IfDef") - 1    },
		{ "Preprocess_IfNotDef", sizeof("Preprocess_IfNotDef") - 1 },
		{ "Preprocess_ElIf",     sizeof("Preprocess_ElIf") - 1     },
		{ "Preprocess_Else",     sizeof("Preprocess_Else") - 1     },
		{ "Preprocess_EndIf",    sizeof("Preprocess_EndIf") - 1    },
		{ "Preprocess_Pragma",   sizeof("Preprocess_Pragma") - 1   },
		{ "Specifiers",          sizeof("Specifiers") - 1          },
		{ "Struct",              sizeof("Struct") - 1              },
		{ "Struct_Fwd",          sizeof("Struct_Fwd") - 1          },
		{ "Struct_Body",         sizeof("Struct_Body") - 1         },
		{ "Template",            sizeof("Template") - 1            },
		{ "Typedef",             sizeof("Typedef") - 1             },
		{ "Typename",            sizeof("Typename") - 1            },
		{ "Union",               sizeof("Union") - 1               },
		{ "Union_Fwd",           sizeof("Union_Fwd") - 1           },
		{ "Union_Body",          sizeof("Union_Body") - 1          },
		{ "Using",               sizeof("Using") - 1               },
		{ "Using_Namespace",     sizeof("Using_Namespace") - 1     },
		{ "Variable",            sizeof("Variable") - 1            },
	};
	return lookup[type];
}

inline gen_Str gen_codetype_to_keyword_str(gen_CodeType type)
{
	gen_local_persist gen_Str lookup[] = {
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "//",              sizeof("//") - 1              },
		{ "private",         sizeof("private") - 1         },
		{ "protected",       sizeof("protected") - 1       },
		{ "public",          sizeof("public") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "class",           sizeof("class") - 1           },
		{ "clsss",           sizeof("clsss") - 1           },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "enum",            sizeof("enum") - 1            },
		{ "enum",            sizeof("enum") - 1            },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "enum class",      sizeof("enum class") - 1      },
		{ "enum class",      sizeof("enum class") - 1      },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "extern",          sizeof("extern") - 1          },
		{ "extern",          sizeof("extern") - 1          },
		{ "friend",          sizeof("friend") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "module",          sizeof("module") - 1          },
		{ "namespace",       sizeof("namespace") - 1       },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "operator",        sizeof("operator") - 1        },
		{ "operator",        sizeof("operator") - 1        },
		{ "operator",        sizeof("operator") - 1        },
		{ "operator",        sizeof("operator") - 1        },
		{ "operator",        sizeof("operator") - 1        },
		{ "operator",        sizeof("operator") - 1        },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "define",          sizeof("define") - 1          },
		{ "include",         sizeof("include") - 1         },
		{ "if",              sizeof("if") - 1              },
		{ "ifdef",           sizeof("ifdef") - 1           },
		{ "ifndef",          sizeof("ifndef") - 1          },
		{ "elif",            sizeof("elif") - 1            },
		{ "else",            sizeof("else") - 1            },
		{ "endif",           sizeof("endif") - 1           },
		{ "pragma",          sizeof("pragma") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "struct",          sizeof("struct") - 1          },
		{ "struct",          sizeof("struct") - 1          },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "template",        sizeof("template") - 1        },
		{ "typedef",         sizeof("typedef") - 1         },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "union",           sizeof("union") - 1           },
		{ "union",           sizeof("union") - 1           },
		{ "__NA__",          sizeof("__NA__") - 1          },
		{ "using",           sizeof("using") - 1           },
		{ "using namespace", sizeof("using namespace") - 1 },
		{ "__NA__",          sizeof("__NA__") - 1          },
	};
	return lookup[type];
}

enum gen_Operator gen_enum_underlying(gen_u32)
{
	Op_Invalid,
	Op_Assign,
	Op_Assign_Add,
	Op_Assign_Subtract,
	Op_Assign_Multiply,
	Op_Assign_Divide,
	Op_Assign_Modulo,
	Op_Assign_BAnd,
	Op_Assign_BOr,
	Op_Assign_BXOr,
	Op_Assign_LShift,
	Op_Assign_RShift,
	Op_Increment,
	Op_Decrement,
	Op_Unary_Plus,
	Op_Unary_Minus,
	Op_UnaryNot,
	Op_Add,
	Op_Subtract,
	Op_Multiply,
	Op_Divide,
	Op_Modulo,
	Op_BNot,
	Op_BAnd,
	Op_BOr,
	Op_BXOr,
	Op_LShift,
	Op_RShift,
	Op_LAnd,
	Op_LOr,
	Op_LEqual,
	Op_LNot,
	Op_Lesser,
	Op_Greater,
	Op_LesserEqual,
	Op_GreaterEqual,
	Op_Subscript,
	Op_Indirection,
	Op_AddressOf,
	Op_MemberOfPointer,
	Op_PtrToMemOfPtr,
	Op_FunctionCall,
	Op_Comma,
	Op_New,
	Op_NewArray,
	Op_Delete,
	Op_DeleteArray,
	Op_NumOps,
	Op_UnderlyingType = 0xffffffffu
};
typedef enum gen_Operator gen_Operator;

inline gen_Str gen_operator_to_str(gen_Operator op)
{
	gen_local_persist gen_Str lookup[] = {
		{ "INVALID",  sizeof("INVALID") - 1  },
		{ "=",        sizeof("=") - 1        },
		{ "+=",       sizeof("+=") - 1       },
		{ "-=",       sizeof("-=") - 1       },
		{ "*=",       sizeof("*=") - 1       },
		{ "/=",       sizeof("/=") - 1       },
		{ "%=",       sizeof("%=") - 1       },
		{ "&=",       sizeof("&=") - 1       },
		{ "|=",       sizeof("|=") - 1       },
		{ "^=",       sizeof("^=") - 1       },
		{ "<<=",      sizeof("<<=") - 1      },
		{ ">>=",      sizeof(">>=") - 1      },
		{ "++",       sizeof("++") - 1       },
		{ "--",       sizeof("--") - 1       },
		{ "+",        sizeof("+") - 1        },
		{ "-",        sizeof("-") - 1        },
		{ "!",        sizeof("!") - 1        },
		{ "+",        sizeof("+") - 1        },
		{ "-",        sizeof("-") - 1        },
		{ "*",        sizeof("*") - 1        },
		{ "/",        sizeof("/") - 1        },
		{ "%",        sizeof("%") - 1        },
		{ "~",        sizeof("~") - 1        },
		{ "&",        sizeof("&") - 1        },
		{ "|",        sizeof("|") - 1        },
		{ "^",        sizeof("^") - 1        },
		{ "<<",       sizeof("<<") - 1       },
		{ ">>",       sizeof(">>") - 1       },
		{ "&&",       sizeof("&&") - 1       },
		{ "||",       sizeof("||") - 1       },
		{ "==",       sizeof("==") - 1       },
		{ "!=",       sizeof("!=") - 1       },
		{ "<",        sizeof("<") - 1        },
		{ ">",        sizeof(">") - 1        },
		{ "<=",       sizeof("<=") - 1       },
		{ ">=",       sizeof(">=") - 1       },
		{ "[]",       sizeof("[]") - 1       },
		{ "*",        sizeof("*") - 1        },
		{ "&",        sizeof("&") - 1        },
		{ "->",       sizeof("->") - 1       },
		{ "->*",      sizeof("->*") - 1      },
		{ "()",       sizeof("()") - 1       },
		{ ",",        sizeof(",") - 1        },
		{ "new",      sizeof("new") - 1      },
		{ "new[]",    sizeof("new[]") - 1    },
		{ "delete",   sizeof("delete") - 1   },
		{ "delete[]", sizeof("delete[]") - 1 },
	};
	return lookup[op];
}

enum gen_Specifier gen_enum_underlying(gen_u32)
{
	Spec_Invalid,
	Spec_Consteval,
	Spec_Constexpr,
	Spec_Constinit,
	Spec_Explicit,
	Spec_External_Linkage,
	Spec_ForceInline,
	Spec_Global,
	Spec_Inline,
	Spec_Internal_Linkage,
	Spec_Local_Persist,
	Spec_Mutable,
	Spec_NeverInline,
	Spec_Ptr,
	Spec_Ref,
	Spec_Register,
	Spec_Restrict,
	Spec_RValue,
	Spec_Static,
	Spec_Thread_Local,
	Spec_Virtual,
	Spec_Const,
	Spec_Final,
	Spec_NoExceptions,
	Spec_Override,
	Spec_Pure,
	Spec_Delete,
	Spec_Volatile,
	Spec_NumSpecifiers,
	Spec_UnderlyingType = 0xffffffffu
};
typedef enum gen_Specifier gen_Specifier;

inline gen_Str gen_spec_to_str(gen_Specifier type)
{
	gen_local_persist gen_Str lookup[] = {
		{ "INVALID",           sizeof("INVALID") - 1           },
		{ "consteval",         sizeof("consteval") - 1         },
		{ "constexpr",         sizeof("constexpr") - 1         },
		{ "constinit",         sizeof("constinit") - 1         },
		{ "explicit",          sizeof("explicit") - 1          },
		{ "extern",            sizeof("extern") - 1            },
		{ "force_inline",   sizeof("force_inline") - 1   },
		{ "global",        sizeof("global") - 1        },
		{ "inline",            sizeof("inline") - 1            },
		{ "internal",      sizeof("internal") - 1      },
		{ "local_persist", sizeof("local_persist") - 1 },
		{ "mutable",           sizeof("mutable") - 1           },
		{ "never_inline",   sizeof("never_inline") - 1   },
		{ "*",		         sizeof("*") - 1                 },
		{ "&",		         sizeof("&") - 1                 },
		{ "register",          sizeof("register") - 1          },
		{ "restrict",          sizeof("restrict") - 1          },
		{ "&&",		        sizeof("&&") - 1                },
		{ "static",            sizeof("static") - 1            },
		{ "thread_local",  sizeof("thread_local") - 1  },
		{ "virtual",           sizeof("virtual") - 1           },
		{ "const",             sizeof("const") - 1             },
		{ "final",             sizeof("final") - 1             },
		{ "noexcept",          sizeof("noexcept") - 1          },
		{ "override",          sizeof("override") - 1          },
		{ "= 0",               sizeof("= 0") - 1               },
		{ "= delete",          sizeof("= delete") - 1          },
		{ "volatile",          sizeof("volatile") - 1          },
	};
	return lookup[type];
}

inline bool gen_spec_is_trailing(gen_Specifier specifier)
{
	switch (specifier)
	{
		case Spec_Const:
		case Spec_Final:
		case Spec_NoExceptions:
		case Spec_Override:
		case Spec_Pure:
		case Spec_Delete:
		case Spec_Volatile:
			return true;
		default:
			return false;
	}
}

inline gen_Specifier gen_str_to_specifier(gen_Str str)
{
	gen_local_persist gen_u32 keymap[Spec_NumSpecifiers];
	gen_do_once_start for (gen_u32 index = 0; index < Spec_NumSpecifiers; index++)
	{
		gen_Str gen_enum_str = gen_spec_to_str((gen_Specifier)index);
		keymap[index]        = gen_crc32(gen_enum_str.Ptr, gen_enum_str.Len);
	}
	gen_do_once_end gen_u32 gen_hash = gen_crc32(str.Ptr, str.Len);
	for (gen_u32 index = 0; index < Spec_NumSpecifiers; index++)
	{
		if (keymap[index] == gen_hash)
			return (gen_Specifier)index;
	}
	return Spec_Invalid;
}
#define GEN_DEFINE_ATTRIBUTE_TOKENS Entry(Tok_Attribute_GEN_API, "GEN_API")

enum gen_TokType
{
	Tok_Invalid,
	Tok_Access_Private,
	Tok_Access_Protected,
	Tok_Access_Public,
	Tok_Access_MemberSymbol,
	Tok_Access_StaticSymbol,
	Tok_Ampersand,
	Tok_Ampersand_DBL,
	Tok_Assign_Classifer,
	Tok_Attribute_Open,
	Tok_Attribute_Close,
	Tok_BraceCurly_Open,
	Tok_BraceCurly_Close,
	Tok_BraceSquare_Open,
	Tok_BraceSquare_Close,
	Tok_Paren_Open,
	Tok_Paren_Close,
	Tok_Comment,
	Tok_Comment_End,
	Tok_Comment_Start,
	Tok_Char,
	Tok_Comma,
	Tok_Decl_Class,
	Tok_Decl_GNU_Attribute,
	Tok_Decl_MSVC_Attribute,
	Tok_Decl_Enum,
	Tok_Decl_Extern_Linkage,
	Tok_Decl_Friend,
	Tok_Decl_Module,
	Tok_Decl_Namespace,
	Tok_Decl_Operator,
	Tok_Decl_Struct,
	Tok_Decl_Template,
	Tok_Decl_Typedef,
	Tok_Decl_Using,
	Tok_Decl_Union,
	Tok_Identifier,
	Tok_Module_Import,
	Tok_Module_Export,
	Tok_NewLine,
	Tok_Number,
	Tok_Operator,
	Tok_Preprocess_Hash,
	Tok_Preprocess_Define,
	Tok_Preprocess_Define_Param,
	Tok_Preprocess_If,
	Tok_Preprocess_IfDef,
	Tok_Preprocess_IfNotDef,
	Tok_Preprocess_ElIf,
	Tok_Preprocess_Else,
	Tok_Preprocess_EndIf,
	Tok_Preprocess_Include,
	Tok_Preprocess_Pragma,
	Tok_Preprocess_Content,
	Tok_Preprocess_Macro_Expr,
	Tok_Preprocess_Macro_Stmt,
	Tok_Preprocess_Macro_Typename,
	Tok_Preprocess_Unsupported,
	Tok_Spec_Alignas,
	Tok_Spec_Const,
	Tok_Spec_Consteval,
	Tok_Spec_Constexpr,
	Tok_Spec_Constinit,
	Tok_Spec_Explicit,
	Tok_Spec_Extern,
	Tok_Spec_Final,
	Tok_Spec_ForceInline,
	Tok_Spec_Global,
	Tok_Spec_Inline,
	Tok_Spec_Internal_Linkage,
	Tok_Spec_LocalPersist,
	Tok_Spec_Mutable,
	Tok_Spec_NeverInline,
	Tok_Spec_Override,
	Tok_Spec_Restrict,
	Tok_Spec_Static,
	Tok_Spec_ThreadLocal,
	Tok_Spec_Volatile,
	Tok_Spec_Virtual,
	Tok_Star,
	Tok_Statement_End,
	Tok_StaticAssert,
	Tok_String,
	Tok_Type_Typename,
	Tok_Type_Unsigned,
	Tok_Type_Signed,
	Tok_Type_Short,
	Tok_Type_Long,
	Tok_Type_bool,
	Tok_Type_char,
	Tok_Type_int,
	Tok_Type_double,
	Tok_Type_MS_int8,
	Tok_Type_MS_int16,
	Tok_Type_MS_int32,
	Tok_Type_MS_int64,
	Tok_Type_MS_W64,
	Tok_Varadic_Argument,
	Tok___Attributes_Start,
	Tok_Attribute_GEN_API,
	Tok_NumTokens,
	Tok_UnderlyingType = 0xffffffffu
};
typedef enum gen_TokType gen_TokType;

inline gen_Str gen_toktype_to_str(gen_TokType type)
{
	gen_local_persist gen_Str lookup[] = {
		{ "__invalid__",          sizeof("__invalid__") - 1          },
		{ "private",              sizeof("private") - 1              },
		{ "protected",            sizeof("protected") - 1            },
		{ "public",               sizeof("public") - 1               },
		{ ".",		            sizeof(".") - 1                    },
		{ "::",		           sizeof("::") - 1                   },
		{ "&",		            sizeof("&") - 1                    },
		{ "&&",		           sizeof("&&") - 1                   },
		{ ":",		            sizeof(":") - 1                    },
		{ "[[",		           sizeof("[[") - 1                   },
		{ "]]",		           sizeof("]]") - 1                   },
		{ "{",		            sizeof("{") - 1                    },
		{ "}",		            sizeof("}") - 1                    },
		{ "[",		            sizeof("[") - 1                    },
		{ "]",		            sizeof("]") - 1                    },
		{ "(",		            sizeof("(") - 1                    },
		{ ")",		            sizeof(")") - 1                    },
		{ "__comment__",          sizeof("__comment__") - 1          },
		{ "__comment_end__",      sizeof("__comment_end__") - 1      },
		{ "__comment_start__",    sizeof("__comment_start__") - 1    },
		{ "__character__",        sizeof("__character__") - 1        },
		{ ",",		            sizeof(",") - 1                    },
		{ "class",                sizeof("class") - 1                },
		{ "__attribute__",        sizeof("__attribute__") - 1        },
		{ "__declspec",           sizeof("__declspec") - 1           },
		{ "enum",                 sizeof("enum") - 1                 },
		{ "extern",               sizeof("extern") - 1               },
		{ "friend",               sizeof("friend") - 1               },
		{ "module",               sizeof("module") - 1               },
		{ "namespace",            sizeof("namespace") - 1            },
		{ "operator",             sizeof("operator") - 1             },
		{ "struct",               sizeof("struct") - 1               },
		{ "template",             sizeof("template") - 1             },
		{ "typedef",              sizeof("typedef") - 1              },
		{ "using",                sizeof("using") - 1                },
		{ "union",                sizeof("union") - 1                },
		{ "__identifier__",       sizeof("__identifier__") - 1       },
		{ "import",               sizeof("import") - 1               },
		{ "export",               sizeof("export") - 1               },
		{ "__new_line__",         sizeof("__new_line__") - 1         },
		{ "__number__",           sizeof("__number__") - 1           },
		{ "__operator__",         sizeof("__operator__") - 1         },
		{ "#",		            sizeof("#") - 1                    },
		{ "define",               sizeof("define") - 1               },
		{ "__define_param__",     sizeof("__define_param__") - 1     },
		{ "if",		           sizeof("if") - 1                   },
		{ "ifdef",                sizeof("ifdef") - 1                },
		{ "ifndef",               sizeof("ifndef") - 1               },
		{ "elif",                 sizeof("elif") - 1                 },
		{ "else",                 sizeof("else") - 1                 },
		{ "endif",                sizeof("endif") - 1                },
		{ "include",              sizeof("include") - 1              },
		{ "pragma",               sizeof("pragma") - 1               },
		{ "__macro_content__",    sizeof("__macro_content__") - 1    },
		{ "__macro_expression__", sizeof("__macro_expression__") - 1 },
		{ "__macro_statment__",   sizeof("__macro_statment__") - 1   },
		{ "__macro_typename__",   sizeof("__macro_typename__") - 1   },
		{ "__unsupported__",      sizeof("__unsupported__") - 1      },
		{ "alignas",              sizeof("alignas") - 1              },
		{ "const",                sizeof("const") - 1                },
		{ "consteval",            sizeof("consteval") - 1            },
		{ "constexpr",            sizeof("constexpr") - 1            },
		{ "constinit",            sizeof("constinit") - 1            },
		{ "explicit",             sizeof("explicit") - 1             },
		{ "extern",               sizeof("extern") - 1               },
		{ "final",                sizeof("final") - 1                },
		{ "force_inline",      sizeof("force_inline") - 1      },
		{ "global",           sizeof("global") - 1           },
		{ "inline",               sizeof("inline") - 1               },
		{ "internal",         sizeof("internal") - 1         },
		{ "local_persist",    sizeof("local_persist") - 1    },
		{ "mutable",              sizeof("mutable") - 1              },
		{ "never_inline",      sizeof("never_inline") - 1      },
		{ "override",             sizeof("override") - 1             },
		{ "restrict",             sizeof("restrict") - 1             },
		{ "static",               sizeof("static") - 1               },
		{ "thread_local",     sizeof("thread_local") - 1     },
		{ "volatile",             sizeof("volatile") - 1             },
		{ "virtual",              sizeof("virtual") - 1              },
		{ "*",		            sizeof("*") - 1                    },
		{ ";",		            sizeof(";") - 1                    },
		{ "static_assert",    sizeof("static_assert") - 1    },
		{ "__string__",           sizeof("__string__") - 1           },
		{ "typename",             sizeof("typename") - 1             },
		{ "unsigned",             sizeof("unsigned") - 1             },
		{ "signed",               sizeof("signed") - 1               },
		{ "short",                sizeof("short") - 1                },
		{ "long",                 sizeof("long") - 1                 },
		{ "bool",                 sizeof("bool") - 1                 },
		{ "char",                 sizeof("char") - 1                 },
		{ "int",		          sizeof("int") - 1                  },
		{ "double",               sizeof("double") - 1               },
		{ "__int8",               sizeof("__int8") - 1               },
		{ "__int16",              sizeof("__int16") - 1              },
		{ "__int32",              sizeof("__int32") - 1              },
		{ "__int64",              sizeof("__int64") - 1              },
		{ "_W64",                 sizeof("_W64") - 1                 },
		{ "...",		          sizeof("...") - 1                  },
		{ "__attrib_start__",     sizeof("__attrib_start__") - 1     },
		{ "GEN_API",              sizeof("GEN_API") - 1              },
	};
	return lookup[type];
}

inline gen_TokType gen_str_to_toktype(gen_Str str)
{
	gen_local_persist gen_u32 keymap[Tok_NumTokens];
	gen_do_once_start for (gen_u32 index = 0; index < Tok_NumTokens; index++)
	{
		gen_Str gen_enum_str = gen_toktype_to_str((gen_TokType)index);
		keymap[index]        = gen_crc32(gen_enum_str.Ptr, gen_enum_str.Len);
	}
	gen_do_once_end gen_u32 gen_hash = gen_crc32(str.Ptr, str.Len);
	for (gen_u32 index = 0; index < Tok_NumTokens; index++)
	{
		if (keymap[index] == gen_hash)
			return (gen_TokType)index;
	}
	return Tok_Invalid;
}

enum TokFlags gen_enum_underlying(gen_u32)
{
	TF_Operator           = gen_bit(0),
	TF_Assign             = gen_bit(1),
	TF_Identifier         = gen_bit(2),
	TF_Preprocess         = gen_bit(3),
	TF_Preprocess_Cond    = gen_bit(4),
	TF_Attribute          = gen_bit(5),
	TF_AccessOperator     = gen_bit(6),
	TF_AccessSpecifier    = gen_bit(7),
	TF_Specifier          = gen_bit(8),
	TF_EndDefinition      = gen_bit(9),    // Either ; or }
	TF_Formatting         = gen_bit(10),
	TF_Literal            = gen_bit(11),
	TF_Macro_Functional   = gen_bit(12),
	TF_Macro_Expects_Body = gen_bit(13),
	TF_Null               = 0,
	TF_UnderlyingType     = GEN_U32_MAX,
};

typedef gen_u32 TokFlags;

struct gen_Token;
typedef struct gen_Token gen_Token;
typedef gen_Token*       gen_Array_gen_Token;

struct gen_Token
{
	gen_Str     Text;
	gen_TokType Type;
	gen_s32     Line;
	gen_s32     Column;
	gen_u32     Flags;
};

#pragma region gen_Array_gen_Token

#define GEN_GENERIC_SLOT_6__array_init            gen_Token, gen_Array_gen_Token_init
#define GEN_GENERIC_SLOT_6__array_init_reserve    gen_Token, gen_Array_gen_Token_init_reserve
#define GEN_GENERIC_SLOT_6__array_append          gen_Array_gen_Token, gen_Array_gen_Token_append
#define GEN_GENERIC_SLOT_6__array_append_items    gen_Array_gen_Token, gen_Array_gen_Token_append_items
#define GEN_GENERIC_SLOT_6__array_append_at       gen_Array_gen_Token, gen_Array_gen_Token_append_at
#define GEN_GENERIC_SLOT_6__array_append_items_at gen_Array_gen_Token, gen_Array_gen_Token_append_items_at
#define GEN_GENERIC_SLOT_6__array_back            gen_Array_gen_Token, gen_Array_gen_Token_back
#define GEN_GENERIC_SLOT_6__array_clear           gen_Array_gen_Token, gen_Array_gen_Token_clear
#define GEN_GENERIC_SLOT_6__array_fill            gen_Array_gen_Token, gen_Array_gen_Token_fill
#define GEN_GENERIC_SLOT_6__array_free            gen_Array_gen_Token, gen_Array_gen_Token_free
#define GEN_GENERIC_SLOT_6__array_grow            gen_Array_gen_Token*, gen_Array_gen_Token_grow
#define GEN_GENERIC_SLOT_6__array_num             gen_Array_gen_Token, gen_Array_gen_Token_num
#define GEN_GENERIC_SLOT_6__array_pop             gen_Array_gen_Token, gen_Array_gen_Token_pop
#define GEN_GENERIC_SLOT_6__array_remove_at       gen_Array_gen_Token, gen_Array_gen_Token_remove_at
#define GEN_GENERIC_SLOT_6__array_reserve         gen_Array_gen_Token, gen_Array_gen_Token_reserve
#define GEN_GENERIC_SLOT_6__array_resize          gen_Array_gen_Token, gen_Array_gen_Token_resize
#define GEN_GENERIC_SLOT_6__array_set_capacity    gen_Array_gen_Token*, gen_Array_gen_Token_set_capacity

gen_Array_gen_Token gen_Array_gen_Token_init(gen_AllocatorInfo allocator);
gen_Array_gen_Token gen_Array_gen_Token_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                gen_Array_gen_Token_append_array(gen_Array_gen_Token* self, gen_Array_gen_Token other);
bool                gen_Array_gen_Token_append(gen_Array_gen_Token* self, gen_Token value);
bool                gen_Array_gen_Token_append_items(gen_Array_gen_Token* self, gen_Token* items, gen_usize item_num);
bool                gen_Array_gen_Token_append_at(gen_Array_gen_Token* self, gen_Token item, gen_usize idx);
bool                gen_Array_gen_Token_append_items_at(gen_Array_gen_Token* self, gen_Token* items, gen_usize item_num, gen_usize idx);
gen_Token*          gen_Array_gen_Token_back(gen_Array_gen_Token self);
void                gen_Array_gen_Token_clear(gen_Array_gen_Token self);
bool                gen_Array_gen_Token_fill(gen_Array_gen_Token self, gen_usize begin, gen_usize end, gen_Token value);
void                gen_Array_gen_Token_free(gen_Array_gen_Token* self);
bool                gen_Array_gen_Token_grow(gen_Array_gen_Token* self, gen_usize min_capacity);
gen_usize           gen_Array_gen_Token_num(gen_Array_gen_Token self);
gen_Token           gen_Array_gen_Token_pop(gen_Array_gen_Token self);
void                gen_Array_gen_Token_remove_at(gen_Array_gen_Token self, gen_usize idx);
bool                gen_Array_gen_Token_reserve(gen_Array_gen_Token* self, gen_usize new_capacity);
bool                gen_Array_gen_Token_resize(gen_Array_gen_Token* self, gen_usize num);
bool                gen_Array_gen_Token_set_capacity(gen_Array_gen_Token* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_Token gen_Array_gen_Token_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_Token, allocator, initial_size);
}

inline gen_Array_gen_Token gen_Array_gen_Token_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_Token) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_Token*, header + 1);
}

gen_forceinline bool gen_Array_gen_Token_append_array(gen_Array_gen_Token* self, gen_Array_gen_Token other)
{
	return gen_array_append_items(*self, (gen_Array_gen_Token)other, gen_Array_gen_Token_num(other));
}

inline bool gen_Array_gen_Token_append(gen_Array_gen_Token* self, gen_Token value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_Token_append_items(gen_Array_gen_Token* self, gen_Token* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_Token) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_Token_append_at(gen_Array_gen_Token* self, gen_Token item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_Token target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_Token));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_Token_append_items_at(gen_Array_gen_Token* self, gen_Token* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Token* target = (*self) + idx + item_num;
	gen_Token* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_Token));
	gen_mem_copy(src, items, item_num * sizeof(gen_Token));
	header->Num += item_num;
	return true;
}

inline gen_Token* gen_Array_gen_Token_back(gen_Array_gen_Token self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_Token_clear(gen_Array_gen_Token self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_Token_fill(gen_Array_gen_Token self, gen_usize begin, gen_usize end, gen_Token value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_Token_free(gen_Array_gen_Token* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_Token_grow(gen_Array_gen_Token* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_Token_num(gen_Array_gen_Token self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_Token gen_Array_gen_Token_pop(gen_Array_gen_Token self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_Token result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_Token_remove_at(gen_Array_gen_Token self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_Token) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_Token_reserve(gen_Array_gen_Token* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_Token_resize(gen_Array_gen_Token* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_Token_set_capacity(gen_Array_gen_Token* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_Token) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_Token) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_Token*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_Token


#define gen_NullToken                  \
	(gen_Token)                        \
	{                                  \
		{}, Tok_Invalid, 0, 0, TF_Null \
	}

gen_forceinline gen_AccessSpec gen_tok_to_access_specifier(gen_Token tok)
{
	return gen_scast(gen_AccessSpec, tok.Type);
}

gen_forceinline bool gen_tok_is_valid(gen_Token tok)
{
	return tok.Text.Ptr && tok.Text.Len && tok.Type != Tok_Invalid;
}

gen_forceinline bool gen_tok_is_access_operator(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_AccessOperator);
}

gen_forceinline bool gen_tok_is_access_specifier(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_AccessSpecifier);
}

gen_forceinline bool gen_tok_is_attribute(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_Attribute);
}

gen_forceinline bool gen_tok_is_operator(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_Operator);
}

gen_forceinline bool gen_tok_is_preprocessor(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_Preprocess);
}

gen_forceinline bool gen_tok_is_preprocess_cond(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_Preprocess_Cond);
}

gen_forceinline bool gen_tok_is_specifier(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_Specifier);
}

gen_forceinline bool gen_tok_is_end_definition(gen_Token tok)
{
	return gen_bitfield_is_set(gen_u32, tok.Flags, TF_EndDefinition);
}

gen_StrBuilder gen_tok_to_strbuilder(gen_Token tok);

typedef struct gen_TokArray gen_TokArray;

struct gen_TokArray
{
	gen_Array(gen_Token) Arr;
	gen_s32 Idx;
};

typedef struct gen_LexContext gen_LexContext;

struct gen_LexContext
{
	gen_Str     content;
	gen_s32     left;
	char const* scanner;
	gen_s32     line;
	gen_s32     column;
	// gen_StringTable     defines;
	gen_Token token;
};

typedef struct gen_StackNode gen_StackNode;

struct gen_StackNode
{
	gen_StackNode* Prev;

	gen_Token* Start;
	gen_Str    Name;        // The name of the gen_AST node (if parsed)
	gen_Str    ProcName;    // The name of the procedure
};

typedef struct gen_ParseContext gen_ParseContext;

struct gen_ParseContext
{
	gen_TokArray   Tokens;
	gen_StackNode* Scope;
};

enum gen_MacroType gen_enum_underlying(gen_u16)
{
	MT_Expression,    // A macro is assumed to be a expression if not resolved.
	MT_Statement,
	MT_Typename,
	MT_Block_Start,       // Not Supported yet
	MT_Block_End,         // Not Supported yet
	MT_Case_Statement,    // Not Supported yet

	MT_UnderlyingType = GEN_U16_MAX,
};

typedef gen_u16 gen_MacroType;

gen_forceinline gen_TokType gen_macrotype__to_toktype(gen_MacroType type)
{
	switch (type)
	{
		case MT_Statement:
			return Tok_Preprocess_Macro_Stmt;
		case MT_Expression:
			return Tok_Preprocess_Macro_Expr;
		case MT_Typename:
			return Tok_Preprocess_Macro_Typename;
	}
	// All others unsupported for now.
	return Tok_Invalid;
}

inline gen_Str gen_macrotype__to_str(gen_MacroType type)
{
	gen_local_persist gen_Str lookup[] = {
		{ "Statement",      sizeof("Statement") - 1      },
        { "Expression",     sizeof("Expression") - 1     },
        { "Typename",       sizeof("Typename") - 1       },
		{ "Block_Start",    sizeof("Block_Start") - 1    },
        { "Block_End",      sizeof("Block_End") - 1      },
        { "Case_Statement", sizeof("Case_Statement") - 1 },
	};
	gen_local_persist gen_Str invalid = { "Invalid", sizeof("Invalid") };
	if (type > MT_Case_Statement)
		return invalid;

	return lookup[type];
}

enum gen_EMacroFlags gen_enum_underlying(gen_u16)
{
	// gen_Macro has parameters (args expected to be passed)
	MF_Functional   = gen_bit(0),    // Expects to assign a braced scope to its body.
	MF_Expects_Body = gen_bit(1),    // gen_lex__eat wil treat this macro as an identifier if the parser attempts to consume it as one.
	// This is a kludge because we don't support push/pop macro pragmas rn.
	MF_Allow_As_Identifier = gen_bit(2
	),    // When parsing identifiers, it will allow the consumption of the macro parameters (as its expected to be a part of constructing the identifier)
	// Example of a decarator macro from stb_sprintf.h:
	// STBSP__PUBLICDEC int STB_SPRINTF_DECORATE(sprintf)(char* buf, char const *fmt, ...) STBSP__ATTRIBUTE_FORMAT(2,3);
	//                       ^^ STB_SPRINTF_DECORATE is decorating sprintf
	MF_Identifier_Decorator = gen_bit(3),    // gen_lex__eat wil treat this macro as an attribute if the parser attempts to consume it as one.
	// This a kludge because unreal has a macro that behaves as both a 'statement' and an attribute (UE_DEPRECATED, PRAGMA_ENABLE_DEPRECATION_WARNINGS, etc)
	// TODO(Ed): We can keep the MF_Allow_As_Attribute flag for macros, however, we need to add the ability of gen_AST_Attributes to chain themselves.
	// Its thats already a thing in the standard language anyway
	// & it would allow UE_DEPRECATED, (UE_PROPERTY / UE_FUNCTION) to chain themselves as attributes of a resolved member function/variable definition
	MF_Allow_As_Attribute = gen_bit(4),    // When a macro is encountered after attributes and specifiers while parsing a function, or variable:
	// It will consume the macro and treat it as resolving the definition.
	// (MUST BE OF MT_Statement TYPE)
	MF_Allow_As_Definition = gen_bit(5),    // Created for Unreal's PURE_VIRTUAL
	MF_Allow_As_Specifier  = gen_bit(6),
	MF_Null                = 0,
	MF_UnderlyingType      = GEN_U16_MAX,
};

typedef gen_u16 gen_EMacroFlags;
typedef gen_u16 gen_MacroFlags;

typedef struct gen_Macro gen_Macro;

struct gen_Macro
{
	gen_StrCached  Name;
	gen_MacroType  Type;
	gen_MacroFlags Flags;
};

gen_forceinline gen_b32 gen_macro_is_functional(gen_Macro macro)
{
	return gen_bitfield_is_set(gen_b16, macro.Flags, MF_Functional);
}

gen_forceinline gen_b32 gen_macro_expects_body(gen_Macro macro)
{
	return gen_bitfield_is_set(gen_b16, macro.Flags, MF_Expects_Body);
}

#if GEN_COMPILER_CPP && ! GEN_C_LIKE_CPP
gen_forceinline gen_b32 is_functional(gen_Macro macro)
{
	return gen_bitfield_is_set(gen_b16, macro.Flags, MF_Functional);
}

gen_forceinline gen_b32 expects_body(gen_Macro macro)
{
	return gen_bitfield_is_set(gen_b16, macro.Flags, MF_Expects_Body);
}
#endif

typedef gen_HashTable(gen_Macro) MacroTable;
#pragma endregion Types

#pragma region AST

/*
  ______   ______  ________      __    __       ______                 __
 /      \ /      \|        \    |  \  |  \     /      \               |  \
|  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\\▓▓▓▓▓▓▓▓    | ▓▓\ | ▓▓    |  ▓▓▓▓▓▓\ ______   ____| ▓▓ ______
| ▓▓__| ▓▓ ▓▓___\▓▓  | ▓▓       | ▓▓▓\| ▓▓    | ▓▓   \▓▓/      \ /      ▓▓/      \
| ▓▓    ▓▓\▓▓    \   | ▓▓       | ▓▓▓▓\ ▓▓    | ▓▓     |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\
| ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\  | ▓▓       | ▓▓\▓▓ ▓▓    | ▓▓   __| ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓
| ▓▓  | ▓▓  \__| ▓▓  | ▓▓       | ▓▓ \▓▓▓▓    | ▓▓__/  \ ▓▓__/ ▓▓ ▓▓__| ▓▓ ▓▓▓▓▓▓▓▓
| ▓▓  | ▓▓\▓▓    ▓▓  | ▓▓       | ▓▓  \▓▓▓     \▓▓    ▓▓\▓▓    ▓▓\▓▓    ▓▓\▓▓     \
 \▓▓   \▓▓ \▓▓▓▓▓▓    \▓▓        \▓▓   \▓▓      \▓▓▓▓▓▓  \▓▓▓▓▓▓  \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓
*/

struct gen_AST;
typedef struct gen_AST gen_AST;
struct gen_AST_Body;
typedef struct gen_AST_Body gen_AST_Body;
struct gen_AST_Attributes;
typedef struct gen_AST_Attributes gen_AST_Attributes;
struct gen_AST_Comment;
typedef struct gen_AST_Comment gen_AST_Comment;
struct gen_AST_Constructor;
typedef struct gen_AST_Constructor gen_AST_Constructor;
// struct gen_AST_BaseClass;
struct gen_AST_Class;
typedef struct gen_AST_Class gen_AST_Class;
struct gen_AST_Define;
typedef struct gen_AST_Define gen_AST_Define;
struct gen_AST_DefineParams;
typedef struct gen_AST_DefineParams gen_AST_DefineParams;
struct gen_AST_Destructor;
typedef struct gen_AST_Destructor gen_AST_Destructor;
struct gen_AST_Enum;
typedef struct gen_AST_Enum gen_AST_Enum;
struct gen_AST_Exec;
typedef struct gen_AST_Exec gen_AST_Exec;
struct gen_AST_Extern;
typedef struct gen_AST_Extern gen_AST_Extern;
struct gen_AST_Include;
typedef struct gen_AST_Include gen_AST_Include;
struct gen_AST_Friend;
typedef struct gen_AST_Friend gen_AST_Friend;
struct gen_AST_Fn;
typedef struct gen_AST_Fn gen_AST_Fn;
struct gen_AST_Module;
typedef struct gen_AST_Module gen_AST_Module;
struct gen_AST_NS;
typedef struct gen_AST_NS gen_AST_NS;
struct gen_AST_Operator;
typedef struct gen_AST_Operator gen_AST_Operator;
struct gen_AST_OpCast;
typedef struct gen_AST_OpCast gen_AST_OpCast;
struct gen_AST_Params;
typedef struct gen_AST_Params gen_AST_Params;
struct gen_AST_Pragma;
typedef struct gen_AST_Pragma gen_AST_Pragma;
struct gen_AST_PreprocessCond;
typedef struct gen_AST_PreprocessCond gen_AST_PreprocessCond;
struct gen_AST_Specifiers;
typedef struct gen_AST_Specifiers gen_AST_Specifiers;


struct gen_AST_Struct;
typedef struct gen_AST_Struct gen_AST_Struct;
struct gen_AST_Template;
typedef struct gen_AST_Template gen_AST_Template;
struct gen_AST_Typename;
typedef struct gen_AST_Typename gen_AST_Typename;
struct gen_AST_Typedef;
typedef struct gen_AST_Typedef gen_AST_Typedef;
struct gen_AST_Union;
typedef struct gen_AST_Union gen_AST_Union;
struct gen_AST_Using;
typedef struct gen_AST_Using gen_AST_Using;
struct gen_AST_Var;
typedef struct gen_AST_Var gen_AST_Var;


typedef gen_AST* gen_Code;

typedef gen_AST_Body*           gen_CodeBody;
typedef gen_AST_Attributes*     gen_CodeAttributes;
typedef gen_AST_Comment*        gen_CodeComment;
typedef gen_AST_Class*          gen_CodeClass;
typedef gen_AST_Constructor*    gen_CodeConstructor;
typedef gen_AST_Define*         gen_CodeDefine;
typedef gen_AST_DefineParams*   gen_CodeDefineParams;
typedef gen_AST_Destructor*     gen_CodeDestructor;
typedef gen_AST_Enum*           gen_CodeEnum;
typedef gen_AST_Exec*           gen_CodeExec;
typedef gen_AST_Extern*         gen_CodeExtern;
typedef gen_AST_Include*        gen_CodeInclude;
typedef gen_AST_Friend*         gen_CodeFriend;
typedef gen_AST_Fn*             gen_CodeFn;
typedef gen_AST_Module*         gen_CodeModule;
typedef gen_AST_NS*             gen_CodeNS;
typedef gen_AST_Operator*       gen_CodeOperator;
typedef gen_AST_OpCast*         gen_CodeOpCast;
typedef gen_AST_Params*         gen_CodeParams;
typedef gen_AST_PreprocessCond* gen_CodePreprocessCond;
typedef gen_AST_Pragma*         gen_CodePragma;
typedef gen_AST_Specifiers*     gen_CodeSpecifiers;


typedef gen_AST_Struct*   gen_CodeStruct;
typedef gen_AST_Template* gen_CodeTemplate;
typedef gen_AST_Typename* gen_CodeTypename;
typedef gen_AST_Typedef*  gen_CodeTypedef;
typedef gen_AST_Union*    gen_CodeUnion;
typedef gen_AST_Using*    gen_CodeUsing;
typedef gen_AST_Var*      gen_CodeVar;

#pragma region gen_Code C-Interface


void                   gen_code__append(gen_Code code, gen_Code other);
GEN_API gen_Str        gen_code__debug_str(gen_Code code);
GEN_API gen_Code       gen_code__duplicate(gen_Code code);
gen_Code*              gen_code__entry(gen_Code code, gen_u32 idx);
bool                   gen_code__has_entries(gen_Code code);
bool                   gen_code__is_body(gen_Code code);
GEN_API bool           gen_code__is_equal(gen_Code code, gen_Code other);
bool                   gen_code__is_valid(gen_Code code);
void                   gen_code__set_global(gen_Code code);
GEN_API gen_StrBuilder gen_code__to_strbuilder(gen_Code self);
GEN_API void           gen_code__to_strbuilder_ref(gen_Code self, gen_StrBuilder* result);
gen_Str                gen_code__type_str(gen_Code self);
GEN_API bool           gen_code__validate_body(gen_Code self);

#pragma endregion gen_Code C - Interface



// Used to identify ASTs that should always be duplicated. (Global constant ASTs)
GEN_API extern gen_Code gen_Code_Global;

// Used to identify invalid generated code.
GEN_API extern gen_Code gen_Code_Invalid;

struct gen_Code_POD
{
	gen_AST* ast;
};
typedef struct gen_Code_POD gen_Code_POD;
gen_static_assert(sizeof(gen_Code) == sizeof(gen_Code_POD), "ERROR: gen_Code is not POD");

// Desired width of the gen_AST data structure.
#define gen_AST_POD_Size 128

#define gen_AST_ArrSpecs_Cap                                                                                                                           \
	(gen_AST_POD_Size - sizeof(gen_Code) - sizeof(gen_StrCached) - sizeof(gen_Code) * 2 - sizeof(gen_Token*) - sizeof(gen_Code) - sizeof(gen_CodeType) \
	 - sizeof(gen_ModuleFlag) - sizeof(gen_u32))                                                                                                       \
	        / sizeof(gen_Specifier)                                                                                                                    \
	    - 1

/*
	Simple gen_AST POD with functionality to seralize into C++ syntax.
	TODO(Ed): Eventually haven't a transparent gen_AST like this will longer be viable once statements & expressions are in (most likely....)
*/
struct gen_AST
{
	union
	{
		struct
		{
			gen_Code InlineCmt;     // Class, Constructor, Destructor, Enum, Friend, Functon, gen_Operator, OpCast, Struct, Typedef, Using, Variable
			gen_Code Attributes;    // Class, Enum, Function, Struct, Typedef, Union, Using, Variable // TODO(Ed): Parameters can have attributes
			gen_Code Specs;         // Class, Destructor, Function, gen_Operator, Struct, Typename, Variable

			union
			{
				gen_Code InitializerList;    // Constructor
				gen_Code ParentType;         // Class, Struct, ParentType->Next has a possible list of interfaces.
				gen_Code ReturnType;         // Function, gen_Operator, Typename
				gen_Code UnderlyingType;     // Enum, Typedef
				gen_Code ValueType;          // Parameter, Variable
			};

			union
			{
				gen_Code gen_Macro;              // Parameter
				gen_Code BitfieldSize;           // Variable (Class/Struct Data Member)
				gen_Code Params;                 // Constructor, Define, Function, gen_Operator, Template, Typename
				gen_Code UnderlyingTypeMacro;    // Enum
			};

			union
			{
				gen_Code ArrExpr;        // Typename
				gen_Code Body;           // Class, Constructor, Define, Destructor, Enum, Friend, Function, Namespace, Struct, Union
				gen_Code Declaration;    // Friend, Template
				gen_Code Value;          // Parameter, Variable
			};

			union
			{
				gen_Code NextVar;          // Variable
				gen_Code SuffixSpecs;      // Typename, Function (Thanks Unreal)
				gen_Code PostNameMacro;    // Only used with parameters for specifically UE_REQUIRES (Thanks Unreal)
			};
		};

		gen_StrCached Content;    // Attributes, Comment, Execution, Include

		struct
		{
			gen_Specifier ArrSpecs[gen_AST_ArrSpecs_Cap];    // Specifiers
			gen_Code      NextSpecs;                         // Specifiers; If ArrSpecs is full, then NextSpecs is used.
		};
	};

	gen_StrCached Name;

	union
	{
		gen_Code Prev;
		gen_Code Front;
		gen_Code Last;
	};

	union
	{
		gen_Code Next;
		gen_Code Back;
	};

	gen_Token*   gen_Token;    // Reference to starting token, only available if it was derived from parsing.
	gen_Code     Parent;
	gen_CodeType Type;
	//	gen_CodeFlag          CodeFlags;
	gen_ModuleFlag ModuleFlags;

	union
	{
		gen_b32 IsFunction;    // Used by typedef to not serialize the name field.

		struct
		{
			gen_b16          IsParamPack;    // Used by typename to know if type should be considered a parameter pack.
			gen_ETypenameTag TypeTag;        // Used by typename to keep track of explicitly declared tags for the identifier (enum, struct, union)
		};

		gen_Operator   Op;
		gen_AccessSpec ParentAccess;
		gen_s32        NumEntries;
		gen_s32
		    VarParenthesizedInit;    // Used by variables to know that initialization is using a gen_constructor_ expression instead of an assignment expression.
	};
};

gen_static_assert(sizeof(gen_AST) == gen_AST_POD_Size, "ERROR: gen_AST is not size of gen_AST_POD_Size");

#define gen_InvalidCode         \
	(void*)                     \
	{                           \
		(void*)gen_Code_Invalid \
	}

#define gen_NullCode gen_nullptr

/*
  ______                 __               ______            __                        ______
 /      \               |  \             |      \          |  \                      /      \
|  ▓▓▓▓▓▓\ ______   ____| ▓▓ ______       \▓▓▓▓▓▓_______  _| ▓▓_    ______   ______ |  ▓▓▓▓▓▓\ ______   _______  ______
| ▓▓   \▓▓/      \ /      ▓▓/      \       | ▓▓ |       \|   ▓▓ \  /      \ /      \| ▓▓_  \▓▓|      \ /       \/      \
| ▓▓     |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\      | ▓▓ | ▓▓▓▓▓▓▓\\▓▓▓▓▓▓ |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\ ▓▓ \     \▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\
| ▓▓   __| ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓      | ▓▓ | ▓▓  | ▓▓ | ▓▓ __| ▓▓    ▓▓ ▓▓   \▓▓ ▓▓▓▓    /      ▓▓ ▓▓     | ▓▓    ▓▓
| ▓▓__/  \ ▓▓__/ ▓▓ ▓▓__| ▓▓ ▓▓▓▓▓▓▓▓     _| ▓▓_| ▓▓  | ▓▓ | ▓▓|  \ ▓▓▓▓▓▓▓▓ ▓▓     | ▓▓     |  ▓▓▓▓▓▓▓ ▓▓_____| ▓▓▓▓▓▓▓▓
 \▓▓    ▓▓\▓▓    ▓▓\▓▓    ▓▓\▓▓     \    |   ▓▓ \ ▓▓  | ▓▓  \▓▓  ▓▓\▓▓     \ ▓▓     | ▓▓      \▓▓    ▓▓\▓▓     \\▓▓     \
  \▓▓▓▓▓▓  \▓▓▓▓▓▓  \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓     \▓▓▓▓▓▓\▓▓   \▓▓   \▓▓▓▓  \▓▓▓▓▓▓▓\▓▓      \▓▓       \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓
*/

#pragma region gen_Code Type C-Interface

#define gen_code_append(code, other)              \
	_Generic(                                     \
	    (code),                                   \
	    gen_Code: gen_code__append,               \
	    gen_CodeBody: gen_code__append,           \
	    gen_CodeAttributes: gen_code__append,     \
	    gen_CodeComment: gen_code__append,        \
	    gen_CodeClass: gen_code__append,          \
	    gen_CodeConstructor: gen_code__append,    \
	    gen_CodeDefine: gen_code__append,         \
	    gen_CodeDefineParams: gen_code__append,   \
	    gen_CodeDestructor: gen_code__append,     \
	    gen_CodeEnum: gen_code__append,           \
	    gen_CodeExec: gen_code__append,           \
	    gen_CodeExtern: gen_code__append,         \
	    gen_CodeInclude: gen_code__append,        \
	    gen_CodeFriend: gen_code__append,         \
	    gen_CodeFn: gen_code__append,             \
	    gen_CodeModule: gen_code__append,         \
	    gen_CodeNS: gen_code__append,             \
	    gen_CodeOperator: gen_code__append,       \
	    gen_CodeOpCast: gen_code__append,         \
	    gen_CodePragma: gen_code__append,         \
	    gen_CodeParams: gen_code__append,         \
	    gen_CodePreprocessCond: gen_code__append, \
	    gen_CodeSpecifiers: gen_code__append,     \
	    gen_CodeStruct: gen_code__append,         \
	    gen_CodeTemplate: gen_code__append,       \
	    gen_CodeTypename: gen_code__append,       \
	    gen_CodeTypedef: gen_code__append,        \
	    gen_CodeUnion: gen_code__append,          \
	    gen_CodeUsing: gen_code__append,          \
	    gen_CodeVar: gen_code__append,            \
	    default: gen_generic_selection_fail       \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code, other)

#define gen_code_debug_str(code)                     \
	_Generic(                                        \
	    (code),                                      \
	    gen_Code: gen_code__debug_str,               \
	    gen_CodeBody: gen_code__debug_str,           \
	    gen_CodeAttributes: gen_code__debug_str,     \
	    gen_CodeComment: gen_code__debug_str,        \
	    gen_CodeClass: gen_code__debug_str,          \
	    gen_CodeConstructor: gen_code__debug_str,    \
	    gen_CodeDefine: gen_code__debug_str,         \
	    gen_CodeDefineParams: gen_code__debug_str,   \
	    gen_CodeDestructor: gen_code__debug_str,     \
	    gen_CodeEnum: gen_code__debug_str,           \
	    gen_CodeExec: gen_code__debug_str,           \
	    gen_CodeExtern: gen_code__debug_str,         \
	    gen_CodeInclude: gen_code__debug_str,        \
	    gen_CodeFriend: gen_code__debug_str,         \
	    gen_CodeFn: gen_code__debug_str,             \
	    gen_CodeModule: gen_code__debug_str,         \
	    gen_CodeNS: gen_code__debug_str,             \
	    gen_CodeOperator: gen_code__debug_str,       \
	    gen_CodeOpCast: gen_code__debug_str,         \
	    gen_CodePragma: gen_code__debug_str,         \
	    gen_CodeParams: gen_code__debug_str,         \
	    gen_CodePreprocessCond: gen_code__debug_str, \
	    gen_CodeSpecifiers: gen_code__debug_str,     \
	    gen_CodeStruct: gen_code__debug_str,         \
	    gen_CodeTemplate: gen_code__debug_str,       \
	    gen_CodeTypename: gen_code__debug_str,       \
	    gen_CodeTypedef: gen_code__debug_str,        \
	    gen_CodeUnion: gen_code__debug_str,          \
	    gen_CodeUsing: gen_code__debug_str,          \
	    gen_CodeVar: gen_code__debug_str,            \
	    default: gen_generic_selection_fail          \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_duplicate(code)                     \
	_Generic(                                        \
	    (code),                                      \
	    gen_Code: gen_code__duplicate,               \
	    gen_CodeBody: gen_code__duplicate,           \
	    gen_CodeAttributes: gen_code__duplicate,     \
	    gen_CodeComment: gen_code__duplicate,        \
	    gen_CodeClass: gen_code__duplicate,          \
	    gen_CodeConstructor: gen_code__duplicate,    \
	    gen_CodeDefine: gen_code__duplicate,         \
	    gen_CodeDefineParams: gen_code__duplicate,   \
	    gen_CodeDestructor: gen_code__duplicate,     \
	    gen_CodeEnum: gen_code__duplicate,           \
	    gen_CodeExec: gen_code__duplicate,           \
	    gen_CodeExtern: gen_code__duplicate,         \
	    gen_CodeInclude: gen_code__duplicate,        \
	    gen_CodeFriend: gen_code__duplicate,         \
	    gen_CodeFn: gen_code__duplicate,             \
	    gen_CodeModule: gen_code__duplicate,         \
	    gen_CodeNS: gen_code__duplicate,             \
	    gen_CodeOperator: gen_code__duplicate,       \
	    gen_CodeOpCast: gen_code__duplicate,         \
	    gen_CodePragma: gen_code__duplicate,         \
	    gen_CodeParams: gen_code__duplicate,         \
	    gen_CodePreprocessCond: gen_code__duplicate, \
	    gen_CodeSpecifiers: gen_code__duplicate,     \
	    gen_CodeStruct: gen_code__duplicate,         \
	    gen_CodeTemplate: gen_code__duplicate,       \
	    gen_CodeTypename: gen_code__duplicate,       \
	    gen_CodeTypedef: gen_code__duplicate,        \
	    gen_CodeUnion: gen_code__duplicate,          \
	    gen_CodeUsing: gen_code__duplicate,          \
	    gen_CodeVar: gen_code__duplicate,            \
	    default: gen_generic_selection_fail          \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_entry(code, idx)                \
	_Generic(                                    \
	    (code),                                  \
	    gen_Code: gen_code__entry,               \
	    gen_CodeBody: gen_code__entry,           \
	    gen_CodeAttributes: gen_code__entry,     \
	    gen_CodeComment: gen_code__entry,        \
	    gen_CodeClass: gen_code__entry,          \
	    gen_CodeConstructor: gen_code__entry,    \
	    gen_CodeDefine: gen_code__entry,         \
	    gen_CodeDefineParams: gen_code__entry,   \
	    gen_CodeDestructor: gen_code__entry,     \
	    gen_CodeEnum: gen_code__entry,           \
	    gen_CodeExec: gen_code__entry,           \
	    gen_CodeExtern: gen_code__entry,         \
	    gen_CodeInclude: gen_code__entry,        \
	    gen_CodeFriend: gen_code__entry,         \
	    gen_CodeFn: gen_code__entry,             \
	    gen_CodeModule: gen_code__entry,         \
	    gen_CodeNS: gen_code__entry,             \
	    gen_CodeOperator: gen_code__entry,       \
	    gen_CodeOpCast: gen_code__entry,         \
	    gen_CodePragma: gen_code__entry,         \
	    gen_CodeParams: gen_code__entry,         \
	    gen_CodePreprocessCond: gen_code__entry, \
	    gen_CodeSpecifiers: gen_code__entry,     \
	    gen_CodeStruct: gen_code__entry,         \
	    gen_CodeTemplate: gen_code__entry,       \
	    gen_CodeTypename: gen_code__entry,       \
	    gen_CodeTypedef: gen_code__entry,        \
	    gen_CodeUnion: gen_code__entry,          \
	    gen_CodeUsing: gen_code__entry,          \
	    gen_CodeVar: gen_code__entry,            \
	    default: gen_generic_selection_fail      \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code, idx)

#define gen_code_has_entries(code)                     \
	_Generic(                                          \
	    (code),                                        \
	    gen_Code: gen_code__has_entries,               \
	    gen_CodeBody: gen_code__has_entries,           \
	    gen_CodeAttributes: gen_code__has_entries,     \
	    gen_CodeComment: gen_code__has_entries,        \
	    gen_CodeClass: gen_code__has_entries,          \
	    gen_CodeConstructor: gen_code__has_entries,    \
	    gen_CodeDefine: gen_code__has_entries,         \
	    gen_CodeDefineParams: gen_code__has_entries,   \
	    gen_CodeDestructor: gen_code__has_entries,     \
	    gen_CodeEnum: gen_code__has_entries,           \
	    gen_CodeExec: gen_code__has_entries,           \
	    gen_CodeExtern: gen_code__has_entries,         \
	    gen_CodeInclude: gen_code__has_entries,        \
	    gen_CodeFriend: gen_code__has_entries,         \
	    gen_CodeFn: gen_code__has_entries,             \
	    gen_CodeModule: gen_code__has_entries,         \
	    gen_CodeNS: gen_code__has_entries,             \
	    gen_CodeOperator: gen_code__has_entries,       \
	    gen_CodeOpCast: gen_code__has_entries,         \
	    gen_CodePragma: gen_code__has_entries,         \
	    gen_CodeParams: gen_code__has_entries,         \
	    gen_CodePreprocessCond: gen_code__has_entries, \
	    gen_CodeSpecifiers: gen_code__has_entries,     \
	    gen_CodeStruct: gen_code__has_entries,         \
	    gen_CodeTemplate: gen_code__has_entries,       \
	    gen_CodeTypename: gen_code__has_entries,       \
	    gen_CodeTypedef: gen_code__has_entries,        \
	    gen_CodeUnion: gen_code__has_entries,          \
	    gen_CodeUsing: gen_code__has_entries,          \
	    gen_CodeVar: gen_code__has_entries,            \
	    default: gen_generic_selection_fail            \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_is_body(code)                     \
	_Generic(                                      \
	    (code),                                    \
	    gen_Code: gen_code__is_body,               \
	    gen_CodeBody: gen_code__is_body,           \
	    gen_CodeAttributes: gen_code__is_body,     \
	    gen_CodeComment: gen_code__is_body,        \
	    gen_CodeClass: gen_code__is_body,          \
	    gen_CodeConstructor: gen_code__is_body,    \
	    gen_CodeDefine: gen_code__is_body,         \
	    gen_CodeDefineParams: gen_code__is_body,   \
	    gen_CodeDestructor: gen_code__is_body,     \
	    gen_CodeEnum: gen_code__is_body,           \
	    gen_CodeExec: gen_code__is_body,           \
	    gen_CodeExtern: gen_code__is_body,         \
	    gen_CodeInclude: gen_code__is_body,        \
	    gen_CodeFriend: gen_code__is_body,         \
	    gen_CodeFn: gen_code__is_body,             \
	    gen_CodeModule: gen_code__is_body,         \
	    gen_CodeNS: gen_code__is_body,             \
	    gen_CodeOperator: gen_code__is_body,       \
	    gen_CodeOpCast: gen_code__is_body,         \
	    gen_CodePragma: gen_code__is_body,         \
	    gen_CodeParams: gen_code__is_body,         \
	    gen_CodePreprocessCond: gen_code__is_body, \
	    gen_CodeSpecifiers: gen_code__is_body,     \
	    gen_CodeStruct: gen_code__is_body,         \
	    gen_CodeTemplate: gen_code__is_body,       \
	    gen_CodeTypename: gen_code__is_body,       \
	    gen_CodeTypedef: gen_code__is_body,        \
	    gen_CodeUnion: gen_code__is_body,          \
	    gen_CodeUsing: gen_code__is_body,          \
	    gen_CodeVar: gen_code__is_body,            \
	    default: gen_generic_selection_fail        \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_is_equal(code, other)              \
	_Generic(                                       \
	    (code),                                     \
	    gen_Code: gen_code__is_equal,               \
	    gen_CodeBody: gen_code__is_equal,           \
	    gen_CodeAttributes: gen_code__is_equal,     \
	    gen_CodeComment: gen_code__is_equal,        \
	    gen_CodeClass: gen_code__is_equal,          \
	    gen_CodeConstructor: gen_code__is_equal,    \
	    gen_CodeDefine: gen_code__is_equal,         \
	    gen_CodeDefineParams: gen_code__is_equal,   \
	    gen_CodeDestructor: gen_code__is_equal,     \
	    gen_CodeEnum: gen_code__is_equal,           \
	    gen_CodeExec: gen_code__is_equal,           \
	    gen_CodeExtern: gen_code__is_equal,         \
	    gen_CodeInclude: gen_code__is_equal,        \
	    gen_CodeFriend: gen_code__is_equal,         \
	    gen_CodeFn: gen_code__is_equal,             \
	    gen_CodeModule: gen_code__is_equal,         \
	    gen_CodeNS: gen_code__is_equal,             \
	    gen_CodeOperator: gen_code__is_equal,       \
	    gen_CodeOpCast: gen_code__is_equal,         \
	    gen_CodePragma: gen_code__is_equal,         \
	    gen_CodeParams: gen_code__is_equal,         \
	    gen_CodePreprocessCond: gen_code__is_equal, \
	    gen_CodeSpecifiers: gen_code__is_equal,     \
	    gen_CodeStruct: gen_code__is_equal,         \
	    gen_CodeTemplate: gen_code__is_equal,       \
	    gen_CodeTypename: gen_code__is_equal,       \
	    gen_CodeTypedef: gen_code__is_equal,        \
	    gen_CodeUnion: gen_code__is_equal,          \
	    gen_CodeUsing: gen_code__is_equal,          \
	    gen_CodeVar: gen_code__is_equal,            \
	    default: gen_generic_selection_fail         \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code, other)

#define gen_code_is_valid(code)                     \
	_Generic(                                       \
	    (code),                                     \
	    gen_Code: gen_code__is_valid,               \
	    gen_CodeBody: gen_code__is_valid,           \
	    gen_CodeAttributes: gen_code__is_valid,     \
	    gen_CodeComment: gen_code__is_valid,        \
	    gen_CodeClass: gen_code__is_valid,          \
	    gen_CodeConstructor: gen_code__is_valid,    \
	    gen_CodeDefine: gen_code__is_valid,         \
	    gen_CodeDefineParams: gen_code__is_valid,   \
	    gen_CodeDestructor: gen_code__is_valid,     \
	    gen_CodeEnum: gen_code__is_valid,           \
	    gen_CodeExec: gen_code__is_valid,           \
	    gen_CodeExtern: gen_code__is_valid,         \
	    gen_CodeInclude: gen_code__is_valid,        \
	    gen_CodeFriend: gen_code__is_valid,         \
	    gen_CodeFn: gen_code__is_valid,             \
	    gen_CodeModule: gen_code__is_valid,         \
	    gen_CodeNS: gen_code__is_valid,             \
	    gen_CodeOperator: gen_code__is_valid,       \
	    gen_CodeOpCast: gen_code__is_valid,         \
	    gen_CodePragma: gen_code__is_valid,         \
	    gen_CodeParams: gen_code__is_valid,         \
	    gen_CodePreprocessCond: gen_code__is_valid, \
	    gen_CodeSpecifiers: gen_code__is_valid,     \
	    gen_CodeStruct: gen_code__is_valid,         \
	    gen_CodeTemplate: gen_code__is_valid,       \
	    gen_CodeTypename: gen_code__is_valid,       \
	    gen_CodeTypedef: gen_code__is_valid,        \
	    gen_CodeUnion: gen_code__is_valid,          \
	    gen_CodeUsing: gen_code__is_valid,          \
	    gen_CodeVar: gen_code__is_valid,            \
	    default: gen_generic_selection_fail         \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_set_global(code)                     \
	_Generic(                                         \
	    (code),                                       \
	    gen_Code: gen_code__set_global,               \
	    gen_CodeBody: gen_code__set_global,           \
	    gen_CodeAttributes: gen_code__set_global,     \
	    gen_CodeComment: gen_code__set_global,        \
	    gen_CodeClass: gen_code__set_global,          \
	    gen_CodeConstructor: gen_code__set_global,    \
	    gen_CodeDefine: gen_code__set_global,         \
	    gen_CodeDefineParams: gen_code__set_global,   \
	    gen_CodeDestructor: gen_code__set_global,     \
	    gen_CodeEnum: gen_code__set_global,           \
	    gen_CodeExec: gen_code__set_global,           \
	    gen_CodeExtern: gen_code__set_global,         \
	    gen_CodeInclude: gen_code__set_global,        \
	    gen_CodeFriend: gen_code__set_global,         \
	    gen_CodeFn: gen_code__set_global,             \
	    gen_CodeModule: gen_code__set_global,         \
	    gen_CodeNS: gen_code__set_global,             \
	    gen_CodeOperator: gen_code__set_global,       \
	    gen_CodeOpCast: gen_code__set_global,         \
	    gen_CodePragma: gen_code__set_global,         \
	    gen_CodeParams: gen_code__set_global,         \
	    gen_CodePreprocessCond: gen_code__set_global, \
	    gen_CodeSpecifiers: gen_code__set_global,     \
	    gen_CodeStruct: gen_code__set_global,         \
	    gen_CodeTemplate: gen_code__set_global,       \
	    gen_CodeTypename: gen_code__set_global,       \
	    gen_CodeTypedef: gen_code__set_global,        \
	    gen_CodeUnion: gen_code__set_global,          \
	    gen_CodeUsing: gen_code__set_global,          \
	    gen_CodeVar: gen_code__set_global,            \
	    default: gen_generic_selection_fail           \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_to_strbuilder(code)                     \
	_Generic(                                            \
	    (code),                                          \
	    gen_Code: gen_code__to_strbuilder,               \
	    gen_CodeBody: gen_code__to_strbuilder,           \
	    gen_CodeAttributes: gen_code__to_strbuilder,     \
	    gen_CodeComment: gen_code__to_strbuilder,        \
	    gen_CodeClass: gen_code__to_strbuilder,          \
	    gen_CodeConstructor: gen_code__to_strbuilder,    \
	    gen_CodeDefine: gen_code__to_strbuilder,         \
	    gen_CodeDefineParams: gen_code__to_strbuilder,   \
	    gen_CodeDestructor: gen_code__to_strbuilder,     \
	    gen_CodeEnum: gen_code__to_strbuilder,           \
	    gen_CodeExec: gen_code__to_strbuilder,           \
	    gen_CodeExtern: gen_code__to_strbuilder,         \
	    gen_CodeInclude: gen_code__to_strbuilder,        \
	    gen_CodeFriend: gen_code__to_strbuilder,         \
	    gen_CodeFn: gen_code__to_strbuilder,             \
	    gen_CodeModule: gen_code__to_strbuilder,         \
	    gen_CodeNS: gen_code__to_strbuilder,             \
	    gen_CodeOperator: gen_code__to_strbuilder,       \
	    gen_CodeOpCast: gen_code__to_strbuilder,         \
	    gen_CodePragma: gen_code__to_strbuilder,         \
	    gen_CodeParams: gen_code__to_strbuilder,         \
	    gen_CodePreprocessCond: gen_code__to_strbuilder, \
	    gen_CodeSpecifiers: gen_code__to_strbuilder,     \
	    gen_CodeStruct: gen_code__to_strbuilder,         \
	    gen_CodeTemplate: gen_code__to_strbuilder,       \
	    gen_CodeTypename: gen_code__to_strbuilder,       \
	    gen_CodeTypedef: gen_code__to_strbuilder,        \
	    gen_CodeUnion: gen_code__to_strbuilder,          \
	    gen_CodeUsing: gen_code__to_strbuilder,          \
	    gen_CodeVar: gen_code__to_strbuilder,            \
	    default: gen_generic_selection_fail              \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_to_strbuilder_ref(code, result)             \
	_Generic(                                                \
	    (code),                                              \
	    gen_Code: gen_code__to_strbuilder_ref,               \
	    gen_CodeBody: gen_code__to_strbuilder_ref,           \
	    gen_CodeAttributes: gen_code__to_strbuilder_ref,     \
	    gen_CodeComment: gen_code__to_strbuilder_ref,        \
	    gen_CodeClass: gen_code__to_strbuilder_ref,          \
	    gen_CodeConstructor: gen_code__to_strbuilder_ref,    \
	    gen_CodeDefine: gen_code__to_strbuilder_ref,         \
	    gen_CodeDefineParams: gen_code__to_strbuilder_ref,   \
	    gen_CodeDestructor: gen_code__to_strbuilder_ref,     \
	    gen_CodeEnum: gen_code__to_strbuilder_ref,           \
	    gen_CodeExec: gen_code__to_strbuilder_ref,           \
	    gen_CodeExtern: gen_code__to_strbuilder_ref,         \
	    gen_CodeInclude: gen_code__to_strbuilder_ref,        \
	    gen_CodeFriend: gen_code__to_strbuilder_ref,         \
	    gen_CodeFn: gen_code__to_strbuilder_ref,             \
	    gen_CodeModule: gen_code__to_strbuilder_ref,         \
	    gen_CodeNS: gen_code__to_strbuilder_ref,             \
	    gen_CodeOperator: gen_code__to_strbuilder_ref,       \
	    gen_CodeOpCast: gen_code__to_strbuilder_ref,         \
	    gen_CodePragma: gen_code__to_strbuilder_ref,         \
	    gen_CodeParams: gen_code__to_strbuilder_ref,         \
	    gen_CodePreprocessCond: gen_code__to_strbuilder_ref, \
	    gen_CodeSpecifiers: gen_code__to_strbuilder_ref,     \
	    gen_CodeStruct: gen_code__to_strbuilder_ref,         \
	    gen_CodeTemplate: gen_code__to_strbuilder_ref,       \
	    gen_CodeTypename: gen_code__to_strbuilder_ref,       \
	    gen_CodeTypedef: gen_code__to_strbuilder_ref,        \
	    gen_CodeUnion: gen_code__to_strbuilder_ref,          \
	    gen_CodeUsing: gen_code__to_strbuilder_ref,          \
	    gen_CodeVar: gen_code__to_strbuilder_ref,            \
	    default: gen_generic_selection_fail                  \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code, result)

#define gen_code_type_str(code)                     \
	_Generic(                                       \
	    (code),                                     \
	    gen_Code: gen_code__type_str,               \
	    gen_CodeBody: gen_code__type_str,           \
	    gen_CodeAttributes: gen_code__type_str,     \
	    gen_CodeComment: gen_code__type_str,        \
	    gen_CodeClass: gen_code__type_str,          \
	    gen_CodeConstructor: gen_code__type_str,    \
	    gen_CodeDefine: gen_code__type_str,         \
	    gen_CodeDefineParams: gen_code__type_str,   \
	    gen_CodeDestructor: gen_code__type_str,     \
	    gen_CodeEnum: gen_code__type_str,           \
	    gen_CodeExec: gen_code__type_str,           \
	    gen_CodeExtern: gen_code__type_str,         \
	    gen_CodeInclude: gen_code__type_str,        \
	    gen_CodeFriend: gen_code__type_str,         \
	    gen_CodeFn: gen_code__type_str,             \
	    gen_CodeModule: gen_code__type_str,         \
	    gen_CodeNS: gen_code__type_str,             \
	    gen_CodeOperator: gen_code__type_str,       \
	    gen_CodeOpCast: gen_code__type_str,         \
	    gen_CodePragma: gen_code__type_str,         \
	    gen_CodeParams: gen_code__type_str,         \
	    gen_CodePreprocessCond: gen_code__type_str, \
	    gen_CodeSpecifiers: gen_code__type_str,     \
	    gen_CodeStruct: gen_code__type_str,         \
	    gen_CodeTemplate: gen_code__type_str,       \
	    gen_CodeTypename: gen_code__type_str,       \
	    gen_CodeTypedef: gen_code__type_str,        \
	    gen_CodeUnion: gen_code__type_str,          \
	    gen_CodeUsing: gen_code__type_str,          \
	    gen_CodeVar: gen_code__type_str,            \
	    default: gen_generic_selection_fail         \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)

#define gen_code_validate_body(code)                     \
	_Generic(                                            \
	    (code),                                          \
	    gen_Code: gen_code__validate_body,               \
	    gen_CodeBody: gen_code__validate_body,           \
	    gen_CodeAttributes: gen_code__validate_body,     \
	    gen_CodeComment: gen_code__validate_body,        \
	    gen_CodeClass: gen_code__validate_body,          \
	    gen_CodeConstructor: gen_code__validate_body,    \
	    gen_CodeDefine: gen_code__validate_body,         \
	    gen_CodeDefineParams: gen_code__validate_body,   \
	    gen_CodeDestructor: gen_code__validate_body,     \
	    gen_CodeEnum: gen_code__validate_body,           \
	    gen_CodeExec: gen_code__validate_body,           \
	    gen_CodeExtern: gen_code__validate_body,         \
	    gen_CodeInclude: gen_code__validate_body,        \
	    gen_CodeFriend: gen_code__validate_body,         \
	    gen_CodeFn: gen_code__validate_body,             \
	    gen_CodeModule: gen_code__validate_body,         \
	    gen_CodeNS: gen_code__validate_body,             \
	    gen_CodeOperator: gen_code__validate_body,       \
	    gen_CodeOpCast: gen_code__validate_body,         \
	    gen_CodePragma: gen_code__validate_body,         \
	    gen_CodeParams: gen_code__validate_body,         \
	    gen_CodePreprocessCond: gen_code__validate_body, \
	    gen_CodeSpecifiers: gen_code__validate_body,     \
	    gen_CodeStruct: gen_code__validate_body,         \
	    gen_CodeTemplate: gen_code__validate_body,       \
	    gen_CodeTypename: gen_code__validate_body,       \
	    gen_CodeTypedef: gen_code__validate_body,        \
	    gen_CodeUnion: gen_code__validate_body,          \
	    gen_CodeUsing: gen_code__validate_body,          \
	    gen_CodeVar: gen_code__validate_body,            \
	    default: gen_generic_selection_fail              \
	) GEN_RESOLVED_FUNCTION_CALL((gen_Code)code)



GEN_API void           gen_body_append(gen_CodeBody body, gen_Code other);
GEN_API void           gen_body_append_body(gen_CodeBody body, gen_CodeBody other);
GEN_API gen_StrBuilder gen_body_to_strbuilder(gen_CodeBody body);
void                   gen_body_to_strbuilder_ref(gen_CodeBody body, gen_StrBuilder* result);
GEN_API void           gen_body_to_strbuilder_export(gen_CodeBody body, gen_StrBuilder* result);

gen_Code gen_begin_CodeBody(gen_CodeBody body);
gen_Code gen_end_CodeBody(gen_CodeBody body);
gen_Code gen_next_CodeBody(gen_CodeBody body, gen_Code entry_iter);

void                   gen_class_add_interface(gen_CodeClass self, gen_CodeTypename interface);
GEN_API gen_StrBuilder gen_class_to_strbuilder(gen_CodeClass self);
GEN_API void           gen_class_to_strbuilder_def(gen_CodeClass self, gen_StrBuilder* result);
GEN_API void           gen_class_to_strbuilder_fwd(gen_CodeClass self, gen_StrBuilder* result);

void                 gen_define_params_append(gen_CodeDefineParams appendee, gen_CodeDefineParams other);
gen_CodeDefineParams gen_define_params_get(gen_CodeDefineParams params, gen_s32 idx);
bool                 gen_define_params_has_entries(gen_CodeDefineParams params);
gen_StrBuilder       gen_define_params_to_strbuilder(gen_CodeDefineParams params);
GEN_API void         gen_define_params_to_strbuilder_ref(gen_CodeDefineParams params, gen_StrBuilder* result);

gen_CodeDefineParams gen_begin_CodeDefineParams(gen_CodeDefineParams params);
gen_CodeDefineParams gen_end_CodeDefineParams(gen_CodeDefineParams params);
gen_CodeDefineParams gen_next_CodeDefineParams(gen_CodeDefineParams params, gen_CodeDefineParams entry_iter);

void           gen_params_append(gen_CodeParams appendee, gen_CodeParams other);
gen_CodeParams gen_params_get(gen_CodeParams params, gen_s32 idx);
bool           gen_params_has_entries(gen_CodeParams params);
gen_StrBuilder gen_params_to_strbuilder(gen_CodeParams params);
GEN_API void   gen_params_to_strbuilder_ref(gen_CodeParams params, gen_StrBuilder* result);

gen_CodeParams gen_begin_CodeParams(gen_CodeParams params);
gen_CodeParams gen_end_CodeParams(gen_CodeParams params);
gen_CodeParams gen_next_CodeParams(gen_CodeParams params, gen_CodeParams entry_iter);

bool           gen_specifiers_append(gen_CodeSpecifiers specifiers, gen_Specifier spec);
bool           gen_specifiers_has(gen_CodeSpecifiers specifiers, gen_Specifier spec);
gen_s32        gen_specifiers_index_of(gen_CodeSpecifiers specifiers, gen_Specifier spec);
gen_s32        gen_specifiers_remove(gen_CodeSpecifiers specifiers, gen_Specifier to_remove);
gen_StrBuilder gen_specifiers_to_strbuilder(gen_CodeSpecifiers specifiers);
GEN_API void   gen_specifiers_to_strbuilder_ref(gen_CodeSpecifiers specifiers, gen_StrBuilder* result);

gen_Specifier* gen_begin_CodeSpecifiers(gen_CodeSpecifiers specifiers);
gen_Specifier* gen_end_CodeSpecifiers(gen_CodeSpecifiers specifiers);
gen_Specifier* gen_next_CodeSpecifiers(gen_CodeSpecifiers specifiers, gen_Specifier* gen_spec_iter);

void                   gen_struct_add_interface(gen_CodeStruct self, gen_CodeTypename interface);
GEN_API gen_StrBuilder gen_struct_to_strbuilder(gen_CodeStruct self);
GEN_API void           gen_struct_to_strbuilder_fwd(gen_CodeStruct self, gen_StrBuilder* result);
GEN_API void           gen_struct_to_strbuilder_def(gen_CodeStruct self, gen_StrBuilder* result);

gen_StrBuilder gen_attributes_to_strbuilder(gen_CodeAttributes attributes);
void           gen_attributes_to_strbuilder_ref(gen_CodeAttributes attributes, gen_StrBuilder* result);

gen_StrBuilder gen_comment_to_strbuilder(gen_CodeComment comment);
void           gen_comment_to_strbuilder_ref(gen_CodeComment comment, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_constructor__to_strbuilder(gen_CodeConstructor gen_constructor_);
GEN_API void           gen_constructor__to_strbuilder_def(gen_CodeConstructor gen_constructor_, gen_StrBuilder* result);
GEN_API void           gen_constructor__to_strbuilder_fwd(gen_CodeConstructor gen_constructor_, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_define_to_strbuilder(gen_CodeDefine self);
GEN_API void           gen_define_to_strbuilder_ref(gen_CodeDefine self, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_destructor__to_strbuilder(gen_CodeDestructor gen_destructor_);
GEN_API void           gen_destructor__to_strbuilder_fwd(gen_CodeDestructor gen_destructor_, gen_StrBuilder* result);
GEN_API void           gen_destructor__to_strbuilder_def(gen_CodeDestructor gen_destructor_, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_enum_to_strbuilder(gen_CodeEnum self);
GEN_API void           gen_enum_to_strbuilder_def(gen_CodeEnum self, gen_StrBuilder* result);
GEN_API void           gen_enum_to_strbuilder_fwd(gen_CodeEnum self, gen_StrBuilder* result);
GEN_API void           gen_enum_to_strbuilder_class_def(gen_CodeEnum self, gen_StrBuilder* result);
GEN_API void           gen_enum_to_strbuilder_class_fwd(gen_CodeEnum self, gen_StrBuilder* result);

gen_StrBuilder gen_exec_to_strbuilder(gen_CodeExec exec);
void           gen_exec_to_strbuilder_ref(gen_CodeExec exec, gen_StrBuilder* result);

void gen_extern_to_strbuilder(gen_CodeExtern self, gen_StrBuilder* result);

gen_StrBuilder gen_include_to_strbuilder(gen_CodeInclude self);
void           gen_include_to_strbuilder_ref(gen_CodeInclude self, gen_StrBuilder* result);

gen_StrBuilder gen_friend_to_strbuilder(gen_CodeFriend self);
void           gen_friend_to_strbuilder_ref(gen_CodeFriend self, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_fn_to_strbuilder(gen_CodeFn self);
GEN_API void           gen_fn_to_strbuilder_def(gen_CodeFn self, gen_StrBuilder* result);
GEN_API void           gen_fn_to_strbuilder_fwd(gen_CodeFn self, gen_StrBuilder* result);

gen_StrBuilder gen_module_to_strbuilder(gen_CodeModule self);
GEN_API void   gen_module_to_strbuilder_ref(gen_CodeModule self, gen_StrBuilder* result);

gen_StrBuilder namespace_to_strbuilder(gen_CodeNS self);
void           namespace_to_strbuilder_ref(gen_CodeNS self, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_code_op_to_strbuilder(gen_CodeOperator self);
GEN_API void           gen_code_op_to_strbuilder_fwd(gen_CodeOperator self, gen_StrBuilder* result);
GEN_API void           gen_code_op_to_strbuilder_def(gen_CodeOperator self, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_opcast_to_strbuilder(gen_CodeOpCast op_cast);
GEN_API void           gen_opcast_to_strbuilder_def(gen_CodeOpCast op_cast, gen_StrBuilder* result);
GEN_API void           gen_opcast_to_strbuilder_fwd(gen_CodeOpCast op_cast, gen_StrBuilder* result);

gen_StrBuilder gen_pragma_to_strbuilder(gen_CodePragma self);
void           gen_pragma_to_strbuilder_ref(gen_CodePragma self, gen_StrBuilder* result);

GEN_API gen_StrBuilder gen_preprocess_to_strbuilder(gen_CodePreprocessCond cond);
void                   gen_preprocess_to_strbuilder_if(gen_CodePreprocessCond cond, gen_StrBuilder* result);
void                   gen_preprocess_to_strbuilder_ifdef(gen_CodePreprocessCond cond, gen_StrBuilder* result);
void                   gen_preprocess_to_strbuilder_ifndef(gen_CodePreprocessCond cond, gen_StrBuilder* result);
void                   gen_preprocess_to_strbuilder_elif(gen_CodePreprocessCond cond, gen_StrBuilder* result);
void                   gen_preprocess_to_strbuilder_else(gen_CodePreprocessCond cond, gen_StrBuilder* result);
void                   gen_preprocess_to_strbuilder_endif(gen_CodePreprocessCond cond, gen_StrBuilder* result);

gen_StrBuilder gen_template_to_strbuilder(gen_CodeTemplate self);
GEN_API void   gen_template_to_strbuilder_ref(gen_CodeTemplate self, gen_StrBuilder* result);

gen_StrBuilder gen_typedef_to_strbuilder(gen_CodeTypedef self);
GEN_API void   gen_typedef_to_strbuilder_ref(gen_CodeTypedef self, gen_StrBuilder* result);

gen_StrBuilder gen_typename_to_strbuilder(gen_CodeTypename self);
GEN_API void   gen_typename_to_strbuilder_ref(gen_CodeTypename self, gen_StrBuilder* result);

GEN_API gen_StrBuilder union_to_strbuilder(gen_CodeUnion self);
GEN_API void           union_to_strbuilder_def(gen_CodeUnion self, gen_StrBuilder* result);
GEN_API void           union_to_strbuilder_fwd(gen_CodeUnion self, gen_StrBuilder* result);

gen_StrBuilder gen_using_to_strbuilder(gen_CodeUsing op_cast);
GEN_API void   gen_using_to_strbuilder_ref(gen_CodeUsing op_cast, gen_StrBuilder* result);
void           gen_using_to_strbuilder_ns(gen_CodeUsing op_cast, gen_StrBuilder* result);

gen_StrBuilder gen_var_to_strbuilder(gen_CodeVar self);
GEN_API void   gen_var_to_strbuilder_ref(gen_CodeVar self, gen_StrBuilder* result);

// TODO(Ed): Move C-Interface inlines here...

#pragma endregion gen_Code Type C - Interface

#pragma region gen_AST Types

/*
  ______   ______  ________      ________
 /      \ /      \|        \    |        \
|  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\\▓▓▓▓▓▓▓▓     \▓▓▓▓▓▓▓▓__    __  ______   ______   _______
| ▓▓__| ▓▓ ▓▓___\▓▓  | ▓▓          | ▓▓  |  \  |  \/      \ /      \ /       \
| ▓▓    ▓▓\▓▓    \   | ▓▓          | ▓▓  | ▓▓  | ▓▓  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\  ▓▓▓▓▓▓▓
| ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\  | ▓▓          | ▓▓  | ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓    ▓▓\▓▓    \
| ▓▓  | ▓▓  \__| ▓▓  | ▓▓          | ▓▓  | ▓▓__/ ▓▓ ▓▓__/ ▓▓ ▓▓▓▓▓▓▓▓_\▓▓▓▓▓▓\
| ▓▓  | ▓▓\▓▓    ▓▓  | ▓▓          | ▓▓   \▓▓    ▓▓ ▓▓    ▓▓\▓▓     \       ▓▓
 \▓▓   \▓▓ \▓▓▓▓▓▓    \▓▓           \▓▓   _\▓▓▓▓▓▓▓ ▓▓▓▓▓▓▓  \▓▓▓▓▓▓▓\▓▓▓▓▓▓▓
                                         |  \__| ▓▓ ▓▓
                                          \▓▓    ▓▓ ▓▓
                                           \▓▓▓▓▓▓ \▓▓
*/

/*
	Show only relevant members of the gen_AST for its type.
	gen_AST* fields are replaced with gen_Code types.
		- Guards assignemnts to gen_AST* fields to ensure the gen_AST is duplicated if assigned to another parent.
*/

struct gen_AST_Body
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
	};

	gen_StrCached Name;
	gen_Code      Front;
	gen_Code      Back;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag)];
	gen_s32       NumEntries;
};
typedef struct gen_AST_Body gen_AST_Body;
gen_static_assert(sizeof(gen_AST_Body) == sizeof(gen_AST), "ERROR: gen_AST_Body is not the same size as gen_AST");

// TODO(Ed): Support chaining attributes (Use parameter linkage pattern)
struct gen_AST_Attributes
{
	union
	{
		char          _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
		gen_StrCached Content;
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Attributes gen_AST_Attributes;
gen_static_assert(sizeof(gen_AST_Attributes) == sizeof(gen_AST), "ERROR: gen_AST_Attributes is not the same size as gen_AST");

#if 0
struct gen_AST_BaseClass
{
union 
{
char _PAD_[ 64 ];  // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

};
gen_StrCached Name;
gen_Code Prev;
gen_Code Next;
gen_Token *  Tok;
gen_Code Parent;
gen_CodeType Type;
char _PAD_UNUSED_[ sizeof(gen_ModuleFlag) + sizeof(gen_u32) ];

};
typedef struct gen_AST_BaseClass gen_AST_BaseClass;
gen_static_assert( sizeof(gen_AST_BaseClass) == sizeof(gen_AST), "ERROR: gen_AST_BaseClass is not the same size as gen_AST");
#endif

struct gen_AST_Comment
{
	union
	{
		char          _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
		gen_StrCached Content;
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Comment gen_AST_Comment;
gen_static_assert(sizeof(gen_AST_Comment) == sizeof(gen_AST), "ERROR: gen_AST_Comment is not the same size as gen_AST");

struct gen_AST_Class
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;    // Only supported by forward declarations
			gen_CodeAttributes Attributes;
			gen_CodeSpecifiers Specs;    // Support for final
			gen_CodeTypename   ParentType;
			char               _PAD_PARAMS_[sizeof(gen_AST*)];
			gen_CodeBody       Body;
			char               _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached    Name;
	gen_CodeTypename Prev;
	gen_CodeTypename Next;
	gen_Token*       Tok;
	gen_Code         Parent;
	gen_CodeType     Type;
	gen_ModuleFlag   ModuleFlags;
	gen_AccessSpec   ParentAccess;
};
typedef struct gen_AST_Class gen_AST_Class;
gen_static_assert(sizeof(gen_AST_Class) == sizeof(gen_AST), "ERROR: gen_AST_Class is not the same size as gen_AST");

struct gen_AST_Constructor
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;    // Only supported by forward declarations
			char               _PAD_PROPERTIES_[sizeof(gen_AST*) * 1];
			gen_CodeSpecifiers Specs;
			gen_Code           InitializerList;
			gen_CodeParams     Params;
			gen_Code           Body;
			char               _PAD_PROPERTIES_2_[sizeof(gen_AST*) * 2];
		};
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Constructor gen_AST_Constructor;
gen_static_assert(sizeof(gen_AST_Constructor) == sizeof(gen_AST), "ERROR: gen_AST_Constructor is not the same size as gen_AST");

struct gen_AST_Define
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			char                 _PAD_PROPERTIES_[sizeof(gen_AST*) * 4];
			gen_CodeDefineParams Params;
			gen_Code             Body;    // Should be completely serialized for now to a: gen_StrCached Content.
			char                 _PAD_PROPERTIES_2_[sizeof(gen_AST*) * 1];
		};
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Define gen_AST_Define;
gen_static_assert(sizeof(gen_AST_Define) == sizeof(gen_AST), "ERROR: gen_AST_Define is not the same size as gen_AST");

struct gen_AST_DefineParams
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
	};

	gen_StrCached        Name;
	gen_CodeDefineParams Last;
	gen_CodeDefineParams Next;
	gen_Token*           Tok;
	gen_Code             Parent;
	gen_CodeType         Type;
	char                 _PAD_UNUSED_[sizeof(gen_ModuleFlag)];
	gen_s32              NumEntries;
};
typedef struct gen_AST_DefineParams gen_AST_DefineParams;
gen_static_assert(sizeof(gen_AST_DefineParams) == sizeof(gen_AST), "ERROR: gen_AST_DefineParams is not the same size as gen_AST");

struct gen_AST_Destructor
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			char               _PAD_PROPERTIES_[sizeof(gen_AST*) * 1];
			gen_CodeSpecifiers Specs;
			char               _PAD_PROPERTIES_2_[sizeof(gen_AST*) * 2];
			gen_Code           Body;
			char               _PAD_PROPERTIES_3_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Destructor gen_AST_Destructor;
gen_static_assert(sizeof(gen_AST_Destructor) == sizeof(gen_AST), "ERROR: gen_AST_Destructor is not the same size as gen_AST");

struct gen_AST_Enum
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			gen_CodeAttributes Attributes;
			char               _PAD_SPEC_[sizeof(gen_AST*)];
			gen_CodeTypename   UnderlyingType;
			gen_Code           UnderlyingTypeMacro;
			gen_CodeBody       Body;
			char               _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	char           _PAD_UNUSED_[sizeof(gen_u32)];
};
typedef struct gen_AST_Enum gen_AST_Enum;
gen_static_assert(sizeof(gen_AST_Enum) == sizeof(gen_AST), "ERROR: gen_AST_Enum is not the same size as gen_AST");

struct gen_AST_Exec
{
	union
	{
		char          _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
		gen_StrCached Content;
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Exec gen_AST_Exec;
gen_static_assert(sizeof(gen_AST_Exec) == sizeof(gen_AST), "ERROR: gen_AST_Exec is not the same size as gen_AST");

struct gen_AST_Extern
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			char         _PAD_PROPERTIES_[sizeof(gen_AST*) * 5];
			gen_CodeBody Body;
			char         _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Extern gen_AST_Extern;
gen_static_assert(sizeof(gen_AST_Extern) == sizeof(gen_AST), "ERROR: gen_AST_Extern is not the same size as gen_AST");

struct gen_AST_Include
{
	union
	{
		char          _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
		gen_StrCached Content;
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Include gen_AST_Include;
gen_static_assert(sizeof(gen_AST_Include) == sizeof(gen_AST), "ERROR: gen_AST_Include is not the same size as gen_AST");

struct gen_AST_Friend
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment InlineCmt;
			char            _PAD_PROPERTIES_[sizeof(gen_AST*) * 4];
			gen_Code        Declaration;
			char            _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Friend gen_AST_Friend;
gen_static_assert(sizeof(gen_AST_Friend) == sizeof(gen_AST), "ERROR: gen_AST_Friend is not the same size as gen_AST");

struct gen_AST_Fn
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			gen_CodeAttributes Attributes;
			gen_CodeSpecifiers Specs;
			gen_CodeTypename   ReturnType;
			gen_CodeParams     Params;
			gen_CodeBody       Body;
			gen_Code           SuffixSpecs;    // Thanks Unreal
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	char           _PAD_UNUSED_[sizeof(gen_u32)];
};
typedef struct gen_AST_Fn gen_AST_Fn;
gen_static_assert(sizeof(gen_AST_Fn) == sizeof(gen_AST), "ERROR: gen_AST_Fn is not the same size as gen_AST");

struct gen_AST_Module
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	char           _PAD_UNUSED_[sizeof(gen_u32)];
};
typedef struct gen_AST_Module gen_AST_Module;
gen_static_assert(sizeof(gen_AST_Module) == sizeof(gen_AST), "ERROR: gen_AST_Module is not the same size as gen_AST");

struct gen_AST_NS
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			char         _PAD_PROPERTIES_[sizeof(gen_AST*) * 5];
			gen_CodeBody Body;
			char         _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	char           _PAD_UNUSED_[sizeof(gen_u32)];
};
typedef struct gen_AST_NS gen_AST_NS;
gen_static_assert(sizeof(gen_AST_NS) == sizeof(gen_AST), "ERROR: gen_AST_NS is not the same size as gen_AST");

struct gen_AST_Operator
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			gen_CodeAttributes Attributes;
			gen_CodeSpecifiers Specs;
			gen_CodeTypename   ReturnType;
			gen_CodeParams     Params;
			gen_CodeBody       Body;
			char               _PAD_PROPERTIES_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	gen_Operator   Op;
};
typedef struct gen_AST_Operator gen_AST_Operator;
gen_static_assert(sizeof(gen_AST_Operator) == sizeof(gen_AST), "ERROR: gen_AST_Operator is not the same size as gen_AST");

struct gen_AST_OpCast
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			char               _PAD_PROPERTIES_[sizeof(gen_AST*)];
			gen_CodeSpecifiers Specs;
			gen_CodeTypename   ValueType;
			char               _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
			gen_CodeBody       Body;
			char               _PAD_PROPERTIES_3_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_OpCast gen_AST_OpCast;
gen_static_assert(sizeof(gen_AST_OpCast) == sizeof(gen_AST), "ERROR: gen_AST_OpCast is not the same size as gen_AST");

struct gen_AST_Params
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			// TODO(Ed): Support attributes for parameters (Some prefix macros can be converted to that...)
			char             _PAD_PROPERTIES_2_[sizeof(gen_AST*) * 3];
			gen_CodeTypename ValueType;
			gen_Code         gen_Macro;
			gen_Code         Value;
			gen_Code         PostNameMacro;    // Thanks Unreal
			// char     _PAD_PROPERTIES_3_[sizeof( gen_AST* )];
		};
	};

	gen_StrCached  Name;
	gen_CodeParams Last;
	gen_CodeParams Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	char           _PAD_UNUSED_[sizeof(gen_ModuleFlag)];
	gen_s32        NumEntries;
};
typedef struct gen_AST_Params gen_AST_Params;
gen_static_assert(sizeof(gen_AST_Params) == sizeof(gen_AST), "ERROR: gen_AST_Params is not the same size as gen_AST");

struct gen_AST_Pragma
{
	union
	{
		char          _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
		gen_StrCached Content;
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_Pragma gen_AST_Pragma;
gen_static_assert(sizeof(gen_AST_Pragma) == sizeof(gen_AST), "ERROR: gen_AST_Pragma is not the same size as gen_AST");

struct gen_AST_PreprocessCond
{
	union
	{
		char          _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
		gen_StrCached Content;
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag) + sizeof(gen_u32)];
};
typedef struct gen_AST_PreprocessCond gen_AST_PreprocessCond;
gen_static_assert(sizeof(gen_AST_PreprocessCond) == sizeof(gen_AST), "ERROR: gen_AST_PreprocessCond is not the same size as gen_AST");

struct gen_AST_Specifiers
{
	gen_Specifier      ArrSpecs[gen_AST_ArrSpecs_Cap];
	gen_CodeSpecifiers NextSpecs;
	gen_StrCached      Name;
	gen_Code           Prev;
	gen_Code           Next;
	gen_Token*         Tok;
	gen_Code           Parent;
	gen_CodeType       Type;
	char               _PAD_UNUSED_[sizeof(gen_ModuleFlag)];
	gen_s32            NumEntries;
};
typedef struct gen_AST_Specifiers gen_AST_Specifiers;
gen_static_assert(sizeof(gen_AST_Specifiers) == sizeof(gen_AST), "ERROR: gen_AST_Specifier is not the same size as gen_AST");

struct gen_AST_Struct
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			gen_CodeAttributes Attributes;
			gen_CodeSpecifiers Specs;    // Support for final
			gen_CodeTypename   ParentType;
			char               _PAD_PARAMS_[sizeof(gen_AST*)];
			gen_CodeBody       Body;
			char               _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached    Name;
	gen_CodeTypename Prev;
	gen_CodeTypename Next;
	gen_Token*       Tok;
	gen_Code         Parent;
	gen_CodeType     Type;
	gen_ModuleFlag   ModuleFlags;
	gen_AccessSpec   ParentAccess;
};
typedef struct gen_AST_Struct gen_AST_Struct;
gen_static_assert(sizeof(gen_AST_Struct) == sizeof(gen_AST), "ERROR: gen_AST_Struct is not the same size as gen_AST");

struct gen_AST_Template
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			char           _PAD_PROPERTIES_[sizeof(gen_AST*) * 4];
			gen_CodeParams Params;
			gen_Code       Declaration;
			char           _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	char           _PAD_UNUSED_[sizeof(gen_u32)];
};
typedef struct gen_AST_Template gen_AST_Template;
gen_static_assert(sizeof(gen_AST_Template) == sizeof(gen_AST), "ERROR: gen_AST_Template is not the same size as gen_AST");

#if 0
// WIP... The type ast is going to become more advanced and lead to a major change to gen_AST design.
struct gen_AST_Type
{
union 
{
char _PAD_[ 64 ];  // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C
struct 
{
char _PAD_INLINE_CMT_[ sizeof(gen_AST*) ];
gen_CodeAttributes Attributes;
gen_CodeSpecifiers Specs;
gen_Code QualifierID;
// gen_CodeTypename ReturnType;      // Only used for function signatures
// gen_CodeParams    Params;          // Only used for function signatures
gen_Code ArrExpr;
// gen_CodeSpecifiers SpecsFuncSuffix; // Only used for function signatures

};

};
gen_StrCached Name;
gen_Code Prev;
gen_Code Next;
gen_Token *  Tok;
gen_Code Parent;
gen_CodeType Type;
char _PAD_UNUSED_[ sizeof(gen_ModuleFlag) ];
gen_b32 IsParamPack;

};
typedef struct gen_AST_Type gen_AST_Type;
gen_static_assert( sizeof(gen_AST_Type) == sizeof(gen_AST), "ERROR: gen_AST_Type is not the same size as gen_AST");
#endif

struct gen_AST_Typename
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			char               _PAD_INLINE_CMT_[sizeof(gen_AST*)];
			gen_CodeAttributes Attributes;
			gen_CodeSpecifiers Specs;
			gen_CodeTypename   ReturnType;    // Only used for function signatures
			gen_CodeParams     Params;        // Only used for function signatures
			gen_Code           ArrExpr;
			gen_CodeSpecifiers SpecsFuncSuffix;    // Only used for function signatures
		};
	};

	gen_StrCached Name;
	gen_Code      Prev;
	gen_Code      Next;
	gen_Token*    Tok;
	gen_Code      Parent;
	gen_CodeType  Type;
	char          _PAD_UNUSED_[sizeof(gen_ModuleFlag)];

	struct
	{
		gen_b16          IsParamPack;    // Used by typename to know if type should be considered a parameter pack.
		gen_ETypenameTag TypeTag;        // Used by typename to keep track of explicitly declared tags for the identifier (enum, struct, union)
	};
};
typedef struct gen_AST_Typename gen_AST_Typename;
gen_static_assert(sizeof(gen_AST_Typename) == sizeof(gen_AST), "ERROR: gen_AST_Type is not the same size as gen_AST");

struct gen_AST_Typedef
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment InlineCmt;
			char            _PAD_PROPERTIES_[sizeof(gen_AST*) * 2];
			gen_Code        UnderlyingType;
			char            _PAD_PROPERTIES_2_[sizeof(gen_AST*) * 3];
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	gen_b32        IsFunction;
};
typedef struct gen_AST_Typedef gen_AST_Typedef;
gen_static_assert(sizeof(gen_AST_Typedef) == sizeof(gen_AST), "ERROR: gen_AST_Typedef is not the same size as gen_AST");

struct gen_AST_Union
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			char               _PAD_INLINE_CMT_[sizeof(gen_AST*)];
			gen_CodeAttributes Attributes;
			char               _PAD_PROPERTIES_[sizeof(gen_AST*) * 3];
			gen_CodeBody       Body;
			char               _PAD_PROPERTIES_2_[sizeof(gen_AST*)];
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	char           _PAD_UNUSED_[sizeof(gen_u32)];
};
typedef struct gen_AST_Union gen_AST_Union;
gen_static_assert(sizeof(gen_AST_Union) == sizeof(gen_AST), "ERROR: gen_AST_Union is not the same size as gen_AST");

struct gen_AST_Using
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			gen_CodeAttributes Attributes;
			char               _PAD_SPECS_[sizeof(gen_AST*)];
			gen_CodeTypename   UnderlyingType;
			char               _PAD_PROPERTIES_[sizeof(gen_AST*) * 3];
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	char           _PAD_UNUSED_[sizeof(gen_u32)];
};
typedef struct gen_AST_Using gen_AST_Using;
gen_static_assert(sizeof(gen_AST_Using) == sizeof(gen_AST), "ERROR: gen_AST_Using is not the same size as gen_AST");

struct gen_AST_Var
{
	union
	{
		char _PAD_[64];    // Had to hardcode _PAD_ because (sizeof(gen_Specifier) * gen_AST_ArrSpecs_Cap + sizeof(gen_AST*)) was 67 bytes in C

		struct
		{
			gen_CodeComment    InlineCmt;
			gen_CodeAttributes Attributes;
			gen_CodeSpecifiers Specs;
			gen_CodeTypename   ValueType;
			gen_Code           BitfieldSize;
			gen_Code           Value;
			gen_CodeVar        NextVar;
		};
	};

	gen_StrCached  Name;
	gen_Code       Prev;
	gen_Code       Next;
	gen_Token*     Tok;
	gen_Code       Parent;
	gen_CodeType   Type;
	gen_ModuleFlag ModuleFlags;
	gen_s32        VarParenthesizedInit;
};
typedef struct gen_AST_Var gen_AST_Var;
gen_static_assert(sizeof(gen_AST_Var) == sizeof(gen_AST), "ERROR: gen_AST_Var is not the same size as gen_AST");

#pragma endregion gen_AST Types

#pragma endregion AST

#pragma region gen_Array_gen_Arena

#define GEN_GENERIC_SLOT_7__array_init            gen_Arena, gen_Array_gen_Arena_init
#define GEN_GENERIC_SLOT_7__array_init_reserve    gen_Arena, gen_Array_gen_Arena_init_reserve
#define GEN_GENERIC_SLOT_7__array_append          gen_Array_gen_Arena, gen_Array_gen_Arena_append
#define GEN_GENERIC_SLOT_7__array_append_items    gen_Array_gen_Arena, gen_Array_gen_Arena_append_items
#define GEN_GENERIC_SLOT_7__array_append_at       gen_Array_gen_Arena, gen_Array_gen_Arena_append_at
#define GEN_GENERIC_SLOT_7__array_append_items_at gen_Array_gen_Arena, gen_Array_gen_Arena_append_items_at
#define GEN_GENERIC_SLOT_7__array_back            gen_Array_gen_Arena, gen_Array_gen_Arena_back
#define GEN_GENERIC_SLOT_7__array_clear           gen_Array_gen_Arena, gen_Array_gen_Arena_clear
#define GEN_GENERIC_SLOT_7__array_fill            gen_Array_gen_Arena, gen_Array_gen_Arena_fill
#define GEN_GENERIC_SLOT_7__array_free            gen_Array_gen_Arena, gen_Array_gen_Arena_free
#define GEN_GENERIC_SLOT_7__array_grow            gen_Array_gen_Arena*, gen_Array_gen_Arena_grow
#define GEN_GENERIC_SLOT_7__array_num             gen_Array_gen_Arena, gen_Array_gen_Arena_num
#define GEN_GENERIC_SLOT_7__array_pop             gen_Array_gen_Arena, gen_Array_gen_Arena_pop
#define GEN_GENERIC_SLOT_7__array_remove_at       gen_Array_gen_Arena, gen_Array_gen_Arena_remove_at
#define GEN_GENERIC_SLOT_7__array_reserve         gen_Array_gen_Arena, gen_Array_gen_Arena_reserve
#define GEN_GENERIC_SLOT_7__array_resize          gen_Array_gen_Arena, gen_Array_gen_Arena_resize
#define GEN_GENERIC_SLOT_7__array_set_capacity    gen_Array_gen_Arena*, gen_Array_gen_Arena_set_capacity

typedef gen_Arena*  gen_Array_gen_Arena;
gen_Array_gen_Arena gen_Array_gen_Arena_init(gen_AllocatorInfo allocator);
gen_Array_gen_Arena gen_Array_gen_Arena_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                gen_Array_gen_Arena_append_array(gen_Array_gen_Arena* self, gen_Array_gen_Arena other);
bool                gen_Array_gen_Arena_append(gen_Array_gen_Arena* self, gen_Arena value);
bool                gen_Array_gen_Arena_append_items(gen_Array_gen_Arena* self, gen_Arena* items, gen_usize item_num);
bool                gen_Array_gen_Arena_append_at(gen_Array_gen_Arena* self, gen_Arena item, gen_usize idx);
bool                gen_Array_gen_Arena_append_items_at(gen_Array_gen_Arena* self, gen_Arena* items, gen_usize item_num, gen_usize idx);
gen_Arena*          gen_Array_gen_Arena_back(gen_Array_gen_Arena self);
void                gen_Array_gen_Arena_clear(gen_Array_gen_Arena self);
bool                gen_Array_gen_Arena_fill(gen_Array_gen_Arena self, gen_usize begin, gen_usize end, gen_Arena value);
void                gen_Array_gen_Arena_free(gen_Array_gen_Arena* self);
bool                gen_Array_gen_Arena_grow(gen_Array_gen_Arena* self, gen_usize min_capacity);
gen_usize           gen_Array_gen_Arena_num(gen_Array_gen_Arena self);
gen_Arena           gen_Array_gen_Arena_pop(gen_Array_gen_Arena self);
void                gen_Array_gen_Arena_remove_at(gen_Array_gen_Arena self, gen_usize idx);
bool                gen_Array_gen_Arena_reserve(gen_Array_gen_Arena* self, gen_usize new_capacity);
bool                gen_Array_gen_Arena_resize(gen_Array_gen_Arena* self, gen_usize num);
bool                gen_Array_gen_Arena_set_capacity(gen_Array_gen_Arena* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_Arena gen_Array_gen_Arena_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_Arena, allocator, initial_size);
}

inline gen_Array_gen_Arena gen_Array_gen_Arena_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_Arena) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_Arena*, header + 1);
}

gen_forceinline bool gen_Array_gen_Arena_append_array(gen_Array_gen_Arena* self, gen_Array_gen_Arena other)
{
	return gen_array_append_items(*self, (gen_Array_gen_Arena)other, gen_Array_gen_Arena_num(other));
}

inline bool gen_Array_gen_Arena_append(gen_Array_gen_Arena* self, gen_Arena value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_Arena_append_items(gen_Array_gen_Arena* self, gen_Arena* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_Arena) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_Arena_append_at(gen_Array_gen_Arena* self, gen_Arena item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_Arena target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_Arena));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_Arena_append_items_at(gen_Array_gen_Arena* self, gen_Arena* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Arena* target = (*self) + idx + item_num;
	gen_Arena* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_Arena));
	gen_mem_copy(src, items, item_num * sizeof(gen_Arena));
	header->Num += item_num;
	return true;
}

inline gen_Arena* gen_Array_gen_Arena_back(gen_Array_gen_Arena self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_Arena_clear(gen_Array_gen_Arena self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_Arena_fill(gen_Array_gen_Arena self, gen_usize begin, gen_usize end, gen_Arena value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_Arena_free(gen_Array_gen_Arena* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_Arena_grow(gen_Array_gen_Arena* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_Arena_num(gen_Array_gen_Arena self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_Arena gen_Array_gen_Arena_pop(gen_Array_gen_Arena self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_Arena result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_Arena_remove_at(gen_Array_gen_Arena self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_Arena) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_Arena_reserve(gen_Array_gen_Arena* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_Arena_resize(gen_Array_gen_Arena* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_Arena_set_capacity(gen_Array_gen_Arena* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_Arena) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_Arena) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_Arena*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_Arena

#pragma region gen_Array_gen_Pool

#define GEN_GENERIC_SLOT_8__array_init            gen_Pool, gen_Array_gen_Pool_init
#define GEN_GENERIC_SLOT_8__array_init_reserve    gen_Pool, gen_Array_gen_Pool_init_reserve
#define GEN_GENERIC_SLOT_8__array_append          gen_Array_gen_Pool, gen_Array_gen_Pool_append
#define GEN_GENERIC_SLOT_8__array_append_items    gen_Array_gen_Pool, gen_Array_gen_Pool_append_items
#define GEN_GENERIC_SLOT_8__array_append_at       gen_Array_gen_Pool, gen_Array_gen_Pool_append_at
#define GEN_GENERIC_SLOT_8__array_append_items_at gen_Array_gen_Pool, gen_Array_gen_Pool_append_items_at
#define GEN_GENERIC_SLOT_8__array_back            gen_Array_gen_Pool, gen_Array_gen_Pool_back
#define GEN_GENERIC_SLOT_8__array_clear           gen_Array_gen_Pool, gen_Array_gen_Pool_clear
#define GEN_GENERIC_SLOT_8__array_fill            gen_Array_gen_Pool, gen_Array_gen_Pool_fill
#define GEN_GENERIC_SLOT_8__array_free            gen_Array_gen_Pool, gen_Array_gen_Pool_free
#define GEN_GENERIC_SLOT_8__array_grow            gen_Array_gen_Pool*, gen_Array_gen_Pool_grow
#define GEN_GENERIC_SLOT_8__array_num             gen_Array_gen_Pool, gen_Array_gen_Pool_num
#define GEN_GENERIC_SLOT_8__array_pop             gen_Array_gen_Pool, gen_Array_gen_Pool_pop
#define GEN_GENERIC_SLOT_8__array_remove_at       gen_Array_gen_Pool, gen_Array_gen_Pool_remove_at
#define GEN_GENERIC_SLOT_8__array_reserve         gen_Array_gen_Pool, gen_Array_gen_Pool_reserve
#define GEN_GENERIC_SLOT_8__array_resize          gen_Array_gen_Pool, gen_Array_gen_Pool_resize
#define GEN_GENERIC_SLOT_8__array_set_capacity    gen_Array_gen_Pool*, gen_Array_gen_Pool_set_capacity

typedef gen_Pool*  gen_Array_gen_Pool;
gen_Array_gen_Pool gen_Array_gen_Pool_init(gen_AllocatorInfo allocator);
gen_Array_gen_Pool gen_Array_gen_Pool_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool               gen_Array_gen_Pool_append_array(gen_Array_gen_Pool* self, gen_Array_gen_Pool other);
bool               gen_Array_gen_Pool_append(gen_Array_gen_Pool* self, gen_Pool value);
bool               gen_Array_gen_Pool_append_items(gen_Array_gen_Pool* self, gen_Pool* items, gen_usize item_num);
bool               gen_Array_gen_Pool_append_at(gen_Array_gen_Pool* self, gen_Pool item, gen_usize idx);
bool               gen_Array_gen_Pool_append_items_at(gen_Array_gen_Pool* self, gen_Pool* items, gen_usize item_num, gen_usize idx);
gen_Pool*          gen_Array_gen_Pool_back(gen_Array_gen_Pool self);
void               gen_Array_gen_Pool_clear(gen_Array_gen_Pool self);
bool               gen_Array_gen_Pool_fill(gen_Array_gen_Pool self, gen_usize begin, gen_usize end, gen_Pool value);
void               gen_Array_gen_Pool_free(gen_Array_gen_Pool* self);
bool               gen_Array_gen_Pool_grow(gen_Array_gen_Pool* self, gen_usize min_capacity);
gen_usize          gen_Array_gen_Pool_num(gen_Array_gen_Pool self);
gen_Pool           gen_Array_gen_Pool_pop(gen_Array_gen_Pool self);
void               gen_Array_gen_Pool_remove_at(gen_Array_gen_Pool self, gen_usize idx);
bool               gen_Array_gen_Pool_reserve(gen_Array_gen_Pool* self, gen_usize new_capacity);
bool               gen_Array_gen_Pool_resize(gen_Array_gen_Pool* self, gen_usize num);
bool               gen_Array_gen_Pool_set_capacity(gen_Array_gen_Pool* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_Pool gen_Array_gen_Pool_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_Pool, allocator, initial_size);
}

inline gen_Array_gen_Pool gen_Array_gen_Pool_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_Pool) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_Pool*, header + 1);
}

gen_forceinline bool gen_Array_gen_Pool_append_array(gen_Array_gen_Pool* self, gen_Array_gen_Pool other)
{
	return gen_array_append_items(*self, (gen_Array_gen_Pool)other, gen_Array_gen_Pool_num(other));
}

inline bool gen_Array_gen_Pool_append(gen_Array_gen_Pool* self, gen_Pool value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_Pool_append_items(gen_Array_gen_Pool* self, gen_Pool* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_Pool) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_Pool_append_at(gen_Array_gen_Pool* self, gen_Pool item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_Pool target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_Pool));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_Pool_append_items_at(gen_Array_gen_Pool* self, gen_Pool* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Pool* target = (*self) + idx + item_num;
	gen_Pool* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_Pool));
	gen_mem_copy(src, items, item_num * sizeof(gen_Pool));
	header->Num += item_num;
	return true;
}

inline gen_Pool* gen_Array_gen_Pool_back(gen_Array_gen_Pool self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_Pool_clear(gen_Array_gen_Pool self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_Pool_fill(gen_Array_gen_Pool self, gen_usize begin, gen_usize end, gen_Pool value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_Pool_free(gen_Array_gen_Pool* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_Pool_grow(gen_Array_gen_Pool* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_Pool_num(gen_Array_gen_Pool self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_Pool gen_Array_gen_Pool_pop(gen_Array_gen_Pool self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_Pool result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_Pool_remove_at(gen_Array_gen_Pool self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_Pool) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_Pool_reserve(gen_Array_gen_Pool* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_Pool_resize(gen_Array_gen_Pool* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_Pool_set_capacity(gen_Array_gen_Pool* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_Pool) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_Pool) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_Pool*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_Pool

#pragma region gen_Array_gen_StrCached

#define GEN_GENERIC_SLOT_2__array_init            gen_StrCached, gen_Array_gen_StrCached_init
#define GEN_GENERIC_SLOT_2__array_init_reserve    gen_StrCached, gen_Array_gen_StrCached_init_reserve
#define GEN_GENERIC_SLOT_2__array_append          gen_Array_gen_StrCached, gen_Array_gen_StrCached_append
#define GEN_GENERIC_SLOT_2__array_append_items    gen_Array_gen_StrCached, gen_Array_gen_StrCached_append_items
#define GEN_GENERIC_SLOT_2__array_append_at       gen_Array_gen_StrCached, gen_Array_gen_StrCached_append_at
#define GEN_GENERIC_SLOT_2__array_append_items_at gen_Array_gen_StrCached, gen_Array_gen_StrCached_append_items_at
#define GEN_GENERIC_SLOT_2__array_back            gen_Array_gen_StrCached, gen_Array_gen_StrCached_back
#define GEN_GENERIC_SLOT_2__array_clear           gen_Array_gen_StrCached, gen_Array_gen_StrCached_clear
#define GEN_GENERIC_SLOT_2__array_fill            gen_Array_gen_StrCached, gen_Array_gen_StrCached_fill
#define GEN_GENERIC_SLOT_2__array_free            gen_Array_gen_StrCached, gen_Array_gen_StrCached_free
#define GEN_GENERIC_SLOT_2__array_grow            gen_Array_gen_StrCached*, gen_Array_gen_StrCached_grow
#define GEN_GENERIC_SLOT_2__array_num             gen_Array_gen_StrCached, gen_Array_gen_StrCached_num
#define GEN_GENERIC_SLOT_2__array_pop             gen_Array_gen_StrCached, gen_Array_gen_StrCached_pop
#define GEN_GENERIC_SLOT_2__array_remove_at       gen_Array_gen_StrCached, gen_Array_gen_StrCached_remove_at
#define GEN_GENERIC_SLOT_2__array_reserve         gen_Array_gen_StrCached, gen_Array_gen_StrCached_reserve
#define GEN_GENERIC_SLOT_2__array_resize          gen_Array_gen_StrCached, gen_Array_gen_StrCached_resize
#define GEN_GENERIC_SLOT_2__array_set_capacity    gen_Array_gen_StrCached*, gen_Array_gen_StrCached_set_capacity

typedef gen_StrCached*  gen_Array_gen_StrCached;
gen_Array_gen_StrCached gen_Array_gen_StrCached_init(gen_AllocatorInfo allocator);
gen_Array_gen_StrCached gen_Array_gen_StrCached_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                    gen_Array_gen_StrCached_append_array(gen_Array_gen_StrCached* self, gen_Array_gen_StrCached other);
bool                    gen_Array_gen_StrCached_append(gen_Array_gen_StrCached* self, gen_StrCached value);
bool                    gen_Array_gen_StrCached_append_items(gen_Array_gen_StrCached* self, gen_StrCached* items, gen_usize item_num);
bool                    gen_Array_gen_StrCached_append_at(gen_Array_gen_StrCached* self, gen_StrCached item, gen_usize idx);
bool                    gen_Array_gen_StrCached_append_items_at(gen_Array_gen_StrCached* self, gen_StrCached* items, gen_usize item_num, gen_usize idx);
gen_StrCached*          gen_Array_gen_StrCached_back(gen_Array_gen_StrCached self);
void                    gen_Array_gen_StrCached_clear(gen_Array_gen_StrCached self);
bool                    gen_Array_gen_StrCached_fill(gen_Array_gen_StrCached self, gen_usize begin, gen_usize end, gen_StrCached value);
void                    gen_Array_gen_StrCached_free(gen_Array_gen_StrCached* self);
bool                    gen_Array_gen_StrCached_grow(gen_Array_gen_StrCached* self, gen_usize min_capacity);
gen_usize               gen_Array_gen_StrCached_num(gen_Array_gen_StrCached self);
gen_StrCached           gen_Array_gen_StrCached_pop(gen_Array_gen_StrCached self);
void                    gen_Array_gen_StrCached_remove_at(gen_Array_gen_StrCached self, gen_usize idx);
bool                    gen_Array_gen_StrCached_reserve(gen_Array_gen_StrCached* self, gen_usize new_capacity);
bool                    gen_Array_gen_StrCached_resize(gen_Array_gen_StrCached* self, gen_usize num);
bool                    gen_Array_gen_StrCached_set_capacity(gen_Array_gen_StrCached* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_StrCached gen_Array_gen_StrCached_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_StrCached, allocator, initial_size);
}

inline gen_Array_gen_StrCached gen_Array_gen_StrCached_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_StrCached) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_StrCached*, header + 1);
}

gen_forceinline bool gen_Array_gen_StrCached_append_array(gen_Array_gen_StrCached* self, gen_Array_gen_StrCached other)
{
	return gen_array_append_items(*self, (gen_Array_gen_StrCached)other, gen_Array_gen_StrCached_num(other));
}

inline bool gen_Array_gen_StrCached_append(gen_Array_gen_StrCached* self, gen_StrCached value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_StrCached_append_items(gen_Array_gen_StrCached* self, gen_StrCached* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_StrCached) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_StrCached_append_at(gen_Array_gen_StrCached* self, gen_StrCached item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_StrCached target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_StrCached));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_StrCached_append_items_at(gen_Array_gen_StrCached* self, gen_StrCached* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_StrCached* target = (*self) + idx + item_num;
	gen_StrCached* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_StrCached));
	gen_mem_copy(src, items, item_num * sizeof(gen_StrCached));
	header->Num += item_num;
	return true;
}

inline gen_StrCached* gen_Array_gen_StrCached_back(gen_Array_gen_StrCached self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_StrCached_clear(gen_Array_gen_StrCached self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_StrCached_fill(gen_Array_gen_StrCached self, gen_usize begin, gen_usize end, gen_StrCached value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_StrCached_free(gen_Array_gen_StrCached* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_StrCached_grow(gen_Array_gen_StrCached* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_StrCached_num(gen_Array_gen_StrCached self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_StrCached gen_Array_gen_StrCached_pop(gen_Array_gen_StrCached self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_StrCached result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_StrCached_remove_at(gen_Array_gen_StrCached self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_StrCached) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_StrCached_reserve(gen_Array_gen_StrCached* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_StrCached_resize(gen_Array_gen_StrCached* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_StrCached_set_capacity(gen_Array_gen_StrCached* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_StrCached) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_StrCached) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_StrCached*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_StrCached

#pragma region MacroTable

#define GEN_GENERIC_SLOT_2__hashtable_init         gen_Macro, MacroTable_init
#define GEN_GENERIC_SLOT_2__hashtable_init_reserve gen_Macro, MacroTable_init_reserve
#define GEN_GENERIC_SLOT_2__hashtable_clear        MacroTable, MacroTable_clear
#define GEN_GENERIC_SLOT_2__hashtable_destroy      MacroTable, MacroTable_destroy
#define GEN_GENERIC_SLOT_2__hashtable_get          MacroTable, MacroTable_get
#define GEN_GENERIC_SLOT_2__hashtable_map          MacroTable, MacroTable_map
#define GEN_GENERIC_SLOT_2__hashtable_map_mut      MacroTable, MacroTable_map_mut
#define GEN_GENERIC_SLOT_2__hashtable_grow         MacroTable*, MacroTable_grow
#define GEN_GENERIC_SLOT_2__hashtable_rehash       MacroTable*, MacroTable_rehash
#define GEN_GENERIC_SLOT_2__hashtable_rehash_fast  MacroTable, MacroTable_rehash_fast
#define GEN_GENERIC_SLOT_2__hashtable_remove_entry MacroTable, MacroTable_remove_entry
#define GEN_GENERIC_SLOT_2__hashtable_set          MacroTable, MacroTable_set
#define GEN_GENERIC_SLOT_2__hashtable_slot         MacroTable, MacroTable_slot

#define GEN_GENERIC_SLOT_2__hashtable__add_entry   MacroTable*, MacroTable__add_entry
#define GEN_GENERIC_SLOT_2__hashtable__find        MacroTable, MacroTable__find
#define GEN_GENERIC_SLOT_2__hashtable__full        MacroTable, MacroTable__full

typedef struct gen_HashTable_gen_Macro MacroTable;
typedef struct gen_HTE_MacroTable      gen_HTE_MacroTable;

struct gen_HTE_MacroTable
{
	gen_u64   Key;
	gen_ssize Next;
	gen_Macro Value;
};

typedef void (*MacroTable_MapProc)(MacroTable self, gen_u64 key, gen_Macro value);
typedef void (*MacroTable_MapMutProc)(MacroTable self, gen_u64 key, gen_Macro* value);

#pragma region gen_Arr_HTE_MacroTable

#define GEN_GENERIC_SLOT_9__array_init            gen_HTE_MacroTable, gen_Arr_HTE_MacroTable_init
#define GEN_GENERIC_SLOT_9__array_init_reserve    gen_HTE_MacroTable, gen_Arr_HTE_MacroTable_init_reserve
#define GEN_GENERIC_SLOT_9__array_append          gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_append
#define GEN_GENERIC_SLOT_9__array_append_items    gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_append_items
#define GEN_GENERIC_SLOT_9__array_append_at       gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_append_at
#define GEN_GENERIC_SLOT_9__array_append_items_at gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_append_items_at
#define GEN_GENERIC_SLOT_9__array_back            gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_back
#define GEN_GENERIC_SLOT_9__array_clear           gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_clear
#define GEN_GENERIC_SLOT_9__array_fill            gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_fill
#define GEN_GENERIC_SLOT_9__array_free            gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_free
#define GEN_GENERIC_SLOT_9__array_grow            gen_Arr_HTE_MacroTable*, gen_Arr_HTE_MacroTable_grow
#define GEN_GENERIC_SLOT_9__array_num             gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_num
#define GEN_GENERIC_SLOT_9__array_pop             gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_pop
#define GEN_GENERIC_SLOT_9__array_remove_at       gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_remove_at
#define GEN_GENERIC_SLOT_9__array_reserve         gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_reserve
#define GEN_GENERIC_SLOT_9__array_resize          gen_Arr_HTE_MacroTable, gen_Arr_HTE_MacroTable_resize
#define GEN_GENERIC_SLOT_9__array_set_capacity    gen_Arr_HTE_MacroTable*, gen_Arr_HTE_MacroTable_set_capacity

typedef gen_HTE_MacroTable* gen_Arr_HTE_MacroTable;
gen_Arr_HTE_MacroTable      gen_Arr_HTE_MacroTable_init(gen_AllocatorInfo allocator);
gen_Arr_HTE_MacroTable      gen_Arr_HTE_MacroTable_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                        gen_Arr_HTE_MacroTable_append_array(gen_Arr_HTE_MacroTable* self, gen_Arr_HTE_MacroTable other);
bool                        gen_Arr_HTE_MacroTable_append(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable value);
bool                        gen_Arr_HTE_MacroTable_append_items(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable* items, gen_usize item_num);
bool                        gen_Arr_HTE_MacroTable_append_at(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable item, gen_usize idx);
bool                        gen_Arr_HTE_MacroTable_append_items_at(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable* items, gen_usize item_num, gen_usize idx);
gen_HTE_MacroTable*         gen_Arr_HTE_MacroTable_back(gen_Arr_HTE_MacroTable self);
void                        gen_Arr_HTE_MacroTable_clear(gen_Arr_HTE_MacroTable self);
bool                        gen_Arr_HTE_MacroTable_fill(gen_Arr_HTE_MacroTable self, gen_usize begin, gen_usize end, gen_HTE_MacroTable value);
void                        gen_Arr_HTE_MacroTable_free(gen_Arr_HTE_MacroTable* self);
bool                        gen_Arr_HTE_MacroTable_grow(gen_Arr_HTE_MacroTable* self, gen_usize min_capacity);
gen_usize                   gen_Arr_HTE_MacroTable_num(gen_Arr_HTE_MacroTable self);
gen_HTE_MacroTable          gen_Arr_HTE_MacroTable_pop(gen_Arr_HTE_MacroTable self);
void                        gen_Arr_HTE_MacroTable_remove_at(gen_Arr_HTE_MacroTable self, gen_usize idx);
bool                        gen_Arr_HTE_MacroTable_reserve(gen_Arr_HTE_MacroTable* self, gen_usize new_capacity);
bool                        gen_Arr_HTE_MacroTable_resize(gen_Arr_HTE_MacroTable* self, gen_usize num);
bool                        gen_Arr_HTE_MacroTable_set_capacity(gen_Arr_HTE_MacroTable* self, gen_usize new_capacity);

gen_forceinline gen_Arr_HTE_MacroTable gen_Arr_HTE_MacroTable_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_HTE_MacroTable, allocator, initial_size);
}

inline gen_Arr_HTE_MacroTable gen_Arr_HTE_MacroTable_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_HTE_MacroTable) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_HTE_MacroTable*, header + 1);
}

gen_forceinline bool gen_Arr_HTE_MacroTable_append_array(gen_Arr_HTE_MacroTable* self, gen_Arr_HTE_MacroTable other)
{
	return gen_array_append_items(*self, (gen_Arr_HTE_MacroTable)other, gen_Arr_HTE_MacroTable_num(other));
}

inline bool gen_Arr_HTE_MacroTable_append(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Arr_HTE_MacroTable_append_items(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_HTE_MacroTable) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Arr_HTE_MacroTable_append_at(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Arr_HTE_MacroTable target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_HTE_MacroTable));
	header->Num++;
	return true;
}

inline bool gen_Arr_HTE_MacroTable_append_items_at(gen_Arr_HTE_MacroTable* self, gen_HTE_MacroTable* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_HTE_MacroTable* target = (*self) + idx + item_num;
	gen_HTE_MacroTable* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_HTE_MacroTable));
	gen_mem_copy(src, items, item_num * sizeof(gen_HTE_MacroTable));
	header->Num += item_num;
	return true;
}

inline gen_HTE_MacroTable* gen_Arr_HTE_MacroTable_back(gen_Arr_HTE_MacroTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Arr_HTE_MacroTable_clear(gen_Arr_HTE_MacroTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Arr_HTE_MacroTable_fill(gen_Arr_HTE_MacroTable self, gen_usize begin, gen_usize end, gen_HTE_MacroTable value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Arr_HTE_MacroTable_free(gen_Arr_HTE_MacroTable* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Arr_HTE_MacroTable_grow(gen_Arr_HTE_MacroTable* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Arr_HTE_MacroTable_num(gen_Arr_HTE_MacroTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_HTE_MacroTable gen_Arr_HTE_MacroTable_pop(gen_Arr_HTE_MacroTable self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_HTE_MacroTable result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Arr_HTE_MacroTable_remove_at(gen_Arr_HTE_MacroTable self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_HTE_MacroTable) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Arr_HTE_MacroTable_reserve(gen_Arr_HTE_MacroTable* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Arr_HTE_MacroTable_resize(gen_Arr_HTE_MacroTable* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Arr_HTE_MacroTable_set_capacity(gen_Arr_HTE_MacroTable* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_HTE_MacroTable) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_HTE_MacroTable) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_HTE_MacroTable*, new_header + 1);
	return true;
}

#pragma endregion gen_Arr_HTE_MacroTable

struct gen_HashTable_gen_Macro
{
	gen_Array_gen_ssize    Hashes;
	gen_Arr_HTE_MacroTable Entries;
};

MacroTable        MacroTable_init(gen_AllocatorInfo allocator);
MacroTable        MacroTable_init_reserve(gen_AllocatorInfo allocator, gen_ssize num);
void              MacroTable_clear(MacroTable self);
void              MacroTable_destroy(MacroTable* self);
gen_Macro*        MacroTable_get(MacroTable self, gen_u64 key);
void              MacroTable_map(MacroTable self, MacroTable_MapProc map_proc);
void              MacroTable_map_mut(MacroTable self, MacroTable_MapMutProc map_proc);
void              MacroTable_grow(MacroTable* self);
void              MacroTable_rehash(MacroTable* self, gen_ssize new_num);
void              MacroTable_rehash_fast(MacroTable self);
void              MacroTable_remove(MacroTable self, gen_u64 key);
void              MacroTable_remove_entry(MacroTable self, gen_ssize idx);
void              MacroTable_set(MacroTable* self, gen_u64 key, gen_Macro value);
gen_ssize         MacroTable_slot(MacroTable self, gen_u64 key);
gen_ssize         MacroTable__add_entry(MacroTable* self, gen_u64 key);
gen_HT_FindResult MacroTable__find(MacroTable self, gen_u64 key);
gen_b32           MacroTable__full(MacroTable self);

MacroTable MacroTable_init(gen_AllocatorInfo allocator)
{
	MacroTable result = gen_hashtable_init_reserve(gen_Macro, allocator, 8);
	return result;
}

MacroTable MacroTable_init_reserve(gen_AllocatorInfo allocator, gen_ssize num)
{
	MacroTable result                        = { 0, 0 };
	result.Hashes                            = gen_array_init_reserve(gen_ssize, allocator, num);
	gen_array_get_header(result.Hashes)->Num = num;
	gen_array_resize(result.Hashes, num);
	gen_array_fill(result.Hashes, 0, num, -1);
	result.Entries = gen_array_init_reserve(gen_HTE_MacroTable, allocator, num);
	return result;
}

void MacroTable_clear(MacroTable self)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_array_clear(self.Entries);
	gen_s32 what = gen_array_num(self.Hashes);
	gen_array_fill(self.Hashes, 0, what, (gen_ssize)-1);
}

void MacroTable_destroy(MacroTable* self)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	if (self->Hashes && gen_array_get_header(self->Hashes)->Capacity)
	{
		gen_array_free(self->Hashes);
		gen_array_free(self->Entries);
	}
}

gen_Macro* MacroTable_get(MacroTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_ssize idx = MacroTable__find(self, key).EntryIndex;
	if (idx > 0)
		return &self.Entries[idx].Value;
	return gen_nullptr;
}

void MacroTable_map(MacroTable self, MacroTable_MapProc map_proc)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	GEN_ASSERT_NOT_NULL(map_proc);
	for (gen_ssize idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
	{
		map_proc(self, self.Entries[idx].Key, self.Entries[idx].Value);
	}
}

void MacroTable_map_mut(MacroTable self, MacroTable_MapMutProc map_proc)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	GEN_ASSERT_NOT_NULL(map_proc);
	for (gen_ssize idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
	{
		map_proc(self, self.Entries[idx].Key, &self.Entries[idx].Value);
	}
}

void MacroTable_grow(MacroTable* self)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	gen_ssize new_num = gen_array_grow_formula(gen_array_get_header(self->Entries)->Num);
	gen_hashtable_rehash(self, new_num);
}

void MacroTable_rehash(MacroTable* self, gen_ssize new_num)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	GEN_ASSERT(new_num > 0);
	gen_ssize        idx;
	gen_ssize        last_added_index;
	gen_ArrayHeader* old_hash_header    = gen_array_get_header(self->Hashes);
	gen_ArrayHeader* old_entries_header = gen_array_get_header(self->Entries);
	MacroTable       new_tbl            = gen_hashtable_init_reserve(gen_Macro, old_hash_header->Allocator, old_hash_header->Num);
	gen_ArrayHeader* new_hash_header    = gen_array_get_header(new_tbl.Hashes);
	for (gen_ssize idx = 0; idx < gen_cast(gen_ssize, old_hash_header->Num); ++idx)
	{
		gen_HTE_MacroTable* entry = &self->Entries[idx];
		gen_HT_FindResult   find_result;
		find_result      = MacroTable__find(new_tbl, entry->Key);
		last_added_index = MacroTable__add_entry(&new_tbl, entry->Key);
		if (find_result.PrevIndex < 0)
			new_tbl.Hashes[find_result.HashIndex] = last_added_index;
		else
			new_tbl.Entries[find_result.PrevIndex].Next = last_added_index;
		new_tbl.Entries[last_added_index].Next  = find_result.EntryIndex;
		new_tbl.Entries[last_added_index].Value = entry->Value;
	}
	MacroTable_destroy(self);
	*self = new_tbl;
}

void MacroTable_rehash_fast(MacroTable self)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_ssize idx;
	for (idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
		self.Entries[idx].Next = -1;
	for (idx = 0; idx < gen_array_get_header(self.Hashes)->Num; idx++)
		self.Hashes[idx] = -1;
	for (idx = 0; idx < gen_array_get_header(self.Entries)->Num; idx++)
	{
		gen_HTE_MacroTable* entry;
		gen_HT_FindResult   find_result;
		entry       = &self.Entries[idx];
		find_result = MacroTable__find(self, entry->Key);
		if (find_result.PrevIndex < 0)
			self.Hashes[find_result.HashIndex] = idx;
		else
			self.Entries[find_result.PrevIndex].Next = idx;
	}
}

void MacroTable_remove(MacroTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_HT_FindResult find_result = MacroTable__find(self, key);
	if (find_result.EntryIndex >= 0)
	{
		gen_array_remove_at(self.Entries, find_result.EntryIndex);
		gen_hashtable_rehash_fast(self);
	}
}

void MacroTable_remove_entry(MacroTable self, gen_ssize idx)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_array_remove_at(self.Entries, idx);
}

void MacroTable_set(MacroTable* self, gen_u64 key, gen_Macro value)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	gen_ssize         idx;
	gen_HT_FindResult find_result;
	if (gen_array_get_header(self->Hashes)->Num == 0)
		gen_hashtable_grow(self);
	find_result = MacroTable__find(*self, key);
	if (find_result.EntryIndex >= 0)
	{
		idx = find_result.EntryIndex;
	}
	else
	{
		idx = MacroTable__add_entry(self, key);
		if (find_result.PrevIndex >= 0)
		{
			self->Entries[find_result.PrevIndex].Next = idx;
		}
		else
		{
			self->Hashes[find_result.HashIndex] = idx;
		}
	}
	self->Entries[idx].Value = value;
	if (MacroTable__full(*self))
		gen_hashtable_grow(self);
}

gen_ssize MacroTable_slot(MacroTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	for (gen_ssize idx = 0; idx < gen_array_get_header(self.Hashes)->Num; ++idx)
		if (self.Hashes[idx] == key)
			return idx;
	return -1;
}

gen_ssize MacroTable__add_entry(MacroTable* self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self);
	GEN_ASSERT_NOT_NULL(self->Hashes);
	GEN_ASSERT_NOT_NULL(self->Entries);
	gen_ssize          idx;
	gen_HTE_MacroTable entry = { key, -1 };
	idx                      = gen_array_get_header(self->Entries)->Num;
	gen_array_append(self->Entries, entry);
	return idx;
}

gen_HT_FindResult MacroTable__find(MacroTable self, gen_u64 key)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_HT_FindResult result      = { -1, -1, -1 };
	gen_ArrayHeader*  hash_header = gen_array_get_header(self.Hashes);
	if (hash_header->Num > 0)
	{
		result.HashIndex  = key % hash_header->Num;
		result.EntryIndex = self.Hashes[result.HashIndex];
		while (result.EntryIndex >= 0)
		{
			if (self.Entries[result.EntryIndex].Key == key)
				break;
			result.PrevIndex  = result.EntryIndex;
			result.EntryIndex = self.Entries[result.EntryIndex].Next;
		}
	}
	return result;
}

gen_b32 MacroTable__full(MacroTable self)
{
	GEN_ASSERT_NOT_NULL(self.Hashes);
	GEN_ASSERT_NOT_NULL(self.Entries);
	gen_ArrayHeader* hash_header    = gen_array_get_header(self.Hashes);
	gen_ArrayHeader* entries_header = gen_array_get_header(self.Entries);
	gen_usize        critical_load  = gen_cast(gen_usize, gen_HashTable_CriticalLoadScale * gen_cast(gen_f32, hash_header->Num));
	gen_b32          result         = entries_header->Num > critical_load;
	return result;
}

#pragma endregion MacroTable

#pragma region Gen Interface

/*
 /      \                       |      \          |  \                      /      \
|  ▓▓▓▓▓▓\ ______  _______       \▓▓▓▓▓▓_______  _| ▓▓_    ______   ______ |  ▓▓▓▓▓▓\ ______   _______  ______
| ▓▓ __\▓▓/      \|       \       | ▓▓ |       \|   ▓▓ \  /      \ /      \| ▓▓_  \▓▓|      \ /       \/      \
| ▓▓|    \  ▓▓▓▓▓▓\ ▓▓▓▓▓▓▓\      | ▓▓ | ▓▓▓▓▓▓▓\\▓▓▓▓▓▓ |  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\ ▓▓ \     \▓▓▓▓▓▓\  ▓▓▓▓▓▓▓  ▓▓▓▓▓▓\
| ▓▓ \▓▓▓▓ ▓▓    ▓▓ ▓▓  | ▓▓      | ▓▓ | ▓▓  | ▓▓ | ▓▓ __| ▓▓    ▓▓ ▓▓   \▓▓ ▓▓▓▓    /      ▓▓ ▓▓     | ▓▓    ▓▓
| ▓▓__| ▓▓ ▓▓▓▓▓▓▓▓ ▓▓  | ▓▓     _| ▓▓_| ▓▓  | ▓▓ | ▓▓|  \ ▓▓▓▓▓▓▓▓ ▓▓     | ▓▓     |  ▓▓▓▓▓▓▓ ▓▓_____| ▓▓▓▓▓▓▓▓
 \▓▓    ▓▓\▓▓     \ ▓▓  | ▓▓    |   ▓▓ \ ▓▓  | ▓▓  \▓▓  ▓▓\▓▓     \ ▓▓     | ▓▓      \▓▓    ▓▓\▓▓     \\▓▓     \
  \▓▓▓▓▓▓  \▓▓▓▓▓▓▓\▓▓   \▓▓     \▓▓▓▓▓▓\▓▓   \▓▓   \▓▓▓▓  \▓▓▓▓▓▓▓\▓▓      \▓▓       \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓ \▓▓▓▓▓▓▓
*/


// Note(Ed): This is subject to heavily change
// with upcoming changes to the library's fallback (default) allocations strategy;
// and major changes to lexer/parser context usage.
struct gen_Context
{
	// User Configuration

	// Persistent Data Allocation
	gen_AllocatorInfo Allocator_DyanmicContainers;    // By default will use a genral slab allocator (TODO(Ed): Currently does not)
	gen_AllocatorInfo Allocator_Pool;                 // By default will use the growing vmem reserve (TODO(Ed): Currently does not)
	gen_AllocatorInfo Allocator_StrCache;             // By default will use a dedicated slab allocator (TODO(Ed): Currently does not)

	// Temporary Allocation
	gen_AllocatorInfo Allocator_Temp;

	// LoggerCallaback* log_callback; // TODO(Ed): Impl user logger callback as an option.

	// Initalization config
	gen_u32 Max_CommentLineLength;    // Used by gen_def_comment
	gen_u32 Max_StrCacheLength;       // Any cached string longer than this is always allocated again.

	gen_u32 InitSize_BuilderBuffer;
	gen_u32 InitSize_CodePoolsArray;
	gen_u32 InitSize_StringArenasArray;

	gen_u32 CodePool_NumBlocks;

	// TODO(Ed): Review these... (No longer needed if using the proper allocation strategy)
	gen_u32 InitSize_LexerTokens;
	gen_u32 SizePer_StringArena;

	gen_u32 InitSize_StrCacheTable;
	gen_u32 InitSize_MacrosTable;

	// TODO(Ed): Symbol Table
	// Keep track of all resolved symbols (naemspaced identifiers)

	// Parser

	// Used by the lexer to persistently treat all these identifiers as preprocessor defines.
	// Populate with strings via gen::gen_cache_str.
	// Functional defines must have format: id( ;at minimum to indicate that the define is only valid with arguments.
	MacroTable Macros;

	// Backend

	// The fallback allocator is utilized if any fo the three above allocators is not specified by the user.
	gen_u32 InitSize_Fallback_Allocator_Bucket_Size;
	gen_Array(gen_Arena) Fallback_AllocatorBuckets;

	gen_StringTable gen_token_fmt_map;

	// gen_Array(gen_Token) LexerTokens;

	gen_Array(gen_Pool) CodePools;
	gen_Array(gen_Arena) StringArenas;

	gen_StringTable StrCache;

	// TODO(Ed): This needs to be just handled by a parser context
	gen_Array(gen_Token) gen_Lexer_Tokens;

	// TODO(Ed): Active parse context vs a parse result need to be separated conceptually
	gen_ParseContext parser;

	// TODO(Ed): Formatting - This will eventually be in a separate struct when in the process of serialization of the builder.
	gen_s32 temp_serialize_indent;
};
typedef struct gen_Context gen_Context;


// TODO(Ed): Eventually this library should opt out of an implicit context for baseline implementation
// This would automatically make it viable for multi-threaded purposes among other things
// An implicit context interface will be provided instead as wrapper procedures as convience.
GEN_API extern gen_Context* gen__ctx;

// Initialize the library. There first ctx initialized must exist for lifetime of other contextes that come after as its the one that
GEN_API void gen_init(gen_Context* ctx);

// Currently manually free's the arenas, code for checking for leaks.
// However on Windows at least, it doesn't need to occur as the OS will clean up after the process.
GEN_API void gen_deinit(gen_Context* ctx);

// Retrieves the active context (not usually needed, but here in case...)
GEN_API gen_Context* gen_get_context();

// Clears the allocations, but doesn't free the memoery, then calls gen_init() again.
// Ease of use.
GEN_API void gen_reset(gen_Context* ctx);

GEN_API void set_context(gen_Context* ctx);

// Mostly intended for the parser
GEN_API gen_Macro* lookup_macro(gen_Str Name);

// Alternative way to add a preprocess define entry for the lexer & parser to utilize
// if the user doesn't want to use gen_def_define
// Macros are tracked by name so if the name already exists the entry will be overwritten.
GEN_API void gen_register_macro(gen_Macro macro);

// Ease of use batch registration
GEN_API void gen_register_macros(gen_s32 num, ...);
GEN_API void gen_register_macros_arr(gen_s32 num, gen_Macro* macros);


// Used internally to retrive or make string allocations.
// Strings are stored in a series of string arenas of fixed size (SizePer_StringArena)
GEN_API gen_StrCached gen_cache_str(gen_Str str);

/*
	This provides a fresh gen_Code gen_AST.
	The gen interface use this as their method from getting a new gen_AST object from the CodePool.
	Use this if you want to make your own API for formatting the supported gen_Code Types.
*/
GEN_API gen_Code gen_make_code();

// Set these before calling gen's gen_init() procedure.

#pragma region Upfront


GEN_API gen_CodeAttributes gen_def_attributes(gen_Str content);
GEN_API gen_CodeComment    gen_def_comment(gen_Str content);

struct gen_Opts_def_struct
{
	gen_CodeBody       body;
	gen_CodeTypename   parent;
	gen_AccessSpec     parent_access;
	gen_CodeAttributes attributes;
	gen_CodeTypename*  interfaces;
	gen_s32            num_interfaces;
	gen_CodeSpecifiers specifiers;    // Only used for final specifier for now.
	gen_ModuleFlag     mflags;
};
typedef struct gen_Opts_def_struct gen_Opts_def_struct;

GEN_API gen_CodeClass gen_def__class(gen_Str name, gen_Opts_def_struct* opts GEN_PARAM_DEFAULT);
#define gen_def_class(name, ...) gen_def__class(name, &(gen_Opts_def_struct) { __VA_ARGS__ })

struct gen_Opts_def_constructor
{
	gen_CodeParams params;
	gen_Code       initializer_list;
	gen_Code       body;
};
typedef struct gen_Opts_def_constructor gen_Opts_def_constructor;

GEN_API gen_CodeConstructor gen_def__constructor(gen_Opts_def_constructor* opts);
#define gen_def_constructor(...) gen_def__constructor(&(gen_Opts_def_constructor) { __VA_ARGS__ })

struct gen_Opts_def_define
{
	gen_CodeDefineParams params;
	gen_Str              content;
	gen_MacroFlags       flags;
	gen_b32              dont_register_to_preprocess_macros;
};
typedef struct gen_Opts_def_define gen_Opts_def_define;

GEN_API gen_CodeDefine gen_def__define(gen_Str name, gen_MacroType type, gen_Opts_def_define* opts GEN_PARAM_DEFAULT);
#define gen_def_define(name, type, ...) gen_def__define(name, type, &(gen_Opts_def_define) { __VA_ARGS__ })

struct gen_Opts_def_destructor
{
	gen_Code           body;
	gen_CodeSpecifiers specifiers;
};
typedef struct gen_Opts_def_destructor gen_Opts_def_destructor;

GEN_API gen_CodeDestructor gen_def__destructor(gen_Opts_def_destructor* opts);
#define gen_def_destructor(...) gen_def__destructor(&(gen_Opts_def_destructor) { __VA_ARGS__ })

struct gen_Opts_def_enum
{
	gen_CodeBody       body;
	gen_CodeTypename   type;
	EnumT              specifier;
	gen_CodeAttributes attributes;
	gen_ModuleFlag     mflags;
	gen_Code           type_macro;
};
typedef struct gen_Opts_def_enum gen_Opts_def_enum;

GEN_API gen_CodeEnum gen_def__enum(gen_Str name, gen_Opts_def_enum* opts GEN_PARAM_DEFAULT);
#define gen_def_enum(name, ...) gen_def__enum(name, &(gen_Opts_def_enum) { __VA_ARGS__ })

GEN_API gen_CodeExec   gen_def_execution(gen_Str content);
GEN_API gen_CodeExtern gen_def_extern_link(gen_Str name, gen_CodeBody body);
GEN_API gen_CodeFriend gen_def_friend(gen_Code code);

struct gen_Opts_def_function
{
	gen_CodeParams     params;
	gen_CodeTypename   ret_type;
	gen_CodeBody       body;
	gen_CodeSpecifiers specs;
	gen_CodeAttributes attrs;
	gen_ModuleFlag     mflags;
};
typedef struct gen_Opts_def_function gen_Opts_def_function;

GEN_API gen_CodeFn gen_def__function(gen_Str name, gen_Opts_def_function* opts GEN_PARAM_DEFAULT);
#define gen_def_function(name, ...) gen_def__function(name, &(gen_Opts_def_function) { __VA_ARGS__ })

struct gen_Opts_def_include
{
	gen_b32 foreign;
};
typedef struct gen_Opts_def_include gen_Opts_def_include;

struct gen_Opts_def_module
{
	gen_ModuleFlag mflags;
};
typedef struct gen_Opts_def_module gen_Opts_def_module;

struct gen_Opts_def_namespace
{
	gen_ModuleFlag mflags;
};
typedef struct gen_Opts_def_namespace gen_Opts_def_namespace;

GEN_API gen_CodeInclude gen_def__include(gen_Str content, gen_Opts_def_include* opts GEN_PARAM_DEFAULT);
#define gen_def_include(content, ...) gen_def__include(content, &(gen_Opts_def_include) { __VA_ARGS__ })

GEN_API gen_CodeModule gen_def__module(gen_Str name, gen_Opts_def_module* opts GEN_PARAM_DEFAULT);
#define gen_def_module(name, ...) gen_def__module(name, &(gen_Opts_def_module) { __VA_ARGS__ })

GEN_API gen_CodeNS gen_def__namespace(gen_Str name, gen_CodeBody body, gen_Opts_def_namespace* opts GEN_PARAM_DEFAULT);
#define gen_def_namespace(name, body, ...) gen_def__namespace(name, body, &(gen_Opts_def_namespace) { __VA_ARGS__ })

struct gen_Opts_def_operator
{
	gen_CodeParams     params;
	gen_CodeTypename   ret_type;
	gen_CodeBody       body;
	gen_CodeSpecifiers specifiers;
	gen_CodeAttributes attributes;
	gen_ModuleFlag     mflags;
};
typedef struct gen_Opts_def_operator gen_Opts_def_operator;

GEN_API gen_CodeOperator gen_def__operator(gen_Operator op, gen_Str nspace, gen_Opts_def_operator* opts GEN_PARAM_DEFAULT);
#define gen_def_operator(op, nspace, ...) gen_def__operator(op, nspace, &(gen_Opts_def_operator) { __VA_ARGS__ })

struct gen_Opts_def_operator_cast
{
	gen_CodeBody       body;
	gen_CodeSpecifiers specs;
};
typedef struct gen_Opts_def_operator_cast gen_Opts_def_operator_cast;

GEN_API gen_CodeOpCast gen_def__operator_cast(gen_CodeTypename type, gen_Opts_def_operator_cast* opts GEN_PARAM_DEFAULT);
#define gen_def_operator_cast(type, ...) gen_def__operator_cast(type, &(gen_Opts_def_operator_cast) { __VA_ARGS__ })

struct gen_Opts_def_param
{
	gen_Code value;
};
typedef struct gen_Opts_def_param gen_Opts_def_param;

GEN_API gen_CodeParams gen_def__param(gen_CodeTypename type, gen_Str name, gen_Opts_def_param* opts GEN_PARAM_DEFAULT);
#define gen_def_param(type, name, ...) gen_def__param(type, name, &(gen_Opts_def_param) { __VA_ARGS__ })

GEN_API gen_CodePragma gen_def_pragma(gen_Str directive);

GEN_API gen_CodePreprocessCond gen_def_preprocess_cond(gen_EPreprocessCOnd type, gen_Str content);

GEN_API gen_CodeSpecifiers gen_def_specifier(gen_Specifier specifier);

GEN_API gen_CodeStruct gen_def__struct(gen_Str name, gen_Opts_def_struct* opts GEN_PARAM_DEFAULT);
#define gen_def_struct(name, ...) gen_def__struct(name, &(gen_Opts_def_struct) { __VA_ARGS__ })

struct gen_Opts_def_template
{
	gen_ModuleFlag mflags;
};
typedef struct gen_Opts_def_template gen_Opts_def_template;

GEN_API gen_CodeTemplate gen_def__template(gen_CodeParams params, gen_Code definition, gen_Opts_def_template* opts GEN_PARAM_DEFAULT);
#define gen_def_template(params, definition, ...) gen_def__template(params, definition, &(gen_Opts_def_template) { __VA_ARGS__ })

struct gen_Opts_def_type
{
	gen_ETypenameTag   type_tag;
	gen_Code           gen_array_expr;
	gen_CodeSpecifiers specifiers;
	gen_CodeAttributes attributes;
};
typedef struct gen_Opts_def_type gen_Opts_def_type;

GEN_API gen_CodeTypename gen_def__type(gen_Str name, gen_Opts_def_type* opts GEN_PARAM_DEFAULT);
#define gen_def_type(name, ...) gen_def__type(name, &(gen_Opts_def_type) { __VA_ARGS__ })

struct gen_Opts_def_typedef
{
	gen_CodeAttributes attributes;
	gen_ModuleFlag     mflags;
};
typedef struct gen_Opts_def_typedef gen_Opts_def_typedef;

GEN_API gen_CodeTypedef gen_def__typedef(gen_Str name, gen_Code type, gen_Opts_def_typedef* opts GEN_PARAM_DEFAULT);
#define gen_def_typedef(name, type, ...) gen_def__typedef(name, type, &(gen_Opts_def_typedef) { __VA_ARGS__ })

struct gen_Opts_def_union
{
	gen_CodeAttributes attributes;
	gen_ModuleFlag     mflags;
};
typedef struct gen_Opts_def_union gen_Opts_def_union;

GEN_API gen_CodeUnion gen_def__union(gen_Str name, gen_CodeBody body, gen_Opts_def_union* opts GEN_PARAM_DEFAULT);
#define gen_def_union(name, body, ...) gen_def__union(name, body, &(gen_Opts_def_union) { __VA_ARGS__ })

struct gen_Opts_def_using
{
	gen_CodeAttributes attributes;
	gen_ModuleFlag     mflags;
};
typedef struct gen_Opts_def_using gen_Opts_def_using;

GEN_API gen_CodeUsing gen_def__using(gen_Str name, gen_CodeTypename type, gen_Opts_def_using* opts GEN_PARAM_DEFAULT);
#define gen_def_using(name, type, ...) gen_def__using(name, type, &(gen_Opts_def_using) { __VA_ARGS__ })

GEN_API gen_CodeUsing gen_def_using_namespace(gen_Str name);

struct gen_Opts_def_variable
{
	gen_Code           value;
	gen_CodeSpecifiers specifiers;
	gen_CodeAttributes attributes;
	gen_ModuleFlag     mflags;
};
typedef struct gen_Opts_def_variable gen_Opts_def_variable;

GEN_API gen_CodeVar gen_def__variable(gen_CodeTypename type, gen_Str name, gen_Opts_def_variable* opts GEN_PARAM_DEFAULT);
#define gen_def_variable(type, name, ...) gen_def__variable(type, name, &(gen_Opts_def_variable) { __VA_ARGS__ })

// Constructs an empty body. Use gen_AST::validate_body() to check if the body is was has valid entries.
gen_CodeBody gen_def_body(gen_CodeType type);

// There are two options for defining a struct body, either varadically provided with the args macro to auto-deduce the arg num,
/// or provide as an array of gen_Code objects.

GEN_API gen_CodeBody         gen_def_class_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_class_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeDefineParams gen_def_define_params(gen_s32 num, ...);
GEN_API gen_CodeDefineParams gen_def_define_params_arr(gen_s32 num, gen_CodeDefineParams* codes);
GEN_API gen_CodeBody         gen_def_enum_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_enum_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeBody         gen_def_export_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_export_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeBody         gen_def_extern_link_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_extern_link_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeBody         gen_def_function_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_function_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeBody         gen_def_global_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_global_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeBody         gen_def_namespace_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_namespace_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeParams       gen_def_params(gen_s32 num, ...);
GEN_API gen_CodeParams       gen_def_params_arr(gen_s32 num, gen_CodeParams* params);
GEN_API gen_CodeSpecifiers   gen_def_specifiers(gen_s32 num, ...);
GEN_API gen_CodeSpecifiers   gen_def_specifiers_arr(gen_s32 num, gen_Specifier* specs);
GEN_API gen_CodeBody         gen_def_struct_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_struct_body_arr(gen_s32 num, gen_Code* codes);
GEN_API gen_CodeBody         gen_def_union_body(gen_s32 num, ...);
GEN_API gen_CodeBody         gen_def_union_body_arr(gen_s32 num, gen_Code* codes);


#pragma endregion Upfront


#pragma region Parsing



GEN_API gen_CodeClass       gen_parse_class(gen_Str gen_class_def);
GEN_API gen_CodeConstructor gen_parse_constructor(gen_Str gen_constructor__def);
GEN_API gen_CodeDefine      gen_parse_define(gen_Str gen_define_def);
GEN_API gen_CodeDestructor  gen_parse_destructor(gen_Str gen_destructor__def);
GEN_API gen_CodeEnum        gen_parse_enum(gen_Str gen_enum_def);
GEN_API gen_CodeBody        gen_parse_export_body(gen_Str export_def);
GEN_API gen_CodeExtern      gen_parse_extern_link(gen_Str exten_link_def);
GEN_API gen_CodeFriend      gen_parse_friend(gen_Str gen_friend_def);
GEN_API gen_CodeFn          gen_parse_function(gen_Str gen_fn_def);
GEN_API gen_CodeBody        gen_parse_global_body(gen_Str gen_body_def);
GEN_API gen_CodeNS          gen_parse_namespace(gen_Str namespace_def);
GEN_API gen_CodeOperator    gen_parse_operator(gen_Str operator_def);
GEN_API gen_CodeOpCast      gen_parse_operator_cast(gen_Str operator_def);
GEN_API gen_CodeStruct      gen_parse_struct(gen_Str gen_struct_def);
GEN_API gen_CodeTemplate    gen_parse_template(gen_Str gen_template_def);
GEN_API gen_CodeTypename    gen_parse_type(gen_Str type_def);
GEN_API gen_CodeTypedef     gen_parse_typedef(gen_Str gen_typedef_def);
GEN_API gen_CodeUnion       gen_parse_union(gen_Str union_def);
GEN_API gen_CodeUsing       gen_parse_using(gen_Str gen_using_def);
GEN_API gen_CodeVar         gen_parse_variable(gen_Str gen_var_def);

#pragma endregion Parsing


#pragma region Untyped text


GEN_API gen_ssize gen_token_fmt_va(char* buf, gen_usize buf_size, gen_s32 num_tokens, va_list va);
//! Do not use directly. Use the gen_token_fmt macro instead.
gen_Str gen_token_fmt_impl(gen_ssize, ...);

GEN_API gen_Code gen_untyped_str(gen_Str content);
GEN_API gen_Code gen_untyped_fmt(char const* fmt, ...);
GEN_API gen_Code gen_untyped_token_fmt(gen_s32 num_tokens, char const* fmt, ...);

#pragma endregion Untyped text


#pragma region Macros


#ifndef gen_main
#define gen_main main
#endif

#ifndef name
//	Convienence for defining any name used with the gen api.
//  Lets you provide the length and string literal to the functions without the need for the DSL.
#if GEN_COMPILER_C
#define name(Id_)                                          \
	(gen_Str)                                              \
	{                                                      \
		gen_stringize(Id_), sizeof(gen_stringize(Id_)) - 1 \
	}
#else
#define name(Id_)                                          \
	gen_Str                                                \
	{                                                      \
		gen_stringize(Id_), sizeof(gen_stringize(Id_)) - 1 \
	}
#endif
#endif

#ifndef code
//  Same as name just used to indicate intention of literal for code instead of names.
#if GEN_COMPILER_C
#define code(...)                                                          \
	(gen_Str)                                                              \
	{                                                                      \
		gen_stringize(__VA_ARGS__), sizeof(gen_stringize(__VA_ARGS__)) - 1 \
	}
#else
#define code(...)                                                          \
	gen_Str                                                                \
	{                                                                      \
		gen_stringize(__VA_ARGS__), sizeof(gen_stringize(__VA_ARGS__)) - 1 \
	}
#endif
#endif

#ifndef args
// Provides the number of arguments while passing args inplace.
#define args(...) gen_num_args(__VA_ARGS__), __VA_ARGS__
#endif

#ifndef gen_code_str
// Just wrappers over common untyped code definition constructions.
#define gen_code_str(...) GEN_NS gen_untyped_str(code(__VA_ARGS__))
#endif

#ifndef gen_code_fmt
#define gen_code_fmt(...) GEN_NS gen_untyped_str(gen_token_fmt(__VA_ARGS__))
#endif

#ifndef gen_parse_fmt
#define gen_parse_fmt(type, ...) GEN_NS gen_parse_##type(gen_token_fmt(__VA_ARGS__))
#endif

#ifndef gen_token_fmt
/*
Takes a format string (char const*) and a list of tokens (gen_Str) and returns a gen_Str of the formatted string.
Tokens are provided in '<'identifier'>' format where '<' '>' are just angle brackets (you can change it in gen_token_fmt_va)
---------------------------------------------------------
	Example - A string with:
		typedef <type> <name> <name>;
	Will have a gen_token_fmt arguments populated with:
		"type", gen_str_for_type,
		"name", gen_str_for_name,
	and:
		gen_stringize( typedef <type> <name> <name>; )
-----------------------------------------------------------
So the full call for this example would be:
	gen_token_fmt(
		"type", gen_str_for_type
	,	"name", gen_str_for_name
	,	gen_stringize(
		typedef <type> <name> <name>
	));
!----------------------------------------------------------
! Note: gen_token_fmt_va is whitespace sensitive for the tokens.
! This can be alleviated by skipping whitespace between brackets but it was choosen to not have that implementation by default.
*/
#define gen_token_fmt(...) GEN_NS gen_token_fmt_impl((gen_num_args(__VA_ARGS__) + 1) / 2, __VA_ARGS__)
#endif

#pragma endregion Macros


#pragma endregion Gen Interface

#pragma region Constants

// Predefined typename codes. Are set to readonly and are setup during gen::gen_init()

GEN_API extern gen_Macro gen_enum_underlying_macro;

GEN_API extern gen_Code gen_access_public;
GEN_API extern gen_Code gen_access_protected;
GEN_API extern gen_Code gen_access_private;

GEN_API extern gen_CodeAttributes gen_attrib_api_export;
GEN_API extern gen_CodeAttributes gen_attrib_api_import;

GEN_API extern gen_Code gen_module_global_fragment;
GEN_API extern gen_Code gen_module_private_fragment;

GEN_API extern gen_Code gen_fmt_newline;

GEN_API extern gen_CodePragma gen_pragma_once;

GEN_API extern gen_CodeParams gen_param_varadic;

GEN_API extern gen_CodePreprocessCond gen_preprocess_else;
GEN_API extern gen_CodePreprocessCond gen_preprocess_endif;

GEN_API extern gen_CodeSpecifiers gen_spec_const;
GEN_API extern gen_CodeSpecifiers gen_spec_consteval;
GEN_API extern gen_CodeSpecifiers gen_spec_constexpr;
GEN_API extern gen_CodeSpecifiers gen_spec_constinit;
GEN_API extern gen_CodeSpecifiers gen_spec_extern_linkage;
GEN_API extern gen_CodeSpecifiers gen_spec_final;
GEN_API extern gen_CodeSpecifiers gen_spec_forceinline;
GEN_API extern gen_CodeSpecifiers gen_spec_global;
GEN_API extern gen_CodeSpecifiers gen_spec_inline;
GEN_API extern gen_CodeSpecifiers gen_spec_internal_linkage;
GEN_API extern gen_CodeSpecifiers gen_spec_local_persist;
GEN_API extern gen_CodeSpecifiers gen_spec_mutable;
GEN_API extern gen_CodeSpecifiers gen_spec_neverinline;
GEN_API extern gen_CodeSpecifiers gen_spec_noexcept;
GEN_API extern gen_CodeSpecifiers gen_spec_override;
GEN_API extern gen_CodeSpecifiers gen_spec_ptr;
GEN_API extern gen_CodeSpecifiers gen_spec_pure;
GEN_API extern gen_CodeSpecifiers gen_spec_ref;
GEN_API extern gen_CodeSpecifiers gen_spec_register;
GEN_API extern gen_CodeSpecifiers gen_spec_rvalue;
GEN_API extern gen_CodeSpecifiers gen_spec_static_member;
GEN_API extern gen_CodeSpecifiers gen_spec_thread_local;
GEN_API extern gen_CodeSpecifiers gen_spec_virtual;
GEN_API extern gen_CodeSpecifiers gen_spec_volatile;

GEN_API extern gen_CodeTypename gen_t_empty;    // Used with varaidc parameters. (Exposing just in case its useful for another circumstance)
GEN_API extern gen_CodeTypename gen_t_auto;
GEN_API extern gen_CodeTypename gen_t_void;
GEN_API extern gen_CodeTypename gen_t_int;
GEN_API extern gen_CodeTypename gen_t_bool;
GEN_API extern gen_CodeTypename gen_t_char;
GEN_API extern gen_CodeTypename gen_t_wchar_t;
GEN_API extern gen_CodeTypename gen_t_class;
GEN_API extern gen_CodeTypename gen_t_typename;

#ifdef GEN_DEFINE_LIBRARY_CODE_CONSTANTS

GEN_API extern gen_CodeTypename gen_t_b32;

GEN_API extern gen_CodeTypename gen_t_s8;
GEN_API extern gen_CodeTypename gen_t_s16;
GEN_API extern gen_CodeTypename gen_t_s32;
GEN_API extern gen_CodeTypename gen_t_s64;

GEN_API extern gen_CodeTypename gen_t_u8;
GEN_API extern gen_CodeTypename gen_t_u16;
GEN_API extern gen_CodeTypename gen_t_u32;
GEN_API extern gen_CodeTypename gen_t_u64;

GEN_API extern gen_CodeTypename gen_t_ssize;
GEN_API extern gen_CodeTypename gen_t_usize;

GEN_API extern gen_CodeTypename gen_t_f32;
GEN_API extern gen_CodeTypename gen_t_f64;
#endif

#pragma endregion Constants

#pragma region Inlines

#pragma region Serialization

inline gen_StrBuilder gen_attributes_to_strbuilder(gen_CodeAttributes attributes)
{
	GEN_ASSERT(attributes);
	char*          raw    = gen_ccast(char*, gen_str_duplicate(attributes->Content, gen_get_context()->Allocator_Temp).Ptr);
	gen_StrBuilder result = { raw };
	return result;
}

inline void gen_attributes_to_strbuilder_ref(gen_CodeAttributes attributes, gen_StrBuilder* result)
{
	GEN_ASSERT(attributes);
	GEN_ASSERT(result);
	gen_strbuilder_append_str(result, attributes->Content);
}

inline gen_StrBuilder gen_comment_to_strbuilder(gen_CodeComment comment)
{
	GEN_ASSERT(comment);
	char*          raw    = gen_ccast(char*, gen_str_duplicate(comment->Content, gen_get_context()->Allocator_Temp).Ptr);
	gen_StrBuilder result = { raw };
	return result;
}

inline void gen_body_to_strbuilder_ref(gen_CodeBody body, gen_StrBuilder* result)
{
	GEN_ASSERT(body != gen_nullptr);
	GEN_ASSERT(result != gen_nullptr);
	gen_Code curr = body->Front;
	gen_s32  left = body->NumEntries;
	while (left--)
	{
		gen_code_to_strbuilder_ref(curr, result);
		// gen_strbuilder_append_fmt( result, "%SB", gen_code_to_strbuilder(curr) );
		curr = curr->Next;
	}
}

inline void gen_comment_to_strbuilder_ref(gen_CodeComment comment, gen_StrBuilder* result)
{
	GEN_ASSERT(comment);
	GEN_ASSERT(result);
	gen_strbuilder_append_str(result, comment->Content);
}

inline gen_StrBuilder gen_define_to_strbuilder(gen_CodeDefine define)
{
	GEN_ASSERT(define);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 512);
	gen_define_to_strbuilder_ref(define, &result);
	return result;
}

inline gen_StrBuilder gen_define_params_to_strbuilder(gen_CodeDefineParams params)
{
	GEN_ASSERT(params);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 128);
	gen_define_params_to_strbuilder_ref(params, &result);
	return result;
}

inline gen_StrBuilder gen_exec_to_strbuilder(gen_CodeExec exec)
{
	GEN_ASSERT(exec);
	char*          raw    = gen_ccast(char*, gen_str_duplicate(exec->Content, gen__ctx->Allocator_Temp).Ptr);
	gen_StrBuilder result = { raw };
	return result;
}

inline void gen_exec_to_strbuilder_ref(gen_CodeExec exec, gen_StrBuilder* result)
{
	GEN_ASSERT(exec);
	GEN_ASSERT(result);
	gen_strbuilder_append_str(result, exec->Content);
}

inline void gen_extern_to_strbuilder(gen_CodeExtern self, gen_StrBuilder* result)
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if (self->Body)
		gen_strbuilder_append_fmt(result, "extern \"%S\"\n{\n%SB\n}\n", self->Name, gen_body_to_strbuilder(self->Body));
	else
		gen_strbuilder_append_fmt(result, "extern \"%S\"\n{}\n", self->Name);
}

inline gen_StrBuilder gen_friend_to_strbuilder(gen_CodeFriend self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 256);
	gen_friend_to_strbuilder_ref(self, &result);
	return result;
}

inline void gen_friend_to_strbuilder_ref(gen_CodeFriend self, gen_StrBuilder* result)
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	gen_strbuilder_append_fmt(result, "friend %SB", gen_code_to_strbuilder(self->Declaration));

	if (self->Declaration->Type != CT_Function && self->Declaration->Type != CT_Operator && (*result)[gen_strbuilder_length(*result) - 1] != ';')
	{
		gen_strbuilder_append_str(result, gen_txt(";"));
	}

	if (self->InlineCmt)
		gen_strbuilder_append_fmt(result, "  %S", self->InlineCmt->Content);
	else
		gen_strbuilder_append_str(result, gen_txt("\n"));
}

inline gen_StrBuilder gen_include_to_strbuilder(gen_CodeInclude include)
{
	GEN_ASSERT(include);
	return gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "#include %S\n", include->Content);
}

inline void gen_include_to_strbuilder_ref(gen_CodeInclude include, gen_StrBuilder* result)
{
	GEN_ASSERT(include);
	GEN_ASSERT(result);
	gen_strbuilder_append_fmt(result, "#include %S\n", include->Content);
}

inline gen_StrBuilder gen_module_to_strbuilder(gen_CodeModule self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 64);
	gen_module_to_strbuilder_ref(self, &result);
	return result;
}

inline gen_StrBuilder namespace_to_strbuilder(gen_CodeNS self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 512);
	namespace_to_strbuilder_ref(self, &result);
	return result;
}

inline void namespace_to_strbuilder_ref(gen_CodeNS self, gen_StrBuilder* result)
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if (gen_bitfield_is_set(gen_u32, self->ModuleFlags, ModuleFlag_Export))
		gen_strbuilder_append_str(result, gen_txt("export "));

	gen_strbuilder_append_fmt(result, "namespace %S\n{\n%SB\n}\n", self->Name, gen_body_to_strbuilder(self->Body));
}

inline gen_StrBuilder gen_params_to_strbuilder(gen_CodeParams self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 128);
	gen_params_to_strbuilder_ref(self, &result);
	return result;
}

inline gen_StrBuilder gen_pragma_to_strbuilder(gen_CodePragma self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 256);
	gen_pragma_to_strbuilder_ref(self, &result);
	return result;
}

inline void gen_pragma_to_strbuilder_ref(gen_CodePragma self, gen_StrBuilder* result)
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	gen_strbuilder_append_fmt(result, "#pragma %S\n", self->Content);
}

inline void gen_preprocess_to_strbuilder_if(gen_CodePreprocessCond cond, gen_StrBuilder* result)
{
	GEN_ASSERT(cond);
	GEN_ASSERT(result);
	gen_strbuilder_append_fmt(result, "#if %S", cond->Content);
}

inline void gen_preprocess_to_strbuilder_ifdef(gen_CodePreprocessCond cond, gen_StrBuilder* result)
{
	GEN_ASSERT(cond);
	GEN_ASSERT(result);
	gen_strbuilder_append_fmt(result, "#ifdef %S\n", cond->Content);
}

inline void gen_preprocess_to_strbuilder_ifndef(gen_CodePreprocessCond cond, gen_StrBuilder* result)
{
	GEN_ASSERT(cond);
	GEN_ASSERT(result);
	gen_strbuilder_append_fmt(result, "#ifndef %S", cond->Content);
}

inline void gen_preprocess_to_strbuilder_elif(gen_CodePreprocessCond cond, gen_StrBuilder* result)
{
	GEN_ASSERT(cond);
	GEN_ASSERT(result);
	gen_strbuilder_append_fmt(result, "#elif %S\n", cond->Content);
}

inline void gen_preprocess_to_strbuilder_else(gen_CodePreprocessCond cond, gen_StrBuilder* result)
{
	GEN_ASSERT(cond);
	GEN_ASSERT(result);
	gen_strbuilder_append_str(result, gen_txt("#else\n"));
}

inline void gen_preprocess_to_strbuilder_endif(gen_CodePreprocessCond cond, gen_StrBuilder* result)
{
	GEN_ASSERT(cond);
	GEN_ASSERT(result);
	gen_strbuilder_append_str(result, gen_txt("#endif\n"));
}

inline gen_StrBuilder gen_specifiers_to_strbuilder(gen_CodeSpecifiers self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 64);
	gen_specifiers_to_strbuilder_ref(self, &result);
	return result;
}

inline gen_StrBuilder gen_template_to_strbuilder(gen_CodeTemplate self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 1024);
	gen_template_to_strbuilder_ref(self, &result);
	return result;
}

inline gen_StrBuilder gen_typedef_to_strbuilder(gen_CodeTypedef self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 128);
	gen_typedef_to_strbuilder_ref(self, &result);
	return result;
}

inline gen_StrBuilder gen_typename_to_strbuilder(gen_CodeTypename self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_str(gen__ctx->Allocator_Temp, gen_txt(""));
	gen_typename_to_strbuilder_ref(self, &result);
	return result;
}

inline gen_StrBuilder gen_using_to_strbuilder(gen_CodeUsing self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, 128);
	switch (self->Type)
	{
		case CT_Using:
			gen_using_to_strbuilder_ref(self, &result);
			break;
		case CT_Using_Namespace:
			gen_using_to_strbuilder_ns(self, &result);
			break;
	}
	return result;
}

inline void gen_using_to_strbuilder_ns(gen_CodeUsing self, gen_StrBuilder* result)
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if (self->InlineCmt)
		gen_strbuilder_append_fmt(result, "using namespace $S;  %S", self->Name, self->InlineCmt->Content);
	else
		gen_strbuilder_append_fmt(result, "using namespace %S;\n", self->Name);
}

inline gen_StrBuilder gen_var_to_strbuilder(gen_CodeVar self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen_get_context()->Allocator_Temp, 256);
	gen_var_to_strbuilder_ref(self, &result);
	return result;
}

#pragma endregion Serialization


#pragma region gen_Code

inline void gen_code__append(gen_Code self, gen_Code other)
{
	GEN_ASSERT(self);
	GEN_ASSERT(other);
	GEN_ASSERT_MSG(self != other, "Attempted to recursively append gen_Code gen_AST to itself.");

	if (other->Parent != gen_nullptr)
		other = gen_code_duplicate(other);

	other->Parent = self;

	if (self->Front == gen_nullptr)
	{
		self->Front = other;
		self->Back  = other;

		self->NumEntries++;
		return;
	}

	gen_Code Current = self->Back;
	Current->Next    = other;
	other->Prev      = Current;
	self->Back       = other;
	self->NumEntries++;
}

inline bool gen_code__is_body(gen_Code self)
{
	GEN_ASSERT(self);
	switch (self->Type)
	{
		case CT_Enum_Body:
		case CT_Class_Body:
		case CT_Union_Body:
		case CT_Export_Body:
		case CT_Global_Body:
		case CT_Struct_Body:
		case CT_Function_Body:
		case CT_Namespace_Body:
		case CT_Extern_Linkage_Body:
			return true;
	}
	return false;
}

inline gen_Code* gen_code__entry(gen_Code self, gen_u32 idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_Code* current = &self->Front;
	while (idx >= 0 && current != gen_nullptr)
	{
		if (idx == 0)
			return gen_rcast(gen_Code*, current);

		current = &(*current)->Next;
		idx--;
	}

	return gen_rcast(gen_Code*, current);
}

gen_forceinline bool gen_code__is_valid(gen_Code self)
{
	GEN_ASSERT(self);
	return self != gen_nullptr && self->Type != CT_Invalid;
}

gen_forceinline bool gen_code__has_entries(gen_Code self)
{
	GEN_ASSERT(self);
	return self->NumEntries > 0;
}

gen_forceinline void gen_code__set_global(gen_Code self)
{
	if (self == gen_nullptr)
	{
		gen_log_failure("gen_Code::set_global: Cannot set code as gen_global, gen_AST is null!");
		return;
	}

	self->Parent = gen_Code_Global;
}

gen_forceinline gen_Str gen_code__type_str(gen_Code self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_codetype_to_str(self->Type);
}

#pragma endregion gen_Code


#pragma region gen_CodeBody

inline void gen_body_append(gen_CodeBody self, gen_Code other)
{
	GEN_ASSERT(self);
	GEN_ASSERT(other);

	if (gen_code_is_body(other))
	{
		gen_body_append_body(self, gen_cast(gen_CodeBody, other));
		return;
	}

	gen_code_append(gen_cast(gen_Code, self), other);
}

inline void gen_body_append_body(gen_CodeBody self, gen_CodeBody body)
{
	GEN_ASSERT(self);
	GEN_ASSERT(body);
	GEN_ASSERT_MSG(self != body, "Attempted to append body to itself.");

	for (gen_Code entry = gen_begin_CodeBody(body); entry != gen_end_CodeBody(body); entry = gen_next_CodeBody(body, entry))
	{
		gen_body_append(self, entry);
	}
}

inline gen_Code gen_begin_CodeBody(gen_CodeBody body)
{
	GEN_ASSERT(body);
	if (body != gen_nullptr)
		return body->Front;

	return gen_NullCode;
}

gen_forceinline gen_Code gen_end_CodeBody(gen_CodeBody body)
{
	GEN_ASSERT(body);
	return body->Back->Next;
}

inline gen_Code gen_next_CodeBody(gen_CodeBody body, gen_Code entry)
{
	GEN_ASSERT(body);
	GEN_ASSERT(entry);
	return entry->Next;
}

#pragma endregion gen_CodeBody


#pragma region gen_CodeClass

inline void gen_class_add_interface(gen_CodeClass self, gen_CodeTypename type)
{
	GEN_ASSERT(self);
	GEN_ASSERT(type);
	gen_CodeTypename possible_slot = self->ParentType;
	if (possible_slot != gen_nullptr)
	{
		// Were adding an interface to parent type, so we need to make sure the parent type is public.
		self->ParentAccess = AccessSpec_Public;
		// If your planning on adding a proper parent,
		// then you'll need to move this over to ParentType->next and update ParentAccess accordingly.
	}

	while (possible_slot->Next != gen_nullptr)
	{
		possible_slot = gen_cast(gen_CodeTypename, possible_slot->Next);
	}

	possible_slot->Next = gen_cast(gen_Code, type);
}

#pragma endregion gen_CodeClass


#pragma region gen_CodeParams

inline void gen_params_append(gen_CodeParams appendee, gen_CodeParams other)
{
	GEN_ASSERT(appendee);
	GEN_ASSERT(other);
	GEN_ASSERT_MSG(appendee != other, "Attempted to append parameter to itself.");
	gen_Code self  = gen_cast(gen_Code, appendee);
	gen_Code entry = gen_cast(gen_Code, other);

	if (entry->Parent != gen_nullptr)
		entry = gen_code_duplicate(entry);

	entry->Parent = self;

	if (self->Last == gen_nullptr)
	{
		self->Last = entry;
		self->Next = entry;
		self->NumEntries++;
		return;
	}

	self->Last->Next = entry;
	self->Last       = entry;
	self->NumEntries++;
}

inline gen_CodeParams gen_params_get(gen_CodeParams self, gen_s32 idx)
{
	GEN_ASSERT(self);
	gen_CodeParams param = self;
	do
	{
		if (++param != gen_nullptr)
			return gen_NullCode;

		param = gen_cast(gen_CodeParams, gen_cast(gen_Code, param)->Next);
	} while (--idx);

	return param;
}

gen_forceinline bool gen_params_has_entries(gen_CodeParams self)
{
	GEN_ASSERT(self);
	return self->NumEntries > 0;
}

gen_forceinline gen_CodeParams gen_begin_CodeParams(gen_CodeParams params)
{
	if (params != gen_nullptr)
		return params;

	return gen_NullCode;
}

gen_forceinline gen_CodeParams gen_end_CodeParams(gen_CodeParams params)
{
	// return { (gen_AST_Params*) gen_rcast( gen_AST*, ast)->Last };
	return gen_NullCode;
}

gen_forceinline gen_CodeParams gen_next_CodeParams(gen_CodeParams params, gen_CodeParams param_iter)
{
	GEN_ASSERT(param_iter);
	return param_iter->Next;
}

#pragma endregion gen_CodeParams


#pragma region gen_CodeDefineParams

gen_forceinline void gen_define_params_append(gen_CodeDefineParams appendee, gen_CodeDefineParams other)
{
	gen_params_append(gen_cast(gen_CodeParams, appendee), gen_cast(gen_CodeParams, other));
}

gen_forceinline gen_CodeDefineParams gen_define_params_get(gen_CodeDefineParams self, gen_s32 idx)
{
	return (gen_CodeDefineParams)(gen_Code)gen_params_get(gen_cast(gen_CodeParams, self), idx);
}

gen_forceinline bool gen_define_params_has_entries(gen_CodeDefineParams self)
{
	return gen_params_has_entries(gen_cast(gen_CodeParams, self));
}

gen_forceinline gen_CodeDefineParams gen_begin_CodeDefineParams(gen_CodeDefineParams params)
{
	return (gen_CodeDefineParams)(gen_Code)gen_begin_CodeParams(gen_cast(gen_CodeParams, (gen_Code)params));
}

gen_forceinline gen_CodeDefineParams gen_end_CodeDefineParams(gen_CodeDefineParams params)
{
	return (gen_CodeDefineParams)(gen_Code)gen_end_CodeParams(gen_cast(gen_CodeParams, (gen_Code)params));
}

gen_forceinline gen_CodeDefineParams gen_next_CodeDefineParams(gen_CodeDefineParams params, gen_CodeDefineParams entry_iter)
{
	return (gen_CodeDefineParams)(gen_Code)gen_next_CodeParams(gen_cast(gen_CodeParams, (gen_Code)params), gen_cast(gen_CodeParams, (gen_Code)entry_iter));
}

#pragma endregion gen_CodeDefineParams


#pragma region gen_CodeSpecifiers

inline bool gen_specifiers_append(gen_CodeSpecifiers self, gen_Specifier spec)
{
	if (self == gen_nullptr)
	{
		gen_log_failure("gen_CodeSpecifiers: Attempted to append to a null specifiers gen_AST!");
		return false;
	}
	if (self->NumEntries == gen_AST_ArrSpecs_Cap)
	{
		gen_log_failure("gen_CodeSpecifiers: Attempted to append over %d specifiers to a specifiers gen_AST!", gen_AST_ArrSpecs_Cap);
		return false;
	}

	self->ArrSpecs[self->NumEntries] = spec;
	self->NumEntries++;
	return true;
}

inline bool gen_specifiers_has(gen_CodeSpecifiers self, gen_Specifier spec)
{
	GEN_ASSERT(self != gen_nullptr);
	for (gen_s32 idx = 0; idx < self->NumEntries; idx++)
	{
		if (self->ArrSpecs[idx] == spec)
			return true;
	}
	return false;
}

inline gen_s32 gen_specifiers_index_of(gen_CodeSpecifiers self, gen_Specifier spec)
{
	GEN_ASSERT(self != gen_nullptr);
	for (gen_s32 idx = 0; idx < self->NumEntries; idx++)
	{
		if (self->ArrSpecs[idx] == spec)
			return idx;
	}
	return -1;
}

inline gen_s32 gen_specifiers_remove(gen_CodeSpecifiers self, gen_Specifier to_remove)
{
	if (self == gen_nullptr)
	{
		gen_log_failure("gen_CodeSpecifiers: Attempted to append to a null specifiers gen_AST!");
		return -1;
	}
	if (self->NumEntries == gen_AST_ArrSpecs_Cap)
	{
		gen_log_failure("gen_CodeSpecifiers: Attempted to append over %d specifiers to a specifiers gen_AST!", gen_AST_ArrSpecs_Cap);
		return -1;
	}

	gen_s32 result = -1;

	gen_s32 curr   = 0;
	gen_s32 next   = 0;
	for (; next < self->NumEntries; ++curr, ++next)
	{
		gen_Specifier spec = self->ArrSpecs[next];
		if (spec == to_remove)
		{
			result = next;

			next++;
			if (next >= self->NumEntries)
				break;

			spec = self->ArrSpecs[next];
		}

		self->ArrSpecs[curr] = spec;
	}

	if (result > -1)
	{
		self->NumEntries--;
	}
	return result;
}

gen_forceinline gen_Specifier* gen_begin_CodeSpecifiers(gen_CodeSpecifiers self)
{
	if (self != gen_nullptr)
		return &self->ArrSpecs[0];

	return gen_nullptr;
}

gen_forceinline gen_Specifier* gen_end_CodeSpecifiers(gen_CodeSpecifiers self)
{
	return self->ArrSpecs + self->NumEntries;
}

gen_forceinline gen_Specifier* gen_next_CodeSpecifiers(gen_CodeSpecifiers self, gen_Specifier* gen_spec_iter)
{
	return gen_spec_iter + 1;
}

#pragma endregion gen_CodeSpecifiers


#pragma region gen_CodeStruct

inline void gen_struct_add_interface(gen_CodeStruct self, gen_CodeTypename type)
{
	gen_CodeTypename possible_slot = self->ParentType;
	if (possible_slot != gen_nullptr)
	{
		// Were adding an interface to parent type, so we need to make sure the parent type is public.
		self->ParentAccess = AccessSpec_Public;
		// If your planning on adding a proper parent,
		// then you'll need to move this over to ParentType->next and update ParentAccess accordingly.
	}

	while (possible_slot->Next != gen_nullptr)
	{
		possible_slot = gen_cast(gen_CodeTypename, possible_slot->Next);
	}

	possible_slot->Next = gen_cast(gen_Code, type);
}

#pragma endregion gen_Code


#pragma region Interface

inline gen_CodeBody gen_def_body(gen_CodeType type)
{
	switch (type)
	{
		case CT_Class_Body:
		case CT_Enum_Body:
		case CT_Export_Body:
		case CT_Extern_Linkage:
		case CT_Function_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
			break;

		default:
			gen_log_failure("gen_def_body: Invalid type %s", gen_codetype_to_str(type).Ptr);
			return (gen_CodeBody)gen_Code_Invalid;
	}

	gen_Code result = gen_make_code();
	result->Type    = type;
	return (gen_CodeBody)result;
}

inline gen_Str gen_token_fmt_impl(gen_ssize num, ...)
{
	gen_local_persist gen_thread_local char buf[GEN_PRINTF_MAXLEN] = { 0 };
	gen_mem_set(buf, 0, GEN_PRINTF_MAXLEN);

	va_list va;
	va_start(va, num);
	gen_ssize result = gen_token_fmt_va(buf, GEN_PRINTF_MAXLEN, num, va);
	va_end(va);

	gen_Str str = { buf, result };
	return str;
}

#pragma endregion Interface
#pragma endregion Inlines


#pragma region gen_Builder


struct gen_Builder;
typedef struct gen_Builder gen_Builder;

GEN_API gen_Builder gen_builder_open(char const* path);
GEN_API void        gen_builder_pad_lines(gen_Builder* builder, gen_s32 num);
GEN_API void        gen_builder__print(gen_Builder* builder, gen_Code code);
GEN_API void        gen_builder_print_fmt_va(gen_Builder* builder, char const* fmt, va_list va);
GEN_API void        gen_builder_write(gen_Builder* builder);

gen_forceinline void gen_builder_print_fmt(gen_Builder* builder, char const* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	gen_builder_print_fmt_va(builder, fmt, va);
	va_end(va);
}

#define gen_builder_print(builder, code)            \
	_Generic(                                       \
	    (code),                                     \
	    gen_Code: gen_builder__print,               \
	    gen_CodeBody: gen_builder__print,           \
	    gen_CodeAttributes: gen_builder__print,     \
	    gen_CodeComment: gen_builder__print,        \
	    gen_CodeClass: gen_builder__print,          \
	    gen_CodeConstructor: gen_builder__print,    \
	    gen_CodeDefine: gen_builder__print,         \
	    gen_CodeDefineParams: gen_builder__print,   \
	    gen_CodeDestructor: gen_builder__print,     \
	    gen_CodeEnum: gen_builder__print,           \
	    gen_CodeExec: gen_builder__print,           \
	    gen_CodeExtern: gen_builder__print,         \
	    gen_CodeInclude: gen_builder__print,        \
	    gen_CodeFriend: gen_builder__print,         \
	    gen_CodeFn: gen_builder__print,             \
	    gen_CodeModule: gen_builder__print,         \
	    gen_CodeNS: gen_builder__print,             \
	    gen_CodeOperator: gen_builder__print,       \
	    gen_CodeOpCast: gen_builder__print,         \
	    gen_CodePragma: gen_builder__print,         \
	    gen_CodeParams: gen_builder__print,         \
	    gen_CodePreprocessCond: gen_builder__print, \
	    gen_CodeSpecifiers: gen_builder__print,     \
	    gen_CodeStruct: gen_builder__print,         \
	    gen_CodeTemplate: gen_builder__print,       \
	    gen_CodeTypename: gen_builder__print,       \
	    gen_CodeTypedef: gen_builder__print,        \
	    gen_CodeUnion: gen_builder__print,          \
	    gen_CodeUsing: gen_builder__print,          \
	    gen_CodeVar: gen_builder__print,            \
	    default: gen_generic_selection_fail         \
	) GEN_RESOLVED_FUNCTION_CALL(builder, (gen_Code)code)

struct gen_Builder
{
	gen_FileInfo   File;
	gen_StrBuilder Buffer;
};

#pragma endregion gen_Builder

#pragma region Scanner

// This is a simple file reader that reads the entire file into memory.
// It has an extra option to skip the first few lines for undesired includes.
// This is done so that includes can be kept in dependency and component files so that intellisense works.
GEN_API gen_Code gen_scan_file(char const* path);

GEN_API gen_CodeBody gen_parse_file(const char* path);

// The follow is basic support for light csv parsing (use it as an example)
// Make something robust if its more serious.

typedef struct gen_CSV_Column gen_CSV_Column;

struct gen_CSV_Column
{
	gen_CSV_Object ADT;
	gen_Array(gen_ADT_Node) Content;
};

typedef struct gen_CSV_Columns2 gen_CSV_Columns2;

struct gen_CSV_Columns2
{
	gen_CSV_Object ADT;
	gen_Array(gen_ADT_Node) Col_1;
	gen_Array(gen_ADT_Node) Col_2;
};

GEN_API gen_CSV_Column   gen_parse_csv_one_column(gen_AllocatorInfo allocator, char const* path);
GEN_API gen_CSV_Columns2 gen_parse_csv_two_columns(gen_AllocatorInfo allocator, char const* path);

#pragma endregion Scanner

GEN_API_C_END
GEN_NS_END


#pragma region GENCPP IMPLEMENTATION GUARD
#if defined(GEN_IMPLEMENTATION) && ! defined(GEN_IMPLEMENTED)
#	define GEN_IMPLEMENTED

//! If its desired to roll your own dependencies, define GEN_ROLL_OWN_DEPENDENCIES before including this file.
// Dependencies are derived from the c-zpl library: https://github.com/zpl-c/zpl
#ifndef GEN_ROLL_OWN_DEPENDENCIES

GEN_NS_BEGIN
GEN_API_C_BEGIN

#pragma region Macros and Includes

#	include <stdio.h>
// NOTE: Ensure we use standard methods for these calls if we use GEN_PICO
#	if ! defined( GEN_PICO_CUSTOM_ROUTINES )
#		if ! defined( GEN_MODULE_CORE )
#			define _strlen                   strlen
#			define _printf_err( fmt, ... )   fprintf( stderr, fmt, __VA_ARGS__ )
#			define _printf_err_va( fmt, va ) vfprintf( stderr, fmt, va )
#		else
#			define _strlen                   gen_c_str_len
#			define _printf_err( fmt, ... )   gen_c_str_fmt_out_err( fmt, __VA_ARGS__ )
#			define _printf_err_va( fmt, va ) gen_c_str_fmt_out_err_va( fmt, va )
#		endif
#	endif
#
#	include <errno.h>
#
#	if defined( GEN_SYSTEM_UNIX ) || defined( GEN_SYSTEM_MACOS )
#		include <unistd.h>
#	elif defined( GEN_SYSTEM_WINDOWS )
#		if ! defined( GEN_NO_WINDOWS_H )
#			ifndef WIN32_LEAN_AND_MEAN
#				ifndef NOMINMAX
#					define NOMINMAX
#				endif
#
#				define WIN32_LEAN_AND_MEAN
#				define WIN32_MEAN_AND_LEAN
#				define VC_EXTRALEAN
#			endif
#			include <windows.h>
#			undef NOMINMAX
#			undef WIN32_LEAN_AND_MEAN
#			undef WIN32_MEAN_AND_LEAN
#			undef VC_EXTRALEAN
#		endif
#	endif

#include <sys/stat.h>

#ifdef GEN_SYSTEM_MACOS
#	include <copyfile.h>
#endif

#ifdef GEN_SYSTEM_CYGWIN
#	include <windows.h>
#endif

#if defined( GEN_SYSTEM_WINDOWS ) && ! defined( GEN_COMPILER_GCC )
#	include <io.h>
#endif

#if defined( GEN_SYSTEM_LINUX )
#	include <sys/types.h>
#endif

#ifdef GEN_BENCHMARK
// Timing includes
#if defined( GEN_SYSTEM_MACOS ) || GEN_SYSTEM_UNIX
#	include <time.h>
#	include <sys/time.h>
#endif

#if defined( GEN_SYSTEM_MACOS )
#	include <mach/mach.h>
#	include <mach/mach_time.h>
#	include <mach/clock.h>
#endif

#if defined( GEN_SYSTEM_EMSCRIPTEN )
#	include <emscripten.h>
#endif

#if defined( GEN_SYSTEM_WINDOWS )
#	include <timezoneapi.h>
#endif
#endif

#pragma endregion Macros and Includes

#pragma region Debug

void gen_assert_handler( char const* condition, char const* file, char const* function, gen_s32 line, char const* msg, ... )
{
	_printf_err( "%s - %s:(%d): Assert Failure: ", file, function, line );

	if ( condition )
		_printf_err( "`%s` \n", condition );

	if ( msg )
	{
		va_list va;
		va_start( va, msg );
		_printf_err_va( msg, va );
		va_end( va );
	}

	_printf_err( "%s", "\n" );
}

gen_s32 gen_assert_crash( char const* condition )
{
	GEN_PANIC( condition );
	return 0;
}

#if defined( GEN_SYSTEM_WINDOWS )
	void gen_process_exit( gen_u32 code )
	{
		ExitProcess( code );
	}
#else
#	include <stdlib.h>

	void gen_process_exit( gen_u32 code )
	{
		exit( code );
	}
#endif

#pragma endregion Debug

#pragma region String Ops

gen_internal
gen_ssize _scan_zpl_i64( const char* text, gen_s32 base, gen_s64* value )
{
	const char* text_begin = text;
	gen_s64         result     = 0;
	gen_b32         negative   = false;

	if ( *text == '-' )
	{
		negative = true;
		text++;
	}

	if ( base == 16 && gen_c_str_compare_len( text, "0x", 2 ) == 0 )
		text += 2;

	for ( ;; )
	{
		gen_s64 v;
		if ( gen_char_is_digit( *text ) )
			v = *text - '0';
		else if ( base == 16 && gen_char_is_hex_digit( *text ) )
			v = hex_digit_to_int( *text );
		else
			break;

		result *= base;
		result += v;
		text++;
	}

	if ( value )
	{
		if ( negative )
			result = -result;
		*value = result;
	}

	return ( text - text_begin );
}

// TODO : Are these good enough for characters?
gen_global const char _num_to_char_table[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"@$";

gen_s64 gen_c_str_to_i64( const char* str, char** gen_end_ptr, gen_s32 base )
{
	gen_ssize  len;
	gen_s64 value;

	if ( ! base )
	{
		if ( ( gen_c_str_len( str ) > 2 ) && ( gen_c_str_compare_len( str, "0x", 2 ) == 0 ) )
			base = 16;
		else
			base = 10;
	}

	len = _scan_zpl_i64( str, base, &value );
	if ( gen_end_ptr )
		*gen_end_ptr = ( char* )str + len;
	return value;
}

void gen_i64_to_str( gen_s64 value, char* string, gen_s32 base )
{
	char* buf      = string;
	gen_b32   negative = false;
	gen_u64   v;

	if ( value < 0 )
	{
		negative = true;
		value    = -value;
	}

	v = gen_scast( gen_u64, value);
	if ( v != 0 )
	{
		while ( v > 0 )
		{
			*buf++  = _num_to_char_table[ v % base ];
			v      /= base;
		}
	}
	else
	{
		*buf++ = '0';
	}
	if ( negative )
		*buf++ = '-';
	*buf = '\0';
	gen_c_str_reverse( string );
}

void gen_u64_to_str( gen_u64 value, char* string, gen_s32 base )
{
	char* buf = string;

	if ( value )
	{
		while ( value > 0 )
		{
			*buf++  = _num_to_char_table[ value % base ];
			value  /= base;
		}
	}
	else
	{
		*buf++ = '0';
	}
	*buf = '\0';

	gen_c_str_reverse( string );
}

gen_f64 gen_c_str_to_f64( const char* str, char** gen_end_ptr )
{
	gen_f64 result, value, sign, scale;
	gen_s32 frac;

	while ( gen_char_is_space( *str ) )
	{
		str++;
	}

	sign = 1.0;
	if ( *str == '-' )
	{
		sign = -1.0;
		str++;
	}
	else if ( *str == '+' )
	{
		str++;
	}

	for ( value = 0.0; gen_char_is_digit( *str ); str++ )
	{
		value = value * 10.0 + ( *str - '0' );
	}

	if ( *str == '.' )
	{
		gen_f64 pow10 = 10.0;
		str++;
		while ( gen_char_is_digit( *str ) )
		{
			value += ( *str - '0' ) / pow10;
			pow10 *= 10.0;
			str++;
		}
	}

	frac  = 0;
	scale = 1.0;
	if ( ( *str == 'e' ) || ( *str == 'E' ) )
	{
		gen_u32 exp;

		str++;
		if ( *str == '-' )
		{
			frac = 1;
			str++;
		}
		else if ( *str == '+' )
		{
			str++;
		}

		for ( exp = 0; gen_char_is_digit( *str ); str++ )
		{
			exp = exp * 10 + ( *str - '0' );
		}
		if ( exp > 308 )
			exp = 308;

		while ( exp >= 50 )
		{
			scale *= 1e50;
			exp   -= 50;
		}
		while ( exp >= 8 )
		{
			scale *= 1e8;
			exp   -= 8;
		}
		while ( exp > 0 )
		{
			scale *= 10.0;
			exp   -= 1;
		}
	}

	result = sign * ( frac ? ( value / scale ) : ( value * scale ) );

	if ( gen_end_ptr )
		* gen_end_ptr = gen_rcast( char*, gen_ccast(char*, str) );

	return result;
}

#pragma endregion String Ops

#pragma region Printing

enum
{
	GEN_FMT_MINUS = gen_bit( 0 ),
	GEN_FMT_PLUS  = gen_bit( 1 ),
	GEN_FMT_ALT   = gen_bit( 2 ),
	GEN_FMT_SPACE = gen_bit( 3 ),
	GEN_FMT_ZERO  = gen_bit( 4 ),

	GEN_FMT_CHAR   = gen_bit( 5 ),
	GEN_FMT_SHORT  = gen_bit( 6 ),
	GEN_FMT_INT    = gen_bit( 7 ),
	GEN_FMT_LONG   = gen_bit( 8 ),
	GEN_FMT_LLONG  = gen_bit( 9 ),
	GEN_FMT_SIZE   = gen_bit( 10 ),
	GEN_FMT_INTPTR = gen_bit( 11 ),

	GEN_FMT_UNSIGNED = gen_bit( 12 ),
	GEN_FMT_LOWER    = gen_bit( 13 ),
	GEN_FMT_UPPER    = gen_bit( 14 ),
	GEN_FMT_WIDTH    = gen_bit( 15 ),

	GEN_FMT_DONE = gen_bit( 30 ),

	GEN_FMT_INTS = GEN_FMT_CHAR | GEN_FMT_SHORT | GEN_FMT_INT | GEN_FMT_LONG | GEN_FMT_LLONG | GEN_FMT_SIZE | GEN_FMT_INTPTR
};

typedef struct gen__format_info gen__format_info;
struct gen__format_info
{
	gen_s32 base;
	gen_s32 flags;
	gen_s32 width;
	gen_s32 precision;
};

gen_internal gen_ssize gen__print_string( char* text, gen_ssize max_len, gen__format_info* info, char const* str )
{
	gen_ssize res = 0, len = 0;
	gen_ssize remaining = max_len;
	char* begin     = text;

	if ( str == NULL && max_len >= 6 )
	{
		res += gen_c_str_copy_nulpad( text, "(null)", 6 );
		return res;
	}

	if ( info && info->precision >= 0 )
		// Made the design decision for this library that precision is the length of the string.
		len = info->precision;
	else
		len = gen_c_str_len( str );

	if ( info && ( info->width == 0 && info->flags & GEN_FMT_WIDTH ) )
	{
		return res;
	}

	if ( info && ( info->width == 0 || info->flags & GEN_FMT_MINUS ) )
	{
		if ( info->precision > 0 )
			len = info->precision < len ? info->precision : len;
		if ( res + len > max_len )
			return res;
		res  += gen_c_str_copy_nulpad( text, str, len );
		text += res;

		if ( info->width > res )
		{
			gen_ssize padding = info->width - len;

			char pad = ( info->flags & GEN_FMT_ZERO ) ? '0' : ' ';
			while ( padding-- > 0 && remaining-- > 0 )
				*text++ = pad, res++;
		}
	}
	else
	{
		if ( info && ( info->width > res ) )
		{
			gen_ssize   padding = info->width - len;
			char pad     = ( info->flags & GEN_FMT_ZERO ) ? '0' : ' ';
			while ( padding-- > 0 && remaining-- > 0 )
				*text++ = pad, res++;
		}

		if ( res + len > max_len )
			return res;
		res += gen_c_str_copy_nulpad( text, str, len );
	}

	if ( info )
	{
		if ( info->flags & GEN_FMT_UPPER )
			gen_c_str_to_upper( begin );
		else if ( info->flags & GEN_FMT_LOWER )
			gen_c_str_to_lower( begin );
	}

	return res;
}

gen_internal gen_ssize gen__print_char( char* text, gen_ssize max_len, gen__format_info* info, char arg )
{
	char str[ 2 ] = "";
	str[ 0 ]      = arg;
	return gen__print_string( text, max_len, info, str );
}

gen_internal gen_ssize gen__print_repeated_char( char* text, gen_ssize max_len, gen__format_info* info, char arg )
{
	gen_ssize  res = 0;
	gen_s32 rem = ( info ) ? ( info->width > 0 ) ? info->width : 1 : 1;
	res     = rem;
	while ( rem-- > 0 )
		*text++ = arg;

	return res;
}

gen_internal gen_ssize gen__print_i64( char* text, gen_ssize max_len, gen__format_info* info, gen_s64 value )
{
	char num[ 130 ];
	gen_i64_to_str( value, num, info ? info->base : 10 );
	return gen__print_string( text, max_len, info, num );
}

gen_internal gen_ssize gen__print_u64( char* text, gen_ssize max_len, gen__format_info* info, gen_u64 value )
{
	char num[ 130 ];
	gen_u64_to_str( value, num, info ? info->base : 10 );
	return gen__print_string( text, max_len, info, num );
}

gen_internal gen_ssize gen__print_f64( char* text, gen_ssize max_len, gen__format_info* info, gen_b32 is_hexadecimal, gen_f64 arg )
{
	// TODO: Handle exponent notation
	gen_ssize    width, len, remaining = max_len;
	char* text_begin = text;

	if ( arg )
	{
		gen_u64 value;
		if ( arg < 0 )
		{
			if ( remaining > 1 )
				*text = '-', remaining--;
			text++;
			arg = -arg;
		}
		else if ( info->flags & GEN_FMT_MINUS )
		{
			if ( remaining > 1 )
				*text = '+', remaining--;
			text++;
		}

		value  = gen_scast( gen_u64, arg);
		len    = gen__print_u64( text, remaining, NULL, value );
		text  += len;

		if ( len >= remaining )
			remaining = gen_min( remaining, 1 );
		else
			remaining -= len;
		arg -= value;

		if ( info->precision < 0 )
			info->precision = 6;

		if ( ( info->flags & GEN_FMT_ALT ) || info->precision > 0 )
		{
			gen_s64 mult = 10;
			if ( remaining > 1 )
				*text = '.', remaining--;
			text++;
			while ( info->precision-- > 0 )
			{
				value  = gen_scast( gen_u64, arg * mult );
				len    = gen__print_u64( text, remaining, NULL, value );
				text  += len;
				if ( len >= remaining )
					remaining = gen_min( remaining, 1 );
				else
					remaining -= len;
				arg  -= gen_scast( gen_f64, value / mult);
				mult *= 10;
			}
		}
	}
	else
	{
		if ( remaining > 1 )
			*text = '0', remaining--;
		text++;
		if ( info->flags & GEN_FMT_ALT )
		{
			if ( remaining > 1 )
				*text = '.', remaining--;
			text++;
		}
	}

	width = info->width - ( text - text_begin );
	if ( width > 0 )
	{
		char  fill = ( info->flags & GEN_FMT_ZERO ) ? '0' : ' ';
		char* end  = text + remaining - 1;
		len        = ( text - text_begin );

		for ( len = ( text - text_begin ); len--; )
		{
			if ( ( text_begin + len + width ) < end )
				*( text_begin + len + width ) = *( text_begin + len );
		}

		len   = width;
		text += len;
		if ( len >= remaining )
			remaining = gen_min( remaining, 1 );
		else
			remaining -= len;

		while ( len-- )
		{
			if ( text_begin + len < end )
				text_begin[ len ] = fill;
		}
	}

	return ( text - text_begin );
}

gen_neverinline gen_ssize gen_c_str_fmt_va( char* text, gen_ssize max_len, char const* fmt, va_list va )
{
	char const* text_begin = text;
	gen_ssize          remaining  = max_len, res;

	while ( *fmt )
	{
		gen__format_info info = { 0 };
		gen_ssize        len  = 0;
		info.precision    = -1;

		while ( *fmt && *fmt != '%' && remaining )
			*text++ = *fmt++;

		if ( *fmt == '%' )
		{
			do
			{
				switch ( *++fmt )
				{
					case '-' :
						{
							info.flags |= GEN_FMT_MINUS;
							break;
						}
					case '+' :
						{
							info.flags |= GEN_FMT_PLUS;
							break;
						}
					case '#' :
						{
							info.flags |= GEN_FMT_ALT;
							break;
						}
					case ' ' :
						{
							info.flags |= GEN_FMT_SPACE;
							break;
						}
					case '0' :
						{
							info.flags |= ( GEN_FMT_ZERO | GEN_FMT_WIDTH );
							break;
						}
					default :
						{
							info.flags |= GEN_FMT_DONE;
							break;
						}
				}
			} while ( ! ( info.flags & GEN_FMT_DONE ) );
		}

		// NOTE: Optional Width
		if ( *fmt == '*' )
		{
			int width = va_arg( va, int );
			if ( width < 0 )
			{
				info.flags |= GEN_FMT_MINUS;
				info.width  = -width;
			}
			else
			{
				info.width = width;
			}
			info.flags |= GEN_FMT_WIDTH;
			fmt++;
		}
		else
		{
			info.width = gen_scast( gen_s32, gen_c_str_to_i64( fmt, gen_ccast( char**, & fmt), 10 ));
			if ( info.width != 0 )
			{
				info.flags |= GEN_FMT_WIDTH;
			}
		}

		// NOTE: Optional Precision
		if ( *fmt == '.' )
		{
			fmt++;
			if ( *fmt == '*' )
			{
				info.precision = va_arg( va, int );
				fmt++;
			}
			else
			{
				info.precision = gen_scast( gen_s32, gen_c_str_to_i64( fmt, gen_ccast( char**, & fmt), 10 ));
			}
			info.flags &= ~GEN_FMT_ZERO;
		}

		switch ( *fmt++ )
		{
			case 'h' :
				if ( *fmt == 'h' )
				{    // hh => char
					info.flags |= GEN_FMT_CHAR;
					fmt++;
				}
				else
				{    // h => short
					info.flags |= GEN_FMT_SHORT;
				}
				break;

			case 'l' :
				if ( *fmt == 'l' )
				{    // ll => long long
					info.flags |= GEN_FMT_LLONG;
					fmt++;
				}
				else
				{    // l => long
					info.flags |= GEN_FMT_LONG;
				}
				break;

				break;

			case 'z' :    // NOTE: zpl_usize
				info.flags |= GEN_FMT_UNSIGNED;
				// fallthrough
			case 't' :    // NOTE: zpl_isize
				info.flags |= GEN_FMT_SIZE;
				break;

			default :
				fmt--;
				break;
		}

		switch ( *fmt )
		{
			case 'u' :
				info.flags |= GEN_FMT_UNSIGNED;
				// fallthrough
			case 'd' :
			case 'i' :
				info.base = 10;
				break;

			case 'o' :
				info.base = 8;
				break;

			case 'x' :
				info.base   = 16;
				info.flags |= ( GEN_FMT_UNSIGNED | GEN_FMT_LOWER );
				break;

			case 'X' :
				info.base   = 16;
				info.flags |= ( GEN_FMT_UNSIGNED | GEN_FMT_UPPER );
				break;

			case 'f' :
			case 'F' :
			case 'g' :
			case 'G' :
				len = gen__print_f64( text, remaining, &info, 0, va_arg( va, gen_f64 ) );
				break;

			case 'a' :
			case 'A' :
				len = gen__print_f64( text, remaining, &info, 1, va_arg( va, gen_f64 ) );
				break;

			case 'c' :
				len = gen__print_char( text, remaining, &info, gen_scast( char, va_arg( va, int ) ));
				break;

			case 's' :
				len = gen__print_string( text, remaining, &info, va_arg( va, char* ) );
				break;

			case 'S':
			{
				if ( *(fmt + 1) == 'B' )
				{

					++ fmt;
					gen_StrBuilder gen_str = { va_arg( va, char*) };

					info.precision = gen_strbuilder_length(gen_str);
					len            = gen__print_string( text, remaining, &info, gen_str );
					break;
				}

				gen_Str gen_str    = va_arg( va, gen_Str);
				info.precision = gen_str.Len;
				len            = gen__print_string( text, remaining, &info, gen_str.Ptr );
			}
			break;

			case 'r' :
				len = gen__print_repeated_char( text, remaining, &info, va_arg( va, int ) );
				break;

			case 'p' :
				info.base   = 16;
				info.flags |= ( GEN_FMT_LOWER | GEN_FMT_UNSIGNED | GEN_FMT_ALT | GEN_FMT_INTPTR );
				break;

			case '%' :
				len = gen__print_char( text, remaining, &info, '%' );
				break;

			default :
				fmt--;
				break;
		}

		fmt++;

		if ( info.base != 0 )
		{
			if ( info.flags & GEN_FMT_UNSIGNED )
			{
				gen_u64 value = 0;
				switch ( info.flags & GEN_FMT_INTS )
				{
					case GEN_FMT_CHAR :
						value = gen_scast( gen_u64, gen_scast( gen_u8, va_arg( va, int )));
						break;
					case GEN_FMT_SHORT :
						value = gen_scast( gen_u64, gen_scast( gen_u16, va_arg( va, int )));
						break;
					case GEN_FMT_LONG:
						value = gen_scast( gen_u64, va_arg( va, unsigned long ));
						break;
					case GEN_FMT_LLONG :
						value = gen_scast( gen_u64, va_arg( va, unsigned long long ));
						break;
					case GEN_FMT_SIZE :
						value = gen_scast( gen_u64, va_arg( va, gen_usize ));
						break;
					case GEN_FMT_INTPTR :
						value = gen_scast( gen_u64, va_arg( va, gen_uptr ));
						break;
					default :
						value = gen_scast( gen_u64, va_arg( va, unsigned int ));
						break;
				}

				len = gen__print_u64( text, remaining, &info, value );
			}
			else
			{
				gen_s64 value = 0;
				switch ( info.flags & GEN_FMT_INTS )
				{
					case GEN_FMT_CHAR :
						value = gen_scast( gen_s64, gen_scast( gen_s8, va_arg( va, int )));
						break;
					case GEN_FMT_SHORT :
						value = gen_scast( gen_s64, gen_scast( gen_s16, va_arg( va, int )));
						break;
					case GEN_FMT_LONG :
						value = gen_scast( gen_s64, va_arg( va, long ));
						break;
					case GEN_FMT_LLONG :
						value = gen_scast( gen_s64, va_arg( va, long long ));
						break;
					case GEN_FMT_SIZE :
						value = gen_scast( gen_s64, va_arg( va, gen_usize ));
						break;
					case GEN_FMT_INTPTR :
						value = gen_scast( gen_s64, va_arg( va, gen_uptr ));
						break;
					default :
						value = gen_scast( gen_s64, va_arg( va, int ));
						break;
				}

				len = gen__print_i64( text, remaining, &info, value );
			}
		}

		text += len;
		if ( len >= remaining )
			remaining = gen_min( remaining, 1 );
		else
			remaining -= len;
	}

	*text++ = '\0';
	res     = ( text - text_begin );
	return ( res >= max_len || res < 0 ) ? -1 : res;
}

char* gen_c_str_fmt_buf_va( char const* fmt, va_list va )
{
	gen_local_persist gen_thread_local char buffer[ GEN_PRINTF_MAXLEN ];
	gen_c_str_fmt_va( buffer, gen_size_of( buffer ), fmt, va );
	return buffer;
}

char* gen_c_str_fmt_buf( char const* fmt, ... )
{
	va_list va;
	char*   str;
	va_start( va, fmt );
	str = gen_c_str_fmt_buf_va( fmt, va );
	va_end( va );
	return str;
}

gen_ssize gen_c_str_fmt_file_va( gen_FileInfo* f, char const* fmt, va_list va )
{
	gen_local_persist gen_thread_local char buf[ GEN_PRINTF_MAXLEN ];
	gen_ssize                              len = gen_c_str_fmt_va( buf, gen_size_of( buf ), fmt, va );
	gen_b32                             res = gen_file_write( f, buf, len - 1 );    // NOTE: prevent extra whitespace
	return res ? len : -1;
}

gen_ssize gen_c_str_fmt_file( gen_FileInfo* f, char const* fmt, ... )
{
	gen_ssize      res;
	va_list va;
	va_start( va, fmt );
	res = gen_c_str_fmt_file_va( f, fmt, va );
	va_end( va );
	return res;
}

gen_ssize gen_c_str_fmt( char* str, gen_ssize n, char const* fmt, ... )
{
	gen_ssize      res;
	va_list va;
	va_start( va, fmt );
	res = gen_c_str_fmt_va( str, n, fmt, va );
	va_end( va );
	return res;
}

gen_ssize gen_c_str_fmt_out_va( char const* fmt, va_list va )
{
	return gen_c_str_fmt_file_va( gen_file_get_standard( EFileStandard_OUTPUT ), fmt, va );
}

gen_ssize gen_c_str_fmt_out_err_va( char const* fmt, va_list va )
{
	return gen_c_str_fmt_file_va( gen_file_get_standard( EFileStandard_ERROR ), fmt, va );
}

gen_ssize gen_c_str_fmt_out_err( char const* fmt, ... )
{
	gen_ssize      res;
	va_list va;
	va_start( va, fmt );
	res = gen_c_str_fmt_out_err_va( fmt, va );
	va_end( va );
	return res;
}

#pragma endregion Printing

#pragma region Memory

void* gen_mem_copy( void* dest, void const* source, gen_ssize n )
{
	if ( dest == gen_nullptr )
	{
		return gen_nullptr;
	}

	return memcpy( dest, source, n );
}

void const* gen_mem_find( void const* data, gen_u8 c, gen_ssize n )
{
	gen_u8 const* s = gen_rcast( gen_u8 const*, data);
	while ( ( gen_rcast( gen_uptr, s) & ( sizeof( gen_usize ) - 1 ) ) && n && *s != c )
	{
		s++;
		n--;
	}
	if ( n && *s != c )
	{
		gen_ssize const* w;
		gen_ssize        k = GEN__ONES * c;
		w           = gen_rcast( gen_ssize const*, s);
		while ( n >= gen_size_of( gen_ssize ) && ! GEN__HAS_ZERO( *w ^ k ) )
		{
			w++;
			n -= gen_size_of( gen_ssize );
		}
		s = gen_rcast( gen_u8 const*, w);
		while ( n && *s != c )
		{
			s++;
			n--;
		}
	}

	return n ? gen_rcast( void const*, s ) : NULL;
}

#define GEN_HEAP_STATS_MAGIC 0xDEADC0DE

typedef struct gen__heap_stats gen__heap_stats;
struct gen__heap_stats
{
	gen_u32 magic;
	gen_ssize  used_memory;
	gen_ssize  alloc_count;
};

gen_global gen__heap_stats _heap_stats_info;

void gen_heap_stats_init( void )
{
	gen_zero_item( &_heap_stats_info );
	_heap_stats_info.magic = GEN_HEAP_STATS_MAGIC;
}

gen_ssize gen_heap_stats_used_memory( void )
{
	GEN_ASSERT_MSG( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call gen_heap_stats_init first!" );
	return _heap_stats_info.used_memory;
}

gen_ssize gen_heap_stats_alloc_count( void )
{
	GEN_ASSERT_MSG( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call gen_heap_stats_init first!" );
	return _heap_stats_info.alloc_count;
}

void gen_heap_stats_check( void )
{
	GEN_ASSERT_MSG( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call gen_heap_stats_init first!" );
	GEN_ASSERT( _heap_stats_info.used_memory == 0 );
	GEN_ASSERT( _heap_stats_info.alloc_count == 0 );
}

typedef struct gen__heap_alloc_info gen__heap_alloc_info;
struct gen__heap_alloc_info
{
	gen_ssize size;
	void* physical_start;
};

void* gen_heap_allocator_proc( void* allocator_data, gen_AllocType type, gen_ssize size, gen_ssize alignment, void* old_memory, gen_ssize old_size, gen_u64 flags )
{
	void* ptr = gen_nullptr;
	// unused( allocator_data );
	// unused( old_size );
	if ( ! alignment )
		alignment = GEN_DEFAULT_MEMORY_ALIGNMENT;

#ifdef GEN_HEAP_ANALYSIS
	gen_ssize alloc_info_size      = gen_size_of( gen__heap_alloc_info );
	gen_ssize alloc_info_remainder = ( alloc_info_size % alignment );
	gen_ssize track_size           = gen_max( alloc_info_size, alignment ) + alloc_info_remainder;
	switch ( type )
	{
		case EAllocation_FREE :
			{
				if ( ! old_memory )
					break;
				gen__heap_alloc_info* alloc_info  = gen_rcast( gen__heap_alloc_info*, old_memory) - 1;
				_heap_stats_info.used_memory -= alloc_info->size;
				_heap_stats_info.alloc_count--;
				old_memory = alloc_info->physical_start;
			}
			break;
		case EAllocation_ALLOC :
			{
				size += track_size;
			}
			break;
		default :
			break;
	}
#endif

	switch ( type )
	{
#if defined( GEN_COMPILER_MSVC ) || ( defined( GEN_COMPILER_GCC ) && defined( GEN_SYSTEM_WINDOWS ) ) || ( defined( GEN_COMPILER_TINYC ) && defined( GEN_SYSTEM_WINDOWS ) )
		case EAllocation_ALLOC :
			ptr = _aligned_malloc( size, alignment );
			if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				gen_zero_size( ptr, size );
			break;
		case EAllocation_FREE :
			_aligned_free( old_memory );
			break;
		case EAllocation_RESIZE :
			{
				gen_AllocatorInfo a = gen_heap();
				ptr             = gen_default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;

#elif defined( GEN_SYSTEM_LINUX ) && ! defined( GEN_CPU_ARM ) && ! defined( GEN_COMPILER_TINYC )
		case EAllocation_ALLOC :
			{
				ptr = aligned_alloc( alignment, ( size + alignment - 1 ) & ~( alignment - 1 ) );

				if ( flags & GEN_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				{
					gen_zero_size( ptr, size );
				}
			}
			break;

		case EAllocation_FREE :
			{
				free( old_memory );
			}
			break;

		case EAllocation_RESIZE :
			{
				gen_AllocatorInfo a = gen_heap();
				ptr             = gen_default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;
#else
		case EAllocation_ALLOC :
			{
				posix_memalign( &ptr, alignment, size );

				if ( flags & GEN_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				{
					gen_zero_size( ptr, size );
				}
			}
			break;

		case EAllocation_FREE :
			{
				free( old_memory );
			}
			break;

		case EAllocation_RESIZE :
			{
				gen_AllocatorInfo a = gen_heap();
				ptr             = gen_default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;
#endif

		case EAllocation_FREE_ALL :
			break;
	}

#ifdef GEN_HEAP_ANALYSIS
	if ( type == EAllocation_ALLOC )
	{
		gen__heap_alloc_info* alloc_info = gen_rcast( gen__heap_alloc_info*, gen_rcast( char*, ptr) + alloc_info_remainder );
		gen_zero_item( alloc_info );
		alloc_info->size              = size - track_size;
		alloc_info->physical_start    = ptr;
		ptr                           = gen_rcast( void*, alloc_info + 1 );
		_heap_stats_info.used_memory += alloc_info->size;
		_heap_stats_info.alloc_count++;
	}
#endif

	return ptr;
}

#pragma region gen_VirtualMemory
gen_VirtualMemory gen_vm_from_memory( void* data, gen_ssize size )
{
	gen_VirtualMemory vm;
	vm.data = data;
	vm.size = size;
	return vm;
}

#if defined( GEN_SYSTEM_WINDOWS )
gen_VirtualMemory gen_vm_alloc( void* addr, gen_ssize size )
{
	gen_VirtualMemory vm;
	GEN_ASSERT( size > 0 );
	vm.data = VirtualAlloc( addr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	vm.size = size;
	return vm;
}

gen_b32 gen_vm_free( gen_VirtualMemory vm )
{
	MEMORY_BASIC_INFORMATION info;
	while ( vm.size > 0 )
	{
		if ( VirtualQuery( vm.data, &info, gen_size_of( info ) ) == 0 )
			return false;
		if ( info.BaseAddress != vm.data || info.AllocationBase != vm.data || info.State != MEM_COMMIT || info.RegionSize > gen_scast( gen_usize, vm.size) )
		{
			return false;
		}
		if ( VirtualFree( vm.data, 0, MEM_RELEASE ) == 0 )
			return false;
		vm.data  = gen_pointer_add( vm.data, info.RegionSize );
		vm.size -= info.RegionSize;
	}
	return true;
}

gen_VirtualMemory gen_vm_trim( gen_VirtualMemory vm, gen_ssize lead_size, gen_ssize size )
{
	gen_VirtualMemory new_vm = { 0 };
	void*             ptr;
	GEN_ASSERT( vm.size >= lead_size + size );

	ptr = gen_pointer_add( vm.data, lead_size );

	gen_vm_free( vm );
	new_vm = gen_vm_alloc( ptr, size );
	if ( new_vm.data == ptr )
		return new_vm;
	if ( new_vm.data )
		gen_vm_free( new_vm );
	return new_vm;
}

gen_b32 gen_vm_purge( gen_VirtualMemory vm )
{
	VirtualAlloc( vm.data, vm.size, MEM_RESET, PAGE_READWRITE );
	// NOTE: Can this really fail?
	return true;
}

gen_ssize gen_virtual_memory_page_size( gen_ssize* alignment_out )
{
	SYSTEM_INFO info;
	GetSystemInfo( &info );
	if ( alignment_out )
		*alignment_out = info.dwAllocationGranularity;
	return info.dwPageSize;
}

#else
#	include <sys/mman.h>

#	ifndef MAP_ANONYMOUS
#		define MAP_ANONYMOUS MAP_ANON
#	endif
gen_VirtualMemory gen_vm_alloc( void* addr, gen_ssize size )
{
	gen_VirtualMemory vm;
	GEN_ASSERT( size > 0 );
	vm.data = mmap( addr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0 );
	vm.size = size;
	return vm;
}

gen_b32 gen_vm_free( gen_VirtualMemory vm )
{
	munmap( vm.data, vm.size );
	return true;
}

gen_VirtualMemory gen_vm_trim( gen_VirtualMemory vm, gen_ssize lead_size, gen_ssize size )
{
	void*  ptr;
	gen_ssize trail_size;
	GEN_ASSERT( vm.size >= lead_size + size );

	ptr        = gen_pointer_add( vm.data, lead_size );
	trail_size = vm.size - lead_size - size;

	if ( lead_size != 0 )
		gen_vm_free( gen_vm_from_memory(( vm.data, lead_size ) );
	if ( trail_size != 0 )
		gen_vm_free( gen_vm_from_memory( ptr, trail_size ) );
	return gen_vm_from_memory( ptr, size );
}

gen_b32 gen_vm_purge( gen_VirtualMemory vm )
{
	int err = madvise( vm.data, vm.size, MADV_DONTNEED );
	return err != 0;
}

gen_ssize gen_virtual_memory_page_size( gen_ssize* alignment_out )
{
	// TODO: Is this always true?
	gen_ssize result = gen_scast( gen_ssize, sysconf( _SC_PAGE_SIZE ));
	if ( alignment_out )
		*alignment_out = result;
	return result;
}
#endif

#pragma endregion gen_VirtualMemory

void* gen_arena_allocator_proc( void* allocator_data, gen_AllocType type, gen_ssize size, gen_ssize alignment, void* old_memory, gen_ssize old_size, gen_u64 flags )
{
	gen_Arena* arena = gen_rcast(gen_Arena*, allocator_data);
	void*      ptr   = NULL;

	// unused( old_size );

	switch ( type )
	{
		case EAllocation_ALLOC :
		{
			void* end        = gen_pointer_add( arena->PhysicalStart, arena->TotalUsed );
			gen_ssize total_size = align_forward_s64( size, alignment );

			// NOTE: Out of memory
			if ( arena->TotalUsed + total_size > (gen_ssize) arena->TotalSize )
			{
				// zpl__printf_err("%s", "gen_Arena out of memory\n");
				GEN_FATAL("gen_Arena out of memory! (Possibly could not fit for the largest size gen_Arena!!)");
			}
			

			ptr              = gen_align_forward( end, alignment );
			arena->TotalUsed += total_size;

			if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				gen_zero_size( ptr, size );
		}
		break;

		case EAllocation_FREE :
			// NOTE: Free all at once
			// Use Temp_Arena_Memory if you want to free a block
			break;

		case EAllocation_FREE_ALL :
			arena->TotalUsed = 0;
			break;

		case EAllocation_RESIZE :
			{
				// TODO : Check if ptr is on top of stack and just extend
				gen_AllocatorInfo a = arena->Backing;
				ptr             = gen_default_resize_align( a, old_memory, old_size, size, alignment );
			}
			break;
	}
	return ptr;
}

void* gen_pool_allocator_proc( void* allocator_data, gen_AllocType type, gen_ssize size, gen_ssize alignment, void* old_memory, gen_ssize old_size, gen_u64 flags )
{
	gen_Pool* pool = gen_rcast( gen_Pool*, allocator_data);
	void* ptr  = NULL;

	// unused( old_size );

	switch ( type )
	{
		case EAllocation_ALLOC :
			{
				gen_uptr gen_next_free;

				GEN_ASSERT( size == pool->BlockSize );
				GEN_ASSERT( alignment == pool->BlockAlign );
				GEN_ASSERT( pool->FreeList != NULL );

				gen_next_free        = * gen_rcast( gen_uptr*, pool->FreeList);
				ptr              = pool->FreeList;
				pool->FreeList   = gen_rcast( void*, gen_next_free);
				pool->TotalSize += pool->BlockSize;

				if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
					gen_zero_size( ptr, size );
			}
			break;

		case EAllocation_FREE :
			{
				gen_uptr* next;
				if ( old_memory == NULL )
					return NULL;

				next             = gen_rcast( gen_uptr*, old_memory);
				*next            = gen_rcast( gen_uptr, pool->FreeList);
				pool->FreeList   = old_memory;
				pool->TotalSize -= pool->BlockSize;
			}
			break;

		case EAllocation_FREE_ALL :
			{
				gen_ssize    actual_block_size, block_index;
				void* curr;
				gen_uptr* end;

				actual_block_size = pool->BlockSize + pool->BlockAlign;
				pool->TotalSize   = 0;

				// NOTE: Init intrusive freelist
				curr = pool->PhysicalStart;
				for ( block_index = 0; block_index < pool->NumBlocks - 1; block_index++ )
				{
					gen_uptr* next = gen_rcast( gen_uptr*, curr);
					* next     = gen_rcast( gen_uptr, curr) + actual_block_size;
					curr       = gen_pointer_add( curr, actual_block_size );
				}

				end            = gen_rcast( gen_uptr*, curr);
				* end          = gen_scast( gen_uptr, NULL);
				pool->FreeList = pool->PhysicalStart;
			}
			break;

		case EAllocation_RESIZE :
			// NOTE: Cannot gen_resize
			GEN_PANIC( "You cannot gen_resize something allocated by with a pool." );
			break;
	}

	return ptr;
}

gen_Pool gen_pool_init_align( gen_AllocatorInfo backing, gen_ssize num_blocks, gen_ssize block_size, gen_ssize block_align )
{
	gen_Pool pool = {};

	gen_ssize    actual_block_size, gen_pool_size, block_index;
	void *data, *curr;
	gen_uptr* end;

	gen_zero_item( &pool );

	pool.Backing     = backing;
	pool.BlockSize   = block_size;
	pool.BlockAlign  = block_align;
	pool.NumBlocks   = num_blocks;

	actual_block_size = block_size + block_align;
	gen_pool_size         = num_blocks * actual_block_size;

	data = gen_alloc_align( backing, gen_pool_size, block_align );

	// NOTE: Init intrusive freelist
	curr = data;
	for ( block_index = 0; block_index < num_blocks - 1; block_index++ )
	{
		gen_uptr* next = ( gen_uptr* ) curr;
		*next      = ( gen_uptr  ) curr + actual_block_size;
		curr       = gen_pointer_add( curr, actual_block_size );
	}

	end  =  ( gen_uptr* ) curr;
	*end =  ( gen_uptr )  NULL;

	pool.PhysicalStart = data;
	pool.FreeList      = data;

	return pool;
}

void gen_pool_clear(gen_Pool* pool)
{
	gen_ssize actual_block_size, block_index;
	void* curr;
	gen_uptr* end;

	actual_block_size = pool->BlockSize + pool->BlockAlign;

	curr = pool->PhysicalStart;
	for ( block_index = 0; block_index < pool->NumBlocks - 1; block_index++ )
	{
		gen_uptr* next = ( gen_uptr* ) curr;
		*next      = ( gen_uptr  ) curr + actual_block_size;
		curr       = gen_pointer_add( curr, actual_block_size );
	}

	end  =  ( gen_uptr* ) curr;
	*end =  ( gen_uptr )  NULL;

	pool->FreeList = pool->PhysicalStart;
}

#pragma endregion Memory

#pragma region Hashing

gen_global gen_u32 const gen__crc32_table[ 256 ] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd,
	0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7, 0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
	0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59, 0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f,
	0x9fbfe4a5, 0xe8b8d433, 0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65, 0x4db26158, 0x3ab551ce,
	0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f, 0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1, 0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0,
	0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703,
	0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d, 0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
	0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777, 0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5,
	0x47b2cf7f, 0x30b5ffe9, 0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

gen_u32 gen_crc32( void const* data, gen_ssize len )
{
	gen_ssize        remaining;
	gen_u32       result = ~( gen_scast( gen_u32, 0) );
	gen_u8 const* c      = gen_rcast( gen_u8 const*, data);
	for ( remaining = len; remaining--; c++ )
		result = ( result >> 8 ) ^ ( gen__crc32_table[ ( result ^ *c ) & 0xff ] );
	return ~result;
}

gen_global gen_u64 const gen__crc64_table[ 256 ] = {
	0x0000000000000000ull, 0x7ad870c830358979ull, 0xf5b0e190606b12f2ull, 0x8f689158505e9b8bull, 0xc038e5739841b68full, 0xbae095bba8743ff6ull, 0x358804e3f82aa47dull,
	0x4f50742bc81f2d04ull, 0xab28ecb46814fe75ull, 0xd1f09c7c5821770cull, 0x5e980d24087fec87ull, 0x24407dec384a65feull, 0x6b1009c7f05548faull, 0x11c8790fc060c183ull,
	0x9ea0e857903e5a08ull, 0xe478989fa00bd371ull, 0x7d08ff3b88be6f81ull, 0x07d08ff3b88be6f8ull, 0x88b81eabe8d57d73ull, 0xf2606e63d8e0f40aull, 0xbd301a4810ffd90eull,
	0xc7e86a8020ca5077ull, 0x4880fbd87094cbfcull, 0x32588b1040a14285ull, 0xd620138fe0aa91f4ull, 0xacf86347d09f188dull, 0x2390f21f80c18306ull, 0x594882d7b0f40a7full,
	0x1618f6fc78eb277bull, 0x6cc0863448deae02ull, 0xe3a8176c18803589ull, 0x997067a428b5bcf0ull, 0xfa11fe77117cdf02ull, 0x80c98ebf2149567bull, 0x0fa11fe77117cdf0ull,
	0x75796f2f41224489ull, 0x3a291b04893d698dull, 0x40f16bccb908e0f4ull, 0xcf99fa94e9567b7full, 0xb5418a5cd963f206ull, 0x513912c379682177ull, 0x2be1620b495da80eull,
	0xa489f35319033385ull, 0xde51839b2936bafcull, 0x9101f7b0e12997f8ull, 0xebd98778d11c1e81ull, 0x64b116208142850aull, 0x1e6966e8b1770c73ull, 0x8719014c99c2b083ull,
	0xfdc17184a9f739faull, 0x72a9e0dcf9a9a271ull, 0x08719014c99c2b08ull, 0x4721e43f0183060cull, 0x3df994f731b68f75ull, 0xb29105af61e814feull, 0xc849756751dd9d87ull,
	0x2c31edf8f1d64ef6ull, 0x56e99d30c1e3c78full, 0xd9810c6891bd5c04ull, 0xa3597ca0a188d57dull, 0xec09088b6997f879ull, 0x96d1784359a27100ull, 0x19b9e91b09fcea8bull,
	0x636199d339c963f2ull, 0xdf7adabd7a6e2d6full, 0xa5a2aa754a5ba416ull, 0x2aca3b2d1a053f9dull, 0x50124be52a30b6e4ull, 0x1f423fcee22f9be0ull, 0x659a4f06d21a1299ull,
	0xeaf2de5e82448912ull, 0x902aae96b271006bull, 0x74523609127ad31aull, 0x0e8a46c1224f5a63ull, 0x81e2d7997211c1e8ull, 0xfb3aa75142244891ull, 0xb46ad37a8a3b6595ull,
	0xceb2a3b2ba0eececull, 0x41da32eaea507767ull, 0x3b024222da65fe1eull, 0xa2722586f2d042eeull, 0xd8aa554ec2e5cb97ull, 0x57c2c41692bb501cull, 0x2d1ab4dea28ed965ull,
	0x624ac0f56a91f461ull, 0x1892b03d5aa47d18ull, 0x97fa21650afae693ull, 0xed2251ad3acf6feaull, 0x095ac9329ac4bc9bull, 0x7382b9faaaf135e2ull, 0xfcea28a2faafae69ull,
	0x8632586aca9a2710ull, 0xc9622c4102850a14ull, 0xb3ba5c8932b0836dull, 0x3cd2cdd162ee18e6ull, 0x460abd1952db919full, 0x256b24ca6b12f26dull, 0x5fb354025b277b14ull,
	0xd0dbc55a0b79e09full, 0xaa03b5923b4c69e6ull, 0xe553c1b9f35344e2ull, 0x9f8bb171c366cd9bull, 0x10e3202993385610ull, 0x6a3b50e1a30ddf69ull, 0x8e43c87e03060c18ull,
	0xf49bb8b633338561ull, 0x7bf329ee636d1eeaull, 0x012b592653589793ull, 0x4e7b2d0d9b47ba97ull, 0x34a35dc5ab7233eeull, 0xbbcbcc9dfb2ca865ull, 0xc113bc55cb19211cull,
	0x5863dbf1e3ac9decull, 0x22bbab39d3991495ull, 0xadd33a6183c78f1eull, 0xd70b4aa9b3f20667ull, 0x985b3e827bed2b63ull, 0xe2834e4a4bd8a21aull, 0x6debdf121b863991ull,
	0x1733afda2bb3b0e8ull, 0xf34b37458bb86399ull, 0x8993478dbb8deae0ull, 0x06fbd6d5ebd3716bull, 0x7c23a61ddbe6f812ull, 0x3373d23613f9d516ull, 0x49aba2fe23cc5c6full,
	0xc6c333a67392c7e4ull, 0xbc1b436e43a74e9dull, 0x95ac9329ac4bc9b5ull, 0xef74e3e19c7e40ccull, 0x601c72b9cc20db47ull, 0x1ac40271fc15523eull, 0x5594765a340a7f3aull,
	0x2f4c0692043ff643ull, 0xa02497ca54616dc8ull, 0xdafce7026454e4b1ull, 0x3e847f9dc45f37c0ull, 0x445c0f55f46abeb9ull, 0xcb349e0da4342532ull, 0xb1eceec59401ac4bull,
	0xfebc9aee5c1e814full, 0x8464ea266c2b0836ull, 0x0b0c7b7e3c7593bdull, 0x71d40bb60c401ac4ull, 0xe8a46c1224f5a634ull, 0x927c1cda14c02f4dull, 0x1d148d82449eb4c6ull,
	0x67ccfd4a74ab3dbfull, 0x289c8961bcb410bbull, 0x5244f9a98c8199c2ull, 0xdd2c68f1dcdf0249ull, 0xa7f41839ecea8b30ull, 0x438c80a64ce15841ull, 0x3954f06e7cd4d138ull,
	0xb63c61362c8a4ab3ull, 0xcce411fe1cbfc3caull, 0x83b465d5d4a0eeceull, 0xf96c151de49567b7ull, 0x76048445b4cbfc3cull, 0x0cdcf48d84fe7545ull, 0x6fbd6d5ebd3716b7ull,
	0x15651d968d029fceull, 0x9a0d8ccedd5c0445ull, 0xe0d5fc06ed698d3cull, 0xaf85882d2576a038ull, 0xd55df8e515432941ull, 0x5a3569bd451db2caull, 0x20ed197575283bb3ull,
	0xc49581ead523e8c2ull, 0xbe4df122e51661bbull, 0x3125607ab548fa30ull, 0x4bfd10b2857d7349ull, 0x04ad64994d625e4dull, 0x7e7514517d57d734ull, 0xf11d85092d094cbfull,
	0x8bc5f5c11d3cc5c6ull, 0x12b5926535897936ull, 0x686de2ad05bcf04full, 0xe70573f555e26bc4ull, 0x9ddd033d65d7e2bdull, 0xd28d7716adc8cfb9ull, 0xa85507de9dfd46c0ull,
	0x273d9686cda3dd4bull, 0x5de5e64efd965432ull, 0xb99d7ed15d9d8743ull, 0xc3450e196da80e3aull, 0x4c2d9f413df695b1ull, 0x36f5ef890dc31cc8ull, 0x79a59ba2c5dc31ccull,
	0x037deb6af5e9b8b5ull, 0x8c157a32a5b7233eull, 0xf6cd0afa9582aa47ull, 0x4ad64994d625e4daull, 0x300e395ce6106da3ull, 0xbf66a804b64ef628ull, 0xc5bed8cc867b7f51ull,
	0x8aeeace74e645255ull, 0xf036dc2f7e51db2cull, 0x7f5e4d772e0f40a7ull, 0x05863dbf1e3ac9deull, 0xe1fea520be311aafull, 0x9b26d5e88e0493d6ull, 0x144e44b0de5a085dull,
	0x6e963478ee6f8124ull, 0x21c640532670ac20ull, 0x5b1e309b16452559ull, 0xd476a1c3461bbed2ull, 0xaeaed10b762e37abull, 0x37deb6af5e9b8b5bull, 0x4d06c6676eae0222ull,
	0xc26e573f3ef099a9ull, 0xb8b627f70ec510d0ull, 0xf7e653dcc6da3dd4ull, 0x8d3e2314f6efb4adull, 0x0256b24ca6b12f26ull, 0x788ec2849684a65full, 0x9cf65a1b368f752eull,
	0xe62e2ad306bafc57ull, 0x6946bb8b56e467dcull, 0x139ecb4366d1eea5ull, 0x5ccebf68aecec3a1ull, 0x2616cfa09efb4ad8ull, 0xa97e5ef8cea5d153ull, 0xd3a62e30fe90582aull,
	0xb0c7b7e3c7593bd8ull, 0xca1fc72bf76cb2a1ull, 0x45775673a732292aull, 0x3faf26bb9707a053ull, 0x70ff52905f188d57ull, 0x0a2722586f2d042eull, 0x854fb3003f739fa5ull,
	0xff97c3c80f4616dcull, 0x1bef5b57af4dc5adull, 0x61372b9f9f784cd4ull, 0xee5fbac7cf26d75full, 0x9487ca0fff135e26ull, 0xdbd7be24370c7322ull, 0xa10fceec0739fa5bull,
	0x2e675fb4576761d0ull, 0x54bf2f7c6752e8a9ull, 0xcdcf48d84fe75459ull, 0xb71738107fd2dd20ull, 0x387fa9482f8c46abull, 0x42a7d9801fb9cfd2ull, 0x0df7adabd7a6e2d6ull,
	0x772fdd63e7936bafull, 0xf8474c3bb7cdf024ull, 0x829f3cf387f8795dull, 0x66e7a46c27f3aa2cull, 0x1c3fd4a417c62355ull, 0x935745fc4798b8deull, 0xe98f353477ad31a7ull,
	0xa6df411fbfb21ca3ull, 0xdc0731d78f8795daull, 0x536fa08fdfd90e51ull, 0x29b7d047efec8728ull,
};

gen_u64 gen_crc64( void const* data, gen_ssize len )
{
	gen_ssize        remaining;
	gen_u64       result = ( gen_scast( gen_u64, 0) );
	gen_u8 const* c      = gen_rcast( gen_u8 const*, data);
	for ( remaining = len; remaining--; c++ )
		result = ( result >> 8 ) ^ ( gen__crc64_table[ ( result ^ *c ) & 0xff ] );
	return result;
}

#pragma endregion Hashing

#pragma region gen_StrBuilder

gen_StrBuilder gen_strbuilder_make_length( gen_AllocatorInfo allocator, char const* str, gen_ssize length )
{
	gen_ssize const header_size = sizeof( gen_StrBuilderHeader );

	gen_s32   alloc_size = header_size + length + 1;
	void* allocation = gen_alloc( allocator, alloc_size );

	if ( allocation == gen_nullptr ) {
		gen_StrBuilder null_string = {gen_nullptr};
		return null_string;
	}

	gen_StrBuilderHeader*
	header = gen_rcast(gen_StrBuilderHeader*, allocation);
	header->Allocator = allocator;
	header->Capacity  = length;
	header->Length    = length;

	gen_StrBuilder  result = { gen_rcast( char*, allocation) + header_size };

	if ( length && str )
		gen_mem_copy( result, str, length );
	else
		gen_mem_set( result, 0, alloc_size - header_size );

	result[ length ] = '\0';

	return result;
}

gen_StrBuilder gen_strbuilder_make_reserve( gen_AllocatorInfo allocator, gen_ssize capacity )
{
	gen_ssize const header_size = sizeof( gen_StrBuilderHeader );

	gen_s32   alloc_size = header_size + capacity + 1;
	void* allocation = gen_alloc( allocator, alloc_size );

	if ( allocation == gen_nullptr ) {
		gen_StrBuilder null_string = {gen_nullptr};
		return null_string;
	}
	gen_mem_set( allocation, 0, alloc_size );

	gen_StrBuilderHeader*
	header            = gen_rcast(gen_StrBuilderHeader*, allocation);
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Length    = 0;

	gen_StrBuilder result = { gen_rcast(char*, allocation) + header_size };
	return result;
}

bool gen_strbuilder_make_space_for(gen_StrBuilder* str, char const* to_append, gen_ssize add_len)
{
	gen_ssize available = gen_strbuilder_avail_space(* str);

	if (available >= add_len) {
		return true;
	}
	else
	{
		gen_ssize new_len, old_size, new_size;
		void* ptr;
		void* new_ptr;

		gen_AllocatorInfo allocator = gen_strbuilder_get_header(* str)->Allocator;
		gen_StrBuilderHeader* header    = gen_nullptr;

		new_len  = gen_strbuilder_grow_formula(gen_strbuilder_length(* str) + add_len);
		ptr      = gen_strbuilder_get_header(* str);
		old_size = gen_size_of(gen_StrBuilderHeader) + gen_strbuilder_length(* str) + 1;
		new_size = gen_size_of(gen_StrBuilderHeader) + new_len + 1;

		new_ptr = gen_resize(allocator, ptr, old_size, new_size);

		if (new_ptr == gen_nullptr)
			return false;

		header = gen_rcast(gen_StrBuilderHeader*, new_ptr);
		header->Allocator = allocator;
		header->Capacity  = new_len;

		char** Data = gen_rcast(char**, str);
		* Data = gen_rcast(char*, header + 1);

		return true;
	}
}

bool gen_strbuilder_append_c_str_len(gen_StrBuilder* str, char const* gen_c_str_to_append, gen_ssize append_length)
{
	GEN_ASSERT(str != gen_nullptr);
	if ( gen_rcast(gen_sptr, gen_c_str_to_append) > 0)
	{
		gen_ssize curr_len = gen_strbuilder_length(* str);

		if ( ! gen_strbuilder_make_space_for(str, gen_c_str_to_append, append_length))
			return false;

		gen_StrBuilderHeader* header = gen_strbuilder_get_header(* str);

		char* Data = * str;
		gen_mem_copy( Data + curr_len, gen_c_str_to_append, append_length);

		Data[curr_len + append_length] = '\0';

		header->Length = curr_len + append_length;
	}
	return gen_c_str_to_append != gen_nullptr;
}

void gen_strbuilder_trim(gen_StrBuilder str, char const* cut_set)
{
	gen_ssize len = 0;

	char* start_pos = str;
	char* gen_end_pos   = gen_scast(char*, str) + gen_strbuilder_length(str) - 1;

	while (start_pos <= gen_end_pos && gen_char_first_occurence(cut_set, *start_pos))
	start_pos++;

	while (gen_end_pos > start_pos && gen_char_first_occurence(cut_set, *gen_end_pos))
	gen_end_pos--;

	len = gen_scast(gen_ssize, (start_pos > gen_end_pos) ? 0 : ((gen_end_pos - start_pos) + 1));

	if (str != start_pos)
		gen_mem_move(str, start_pos, len);

	str[len] = '\0';

	gen_strbuilder_get_header(str)->Length = len;
}

gen_StrBuilder gen_strbuilder_visualize_whitespace(gen_StrBuilder const str)
{
	gen_StrBuilderHeader* header = (gen_StrBuilderHeader*)(gen_scast(char const*, str) - sizeof(gen_StrBuilderHeader));
	gen_StrBuilder        result = gen_strbuilder_make_reserve(header->Allocator, gen_strbuilder_length(str) * 2); // Assume worst case for space requirements.

	for (char const* c = gen_strbuilder_begin(str); c != gen_strbuilder_end(str); c = gen_strbuilder_next(str, c))
	switch ( * c )
	{
		case ' ':
			gen_strbuilder_append_str(& result, gen_txt("·"));
		break;
		case '\t':
			gen_strbuilder_append_str(& result, gen_txt("→"));
		break;
		case '\n':
			gen_strbuilder_append_str(& result, gen_txt("↵"));
		break;
		case '\r':
			gen_strbuilder_append_str(& result, gen_txt("⏎"));
		break;
		case '\v':
			gen_strbuilder_append_str(& result, gen_txt("⇕"));
		break;
		case '\f':
			gen_strbuilder_append_str(& result, gen_txt("⌂"));
		break;
		default:
			gen_strbuilder_append_char(& result, * c);
		break;
	}

	return result;
}

#pragma endregion gen_StrBuilder

#pragma region File Handling

#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )

gen_internal
wchar_t* gen__alloc_utf8_to_ucs2( gen_AllocatorInfo a, char const* text, gen_ssize* w_len_ )
{
	wchar_t* w_text = NULL;
	gen_ssize       len = 0, w_len = 0, w_len1 = 0;
	if ( text == NULL )
	{
		if ( w_len_ )
			*w_len_ = w_len;
		return NULL;
	}
	len = gen_c_str_len( text );
	if ( len == 0 )
	{
		if ( w_len_ )
			*w_len_ = w_len;
		return NULL;
	}
	w_len = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, text, gen_scast( int, len), NULL, 0 );
	if ( w_len == 0 )
	{
		if ( w_len_ )
			*w_len_ = w_len;
		return NULL;
	}
	w_text = gen_alloc_array( a, wchar_t, w_len + 1 );
	w_len1 = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, text, gen_scast( int, len), w_text, gen_scast( int, w_len) );
	if ( w_len1 == 0 )
	{
		gen_allocator_free( a, w_text );
		if ( w_len_ )
			*w_len_ = 0;
		return NULL;
	}
	w_text[ w_len ] = 0;
	if ( w_len_ )
		*w_len_ = w_len;
	return w_text;
}

gen_internal
GEN_FILE_SEEK_PROC( gen__win32_file_seek )
{
	LARGE_INTEGER li_offset;
	li_offset.QuadPart = offset;
	if ( ! SetFilePointerEx( fd.p, li_offset, &li_offset, whence ) )
	{
		return false;
	}

	if ( new_offset )
		*new_offset = li_offset.QuadPart;
	return true;
}

gen_internal
GEN_FILE_READ_AT_PROC( gen__win32_file_read )
{
	// unused( stop_at_newline );
	gen_b32 result = false;
	gen__win32_file_seek( fd, offset, ESeekWhence_BEGIN, NULL );
	DWORD size_ = gen_scast( DWORD, ( size > GEN_I32_MAX ? GEN_I32_MAX : size ));
	DWORD bytes_read_;
	if ( ReadFile( fd.p, buffer, size_, &bytes_read_, NULL ) )
	{
		if ( bytes_read )
			*bytes_read = bytes_read_;
		result = true;
	}

	return result;
}

gen_internal
GEN_FILE_WRITE_AT_PROC( gen__win32_file_write )
{
	DWORD size_ = gen_scast( DWORD, ( size > GEN_I32_MAX ? GEN_I32_MAX : size ));
	DWORD bytes_written_;
	gen__win32_file_seek( fd, offset, ESeekWhence_BEGIN, NULL );
	if ( WriteFile( fd.p, buffer, size_, &bytes_written_, NULL ) )
	{
		if ( bytes_written )
			*bytes_written = bytes_written_;
		return true;
	}
	return false;
}

gen_internal
GEN_FILE_CLOSE_PROC( gen__win32_file_close )
{
	CloseHandle( fd.p );
}

gen_FileOperations const default_file_operations = { gen__win32_file_read, gen__win32_file_write, gen__win32_file_seek, gen__win32_file_close };

gen_neverinline
GEN_FILE_OPEN_PROC( gen__win32_file_open )
{
	DWORD    desired_access;
	DWORD    creation_disposition;
	void*    handle;
	wchar_t* w_text;

	switch ( mode & GEN_FILE_MODES )
	{
		case EFileMode_READ :
			desired_access       = GENERIC_READ;
			creation_disposition = OPEN_EXISTING;
			break;
		case EFileMode_WRITE :
			desired_access       = GENERIC_WRITE;
			creation_disposition = CREATE_ALWAYS;
			break;
		case EFileMode_APPEND :
			desired_access       = GENERIC_WRITE;
			creation_disposition = OPEN_ALWAYS;
			break;
		case EFileMode_READ | EFileMode_RW :
			desired_access       = GENERIC_READ | GENERIC_WRITE;
			creation_disposition = OPEN_EXISTING;
			break;
		case EFileMode_WRITE | EFileMode_RW :
			desired_access       = GENERIC_READ | GENERIC_WRITE;
			creation_disposition = CREATE_ALWAYS;
			break;
		case EFileMode_APPEND | EFileMode_RW :
			desired_access       = GENERIC_READ | GENERIC_WRITE;
			creation_disposition = OPEN_ALWAYS;
			break;
		default :
			GEN_PANIC( "Invalid file mode" );
			return EFileError_INVALID;
	}

	w_text = gen__alloc_utf8_to_ucs2( gen_heap(), filename, NULL );
	handle = CreateFileW( w_text, desired_access, FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, creation_disposition, FILE_ATTRIBUTE_NORMAL, NULL );

	gen_allocator_free( gen_heap(), w_text );

	if ( handle == INVALID_HANDLE_VALUE )
	{
		DWORD err = GetLastError();
		switch ( err )
		{
			case ERROR_FILE_NOT_FOUND :
				return EFileError_NOT_EXISTS;
			case ERROR_FILE_EXISTS :
				return EFileError_EXISTS;
			case ERROR_ALREADY_EXISTS :
				return EFileError_EXISTS;
			case ERROR_ACCESS_DENIED :
				return EFileError_PERMISSION;
		}
		return EFileError_INVALID;
	}

	if ( mode & EFileMode_APPEND )
	{
		LARGE_INTEGER offset = { { 0 } };
		if ( ! SetFilePointerEx( handle, offset, NULL, ESeekWhence_END ) )
		{
			CloseHandle( handle );
			return EFileError_INVALID;
		}
	}

	fd->p = handle;
	*ops  = default_file_operations;
	return EFileError_NONE;
}

#else    // POSIX
#	include <fcntl.h>

gen_internal
GEN_FILE_SEEK_PROC( gen__posix_file_seek )
{
#	if defined( GEN_SYSTEM_OSX )
	gen_s64 res = lseek( fd.i, offset, whence );
#	else    // TODO(ZaKlaus): @fixme lseek64
	gen_s64 res = lseek( fd.i, offset, whence );
#	endif
	if ( res < 0 )
		return false;
	if ( new_offset )
		*new_offset = res;
	return true;
}

gen_internal
GEN_FILE_READ_AT_PROC( gen__posix_file_read )
{
	unused( stop_at_newline );
	gen_ssize res = pread( fd.i, buffer, size, offset );
	if ( res < 0 )
		return false;
	if ( bytes_read )
		*bytes_read = res;
	return true;
}

gen_internal
GEN_FILE_WRITE_AT_PROC( gen__posix_file_write )
{
	gen_ssize  res;
	gen_s64 curr_offset = 0;
	gen__posix_file_seek( fd, 0, ESeekWhence_CURRENT, &curr_offset );
	if ( curr_offset == offset )
	{
		// NOTE: Writing to stdout et al. doesn't like pwrite for numerous reasons
		res = write( gen_scast( int, fd.i), buffer, size );
	}
	else
	{
		res = pwrite( gen_scast( int, fd.i), buffer, size, offset );
	}
	if ( res < 0 )
		return false;
	if ( bytes_written )
		*bytes_written = res;
	return true;
}

gen_internal
GEN_FILE_CLOSE_PROC( gen__posix_file_close )
{
	close( fd.i );
}

gen_FileOperations const default_file_operations = { gen__posix_file_read, gen__posix_file_write, gen__posix_file_seek, gen__posix_file_close };

gen_neverinline
GEN_FILE_OPEN_PROC( gen__posix_file_open )
{
	gen_s32 os_mode;
	switch ( mode & GEN_FILE_MODES )
	{
		case EFileMode_READ :
			os_mode = O_RDONLY;
			break;
		case EFileMode_WRITE :
			os_mode = O_WRONLY | O_CREAT | O_TRUNC;
			break;
		case EFileMode_APPEND :
			os_mode = O_WRONLY | O_APPEND | O_CREAT;
			break;
		case EFileMode_READ | EFileMode_RW :
			os_mode = O_RDWR;
			break;
		case EFileMode_WRITE | EFileMode_RW :
			os_mode = O_RDWR | O_CREAT | O_TRUNC;
			break;
		case EFileMode_APPEND | EFileMode_RW :
			os_mode = O_RDWR | O_APPEND | O_CREAT;
			break;
		default :
			GEN_PANIC( "Invalid file mode" );
			return EFileError_INVALID;
	}

	fd->i = open( filename, os_mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH );
	if ( fd->i < 0 )
	{
		// TODO : More file errors
		return EFileError_INVALID;
	}

	*ops = default_file_operations;
	return EFileError_NONE;
}

// POSIX
#endif

gen_internal void gen__dirinfo_free_entry( gen_DirEntry* entry );

// TODO(zpl) : Is this a bad idea?
gen_global gen_b32      gen__std_file_set                     = false;
gen_global gen_FileInfo _std_files[ EFileStandard_COUNT ] = {
{
	{ gen_nullptr, gen_nullptr, gen_nullptr, gen_nullptr },
	{ gen_nullptr },
	0,
	gen_nullptr,
	0,
	gen_nullptr
} };

#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )

gen_FileInfo* gen_file_get_standard( gen_FileStandardType std )
{
	if ( ! gen__std_file_set )
	{
#	define GEN__SET_STD_FILE( type, v ) \
		_std_files[ type ].fd.p = v;    \
		_std_files[ type ].ops  = default_file_operations
		GEN__SET_STD_FILE( EFileStandard_INPUT, GetStdHandle( STD_INPUT_HANDLE ) );
		GEN__SET_STD_FILE( EFileStandard_OUTPUT, GetStdHandle( STD_OUTPUT_HANDLE ) );
		GEN__SET_STD_FILE( EFileStandard_ERROR, GetStdHandle( STD_ERROR_HANDLE ) );
#	undef GEN__SET_STD_FILE
		gen__std_file_set = true;
	}
	return &_std_files[ std ];
}

#else    // POSIX

gen_FileInfo* gen_file_get_standard( gen_FileStandardType std )
{
	if ( ! gen__std_file_set )
	{
#	define GEN__SET_STD_FILE( type, v ) \
		_std_files[ type ].fd.i = v;     \
		_std_files[ type ].ops  = default_file_operations
		GEN__SET_STD_FILE( EFileStandard_INPUT, 0 );
		GEN__SET_STD_FILE( EFileStandard_OUTPUT, 1 );
		GEN__SET_STD_FILE( EFileStandard_ERROR, 2 );
#	undef GEN__SET_STD_FILE
		gen__std_file_set = true;
	}
	return &_std_files[ std ];
}

#endif

gen_FileError gen_file_close( gen_FileInfo* f )
{
	if ( ! f )
		return EFileError_INVALID;

	if ( f->filename )
		gen_allocator_free( gen_heap(), gen_ccast( char*, f->filename ));

#if defined( GEN_SYSTEM_WINDOWS )
	if ( f->fd.p == INVALID_HANDLE_VALUE )
		return EFileError_INVALID;
#else
	if ( f->fd.i < 0 )
		return EFileError_INVALID;
#endif

	if ( f->is_temp )
	{
		f->ops.close( f->fd );
		return EFileError_NONE;
	}

	if ( ! f->ops.read_at )
		f->ops = default_file_operations;
	f->ops.close( f->fd );

#if 0
	if ( f->Dir )
	{
		gen__dirinfo_free_entry( f->Dir );
		gen_mfree( f->Dir );
		f->Dir = NULL;
	}
#endif

	return EFileError_NONE;
}

gen_FileError gen_file_new( gen_FileInfo* f, gen_FileDescriptor fd, gen_FileOperations ops, char const* filename )
{
	gen_FileError err = EFileError_NONE;
	gen_ssize        len = gen_c_str_len( filename );

	f->ops             = ops;
	f->fd              = fd;
	f->dir             = gen_nullptr;
	f->last_write_time = 0;
	f->filename        = gen_alloc_array( gen_heap(), char, len + 1 );
	gen_mem_copy( gen_ccast( char*, f->filename), gen_ccast( char*, filename), len + 1 );

	return err;
}

gen_FileError gen_file_open( gen_FileInfo* f, char const* filename )
{
	return gen_file_open_mode( f, EFileMode_READ, filename );
}

gen_FileError gen_file_open_mode( gen_FileInfo* f, gen_FileMode mode, char const* filename )
{
	gen_FileInfo gen_file_ =
	{
		{ gen_nullptr, gen_nullptr, gen_nullptr, gen_nullptr },
		{ gen_nullptr },
		0,
		gen_nullptr,
		0,
		gen_nullptr
		};

	*f = gen_file_;
	gen_FileError err;

#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )
	err = gen__win32_file_open( &f->fd, &f->ops, mode, filename );
#else
	err = gen__posix_file_open( &f->fd, &f->ops, mode, filename );
#endif

	if ( err == EFileError_NONE )
		return gen_file_new( f, f->fd, f->ops, filename );

	return err;
}

gen_s64 gen_file_size( gen_FileInfo* f )
{
	gen_s64 size        = 0;
	gen_s64 prev_offset = gen_file_tell( f );

	gen_file_seek_to_end( f );
	size = gen_file_tell( f );

	gen_file_seek( f, prev_offset );

	return size;
}

FileContents gen_file_read_contents( gen_AllocatorInfo a, gen_b32 zero_terminate, char const* filepath )
{
	FileContents result;
	gen_FileInfo     file  ;

	result.allocator = a;

	if ( gen_file_open( &file, filepath ) == EFileError_NONE )
	{
		gen_ssize fsize = gen_scast( gen_ssize , gen_file_size( &file ));
		if ( fsize > 0 )
		{
			result.data = gen_alloc( a, zero_terminate ? fsize + 1 : fsize );
			result.size = fsize;
			gen_file_read_at( &file, result.data, result.size, 0 );
			if ( zero_terminate )
			{
				gen_u8* str      = gen_rcast( gen_u8*, result.data);
				str[ fsize ] = '\0';
			}
		}
		gen_file_close( &file );
	}

	return result;
}

typedef struct gen__memory_fd gen__memory_fd;
struct gen__memory_fd
{
	gen_u8            magic;
	gen_u8*           buf;    //< zpl_array OR plain buffer if we can't write
	gen_ssize         cursor;
	gen_AllocatorInfo allocator;

	FileStreamFlags flags;
	gen_ssize           cap;
};

#define GEN__FILE_STREAM_FD_MAGIC 37

gen_FileDescriptor gen__file_stream_fd_make( gen__memory_fd* d );
gen__memory_fd*    gen__file_stream_from_fd( gen_FileDescriptor fd );

inline
gen_FileDescriptor gen__file_stream_fd_make( gen__memory_fd* d )
{
	gen_FileDescriptor fd = { 0 };
	fd.p              = ( void* )d;
	return fd;
}

inline
gen__memory_fd* gen__file_stream_from_fd( gen_FileDescriptor fd )
{
	gen__memory_fd* d = ( gen__memory_fd* )fd.p;
	GEN_ASSERT( d->magic == GEN__FILE_STREAM_FD_MAGIC );
	return d;
}

gen_b8 gen_file_stream_new( gen_FileInfo* file, gen_AllocatorInfo allocator )
{
	GEN_ASSERT_NOT_NULL( file );

	gen__memory_fd* d = ( gen__memory_fd* )gen_alloc( allocator, gen_size_of( gen__memory_fd ) );

	if ( ! d )
		return false;

	gen_zero_item( file );
	d->magic     = GEN__FILE_STREAM_FD_MAGIC;
	d->allocator = allocator;
	d->flags     = EFileStream_CLONE_WRITABLE;
	d->cap       = 0;
	d->buf       = gen_array_init( gen_u8, allocator );

	if ( ! d->buf )
		return false;

	file->ops             = memory_file_operations;
	file->fd              = gen__file_stream_fd_make( d );
	file->dir             = NULL;
	file->last_write_time = 0;
	file->filename        = NULL;
	file->is_temp         = true;
	return true;
}

gen_b8 gen_file_stream_open( gen_FileInfo* file, gen_AllocatorInfo allocator, gen_u8* buffer, gen_ssize size, FileStreamFlags flags )
{
	GEN_ASSERT_NOT_NULL( file );
	gen__memory_fd* d = ( gen__memory_fd* )gen_alloc( allocator, gen_size_of( gen__memory_fd ) );
	if ( ! d )
		return false;
	gen_zero_item( file );
	d->magic     = GEN__FILE_STREAM_FD_MAGIC;
	d->allocator = allocator;
	d->flags     = flags;
	if ( d->flags & EFileStream_CLONE_WRITABLE )
	{
		gen_Array(gen_u8) arr = gen_array_init_reserve(gen_u8, allocator, size );
		d->buf = arr;

		if ( ! d->buf )
			return false;

		gen_mem_copy( d->buf, buffer, size );
		d->cap = size;

		gen_array_get_header(arr)->Num = size;
	}
	else
	{
		d->buf = buffer;
		d->cap = size;
	}
	file->ops             = memory_file_operations;
	file->fd              = gen__file_stream_fd_make( d );
	file->dir             = NULL;
	file->last_write_time = 0;
	file->filename        = NULL;
	file->is_temp         = true;
	return true;
}

gen_u8* gen_file_stream_buf( gen_FileInfo* file, gen_ssize* size )
{
	GEN_ASSERT_NOT_NULL( file );
	gen__memory_fd* d = gen__file_stream_from_fd( file->fd );
	if ( size )
		*size = d->cap;
	return d->buf;
}

gen_internal
GEN_FILE_SEEK_PROC( gen__memory_file_seek )
{
	gen__memory_fd* d      = gen__file_stream_from_fd( fd );
	gen_ssize          buflen = d->cap;

	if ( whence == ESeekWhence_BEGIN )
		d->cursor = 0;
	else if ( whence == ESeekWhence_END )
		d->cursor = buflen;

	d->cursor = gen_max( 0, gen_clamp( d->cursor + offset, 0, buflen ) );
	if ( new_offset )
		*new_offset = d->cursor;
	return true;
}

gen_internal
GEN_FILE_READ_AT_PROC( gen__memory_file_read )
{
	// unused( stop_at_newline );
	gen__memory_fd* d = gen__file_stream_from_fd( fd );
	gen_mem_copy( buffer, d->buf + offset, size );
	if ( bytes_read )
		*bytes_read = size;
	return true;
}

gen_internal
GEN_FILE_WRITE_AT_PROC( gen__memory_file_write )
{
	gen__memory_fd* d = gen__file_stream_from_fd( fd );

	if ( ! ( d->flags & ( EFileStream_CLONE_WRITABLE | EFileStream_WRITABLE ) ) )
		return false;

	gen_ssize buflen   = d->cap;
	gen_ssize extralen = gen_max( 0, size - ( buflen - offset ) );
	gen_ssize rwlen    = size - extralen;
	gen_ssize new_cap  = buflen + extralen;

	if ( d->flags & EFileStream_CLONE_WRITABLE )
	{
		gen_Array(gen_u8) arr = { d->buf };

		if ( gen_array_get_header(arr)->Capacity < gen_scast(gen_usize, new_cap) )
		{
			if ( ! gen_array_grow( & arr, ( gen_s64 )( new_cap ) ) )
				return false;
			d->buf = arr;
		}
	}

	gen_mem_copy( d->buf + offset, buffer, rwlen );

	if ( ( d->flags & EFileStream_CLONE_WRITABLE ) && extralen > 0 )
	{
		gen_Array(gen_u8) arr = { d->buf };

		gen_mem_copy( d->buf + offset + rwlen, gen_pointer_add_const( buffer, rwlen ), extralen );
		d->cap = new_cap;
		gen_array_get_header(arr)->Capacity = new_cap;
	}
	else
	{
		extralen = 0;
	}

	if ( bytes_written )
		*bytes_written = ( rwlen + extralen );
	return true;
}

gen_internal
GEN_FILE_CLOSE_PROC( gen__memory_file_close )
{
	gen__memory_fd*   d         = gen__file_stream_from_fd( fd );
	gen_AllocatorInfo allocator = d->allocator;

	if ( d->flags & EFileStream_CLONE_WRITABLE )
	{
		gen_Array(gen_u8) arr = { d->buf };
		gen_array_free(arr);
	}

	gen_allocator_free( allocator, d );
}

gen_FileOperations const memory_file_operations = { gen__memory_file_read, gen__memory_file_write, gen__memory_file_seek, gen__memory_file_close };

#pragma endregion File Handling

#pragma region Timing

#ifdef GEN_BENCHMARK
	#if defined( GEN_COMPILER_MSVC ) && ! defined( __clang__ )
	gen_u64 gen_read_cpu_time_stamp_counter( void )
	{
		return __rdtsc();
	}
	#elif defined( __i386__ )
	gen_u64 gen_read_cpu_time_stamp_counter( void )
	{
		gen_u64 x;
		__asm__ volatile( ".byte 0x0f, 0x31" : "=A"( x ) );
		return x;
	}
	#elif defined( __x86_64__ )
	gen_u64 gen_read_cpu_time_stamp_counter( void )
	{
		gen_u32 hi, lo;
		__asm__ __volatile__( "rdtsc" : "=a"( lo ), "=d"( hi ) );
		return gen_scast( gen_u64, lo ) | ( gen_scast( gen_u64, hi ) << 32 );
	}
	#elif defined( __powerpc__ )
	gen_u64 gen_read_cpu_time_stamp_counter( void )
	{
		gen_u64 result = 0;
		gen_u32 upper, lower, tmp;
		__asm__ volatile(
			"0:                   \n"
			"\tmftbu   %0         \n"
			"\tmftb    %1         \n"
			"\tmftbu   %2         \n"
			"\tcmpw    %2,%0      \n"
			"\tbne     0b         \n"
			: "=r"( upper ), "=r"( lower ), "=r"( tmp )
		);
		result = upper;
		result = result << 32;
		result = result | lower;

		return result;
	}
	#elif defined( GEN_SYSTEM_EMSCRIPTEN )
	gen_u64 gen_read_cpu_time_stamp_counter( void )
	{
		return ( gen_u64 )( emscripten_get_now() * 1e+6 );
	}
	#elif defined( GEN_CPU_ARM ) && ! defined( GEN_COMPILER_TINYC )
	gen_u64 gen_read_cpu_time_stamp_counter( void )
	{
	#	if defined( __aarch64__ )
		int64_t r = 0;
		asm volatile( "mrs %0, cntvct_el0" : "=r"( r ) );
	#	elif ( __ARM_ARCH >= 6 )
		uint32_t r = 0;
		uint32_t pmccntr;
		uint32_t pmuseren;
		uint32_t pmcntenset;

		// Read the user mode perf monitor counter access permissions.
		asm volatile( "mrc p15, 0, %0, c9, c14, 0" : "=r"( pmuseren ) );
		if ( pmuseren & 1 )
		{    // Allows reading perfmon counters for user mode code.
			asm volatile( "mrc p15, 0, %0, c9, c12, 1" : "=r"( pmcntenset ) );
			if ( pmcntenset & 0x80000000ul )
			{    // Is it counting?
				asm volatile( "mrc p15, 0, %0, c9, c13, 0" : "=r"( pmccntr ) );
				// The counter is set up to count every 64th cycle
				return ( ( int64_t )pmccntr ) * 64;    // Should optimize to << 6
			}
		}
	#	else
	#		error "No suitable method for gen_read_cpu_time_stamp_counter for this cpu type"
	#	endif

		return r;
	}
	#else
	gen_u64 gen_read_cpu_time_stamp_counter( void )
	{
		GEN_PANIC( "gen_read_cpu_time_stamp_counter is not supported on this particular setup" );
		return -0;
	}
	#endif

	#if defined( GEN_SYSTEM_WINDOWS ) || defined( GEN_SYSTEM_CYGWIN )

	gen_u64 gen_time_rel_ms( void )
	{
		gen_local_persist LARGE_INTEGER win32_perf_count_freq = {};
		gen_u64                         result;
		LARGE_INTEGER               counter;
		gen_local_persist LARGE_INTEGER win32_perf_counter = {};
		if ( ! win32_perf_count_freq.QuadPart )
		{
			QueryPerformanceFrequency( &win32_perf_count_freq );
			GEN_ASSERT( win32_perf_count_freq.QuadPart != 0 );
			QueryPerformanceCounter( &win32_perf_counter );
		}

		QueryPerformanceCounter( &counter );

		result = ( counter.QuadPart - win32_perf_counter.QuadPart ) * 1000 / ( win32_perf_count_freq.QuadPart );
		return result;
	}

	#else

	#	if defined( GEN_SYSTEM_LINUX ) || defined( GEN_SYSTEM_FREEBSD ) || defined( GEN_SYSTEM_OPENBSD ) || defined( GEN_SYSTEM_EMSCRIPTEN )
	gen_u64 gen__unix_gettime( void )
	{
		struct timespec t;
		gen_u64             result;

		clock_gettime( 1 /*CLOCK_MONOTONIC*/, &t );
		result = 1000 * t.tv_sec + 1.0e-6 * t.tv_nsec;
		return result;
	}
	#	endif

	gen_u64 gen_time_rel_ms( void )
	{
	#	if defined( GEN_SYSTEM_OSX )
		gen_u64 result;

		gen_local_persist gen_u64 timebase  = 0;
		gen_local_persist gen_u64 timestart = 0;

		if ( ! timestart )
		{
			mach_timebase_info_data_t tb = { 0 };
			mach_timebase_info( &tb );
			timebase   = tb.numer;
			timebase  /= tb.denom;
			timestart  = mach_absolute_time();
		}

		// NOTE: mach_absolute_time() returns things in nanoseconds
		result = 1.0e-6 * ( mach_absolute_time() - timestart ) * timebase;
		return result;
	#	else
		gen_local_persist gen_u64 unix_timestart = 0.0;

		if ( ! unix_timestart )
		{
			unix_timestart = gen__unix_gettime();
		}

		gen_u64 now = gen__unix_gettime();

		return ( now - unix_timestart );
	#	endif
	}
	#endif

	gen_f64 gen_time_rel( void )
	{
		return ( gen_f64 )( gen_time_rel_ms() * 1e-3 );
	}
#endif

#pragma endregion Timing

#pragma region ADT

#define gen__adt_fprintf(s_, fmt_, ...)                      \
	do                                                       \
	{                                                        \
		if (gen_c_str_fmt_file(s_, fmt_, ##__VA_ARGS__) < 0) \
			return EADT_ERROR_OUT_OF_MEMORY;                 \
	} while (0)

gen_u8 gen_adt_make_branch(gen_ADT_Node* node, gen_AllocatorInfo backing, char const* name, gen_b32 is_array)
{
	gen_ADT_Type type = EADT_TYPE_OBJECT;
	if (is_array)
		type = EADT_TYPE_ARRAY;

	gen_ADT_Node* parent = node->parent;
	gen_zero_item(node);

	node->type   = type;
	node->name   = name;
	node->parent = parent;
	node->nodes  = gen_array_init(gen_ADT_Node, backing);

	if (! node->nodes)
		return EADT_ERROR_OUT_OF_MEMORY;

	return 0;
}

gen_u8 gen_adt_destroy_branch(gen_ADT_Node* node)
{
	GEN_ASSERT_NOT_NULL(node);
	if ((node->type == EADT_TYPE_OBJECT || node->type == EADT_TYPE_ARRAY) && node->nodes)
	{
		for (gen_ssize i = 0; i < gen_scast(gen_ssize, gen_array_num(node->nodes)); ++i)
		{
			gen_adt_destroy_branch(node->nodes + i);
		}

		gen_array_free(node->nodes);
	}
	return 0;
}

gen_u8 gen_adt_make_leaf(gen_ADT_Node* node, char const* name, gen_ADT_Type type)
{
	GEN_ASSERT(type != EADT_TYPE_OBJECT && type != EADT_TYPE_ARRAY);

	gen_ADT_Node* parent = node->parent;
	gen_zero_item(node);

	node->type   = type;
	node->name   = name;
	node->parent = parent;
	return 0;
}

gen_ADT_Node* gen_adt_find(gen_ADT_Node* node, char const* name, gen_b32 deep_search)
{
	if (node->type != EADT_TYPE_OBJECT)
	{
		return NULL;
	}

	for (gen_ssize i = 0; i < gen_scast(gen_ssize, gen_array_num(node->nodes)); i++)
	{
		if (! gen_c_str_compare(node->nodes[i].name, name))
		{
			return (node->nodes + i);
		}
	}

	if (deep_search)
	{
		for (gen_ssize i = 0; i < gen_scast(gen_ssize, gen_array_num(node->nodes)); i++)
		{
			gen_ADT_Node* res = gen_adt_find(node->nodes + i, name, deep_search);

			if (res != NULL)
				return res;
		}
	}

	return NULL;
}

gen_internal gen_ADT_Node* gen__adt_get_value(gen_ADT_Node* node, char const* value)
{
	switch (node->type)
	{
		case EADT_TYPE_MULTISTRING:
		case EADT_TYPE_STRING:
		{
			if (node->string && ! gen_c_str_compare(node->string, value))
			{
				return node;
			}
		}
		break;
		case EADT_TYPE_INTEGER:
		case EADT_TYPE_REAL:
		{
			char         back[4096] = { 0 };
			gen_FileInfo tmp;

			/* allocate a file descriptor for a memory-mapped number to string conversion, input source buffer is not cloned, however. */
			gen_file_stream_open(&tmp, gen_heap(), (gen_u8*)back, gen_size_of(back), EFileStream_WRITABLE);
			gen_adt_print_number(&tmp, node);

			gen_ssize fsize = 0;
			gen_u8*   buf   = gen_file_stream_buf(&tmp, &fsize);

			if (! gen_c_str_compare((char const*)buf, value))
			{
				gen_file_close(&tmp);
				return node;
			}

			gen_file_close(&tmp);
		}
		break;
		default:
			break; /* node doesn't support value based lookup */
	}

	return NULL;
}

gen_internal gen_ADT_Node* gen__adt_get_field(gen_ADT_Node* node, char* name, char* value)
{
	for (gen_ssize i = 0; i < gen_scast(gen_ssize, gen_array_num(node->nodes)); i++)
	{
		if (! gen_c_str_compare(node->nodes[i].name, name))
		{
			gen_ADT_Node* child = &node->nodes[i];
			if (gen__adt_get_value(child, value))
			{
				return node; /* this object does contain a field of a specified value! */
			}
		}
	}

	return NULL;
}

gen_ADT_Node* gen_adt_query(gen_ADT_Node* node, char const* uri)
{
	GEN_ASSERT_NOT_NULL(uri);

	if (*uri == '/')
	{
		uri++;
	}

	if (*uri == 0)
	{
		return node;
	}

	if (! node || (node->type != EADT_TYPE_OBJECT && node->type != EADT_TYPE_ARRAY))
	{
		return NULL;
	}

#if defined EADT_URI_DEBUG || 0
	gen_c_str_fmt_out("uri: %s\n", uri);
#endif

	char *        p = (char*)uri, *b = p, *e = p;
	gen_ADT_Node* found_node = NULL;

	b                        = p;
	p = e     = (char*)gen_c_str_skip(p, '/');
	char* buf = gen_c_str_fmt_buf("%.*s", (int)(e - b), b);

	/* handle field value lookup */
	if (*b == '[')
	{
		char *l_p = buf + 1, *l_b = l_p, *l_e = l_p, *l_b2 = l_p, *l_e2 = l_p;
		l_e  = (char*)gen_c_str_skip(l_p, '=');
		l_e2 = (char*)gen_c_str_skip(l_p, ']');

		if ((! *l_e && node->type != EADT_TYPE_ARRAY) || ! *l_e2)
		{
			GEN_ASSERT_MSG(0, "Invalid field value lookup");
			return NULL;
		}

		*l_e2 = 0;

		/* [field=value] */
		if (*l_e)
		{
			*l_e = 0;
			l_b2 = l_e + 1;

			/* run a value comparison against our own fields */
			if (node->type == EADT_TYPE_OBJECT)
			{
				found_node = gen__adt_get_field(node, l_b, l_b2);
			}

			/* run a value comparison against any child that is an object node */
			else if (node->type == EADT_TYPE_ARRAY)
			{
				for (gen_ssize i = 0; i < gen_scast(gen_ssize, gen_array_num(node->nodes)); i++)
				{
					gen_ADT_Node* child = &node->nodes[i];
					if (child->type != EADT_TYPE_OBJECT)
					{
						continue;
					}

					found_node = gen__adt_get_field(child, l_b, l_b2);

					if (found_node)
						break;
				}
			}
		}
		/* [value] */
		else
		{
			for (gen_ssize i = 0; i < gen_scast(gen_ssize, gen_array_num(node->nodes)); i++)
			{
				gen_ADT_Node* child = &node->nodes[i];
				if (gen__adt_get_value(child, l_b2))
				{
					found_node = child;
					break; /* we found a matching value in array, ignore the rest of it */
				}
			}
		}

		/* go deeper if uri continues */
		if (*e)
		{
			return gen_adt_query(found_node, e + 1);
		}
	}
	/* handle field name lookup */
	else if (node->type == EADT_TYPE_OBJECT)
	{
		found_node = gen_adt_find(node, buf, false);

		/* go deeper if uri continues */
		if (*e)
		{
			return gen_adt_query(found_node, e + 1);
		}
	}
	/* handle array index lookup */
	else
	{
		gen_ssize idx = (gen_ssize)gen_c_str_to_i64(buf, NULL, 10);
		if (idx >= 0 && idx < gen_scast(gen_ssize, gen_array_num(node->nodes)))
		{
			found_node = &node->nodes[idx];

			/* go deeper if uri continues */
			if (*e)
			{
				return gen_adt_query(found_node, e + 1);
			}
		}
	}

	return found_node;
}

gen_ADT_Node* gen_adt_alloc_at(gen_ADT_Node* parent, gen_ssize index)
{
	if (! parent || (parent->type != EADT_TYPE_OBJECT && parent->type != EADT_TYPE_ARRAY))
	{
		return NULL;
	}

	if (! parent->nodes)
		return NULL;

	if (index < 0 || index > gen_scast(gen_ssize, gen_array_num(parent->nodes)))
		return NULL;

	gen_ADT_Node o = { 0 };
	o.parent       = parent;
	if (! gen_array_append_at(parent->nodes, o, index))
		return NULL;

	gen_ADT_Node* node = &parent->nodes[index];
	return node;
}

gen_ADT_Node* gen_adt_alloc(gen_ADT_Node* parent)
{
	if (! parent || (parent->type != EADT_TYPE_OBJECT && parent->type != EADT_TYPE_ARRAY))
	{
		return NULL;
	}

	if (! parent->nodes)
		return NULL;

	return gen_adt_alloc_at(parent, gen_array_num(parent->nodes));
}

gen_b8 gen_adt_set_obj(gen_ADT_Node* obj, char const* name, gen_AllocatorInfo backing)
{
	return gen_adt_make_branch(obj, backing, name, 0);
}

gen_b8 gen_adt_set_arr(gen_ADT_Node* obj, char const* name, gen_AllocatorInfo backing)
{
	return gen_adt_make_branch(obj, backing, name, 1);
}

gen_b8 gen_adt_set_str(gen_ADT_Node* obj, char const* name, char const* value)
{
	gen_adt_make_leaf(obj, name, EADT_TYPE_STRING);
	obj->string = value;
	return true;
}

gen_b8 gen_adt_set_flt(gen_ADT_Node* obj, char const* name, gen_f64 value)
{
	gen_adt_make_leaf(obj, name, EADT_TYPE_REAL);
	obj->real = value;
	return true;
}

gen_b8 gen_adt_set_int(gen_ADT_Node* obj, char const* name, gen_s64 value)
{
	gen_adt_make_leaf(obj, name, EADT_TYPE_INTEGER);
	obj->integer = value;
	return true;
}

gen_ADT_Node* gen_adt_move_node_at(gen_ADT_Node* node, gen_ADT_Node* new_parent, gen_ssize index)
{
	GEN_ASSERT_NOT_NULL(node);
	GEN_ASSERT_NOT_NULL(new_parent);
	gen_ADT_Node* old_parent = node->parent;
	gen_ADT_Node* new_node   = gen_adt_alloc_at(new_parent, index);
	*new_node                = *node;
	new_node->parent         = new_parent;
	if (old_parent)
	{
		gen_adt_remove_node(node);
	}
	return new_node;
}

gen_ADT_Node* gen_adt_move_node(gen_ADT_Node* node, gen_ADT_Node* new_parent)
{
	GEN_ASSERT_NOT_NULL(node);
	GEN_ASSERT_NOT_NULL(new_parent);
	GEN_ASSERT(new_parent->type == EADT_TYPE_ARRAY || new_parent->type == EADT_TYPE_OBJECT);
	return gen_adt_move_node_at(node, new_parent, gen_array_num(new_parent->nodes));
}

void gen_adt_swap_nodes(gen_ADT_Node* node, gen_ADT_Node* other_node)
{
	GEN_ASSERT_NOT_NULL(node);
	GEN_ASSERT_NOT_NULL(other_node);
	gen_ADT_Node* parent               = node->parent;
	gen_ADT_Node* other_parent         = other_node->parent;
	gen_ssize     index                = (gen_pointer_diff(parent->nodes, node) / gen_size_of(gen_ADT_Node));
	gen_ssize     index2               = (gen_pointer_diff(other_parent->nodes, other_node) / gen_size_of(gen_ADT_Node));
	gen_ADT_Node  temp                 = parent->nodes[index];
	temp.parent                        = other_parent;
	other_parent->nodes[index2].parent = parent;
	parent->nodes[index]               = other_parent->nodes[index2];
	other_parent->nodes[index2]        = temp;
}

void gen_adt_remove_node(gen_ADT_Node* node)
{
	GEN_ASSERT_NOT_NULL(node);
	GEN_ASSERT_NOT_NULL(node->parent);
	gen_ADT_Node* parent = node->parent;
	gen_ssize     index  = (gen_pointer_diff(parent->nodes, node) / gen_size_of(gen_ADT_Node));
	gen_array_remove_at(parent->nodes, index);
}

gen_ADT_Node* gen_adt_append_obj(gen_ADT_Node* parent, char const* name)
{
	gen_ADT_Node* o = gen_adt_alloc(parent);
	if (! o)
		return NULL;
	if (gen_adt_set_obj(o, name, gen_array_get_header(parent->nodes)->Allocator))
	{
		gen_adt_remove_node(o);
		return NULL;
	}
	return o;
}

gen_ADT_Node* gen_adt_append_arr(gen_ADT_Node* parent, char const* name)
{
	gen_ADT_Node* o = gen_adt_alloc(parent);
	if (! o)
		return NULL;

	gen_ArrayHeader* node_header = gen_array_get_header(parent->nodes);
	if (gen_adt_set_arr(o, name, node_header->Allocator))
	{
		gen_adt_remove_node(o);
		return NULL;
	}
	return o;
}

gen_ADT_Node* gen_adt_append_str(gen_ADT_Node* parent, char const* name, char const* value)
{
	gen_ADT_Node* o = gen_adt_alloc(parent);
	if (! o)
		return NULL;
	gen_adt_set_str(o, name, value);
	return o;
}

gen_ADT_Node* gen_adt_append_flt(gen_ADT_Node* parent, char const* name, gen_f64 value)
{
	gen_ADT_Node* o = gen_adt_alloc(parent);
	if (! o)
		return NULL;
	gen_adt_set_flt(o, name, value);
	return o;
}

gen_ADT_Node* gen_adt_append_int(gen_ADT_Node* parent, char const* name, gen_s64 value)
{
	gen_ADT_Node* o = gen_adt_alloc(parent);
	if (! o)
		return NULL;
	gen_adt_set_int(o, name, value);
	return o;
}

/* parser helpers */
char* gen_adt_parse_number_strict(gen_ADT_Node* node, char* base_str)
{
	GEN_ASSERT_NOT_NULL(node);
	GEN_ASSERT_NOT_NULL(base_str);
	char *p = base_str, *e = p;

	while (*e)
		++e;

	while (*p && (gen_char_first_occurence("eE.+-", *p) || gen_char_is_hex_digit(*p)))
	{
		++p;
	}

	if (p >= e)
	{
		return gen_adt_parse_number(node, base_str);
	}

	return base_str;
}

char* gen_adt_parse_number(gen_ADT_Node* node, char* base_str)
{
	GEN_ASSERT_NOT_NULL(node);
	GEN_ASSERT_NOT_NULL(base_str);
	char *p = base_str, *e = p;

	gen_s32      base         = 0;
	gen_s32      base2        = 0;
	gen_u8       base2_offset = 0;
	gen_s8       exp = 0, orig_exp = 0;
	gen_u8       neg_zero   = 0;
	gen_u8       lead_digit = 0;
	gen_ADT_Type node_type  = EADT_TYPE_UNINITIALISED;
	gen_u8       node_props = 0;

	/* skip false positives and special cases */
	if (! ! gen_char_first_occurence("eE", *p) || (! ! gen_char_first_occurence(".+-", *p) && ! gen_char_is_hex_digit(*(p + 1)) && *(p + 1) != '.'))
	{
		return ++base_str;
	}

	node_type         = EADT_TYPE_INTEGER;
	neg_zero          = false;

	gen_ssize ib      = 0;
	char      buf[48] = { 0 };

	if (*e == '+')
		++e;
	else if (*e == '-')
	{
		buf[ib++] = *e++;
	}

	if (*e == '.')
	{
		node_type  = EADT_TYPE_REAL;
		node_props = EADT_PROPS_IS_PARSED_REAL;
		lead_digit = false;
		buf[ib++]  = '0';
		do
		{
			buf[ib++] = *e;
		} while (gen_char_is_digit(*++e));
	}
	else
	{
		if (! gen_c_str_compare_len(e, "0x", 2) || ! gen_c_str_compare_len(e, "0X", 2))
		{
			node_props = EADT_PROPS_IS_HEX;
		}

		/* bail if ZPL_ADT_PROPS_IS_HEX is unset but we get 'x' on input */
		if (gen_char_to_lower(*e) == 'x' && (node_props != EADT_PROPS_IS_HEX))
		{
			return ++base_str;
		}

		while (gen_char_is_hex_digit(*e) || gen_char_to_lower(*e) == 'x')
		{
			buf[ib++] = *e++;
		}

		if (*e == '.')
		{
			node_type    = EADT_TYPE_REAL;
			lead_digit   = true;
			gen_u32 step = 0;

			do
			{
				buf[ib++] = *e;
				++step;
			} while (gen_char_is_digit(*++e));

			if (step < 2)
			{
				buf[ib++] = '0';
			}
		}
	}

	/* check if we have a dot here, this is a false positive (IP address, ...) */
	if (*e == '.')
	{
		return ++base_str;
	}

	gen_f32   eb        = 10;
	char      expbuf[6] = { 0 };
	gen_ssize expi      = 0;

	if (*e && ! ! gen_char_first_occurence("eE", *e))
	{
		++e;
		if (*e == '+' || *e == '-' || gen_char_is_digit(*e))
		{
			if (*e == '-')
			{
				eb = 0.1f;
			}
			if (! gen_char_is_digit(*e))
			{
				++e;
			}
			while (gen_char_is_digit(*e))
			{
				expbuf[expi++] = *e++;
			}
		}

		orig_exp = exp = (gen_u8)gen_c_str_to_i64(expbuf, NULL, 10);
	}

	if (node_type == EADT_TYPE_INTEGER)
	{
		node->integer = gen_c_str_to_i64(buf, 0, 0);
#ifndef GEN_PARSER_DISABLE_ANALYSIS
		/* special case: negative zero */
		if (node->integer == 0 && buf[0] == '-')
		{
			neg_zero = true;
		}
#endif
		while (orig_exp-- > 0)
		{
			node->integer *= (gen_s64)eb;
		}
	}
	else
	{
		node->real = gen_c_str_to_f64(buf, 0);

#ifndef GEN_PARSER_DISABLE_ANALYSIS
		char *q = buf, *base_string = q, *base_string2 = q;
		base_string               = gen_ccast(char*, gen_c_str_skip(base_string, '.'));
		*base_string              = '\0';
		base_string2              = base_string + 1;
		char* base_strbuilder_off = base_string2;
		while (*base_strbuilder_off++ == '0')
			base2_offset++;

		base  = (gen_s32)gen_c_str_to_i64(q, 0, 0);
		base2 = (gen_s32)gen_c_str_to_i64(base_string2, 0, 0);
		if (exp)
		{
			exp        = exp * (! (eb == 10.0f) ? -1 : 1);
			node_props = EADT_PROPS_IS_EXP;
		}

		/* special case: negative zero */
		if (base == 0 && buf[0] == '-')
		{
			neg_zero = true;
		}
#endif
		while (orig_exp-- > 0)
		{
			node->real *= eb;
		}
	}

	node->type  = node_type;
	node->props = node_props;

#ifndef GEN_PARSER_DISABLE_ANALYSIS
	node->base         = base;
	node->base2        = base2;
	node->base2_offset = base2_offset;
	node->exp          = exp;
	node->neg_zero     = neg_zero;
	node->lead_digit   = lead_digit;
#else
	unused(base);
	unused(base2);
	unused(base2_offset);
	unused(exp);
	unused(neg_zero);
	unused(lead_digit);
#endif
	return e;
}

gen_ADT_Error gen_adt_print_number(gen_FileInfo* file, gen_ADT_Node* node)
{
	GEN_ASSERT_NOT_NULL(file);
	GEN_ASSERT_NOT_NULL(node);
	if (node->type != EADT_TYPE_INTEGER && node->type != EADT_TYPE_REAL)
	{
		return EADT_ERROR_INVALID_TYPE;
	}

#ifndef GEN_PARSER_DISABLE_ANALYSIS
	if (node->neg_zero)
	{
		gen__adt_fprintf(file, "-");
	}
#endif

	switch (node->type)
	{
		case EADT_TYPE_INTEGER:
		{
			if (node->props == EADT_PROPS_IS_HEX)
			{
				gen__adt_fprintf(file, "0x%llx", (long long)node->integer);
			}
			else
			{
				gen__adt_fprintf(file, "%lld", (long long)node->integer);
			}
		}
		break;

		case EADT_TYPE_REAL:
		{
			if (node->props == EADT_PROPS_NAN)
			{
				gen__adt_fprintf(file, "NaN");
			}
			else if (node->props == EADT_PROPS_NAN_NEG)
			{
				gen__adt_fprintf(file, "-NaN");
			}
			else if (node->props == EADT_PROPS_INFINITY)
			{
				gen__adt_fprintf(file, "Infinity");
			}
			else if (node->props == EADT_PROPS_INFINITY_NEG)
			{
				gen__adt_fprintf(file, "-Infinity");
			}
			else if (node->props == EADT_PROPS_TRUE)
			{
				gen__adt_fprintf(file, "true");
			}
			else if (node->props == EADT_PROPS_FALSE)
			{
				gen__adt_fprintf(file, "false");
			}
			else if (node->props == EADT_PROPS_NULL)
			{
				gen__adt_fprintf(file, "null");
#ifndef GEN_PARSER_DISABLE_ANALYSIS
			}
			else if (node->props == EADT_PROPS_IS_EXP)
			{
				gen__adt_fprintf(file, "%lld.%0*d%llde%lld", (long long)node->base, node->base2_offset, 0, (long long)node->base2, (long long)node->exp);
			}
			else if (node->props == EADT_PROPS_IS_PARSED_REAL)
			{
				if (! node->lead_digit)
					gen__adt_fprintf(file, ".%0*d%lld", node->base2_offset, 0, (long long)node->base2);
				else
					gen__adt_fprintf(file, "%lld.%0*d%lld", (long long int)node->base2_offset, 0, (int)node->base, (long long)node->base2);
#endif
			}
			else
			{
				gen__adt_fprintf(file, "%f", node->real);
			}
		}
		break;
	}

	return EADT_ERROR_NONE;
}

gen_ADT_Error gen_adt_print_string(gen_FileInfo* file, gen_ADT_Node* node, char const* escaped_chars, char const* escape_symbol)
{
	GEN_ASSERT_NOT_NULL(file);
	GEN_ASSERT_NOT_NULL(node);
	GEN_ASSERT_NOT_NULL(escaped_chars);
	if (node->type != EADT_TYPE_STRING && node->type != EADT_TYPE_MULTISTRING)
	{
		return EADT_ERROR_INVALID_TYPE;
	}

	/* escape string */
	char const *p = node->string, *b = p;

	if (! p)
		return EADT_ERROR_NONE;

	do
	{
		p = gen_c_str_skip_any(p, escaped_chars);
		gen__adt_fprintf(file, "%.*s", gen_pointer_diff(b, p), b);
		if (*p && ! ! gen_char_first_occurence(escaped_chars, *p))
		{
			gen__adt_fprintf(file, "%s%c", escape_symbol, *p);
			p++;
		}
		b = p;
	} while (*p);

	return EADT_ERROR_NONE;
}

gen_ADT_Error gen_adt_c_str_to_number(gen_ADT_Node* node)
{
	GEN_ASSERT(node);

	if (node->type == EADT_TYPE_REAL || node->type == EADT_TYPE_INTEGER)
		return EADT_ERROR_ALREADY_CONVERTED; /* this is already converted/parsed */
	if (node->type != EADT_TYPE_STRING && node->type != EADT_TYPE_MULTISTRING)
	{
		return EADT_ERROR_INVALID_TYPE;
	}

	gen_adt_parse_number(node, (char*)node->string);

	return EADT_ERROR_NONE;
}

gen_ADT_Error gen_adt_c_str_to_number_strict(gen_ADT_Node* node)
{
	GEN_ASSERT(node);

	if (node->type == EADT_TYPE_REAL || node->type == EADT_TYPE_INTEGER)
		return EADT_ERROR_ALREADY_CONVERTED; /* this is already converted/parsed */
	if (node->type != EADT_TYPE_STRING && node->type != EADT_TYPE_MULTISTRING)
	{
		return EADT_ERROR_INVALID_TYPE;
	}

	gen_adt_parse_number_strict(node, (char*)node->string);

	return EADT_ERROR_NONE;
}

#undef gen__adt_fprintf

#pragma endregion ADT

#pragma region CSV

#ifdef GEN_CSV_DEBUG
#define GEN_CSV_ASSERT(msg) GEN_PANIC(msg)
#else
#define GEN_CSV_ASSERT(msg)
#endif

gen_u8 gen_csv_parse_delimiter(gen_CSV_Object* root, char* text, gen_AllocatorInfo allocator, gen_b32 has_header, char delim)
{
	gen_CSV_Error error = ECSV_Error__NONE;
	GEN_ASSERT_NOT_NULL(root);
	GEN_ASSERT_NOT_NULL(text);
	gen_zero_item(root);

	gen_adt_make_branch(root, allocator, NULL, has_header ? false : true);

	char* currentChar = text;
	char* beginChar;
	char* endChar;

	gen_ssize columnIndex      = 0;
	gen_ssize totalColumnIndex = 0;

	do
	{
		char delimiter = 0;
		currentChar    = gen_ccast(char*, gen_c_str_trim(currentChar, false));

		if (*currentChar == 0)
			break;

		gen_ADT_Node rowItem = { 0 };
		rowItem.type         = EADT_TYPE_STRING;

#ifndef GEN_PARSER_DISABLE_ANALYSIS
		rowItem.name_style = EADT_NAME_STYLE_NO_QUOTES;
#endif

		/* handle string literals */
		if (*currentChar == '"')
		{
			currentChar    += 1;
			beginChar       = currentChar;
			endChar         = currentChar;
			rowItem.string  = beginChar;
#ifndef GEN_PARSER_DISABLE_ANALYSIS
			rowItem.name_style = EADT_NAME_STYLE_DOUBLE_QUOTE;
#endif
			do
			{
				endChar = gen_ccast(char*, gen_c_str_skip(endChar, '"'));

				if (*endChar && *(endChar + 1) == '"')
				{
					endChar += 2;
				}
				else
					break;
			} while (*endChar);

			if (*endChar == 0)
			{
				GEN_CSV_ASSERT("unmatched quoted string");
				error = ECSV_Error__UNEXPECTED_END_OF_INPUT;
				return error;
			}

			*endChar    = 0;
			currentChar = gen_ccast(char*, gen_c_str_trim(endChar + 1, true));
			delimiter   = *currentChar;

			/* unescape escaped quotes (so that unescaped text escapes :) */
			{
				char* escapedChar = beginChar;
				do
				{
					if (*escapedChar == '"' && *(escapedChar + 1) == '"')
					{
						gen_mem_move(escapedChar, escapedChar + 1, gen_c_str_len(escapedChar));
					}
					escapedChar++;
				} while (*escapedChar);
			}
		}
		else if (*currentChar == delim)
		{
			delimiter      = *currentChar;
			rowItem.string = "";
		}
		else if (*currentChar)
		{
			/* regular data */
			beginChar      = currentChar;
			endChar        = currentChar;
			rowItem.string = beginChar;

			do
			{
				endChar++;
			} while (*endChar && *endChar != delim && *endChar != '\n');

			if (*endChar)
			{
				currentChar = gen_ccast(char*, gen_c_str_trim(endChar, true));

				while (gen_char_is_space(*(endChar - 1)))
				{
					endChar--;
				}

				delimiter = *currentChar;
				*endChar  = 0;
			}
			else
			{
				delimiter   = 0;
				currentChar = endChar;
			}

			/* check if number and process if so */
			gen_b32 skip_number = false;
			char*   num_p       = beginChar;

			// We only consider hexadecimal values if they start with 0x
			if (gen_c_str_len(num_p) > 2 && num_p[0] == '0' && (num_p[1] == 'x' || num_p[1] == 'X'))
			{
				num_p += 2;    // skip '0x' prefix
				do
				{
					if (! gen_char_is_hex_digit(*num_p))
					{
						skip_number = true;
						break;
					}
				} while (*num_p++);
			}
			else
			{
				skip_number = true;
			}

			if (! skip_number)
			{
				gen_adt_c_str_to_number(&rowItem);
			}
		}

		if (columnIndex >= gen_scast(gen_ssize, gen_array_num(root->nodes)))
		{
			gen_adt_append_arr(root, NULL);
		}

		gen_array_append(root->nodes[columnIndex].nodes, rowItem);

		if (delimiter == delim)
		{
			columnIndex++;
			currentChar++;
		}
		else if (delimiter == '\n' || delimiter == 0)
		{
			/* check if number of rows is not mismatched */
			if (totalColumnIndex < columnIndex)
				totalColumnIndex = columnIndex;

			else if (totalColumnIndex != columnIndex)
			{
				GEN_CSV_ASSERT("mismatched rows");
				error = ECSV_Error__MISMATCHED_ROWS;
				return error;
			}

			columnIndex = 0;

			if (delimiter != 0)
				currentChar++;
		}
	} while (*currentChar);

	if (gen_array_num(root->nodes) == 0)
	{
		GEN_CSV_ASSERT("unexpected end of input. stream is empty.");
		error = ECSV_Error__UNEXPECTED_END_OF_INPUT;
		return error;
	}

	/* consider first row as a header. */
	if (has_header)
	{
		for (gen_ssize i = 0; i < gen_scast(gen_ssize, gen_array_num(root->nodes)); i++)
		{
			gen_CSV_Object* col = root->nodes + i;
			gen_CSV_Object* hdr = col->nodes;
			col->name           = hdr->string;
			gen_array_remove_at(col->nodes, 0);
		}
	}

	return error;
}

void gen_csv_free(gen_CSV_Object* obj)
{
	gen_adt_destroy_branch(obj);
}

void gen__csv_write_record(gen_FileInfo* file, gen_CSV_Object* node)
{
	switch (node->type)
	{
		case EADT_TYPE_STRING:
		{
#ifndef GEN_PARSER_DISABLE_ANALYSIS
			switch (node->name_style)
			{
				case EADT_NAME_STYLE_DOUBLE_QUOTE:
				{
					gen_c_str_fmt_file(file, "\"");
					gen_adt_print_string(file, node, "\"", "\"");
					gen_c_str_fmt_file(file, "\"");
				}
				break;

				case EADT_NAME_STYLE_NO_QUOTES:
				{
#endif
					gen_c_str_fmt_file(file, "%s", node->string);
#ifndef GEN_PARSER_DISABLE_ANALYSIS
				}
				break;
			}
#endif
		}
		break;

		case EADT_TYPE_REAL:
		case EADT_TYPE_INTEGER:
		{
			gen_adt_print_number(file, node);
		}
		break;
	}
}

void gen__csv_write_header(gen_FileInfo* file, gen_CSV_Object* header)
{
	gen_CSV_Object temp = *header;
	temp.string         = temp.name;
	temp.type           = EADT_TYPE_STRING;
	gen__csv_write_record(file, &temp);
}

void gen_csv_write_delimiter(gen_FileInfo* file, gen_CSV_Object* obj, char delimiter)
{
	GEN_ASSERT_NOT_NULL(file);
	GEN_ASSERT_NOT_NULL(obj);
	GEN_ASSERT(obj->nodes);
	gen_ssize cols = gen_array_num(obj->nodes);
	if (cols == 0)
		return;

	gen_ssize rows = gen_array_num(obj->nodes[0].nodes);
	if (rows == 0)
		return;

	gen_b32 has_headers = obj->nodes[0].name != NULL;

	if (has_headers)
	{
		for (gen_ssize i = 0; i < cols; i++)
		{
			gen__csv_write_header(file, &obj->nodes[i]);
			if (i + 1 != cols)
			{
				gen_c_str_fmt_file(file, "%c", delimiter);
			}
		}
		gen_c_str_fmt_file(file, "\n");
	}

	for (gen_ssize r = 0; r < rows; r++)
	{
		for (gen_ssize i = 0; i < cols; i++)
		{
			gen__csv_write_record(file, &obj->nodes[i].nodes[r]);
			if (i + 1 != cols)
			{
				gen_c_str_fmt_file(file, "%c", delimiter);
			}
		}
		gen_c_str_fmt_file(file, "\n");
	}
}

gen_StrBuilder gen_csv_write_strbuilder_delimiter(gen_AllocatorInfo a, gen_CSV_Object* obj, char delimiter)
{
	gen_FileInfo tmp;
	gen_file_stream_new(&tmp, a);
	gen_csv_write_delimiter(&tmp, obj, delimiter);

	gen_ssize      fsize;
	gen_u8*        buf    = gen_file_stream_buf(&tmp, &fsize);
	gen_StrBuilder output = gen_strbuilder_make_length(a, (char*)buf, fsize);
	gen_file_close(&tmp);
	return output;
}

#undef gen__adt_fprintf

#pragma endregion CSV

GEN_API_C_END
GEN_NS_END

// GEN_ROLL_OWN_DEPENDENCIES
#endif

GEN_NS_BEGIN
GEN_API_C_BEGIN

#pragma region StaticData
GEN_API gen_global gen_Context* gen__ctx;
GEN_API gen_global gen_u32      context_counter;

#pragma region Constants
GEN_API gen_global gen_Macro gen_enum_underlying_macro;

GEN_API gen_global gen_Code gen_Code_Global;
GEN_API gen_global gen_Code gen_Code_Invalid;

GEN_API gen_global gen_Code gen_access_public;
GEN_API gen_global gen_Code gen_access_protected;
GEN_API gen_global gen_Code gen_access_private;

GEN_API gen_global gen_CodeAttributes gen_attrib_api_export;
GEN_API gen_global gen_CodeAttributes gen_attrib_api_import;

GEN_API gen_global gen_Code gen_module_global_fragment;
GEN_API gen_global gen_Code gen_module_private_fragment;

GEN_API gen_global gen_Code gen_fmt_newline;

GEN_API gen_global gen_CodeParams gen_param_varadic;

GEN_API gen_global gen_CodePragma gen_pragma_once;

GEN_API gen_global gen_CodePreprocessCond gen_preprocess_else;
GEN_API gen_global gen_CodePreprocessCond gen_preprocess_endif;

GEN_API gen_global gen_CodeSpecifiers gen_spec_const;
GEN_API gen_global gen_CodeSpecifiers gen_spec_consteval;
GEN_API gen_global gen_CodeSpecifiers gen_spec_constexpr;
GEN_API gen_global gen_CodeSpecifiers gen_spec_constinit;
GEN_API gen_global gen_CodeSpecifiers gen_spec_extern_linkage;
GEN_API gen_global gen_CodeSpecifiers gen_spec_final;
GEN_API gen_global gen_CodeSpecifiers gen_spec_forceinline;
GEN_API gen_global gen_CodeSpecifiers gen_spec_global;
GEN_API gen_global gen_CodeSpecifiers gen_spec_inline;
GEN_API gen_global gen_CodeSpecifiers gen_spec_internal_linkage;
GEN_API gen_global gen_CodeSpecifiers gen_spec_local_persist;
GEN_API gen_global gen_CodeSpecifiers gen_spec_mutable;
GEN_API gen_global gen_CodeSpecifiers gen_spec_noexcept;
GEN_API gen_global gen_CodeSpecifiers gen_spec_neverinline;
GEN_API gen_global gen_CodeSpecifiers gen_spec_override;
GEN_API gen_global gen_CodeSpecifiers gen_spec_ptr;
GEN_API gen_global gen_CodeSpecifiers gen_spec_pure;
GEN_API gen_global gen_CodeSpecifiers gen_spec_ref;
GEN_API gen_global gen_CodeSpecifiers gen_spec_register;
GEN_API gen_global gen_CodeSpecifiers gen_spec_rvalue;
GEN_API gen_global gen_CodeSpecifiers gen_spec_static_member;
GEN_API gen_global gen_CodeSpecifiers gen_spec_thread_local;
GEN_API gen_global gen_CodeSpecifiers gen_spec_virtual;
GEN_API gen_global gen_CodeSpecifiers gen_spec_volatile;

GEN_API gen_global gen_CodeTypename gen_t_empty;
GEN_API gen_global gen_CodeTypename gen_t_auto;
GEN_API gen_global gen_CodeTypename gen_t_void;
GEN_API gen_global gen_CodeTypename gen_t_int;
GEN_API gen_global gen_CodeTypename gen_t_bool;
GEN_API gen_global gen_CodeTypename gen_t_char;
GEN_API gen_global gen_CodeTypename gen_t_wchar_t;
GEN_API gen_global gen_CodeTypename gen_t_class;
GEN_API gen_global gen_CodeTypename gen_t_typename;

#ifdef GEN_DEFINE_LIBRARY_CODE_CONSTANTS
GEN_API gen_global gen_CodeTypename gen_t_b32;

GEN_API gen_global gen_CodeTypename gen_t_s8;
GEN_API gen_global gen_CodeTypename gen_t_s16;
GEN_API gen_global gen_CodeTypename gen_t_s32;
GEN_API gen_global gen_CodeTypename gen_t_s64;

GEN_API gen_global gen_CodeTypename gen_t_u8;
GEN_API gen_global gen_CodeTypename gen_t_u16;
GEN_API gen_global gen_CodeTypename gen_t_u32;
GEN_API gen_global gen_CodeTypename gen_t_u64;

GEN_API gen_global gen_CodeTypename gen_t_ssize;
GEN_API gen_global gen_CodeTypename gen_t_usize;

GEN_API gen_global gen_CodeTypename gen_t_f32;
GEN_API gen_global gen_CodeTypename gen_t_f64;
#endif

#pragma endregion Constants

#pragma endregion StaticData

#pragma region AST

// These macros are used in the swtich cases within ast.cpp, inteface.upfront.cpp, parser.cpp

#	define GEN_AST_BODY_CLASS_UNALLOWED_TYPES_CASES    \
	case CT_PlatformAttributes:                  \
	case CT_Class_Body:                          \
	case CT_Enum_Body:                           \
	case CT_Extern_Linkage:                      \
	case CT_Function_Body:                       \
	case CT_Function_Fwd:                        \
	case CT_Global_Body:                         \
	case CT_Namespace:                           \
	case CT_Namespace_Body:                      \
	case CT_Operator:                            \
	case CT_Operator_Fwd:                        \
	case CT_Parameters:                          \
	case CT_Specifiers:                          \
	case CT_Struct_Body:                         \
	case CT_Typename
#	define GEN_AST_BODY_STRUCT_UNALLOWED_TYPES_CASES GEN_AST_BODY_CLASS_UNALLOWED_TYPES_CASES

#	define GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES_CASES \
	case CT_Access_Public:                          \
	case CT_Access_Protected:                       \
	case CT_Access_Private:                         \
	case CT_PlatformAttributes:                     \
	case CT_Class_Body:                             \
	case CT_Enum_Body:                              \
	case CT_Extern_Linkage:                         \
	case CT_Friend:                                 \
	case CT_Function_Body:                          \
	case CT_Function_Fwd:                           \
	case CT_Global_Body:                            \
	case CT_Namespace:                              \
	case CT_Namespace_Body:                         \
	case CT_Operator:                               \
	case CT_Operator_Fwd:                           \
	case CT_Operator_Member:                        \
	case CT_Operator_Member_Fwd:                    \
	case CT_Parameters:                             \
	case CT_Specifiers:                             \
	case CT_Struct_Body:                            \
	case CT_Typename

#	define GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES_CASES     \
	case CT_Access_Public:                         \
	case CT_Access_Protected:                      \
	case CT_Access_Private:                        \
	case CT_PlatformAttributes:                    \
	case CT_Class_Body:                            \
	case CT_Enum_Body:                             \
	case CT_Execution:                             \
	case CT_Friend:                                \
	case CT_Function_Body:                         \
	case CT_Namespace_Body:                        \
	case CT_Operator_Member:                       \
	case CT_Operator_Member_Fwd:                   \
	case CT_Parameters:                            \
	case CT_Specifiers:                            \
	case CT_Struct_Body:                           \
	case CT_Typename
#	define GEN_AST_BODY_EXPORT_UNALLOWED_TYPES_CASES         GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES_CASES
#	define GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES_CASES GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES_CASES

#	define GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES_CASES \
	case CT_Access_Public:                        \
	case CT_Access_Protected:                     \
	case CT_Access_Private:                       \
	case CT_PlatformAttributes:                   \
	case CT_Class_Body:                           \
	case CT_Enum_Body:                            \
	case CT_Execution:                            \
	case CT_Friend:                               \
	case CT_Function_Body:                        \
	case CT_Namespace_Body:                       \
	case CT_Operator_Member:                      \
	case CT_Operator_Member_Fwd:                  \
	case CT_Parameters:                           \
	case CT_Specifiers:                           \
	case CT_Struct_Body:                          \
	case CT_Typename

// This serializes all the data-members in a "debug" format, where each member is printed with its associated value.
gen_Str gen_code__debug_str( gen_Code self)
{
GEN_ASSERT(self != gen_nullptr);
	gen_StrBuilder  result_stack = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, gen_kilobytes(1) );
	gen_StrBuilder* result       = & result_stack;

	if ( self->Parent )
		gen_strbuilder_append_fmt( result, "\n\tParent       : %S %S", gen_code_type_str(self->Parent), self->Name.Len ? self->Name : gen_txt("Null") );
	else
		gen_strbuilder_append_fmt( result, "\n\tParent       : %S", gen_txt("Null") );

	gen_strbuilder_append_fmt( result, "\n\tName         : %S", self->Name.Len ? self->Name : gen_txt("Null") );
	gen_strbuilder_append_fmt( result, "\n\tType         : %S", gen_code_type_str(self) );
	gen_strbuilder_append_fmt( result, "\n\tModule Flags : %S", gen_module_flag_to_str( self->ModuleFlags ) );

	switch ( self->Type )
	{
		case CT_Invalid:
		case CT_NewLine:
		case CT_Access_Private:
		case CT_Access_Protected:
		case CT_Access_Public:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
		break;

		case CT_Untyped:
		case CT_Execution:
		case CT_Comment:
		case CT_PlatformAttributes:
		case CT_Preprocess_Include:
		case CT_Preprocess_Pragma:
		case CT_Preprocess_If:
		case CT_Preprocess_ElIf:
		case CT_Preprocess_Else:
		case CT_Preprocess_IfDef:
		case CT_Preprocess_IfNotDef:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tContent: %S", self->Content );
		break;

		case CT_Preprocess_Define:
			// TODO(ED): Needs implementaton
			gen_log_failure("gen_code_debug_str: NOT IMPLEMENTED for CT_Preprocess_Define");
		break;

		case CT_Class:
		case CT_Struct:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt   : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes  : %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParentAccess: %S", self->ParentType ? gen_access_spec_to_str( self->ParentAccess )                  : gen_txt("No Parent") );
			gen_strbuilder_append_fmt( result, "\n\tParentType  : %S", self->ParentType ? gen_code_type_str(self->ParentType)                           : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody        : %S", self->Body       ? gen_code_debug_str(self->Body)                                : gen_txt("Null") );
		break;

		case CT_Class_Fwd:
		case CT_Struct_Fwd:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt   : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes  : %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParentAccess: %S", self->ParentType ? gen_access_spec_to_str( self->ParentAccess )                  : gen_txt("No Parent") );
			gen_strbuilder_append_fmt( result, "\n\tParentType  : %S", self->ParentType ? gen_code_type_str(self->ParentType)                           : gen_txt("Null") );
		break;

		case CT_Constructor:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt      : %S", self->InlineCmt       ? self->InlineCmt->Content                                       : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs           ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs) )           : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tInitializerList: %S", self->InitializerList ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->InitializerList) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParams         : %S", self->Params          ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params) )          : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody           : %S", self->Body            ? gen_code_debug_str(self->Body)                                     : gen_txt("Null") );
		break;

		case CT_Constructor_Fwd:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt      : %S", self->InlineCmt       ? self->InlineCmt->Content                                       : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs           ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs) )           : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tInitializerList: %S", self->InitializerList ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->InitializerList) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParams         : %S", self->Params          ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params) )          : gen_txt("Null") );
		break;

		case CT_Destructor:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt      : %S", self->InlineCmt       ? self->InlineCmt->Content                             : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs           ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody           : %S", self->Body            ? gen_code_debug_str(self->Body)                           : gen_txt("Null") );
		break;

		case CT_Destructor_Fwd:
		break;

		case CT_Enum:
		case CT_Enum_Class:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt       : %S", self->InlineCmt      ? self->InlineCmt->Content                                     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes      : %S", self->Attributes     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) )    : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tUnderlying Type : %S", self->UnderlyingType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->UnderlyingType)) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody            : %S", self->Body           ? gen_code_debug_str(self->Body)                                   : gen_txt("Null") );
		break;

		case CT_Enum_Fwd:
		case CT_Enum_Class_Fwd:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt       : %S", self->InlineCmt      ? self->InlineCmt->Content                                     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes      : %S", self->Attributes     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) )    : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tUnderlying Type : %S", self->UnderlyingType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->UnderlyingType)) : gen_txt("Null") );
		break;

		case CT_Extern_Linkage:
		case CT_Namespace:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tBody: %S", self->Body ? gen_code_debug_str(self->Body) : gen_txt("Null") );
		break;

		case CT_Friend:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt  : %S", self->InlineCmt   ? self->InlineCmt->Content                                  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tDeclaration: %S", self->Declaration ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Declaration)) : gen_txt("Null") );
		break;

		case CT_Function:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))       : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ReturnType))  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params))      : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? gen_code_debug_str(self->Body)                                : gen_txt("Null") );
		break;

		case CT_Function_Fwd:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))       : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ReturnType))  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params))      : gen_txt("Null") );
		break;

		case CT_Module:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
		break;

		case CT_Operator:
		case CT_Operator_Member:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))       : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ReturnType))  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params))      : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? gen_code_debug_str(self->Body)                                : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tOp        : %S", gen_operator_to_str( self->Op ) );
		break;

		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs) )      : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tReturnType: %S", self->ReturnType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ReturnType) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParams    : %S", self->Params     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params) )     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tOp        : %S", gen_operator_to_str( self->Op ) );
		break;

		case CT_Operator_Cast:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tValueType : %S", self->ValueType  ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ValueType)) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? gen_code_debug_str(self->Body)                              : gen_txt("Null") );
		break;

		case CT_Operator_Cast_Fwd:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt : %S", self->InlineCmt  ? self->InlineCmt->Content                                : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs     : %S", self->Specs      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tValueType : %S", self->ValueType  ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ValueType)) : gen_txt("Null") );
		break;

		case CT_Parameters:
			gen_strbuilder_append_fmt( result, "\n\tNumEntries: %d", self->NumEntries );
			gen_strbuilder_append_fmt( result, "\n\tLast      : %S", self->Last->Name );
			gen_strbuilder_append_fmt( result, "\n\tNext      : %S", self->Next->Name );
			gen_strbuilder_append_fmt( result, "\n\tValueType : %S", self->ValueType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ValueType)) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tValue     : %S", self->Value     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Value))     : gen_txt("Null") );
		break;

		case CT_Parameters_Define:
			// TODO(ED): Needs implementaton
			gen_log_failure("gen_code_debug_str: NOT IMPLEMENTED for CT_Parameters_Define");
		break;

		case CT_Specifiers:
		{
			gen_strbuilder_append_fmt( result, "\n\tNumEntries: %d", self->NumEntries );
			gen_strbuilder_append_str( result, gen_txt("\n\tArrSpecs: ") );

			gen_s32 idx  = 0;
			gen_s32 left = self->NumEntries;
			while ( left-- )
			{
				gen_Str spec = gen_spec_to_str( self->ArrSpecs[idx] );
				gen_strbuilder_append_fmt( result, "%.*s, ", spec.Len, spec.Ptr );
				idx++;
			}
			gen_strbuilder_append_fmt( result, "\n\tNextSpecs: %S", self->NextSpecs ? gen_code_debug_str(self->NextSpecs) : gen_txt("Null") );
		}
		break;

		case CT_Template:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tParams     : %S", self->Params      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params))      : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tDeclaration: %S", self->Declaration ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Declaration)) : gen_txt("Null") );
		break;

		case CT_Typedef:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt     : %S", self->InlineCmt      ? self->InlineCmt->Content                                     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tUnderlyingType: %S", self->UnderlyingType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->UnderlyingType)) : gen_txt("Null") );
		break;

		case CT_Typename:
			gen_strbuilder_append_fmt( result, "\n\tAttributes     : %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs          : %S", self->Specs      ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))       : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tReturnType     : %S", self->ReturnType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ReturnType))  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tParams         : %S", self->Params     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Params))      : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tArrExpr        : %S", self->ArrExpr    ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ArrExpr))     : gen_txt("Null") );
		break;

		case CT_Union:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tAttributes: %S", self->Attributes ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) ) : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBody      : %S", self->Body       ? gen_code_debug_str(self->Body)                                : gen_txt("Null") );
		break;

		case CT_Using:
			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt     : %S", self->InlineCmt      ? self->InlineCmt->Content                                      : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes    : %S", self->Attributes     ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) )     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tUnderlyingType: %S", self->UnderlyingType ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->UnderlyingType))  : gen_txt("Null") );
		break;

		case CT_Variable:

			if ( self->Parent && self->Parent->Type == CT_Variable )
			{
				// Its a NextVar
				gen_strbuilder_append_fmt( result, "\n\tSpecs       : %S", self->Specs        ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))        : gen_txt("Null") );
				gen_strbuilder_append_fmt( result, "\n\tValue       : %S", self->Value        ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Value))        : gen_txt("Null") );
				gen_strbuilder_append_fmt( result, "\n\tBitfieldSize: %S", self->BitfieldSize ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->BitfieldSize)) : gen_txt("Null") );
				gen_strbuilder_append_fmt( result, "\n\tNextVar     : %S", self->NextVar      ? gen_code_debug_str(self->NextVar)                              : gen_txt("Null") );
				break;
			}

			if ( self->Prev )
				gen_strbuilder_append_fmt( result, "\n\tPrev: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );
			if ( self->Next )
				gen_strbuilder_append_fmt( result, "\n\tNext: %S %S", gen_code_type_str(self->Prev), self->Prev->Name.Len ? self->Prev->Name : gen_txt("Null") );

			gen_strbuilder_append_fmt( result, "\n\tInlineCmt   : %S", self->InlineCmt    ? self->InlineCmt->Content                                    : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tAttributes  : %S", self->Attributes   ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Attributes) )   : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tSpecs       : %S", self->Specs        ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Specs))         : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tValueType   : %S", self->ValueType    ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->ValueType))     : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tBitfieldSize: %S", self->BitfieldSize ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->BitfieldSize))  : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tValue       : %S", self->Value        ? gen_strbuilder_to_str( gen_code_to_strbuilder(self->Value))         : gen_txt("Null") );
			gen_strbuilder_append_fmt( result, "\n\tNextVar     : %S", self->NextVar      ? gen_code_debug_str(self->NextVar)                               : gen_txt("Null") );
		break;
	}

	return gen_strbuilder_to_str( * result );
}

gen_Code gen_code__duplicate( gen_Code self)
{
gen_Code result = gen_make_code();

	void* mem_result = gen_rcast(void*, gen_cast(gen_AST*, result));
	void* mem_self   = gen_rcast(void*, gen_cast(gen_AST*, self));
	gen_mem_copy( mem_result, mem_self, sizeof( gen_AST ) );

	result->Parent = gen_NullCode;
	return result;
}

gen_StrBuilder gen_code__to_strbuilder( gen_Code self)
{
gen_StrBuilder result = gen_strbuilder_make_str( gen__ctx->Allocator_Temp, gen_txt("") );
	gen_code_to_strbuilder_ref( self, & result );
	return result;
}

void gen_code__to_strbuilder_ref( gen_Code self,  gen_StrBuilder *  result)
{
GEN_ASSERT(self != gen_nullptr);
	gen_local_persist gen_thread_local
	char SerializationLevel = 0;

	switch ( self->Type )
	{
		case CT_Invalid:
		#ifdef GEN_DONT_ALLOW_INVALID_CODE
			gen_log_failure("Attempted to serialize invalid code! - %S", Parent ? Parent->gen_code_debug_str() : Name );
		#else
			gen_strbuilder_append_fmt( result, "Invalid gen_Code!" );
		#endif
		break;

		case CT_NewLine:
			gen_strbuilder_append_str( result, gen_txt("\n"));
		break;

		case CT_Untyped:
		case CT_Execution:
		case CT_Comment:
		case CT_PlatformAttributes:
			gen_strbuilder_append_str( result, self->Content );
		break;

		case CT_Access_Private:
		case CT_Access_Protected:
		case CT_Access_Public:
			gen_strbuilder_append_str( result, self->Name );
		break;

		case CT_Class:
			gen_class_to_strbuilder_def(gen_cast(gen_CodeClass, self), result );
		break;

		case CT_Class_Fwd:
			gen_class_to_strbuilder_fwd(gen_cast(gen_CodeClass, self), result );
		break;

		case CT_Constructor:
			gen_constructor__to_strbuilder_def(gen_cast(gen_CodeConstructor, self), result );
		break;

		case CT_Constructor_Fwd:
			gen_constructor__to_strbuilder_fwd(gen_cast(gen_CodeConstructor, self), result );
		break;

		case CT_Destructor:
			gen_destructor__to_strbuilder_def(gen_cast(gen_CodeDestructor, self), result );
		break;

		case CT_Destructor_Fwd:
			gen_destructor__to_strbuilder_fwd(gen_cast(gen_CodeDestructor, self), result );
		break;

		case CT_Enum:
			gen_enum_to_strbuilder_def(gen_cast(gen_CodeEnum, self), result );
		break;

		case CT_Enum_Fwd:
			gen_enum_to_strbuilder_fwd(gen_cast(gen_CodeEnum, self), result );
		break;

		case CT_Enum_Class:
			gen_enum_to_strbuilder_class_def(gen_cast(gen_CodeEnum, self), result );
		break;

		case CT_Enum_Class_Fwd:
			gen_enum_to_strbuilder_class_fwd(gen_cast(gen_CodeEnum, self), result );
		break;

		case CT_Export_Body:
			gen_body_to_strbuilder_export(gen_cast(gen_CodeBody, self), result );
		break;

		case CT_Extern_Linkage:
			gen_extern_to_strbuilder(gen_cast(gen_CodeExtern, self), result );
		break;

		case CT_Friend:
			gen_friend_to_strbuilder_ref(gen_cast(gen_CodeFriend, self), result );
		break;

		case CT_Function:
			gen_fn_to_strbuilder_def(gen_cast(gen_CodeFn, self), result );
		break;

		case CT_Function_Fwd:
			gen_fn_to_strbuilder_fwd(gen_cast(gen_CodeFn, self), result );
		break;

		case CT_Module:
			gen_module_to_strbuilder_ref(gen_cast(gen_CodeModule, self), result );
		break;

		case CT_Namespace:
			namespace_to_strbuilder_ref(gen_cast(gen_CodeNS, self), result );
		break;

		case CT_Operator:
		case CT_Operator_Member:
			gen_code_op_to_strbuilder_def(gen_cast(gen_CodeOperator, self), result );
		break;

		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
			gen_code_op_to_strbuilder_fwd(gen_cast(gen_CodeOperator, self), result );
		break;

		case CT_Operator_Cast:
			gen_opcast_to_strbuilder_def(gen_cast(gen_CodeOpCast, self), result );
		break;

		case CT_Operator_Cast_Fwd:
			gen_opcast_to_strbuilder_fwd(gen_cast(gen_CodeOpCast, self), result );
		break;

		case CT_Parameters:
			gen_params_to_strbuilder_ref(gen_cast(gen_CodeParams, self), result );
		break;

		case CT_Parameters_Define:
			gen_define_params_to_strbuilder_ref(gen_cast(gen_CodeDefineParams, self), result);
		break;

		case CT_Preprocess_Define:
			gen_define_to_strbuilder_ref(gen_cast(gen_CodeDefine, self), result );
		break;

		case CT_Preprocess_If:
			gen_preprocess_to_strbuilder_if(gen_cast(gen_CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_IfDef:
			gen_preprocess_to_strbuilder_ifdef(gen_cast(gen_CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_IfNotDef:
			gen_preprocess_to_strbuilder_ifndef(gen_cast(gen_CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_Include:
			gen_include_to_strbuilder_ref(gen_cast(gen_CodeInclude, self), result );
		break;

		case CT_Preprocess_ElIf:
			gen_preprocess_to_strbuilder_elif(gen_cast(gen_CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_Else:
			gen_preprocess_to_strbuilder_else(gen_cast(gen_CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_EndIf:
			gen_preprocess_to_strbuilder_endif(gen_cast(gen_CodePreprocessCond, self), result );
		break;

		case CT_Preprocess_Pragma:
			gen_pragma_to_strbuilder_ref(gen_cast(gen_CodePragma, self), result );
		break;

		case CT_Specifiers:
			gen_specifiers_to_strbuilder_ref(gen_cast(gen_CodeSpecifiers, self), result );
		break;

		case CT_Struct:
			gen_struct_to_strbuilder_def(gen_cast(gen_CodeStruct, self), result );
		break;

		case CT_Struct_Fwd:
			gen_struct_to_strbuilder_fwd(gen_cast(gen_CodeStruct, self), result );
		break;

		case CT_Template:
			gen_template_to_strbuilder_ref(gen_cast(gen_CodeTemplate, self), result );
		break;

		case CT_Typedef:
			gen_typedef_to_strbuilder_ref(gen_cast(gen_CodeTypedef, self), result );
		break;

		case CT_Typename:
			gen_typename_to_strbuilder_ref(gen_cast(gen_CodeTypename, self), result );
		break;

		case CT_Union:
			union_to_strbuilder_def( gen_cast(gen_CodeUnion, self), result );
		break;

		case CT_Union_Fwd:
			union_to_strbuilder_fwd( gen_cast(gen_CodeUnion, self), result );
		break;

		case CT_Using:
			gen_using_to_strbuilder_ref(gen_cast(gen_CodeUsing, self), result );
		break;

		case CT_Using_Namespace:
			gen_using_to_strbuilder_ns(gen_cast(gen_CodeUsing, self), result );
		break;

		case CT_Variable:
			gen_var_to_strbuilder_ref(gen_cast(gen_CodeVar, self), result );
		break;

		case CT_Enum_Body:
		case CT_Class_Body:
		case CT_Extern_Linkage_Body:
		case CT_Function_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
			gen_body_to_strbuilder_ref( gen_cast(gen_CodeBody, self), result );
		break;
	}
}

bool gen_code__is_equal( gen_Code self,  gen_Code other)
{
/*
	gen_AST values are either some gen_u32 value, a cached string, or a pointer to another gen_AST.

	gen_u32 values are compared by value.
	Cached strings are compared by pointer.
	gen_AST nodes are compared with gen_AST::is_equal.
*/
	if ( other == gen_nullptr )
	{
		gen_log_fmt( "gen_AST::is_equal: other is null\nAST: %S", gen_code_debug_str(self) );
		return false;
	}

	if ( self->Type != other->Type )
	{
		gen_log_fmt("gen_AST::is_equal: Type check failure with other\nAST: %S\nOther: %S"
			, gen_code_debug_str(self)
			, gen_code_debug_str(other)
		);

		return false;
	}

	switch ( self->Type )
	{
	#define check_member_val( val )                           \
	if ( self->val != other->val )                            \
	{                                                         \
		gen_log_fmt("\nAST::is_equal: Member - " #val " failed\n" \
		        "gen_AST  : %S\n"                                 \
		        "Other: %S\n"                                 \
		    , gen_code_debug_str(self)                            \
		    ,gen_code_debug_str(other)                            \
		);                                                    \
                                                              \
		return false;                                         \
	}

	#define check_member_str( str )                                 \
	if ( ! gen_str_are_equal( self->str, other->str ) )                 \
	{                                                               \
		gen_log_fmt("\nAST::is_equal: Member string - "#str " failed\n" \
				"gen_AST  : %S\n"                                       \
				"Other: %S\n"                                       \
			, gen_code_debug_str(self)                                  \
			,gen_code_debug_str(other)                                  \
		);                                                          \
	                                                                \
		return false;                                               \
	}

	#define check_member_content( content )                                  \
	if ( ! gen_str_are_equal( self->content, other->content ))                   \
	{                                                                        \
		gen_log_fmt("\nAST::is_equal: Member content - "#content " failed\n"     \
				"gen_AST  : %S\n"                                                \
				"Other: %S\n"                                                \
			, gen_code_debug_str(self)                                           \
			, gen_code_debug_str(other)                                          \
		);                                                                   \
                                                                             \
		gen_log_fmt("Content cannot be trusted to be unique with this check "    \
			"so it must be verified by eye for now\n"                        \
			"gen_AST   Content:\n%S\n"                                           \
			"Other Content:\n%S\n"                                           \
			, gen_str_visualize_whitespace(self->content, gen__ctx->Allocator_Temp)  \
			, gen_str_visualize_whitespace(other->content, gen__ctx->Allocator_Temp) \
		);                                                                   \
	}

	#define check_member_ast( ast )                                                                \
	if ( self->ast )                                                                               \
	{                                                                                              \
		if ( other->ast == gen_nullptr )                                                               \
		{                                                                                          \
			gen_log_fmt("\nAST::is_equal: Failed for member " #ast " other equivalent param is null\n" \
					"gen_AST  : %S\n"                                                                  \
					"Other: %S\n"                                                                  \
					"For ast member: %S\n"                                                         \
				, gen_code_debug_str(self)                                                             \
				, gen_code_debug_str(other)                                                            \
				, gen_code_debug_str(self->ast)                                                        \
			);                                                                                     \
                                                                                                   \
			return false;                                                                          \
		}                                                                                          \
                                                                                                   \
		if ( ! gen_code_is_equal(self->ast, other->ast ) )                                             \
		{                                                                                          \
			gen_log_fmt( "\nAST::is_equal: Failed for " #ast"\n"                                       \
					"gen_AST  : %S\n"                                                                  \
					"Other: %S\n"                                                                  \
					"For     ast member: %S\n"                                                     \
					"other's ast member: %S\n"                                                     \
				, gen_code_debug_str(self)                                                             \
				, gen_code_debug_str(other)                                                            \
				, gen_code_debug_str(self->ast)                                                        \
				, gen_code_debug_str(other->ast)                                                       \
			);                                                                                     \
		                                                                                           \
			return false;                                                                          \
		}                                                                                          \
	}

		case CT_NewLine:
		case CT_Access_Public:
		case CT_Access_Protected:
		case CT_Access_Private:
		case CT_Preprocess_Else:
		case CT_Preprocess_EndIf:
			return true;


		// Comments are not validated.
		case CT_Comment:
			return true;

		case CT_Execution:
		case CT_PlatformAttributes:
		case CT_Untyped:
		{
			check_member_content( Content );
			return true;
		}

		case CT_Class_Fwd:
		case CT_Struct_Fwd:
		{
			check_member_str( Name );
			check_member_ast( ParentType );
			check_member_val( ParentAccess );
			check_member_ast( Attributes );

			return true;
		}

		case CT_Class:
		case CT_Struct:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ParentType );
			check_member_val( ParentAccess );
			check_member_ast( Attributes );
			check_member_ast( Body );

			return true;
		}

		case CT_Constructor:
		{
			check_member_ast( InitializerList );
			check_member_ast( Params );
			check_member_ast( Body );

			return true;
		}

		case CT_Constructor_Fwd:
		{
			check_member_ast( InitializerList );
			check_member_ast( Params );

			return true;
		}

		case CT_Destructor:
		{
			check_member_ast( Specs );
			check_member_ast( Body );

			return true;
		}

		case CT_Destructor_Fwd:
		{
			check_member_ast( Specs );

			return true;
		}

		case CT_Enum:
		case CT_Enum_Class:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
			check_member_ast( UnderlyingType );
			check_member_ast( Body );
			check_member_ast( UnderlyingTypeMacro );

			return true;
		}

		case CT_Enum_Fwd:
		case CT_Enum_Class_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
			check_member_ast( UnderlyingType );
			check_member_ast( UnderlyingTypeMacro );

			return true;
		}

		case CT_Extern_Linkage:
		{
			check_member_str( Name );
			check_member_ast( Body );

			return true;
		}

		case CT_Friend:
		{
			check_member_str( Name );
			check_member_ast( Declaration );

			return true;
		}

		case CT_Function:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );
			check_member_ast( Body );

			return true;
		}

		case CT_Function_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );

			return true;
		}

		case CT_Module:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );

			return true;
		}

		case CT_Namespace:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Body );

			return true;
		}

		case CT_Operator:
		case CT_Operator_Member:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );
			check_member_ast( Body );

			return true;
		}

		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ReturnType );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( Params );

			return true;
		}

		case CT_Operator_Cast:
		{
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( ValueType );
			check_member_ast( Body );

			return true;
		}

		case CT_Operator_Cast_Fwd:
		{
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( ValueType );

			return true;
		}

		case CT_Parameters:
		{
			if ( self->NumEntries > 1 )
			{
				gen_Code curr       = self;
				gen_Code curr_other = other;
				while ( curr != gen_nullptr  )
				{
					if ( curr )
					{
						if ( curr_other == gen_nullptr )
						{
							gen_log_fmt("\nAST::is_equal: Failed for parameter, other equivalent param is null\n"
									"gen_AST  : %S\n"
									"Other: %S\n"
									"For ast member: %S\n"
								, gen_code_debug_str(curr)
							);

							return false;
						}

						if ( gen_str_are_equal(curr->Name, curr_other->Name) )
						{
							gen_log_fmt( "\nAST::is_equal: Failed for parameter name check\n"
									"gen_AST  : %S\n"
									"Other: %S\n"
									"For     ast member: %S\n"
									"other's ast member: %S\n"
								, gen_code_debug_str(self)
								, gen_code_debug_str(other)
								, gen_code_debug_str(curr)
								, gen_code_debug_str(curr_other)
							);
							return false;
						}

						if ( curr->ValueType && ! gen_code_is_equal(curr->ValueType, curr_other->ValueType) )
						{
							gen_log_fmt( "\nAST::is_equal: Failed for parameter value type check\n"
									"gen_AST  : %S\n"
									"Other: %S\n"
									"For     ast member: %S\n"
									"other's ast member: %S\n"
								, gen_code_debug_str(self)
								, gen_code_debug_str(other)
								, gen_code_debug_str(curr)
								, gen_code_debug_str(curr_other)
							);
							return false;
						}

						if ( curr->Value && ! gen_code_is_equal(curr->Value, curr_other->Value) )
						{
							gen_log_fmt( "\nAST::is_equal: Failed for parameter value check\n"
									"gen_AST  : %S\n"
									"Other: %S\n"
									"For     ast member: %S\n"
									"other's ast member: %S\n"
								, gen_code_debug_str(self)
								, gen_code_debug_str(other)
								, gen_code_debug_str(curr)
								, gen_code_debug_str(curr_other)
							);
							return false;
						}
					}

					curr       = curr->Next;
					curr_other = curr_other->Next;
				}

				check_member_val( NumEntries );

				return true;
			}

			check_member_str( Name );
			check_member_ast( ValueType );
			check_member_ast( Value );
			check_member_ast( ArrExpr );

			return true;
		}

		case CT_Parameters_Define:
		{
			// TODO(ED): Needs implementaton
			gen_log_failure("gen_code_is_equal: NOT IMPLEMENTED for CT_Parameters_Define");
			return false;
		}

		case CT_Preprocess_Define:
		{
			check_member_str( Name );
			check_member_content( Body->Content );
			return true;
		}

		case CT_Preprocess_If:
		case CT_Preprocess_IfDef:
		case CT_Preprocess_IfNotDef:
		case CT_Preprocess_ElIf:
		{
			check_member_content( Content );

			return true;
		}

		case CT_Preprocess_Include:
		case CT_Preprocess_Pragma:
		{
			check_member_content( Content );

			return true;
		}

		case CT_Specifiers:
		{
			check_member_val( NumEntries );
			check_member_str( Name );
			for ( gen_s32 idx = 0; idx < self->NumEntries; ++idx )
			{
				check_member_val( ArrSpecs[ idx ] );
			}
			return true;
		}

		case CT_Template:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Params );
			check_member_ast( Declaration );

			return true;
		}

		case CT_Typedef:
		{
			check_member_val( IsFunction );
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( UnderlyingType );

			return true;
		}
		case CT_Typename:
		{
			check_member_val( IsParamPack );
			check_member_str( Name );
			check_member_ast( Specs );
			check_member_ast( ArrExpr );

			return true;
		}

		case CT_Union:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
			check_member_ast( Body );

			return true;
		}

		case CT_Union_Fwd:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( Attributes );
		}

		case CT_Using:
		case CT_Using_Namespace:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( UnderlyingType );
			check_member_ast( Attributes );

			return true;
		}

		case CT_Variable:
		{
			check_member_val( ModuleFlags );
			check_member_str( Name );
			check_member_ast( ValueType );
			check_member_ast( BitfieldSize );
			check_member_ast( Value );
			check_member_ast( Attributes );
			check_member_ast( Specs );
			check_member_ast( NextVar );

			return true;
		}

		case CT_Class_Body:
		case CT_Enum_Body:
		case CT_Export_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
		{
			check_member_ast( Front );
			check_member_ast( Back );

			gen_Code curr       = self->Front;
			gen_Code curr_other = other->Front;
			while ( curr != gen_nullptr )
			{
				if ( curr_other == gen_nullptr )
				{
					gen_log_fmt("\nAST::is_equal: Failed for body, other equivalent param is null\n"
							"gen_AST  : %S\n"
							"Other: %S\n"
						, gen_code_debug_str(curr)
						, gen_code_debug_str(other)
					);

					return false;
				}

				if ( ! gen_code_is_equal( curr, curr_other ) )
				{
					gen_log_fmt( "\nAST::is_equal: Failed for body\n"
							"gen_AST  : %S\n"
							"Other: %S\n"
							"For     ast member: %S\n"
							"other's ast member: %S\n"
						, gen_code_debug_str(self)
						, gen_code_debug_str(other)
						, gen_code_debug_str(curr)
						, gen_code_debug_str(curr_other)
					);

					return false;
				}

				curr       = curr->Next;
				curr_other = curr_other->Next;
			}

			check_member_val( NumEntries );

			return true;
		}

	#undef check_member_val
	#undef check_member_str
	#undef check_member_ast
	}

	return true;
}

bool gen_code__validate_body( gen_Code self)
{
switch ( self->Type )
	{
		case CT_Class_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for (gen_Code gen_code_entry = gen_begin_CodeBody(body); gen_code_entry != gen_end_CodeBody(body); gen_next_CodeBody(body, gen_code_entry)) switch (gen_code_entry->Type)
			{
				GEN_AST_BODY_CLASS_UNALLOWED_TYPES_CASES:
					gen_log_failure("gen_AST::validate_body: Invalid entry in body %S", gen_code_debug_str(gen_code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Enum_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for ( gen_Code entry = gen_begin_CodeBody(body); entry != gen_end_CodeBody(body); gen_next_CodeBody(body, entry) )
			{
				if ( entry->Type != CT_Untyped )
				{
					gen_log_failure( "gen_AST::validate_body: Invalid entry in enum body (needs to be untyped or comment) %S", gen_code_debug_str(entry) );
					return false;
				}
			}
		}
		break;
		case CT_Export_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for (gen_Code gen_code_entry = gen_begin_CodeBody(body); gen_code_entry != gen_end_CodeBody(body); gen_next_CodeBody(body, gen_code_entry)) switch (gen_code_entry->Type)
			{
				GEN_AST_BODY_EXPORT_UNALLOWED_TYPES_CASES:
					gen_log_failure("gen_AST::validate_body: Invalid entry in body %S", gen_code_debug_str(gen_code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Extern_Linkage:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for (gen_Code gen_code_entry = gen_begin_CodeBody(body); gen_code_entry != gen_end_CodeBody(body); gen_next_CodeBody(body, gen_code_entry)) switch (gen_code_entry->Type)
			{
				GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES_CASES:
					gen_log_failure("gen_AST::validate_body: Invalid entry in body %S", gen_code_debug_str(gen_code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Function_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for (gen_Code gen_code_entry = gen_begin_CodeBody(body); gen_code_entry != gen_end_CodeBody(body); gen_next_CodeBody(body, gen_code_entry)) switch (gen_code_entry->Type)
			{
				GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES_CASES:
					gen_log_failure("gen_AST::validate_body: Invalid entry in body %S", gen_code_debug_str(gen_code_entry));
				return false;

				default:
				continue;
			}
		}
		break;
		case CT_Global_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for ( gen_Code entry = gen_begin_CodeBody(body); entry != gen_end_CodeBody(body); gen_next_CodeBody(body, entry) )switch (entry->Type)
			{
				GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES_CASES:
					gen_log_failure("gen_AST::validate_body: Invalid entry in body %S", gen_code_debug_str(entry));
				return false;
			}
		}
		break;
		case CT_Namespace_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for ( gen_Code entry = gen_begin_CodeBody(body); entry != gen_end_CodeBody(body); gen_next_CodeBody(body, entry) ) switch (entry->Type)
			{
				GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES_CASES:
					gen_log_failure("gen_AST::validate_body: Invalid entry in body %S", gen_code_debug_str(entry));
				return false;
			}
		}
		break;
		case CT_Struct_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for ( gen_Code entry = gen_begin_CodeBody(body); entry != gen_end_CodeBody(body); gen_next_CodeBody(body, entry) ) switch (entry->Type)
			{
				GEN_AST_BODY_STRUCT_UNALLOWED_TYPES_CASES:
					gen_log_failure("gen_AST::validate_body: Invalid entry in body %S", gen_code_debug_str(entry));
				return false;
			}
		}
		break;
		case CT_Union_Body:
		{
			gen_CodeBody body = gen_cast(gen_CodeBody, self);
			for ( gen_Code entry = gen_begin_CodeBody(body); entry != gen_end_CodeBody(body); gen_next_CodeBody(body, entry) )
			{
				if ( entry->Type != CT_Untyped )
				{
					gen_log_failure( "gen_AST::validate_body: Invalid entry in union body (needs to be untyped or comment) %S", gen_code_debug_str(entry) );
					return false;
				}
			}
		}
		break;

		default:
			gen_log_failure( "gen_AST::validate_body: Invalid this gen_AST does not have a body %S", gen_code_debug_str(self) );
			return false;
	}
	return false;
}

gen_StrBuilder gen_body_to_strbuilder(gen_CodeBody body)
{
	GEN_ASSERT(body);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 128 );
	switch ( body->Type )
	{
		case CT_Untyped:
		case CT_Execution:
			gen_strbuilder_append_str( & result, gen_cast(gen_Code, body)->Content );
		break;

		case CT_Enum_Body:
		case CT_Class_Body:
		case CT_Extern_Linkage_Body:
		case CT_Function_Body:
		case CT_Global_Body:
		case CT_Namespace_Body:
		case CT_Struct_Body:
		case CT_Union_Body:
			gen_body_to_strbuilder_ref( body, & result );
		break;

		case CT_Export_Body:
			gen_body_to_strbuilder_export( body, & result );
		break;
	}
	return result;
}

void gen_body_to_strbuilder_export( gen_CodeBody body, gen_StrBuilder* result )
{
	GEN_ASSERT(body   != gen_nullptr);
	GEN_ASSERT(result != gen_nullptr);
	gen_strbuilder_append_fmt( result, "export\n{\n" );

	gen_Code curr = body->Front;
	gen_s32  left = body->NumEntries;
	while ( left-- )
	{
		gen_code_to_strbuilder_ref(curr, result);
		// gen_strbuilder_append_fmt( result, "%SB", gen_code_to_strbuilder(curr) );
		curr = curr->Next;
	}

	gen_strbuilder_append_fmt( result, "};\n" );
}

gen_StrBuilder gen_constructor__to_strbuilder(gen_CodeConstructor self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 128 );
	switch (self->Type)
	{
		case CT_Constructor:
			gen_constructor__to_strbuilder_def( self, & result );
		break;
		case CT_Constructor_Fwd:
			gen_constructor__to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void gen_constructor__to_strbuilder_def(gen_CodeConstructor self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	gen_Code ClassStructParent = self->Parent->Parent;
	if (ClassStructParent) {
		gen_strbuilder_append_str( result, ClassStructParent->Name );
	}
	else {
		gen_strbuilder_append_str( result, self->Name );
	}

	if ( self->Params )
		gen_strbuilder_append_fmt( result, "( %SB )", gen_params_to_strbuilder(self->Params) );
	else
		gen_strbuilder_append_str( result, gen_txt("()") );

	if ( self->InitializerList )
		gen_strbuilder_append_fmt( result, " : %SB", gen_code_to_strbuilder(self->InitializerList) );

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, " // %S", self->InlineCmt->Content );

	gen_strbuilder_append_fmt( result, "\n{\n%SB\n}\n", gen_code_to_strbuilder(self->Body) );
}

void gen_constructor__to_strbuilder_fwd(gen_CodeConstructor self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	gen_Code ClassStructParent = self->Parent->Parent;
	if (ClassStructParent) {
		gen_strbuilder_append_str( result, ClassStructParent->Name );
	}
	else {
		gen_strbuilder_append_str( result, self->Name );
	}

	if ( self->Params )
		gen_strbuilder_append_fmt( result, "( %SB )", gen_params_to_strbuilder(self->Params) );
	else
		gen_strbuilder_append_fmt( result, "()");

	if (self->Body)
		gen_strbuilder_append_fmt( result, " = %SB", gen_code_to_strbuilder(self->Body) );

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, "; // %S\n", self->InlineCmt->Content );
	else
		gen_strbuilder_append_str( result, gen_txt(";\n") );
}

gen_StrBuilder gen_class_to_strbuilder( gen_CodeClass self )
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Class:
			gen_class_to_strbuilder_def(self, & result );
		break;
		case CT_Class_Fwd:
			gen_class_to_strbuilder_fwd(self, & result );
		break;
	}
	return result;
}

void gen_class_to_strbuilder_def( gen_CodeClass self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);

	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	gen_strbuilder_append_str( result, gen_txt("class ") );

	if ( self->Attributes )
	{
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );
	}

	if ( self->Name.Len )
		gen_strbuilder_append_str( result, self->Name );

	if (self->Specs && gen_specifiers_has(self->Specs, Spec_Final))
		gen_strbuilder_append_str(result, gen_txt(" final"));

	if ( self->ParentType )
	{
		gen_Str access_level = gen_access_spec_to_str( self->ParentAccess );
		gen_strbuilder_append_fmt( result, " : %S %SB", self->Name, access_level, gen_typename_to_strbuilder(self->ParentType) );

		gen_CodeTypename interface = gen_cast(gen_CodeTypename, self->ParentType->Next);
		if ( interface )
			gen_strbuilder_append_str( result, gen_txt("\n") );

		while ( interface )
		{
			gen_strbuilder_append_fmt( result, ", public %SB", gen_typename_to_strbuilder(interface) );
			interface = interface->Next ? gen_cast(gen_CodeTypename, interface->Next) : gen_NullCode;
		}
	}

	if ( self->InlineCmt )
	{
		gen_strbuilder_append_fmt( result, " // %S", self->InlineCmt->Content );
	}

	gen_strbuilder_append_fmt( result, "\n{\n%SB\n}", gen_body_to_strbuilder(self->Body) );

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		gen_strbuilder_append_str( result, gen_txt(";\n") );
}

void gen_class_to_strbuilder_fwd( gen_CodeClass self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);

	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "class %SB %S", gen_attributes_to_strbuilder(self->Attributes), self->Name );

	else gen_strbuilder_append_fmt( result, "class %S", self->Name );

	// Check if it can have an end-statement
	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			gen_strbuilder_append_fmt( result, "; // %S\n", self->InlineCmt->Content );
		else
			gen_strbuilder_append_str( result, gen_txt(";\n") );
	}
}

void gen_define_to_strbuilder_ref(gen_CodeDefine define, gen_StrBuilder* result )
{
	GEN_ASSERT(define);
	GEN_ASSERT(define->Body);
	GEN_ASSERT(define->Body->Content.Ptr && define->Body->Content.Len > 0);
	if (define->Params) {
		gen_StrBuilder gen_params_builder = gen_define_params_to_strbuilder(define->Params);
		gen_strbuilder_append_fmt( result, "#define %S(%S) %S", define->Name, gen_strbuilder_to_str(gen_params_builder), define->Body->Content );
	}
	else {
		gen_strbuilder_append_fmt( result, "#define %S %S", define->Name, define->Body->Content );
	}
}

void gen_define_params_to_strbuilder_ref(gen_CodeDefineParams self, gen_StrBuilder* result)
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Name.Ptr && self->Name.Len )
	{
		gen_strbuilder_append_fmt( result, " %S", self->Name );
	}
	if ( self->NumEntries - 1 > 0 )
	{
		for ( gen_CodeDefineParams param = gen_begin_CodeDefineParams(self->Next); param != gen_end_CodeDefineParams(self->Next); param = gen_next_CodeDefineParams(self->Next, param) )
		{
			gen_strbuilder_append_fmt( result, ", %SB", gen_define_params_to_strbuilder(param) );
		}
	}
}

gen_StrBuilder gen_destructor__to_strbuilder(gen_CodeDestructor self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 128 );
	switch ( self->Type )
	{
		case CT_Destructor:
			gen_destructor__to_strbuilder_def( self, & result );
		break;
		case CT_Destructor_Fwd:
			gen_destructor__to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void gen_destructor__to_strbuilder_def(gen_CodeDestructor self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Name.Len )
	{
		gen_strbuilder_append_fmt( result, "%S()", self->Name );
	}
	else if ( self->Specs )
	{
		if ( gen_specifiers_has(self->Specs, Spec_Virtual ) )
			gen_strbuilder_append_fmt( result, "virtual ~%S()", self->Parent->Name );
		else
			gen_strbuilder_append_fmt( result, "~%S()", self->Parent->Name );
	}
	else
		gen_strbuilder_append_fmt( result, "~%S()", self->Parent->Name );

	gen_strbuilder_append_fmt( result, "\n{\n%SB\n}\n", gen_code_to_strbuilder(self->Body) );
}

void gen_destructor__to_strbuilder_fwd(gen_CodeDestructor self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Specs )
	{
		if ( gen_specifiers_has(self->Specs, Spec_Virtual ) )
			gen_strbuilder_append_fmt( result, "virtual ~%S();\n", self->Parent->Name );
		else
			gen_strbuilder_append_fmt( result, "~%S()", self->Parent->Name );

		if ( gen_specifiers_has(self->Specs, Spec_Pure ) )
			gen_strbuilder_append_str( result, gen_txt(" = 0;") );
		else if (self->Body)
			gen_strbuilder_append_fmt( result, " = %SB;", gen_code_to_strbuilder(self->Body) );
	}
	else
		gen_strbuilder_append_fmt( result, "~%S();", self->Parent->Name );

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, "  %S", self->InlineCmt->Content );
	else
		gen_strbuilder_append_str( result, gen_txt("\n"));
}

gen_StrBuilder gen_enum_to_strbuilder(gen_CodeEnum self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Enum:
			gen_enum_to_strbuilder_def(self, & result );
		break;
		case CT_Enum_Fwd:
			gen_enum_to_strbuilder_fwd(self, & result );
		break;
		case CT_Enum_Class:
			gen_enum_to_strbuilder_class_def(self, & result );
		break;
		case CT_Enum_Class_Fwd:
			gen_enum_to_strbuilder_class_fwd(self, & result );
		break;
	}
	return result;
}

void gen_enum_to_strbuilder_def(gen_CodeEnum self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes || self->UnderlyingType || self->UnderlyingTypeMacro )
	{
		gen_strbuilder_append_str( result, gen_txt("enum ") );

		if ( self->Attributes )
			gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

		if ( self->UnderlyingType )
			gen_strbuilder_append_fmt( result, "%S : %SB\n{\n%SB\n}"
				, self->Name
				, gen_typename_to_strbuilder(self->UnderlyingType)
				, gen_body_to_strbuilder(self->Body)
			);
		else if ( self->UnderlyingTypeMacro )
			gen_strbuilder_append_fmt( result, "%S %SB\n{\n%SB\n}"
				, self->Name
				, gen_code_to_strbuilder(self->UnderlyingTypeMacro)
				, gen_body_to_strbuilder(self->Body)
			);

		else gen_strbuilder_append_fmt( result, "%S\n{\n%SB\n}", self->Name, gen_body_to_strbuilder(self->Body) );
	}
	else gen_strbuilder_append_fmt( result, "enum %S\n{\n%SB\n}", self->Name, gen_body_to_strbuilder(self->Body) );

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		gen_strbuilder_append_str( result, gen_txt(";\n"));
}

void gen_enum_to_strbuilder_fwd(gen_CodeEnum self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	if ( self->UnderlyingType )
		gen_strbuilder_append_fmt( result, "enum %S : %SB", self->Name, gen_typename_to_strbuilder(self->UnderlyingType) );
	else if (self->UnderlyingTypeMacro)
	{
		gen_log_fmt("IDENTIFIED A UNDERLYING ENUM MACRO");
		gen_strbuilder_append_fmt( result, "enum %S %SB", self->Name, gen_code_to_strbuilder(self->UnderlyingTypeMacro) );
	}
	else
		gen_strbuilder_append_fmt( result, "enum %S", self->Name );

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			gen_strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			gen_strbuilder_append_str( result, gen_txt(";\n"));
	}
}

void gen_enum_to_strbuilder_class_def(gen_CodeEnum self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes || self->UnderlyingType )
	{
		gen_strbuilder_append_str( result, gen_txt("enum class ") );

		if ( self->Attributes )
		{
			gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );
		}

		if ( self->UnderlyingType )
		{
			gen_strbuilder_append_fmt( result, "%S : %SB\n{\n%SB\n}", self->Name, gen_typename_to_strbuilder(self->UnderlyingType), gen_body_to_strbuilder(self->Body) );
		}
		else
		{
			gen_strbuilder_append_fmt( result, "%S\n{\n%SB\n}", self->Name, gen_body_to_strbuilder(self->Body) );
		}
	}
	else
	{
		gen_strbuilder_append_fmt( result, "enum %S\n{\n%SB\n}", self->Name, gen_body_to_strbuilder(self->Body) );
	}

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		gen_strbuilder_append_str( result, gen_txt(";\n"));
}

void gen_enum_to_strbuilder_class_fwd(gen_CodeEnum self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	gen_strbuilder_append_str( result, gen_txt("enum class ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	gen_strbuilder_append_fmt( result, "%S : %SB", self->Name, gen_typename_to_strbuilder(self->UnderlyingType) );

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			gen_strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			gen_strbuilder_append_str( result, gen_txt(";\n"));
	}
}

gen_StrBuilder gen_fn_to_strbuilder(gen_CodeFn self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Function:
			gen_fn_to_strbuilder_def(self, & result );
		break;
		case CT_Function_Fwd:
			gen_fn_to_strbuilder_fwd(self, & result );
		break;
	}
	return result;
}

void gen_fn_to_strbuilder_def(gen_CodeFn self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, " %SB ", gen_attributes_to_strbuilder(self->Attributes) );

	bool prefix_specs = false;
	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );

				prefix_specs = true;
			}
		}
	}

	if ( self->Attributes || prefix_specs )
		gen_strbuilder_append_str( result, gen_txt("\n") );

	if ( self->ReturnType )
		gen_strbuilder_append_fmt( result, "%SB %S(", gen_typename_to_strbuilder(self->ReturnType), self->Name );

	else
		gen_strbuilder_append_fmt( result, "%S(", self->Name );

	if ( self->Params )
		gen_strbuilder_append_fmt( result, "%SB)", gen_params_to_strbuilder(self->Params) );

	else
		gen_strbuilder_append_str( result, gen_txt(")") );

	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}
	}

	// This is bodged in for now SOLEY for Unreal's PURE_VIRTUAL functional macro
	if ( self->SuffixSpecs )
		gen_strbuilder_append_fmt( result, " %SB", gen_code_to_strbuilder(self->SuffixSpecs) );

	gen_strbuilder_append_fmt( result, "\n{\n%SB\n}\n", gen_body_to_strbuilder(self->Body) );
}

void gen_fn_to_strbuilder_fwd(gen_CodeFn self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	gen_b32 prefix_specs = false;
	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! gen_spec_is_trailing( * spec ) || ! ( * spec != Spec_Pure) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );

				prefix_specs = true;
			}
		}
	}

	if ( self->Attributes || prefix_specs )
	{
		gen_strbuilder_append_str( result, gen_txt("\n") );
	}

	if ( self->ReturnType )
		gen_strbuilder_append_fmt( result, "%SB %S(", gen_typename_to_strbuilder(self->ReturnType), self->Name );

	else
		gen_strbuilder_append_fmt( result, "%S(", self->Name );

	if ( self->Params )
		gen_strbuilder_append_fmt( result, "%SB)", gen_params_to_strbuilder(self->Params) );

	else
		gen_strbuilder_append_str( result, gen_txt(")") );

	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}

		if ( gen_specifiers_has(self->Specs, Spec_Pure ) )
			gen_strbuilder_append_str( result, gen_txt(" = 0") );
		else if ( gen_specifiers_has(self->Specs, Spec_Delete ) )
			gen_strbuilder_append_str( result, gen_txt(" = delete") );
	}

	// This is bodged in for now SOLEY for Unreal's PURE_VIRTUAL functional macro (I kept it open ended for other jank)
	if ( self->SuffixSpecs )
		gen_strbuilder_append_fmt( result, " %SB", gen_code_to_strbuilder(self->SuffixSpecs) );

	if (self->Body)
		gen_strbuilder_append_fmt( result, " = %SB;", gen_body_to_strbuilder(self->Body) );

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
	else
		gen_strbuilder_append_str( result, gen_txt(";\n") );
}

void gen_module_to_strbuilder_ref(gen_CodeModule self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if (((gen_scast(gen_u32, ModuleFlag_Export) & gen_scast(gen_u32, self->ModuleFlags)) == gen_scast(gen_u32, ModuleFlag_Export)))
		gen_strbuilder_append_str( result, gen_txt("export "));

	if (((gen_scast(gen_u32, ModuleFlag_Import) & gen_scast(gen_u32, self->ModuleFlags)) == gen_scast(gen_u32, ModuleFlag_Import)))
		gen_strbuilder_append_str( result, gen_txt("import "));

	gen_strbuilder_append_fmt( result, "%S;\n", self->Name );
}

gen_StrBuilder gen_code_op_to_strbuilder(gen_CodeOperator self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Operator:
		case CT_Operator_Member:
			gen_code_op_to_strbuilder_def( self, & result );
		break;
		case CT_Operator_Fwd:
		case CT_Operator_Member_Fwd:
			gen_code_op_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void gen_code_op_to_strbuilder_def(gen_CodeOperator self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}
	}

	if ( self->Attributes || self->Specs )
	{
		gen_strbuilder_append_str( result, gen_txt("\n") );
	}

	if ( self->ReturnType )
		gen_strbuilder_append_fmt( result, "%SB %S (", gen_typename_to_strbuilder(self->ReturnType), self->Name );

	if ( self->Params )
		gen_strbuilder_append_fmt( result, "%SB)", gen_params_to_strbuilder(self->Params) );

	else
		gen_strbuilder_append_str( result, gen_txt(")") );

	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}
	}

	gen_strbuilder_append_fmt( result, "\n{\n%SB\n}\n"
		, gen_body_to_strbuilder(self->Body)
	);
}

void gen_code_op_to_strbuilder_fwd(gen_CodeOperator self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB\n", gen_attributes_to_strbuilder(self->Attributes) );

	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}
	}

	if ( self->Attributes || self->Specs )
	{
		gen_strbuilder_append_str( result, gen_txt("\n") );
	}

	gen_strbuilder_append_fmt( result, "%SB %S (", gen_typename_to_strbuilder(self->ReturnType), self->Name );

	if ( self->Params )
		gen_strbuilder_append_fmt( result, "%SB)", gen_params_to_strbuilder(self->Params) );

	else
		gen_strbuilder_append_fmt( result, ")" );

	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}
	}

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
	else
		gen_strbuilder_append_str( result, gen_txt(";\n") );
}

gen_StrBuilder gen_opcast_to_strbuilder(gen_CodeOpCast self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 128 );
	switch ( self->Type )
	{
		case CT_Operator_Cast:
			gen_opcast_to_strbuilder_def(self, & result );
		break;
		case CT_Operator_Cast_Fwd:
			gen_opcast_to_strbuilder_fwd(self, & result );
		break;
	}
	return result;
}

void gen_opcast_to_strbuilder_def(gen_CodeOpCast self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, "%*s ", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}

		if ( self->Name.Ptr && self->Name.Len )
			gen_strbuilder_append_fmt( result, "%S operator %SB()", self->Name, gen_typename_to_strbuilder(self->ValueType) );
		else
			gen_strbuilder_append_fmt( result, "operator %SB()", gen_typename_to_strbuilder(self->ValueType) );

		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %.*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}

		gen_strbuilder_append_fmt( result, "\n{\n%SB\n}\n", gen_body_to_strbuilder(self->Body) );
		return;
	}

	if ( self->Name.Ptr && self->Name.Len )
		gen_strbuilder_append_fmt( result, "%S operator %SB()\n{\n%SB\n}\n", self->Name, gen_typename_to_strbuilder(self->ValueType), gen_body_to_strbuilder(self->Body) );
	else
		gen_strbuilder_append_fmt( result, "operator %SB()\n{\n%SB\n}\n", gen_typename_to_strbuilder(self->ValueType), gen_body_to_strbuilder(self->Body) );
}

void gen_opcast_to_strbuilder_fwd(gen_CodeOpCast self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Specs )
	{
		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( ! gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, "%*s ", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}

		gen_strbuilder_append_fmt( result, "operator %SB()", gen_typename_to_strbuilder(self->ValueType) );

		for ( gen_Specifier* spec = gen_begin_CodeSpecifiers(self->Specs); spec != gen_end_CodeSpecifiers(self->Specs); spec = gen_next_CodeSpecifiers(self->Specs, spec) )
		{
			if ( gen_spec_is_trailing( * spec ) )
			{
				gen_Str gen_spec_str = gen_spec_to_str( * spec );
				gen_strbuilder_append_fmt( result, " %*s", gen_spec_str.Len, gen_spec_str.Ptr );
			}
		}

		if ( self->InlineCmt )
			gen_strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			gen_strbuilder_append_str( result, gen_txt(";\n") );
		return;
	}

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, "operator %SB();  %SB", gen_typename_to_strbuilder(self->ValueType) );
	else
		gen_strbuilder_append_fmt( result, "operator %SB();\n", gen_typename_to_strbuilder(self->ValueType) );
}

void gen_params_to_strbuilder_ref( gen_CodeParams self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->gen_Macro )
	{
		// Related to parsing: ( <macro>, ... )
		gen_strbuilder_append_str( result, self->gen_Macro->Content );
		// Could also be: ( <macro> <type <name>, ... )
	}

	if ( self->Name.Ptr && self->Name.Len )
	{
		if ( self->ValueType == gen_nullptr )
			gen_strbuilder_append_fmt( result, " %S", self->Name );
		else
			gen_strbuilder_append_fmt( result, " %SB %S", gen_typename_to_strbuilder(self->ValueType), self->Name );

	}
	else if ( self->ValueType )
		gen_strbuilder_append_fmt( result, " %SB", gen_typename_to_strbuilder(self->ValueType) );

	if ( self->PostNameMacro )
	{
		gen_strbuilder_append_fmt( result, " %SB", gen_code_to_strbuilder(self->PostNameMacro) );
	}

	if ( self->Value )
		gen_strbuilder_append_fmt( result, " = %SB", gen_code_to_strbuilder(self->Value) );

	if ( self->NumEntries - 1 > 0 )
	{
		for ( gen_CodeParams param = gen_begin_CodeParams(self->Next); param != gen_end_CodeParams(self->Next); param = gen_next_CodeParams(self->Next, param) )
		{
			gen_strbuilder_append_fmt( result, ", %SB", gen_params_to_strbuilder(param) );
		}
	}
}

gen_StrBuilder gen_preprocess_to_strbuilder(gen_CodePreprocessCond self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 256 );
	switch ( self->Type )
	{
		case CT_Preprocess_If:
			gen_preprocess_to_strbuilder_if( self, & result );
		break;
		case CT_Preprocess_IfDef:
			gen_preprocess_to_strbuilder_ifdef( self, & result );
		break;
		case CT_Preprocess_IfNotDef:
			gen_preprocess_to_strbuilder_ifndef( self, & result );
		break;
		case CT_Preprocess_ElIf:
			gen_preprocess_to_strbuilder_elif( self, & result );
		break;
		case CT_Preprocess_Else:
			gen_preprocess_to_strbuilder_else( self, & result );
		break;
		case CT_Preprocess_EndIf:
			gen_preprocess_to_strbuilder_endif( self, & result );
		break;
	}
	return result;
}

void gen_specifiers_to_strbuilder_ref( gen_CodeSpecifiers self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	gen_s32 idx  = 0;
	gen_s32 left = self->NumEntries;
	while ( left -- )
	{
		gen_Specifier spec     = self->ArrSpecs[idx];
		gen_Str       gen_spec_str = gen_spec_to_str( spec );
		if ( idx > 0 ) switch (spec) {
			case Spec_Ptr:
			case Spec_Ref:
			case Spec_RValue:
			break;

			default:
				gen_strbuilder_append_str(result, gen_txt(" "));
			break;
		}
		gen_strbuilder_append_fmt( result, "%S", gen_spec_str );
		idx++;
	}
	gen_strbuilder_append_str(result, gen_txt(" "));
}

gen_StrBuilder gen_struct_to_strbuilder(gen_CodeStruct self)
{
	GEN_ASSERT(self);
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Struct:
			gen_struct_to_strbuilder_def( self, & result );
		break;
		case CT_Struct_Fwd:
			gen_struct_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void gen_struct_to_strbuilder_def( gen_CodeStruct self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	gen_strbuilder_append_str( result, gen_txt("struct ") );

	if ( self->Attributes )
	{
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );
	}

	if ( self->Name.Len )
		gen_strbuilder_append_str( result, self->Name );

	if (self->Specs && gen_specifiers_has(self->Specs, Spec_Final))
		gen_strbuilder_append_str( result, gen_txt(" final"));

	if ( self->ParentType )
	{
		gen_Str access_level = gen_access_spec_to_str( self->ParentAccess );

		gen_strbuilder_append_fmt( result, " : %S %SB", access_level, gen_typename_to_strbuilder(self->ParentType) );

		gen_CodeTypename interface = gen_cast(gen_CodeTypename, self->ParentType->Next);
		if ( interface )
			gen_strbuilder_append_str( result, gen_txt("\n") );

		while ( interface )
		{
			gen_strbuilder_append_fmt( result, ", %SB", gen_typename_to_strbuilder(interface) );
			interface = interface->Next ? gen_cast( gen_CodeTypename, interface->Next) : gen_NullCode;
		}
	}

	if ( self->InlineCmt )
	{
		gen_strbuilder_append_fmt( result, " // %S", self->InlineCmt->Content );
	}

	gen_strbuilder_append_fmt( result, "\n{\n%SB\n}", gen_body_to_strbuilder(self->Body) );

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		gen_strbuilder_append_str( result, gen_txt(";\n"));
}

void gen_struct_to_strbuilder_fwd( gen_CodeStruct self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "struct %SB %S", gen_attributes_to_strbuilder(self->Attributes), self->Name );

	else gen_strbuilder_append_fmt( result, "struct %S", self->Name );

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
	{
		if ( self->InlineCmt )
			gen_strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content );
		else
			gen_strbuilder_append_str( result, gen_txt( ";\n") );
	}
}

void gen_template_to_strbuilder_ref(gen_CodeTemplate self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Params )
		gen_strbuilder_append_fmt( result, "template< %SB >\n%SB", gen_params_to_strbuilder(self->Params), gen_code_to_strbuilder(self->Declaration) );
	else
		gen_strbuilder_append_fmt( result, "template<>\n%SB", gen_code_to_strbuilder(self->Declaration) );
}

void gen_typedef_to_strbuilder_ref(gen_CodeTypedef self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	gen_strbuilder_append_str( result, gen_txt("typedef "));

	// Determines if the typedef is a function typename
	if ( self->UnderlyingType->ReturnType )
		gen_strbuilder_append_string( result, gen_code_to_strbuilder(self->UnderlyingType) );
	else
		gen_strbuilder_append_fmt( result, "%SB %S", gen_code_to_strbuilder(self->UnderlyingType), self->Name );

	if ( self->UnderlyingType->Type == CT_Typename && self->UnderlyingType->ArrExpr )
	{
		gen_strbuilder_append_fmt( result, "[ %SB ];", gen_code_to_strbuilder(self->UnderlyingType->ArrExpr) );

		gen_Code gen_next_arr_expr = self->UnderlyingType->ArrExpr->Next;
		while ( gen_next_arr_expr )
		{
			gen_strbuilder_append_fmt( result, "[ %SB ];", gen_code_to_strbuilder(gen_next_arr_expr) );
			gen_next_arr_expr = gen_next_arr_expr->Next;
		}
	}
	else
	{
		gen_strbuilder_append_str( result, gen_txt(";") );
	}

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, "  %S", self->InlineCmt->Content);
	else
		gen_strbuilder_append_str( result, gen_txt("\n"));
}

void gen_typename_to_strbuilder_ref(gen_CodeTypename self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	#if defined(GEN_USE_NEW_TYPENAME_PARSING)
		if ( self->ReturnType && self->Params )
		{
			if ( self->Attributes )
				gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );
			else
			{
				if ( self->Specs )
					gen_strbuilder_append_fmt( result, "%SB ( %S ) ( %SB ) %SB", gen_typename_to_strbuilder(self->ReturnType), self->Name, gen_params_to_strbuilder(self->Params), gen_specifiers_to_strbuilder(self->Specs) );
				else
					gen_strbuilder_append_fmt( result, "%SB ( %S ) ( %SB )", gen_typename_to_strbuilder(self->ReturnType), self->Name, gen_params_to_strbuilder(self->Params) );
			}

			break;
		}
	#else
		if ( self->ReturnType && self->Params )
		{
			if ( self->Attributes )
				gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );
			else
			{
				if ( self->Specs )
					gen_strbuilder_append_fmt( result, "%SB %S ( %SB ) %SB", gen_typename_to_strbuilder(self->ReturnType), self->Name, gen_params_to_strbuilder(self->Params), gen_specifiers_to_strbuilder(self->Specs) );
				else
					gen_strbuilder_append_fmt( result, "%SB %S ( %SB )", gen_typename_to_strbuilder(self->ReturnType), self->Name, gen_params_to_strbuilder(self->Params) );
			}

			return;
		}
	#endif

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	switch ( self->TypeTag )
	{
		case Tag_Class  : gen_strbuilder_append_str( result, gen_txt("class "));  break;
		case Tag_Enum   : gen_strbuilder_append_str( result, gen_txt("enum "));   break;
		case Tag_Struct : gen_strbuilder_append_str( result, gen_txt("struct ")); break;
		case Tag_Union  : gen_strbuilder_append_str( result, gen_txt("union "));  break;
		default:
			break;
	}

	if ( self->Specs )
		gen_strbuilder_append_fmt( result, "%S %SB", self->Name, gen_specifiers_to_strbuilder(self->Specs) );
	else
		gen_strbuilder_append_fmt( result, "%S", self->Name );

	if ( self->IsParamPack )
		gen_strbuilder_append_str( result, gen_txt("..."));
}

gen_StrBuilder union_to_strbuilder(gen_CodeUnion self)
{
	GEN_ASSERT(self);
	gen_StrBuilder result = gen_strbuilder_make_reserve( gen__ctx->Allocator_Temp, 512 );
	switch ( self->Type )
	{
		case CT_Union:
			union_to_strbuilder_def( self, & result );
		break;
		case CT_Union_Fwd:
			union_to_strbuilder_fwd( self, & result );
		break;
	}
	return result;
}

void union_to_strbuilder_def(gen_CodeUnion self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	gen_strbuilder_append_str( result, gen_txt("union ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	if ( self->Name.Len )
	{
		gen_strbuilder_append_fmt( result, "%S\n{\n%SB\n}"
			, self->Name
			, gen_body_to_strbuilder(self->Body)
		);
	}
	else
	{
		// Anonymous union
		gen_strbuilder_append_fmt( result, "\n{\n%SB\n}"
			, gen_body_to_strbuilder(self->Body)
		);
	}

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		gen_strbuilder_append_str( result, gen_txt(";\n"));
}

void union_to_strbuilder_fwd(gen_CodeUnion self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	gen_strbuilder_append_str( result, gen_txt("union ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	if ( self->Name.Len )
	{
		gen_strbuilder_append_fmt( result, "%S", self->Name);
	}

	if ( self->Parent == gen_nullptr || ( self->Parent->Type != CT_Typedef && self->Parent->Type != CT_Variable ) )
		gen_strbuilder_append_str( result, gen_txt(";\n"));
}

void gen_using_to_strbuilder_ref(gen_CodeUsing self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes )
		gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

	if ( self->UnderlyingType )
	{
		gen_strbuilder_append_fmt( result, "using %S = %SB", self->Name, gen_typename_to_strbuilder(self->UnderlyingType) );

		if ( self->UnderlyingType->ArrExpr )
		{
			gen_strbuilder_append_fmt( result, "[ %SB ]", gen_code_to_strbuilder(self->UnderlyingType->ArrExpr) );

			gen_Code gen_next_arr_expr = self->UnderlyingType->ArrExpr->Next;
			while ( gen_next_arr_expr )
			{
				gen_strbuilder_append_fmt( result, "[ %SB ]", gen_code_to_strbuilder(gen_next_arr_expr) );
				gen_next_arr_expr = gen_next_arr_expr->Next;
			}
		}

		gen_strbuilder_append_str( result, gen_txt(";") );
	}
	else
		gen_strbuilder_append_fmt( result, "using %S;", self->Name );

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, "  %S\n", self->InlineCmt->Content );
	else
		gen_strbuilder_append_str( result, gen_txt("\n"));
}

gen_neverinline
void gen_var_to_strbuilder_ref(gen_CodeVar self, gen_StrBuilder* result )
{
	GEN_ASSERT(self);
	GEN_ASSERT(result);
	if ( self->Parent && self->Parent->Type == CT_Variable )
	{
		// Its a comma-separated variable ( a NextVar )

		if ( self->Specs )
			gen_strbuilder_append_fmt( result, "%SB ", gen_specifiers_to_strbuilder(self->Specs) );

		gen_strbuilder_append_str( result, self->Name );

		if ( self->ValueType && self->ValueType->ArrExpr )
		{
			gen_strbuilder_append_fmt( result, "[ %SB ]", gen_code_to_strbuilder(self->ValueType->ArrExpr) );

			gen_Code gen_next_arr_expr = self->ValueType->ArrExpr->Next;
			while ( gen_next_arr_expr )
			{
				gen_strbuilder_append_fmt( result, "[ %SB ]", gen_code_to_strbuilder(gen_next_arr_expr) );
				gen_next_arr_expr = gen_next_arr_expr->Next;
			}
		}

		if ( self->Value )
		{
			if ( self->VarParenthesizedInit )
				gen_strbuilder_append_fmt( result, "( %SB ", gen_code_to_strbuilder(self->Value) );
			else
				gen_strbuilder_append_fmt( result, " = %SB", gen_code_to_strbuilder(self->Value) );
		}

		// Keep the chain going...
		if ( self->NextVar )
			gen_strbuilder_append_fmt( result, ", %SB", gen_var_to_strbuilder(self->NextVar) );

		if ( self->VarParenthesizedInit )
			gen_strbuilder_append_str( result, gen_txt(" )"));

		return;
	}

	if ( gen_bitfield_is_set( gen_u32, self->ModuleFlags, ModuleFlag_Export ))
		gen_strbuilder_append_str( result, gen_txt("export ") );

	if ( self->Attributes || self->Specs )
	{
		if ( self->Attributes )
			gen_strbuilder_append_fmt( result, "%SB ", gen_attributes_to_strbuilder(self->Attributes) );

		if ( self->Specs )
			gen_strbuilder_append_fmt( result, "%SB", gen_specifiers_to_strbuilder(self->Specs) );

		gen_strbuilder_append_fmt( result, "%SB %S", gen_typename_to_strbuilder(self->ValueType), self->Name );

		if ( self->ValueType && self->ValueType->ArrExpr )
		{
			gen_strbuilder_append_fmt( result, "[ %SB ]", gen_code_to_strbuilder(self->ValueType->ArrExpr) );

			gen_Code gen_next_arr_expr = self->ValueType->ArrExpr->Next;
			while ( gen_next_arr_expr )
			{
				gen_strbuilder_append_fmt( result, "[ %SB ]", gen_code_to_strbuilder(gen_next_arr_expr) );
				gen_next_arr_expr = gen_next_arr_expr->Next;
			}
		}

		if ( self->BitfieldSize )
			gen_strbuilder_append_fmt( result, " : %SB", gen_code_to_strbuilder(self->BitfieldSize) );

		if ( self->Value )
		{
			if ( self->VarParenthesizedInit )
				gen_strbuilder_append_fmt( result, "( %SB ", gen_code_to_strbuilder(self->Value) );
			else
				gen_strbuilder_append_fmt( result, " = %SB", gen_code_to_strbuilder(self->Value) );
		}

		if ( self->NextVar )
			gen_strbuilder_append_fmt( result, ", %SB", gen_var_to_strbuilder(self->NextVar) );

		if ( self->VarParenthesizedInit )
			gen_strbuilder_append_str( result, gen_txt(" )"));

		if ( self->InlineCmt )
			gen_strbuilder_append_fmt( result, ";  %S", self->InlineCmt->Content);
		else
			gen_strbuilder_append_str( result, gen_txt(";\n") );

		return;
	}

	if ( self->BitfieldSize )
		gen_strbuilder_append_fmt( result, "%SB %S : %SB", gen_typename_to_strbuilder(self->ValueType), self->Name, gen_code_to_strbuilder(self->BitfieldSize) );

	else if ( self->ValueType && self->ValueType->ArrExpr )
	{
		gen_strbuilder_append_fmt( result, "%SB %S[ %SB ]", gen_typename_to_strbuilder(self->ValueType), self->Name, gen_code_to_strbuilder(self->ValueType->ArrExpr) );

		gen_Code gen_next_arr_expr = self->ValueType->ArrExpr->Next;
		while ( gen_next_arr_expr )
		{
			gen_strbuilder_append_fmt( result, "[ %SB ]", gen_code_to_strbuilder(gen_next_arr_expr) );
			gen_next_arr_expr = gen_next_arr_expr->Next;
		}
	}

	else
		gen_strbuilder_append_fmt( result, "%SB %S", gen_typename_to_strbuilder(self->ValueType), self->Name );

	if ( self->Value )
	{
		if ( self->VarParenthesizedInit )
			gen_strbuilder_append_fmt( result, "( %SB ", gen_code_to_strbuilder(self->Value) );
		else
			gen_strbuilder_append_fmt( result, " = %SB", gen_code_to_strbuilder(self->Value) );
	}

	if ( self->NextVar )
		gen_strbuilder_append_fmt( result, ", %SB", gen_var_to_strbuilder( self->NextVar) );

	if ( self->VarParenthesizedInit )
		gen_strbuilder_append_str( result, gen_txt(" )"));

	gen_strbuilder_append_str( result, gen_txt(";") );

	if ( self->InlineCmt )
		gen_strbuilder_append_fmt( result, "  %S", self->InlineCmt->Content);
	else
		gen_strbuilder_append_str( result, gen_txt("\n"));
}
#pragma endregion AST

#pragma region Interface

gen_internal void gen_parser_init();
gen_internal void gen_parser_deinit();

gen_internal void* gen_Global_Allocator_Proc(
    void*         allocator_data,
    gen_AllocType type,
    gen_ssize     size,
    gen_ssize     alignment,
    void*         old_memory,
    gen_ssize     old_size,
    gen_u64       flags
)
{
	GEN_ASSERT(gen__ctx);
	GEN_ASSERT(gen__ctx->Fallback_AllocatorBuckets);
	gen_Arena* last = gen_array_back(gen__ctx->Fallback_AllocatorBuckets);

	switch (type)
	{
		case EAllocation_ALLOC:
		{
			if ((last->TotalUsed + size) > last->TotalSize)
			{
				gen_Arena bucket = gen_arena_init_from_allocator(gen_heap(), gen__ctx->InitSize_Fallback_Allocator_Bucket_Size);

				if (bucket.PhysicalStart == gen_nullptr)
					GEN_FATAL("Failed to create bucket for Fallback_AllocatorBuckets");

				if (! gen_array_append(gen__ctx->Fallback_AllocatorBuckets, bucket))
					GEN_FATAL("Failed to append bucket to Fallback_AllocatorBuckets");

				last = gen_array_back(gen__ctx->Fallback_AllocatorBuckets);
			}

			return gen_alloc_align(gen_arena_allocator_info(last), size, alignment);
		}
		case EAllocation_FREE:
		{
			// Doesn't recycle.
		}
		break;
		case EAllocation_FREE_ALL:
		{
			// Memory::cleanup instead.
		}
		break;
		case EAllocation_RESIZE:
		{
			if (last->TotalUsed + size > last->TotalSize)
			{
				gen_Arena bucket = gen_arena_init_from_allocator(gen_heap(), gen__ctx->InitSize_Fallback_Allocator_Bucket_Size);

				if (bucket.PhysicalStart == gen_nullptr)
					GEN_FATAL("Failed to create bucket for Fallback_AllocatorBuckets");

				if (! gen_array_append(gen__ctx->Fallback_AllocatorBuckets, bucket))
					GEN_FATAL("Failed to append bucket to Fallback_AllocatorBuckets");

				last = gen_array_back(gen__ctx->Fallback_AllocatorBuckets);
			}

			void* result = gen_alloc_align(last->Backing, size, alignment);

			if (result != gen_nullptr && old_memory != gen_nullptr)
			{
				gen_mem_copy(result, old_memory, old_size);
			}

			return result;
		}
	}

	return gen_nullptr;
}

gen_internal void gen_define_constants()
{
	// We only initalize these if there is no base context.
	if (context_counter > 0)
		return;

	gen_Code_Global          = gen_make_code();
	gen_Code_Global->Name    = gen_cache_str(gen_txt("Global gen_Code"));
	gen_Code_Global->Content = gen_Code_Global->Name;

	gen_Code_Invalid         = gen_make_code();
	gen_code_set_global(gen_Code_Invalid);

	gen_t_empty       = (gen_CodeTypename)gen_make_code();
	gen_t_empty->Type = CT_Typename;
	gen_t_empty->Name = gen_cache_str(gen_txt(""));
	gen_code_set_global(gen_cast(gen_Code, gen_t_empty));

	gen_access_private       = gen_make_code();
	gen_access_private->Type = CT_Access_Private;
	gen_access_private->Name = gen_cache_str(gen_txt("private:\n"));
	gen_code_set_global(gen_cast(gen_Code, gen_access_private));

	gen_access_protected       = gen_make_code();
	gen_access_protected->Type = CT_Access_Protected;
	gen_access_protected->Name = gen_cache_str(gen_txt("protected:\n"));
	gen_code_set_global(gen_access_protected);

	gen_access_public       = gen_make_code();
	gen_access_public->Type = CT_Access_Public;
	gen_access_public->Name = gen_cache_str(gen_txt("public:\n"));
	gen_code_set_global(gen_access_public);

	gen_Str api_export_str = code(GEN_API_Export_Code);
	gen_attrib_api_export  = gen_def_attributes(api_export_str);
	gen_code_set_global(gen_cast(gen_Code, gen_attrib_api_export));

	gen_Str api_import_str = code(GEN_API_Import_Code);
	gen_attrib_api_import  = gen_def_attributes(api_import_str);
	gen_code_set_global(gen_cast(gen_Code, gen_attrib_api_import));

	gen_module_global_fragment          = gen_make_code();
	gen_module_global_fragment->Type    = CT_Untyped;
	gen_module_global_fragment->Name    = gen_cache_str(gen_txt("module;"));
	gen_module_global_fragment->Content = gen_module_global_fragment->Name;
	gen_code_set_global(gen_cast(gen_Code, gen_module_global_fragment));

	gen_module_private_fragment          = gen_make_code();
	gen_module_private_fragment->Type    = CT_Untyped;
	gen_module_private_fragment->Name    = gen_cache_str(gen_txt("module : private;"));
	gen_module_private_fragment->Content = gen_module_private_fragment->Name;
	gen_code_set_global(gen_cast(gen_Code, gen_module_private_fragment));

	gen_fmt_newline       = gen_make_code();
	gen_fmt_newline->Type = CT_NewLine;
	gen_code_set_global((gen_Code)gen_fmt_newline);

	gen_pragma_once          = (gen_CodePragma)gen_make_code();
	gen_pragma_once->Type    = CT_Preprocess_Pragma;
	gen_pragma_once->Name    = gen_cache_str(gen_txt("once"));
	gen_pragma_once->Content = gen_pragma_once->Name;
	gen_code_set_global((gen_Code)gen_pragma_once);

	gen_param_varadic            = (gen_CodeParams)gen_make_code();
	gen_param_varadic->Type      = CT_Parameters;
	gen_param_varadic->Name      = gen_cache_str(gen_txt("..."));
	gen_param_varadic->ValueType = gen_t_empty;
	gen_code_set_global((gen_Code)gen_param_varadic);

	gen_preprocess_else       = (gen_CodePreprocessCond)gen_make_code();
	gen_preprocess_else->Type = CT_Preprocess_Else;
	gen_code_set_global((gen_Code)gen_preprocess_else);

	gen_preprocess_endif       = (gen_CodePreprocessCond)gen_make_code();
	gen_preprocess_endif->Type = CT_Preprocess_EndIf;
	gen_code_set_global((gen_Code)gen_preprocess_endif);

	gen_Str auto_str = gen_txt("auto");
	gen_t_auto       = gen_def_type(auto_str);
	gen_code_set_global(gen_t_auto);
	gen_Str void_str = gen_txt("void");
	gen_t_void       = gen_def_type(void_str);
	gen_code_set_global(gen_t_void);
	gen_Str int_str = gen_txt("int");
	gen_t_int       = gen_def_type(int_str);
	gen_code_set_global(gen_t_int);
	gen_Str bool_str = gen_txt("bool");
	gen_t_bool       = gen_def_type(bool_str);
	gen_code_set_global(gen_t_bool);
	gen_Str gen_char_str = gen_txt("char");
	gen_t_char           = gen_def_type(gen_char_str);
	gen_code_set_global(gen_t_char);
	gen_Str wchar_str = gen_txt("wchar_t");
	gen_t_wchar_t     = gen_def_type(wchar_str);
	gen_code_set_global(gen_t_wchar_t);
	gen_Str gen_class_str = gen_txt("class");
	gen_t_class           = gen_def_type(gen_class_str);
	gen_code_set_global(gen_t_class);
	gen_Str gen_typename_str = gen_txt("typename");
	gen_t_typename           = gen_def_type(gen_typename_str);
	gen_code_set_global(gen_t_typename);

#ifdef GEN_DEFINE_LIBRARY_CODE_CONSTANTS
	gen_t_b32 = gen_def_type(name(gen_b32));
	gen_code_set_global(gen_t_b32);

	gen_Str s8_str = gen_txt("gen_s8");
	gen_t_s8       = gen_def_type(s8_str);
	gen_code_set_global(gen_t_s8);
	gen_Str s16_str = gen_txt("gen_s16");
	gen_t_s16       = gen_def_type(s16_str);
	gen_code_set_global(gen_t_s16);
	gen_Str s32_str = gen_txt("gen_s32");
	gen_t_s32       = gen_def_type(s32_str);
	gen_code_set_global(gen_t_s32);
	gen_Str s64_str = gen_txt("gen_s64");
	gen_t_s64       = gen_def_type(s64_str);
	gen_code_set_global(gen_t_s64);

	gen_Str u8_str = gen_txt("gen_u8");
	gen_t_u8       = gen_def_type(u8_str);
	gen_code_set_global(gen_t_u8);
	gen_Str u16_str = gen_txt("gen_u16");
	gen_t_u16       = gen_def_type(u16_str);
	gen_code_set_global(gen_t_u16);
	gen_Str u32_str = gen_txt("gen_u32");
	gen_t_u32       = gen_def_type(u32_str);
	gen_code_set_global(gen_t_u32);
	gen_Str u64_str = gen_txt("gen_u64");
	gen_t_u64       = gen_def_type(u64_str);
	gen_code_set_global(gen_t_u64);

	gen_Str ssize_str = gen_txt("gen_ssize");
	gen_t_ssize       = gen_def_type(ssize_str);
	gen_code_set_global(gen_t_ssize);
	gen_Str usize_str = gen_txt("gen_usize");
	gen_t_usize       = gen_def_type(usize_str);
	gen_code_set_global(gen_t_usize);

	gen_Str f32_str = gen_txt("gen_f32");
	gen_t_f32       = gen_def_type(f32_str);
	gen_code_set_global(gen_t_f32);
	gen_Str f64_str = gen_txt("gen_f64");
	gen_t_f64       = gen_def_type(f64_str);
	gen_code_set_global(gen_t_f64);
#endif

	gen_spec_const = gen_def_specifier(Spec_Const);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_const));
	gen_spec_consteval = gen_def_specifier(Spec_Consteval);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_consteval));
	gen_spec_constexpr = gen_def_specifier(Spec_Constexpr);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_constexpr));
	gen_spec_constinit = gen_def_specifier(Spec_Constinit);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_constinit));
	gen_spec_extern_linkage = gen_def_specifier(Spec_External_Linkage);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_extern_linkage));
	gen_spec_final = gen_def_specifier(Spec_Final);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_final));
	gen_spec_forceinline = gen_def_specifier(Spec_ForceInline);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_forceinline));
	gen_spec_global = gen_def_specifier(Spec_Global);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_global));
	gen_spec_inline = gen_def_specifier(Spec_Inline);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_inline));
	gen_spec_internal_linkage = gen_def_specifier(Spec_Internal_Linkage);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_internal_linkage));
	gen_spec_local_persist = gen_def_specifier(Spec_Local_Persist);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_local_persist));
	gen_spec_mutable = gen_def_specifier(Spec_Mutable);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_mutable));
	gen_spec_neverinline = gen_def_specifier(Spec_NeverInline);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_neverinline));
	gen_spec_noexcept = gen_def_specifier(Spec_NoExceptions);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_noexcept));
	gen_spec_override = gen_def_specifier(Spec_Override);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_override));
	gen_spec_ptr = gen_def_specifier(Spec_Ptr);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_ptr));
	gen_spec_pure = gen_def_specifier(Spec_Pure);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_pure));
	gen_spec_ref = gen_def_specifier(Spec_Ref);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_ref));
	gen_spec_register = gen_def_specifier(Spec_Register);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_register));
	gen_spec_rvalue = gen_def_specifier(Spec_RValue);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_rvalue));
	gen_spec_static_member = gen_def_specifier(Spec_Static);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_static_member));
	gen_spec_thread_local = gen_def_specifier(Spec_Thread_Local);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_thread_local));
	gen_spec_virtual = gen_def_specifier(Spec_Virtual);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_virtual));
	gen_spec_volatile = gen_def_specifier(Spec_Volatile);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_volatile));

	gen_spec_local_persist = gen_def_specifiers(1, Spec_Local_Persist);
	gen_code_set_global(gen_cast(gen_Code, gen_spec_local_persist));

	if (gen_enum_underlying_macro.Name.Len == 0)
	{
		gen_enum_underlying_macro.Name  = gen_txt("gen_enum_underlying");
		gen_enum_underlying_macro.Type  = MT_Expression;
		gen_enum_underlying_macro.Flags = MF_Functional;
	}
	gen_register_macro(gen_enum_underlying_macro);
}

void gen_init(gen_Context* ctx)
{
	gen_do_once()
	{
		context_counter = 0;
	}
	gen_AllocatorInfo fallback_allocator = { &gen_Global_Allocator_Proc, gen_nullptr };

	gen_b32 gen_using_fallback_allocator = false;
	if (ctx->Allocator_DyanmicContainers.Proc == gen_nullptr)
	{
		ctx->Allocator_DyanmicContainers = fallback_allocator;
		gen_using_fallback_allocator     = true;
	}
	if (ctx->Allocator_Pool.Proc == gen_nullptr)
	{
		ctx->Allocator_Pool          = fallback_allocator;
		gen_using_fallback_allocator = true;
	}
	if (ctx->Allocator_StrCache.Proc == gen_nullptr)
	{
		ctx->Allocator_StrCache      = fallback_allocator;
		gen_using_fallback_allocator = true;
	}
	if (ctx->Allocator_Temp.Proc == gen_nullptr)
	{
		ctx->Allocator_Temp          = fallback_allocator;
		gen_using_fallback_allocator = true;
	}
	// Setup fallback allocator
	if (gen_using_fallback_allocator)
	{
		ctx->Fallback_AllocatorBuckets = gen_array_init_reserve(gen_Arena, gen_heap(), 128);
		if (ctx->Fallback_AllocatorBuckets == gen_nullptr)
			GEN_FATAL("Failed to reserve memory for Fallback_AllocatorBuckets");

		gen_Arena bucket = gen_arena_init_from_allocator(gen_heap(), ctx->InitSize_Fallback_Allocator_Bucket_Size);
		if (bucket.PhysicalStart == gen_nullptr)
			GEN_FATAL("Failed to create first bucket for Fallback_AllocatorBuckets");

		gen_array_append(ctx->Fallback_AllocatorBuckets, bucket);
	}

	if (ctx->Max_CommentLineLength == 0)
	{
		ctx->Max_CommentLineLength = 1024;
	}
	if (ctx->Max_StrCacheLength == 0)
	{
		ctx->Max_StrCacheLength = gen_kilobytes(512);
	}

	if (ctx->InitSize_BuilderBuffer == 0)
	{
		ctx->InitSize_BuilderBuffer = gen_megabytes(2);
	}
	if (ctx->InitSize_CodePoolsArray == 0)
	{
		ctx->InitSize_CodePoolsArray = 16;
	}
	if (ctx->InitSize_StringArenasArray == 0)
	{
		ctx->InitSize_StringArenasArray = 16;
	}
	if (ctx->CodePool_NumBlocks == 0)
	{
		ctx->CodePool_NumBlocks = gen_kilobytes(16);
	}

	if (ctx->InitSize_LexerTokens == 0)
	{
		ctx->InitSize_LexerTokens = gen_kilobytes(64);
	}
	if (ctx->SizePer_StringArena == 0)
	{
		ctx->SizePer_StringArena = gen_megabytes(1);
	}

	if (ctx->InitSize_Fallback_Allocator_Bucket_Size == 0)
	{
		ctx->InitSize_Fallback_Allocator_Bucket_Size = gen_megabytes(8);
	}

	if (ctx->InitSize_StrCacheTable == 0)
	{
		ctx->InitSize_StrCacheTable = gen_kilobytes(8);
	}
	if (ctx->InitSize_MacrosTable == 0)
	{
		ctx->InitSize_MacrosTable = gen_kilobytes(8);
	}

	// Override the current context (user has to put it back if unwanted).
	gen__ctx = ctx;

	// Setup the arrays
	{
		ctx->CodePools = gen_array_init_reserve(gen_Pool, ctx->Allocator_DyanmicContainers, ctx->InitSize_CodePoolsArray);
		if (ctx->CodePools == gen_nullptr)
			GEN_FATAL("gen::gen_init: Failed to initialize the CodePools array");

		ctx->StringArenas = gen_array_init_reserve(gen_Arena, ctx->Allocator_DyanmicContainers, ctx->InitSize_StringArenasArray);
		if (ctx->StringArenas == gen_nullptr)
			GEN_FATAL("gen::gen_init: Failed to initialize the StringArenas array");
	}
	// Setup the code pool and code entries arena.
	{
		gen_Pool gen_code_pool = gen_pool_init(ctx->Allocator_Pool, ctx->CodePool_NumBlocks, sizeof(gen_AST));
		if (gen_code_pool.PhysicalStart == gen_nullptr)
			GEN_FATAL("gen::gen_init: Failed to initialize the code pool");
		gen_array_append(ctx->CodePools, gen_code_pool);

		// TODO(Ed): Eventually the string arenas needs to be phased out for a dedicated string slab allocator
		gen_Arena gen_strbuilder_arena = gen_arena_init_from_allocator(ctx->Allocator_StrCache, ctx->SizePer_StringArena);
		if (gen_strbuilder_arena.PhysicalStart == gen_nullptr)
			GEN_FATAL("gen::gen_init: Failed to initialize the string arena");
		gen_array_append(ctx->StringArenas, gen_strbuilder_arena);
	}
	// Setup the gen_hash tables
	{
		ctx->StrCache = gen_hashtable_init_reserve(gen_StrCached, ctx->Allocator_DyanmicContainers, ctx->InitSize_StrCacheTable);
		if (ctx->StrCache.Entries == gen_nullptr)
			GEN_FATAL("gen::gen_init: Failed to initialize the StringCache");

		ctx->Macros = gen_hashtable_init_reserve(gen_Macro, ctx->Allocator_DyanmicContainers, ctx->InitSize_MacrosTable);
		if (ctx->Macros.Hashes == gen_nullptr || ctx->Macros.Entries == gen_nullptr)
		{
			GEN_FATAL("gen::gen_init: Failed to initialize the PreprocessMacros table");
		}
	}

	gen_define_constants();
	gen_parser_init();

	++context_counter;
}

void gen_deinit(gen_Context* ctx)
{
	GEN_ASSERT(context_counter);
	GEN_ASSERT_MSG(context_counter > 0, "Attempted to gen_deinit a context that for some reason wan't accounted for!");
	gen_usize index = 0;
	gen_usize left  = gen_array_num(ctx->CodePools);
	do
	{
		gen_Pool* gen_code_pool = &ctx->CodePools[index];
		gen_pool_free(gen_code_pool);
		index++;
	} while (left--, left);

	index = 0;
	left  = gen_array_num(ctx->StringArenas);
	do
	{
		gen_Arena* gen_strbuilder_arena = &ctx->StringArenas[index];
		gen_arena_free(gen_strbuilder_arena);
		index++;
	} while (left--, left);

	gen_hashtable_destroy(ctx->StrCache);

	gen_array_free(ctx->CodePools);
	gen_array_free(ctx->StringArenas);

	gen_hashtable_destroy(ctx->Macros);

	left = gen_array_num(ctx->Fallback_AllocatorBuckets);
	if (left)
	{
		index = 0;
		do
		{
			gen_Arena* bucket = &ctx->Fallback_AllocatorBuckets[index];
			gen_arena_free(bucket);
			index++;
		} while (left--, left);
		gen_array_free(ctx->Fallback_AllocatorBuckets);
	}
	gen_parser_deinit();

	if (gen__ctx == ctx)
		gen__ctx = gen_nullptr;
	--context_counter;

	gen_Context wipe = {};
	*ctx             = wipe;
}

gen_Context* gen_get_context()
{
	return gen__ctx;
}

void gen_reset(gen_Context* ctx)
{
	gen_s32 index = 0;
	gen_s32 left  = gen_array_num(ctx->CodePools);
	do
	{
		gen_Pool* gen_code_pool = &ctx->CodePools[index];
		gen_pool_clear(gen_code_pool);
		index++;
	} while (left--, left);

	index = 0;
	left  = gen_array_num(ctx->StringArenas);
	do
	{
		gen_Arena* gen_strbuilder_arena = &ctx->StringArenas[index];
		gen_strbuilder_arena->TotalUsed = 0;
		;
		index++;
	} while (left--, left);

	gen_hashtable_clear(ctx->StrCache);
	gen_hashtable_clear(ctx->Macros);
	gen_define_constants();
}

void set_context(gen_Context* new_ctx)
{
	GEN_ASSERT(new_ctx);
	gen__ctx = new_ctx;
}

gen_AllocatorInfo get_cached_str_allocator(gen_s32 gen_str_length)
{
	gen_Arena* last     = gen_array_back(gen__ctx->StringArenas);
	gen_usize  size_req = gen_str_length + sizeof(gen_StrBuilderHeader) + sizeof(char*);
	if (last->TotalUsed + gen_scast(gen_ssize, size_req) > last->TotalSize)
	{
		gen_Arena new_arena = gen_arena_init_from_allocator(gen__ctx->Allocator_StrCache, gen__ctx->SizePer_StringArena);
		if (! gen_array_append(gen__ctx->StringArenas, new_arena))
			GEN_FATAL("gen::get_cached_str_allocator: Failed to allocate a new string arena");

		last = gen_array_back(gen__ctx->StringArenas);
	}
	return gen_arena_allocator_info(last);
}

// Will either make or retrive a code string.
gen_StrCached gen_cache_str(gen_Str str)
{
	if (str.Len > gen__ctx->Max_StrCacheLength)
	{
		// Do not cache the string, just shove into the arena and and return it.
		gen_Str result = gen_strbuilder_to_str(gen_strbuilder_make_str(get_cached_str_allocator(str.Len), str));
		return result;
	}
	gen_u64 key = gen_crc32(str.Ptr, str.Len);
	{
		gen_StrCached* result = gen_hashtable_get(gen__ctx->StrCache, key);
		if (result)
			return *result;
	}
	gen_Str result = gen_strbuilder_to_str(gen_strbuilder_make_str(get_cached_str_allocator(str.Len), str));
	gen_hashtable_set(gen__ctx->StrCache, key, result);
	return result;
}

// Used internally to retireve a gen_Code object form the CodePool.
gen_Code gen_make_code()
{
	gen_Pool* allocator = gen_array_back(gen__ctx->CodePools);
	if (allocator->FreeList == gen_nullptr)
	{
		gen_Pool gen_code_pool = gen_pool_init(gen__ctx->Allocator_Pool, gen__ctx->CodePool_NumBlocks, sizeof(gen_AST));

		if (gen_code_pool.PhysicalStart == gen_nullptr)
			GEN_FATAL("gen::gen_make_code: Failed to allocate a new code pool - CodePool allcoator returned gen_nullptr.");

		if (! gen_array_append(gen__ctx->CodePools, gen_code_pool))
			GEN_FATAL("gen::gen_make_code: Failed to allocate a new code pool - CodePools failed to append new pool.");

		allocator = gen_array_back(gen__ctx->CodePools);
	}
	gen_Code result = { gen_rcast(gen_AST*, gen_alloc(gen_pool_allocator_info(allocator), sizeof(gen_AST))) };
	gen_mem_set(gen_rcast(void*, gen_cast(gen_AST*, result)), 0, sizeof(gen_AST));
	return result;
}

gen_Macro* lookup_macro(gen_Str name)
{
	gen_u32 key = gen_crc32(name.Ptr, name.Len);
	return gen_hashtable_get(gen__ctx->Macros, key);
}

void gen_register_macro(gen_Macro macro)
{
	GEN_ASSERT_NOT_NULL(macro.Name.Ptr);
	GEN_ASSERT(macro.Name.Len > 0);
	gen_u32 key = gen_crc32(macro.Name.Ptr, macro.Name.Len);
	macro.Name  = gen_cache_str(macro.Name);
	gen_hashtable_set(gen__ctx->Macros, key, macro);
}

void gen_register_macros(gen_s32 num, ...)
{
	GEN_ASSERT(num > 0);
	va_list va;
	va_start(va, num);
	do
	{
		gen_Macro macro = va_arg(va, gen_Macro);
		GEN_ASSERT_NOT_NULL(macro.Name.Ptr);
		GEN_ASSERT(macro.Name.Len > 0);
		macro.Name  = gen_cache_str(macro.Name);

		gen_u32 key = gen_crc32(macro.Name.Ptr, macro.Name.Len);
		gen_hashtable_set(gen__ctx->Macros, key, macro);
	} while (num--, num > 0);
	va_end(va);
}

void gen_register_macros_arr(gen_s32 num, gen_Macro* macros)
{
	GEN_ASSERT(num > 0);
	do
	{
		gen_Macro macro = *macros;
		GEN_ASSERT_NOT_NULL(macro.Name.Ptr);
		GEN_ASSERT(macro.Name.Len > 0);
		macro.Name  = gen_cache_str(macro.Name);

		gen_u32 key = gen_crc32(macro.Name.Ptr, macro.Name.Len);
		gen_hashtable_set(gen__ctx->Macros, key, macro);
		++macros;
	} while (num--, num > 0);
}

#pragma region Upfront

enum OpValidateResult gen_enum_underlying(gen_u32)
{
	OpValResult_Fail,
	OpValResult_Global,
	OpValResult_Member
};

typedef gen_u32 OpValidateResult;

gen_internal gen_neverinline OpValidateResult
    gen_operator__validate(gen_Operator op, gen_CodeParams gen_params_code, gen_CodeTypename ret_type, gen_CodeSpecifiers specifier)
{
	if (op == Op_Invalid)
	{
		gen_log_failure("gen::gen_def_operator: op cannot be invalid");
		return OpValResult_Fail;
	}

#pragma region Helper Macros
#define check_params()                                                                                                                            \
	if (! gen_params_code)                                                                                                                        \
	{                                                                                                                                             \
		gen_log_failure("gen::gen_def_operator: params is null and operator %S requires it", gen_operator_to_str(op));                            \
		return OpValResult_Fail;                                                                                                                  \
	}                                                                                                                                             \
	if (gen_params_code->Type != CT_Parameters)                                                                                                   \
	{                                                                                                                                             \
		gen_log_failure("gen::gen_def_operator: params is not of Parameters type - %S", gen_code_debug_str(gen_cast(gen_Code, gen_params_code))); \
		return OpValResult_Fail;                                                                                                                  \
	}

#define check_param_eq_ret()                                                                                                     \
	if (! is_member_symbol && ! gen_code_is_equal(gen_cast(gen_Code, gen_params_code->ValueType), gen_cast(gen_Code, ret_type))) \
	{                                                                                                                            \
		gen_log_failure(                                                                                                         \
		    "gen::gen_def_operator: operator %S requires first parameter to equal return type\n"                                 \
		    "param types: %S\n"                                                                                                  \
		    "return type: %S",                                                                                                   \
		    gen_operator_to_str(op),                                                                                             \
		    gen_code_debug_str(gen_cast(gen_Code, gen_params_code)),                                                             \
		    gen_code_debug_str(gen_cast(gen_Code, ret_type))                                                                     \
		);                                                                                                                       \
		return OpValResult_Fail;                                                                                                 \
	}
#pragma endregion Helper Macros

	if (! ret_type)
	{
		gen_log_failure("gen::gen_def_operator: ret_type is null but is required by operator %S", gen_operator_to_str(op));
	}

	if (ret_type->Type != CT_Typename)
	{
		gen_log_failure(
		    "gen::gen_def_operator: operator %S - ret_type is not of typename type - %S",
		    gen_operator_to_str(op),
		    gen_code_debug_str(gen_cast(gen_Code, ret_type))
		);
		return OpValResult_Fail;
	}

	bool is_member_symbol = false;

	switch (op)
	{
#define specs(...) gen_num_args(__VA_ARGS__), __VA_ARGS__
		case Op_Assign:
			check_params();

			if (gen_params_code->NumEntries > 1)
			{
				gen_log_failure(
				    "gen::gen_def_operator: "
				    "operator %S does not support non-member definition (more than one parameter provided) - %S",
				    gen_operator_to_str(op),
				    gen_code_debug_str(gen_cast(gen_Code, gen_params_code))
				);
				return OpValResult_Fail;
			}

			is_member_symbol = true;
			break;

		case Op_Assign_Add:
		case Op_Assign_Subtract:
		case Op_Assign_Multiply:
		case Op_Assign_Divide:
		case Op_Assign_Modulo:
		case Op_Assign_BAnd:
		case Op_Assign_BOr:
		case Op_Assign_BXOr:
		case Op_Assign_LShift:
		case Op_Assign_RShift:
			check_params();

			if (gen_params_code->NumEntries == 1)
				is_member_symbol = true;

			else
				check_param_eq_ret();

			if (gen_params_code->NumEntries > 2)
			{
				gen_log_failure(
				    "gen::gen_def_operator: operator %S may not be defined with more than two parametes - param count; %d\n%S",
				    gen_operator_to_str(op),
				    gen_params_code->NumEntries,
				    gen_code_debug_str(gen_cast(gen_Code, gen_params_code))
				);
				return OpValResult_Fail;
			}
			break;

		case Op_Increment:
		case Op_Decrement:
			// If its not set, it just means its a prefix member op.
			if (gen_params_code)
			{
				if (gen_params_code->Type != CT_Parameters)
				{
					gen_log_failure(
					    "gen::gen_def_operator: operator %S params code provided is not of Parameters type - %S",
					    gen_operator_to_str(op),
					    gen_code_debug_str(gen_cast(gen_Code, gen_params_code))
					);
					return OpValResult_Fail;
				}

				switch (gen_params_code->NumEntries)
				{
					case 1:
						if (gen_code_is_equal((gen_Code)gen_params_code->ValueType, (gen_Code)gen_t_int))
							is_member_symbol = true;

						else
							check_param_eq_ret();
						break;

					case 2:
						check_param_eq_ret();

						if (! gen_code_is_equal((gen_Code)gen_params_get(gen_params_code, 1), (gen_Code)gen_t_int))
						{
							gen_log_failure(
							    "gen::gen_def_operator: "
							    "operator %S requires second parameter of non-member definition to be int for post-decrement",
							    gen_operator_to_str(op)
							);
							return OpValResult_Fail;
						}
						break;

					default:
						gen_log_failure(
						    "gen::gen_def_operator: operator %S recieved unexpected number of parameters recived %d instead of 0-2",
						    gen_operator_to_str(op),
						    gen_params_code->NumEntries
						);
						return OpValResult_Fail;
				}
			}
			break;

		case Op_Unary_Plus:
		case Op_Unary_Minus:
			if (! gen_params_code)
				is_member_symbol = true;

			else
			{
				if (gen_params_code->Type != CT_Parameters)
				{
					gen_log_failure("gen::gen_def_operator: params is not of Parameters type - %S", gen_code_debug_str((gen_Code)gen_params_code));
					return OpValResult_Fail;
				}

				if (gen_code_is_equal((gen_Code)gen_params_code->ValueType, (gen_Code)ret_type))
				{
					gen_log_failure(
					    "gen::gen_def_operator: "
					    "operator %S is non-member symbol yet first paramter does not equal return type\n"
					    "param type: %S\n"
					    "return type: %S\n",
					    gen_code_debug_str((gen_Code)gen_params_code),
					    gen_code_debug_str((gen_Code)ret_type)
					);
					return OpValResult_Fail;
				}

				if (gen_params_code->NumEntries > 1)
				{
					gen_log_failure(
					    "gen::gen_def_operator: operator %S may not have more than one parameter - param count: %d",
					    gen_operator_to_str(op),
					    gen_params_code->NumEntries
					);
					return OpValResult_Fail;
				}
			}
			break;

		case Op_BNot:
		{
			// Some compilers let you do this...
#if 0
			if ( ! ret_type.is_equal( gen_t_bool) )
			{
				gen_log_failure( "gen::gen_def_operator: operator %S return type is not a boolean - %S", gen_operator_to_str(op) gen_code_debug_str(gen_params_code) );
				return OpValidateResult::Fail;
			}
#endif

			if (! gen_params_code)
				is_member_symbol = true;

			else
			{
				if (gen_params_code->Type != CT_Parameters)
				{
					gen_log_failure(
					    "gen::gen_def_operator: operator %S - params is not of Parameters type - %S",
					    gen_operator_to_str(op),
					    gen_code_debug_str((gen_Code)gen_params_code)
					);
					return OpValResult_Fail;
				}

				if (gen_params_code->NumEntries > 1)
				{
					gen_log_failure(
					    "gen::gen_def_operator: operator %S may not have more than one parameter - param count: %d",
					    gen_operator_to_str(op),
					    gen_params_code->NumEntries
					);
					return OpValResult_Fail;
				}
			}
			break;
		}

		case Op_Add:
		case Op_Subtract:
		case Op_Multiply:
		case Op_Divide:
		case Op_Modulo:
		case Op_BAnd:
		case Op_BOr:
		case Op_BXOr:
		case Op_LShift:
		case Op_RShift:
			check_params();

			switch (gen_params_code->NumEntries)
			{
				case 1:
					is_member_symbol = true;
					break;

				case 2:
					// This is allowed for arithemtic operators
					// if ( ! gen_code_is_equal((gen_Code)gen_params_code->ValueType, (gen_Code)ret_type ) )
					// {
					// 	gen_log_failure("gen::gen_def_operator: "
					// 		"operator %S is non-member symbol yet first paramter does not equal return type\n"
					// 		"param type: %S\n"
					// 		"return type: %S\n"
					// 		, gen_code_debug_str((gen_Code)gen_params_code)
					// 		, gen_code_debug_str((gen_Code)ret_type)
					// 	);
					// 	return OpValResult_Fail;
					// }
					break;

				default:
					gen_log_failure(
					    "gen::gen_def_operator: operator %S recieved unexpected number of paramters recived %d instead of 0-2",
					    gen_operator_to_str(op),
					    gen_params_code->NumEntries
					);
					return OpValResult_Fail;
			}
			break;

		case Op_UnaryNot:
			if (! gen_params_code)
				is_member_symbol = true;

			else
			{
				if (gen_params_code->Type != CT_Parameters)
				{
					gen_log_failure(
					    "gen::gen_def_operator: operator %S - params is not of Parameters type - %S",
					    gen_operator_to_str(op),
					    gen_code_debug_str((gen_Code)gen_params_code)
					);
					return OpValResult_Fail;
				}

				if (gen_params_code->NumEntries != 1)
				{
					gen_log_failure(
					    "gen::gen_def_operator: operator %S recieved unexpected number of paramters recived %d instead of 0-1",
					    gen_operator_to_str(op),
					    gen_params_code->NumEntries
					);
					return OpValResult_Fail;
				}
			}

			if (! gen_code_is_equal((gen_Code)ret_type, (gen_Code)gen_t_bool))
			{
				gen_log_failure(
				    "gen::gen_def_operator: operator %S return type must be of type bool - %S",
				    gen_operator_to_str(op),
				    gen_code_debug_str((gen_Code)ret_type)
				);
				return OpValResult_Fail;
			}
			break;

		case Op_LAnd:
		case Op_LOr:
		case Op_LEqual:
		case Op_LNot:
		case Op_Lesser:
		case Op_Greater:
		case Op_LesserEqual:
		case Op_GreaterEqual:
			check_params();

			switch (gen_params_code->NumEntries)
			{
				case 1:
					is_member_symbol = true;
					break;

				case 2:
					break;

				default:
					gen_log_failure(
					    "gen::gen_def_operator: operator %S recieved unexpected number of paramters recived %d instead of 1-2",
					    gen_operator_to_str(op),
					    gen_params_code->NumEntries
					);
					return OpValResult_Fail;
			}
			break;

		case Op_Indirection:
		case Op_AddressOf:
		case Op_MemberOfPointer:
			if (gen_params_code && gen_params_code->NumEntries > 1)
			{
				gen_log_failure(
				    "gen::gen_def_operator: operator %S recieved unexpected number of paramters recived %d instead of 0-1",
				    gen_operator_to_str(op),
				    gen_params_code->NumEntries
				);
				return OpValResult_Fail;
			}
			else
			{
				is_member_symbol = true;
			}
			break;

		case Op_PtrToMemOfPtr:
			if (gen_params_code)
			{
				gen_log_failure(
				    "gen::gen_def_operator: operator %S expects no paramters - %S",
				    gen_operator_to_str(op),
				    gen_code_debug_str((gen_Code)gen_params_code)
				);
				return OpValResult_Fail;
			}
			break;

		case Op_Subscript:
		case Op_FunctionCall:
		case Op_Comma:
			check_params();
			break;

		case Op_New:
		case Op_Delete:
			// This library doesn't support validating new and delete yet.
			break;
#undef specs
	}

	return is_member_symbol ? OpValResult_Member : OpValResult_Global;
#undef check_params
#undef check_ret_type
#undef check_param_eq_ret
}

gen_forceinline bool name__check(char const* context, gen_Str name)
{
	if (name.Len <= 0)
	{
		gen_log_failure("gen::%s: Invalid name length provided - %d", name.Len);
		return false;
	}
	if (name.Ptr == gen_nullptr)
	{
		gen_log_failure("gen::%s: name is null");
		return false;
	}
	return true;
}

#define name_check(context, name) name__check(#context, name)

gen_forceinline bool null__check(char const* context, char const* gen_code_id, gen_Code code)
{
	if (code == gen_nullptr)
	{
		gen_log_failure("gen::%s: %s provided is null", context, gen_code_id);
		return false;
	}
	return true;
}

#define null_check(context, code) null__check(#context, #code, gen_cast(gen_Code, code))

/*
The implementation of the upfront gen_constructor_s involves doing three things:
* Validate the arguments given to construct the intended type of gen_AST is valid.
* Construct said gen_AST type.
* Lock the gen_AST (set to readonly) and return the valid object.

If any of the validation fails, it triggers a call to gen_log_failure with as much info the give the user so that they can hopefully
identify the issue without having to debug too much (at least they can debug though...)

The largest of the functions is related to operator overload definitions.
The library validates a good protion of their form and thus the argument processing for is quite a gen_bit.
*/
gen_CodeAttributes gen_def_attributes(gen_Str content)
{
	if (content.Len <= 0 || content.Ptr == gen_nullptr)
	{
		gen_log_failure("gen::gen_def_attributes: Invalid attributes provided");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_Code result = gen_make_code();
	result->Type    = CT_PlatformAttributes;
	result->Name    = gen_cache_str(content);
	result->Content = result->Name;
	return (gen_CodeAttributes)result;
}

gen_CodeComment gen_def_comment(gen_Str content)
{
	if (content.Len <= 0 || content.Ptr == gen_nullptr)
	{
		gen_log_failure("gen::gen_def_comment: Invalid comment provided:");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_StrBuilder cmt_formatted = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, gen_kilobytes(1));
	char const*    end           = content.Ptr + content.Len;
	char const*    scanner       = content.Ptr;
	gen_s32        curr          = 0;
	do
	{
		char const* next   = scanner;
		gen_s32     length = 0;
		while (next != end && scanner[length] != '\n')
		{
			next = scanner + length;
			length++;
		}
		length++;

		gen_strbuilder_append_fmt(&cmt_formatted, "//%.*s", length, scanner);
		scanner += length;
	} while (scanner <= end);

	if (*gen_strbuilder_back(cmt_formatted) != '\n')
		gen_strbuilder_append_str(&cmt_formatted, gen_txt("\n"));

	gen_Str name    = gen_strbuilder_to_str(cmt_formatted);

	gen_Code result = gen_make_code();
	result->Type    = CT_Comment;
	result->Name    = gen_cache_str(name);
	result->Content = result->Name;

	gen_strbuilder_free(&cmt_formatted);

	return (gen_CodeComment)result;
}

gen_CodeConstructor gen_def__constructor(gen_Opts_def_constructor* opt)
{
	gen_Opts_def_constructor p = get_optional(opt);

	if (p.params && p.params->Type != CT_Parameters)
	{
		gen_log_failure("gen::gen_def_constructor: params must be of Parameters type - %s", gen_code_debug_str((gen_Code)p.params));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeConstructor result = (gen_CodeConstructor)gen_make_code();
	if (p.params)
	{
		result->Params = p.params;
	}
	if (p.initializer_list)
	{
		result->InitializerList = p.initializer_list;
	}
	if (p.body)
	{
		switch (p.body->Type)
		{
			case CT_Function_Body:
			case CT_Untyped:
				break;

			default:
				gen_log_failure("gen::gen_def_constructor: body must be either of Function_Body or Untyped type - %s", gen_code_debug_str(p.body));
				return gen_InvalidCode;
		}

		result->Type = CT_Constructor;
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Constructor_Fwd;
	}
	return result;
}

gen_CodeClass gen_def__class(gen_Str name, gen_Opts_def_struct* opt)
{
	gen_Opts_def_struct p = get_optional(opt);

	if (! name_check(gen_def_class, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_class: attributes was not a 'PlatformAttributes' type: %s", gen_code_debug_str(p.attributes));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.parent && (p.parent->Type != CT_Class && p.parent->Type != CT_Struct && p.parent->Type != CT_Typename && p.parent->Type != CT_Untyped))
	{
		gen_log_failure("gen::gen_def_class: parent provided is not type 'Class', 'Struct', 'Typeanme', or 'Untyped': %s", gen_code_debug_str(p.parent));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeClass result = (gen_CodeClass)gen_make_code();
	result->Name         = gen_cache_str(name);
	result->ModuleFlags  = p.mflags;
	result->Attributes   = p.attributes;
	result->Specs        = p.specifiers;
	result->ParentAccess = p.parent_access;
	result->ParentType   = p.parent;
	if (p.body)
	{
		switch (p.body->Type)
		{
			case CT_Class_Body:
			case CT_Untyped:
				break;

			default:
				gen_log_failure("gen::gen_def_class: body must be either of Class_Body or Untyped type - %s", gen_code_debug_str(p.body));
				return gen_InvalidCode;
		}

		result->Type         = CT_Class;
		result->Body         = p.body;
		result->Body->Parent = gen_cast(gen_Code, result);
	}
	else
	{
		result->Type = CT_Class_Fwd;
	}
	for (gen_s32 idx = 0; idx < p.num_interfaces; idx++)
	{
		gen_class_add_interface(result, p.interfaces[idx]);
	}
	return result;
}

gen_CodeDefine gen_def__define(gen_Str name, gen_MacroType type, gen_Opts_def_define* opt)
{
	gen_Opts_def_define p = get_optional(opt);

	if (! name_check(gen_def_define, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeDefine result = (gen_CodeDefine)gen_make_code();
	result->Type          = CT_Preprocess_Define;
	result->Name          = gen_cache_str(name);
	result->Params        = p.params;
	if (p.content.Len <= 0 || p.content.Ptr == gen_nullptr)
		result->Body = gen_untyped_str(gen_txt("\n"));
	else
		result->Body = gen_untyped_str(gen_strbuilder_to_str(gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "%S\n", p.content)));

	gen_b32 register_define = ! p.dont_register_to_preprocess_macros;
	if (register_define)
	{
		gen_Macro gen_macro_entry = { result->Name, type, p.flags };
		gen_register_macro(gen_macro_entry);
	}
	return result;
}

gen_CodeDestructor gen_def__destructor(gen_Opts_def_destructor* opt)
{
	gen_Opts_def_destructor p = get_optional(opt);

	if (p.specifiers && p.specifiers->Type != CT_Specifiers)
	{
		gen_log_failure("gen::gen_def_destructor: specifiers was not a 'Specifiers' type: %s", gen_code_debug_str(p.specifiers));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeDestructor result = (gen_CodeDestructor)gen_make_code();
	result->Specs             = p.specifiers;
	if (p.body)
	{
		switch (p.body->Type)
		{
			case CT_Function_Body:
			case CT_Untyped:
				break;

			default:
				gen_log_failure("gen::gen_def_destructor: body must be either of Function_Body or Untyped type - %s", gen_code_debug_str(p.body));
				return gen_InvalidCode;
		}

		result->Type = CT_Destructor;
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Destructor_Fwd;
	}
	return result;
}

gen_CodeEnum gen_def__enum(gen_Str name, gen_Opts_def_enum* opt)
{
	gen_Opts_def_enum p = get_optional(opt);

	if (! name_check(gen_def_enum, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.type && p.type->Type != CT_Typename)
	{
		gen_log_failure("gen::gen_def_enum: enum underlying type provided was not of type Typename: %s", gen_code_debug_str(p.type));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_enum: attributes was not a 'PlatformAttributes' type: %s", gen_code_debug_str(p.attributes));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeEnum result = (gen_CodeEnum)gen_make_code();
	result->Name        = gen_cache_str(name);
	result->ModuleFlags = p.mflags;
	if (p.body)
	{
		switch (p.body->Type)
		{
			case CT_Enum_Body:
			case CT_Untyped:
				break;

			default:
				gen_log_failure("gen::gen_def_enum: body must be of Enum_Body or Untyped type %s", gen_code_debug_str(p.body));
				return gen_InvalidCode;
		}

		result->Type = p.specifier == EnumDecl_Class ? CT_Enum_Class : CT_Enum;

		result->Body = p.body;
	}
	else
	{
		result->Type = p.specifier == EnumDecl_Class ? CT_Enum_Class_Fwd : CT_Enum_Fwd;
	}
	result->Attributes = p.attributes;

	if (p.type)
	{
		result->UnderlyingType = p.type;
	}
	else if (p.type_macro)
	{
		result->UnderlyingTypeMacro = p.type_macro;
	}
	else if (result->Type != CT_Enum_Class_Fwd && result->Type != CT_Enum_Fwd)
	{
		gen_log_failure("gen::gen_def_enum: enum forward declaration must have an underlying type");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	return result;
}

gen_CodeExec gen_def_execution(gen_Str content)
{
	if (content.Len <= 0 || content.Ptr == gen_nullptr)
	{
		gen_log_failure("gen::gen_def_execution: Invalid execution provided");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeExec result = (gen_CodeExec)gen_make_code();
	result->Type        = CT_Execution;
	result->Content     = gen_cache_str(content);
	return result;
}

gen_CodeExtern gen_def_extern_link(gen_Str name, gen_CodeBody body)
{
	if (! name_check(gen_def_extern_link, name) || ! null_check(gen_def_extern_link, body))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (body->Type != CT_Extern_Linkage_Body && body->Type != CT_Untyped)
	{
		gen_log_failure("gen::gen_def_extern_linkage: body is not of gen_extern_linkage or untyped type %s", gen_code_debug_str(body));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeExtern result = (gen_CodeExtern)gen_make_code();
	result->Type          = CT_Extern_Linkage;
	result->Name          = gen_cache_str(name);
	result->Body          = body;
	return result;
}

gen_CodeFriend gen_def_friend(gen_Code declaration)
{
	if (! null_check(gen_def_friend, declaration))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	switch (declaration->Type)
	{
		case CT_Class_Fwd:
		case CT_Function_Fwd:
		case CT_Operator_Fwd:
		case CT_Struct_Fwd:
		case CT_Class:
		case CT_Function:
		case CT_Operator:
		case CT_Struct:
			break;

		default:
			gen_log_failure("gen::gen_def_friend: requires declartion to have class, function, operator, or struct - %s", gen_code_debug_str(declaration));
			return gen_InvalidCode;
	}
	gen_CodeFriend result = (gen_CodeFriend)gen_make_code();
	result->Type          = CT_Friend;
	result->Declaration   = declaration;
	return result;
}

gen_CodeFn gen_def__function(gen_Str name, gen_Opts_def_function* opt)
{
	gen_Opts_def_function p = get_optional(opt);

	if (! name_check(gen_def_function, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.params && p.params->Type != CT_Parameters)
	{
		gen_log_failure("gen::gen_def_function: params was not a `Parameters` type: %s", gen_code_debug_str(p.params));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.ret_type && p.ret_type->Type != CT_Typename)
	{
		gen_log_failure("gen::gen_def_function: ret_type was not a Typename: %s", gen_code_debug_str(p.ret_type));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.specs && p.specs->Type != CT_Specifiers)
	{
		gen_log_failure("gen::gen_def_function: specifiers was not a `Specifiers` type: %s", gen_code_debug_str(p.specs));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.attrs && p.attrs->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_function: attributes was not a `PlatformAttributes` type: %s", gen_code_debug_str(p.attrs));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeFn result   = (gen_CodeFn)gen_make_code();
	result->Name        = gen_cache_str(name);
	result->ModuleFlags = p.mflags;
	if (p.body)
	{
		switch (p.body->Type)
		{
			case CT_Function_Body:
			case CT_Execution:
			case CT_Untyped:
				break;

			default:
			{
				gen_log_failure("gen::gen_def_function: body must be either of Function_Body, Execution, or Untyped type. %s", gen_code_debug_str(p.body));
				return gen_InvalidCode;
			}
		}
		result->Type = CT_Function;
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Function_Fwd;
	}
	result->Attributes = p.attrs;
	result->Specs      = p.specs;
	result->Params     = p.params;
	result->ReturnType = p.ret_type ? p.ret_type : gen_t_void;
	return result;
}

gen_CodeInclude gen_def__include(gen_Str path, gen_Opts_def_include* opt)
{
	gen_Opts_def_include p = get_optional(opt);

	if (path.Len <= 0 || path.Ptr == gen_nullptr)
	{
		gen_log_failure("gen::gen_def_include: Invalid path provided - %d");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_StrBuilder content = p.foreign ? gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "<%.*s>", path.Len, path.Ptr)
	                                   : gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "\"%.*s\"", path.Len, path.Ptr);

	gen_CodeInclude result = (gen_CodeInclude)gen_make_code();
	result->Type           = CT_Preprocess_Include;
	result->Name           = gen_cache_str(gen_strbuilder_to_str(content));
	result->Content        = result->Name;
	return result;
}

gen_CodeModule gen_def__module(gen_Str name, gen_Opts_def_module* opt)
{
	gen_Opts_def_module p = get_optional(opt);

	if (! name_check(gen_def_module, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeModule result = (gen_CodeModule)gen_make_code();
	result->Type          = CT_Module;
	result->Name          = gen_cache_str(name);
	result->ModuleFlags   = p.mflags;
	return result;
}

gen_CodeNS gen_def__namespace(gen_Str name, gen_CodeBody body, gen_Opts_def_namespace* opt)
{
	gen_Opts_def_namespace p = get_optional(opt);

	if (! name_check(gen_def_namespace, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (! null_check(gen_def_namespace, body))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (body && body->Type != CT_Namespace_Body && body->Type != CT_Untyped)
	{
		gen_log_failure("gen::gen_def_namespace: body is not of namespace or untyped type %s", gen_code_debug_str(body));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeNS result   = (gen_CodeNS)gen_make_code();
	result->Type        = CT_Namespace;
	result->Name        = gen_cache_str(name);
	result->ModuleFlags = p.mflags;
	result->Body        = body;
	return result;
}

gen_CodeOperator gen_def__operator(gen_Operator op, gen_Str nspace, gen_Opts_def_operator* opt)
{
	gen_Opts_def_operator p = get_optional(opt);

	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_operator: PlatformAttributes was provided but its not of attributes type: %s", gen_code_debug_str(p.attributes));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.specifiers && p.specifiers->Type != CT_Specifiers)
	{
		gen_log_failure("gen::gen_def_operator: Specifiers was provided but its not of specifiers type: %s", gen_code_debug_str(p.specifiers));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	OpValidateResult check_result = gen_operator__validate(op, p.params, p.ret_type, p.specifiers);
	if (check_result == OpValResult_Fail)
	{
		return gen_InvalidCode;
	}

	char const* name = gen_nullptr;

	gen_Str op_str   = gen_operator_to_str(op);
	if (nspace.Len > 0)
		name = gen_c_str_fmt_buf("%.*soperator %.*s", nspace.Len, nspace.Ptr, op_str.Len, op_str.Ptr);
	else
		name = gen_c_str_fmt_buf("operator %.*s", op_str.Len, op_str.Ptr);

	gen_Str name_resolved   = { name, gen_c_str_len(name) };

	gen_CodeOperator result = (gen_CodeOperator)gen_make_code();
	result->Name            = gen_cache_str(name_resolved);
	result->ModuleFlags     = p.mflags;
	result->Op              = op;
	if (p.body)
	{
		switch (p.body->Type)
		{
			case CT_Function_Body:
			case CT_Execution:
			case CT_Untyped:
				break;

			default:
			{
				gen_log_failure("gen::gen_def_operator: body must be either of Function_Body, Execution, or Untyped type. %s", gen_code_debug_str(p.body));
				GEN_DEBUG_TRAP();
				return gen_InvalidCode;
			}
		}

		result->Type = check_result == OpValResult_Global ? CT_Operator : CT_Operator_Member;

		result->Body = p.body;
	}
	else
	{
		result->Type = check_result == OpValResult_Global ? CT_Operator_Fwd : CT_Operator_Member_Fwd;
	}
	result->Attributes = p.attributes;
	result->Specs      = p.specifiers;
	result->ReturnType = p.ret_type;
	result->Params     = p.params;
	return result;
}

gen_CodeOpCast gen_def__operator_cast(gen_CodeTypename type, gen_Opts_def_operator_cast* opt)
{
	gen_Opts_def_operator_cast p = get_optional(opt);

	if (! null_check(gen_def_operator_cast, type))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (type->Type != CT_Typename)
	{
		gen_log_failure("gen::gen_def_operator_cast: type is not a typename - %s", gen_code_debug_str(type));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeOpCast result = (gen_CodeOpCast)gen_make_code();
	if (p.body)
	{
		result->Type = CT_Operator_Cast;

		if (p.body->Type != CT_Function_Body && p.body->Type != CT_Execution)
		{
			gen_log_failure("gen::gen_def_operator_cast: body is not of function body or execution type - %s", gen_code_debug_str(p.body));
			GEN_DEBUG_TRAP();
			return gen_InvalidCode;
		}
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Operator_Cast_Fwd;
	}
	result->Specs     = p.specs;
	result->ValueType = type;
	return result;
}

gen_CodeParams gen_def__param(gen_CodeTypename type, gen_Str name, gen_Opts_def_param* opt)
{
	gen_Opts_def_param p = get_optional(opt);

	if (! name_check(gen_def_param, name) || ! null_check(gen_def_param, type))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (type->Type != CT_Typename)
	{
		gen_log_failure("gen::gen_def_param: type is not a typename - %s", gen_code_debug_str(type));
		return gen_InvalidCode;
	}
	if (p.value && p.value->Type != CT_Untyped)
	{
		gen_log_failure("gen::gen_def_param: value is not untyped - %s", gen_code_debug_str(p.value));
		return gen_InvalidCode;
	}
	gen_CodeParams result = (gen_CodeParams)gen_make_code();
	result->Type          = CT_Parameters;
	result->Name          = gen_cache_str(name);
	result->ValueType     = type;
	result->Value         = p.value;
	result->NumEntries++;
	return result;
}

gen_CodePragma gen_def_pragma(gen_Str directive)
{
	if (directive.Len <= 0 || directive.Ptr == gen_nullptr)
	{
		gen_log_failure("gen::gen_def_comment: Invalid comment provided:");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodePragma result = (gen_CodePragma)gen_make_code();
	result->Type          = CT_Preprocess_Pragma;
	result->Content       = gen_cache_str(directive);
	return result;
}

gen_CodePreprocessCond gen_def_preprocess_cond(gen_EPreprocessCOnd type, gen_Str expr)
{
	if (expr.Len <= 0 || expr.Ptr == gen_nullptr)
	{
		gen_log_failure("gen::gen_def_comment: Invalid comment provided:");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodePreprocessCond result = (gen_CodePreprocessCond)gen_make_code();
	result->Content               = gen_cache_str(expr);
	switch (type)
	{
		case PreprocessCond_If:
			result->Type = CT_Preprocess_If;
			break;
		case PreprocessCond_IfDef:
			result->Type = CT_Preprocess_IfDef;
			break;
		case PreprocessCond_IfNotDef:
			result->Type = CT_Preprocess_IfNotDef;
			break;
		case PreprocessCond_ElIf:
			result->Type = CT_Preprocess_ElIf;
			break;
	}
	return result;
}

gen_CodeSpecifiers gen_def_specifier(gen_Specifier spec)
{
	gen_CodeSpecifiers result = (gen_CodeSpecifiers)gen_make_code();
	result->Type              = CT_Specifiers;
	gen_specifiers_append(result, spec);
	return result;
}

gen_CodeStruct gen_def__struct(gen_Str name, gen_Opts_def_struct* opt)
{
	gen_Opts_def_struct p = get_optional(opt);

	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_struct: attributes was not a `PlatformAttributes` type - %s", gen_code_debug_str(gen_cast(gen_Code, p.attributes)));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.parent && p.parent->Type != CT_Typename)
	{
		gen_log_failure("gen::gen_def_struct: parent was not a `Struct` type - %s", gen_code_debug_str(p.parent));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.body && p.body->Type != CT_Struct_Body)
	{
		gen_log_failure("gen::gen_def_struct: body was not a Struct_Body type - %s", gen_code_debug_str(p.body));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeStruct result = (gen_CodeStruct)gen_make_code();
	result->ModuleFlags   = p.mflags;
	if (name.Len)
		result->Name = gen_cache_str(name);

	if (p.body)
	{
		result->Type = CT_Struct;
		result->Body = p.body;
	}
	else
	{
		result->Type = CT_Struct_Fwd;
	}
	result->Attributes   = p.attributes;
	result->Specs        = p.specifiers;
	result->ParentAccess = p.parent_access;
	result->ParentType   = p.parent;

	for (gen_s32 idx = 0; idx < p.num_interfaces; idx++)
	{
		gen_struct_add_interface(result, p.interfaces[idx]);
	}
	return result;
}

gen_CodeTemplate gen_def__template(gen_CodeParams params, gen_Code declaration, gen_Opts_def_template* opt)
{
	gen_Opts_def_template p = get_optional(opt);

	if (! null_check(gen_def_template, declaration))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (params && params->Type != CT_Parameters)
	{
		gen_log_failure("gen::gen_def_template: params is not of parameters type - %s", gen_code_debug_str(params));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	switch (declaration->Type)
	{
		case CT_Class:
		case CT_Function:
		case CT_Struct:
		case CT_Variable:
		case CT_Using:
			break;

		default:
			gen_log_failure(
			    "gen::gen_def_template: declaration is not of class, function, struct, variable, or using type - %s",
			    gen_code_debug_str(declaration)
			);
	}
	gen_CodeTemplate result = (gen_CodeTemplate)gen_make_code();
	result->Type            = CT_Template;
	result->ModuleFlags     = p.mflags;
	result->Params          = params;
	result->Declaration     = declaration;
	return result;
}

gen_CodeTypename gen_def__type(gen_Str name, gen_Opts_def_type* opt)
{
	gen_Opts_def_type p = get_optional(opt);

	if (! name_check(gen_def_type, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_Code           gen_array_expr = p.gen_array_expr;
	gen_CodeSpecifiers specifiers     = p.specifiers;
	gen_CodeAttributes attributes     = p.attributes;
	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_type: attributes is not of attributes type - %s", gen_code_debug_str((gen_Code)p.attributes));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.specifiers && p.specifiers->Type != CT_Specifiers)
	{
		gen_log_failure("gen::gen_def_type: specifiers is not of specifiers type - %s", gen_code_debug_str((gen_Code)p.specifiers));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.gen_array_expr && p.gen_array_expr->Type != CT_Untyped)
	{
		gen_log_failure("gen::gen_def_type: arrayexpr is not of untyped type - %s", gen_code_debug_str((gen_Code)p.gen_array_expr));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeTypename result = (gen_CodeTypename)gen_make_code();
	result->Name            = gen_cache_str(name);
	result->Type            = CT_Typename;
	result->Attributes      = p.attributes;
	result->Specs           = p.specifiers;
	result->ArrExpr         = p.gen_array_expr;
	result->TypeTag         = p.type_tag;
	return result;
}

gen_CodeTypedef gen_def__typedef(gen_Str name, gen_Code type, gen_Opts_def_typedef* opt)
{
	gen_Opts_def_typedef p = get_optional(opt);

	if (! null_check(gen_def_typedef, type))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	switch (type->Type)
	{
		case CT_Class:
		case CT_Class_Fwd:
		case CT_Enum:
		case CT_Enum_Fwd:
		case CT_Enum_Class:
		case CT_Enum_Class_Fwd:
		case CT_Function_Fwd:
		case CT_Struct:
		case CT_Struct_Fwd:
		case CT_Union:
		case CT_Typename:
			break;
		default:
			gen_log_failure(
			    "gen::gen_def_typedef: type was not a Class, Enum, Function Forward, Struct, Typename, or Union - %s",
			    gen_code_debug_str((gen_Code)type)
			);
			GEN_DEBUG_TRAP();
			return gen_InvalidCode;
	}
	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_typedef: attributes was not a PlatformAttributes - %s", gen_code_debug_str((gen_Code)p.attributes));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	// Registering the type.
	gen_CodeTypename registered_type = gen_def_type(name);
	if (! registered_type)
	{
		gen_log_failure("gen::gen_def_typedef: failed to register type");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeTypedef result = (gen_CodeTypedef)gen_make_code();
	result->Type           = CT_Typedef;
	result->ModuleFlags    = p.mflags;
	result->UnderlyingType = type;

	if (name.Len <= 0)
	{
		if (type->Type != CT_Untyped)
		{
			gen_log_failure("gen::gen_def_typedef: name was empty and type was not untyped (indicating its a function typedef) - %s", gen_code_debug_str(type));
			GEN_DEBUG_TRAP();
			return gen_InvalidCode;
		}
		result->Name       = gen_cache_str(type->Name);
		result->IsFunction = true;
	}
	else
	{
		result->Name       = gen_cache_str(name);
		result->IsFunction = false;
	}
	return result;
}

gen_CodeUnion gen_def__union(gen_Str name, gen_CodeBody body, gen_Opts_def_union* opt)
{
	gen_Opts_def_union p = get_optional(opt);

	if (! null_check(gen_def_union, body))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (body->Type != CT_Union_Body)
	{
		gen_log_failure("gen::gen_def_union: body was not a Union_Body type - %s", gen_code_debug_str(body));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_union: attributes was not a PlatformAttributes type - %s", gen_code_debug_str(p.attributes));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeUnion result = (gen_CodeUnion)gen_make_code();
	result->ModuleFlags  = p.mflags;
	result->Type         = CT_Union;
	result->Body         = body;
	result->Attributes   = p.attributes;
	if (name.Ptr)
		result->Name = gen_cache_str(name);
	return result;
}

gen_CodeUsing gen_def__using(gen_Str name, gen_CodeTypename type, gen_Opts_def_using* opt)
{
	gen_Opts_def_using p = get_optional(opt);

	if (! name_check(gen_def_using, name) || null_check(gen_def_using, type))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}

	gen_CodeTypename register_type = gen_def_type(name);
	if (! register_type)
	{
		gen_log_failure("gen::gen_def_using: failed to register type");
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_using: attributes was not a PlatformAttributes type - %s", gen_code_debug_str(p.attributes));
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeUsing result   = (gen_CodeUsing)gen_make_code();
	result->Name           = gen_cache_str(name);
	result->ModuleFlags    = p.mflags;
	result->Type           = CT_Using;
	result->UnderlyingType = type;
	result->Attributes     = p.attributes;
	return result;
}

gen_CodeUsing gen_def_using_namespace(gen_Str name)
{
	if (! name_check(gen_def_using_namespace, name))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	gen_CodeUsing result = (gen_CodeUsing)gen_make_code();
	result->Name         = gen_cache_str(name);
	result->Type         = CT_Using_Namespace;
	return result;
}

gen_CodeVar gen_def__variable(gen_CodeTypename type, gen_Str name, gen_Opts_def_variable* opt)
{
	gen_Opts_def_variable p = get_optional(opt);

	if (! name_check(gen_def_variable, name) || ! null_check(gen_def_variable, type))
	{
		GEN_DEBUG_TRAP();
		return gen_InvalidCode;
	}
	if (p.attributes && p.attributes->Type != CT_PlatformAttributes)
	{
		gen_log_failure("gen::gen_def_variable: attributes was not a `PlatformAttributes` type - %s", gen_code_debug_str(p.attributes));
		return gen_InvalidCode;
	}
	if (p.specifiers && p.specifiers->Type != CT_Specifiers)
	{
		gen_log_failure("gen::gen_def_variable: specifiers was not a `Specifiers` type - %s", gen_code_debug_str(p.specifiers));
		return gen_InvalidCode;
	}
	if (type->Type != CT_Typename)
	{
		gen_log_failure("gen::gen_def_variable: type was not a Typename - %s", gen_code_debug_str(type));
		return gen_InvalidCode;
	}
	if (p.value && p.value->Type != CT_Untyped)
	{
		gen_log_failure("gen::gen_def_variable: value was not a `Untyped` type - %s", gen_code_debug_str(p.value));
		return gen_InvalidCode;
	}
	gen_CodeVar result  = (gen_CodeVar)gen_make_code();
	result->Name        = gen_cache_str(name);
	result->Type        = CT_Variable;
	result->ModuleFlags = p.mflags;
	result->ValueType   = type;
	result->Attributes  = p.attributes;
	result->Specs       = p.specifiers;
	result->Value       = p.value;
	return result;
}

#pragma region Helper Macros for gen_def_**_body functions

#define gen_def_body_start(Name_)                                                         \
	if (num <= 0)                                                                         \
	{                                                                                     \
		gen_log_failure("gen::" gen_stringize(Name_) ": num cannot be zero or negative"); \
		return gen_InvalidCode;                                                           \
	}

#define gen_def_body_code_array_start(Name_)                                               \
	if (num <= 0)                                                                          \
	{                                                                                      \
		gen_log_failure("gen::" gen_stringize(Name_) ": num cannot be zero or negative");  \
		return gen_InvalidCode;                                                            \
	}                                                                                      \
	if (codes == gen_nullptr)                                                              \
	{                                                                                      \
		gen_log_failure("gen::" gen_stringize(Name_) " : Provided a null array of codes"); \
		return gen_InvalidCode;                                                            \
	}

#pragma endregion Helper Macros for gen_def_** _body functions

gen_CodeBody gen_def_class_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_class_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Class_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_class_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_CLASS_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_class_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_class_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_class_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Function_Body;
	do
	{
		gen_Code entry = *codes;
		codes++;
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_class_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_CLASS_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_class_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);

	return result;
}

gen_CodeDefineParams gen_def_define_params(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_define_params);

	va_list va;
	va_start(va, num);

	gen_Code_POD         pod   = va_arg(va, gen_Code_POD);
	gen_CodeDefineParams param = gen_pcast(gen_CodeDefineParams, pod);

	null_check(gen_def_define_params, param);
	if (param->Type != CT_Parameters_Define)
	{
		gen_log_failure("gen::gen_def_define_params: param %d is not a parameter for a preprocessor define", num - num + 1);
		return gen_InvalidCode;
	}

	gen_CodeDefineParams result = (gen_CodeDefineParams)gen_code_duplicate(param);
	while (--num)
	{
		pod   = va_arg(va, gen_Code_POD);
		param = gen_pcast(gen_CodeDefineParams, pod);
		if (param->Type != CT_Parameters_Define)
		{
			gen_log_failure("gen::gen_def_define_params: param %d is not a parameter for a preprocessor define", num - num + 1);
			return gen_InvalidCode;
		}
		gen_define_params_append(result, param);
	}
	va_end(va);

	return result;
}

gen_CodeDefineParams gen_def_define_params_arr(gen_s32 num, gen_CodeDefineParams* codes)
{
	gen_def_body_code_array_start(gen_def_define_params);

#define check_current(current)                                                                                         \
	if (current == gen_nullptr)                                                                                        \
	{                                                                                                                  \
		gen_log_failure("gen::gen_def_define_params: Provide a null code in codes array");                             \
		return gen_InvalidCode;                                                                                        \
	}                                                                                                                  \
	if (current->Type != CT_Parameters_Define)                                                                         \
	{                                                                                                                  \
		gen_log_failure(                                                                                               \
		    "gen::gen_def_define_params: gen_Code in coes array is not of paramter for preprocessor define type - %s", \
		    gen_code_debug_str(current)                                                                                \
		);                                                                                                             \
		return gen_InvalidCode;                                                                                        \
	}
	gen_CodeDefineParams current = (gen_CodeDefineParams)gen_code_duplicate(*codes);
	check_current(current);

	gen_CodeDefineParams result = (gen_CodeDefineParams)gen_make_code();
	result->Name                = current->Name;
	result->Type                = current->Type;
	while (codes++, current = *codes, num--, num > 0)
	{
		check_current(current);
		gen_define_params_append(result, current);
	}
#undef check_current

	return result;
}

gen_CodeBody gen_def_enum_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_enum_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Enum_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure("gen::gen_def_enum_body: Provided a null entry");
			return gen_InvalidCode;
		}
		if (entry->Type != CT_Untyped && entry->Type != CT_Comment)
		{
			gen_log_failure("gen::gen_def_enum_body: Entry type is not allowed - %s. Must be of untyped or comment type.", gen_code_debug_str(entry));
			return gen_InvalidCode;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return (gen_CodeBody)result;
}

gen_CodeBody gen_def_enum_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_enum_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Enum_Body;
	do
	{
		gen_Code entry = *codes;
		if (! entry)
		{
			gen_log_failure("gen::gen_def_enum_body: Provided a null entry");
			return gen_InvalidCode;
		}
		if (entry->Type != CT_Untyped && entry->Type != CT_Comment)
		{
			gen_log_failure("gen::gen_def_enum_body: Entry type is not allowed: %s", gen_code_debug_str(entry));
			return gen_InvalidCode;
		}
		gen_body_append(result, entry);
	} while (codes++, num--, num > 0);

	return result;
}

gen_CodeBody gen_def_export_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_export_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Export_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_export_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_EXPORT_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_export_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_export_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_export_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Export_Body;
	do
	{
		gen_Code entry = *codes;
		codes++;
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_export_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_EXPORT_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_export_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);

	return result;
}

gen_CodeBody gen_def_extern_link_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_extern_linkage_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Extern_Linkage_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_extern_linkage_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_extern_linkage_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_extern_link_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_extern_linkage_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Extern_Linkage_Body;
	do
	{
		gen_Code entry = *codes;
		codes++;
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_extern_linkage_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_EXTERN_LINKAGE_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_extern_linkage_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);

	return result;
}

gen_CodeBody gen_def_function_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_function_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Function_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure("gen::" gen_stringize(gen_def_function_body) ": Provided an null entry");
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES_CASES:
			gen_log_failure("gen::" gen_stringize(gen_def_function_body) ": Entry type is not allowed: %s", gen_code_debug_str(entry));
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_function_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_function_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Function_Body;
	do
	{
		gen_Code entry = *codes;
		codes++;
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_function_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_FUNCTION_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_function_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);

	return result;
}

gen_CodeBody gen_def_global_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_global_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Global_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_global_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
			case CT_Global_Body:
				// result.gen_body_append( entry.gen_code_cast<gen_CodeBody>() ) ;
				gen_body_append_body(result, gen_cast(gen_CodeBody, entry));
				continue;

			GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES_CASES:
				gen_log_failure(
				    "gen::"
				    "gen_def_global_body"
				    ": Entry type is not allowed: %s",
				    gen_code_debug_str(entry)
				);
				return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_global_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_global_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Global_Body;
	do
	{
		gen_Code entry = *codes;
		codes++;
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_global_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
			case CT_Global_Body:
				gen_body_append_body(result, gen_cast(gen_CodeBody, entry));
				continue;

			GEN_AST_BODY_GLOBAL_UNALLOWED_TYPES_CASES:
				gen_log_failure(
				    "gen::"
				    "gen_def_global_body"
				    ": Entry type is not allowed: %s",
				    gen_code_debug_str(entry)
				);
				return gen_InvalidCode;

			default:
				break;
		}

		gen_body_append(result, entry);
	} while (num--, num > 0);

	return result;
}

gen_CodeBody gen_def_namespace_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_namespace_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Namespace_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_namespace_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_namespace_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_namespace_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_namespace_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Global_Body;
	do
	{
		gen_Code entry = *codes;
		codes++;
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_namespace_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_NAMESPACE_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_namespace_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);

	return result;
}

gen_CodeParams gen_def_params(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_params);

	va_list va;
	va_start(va, num);

	gen_Code_POD   pod   = va_arg(va, gen_Code_POD);
	gen_CodeParams param = gen_pcast(gen_CodeParams, pod);

	null_check(gen_def_params, param);
	if (param->Type != CT_Parameters)
	{
		gen_log_failure("gen::gen_def_params: param %d is not a Parameters", num - num + 1);
		return gen_InvalidCode;
	}

	gen_CodeParams result = (gen_CodeParams)gen_code_duplicate(param);
	while (--num)
	{
		pod   = va_arg(va, gen_Code_POD);
		param = gen_pcast(gen_CodeParams, pod);
		if (param->Type != CT_Parameters)
		{
			gen_log_failure("gen::gen_def_params: param %d is not a Parameters", num - num + 1);
			return gen_InvalidCode;
		}
		gen_params_append(result, param);
	}
	va_end(va);

	return result;
}

gen_CodeParams gen_def_params_arr(gen_s32 num, gen_CodeParams* codes)
{
	gen_def_body_code_array_start(gen_def_params);

#define check_current(current)                                                                                                    \
	if (current == gen_nullptr)                                                                                                   \
	{                                                                                                                             \
		gen_log_failure("gen::gen_def_params: Provide a null code in codes array");                                               \
		return gen_InvalidCode;                                                                                                   \
	}                                                                                                                             \
	if (current->Type != CT_Parameters)                                                                                           \
	{                                                                                                                             \
		gen_log_failure("gen::gen_def_params: gen_Code in coes array is not of paramter type - %s", gen_code_debug_str(current)); \
		return gen_InvalidCode;                                                                                                   \
	}
	gen_CodeParams current = (gen_CodeParams)gen_code_duplicate(*codes);
	check_current(current);

	gen_CodeParams result = (gen_CodeParams)gen_make_code();
	result->Name          = current->Name;
	result->Type          = current->Type;
	result->ValueType     = current->ValueType;
	while (codes++, current = *codes, num--, num > 0)
	{
		check_current(current);
		gen_params_append(result, current);
	}
#undef check_current

	return result;
}

gen_CodeSpecifiers gen_def_specifiers(gen_s32 num, ...)
{
	if (num <= 0)
	{
		gen_log_failure("gen::gen_def_specifiers: num cannot be zero or less");
		return gen_InvalidCode;
	}
	if (num > gen_AST_ArrSpecs_Cap)
	{
		gen_log_failure("gen::gen_def_specifiers: num of speciifers to define gen_AST larger than gen_AST specicifier capacity - %d", num);
		return gen_InvalidCode;
	}
	gen_CodeSpecifiers result = (gen_CodeSpecifiers)gen_make_code();
	result->Type              = CT_Specifiers;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Specifier type = (gen_Specifier)va_arg(va, int);
		gen_specifiers_append(result, type);
	} while (--num, num);
	va_end(va);

	return result;
}

gen_CodeSpecifiers gen_def_specifiers_arr(gen_s32 num, gen_Specifier* specs)
{
	if (num <= 0)
	{
		gen_log_failure("gen::gen_def_specifiers: num cannot be zero or less");
		return gen_InvalidCode;
	}
	if (num > gen_AST_ArrSpecs_Cap)
	{
		gen_log_failure("gen::gen_def_specifiers: num of speciifers to define gen_AST larger than gen_AST specicifier capacity - %d", num);
		return gen_InvalidCode;
	}
	gen_CodeSpecifiers result = (gen_CodeSpecifiers)gen_make_code();
	result->Type              = CT_Specifiers;

	gen_s32 idx               = 0;
	do
	{
		gen_specifiers_append(result, specs[idx]);
		idx++;
	} while (--num, num);

	return result;
}

gen_CodeBody gen_def_struct_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_struct_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Struct_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_struct_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_STRUCT_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_struct_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_struct_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_struct_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Struct_Body;
	do
	{
		gen_Code entry = *codes;
		codes++;
		if (! entry)
		{
			gen_log_failure(
			    "gen::"
			    "gen_def_struct_body"
			    ": Provided an null entry"
			);
			return gen_InvalidCode;
		}
		switch (entry->Type)
		{
		GEN_AST_BODY_STRUCT_UNALLOWED_TYPES_CASES:
			gen_log_failure(
			    "gen::"
			    "gen_def_struct_body"
			    ": Entry type is not allowed: %s",
			    gen_code_debug_str(entry)
			);
			return gen_InvalidCode;

			default:
				break;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);

	return result;
}

gen_CodeBody gen_def_union_body(gen_s32 num, ...)
{
	gen_def_body_start(gen_def_union_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Union_Body;

	va_list va;
	va_start(va, num);
	do
	{
		gen_Code_POD pod   = va_arg(va, gen_Code_POD);
		gen_Code     entry = gen_pcast(gen_Code, pod);
		if (! entry)
		{
			gen_log_failure("gen::gen_def_union_body: Provided a null entry");
			return gen_InvalidCode;
		}
		if (entry->Type != CT_Untyped && entry->Type != CT_Comment)
		{
			gen_log_failure("gen::gen_def_union_body: Entry type is not allowed - %s. Must be of untyped or comment type.", gen_code_debug_str(entry));
			return gen_InvalidCode;
		}
		gen_body_append(result, entry);
	} while (num--, num > 0);
	va_end(va);

	return result;
}

gen_CodeBody gen_def_union_body_arr(gen_s32 num, gen_Code* codes)
{
	gen_def_body_code_array_start(gen_def_union_body);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Union_Body;
	do
	{
		gen_Code entry = *codes;
		if (! entry)
		{
			gen_log_failure("gen::gen_def_union_body: Provided a null entry");
			return gen_InvalidCode;
		}
		if (entry->Type != CT_Untyped && entry->Type != CT_Comment)
		{
			gen_log_failure("gen::gen_def_union_body: Entry type is not allowed: %s", gen_code_debug_str(entry));
			return gen_InvalidCode;
		}
		gen_body_append(result, entry);
	} while (codes++, num--, num > 0);

	return (gen_CodeBody)result;
}

#undef name_check
#undef null_check
#undef gen_def_body_start
#undef gen_def_body_code_array_start

#pragma endregion Upfront

#pragma region Parsing


gen_StrBuilder gen_tok_to_strbuilder(gen_Token tok)
{
	gen_StrBuilder result   = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, gen_kilobytes(4));
	gen_Str        type_str = gen_toktype_to_str(tok.Type);

	gen_strbuilder_append_fmt(
	    &result,
	    "Line: %d Column: %d, Type: %.*s Content: %.*s",
	    tok.Line,
	    tok.Column,
	    type_str.Len,
	    type_str.Ptr,
	    tok.Text.Len,
	    tok.Text.Ptr
	);
	return result;
}

bool gen_lex__eat(gen_TokArray* self, gen_TokType type);

gen_Token* gen_lex_current(gen_TokArray* self, bool skip_formatting)
{
	if (skip_formatting)
	{
		while (self->Arr[self->Idx].Type == Tok_NewLine || self->Arr[self->Idx].Type == Tok_Comment)
			self->Idx++;
	}
	return &self->Arr[self->Idx];
}

gen_Token* gen_lex_peek(gen_TokArray self, bool skip_formatting)
{
	gen_s32 idx = self.Idx;
	if (skip_formatting)
	{
		while (self.Arr[idx].Type == Tok_NewLine)
			idx++;

		return &self.Arr[idx];
	}
	return &self.Arr[idx];
}

gen_Token* gen_lex_previous(gen_TokArray self, bool skip_formatting)
{
	gen_s32 idx = self.Idx;
	if (skip_formatting)
	{
		while (self.Arr[idx].Type == Tok_NewLine)
			idx--;

		return &self.Arr[idx];
	}
	return &self.Arr[idx - 1];
}

gen_Token* gen_lex_next(gen_TokArray self, bool skip_formatting)
{
	gen_s32 idx = self.Idx;
	if (skip_formatting)
	{
		while (self.Arr[idx].Type == Tok_NewLine)
			idx++;

		return &self.Arr[idx + 1];
	}
	return &self.Arr[idx + 1];
}

enum
{
	Lex_Continue,
	Lex_ReturnNull,
};

gen_forceinline void lexer_move_forward(gen_LexContext* ctx)
{
	if (*ctx->scanner == '\n')
	{
		ctx->line   += 1;
		ctx->column  = 1;
	}
	else
	{
		++ctx->column;
	}
	--ctx->left;
	++ctx->scanner;
}

#define move_forward() lexer_move_forward(ctx)

gen_forceinline void lexer_skip_whitespace(gen_LexContext* ctx)
{
	while (ctx->left && gen_char_is_space(*ctx->scanner))
		move_forward();
}

#define skip_whitespace() lexer_skip_whitespace(ctx)

gen_forceinline void lexer_end_line(gen_LexContext* ctx)
{
	while (ctx->left && (*ctx->scanner) == ' ')
		move_forward();

	if (ctx->left && (*ctx->scanner) == '\r')
	{
		move_forward();
		move_forward();
	}
	else if (ctx->left && (*ctx->scanner) == '\n')
		move_forward();
}

#define gen_end_line() lexer_end_line(ctx)

// TODO(Ed): We need to to attempt to recover from a gen_lex failure?
gen_s32 gen_lex_preprocessor_define(gen_LexContext* ctx)
{
	gen_Token name = {
		{ ctx->scanner, 1 },
		Tok_Identifier,
		ctx->line,
		ctx->column,
		TF_Preprocess
	};
	move_forward();

	while (ctx->left && (gen_char_is_alphanumeric((*ctx->scanner)) || (*ctx->scanner) == '_'))
	{
		move_forward();
		name.Text.Len++;
	}

	gen_Specifier spec          = gen_str_to_specifier(name.Text);
	gen_TokType   attrib        = gen_str_to_toktype(name.Text);
	gen_b32       not_specifier = spec == Spec_Invalid;
	gen_b32       not_attribute = attrib <= Tok___Attributes_Start;

	gen_Macro  macro            = { name.Text, MT_Expression, (gen_MacroFlags)0 };
	gen_Macro* registered_macro = lookup_macro(name.Text);

	if (registered_macro == gen_nullptr && not_specifier && not_attribute)
	{
		gen_log_fmt(
		    "Warning: '%S' was not registered before the lexer processed its #define directive, it will be registered as a expression macro\n",
		    name.Text
		);
		// GEN_DEBUG_TRAP();
	}
	gen_array_append(gen__ctx->gen_Lexer_Tokens, name);

	if (ctx->left && (*ctx->scanner) == '(')
	{
		if (registered_macro && ! gen_macro_is_functional(*registered_macro))
		{
			gen_log_fmt(
			    "Warning: %S registered macro is not flagged as functional yet the definition detects opening parenthesis '(' for arguments\n",
			    name.Text
			);
			// GEN_DEBUG_TRAP();
		}
		else
		{
			macro.Flags |= MF_Functional;
		}

		gen_Token opening_paren = {
			{ ctx->scanner, 1 },
			Tok_Paren_Open,
			ctx->line,
			ctx->column,
			TF_Preprocess
		};
		gen_array_append(gen__ctx->gen_Lexer_Tokens, opening_paren);
		move_forward();

		gen_Token last_parameter = {};
		// We need to tokenize the define's arguments now:
		while (ctx->left && *ctx->scanner != ')')
		{
			skip_whitespace();

			gen_Str possible_varadic = { ctx->scanner, 3 };
			if (ctx->left > 3 && gen_str_are_equal(gen_txt("..."), possible_varadic))
			{
				gen_Token parameter = {
					{ ctx->scanner, 3 },
					Tok_Preprocess_Define_Param,
					ctx->line,
					ctx->column,
					TF_Preprocess
				};
				move_forward();
				move_forward();
				move_forward();

				gen_array_append(gen__ctx->gen_Lexer_Tokens, parameter);
				skip_whitespace();
				last_parameter = parameter;

				while ((*ctx->scanner) == '\\')
				{
					move_forward();
					skip_whitespace();
				}
				if (*ctx->scanner != ')')
				{
					gen_log_failure(
					    "gen_lex_preprocessor_define(%d, %d): Expected a ')' after '...' (varaidc macro param) %S\n",
					    ctx->line,
					    ctx->column,
					    name.Text
					);
					return Lex_ReturnNull;
				}
				break;
			}
			else if ((*ctx->scanner) == '\\')
			{
				move_forward();
				skip_whitespace();
				continue;
			}
			else if (gen_char_is_alpha((*ctx->scanner)) || (*ctx->scanner) == '_')
			{
				gen_Token parameter = {
					{ ctx->scanner, 1 },
					Tok_Preprocess_Define_Param,
					ctx->line,
					ctx->column,
					TF_Preprocess
				};
				move_forward();

				while (ctx->left && (gen_char_is_alphanumeric((*ctx->scanner)) || (*ctx->scanner) == '_'))
				{
					move_forward();
					parameter.Text.Len++;
				}
				gen_array_append(gen__ctx->gen_Lexer_Tokens, parameter);
				skip_whitespace();
				last_parameter = parameter;
			}
			else
			{
				gen_log_failure(
				    "gen_lex_preprocessor_define(%d, %d): Expected a '_' or alpha character for a parameter name for %S\n",
				    ctx->line,
				    ctx->column,
				    name.Text
				);
				return Lex_ReturnNull;
			}

			if (*ctx->scanner == ')')
				break;

			// There should be a comma
			if (*ctx->scanner != ',')
			{
				gen_log_failure(
				    "gen_lex_preprocessor_define(%d, %d): Expected a comma after parameter %S for %S\n",
				    ctx->line,
				    ctx->column,
				    last_parameter.Text,
				    name.Text
				);
				return Lex_ReturnNull;
			}
			gen_Token comma = {
				{ ctx->scanner, 1 },
				Tok_Comma,
				ctx->line,
				ctx->column,
				TF_Preprocess
			};
			gen_array_append(gen__ctx->gen_Lexer_Tokens, comma);
			move_forward();
		}

		if (*ctx->scanner != ')')
		{
			gen_log_failure(
			    "gen_lex_preprocessor_define(%d, %d): Expected a ')' after last_parameter %S for %S (ran out of characters...)\n",
			    ctx->line,
			    ctx->column,
			    last_parameter.Text,
			    name.Text
			);
			return Lex_ReturnNull;
		}
		gen_Token closing_paren = {
			{ ctx->scanner, 1 },
			Tok_Paren_Close,
			ctx->line,
			ctx->column,
			TF_Preprocess
		};
		gen_array_append(gen__ctx->gen_Lexer_Tokens, closing_paren);
		move_forward();
	}
	else if (registered_macro && gen_macro_is_functional(*registered_macro))
	{
		if (registered_macro && ! gen_macro_is_functional(*registered_macro))
		{
			gen_log_fmt(
			    "Warning: %S registered macro is flagged as functional yet the definition detects no opening parenthesis '(' for arguments\n",
			    name.Text
			);
			GEN_DEBUG_TRAP();
		}
	}

	if (registered_macro == gen_nullptr)
	{
		gen_register_macro(macro);
	}

	// Define's content handled by gen_lex_preprocessor_directive (the original caller of this)
	return Lex_Continue;
}

// TODO(Ed): We need to to attempt to recover from a gen_lex failure?
gen_s32 gen_lex_preprocessor_directive(gen_LexContext* ctx)
{
	char const* gen_hash = ctx->scanner;
	gen_Token   hash_tok = {
        { gen_hash, 1 },
        Tok_Preprocess_Hash,
        ctx->line,
        ctx->column,
        TF_Preprocess
	};
	gen_array_append(gen__ctx->gen_Lexer_Tokens, hash_tok);

	move_forward();
	skip_whitespace();

	ctx->token.Text.Ptr = ctx->scanner;
	while (ctx->left && ! gen_char_is_space((*ctx->scanner)))
	{
		move_forward();
		ctx->token.Text.Len++;
	}

	ctx->token.Type      = gen_str_to_toktype(ctx->token.Text);

	bool is_preprocessor = ctx->token.Type >= Tok_Preprocess_Define && ctx->token.Type <= Tok_Preprocess_Pragma;
	if (! is_preprocessor)
	{
		ctx->token.Type = Tok_Preprocess_Unsupported;

		// Its an unsupported directive, skip it
		gen_s32 within_string = false;
		gen_s32 within_char   = false;
		while (ctx->left)
		{
			if (*ctx->scanner == '"' && ! within_char)
				within_string ^= true;

			if (*ctx->scanner == '\'' && ! within_string)
				within_char ^= true;

			if (*ctx->scanner == '\\' && ! within_string && ! within_char)
			{
				move_forward();
				ctx->token.Text.Len++;

				if ((*ctx->scanner) == '\r')
				{
					move_forward();
					ctx->token.Text.Len++;
				}

				if ((*ctx->scanner) == '\n')
				{
					move_forward();
					ctx->token.Text.Len++;
					continue;
				}
				else
				{
					gen_log_failure(
					    "gen::Parser::gen_lex: Invalid escape sequence '\\%c' (%d, %d)"
					    " in preprocessor directive (%d, %d)\n%.100s",
					    (*ctx->scanner),
					    ctx->line,
					    ctx->column,
					    ctx->token.Line,
					    ctx->token.Column,
					    ctx->token.Text
					);
					break;
				}
			}

			if ((*ctx->scanner) == '\r')
			{
				move_forward();
				ctx->token.Text.Len++;
			}

			if ((*ctx->scanner) == '\n')
			{
				move_forward();
				ctx->token.Text.Len++;
				break;
			}

			move_forward();
			ctx->token.Text.Len++;
		}

		ctx->token.Text.Len = ctx->token.Text.Len + ctx->token.Text.Ptr - gen_hash;
		ctx->token.Text.Ptr = gen_hash;
		gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);
		return Lex_Continue;    // Skip found token, its all handled here.
	}

	if (ctx->token.Type == Tok_Preprocess_Else || ctx->token.Type == Tok_Preprocess_EndIf)
	{
		ctx->token.Flags |= TF_Preprocess_Cond;
		gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);
		gen_end_line();
		return Lex_Continue;
	}
	else if (ctx->token.Type >= Tok_Preprocess_If && ctx->token.Type <= Tok_Preprocess_ElIf)
	{
		ctx->token.Flags |= TF_Preprocess_Cond;
	}

	gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);

	skip_whitespace();

	if (ctx->token.Type == Tok_Preprocess_Define)
	{
		gen_u32 result = gen_lex_preprocessor_define(ctx);    // handles: #define <name>( <params> ) - define's content handled later on within this scope.
		if (result != Lex_Continue)
			return Lex_ReturnNull;
	}

	gen_Token gen_preprocess_content = {
		{ ctx->scanner, 0 },
		Tok_Preprocess_Content,
		ctx->line,
		ctx->column,
		TF_Preprocess
	};

	if (ctx->token.Type == Tok_Preprocess_Include)
	{
		gen_preprocess_content.Type = Tok_String;

		if ((*ctx->scanner) != '"' && (*ctx->scanner) != '<')
		{
			gen_StrBuilder directive_str =
			    gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "%.*s", gen_min(80, ctx->left + gen_preprocess_content.Text.Len), ctx->token.Text.Ptr);

			gen_log_failure(
			    "gen::Parser::gen_lex: Expected '\"' or '<' after #include, not '%c' (%d, %d)\n%s",
			    (*ctx->scanner),
			    gen_preprocess_content.Line,
			    gen_preprocess_content.Column,
			    (char*)directive_str
			);
			return Lex_ReturnNull;
		}
		move_forward();
		gen_preprocess_content.Text.Len++;

		while (ctx->left && (*ctx->scanner) != '"' && (*ctx->scanner) != '>')
		{
			move_forward();
			gen_preprocess_content.Text.Len++;
		}

		move_forward();
		gen_preprocess_content.Text.Len++;

		if ((*ctx->scanner) == '\r' && ctx->scanner[1] == '\n')
		{
			move_forward();
			move_forward();
		}
		else if ((*ctx->scanner) == '\n')
		{
			move_forward();
		}

		gen_array_append(gen__ctx->gen_Lexer_Tokens, gen_preprocess_content);
		return Lex_Continue;    // Skip found token, its all handled here.
	}

	gen_s32 within_string = false;
	gen_s32 within_char   = false;

	// Consume preprocess content
	while (ctx->left)
	{
		if ((*ctx->scanner) == '"' && ! within_char)
			within_string ^= true;

		if ((*ctx->scanner) == '\'' && ! within_string)
			within_char ^= true;

		if ((*ctx->scanner) == '\\' && ! within_string && ! within_char)
		{
			move_forward();
			gen_preprocess_content.Text.Len++;

			if ((*ctx->scanner) == '\r')
			{
				move_forward();
				gen_preprocess_content.Text.Len++;
			}

			if ((*ctx->scanner) == '\n')
			{
				move_forward();
				gen_preprocess_content.Text.Len++;
				continue;
			}
			else
			{
				gen_StrBuilder directive_str = gen_strbuilder_make_length(gen__ctx->Allocator_Temp, ctx->token.Text.Ptr, ctx->token.Text.Len);
				gen_StrBuilder content_str   = gen_strbuilder_fmt_buf(
                    gen__ctx->Allocator_Temp,
                    "%.*s",
                    gen_min(400, ctx->left + gen_preprocess_content.Text.Len),
                    gen_preprocess_content.Text.Ptr
                );

				gen_log_failure(
				    "gen::Parser::gen_lex: Invalid escape sequence '\\%c' (%d, %d)"
				    " in preprocessor directive '%s' (%d, %d)\n%s",
				    (*ctx->scanner),
				    ctx->line,
				    ctx->column,
				    directive_str,
				    gen_preprocess_content.Line,
				    gen_preprocess_content.Column,
				    content_str
				);
				return Lex_ReturnNull;
				break;
			}
		}

		if ((*ctx->scanner) == '\r')
		{
			break;
			//move_forward();
		}

		if ((*ctx->scanner) == '\n')
		{
			//move_forward();
			break;
		}

		move_forward();
		gen_preprocess_content.Text.Len++;
	}

	gen_array_append(gen__ctx->gen_Lexer_Tokens, gen_preprocess_content);
	return Lex_Continue;    // Skip found token, its all handled here.
}

void gen_lex_found_token(gen_LexContext* ctx)
{
	if (ctx->token.Type != Tok_Invalid)
	{
		gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);
		return;
	}

	gen_TokType type = gen_str_to_toktype(ctx->token.Text);

	if (type == Tok_Preprocess_Define || type == Tok_Preprocess_Include)
	{
		ctx->token.Flags |= TF_Identifier;
	}

	if (type <= Tok_Access_Public && type >= Tok_Access_Private)
	{
		ctx->token.Flags |= TF_AccessSpecifier;
	}
	if (type > Tok___Attributes_Start)
	{
		ctx->token.Flags |= TF_Attribute;
	}
	if (type == Tok_Decl_Extern_Linkage)
	{
		skip_whitespace();

		if ((*ctx->scanner) != '"')
		{
			type              = Tok_Spec_Extern;
			ctx->token.Flags |= TF_Specifier;
		}

		ctx->token.Type = type;
		gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);
		return;
	}
	if ((type <= Tok_Star && type >= Tok_Spec_Alignas) || type == Tok_Ampersand || type == Tok_Ampersand_DBL)
	{
		ctx->token.Type   = type;
		ctx->token.Flags |= TF_Specifier;
		gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);
		return;
	}
	if (type != Tok_Invalid)
	{
		ctx->token.Type = type;
		gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);
		return;
	}

	gen_Macro* macro             = lookup_macro(ctx->token.Text);
	gen_b32    has_args          = ctx->left && (*ctx->scanner) == '(';
	gen_b32    resolved_to_macro = false;
	if (macro)
	{
		ctx->token.Type       = gen_macrotype__to_toktype(macro->Type);
		gen_b32 is_functional = gen_macro_is_functional(*macro);
		resolved_to_macro     = has_args ? is_functional : ! is_functional;
		if (! resolved_to_macro && GEN_BUILD_DEBUG)
		{
			gen_log_fmt(
			    "Info(%d, %d): %S identified as a macro but usage here does not resolve to one (interpreting as identifier)\n",
			    ctx->token.Line,
			    ctx->token.Line,
			    macro->Name
			);
		}
	}
	if (resolved_to_macro)
	{
		// TODO(Ed): When we introduce a macro gen_AST (and expression support), we'll properly gen_lex this section.
		// Want to ignore any arguments the define may have as they can be execution expressions.
		if (has_args)
		{
			ctx->token.Flags |= TF_Macro_Functional;
		}
		if (gen_bitfield_is_set(gen_MacroFlags, macro->Flags, MF_Allow_As_Attribute))
		{
			ctx->token.Flags |= TF_Attribute;
		}
		if (gen_bitfield_is_set(gen_MacroFlags, macro->Flags, MF_Allow_As_Specifier))
		{
			ctx->token.Flags |= TF_Specifier;
		}
	}
	else
	{
		ctx->token.Type = Tok_Identifier;
	}

	gen_array_append(gen__ctx->gen_Lexer_Tokens, ctx->token);
}

// TODO(Ed): We need to to attempt to recover from a gen_lex failure?
gen_neverinline gen_TokArray gen_lex(gen_Str content)
{
	gen_LexContext  c;
	gen_LexContext* ctx     = &c;
	c.content               = content;
	c.left                  = content.Len;
	c.scanner               = content.Ptr;

	char const* word        = c.scanner;
	gen_s32     word_length = 0;

	c.line                  = 1;
	c.column                = 1;

	skip_whitespace();
	if (c.left <= 0)
	{
		gen_log_failure("gen::gen_lex: no tokens found (only whitespace provided)");
		gen_TokArray null_array = {};
		return null_array;
	}

	gen_array_clear(gen__ctx->gen_Lexer_Tokens);

	gen_b32 gen_preprocess_args = true;

	while (c.left)
	{
#if 0
		if (Tokens.num())
		{
			gen_log_fmt("\nLastTok: %SB", Tokens.back().to_strbuilder());
		}
#endif

		{
			gen_Token thanks_c = {
				{ c.scanner, 0 },
				Tok_Invalid,
				c.line,
				c.column,
				TF_Null
			};
			c.token = thanks_c;
		}

		bool is_define = false;

		if (c.column == 1)
		{
			if ((*ctx->scanner) == '\r')
			{
				move_forward();
				c.token.Text.Len = 1;
			}

			if ((*ctx->scanner) == '\n')
			{
				move_forward();

				c.token.Type = Tok_NewLine;
				c.token.Text.Len++;

				gen_array_append(gen__ctx->gen_Lexer_Tokens, c.token);
				continue;
			}
		}

		c.token.Text.Len = 0;

		skip_whitespace();
		if (c.left <= 0)
			break;

		switch ((*ctx->scanner))
		{
			case '#':
			{
				gen_s32 result = gen_lex_preprocessor_directive(ctx);
				switch (result)
				{
					case Lex_Continue:
					{
						//gen_TokType last_type = Tokens[gen_array_get_header(Tokens)->Num - 2].Type;
						//if ( last_type == Tok_Preprocess_Pragma )
						{
							{
								gen_Token thanks_c = {
									{ c.scanner, 0 },
									Tok_Invalid,
									c.line,
									c.column,
									TF_Null
								};
								c.token = thanks_c;
							}
							if ((*ctx->scanner) == '\r')
							{
								move_forward();
								c.token.Text.Len = 1;
							}

							if ((*ctx->scanner) == '\n')
							{
								c.token.Type = Tok_NewLine;
								c.token.Text.Len++;
								move_forward();

								gen_array_append(gen__ctx->gen_Lexer_Tokens, c.token);
							}
						}
						continue;
					}

					case Lex_ReturnNull:
					{
						gen_TokArray gen_tok_array = {};
						return gen_tok_array;
					}
				}
			}
			case '.':
			{
				gen_Str text  = { c.scanner, 1 };
				c.token.Text  = text;
				c.token.Type  = Tok_Access_MemberSymbol;
				c.token.Flags = TF_AccessOperator;

				if (c.left)
				{
					move_forward();
				}

				if ((*ctx->scanner) == '.')
				{
					move_forward();
					if ((*ctx->scanner) == '.')
					{
						c.token.Text.Len = 3;
						c.token.Type     = Tok_Varadic_Argument;
						c.token.Flags    = TF_Null;
						move_forward();
					}
					else
					{
						gen_StrBuilder context_str = gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "%s", c.scanner, gen_min(100, c.left));

						gen_log_failure(
						    "gen::gen_lex: invalid varadic argument, expected '...' got '..%c' (%d, %d)\n%s",
						    (*ctx->scanner),
						    c.line,
						    c.column,
						    context_str
						);
					}
				}

				goto FoundToken;
			}
			case '&':
			{
				gen_Str text   = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Ampersand;
				c.token.Flags |= TF_Operator;
				c.token.Flags |= TF_Specifier;

				if (c.left)
					move_forward();

				if ((*ctx->scanner) == '&')    // &&
				{
					c.token.Text.Len = 2;
					c.token.Type     = Tok_Ampersand_DBL;

					if (c.left)
						move_forward();
				}

				goto FoundToken;
			}
			case ':':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Assign_Classifer;
				// Can be either a classifier (ParentType, Bitfield width), or ternary else
				// token.Type   = Tok_Colon;

				if (c.left)
					move_forward();

				if ((*ctx->scanner) == ':')
				{
					move_forward();
					c.token.Type = Tok_Access_StaticSymbol;
					c.token.Text.Len++;
				}
				goto FoundToken;
			}
			case '{':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_BraceCurly_Open;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '}':
			{
				gen_Str text  = { c.scanner, 1 };
				c.token.Text  = text;
				c.token.Type  = Tok_BraceCurly_Close;
				c.token.Flags = TF_EndDefinition;

				if (c.left)
					move_forward();

				gen_end_line();
				goto FoundToken;
			}
			case '[':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_BraceSquare_Open;
				if (c.left)
				{
					move_forward();

					if ((*ctx->scanner) == ']')
					{
						c.token.Text.Len = 2;
						c.token.Type     = Tok_Operator;
						move_forward();
					}
				}
				goto FoundToken;
			}
			case ']':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_BraceSquare_Close;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '(':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Paren_Open;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case ')':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Paren_Close;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '\'':
			{
				gen_Str text  = { c.scanner, 1 };
				c.token.Text  = text;
				c.token.Type  = Tok_Char;
				c.token.Flags = TF_Literal;

				move_forward();

				if (c.left && (*ctx->scanner) == '\\')
				{
					move_forward();
					c.token.Text.Len++;

					if ((*ctx->scanner) == '\'')
					{
						move_forward();
						c.token.Text.Len++;
					}
				}

				while (c.left && (*ctx->scanner) != '\'')
				{
					move_forward();
					c.token.Text.Len++;
				}

				if (c.left)
				{
					move_forward();
					c.token.Text.Len++;
				}
				goto FoundToken;
			}
			case ',':
			{
				gen_Str text  = { c.scanner, 1 };
				c.token.Text  = text;
				c.token.Type  = Tok_Comma;
				c.token.Flags = TF_Operator;

				if (c.left)
					move_forward();
				goto FoundToken;
			}
			case '*':
			{
				gen_Str text   = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_Star;
				c.token.Flags |= TF_Specifier;
				c.token.Flags |= TF_Operator;

				if (c.left)
					move_forward();

				if ((*ctx->scanner) == '=')
				{
					c.token.Text.Len++;
					c.token.Flags |= TF_Assign;
					// c.token.Type = Tok_Assign_Multiply;

					if (c.left)
						move_forward();
				}

				goto FoundToken;
			}
			case ';':
			{
				gen_Str text  = { c.scanner, 1 };
				c.token.Text  = text;
				c.token.Type  = Tok_Statement_End;
				c.token.Flags = TF_EndDefinition;

				if (c.left)
					move_forward();

				gen_end_line();
				goto FoundToken;
			}
			case '"':
			{
				gen_Str text   = { c.scanner, 1 };
				c.token.Text   = text;
				c.token.Type   = Tok_String;
				c.token.Flags |= TF_Literal;

				move_forward();
				while (c.left)
				{
					if ((*ctx->scanner) == '"')
					{
						move_forward();
						break;
					}

					if ((*ctx->scanner) == '\\')
					{
						move_forward();
						c.token.Text.Len++;

						if (c.left)
						{
							move_forward();
							c.token.Text.Len++;
						}
						continue;
					}

					move_forward();
					c.token.Text.Len++;
				}
				goto FoundToken;
			}
			case '?':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Operator;
				// c.token.Type     = Tok_Ternary;
				c.token.Flags = TF_Operator;

				if (c.left)
					move_forward();

				goto FoundToken;
			}
			case '=':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Operator;
				// c.token.Type     = Tok_Assign;
				c.token.Flags  = TF_Operator;
				c.token.Flags |= TF_Assign;

				if (c.left)
					move_forward();

				if ((*ctx->scanner) == '=')
				{
					c.token.Text.Len++;
					c.token.Flags = TF_Operator;

					if (c.left)
						move_forward();
				}

				goto FoundToken;
			}
			case '+':
			{
				// c.token.Type = Tok_Add
			}
			case '%':
			{
				// c.token.Type = Tok_Modulo;
			}
			case '^':
			{
				// c.token.Type = Tok_B_XOr;
			}
			case '~':
			{
				// c.token.Type = Tok_Unary_Not;
			}
			case '!':
			{
				// c.token.Type = Tok_L_Not;
			}
			case '<':
			{
				// c.token.Type = Tok_Lesser;
			}
			case '>':
			{
				// c.token.Type = Tok_Greater;
			}
			case '|':
			{
				gen_Str text  = { c.scanner, 1 };
				c.token.Text  = text;
				c.token.Type  = Tok_Operator;
				c.token.Flags = TF_Operator;
				// token.Type   = Tok_L_Or;

				if (c.left)
					move_forward();

				if ((*ctx->scanner) == '=')
				{
					c.token.Text.Len++;
					c.token.Flags |= TF_Assign;
					// token.Flags |= TokFlags::Assignment;
					// token.Type = Tok_Assign_L_Or;

					if (c.left)
						move_forward();
				}
				else
					while (c.left && (*ctx->scanner) == *(c.scanner - 1) && c.token.Text.Len < 3)
					{
						c.token.Text.Len++;

						if (c.left)
							move_forward();
					}
				goto FoundToken;
			}

			// Dash is unfortunately a gen_bit more complicated...
			case '-':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Operator;
				// token.Type = Tok_Subtract;
				c.token.Flags = TF_Operator;
				if (c.left)
				{
					move_forward();

					if ((*ctx->scanner) == '>')
					{
						c.token.Text.Len++;
						//						token.Type = Tok_Access_PointerToMemberSymbol;
						c.token.Flags |= TF_AccessOperator;
						move_forward();

						if ((*ctx->scanner) == '*')
						{
							//							token.Type = Tok_Access_PointerToMemberOfPointerSymbol;
							c.token.Text.Len++;
							move_forward();
						}
					}
					else if ((*ctx->scanner) == '=')
					{
						c.token.Text.Len++;
						// token.Type = Tok_Assign_Subtract;
						c.token.Flags |= TF_Assign;

						if (c.left)
							move_forward();
					}
					else
						while (c.left && (*ctx->scanner) == *(c.scanner - 1) && c.token.Text.Len < 3)
						{
							c.token.Text.Len++;

							if (c.left)
								move_forward();
						}
				}
				goto FoundToken;
			}
			case '/':
			{
				gen_Str text = { c.scanner, 1 };
				c.token.Text = text;
				c.token.Type = Tok_Operator;
				// token.Type   = Tok_Divide;
				c.token.Flags = TF_Operator;
				move_forward();

				if (c.left)
				{
					if ((*ctx->scanner) == '=')
					{
						// token.Type = TokeType::Assign_Divide;
						move_forward();
						c.token.Text.Len++;
						c.token.Flags = TF_Assign;
					}
					else if ((*ctx->scanner) == '/')
					{
						c.token.Type     = Tok_Comment;
						c.token.Text.Len = 2;
						c.token.Flags    = TF_Null;
						move_forward();

						while (c.left && (*ctx->scanner) != '\n' && (*ctx->scanner) != '\r')
						{
							move_forward();
							c.token.Text.Len++;
						}

						if ((*ctx->scanner) == '\r')
						{
							move_forward();
							c.token.Text.Len++;
						}
						if ((*ctx->scanner) == '\n')
						{
							move_forward();
							c.token.Text.Len++;
						}
						gen_array_append(gen__ctx->gen_Lexer_Tokens, c.token);
						continue;
					}
					else if ((*ctx->scanner) == '*')
					{
						c.token.Type     = Tok_Comment;
						c.token.Text.Len = 2;
						c.token.Flags    = TF_Null;
						move_forward();

						bool star   = (*ctx->scanner) == '*';
						bool slash  = c.scanner[1] == '/';
						bool at_end = star && slash;
						while (c.left && ! at_end)
						{
							move_forward();
							c.token.Text.Len++;

							star   = (*ctx->scanner) == '*';
							slash  = c.scanner[1] == '/';
							at_end = star && slash;
						}
						c.token.Text.Len += 2;
						move_forward();
						move_forward();

						if ((*ctx->scanner) == '\r')
						{
							move_forward();
							c.token.Text.Len++;
						}
						if ((*ctx->scanner) == '\n')
						{
							move_forward();
							c.token.Text.Len++;
						}
						gen_array_append(gen__ctx->gen_Lexer_Tokens, c.token);
						// gen_end_line();
						continue;
					}
				}
				goto FoundToken;
			}
		}

		if (gen_char_is_alpha((*ctx->scanner)) || (*ctx->scanner) == '_')
		{
			gen_Str text = { c.scanner, 1 };
			c.token.Text = text;
			move_forward();

			while (c.left && (gen_char_is_alphanumeric((*ctx->scanner)) || (*ctx->scanner) == '_'))
			{
				move_forward();
				c.token.Text.Len++;
			}

			goto FoundToken;
		}
		else if (gen_char_is_digit((*ctx->scanner)))
		{
			// This is a very brute force gen_lex, no checks are done for validity of literal.

			gen_Str text  = { c.scanner, 1 };
			c.token.Text  = text;
			c.token.Type  = Tok_Number;
			c.token.Flags = TF_Literal;
			move_forward();

			if (c.left
			    && ((*ctx->scanner) == 'x' || (*ctx->scanner) == 'X' || (*ctx->scanner) == 'b' || (*ctx->scanner) == 'B' || (*ctx->scanner) == 'o'
			        || (*ctx->scanner) == 'O'))
			{
				move_forward();
				c.token.Text.Len++;

				while (c.left && gen_char_is_hex_digit((*ctx->scanner)))
				{
					move_forward();
					c.token.Text.Len++;
				}

				goto FoundToken;
			}

			while (c.left && gen_char_is_digit((*ctx->scanner)))
			{
				move_forward();
				c.token.Text.Len++;
			}

			if (c.left && (*ctx->scanner) == '.')
			{
				move_forward();
				c.token.Text.Len++;

				while (c.left && gen_char_is_digit((*ctx->scanner)))
				{
					move_forward();
					c.token.Text.Len++;
				}

				// Handle number literal suffixes in a botched way
				if (c.left
				    && ((*ctx->scanner) == 'l' || (*ctx->scanner) == 'L' ||    // long/long long
				        (*ctx->scanner) == 'u' || (*ctx->scanner) == 'U' ||    // unsigned
				        (*ctx->scanner) == 'f' || (*ctx->scanner) == 'F' ||    // float
				        (*ctx->scanner) == 'i' || (*ctx->scanner) == 'I' ||    // imaginary
				        (*ctx->scanner) == 'z' || (*ctx->scanner) == 'Z'))     // complex
				{
					char prev = (*ctx->scanner);
					move_forward();
					c.token.Text.Len++;

					// Handle 'll'/'LL' as a special case when we just processed an 'l'/'L'
					if (c.left && (prev == 'l' || prev == 'L') && ((*ctx->scanner) == 'l' || (*ctx->scanner) == 'L'))
					{
						move_forward();
						c.token.Text.Len++;
					}
				}
			}

			goto FoundToken;
		}
		else
		{
			gen_s32 start = gen_max(0, gen_array_num(gen__ctx->gen_Lexer_Tokens) - 100);
			gen_log_fmt("\n%d\n", start);
			for (gen_s32 idx = start; idx < gen_array_num(gen__ctx->gen_Lexer_Tokens); idx++)
			{
				gen_log_fmt(
				    "gen_Token %d Type: %s : %.*s\n",
				    idx,
				    gen_toktype_to_str(gen__ctx->gen_Lexer_Tokens[idx].Type).Ptr,
				    gen__ctx->gen_Lexer_Tokens[idx].Text.Len,
				    gen__ctx->gen_Lexer_Tokens[idx].Text.Ptr
				);
			}

			gen_StrBuilder context_str = gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "%.*s", gen_min(100, c.left), c.scanner);
			gen_log_failure("Failed to gen_lex token '%c' (%d, %d)\n%s", (*ctx->scanner), c.line, c.column, context_str);

			// Skip to next whitespace since we can't know if anything else is valid until then.
			while (c.left && ! gen_char_is_space((*ctx->scanner)))
			{
				move_forward();
			}
		}

	FoundToken:
	{
		gen_lex_found_token(ctx);
		gen_TokType last_type = gen_array_back(gen__ctx->gen_Lexer_Tokens)->Type;
		if (last_type == Tok_Preprocess_Macro_Stmt || last_type == Tok_Preprocess_Macro_Expr)
		{
			gen_Token thanks_c = {
				{ c.scanner, 0 },
				Tok_Invalid,
				c.line,
				c.column,
				TF_Null
			};
			c.token = thanks_c;
			if ((*ctx->scanner) == '\r')
			{
				move_forward();
				c.token.Text.Len = 1;
			}
			if ((*ctx->scanner) == '\n')
			{
				c.token.Type = Tok_NewLine;
				c.token.Text.Len++;
				move_forward();

				gen_array_append(gen__ctx->gen_Lexer_Tokens, c.token);
				continue;
			}
		}
	}
	}

	if (gen_array_num(gen__ctx->gen_Lexer_Tokens) == 0)
	{
		gen_log_failure("Failed to gen_lex any tokens");
		gen_TokArray gen_tok_array = {};
		return gen_tok_array;
	}

	gen_TokArray result = { gen__ctx->gen_Lexer_Tokens, 0 };
	return result;
}

#undef move_forward
#undef skip_whitespace
#undef gen_end_line

#pragma region gen_Array_gen_CodeTypename

#define GEN_GENERIC_SLOT_10__array_init            gen_CodeTypename, gen_Array_gen_CodeTypename_init
#define GEN_GENERIC_SLOT_10__array_init_reserve    gen_CodeTypename, gen_Array_gen_CodeTypename_init_reserve
#define GEN_GENERIC_SLOT_10__array_append          gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_append
#define GEN_GENERIC_SLOT_10__array_append_items    gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_append_items
#define GEN_GENERIC_SLOT_10__array_append_at       gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_append_at
#define GEN_GENERIC_SLOT_10__array_append_items_at gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_append_items_at
#define GEN_GENERIC_SLOT_10__array_back            gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_back
#define GEN_GENERIC_SLOT_10__array_clear           gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_clear
#define GEN_GENERIC_SLOT_10__array_fill            gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_fill
#define GEN_GENERIC_SLOT_10__array_free            gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_free
#define GEN_GENERIC_SLOT_10__array_grow            gen_Array_gen_CodeTypename*, gen_Array_gen_CodeTypename_grow
#define GEN_GENERIC_SLOT_10__array_num             gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_num
#define GEN_GENERIC_SLOT_10__array_pop             gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_pop
#define GEN_GENERIC_SLOT_10__array_remove_at       gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_remove_at
#define GEN_GENERIC_SLOT_10__array_reserve         gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_reserve
#define GEN_GENERIC_SLOT_10__array_resize          gen_Array_gen_CodeTypename, gen_Array_gen_CodeTypename_resize
#define GEN_GENERIC_SLOT_10__array_set_capacity    gen_Array_gen_CodeTypename*, gen_Array_gen_CodeTypename_set_capacity

typedef gen_CodeTypename*  gen_Array_gen_CodeTypename;
gen_Array_gen_CodeTypename gen_Array_gen_CodeTypename_init(gen_AllocatorInfo allocator);
gen_Array_gen_CodeTypename gen_Array_gen_CodeTypename_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity);
bool                       gen_Array_gen_CodeTypename_append_array(gen_Array_gen_CodeTypename* self, gen_Array_gen_CodeTypename other);
bool                       gen_Array_gen_CodeTypename_append(gen_Array_gen_CodeTypename* self, gen_CodeTypename value);
bool                       gen_Array_gen_CodeTypename_append_items(gen_Array_gen_CodeTypename* self, gen_CodeTypename* items, gen_usize item_num);
bool                       gen_Array_gen_CodeTypename_append_at(gen_Array_gen_CodeTypename* self, gen_CodeTypename item, gen_usize idx);
bool              gen_Array_gen_CodeTypename_append_items_at(gen_Array_gen_CodeTypename* self, gen_CodeTypename* items, gen_usize item_num, gen_usize idx);
gen_CodeTypename* gen_Array_gen_CodeTypename_back(gen_Array_gen_CodeTypename self);
void              gen_Array_gen_CodeTypename_clear(gen_Array_gen_CodeTypename self);
bool              gen_Array_gen_CodeTypename_fill(gen_Array_gen_CodeTypename self, gen_usize begin, gen_usize end, gen_CodeTypename value);
void              gen_Array_gen_CodeTypename_free(gen_Array_gen_CodeTypename* self);
bool              gen_Array_gen_CodeTypename_grow(gen_Array_gen_CodeTypename* self, gen_usize min_capacity);
gen_usize         gen_Array_gen_CodeTypename_num(gen_Array_gen_CodeTypename self);
gen_CodeTypename  gen_Array_gen_CodeTypename_pop(gen_Array_gen_CodeTypename self);
void              gen_Array_gen_CodeTypename_remove_at(gen_Array_gen_CodeTypename self, gen_usize idx);
bool              gen_Array_gen_CodeTypename_reserve(gen_Array_gen_CodeTypename* self, gen_usize new_capacity);
bool              gen_Array_gen_CodeTypename_resize(gen_Array_gen_CodeTypename* self, gen_usize num);
bool              gen_Array_gen_CodeTypename_set_capacity(gen_Array_gen_CodeTypename* self, gen_usize new_capacity);

gen_forceinline gen_Array_gen_CodeTypename gen_Array_gen_CodeTypename_init(gen_AllocatorInfo allocator)
{
	size_t initial_size = gen_array_grow_formula(0);
	return gen_array_init_reserve(gen_CodeTypename, allocator, initial_size);
}

inline gen_Array_gen_CodeTypename gen_Array_gen_CodeTypename_init_reserve(gen_AllocatorInfo allocator, gen_usize capacity)
{
	GEN_ASSERT(capacity > 0);
	gen_ArrayHeader* header = gen_rcast(gen_ArrayHeader*, gen_alloc(allocator, sizeof(gen_ArrayHeader) + sizeof(gen_CodeTypename) * capacity));
	if (header == gen_nullptr)
		return gen_nullptr;
	header->Allocator = allocator;
	header->Capacity  = capacity;
	header->Num       = 0;
	return gen_rcast(gen_CodeTypename*, header + 1);
}

gen_forceinline bool gen_Array_gen_CodeTypename_append_array(gen_Array_gen_CodeTypename* self, gen_Array_gen_CodeTypename other)
{
	return gen_array_append_items(*self, (gen_Array_gen_CodeTypename)other, gen_Array_gen_CodeTypename_num(other));
}

inline bool gen_Array_gen_CodeTypename_append(gen_Array_gen_CodeTypename* self, gen_CodeTypename value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num == header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	(*self)[header->Num] = value;
	header->Num++;
	return true;
}

inline bool gen_Array_gen_CodeTypename_append_items(gen_Array_gen_CodeTypename* self, gen_CodeTypename* items, gen_usize item_num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(items != gen_nullptr);
	GEN_ASSERT(item_num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Num + item_num > header->Capacity)
	{
		if (! gen_array_grow(self, header->Capacity + item_num))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_mem_copy((*self) + header->Num, items, sizeof(gen_CodeTypename) * item_num);
	header->Num += item_num;
	return true;
}

inline bool gen_Array_gen_CodeTypename_append_at(gen_Array_gen_CodeTypename* self, gen_CodeTypename item, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
		idx = header->Num - 1;
	if (idx < 0)
		idx = 0;
	if (header->Capacity < header->Num + 1)
	{
		if (! gen_array_grow(self, header->Capacity + 1))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_Array_gen_CodeTypename target = (*self) + idx;
	gen_mem_move(target + 1, target, (header->Num - idx) * sizeof(gen_CodeTypename));
	header->Num++;
	return true;
}

inline bool gen_Array_gen_CodeTypename_append_items_at(gen_Array_gen_CodeTypename* self, gen_CodeTypename* items, gen_usize item_num, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (idx >= header->Num)
	{
		return gen_array_append_items(*self, items, item_num);
	}
	if (item_num > header->Capacity)
	{
		if (! gen_array_grow(self, item_num + header->Capacity))
			return false;
		header = gen_array_get_header(*self);
	}
	gen_CodeTypename* target = (*self) + idx + item_num;
	gen_CodeTypename* src    = (*self) + idx;
	gen_mem_move(target, src, (header->Num - idx) * sizeof(gen_CodeTypename));
	gen_mem_copy(src, items, item_num * sizeof(gen_CodeTypename));
	header->Num += item_num;
	return true;
}

inline gen_CodeTypename* gen_Array_gen_CodeTypename_back(gen_Array_gen_CodeTypename self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (header->Num == 0)
		return 0;
	return self + header->Num - 1;
}

inline void gen_Array_gen_CodeTypename_clear(gen_Array_gen_CodeTypename self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	header->Num             = 0;
}

inline bool gen_Array_gen_CodeTypename_fill(gen_Array_gen_CodeTypename self, gen_usize begin, gen_usize end, gen_CodeTypename value)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(begin <= end);
	gen_ArrayHeader* header = gen_array_get_header(self);
	if (begin < 0 || end > header->Num)
		return false;
	for (gen_ssize idx = (gen_ssize)begin; idx < (gen_ssize)end; idx++)
		self[idx] = value;
	return true;
}

inline void gen_Array_gen_CodeTypename_free(gen_Array_gen_CodeTypename* self)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	gen_allocator_free(header->Allocator, header);
	self = 0;
}

inline bool gen_Array_gen_CodeTypename_grow(gen_Array_gen_CodeTypename* self, gen_usize min_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(min_capacity > 0);
	gen_ArrayHeader* header       = gen_array_get_header(*self);
	gen_usize        new_capacity = gen_array_grow_formula(header->Capacity);
	if (new_capacity < min_capacity)
		new_capacity = min_capacity;
	return gen_array_set_capacity(self, new_capacity);
}

gen_forceinline gen_usize gen_Array_gen_CodeTypename_num(gen_Array_gen_CodeTypename self)
{
	GEN_ASSERT(self != gen_nullptr);
	return gen_array_get_header(self)->Num;
}

inline gen_CodeTypename gen_Array_gen_CodeTypename_pop(gen_Array_gen_CodeTypename self)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(header->Num > 0);
	gen_CodeTypename result = self[header->Num - 1];
	header->Num--;
	return result;
}

gen_forceinline void gen_Array_gen_CodeTypename_remove_at(gen_Array_gen_CodeTypename self, gen_usize idx)
{
	GEN_ASSERT(self != gen_nullptr);
	gen_ArrayHeader* header = gen_array_get_header(self);
	GEN_ASSERT(idx < header->Num);
	gen_mem_move(self + idx, self + idx + 1, sizeof(gen_CodeTypename) * (header->Num - idx - 1));
	header->Num--;
}

inline bool gen_Array_gen_CodeTypename_reserve(gen_Array_gen_CodeTypename* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < new_capacity)
		return gen_array_set_capacity(self, new_capacity);
	return true;
}

inline bool gen_Array_gen_CodeTypename_resize(gen_Array_gen_CodeTypename* self, gen_usize num)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(num > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (header->Capacity < num)
	{
		if (! gen_array_grow(self, num))
			return false;
		header = gen_array_get_header(*self);
	}
	header->Num = num;
	return true;
}

inline bool gen_Array_gen_CodeTypename_set_capacity(gen_Array_gen_CodeTypename* self, gen_usize new_capacity)
{
	GEN_ASSERT(self != gen_nullptr);
	GEN_ASSERT(*self != gen_nullptr);
	GEN_ASSERT(new_capacity > 0);
	gen_ArrayHeader* header = gen_array_get_header(*self);
	if (new_capacity == header->Capacity)
		return true;
	if (new_capacity < header->Num)
	{
		header->Num = new_capacity;
		return true;
	}
	gen_usize        size       = sizeof(gen_ArrayHeader) + sizeof(gen_CodeTypename) * new_capacity;
	gen_ArrayHeader* new_header = gen_cast(gen_ArrayHeader*, gen_alloc(header->Allocator, size));
	if (new_header == 0)
		return false;
	gen_mem_move(new_header, header, sizeof(gen_ArrayHeader) + sizeof(gen_CodeTypename) * header->Num);
	new_header->Capacity = new_capacity;
	gen_allocator_free(header->Allocator, &header);
	*self = gen_cast(gen_CodeTypename*, new_header + 1);
	return true;
}

#pragma endregion gen_Array_gen_CodeTypename

// These macros are used in the swtich cases within parser.cpp

#define GEN_PARSER_CLASS_STRUCT_BODY_ALLOWED_MEMBER_TOK_SPECIFIER_CASES \
case Tok_Spec_Consteval:   \
case Tok_Spec_Constexpr:   \
case Tok_Spec_Constinit:   \
case Tok_Spec_Explicit:    \
case Tok_Spec_ForceInline: \
case Tok_Spec_Inline:      \
case Tok_Spec_Mutable:     \
case Tok_Spec_NeverInline: \
case Tok_Spec_Static:      \
case Tok_Spec_Volatile:    \
case Tok_Spec_Virtual

#define GEN_PARSER_CLASS_STRUCT_BODY_ALLOWED_MEMBER_SPECIFIER_CASES \
case Spec_Constexpr:   \
case Spec_Constinit:   \
case Spec_Explicit:    \
case Spec_Inline:      \
case Spec_ForceInline: \
case Spec_Mutable:     \
case Spec_NeverInline: \
case Spec_Static:      \
case Spec_Volatile:    \
case Spec_Virtual

#define GEN_PARSER_CLASS_GLOBAL_NSPACE_ALLOWED_MEMBER_TOK_SPECIFIER_CASES \
case Tok_Spec_Consteval:        \
case Tok_Spec_Constexpr:        \
case Tok_Spec_Constinit:        \
case Tok_Spec_Extern:           \
case Tok_Spec_ForceInline:      \
case Tok_Spec_Global:           \
case Tok_Spec_Inline:           \
case Tok_Spec_Internal_Linkage: \
case Tok_Spec_NeverInline:      \
case Tok_Spec_Static

#define GEN_PARSER_CLASS_GLOBAL_NSPACE_ALLOWED_MEMBER_SPECIFIER_CASES \
case Spec_Constexpr:        \
case Spec_Constinit:        \
case Spec_ForceInline:      \
case Spec_Global:           \
case Spec_External_Linkage: \
case Spec_Internal_Linkage: \
case Spec_Inline:           \
case Spec_Mutable:          \
case Spec_NeverInline:      \
case Spec_Static:           \
case Spec_Volatile

#define GEN_PARSER_FRIEND_ALLOWED_SPECIFIER_CASES \
case Spec_Const:       \
case Spec_Inline:      \
case Spec_ForceInline

#define GEN_PARSER_FUNCTION_ALLOWED_SPECIFIER_CASES \
case Spec_Const:            \
case Spec_Consteval:        \
case Spec_Constexpr:        \
case Spec_External_Linkage: \
case Spec_Internal_Linkage: \
case Spec_ForceInline:      \
case Spec_Inline:           \
case Spec_NeverInline:      \
case Spec_Static

#define GEN_PARSER_OPERATOR_ALLOWED_SPECIFIER_CASES \
case Spec_Const:       \
case Spec_Constexpr:   \
case Spec_ForceInline: \
case Spec_Inline:      \
case Spec_NeverInline: \
case Spec_Static

#define GEN_PARSER_TEMPLATE_ALLOWED_SPECIFIER_CASES \
case Spec_Const:            \
case Spec_Constexpr:        \
case Spec_Constinit:        \
case Spec_External_Linkage: \
case Spec_Global:           \
case Spec_Inline:           \
case Spec_ForceInline:      \
case Spec_Local_Persist:    \
case Spec_Mutable:          \
case Spec_Static:           \
case Spec_Thread_Local:     \
case Spec_Volatile

#define GEN_PARSER_VARIABLE_ALLOWED_SPECIFIER_CASES \
case Spec_Const:            \
case Spec_Constexpr:        \
case Spec_Constinit:        \
case Spec_External_Linkage: \
case Spec_Global:           \
case Spec_Inline:           \
case Spec_Local_Persist:    \
case Spec_Mutable:          \
case Spec_Restrict:         \
case Spec_Static:           \
case Spec_Thread_Local:     \
case Spec_Volatile

#define GEN_PARSER_TYPENAME_ALLOWED_SUFFIX_SPECIFIER_CASES \
case Spec_Const:    \
case Spec_Ptr:      \
case Spec_Restrict: \
case Spec_Ref:      \
case Spec_RValue



// TODO(Ed) : Rename ETok_Capture_Start, ETok_Capture_End to Open_Parenthesis adn Close_Parenthesis

#define gen_lex_dont_skip_formatting (bool)false
#define gen_lex_skip_formatting      (bool)true

void gen_parser_push(gen_ParseContext* ctx, gen_StackNode* node)
{
	node->Prev = ctx->Scope;
	ctx->Scope = node;

#if 0 && GEN_BUILD_DEBUG
	gen_log_fmt("\tEntering gen__ctx->parser: %.*s\n", Scope->ProcName.Len, Scope->ProcName.Ptr );
#endif
}

void gen_parser_pop(gen_ParseContext* ctx)
{
#if 0 && GEN_BUILD_DEBUG
	gen_log_fmt("\tPopping  gen__ctx->parser: %.*s\n", Scope->ProcName.Len, Scope->ProcName.Ptr );
#endif
	ctx->Scope = ctx->Scope->Prev;
}

gen_StrBuilder gen_parser_to_strbuilder(gen_ParseContext ctx)
{
	gen_StrBuilder result = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, gen_kilobytes(4));

	gen_Token scope_start = *ctx.Scope->Start;
	gen_Token last_valid =
	    ctx.Tokens.Idx >= gen_array_num(ctx.Tokens.Arr) ? ctx.Tokens.Arr[gen_array_num(ctx.Tokens.Arr) - 1] : (*gen_lex_current(&ctx.Tokens, true));

	gen_sptr    length  = scope_start.Text.Len;
	char const* current = scope_start.Text.Ptr + length;
	while (current <= gen_array_back(ctx.Tokens.Arr)->Text.Ptr && (*current) != '\n' && length < 74)
	{
		current++;
		length++;
	}

	gen_Str        scope_str = { scope_start.Text.Ptr, length };
	gen_StrBuilder line      = gen_strbuilder_make_str(gen__ctx->Allocator_Temp, scope_str);
	gen_strbuilder_append_fmt(&result, "\tScope    : %s\n", line);
	gen_strbuilder_free(&line);

	gen_sptr dist                = (gen_sptr)last_valid.Text.Ptr - (gen_sptr)scope_start.Text.Ptr + 2;
	gen_sptr length_from_err     = dist;

	gen_Str        err_str       = { last_valid.Text.Ptr, length_from_err };
	gen_StrBuilder line_from_err = gen_strbuilder_make_str(gen__ctx->Allocator_Temp, err_str);

	if (length_from_err < 100)
		gen_strbuilder_append_fmt(&result, "\t(%d, %d):%*c\n", last_valid.Line, last_valid.Column, length_from_err, '^');
	else
		gen_strbuilder_append_fmt(&result, "\t(%d, %d)\n", last_valid.Line, last_valid.Column);

	gen_StackNode* curr_scope = ctx.Scope;
	gen_s32        level      = 0;
	do
	{
		if (curr_scope->Name.Ptr)
		{
			gen_strbuilder_append_fmt(&result, "\t%d: %s, gen_AST Name: %.*s\n", level, curr_scope->ProcName.Ptr, curr_scope->Name.Len, curr_scope->Name.Ptr);
		}
		else
		{
			gen_strbuilder_append_fmt(&result, "\t%d: %s\n", level, curr_scope->ProcName.Ptr);
		}

		curr_scope = curr_scope->Prev;
		level++;
	} while (curr_scope);
	return result;
}

bool gen_lex__eat(gen_TokArray* self, gen_TokType type)
{
	if (gen_array_num(self->Arr) - self->Idx <= 0)
	{
		gen_log_failure("No tokens left.\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		return false;
	}

	gen_Token at_idx = self->Arr[self->Idx];

	if ((at_idx.Type == Tok_NewLine && type != Tok_NewLine) || (at_idx.Type == Tok_Comment && type != Tok_Comment))
	{
		self->Idx++;
	}

	gen_b32 not_accepted  = at_idx.Type != type;
	gen_b32 is_identifier = at_idx.Type == Tok_Identifier;
	if (not_accepted)
	{
		gen_Macro* macro                = lookup_macro(at_idx.Text);
		gen_b32    accept_as_identifier = macro && gen_bitfield_is_set(gen_MacroFlags, macro->Flags, MF_Allow_As_Identifier);
		not_accepted                    = type == Tok_Identifier && accept_as_identifier ? false : true;
	}
	if (not_accepted)
	{
		gen_Token tok = *gen_lex_current(self, gen_lex_skip_formatting);
		gen_log_failure(
		    "Parse Error, gen_TokArray::eat, Expected: ' %s ' not ' %.*s ' (%d, %d)`\n%s",
		    gen_toktype_to_str(type).Ptr,
		    at_idx.Text.Len,
		    at_idx.Text.Ptr,
		    tok.Line,
		    tok.Column,
		    gen_parser_to_strbuilder(gen__ctx->parser)
		);
		GEN_DEBUG_TRAP();
		return false;
	}

#if 0 && GEN_BUILD_DEBUG
	gen_log_fmt("Ate: %SB\n", self->Arr[Idx].to_strbuilder() );
#endif

	self->Idx++;
	return true;
}

gen_internal void gen_parser_init()
{
	gen__ctx->gen_Lexer_Tokens = gen_array_init_reserve(gen_Token, gen__ctx->Allocator_DyanmicContainers, gen__ctx->InitSize_LexerTokens);
}

gen_internal void gen_parser_deinit()
{
	gen_Array(gen_Token) null_array = { gen_nullptr };
	gen__ctx->gen_Lexer_Tokens      = null_array;
}

#pragma region Helper Macros


#define check_parse_args(def) _check_parse_args(def, gen_stringize(_func_))

bool _check_parse_args(gen_Str def, char const* func_name)
{
	if (def.Len <= 0)
	{
		gen_log_failure(gen_c_str_fmt_buf("gen::%s: length must greater than 0", func_name));
		gen_parser_pop(&gen__ctx->parser);
		return false;
	}
	if (def.Ptr == gen_nullptr)
	{
		gen_log_failure(gen_c_str_fmt_buf("gen::%s: def was null", func_name));
		gen_parser_pop(&gen__ctx->parser);
		return false;
	}
	return true;
}

#define currtok_noskip (*gen_lex_current(&gen__ctx->parser.Tokens, gen_lex_dont_skip_formatting))
#define currtok        (*gen_lex_current(&gen__ctx->parser.Tokens, gen_lex_skip_formatting))
#define peektok        (*gen_lex_peek(gen__ctx->parser.Tokens, gen_lex_skip_formatting))
#define prevtok        (*gen_lex_previous(gen__ctx->parser.Tokens, gen_lex_dont_skip_formatting))
#define nexttok        (*gen_lex_next(gen__ctx->parser.Tokens, gen_lex_skip_formatting))
#define nexttok_noskip (*gen_lex_next(gen__ctx->parser.Tokens, gen_lex_dont_skip_formatting))
#define eat(Type_)     gen_lex__eat(&gen__ctx->parser.Tokens, Type_)
#define left           (gen_array_num(gen__ctx->parser.Tokens.Arr) - gen__ctx->parser.Tokens.Idx)

#if GEN_COMPILER_CPP
#define gen_def_assign(...) \
	{                       \
		__VA_ARGS__         \
	}
#else
#define gen_def_assign(...) __VA_ARGS__
#endif


#ifdef check

#define CHECK_WAS_DEFINED

#pragma push_macro("check")

#undef check
#endif

#define check_noskip(Type_) (left && currtok_noskip.Type == Type_)
#define check(Type_)        (left && currtok.Type == Type_)

#define push_scope()                                                                                                                                  \
	gen_Str       null_name = {};                                                                                                                     \
	gen_StackNode scope     = { gen_nullptr, gen_lex_current(&gen__ctx->parser.Tokens, gen_lex_dont_skip_formatting), null_name, gen_txt(__func__) }; \
	gen_parser_push(&gen__ctx->parser, &scope)

#pragma endregion Helper Macros


// Procedure Forwards ( Entire parser gen_internal parser interface )

gen_internal gen_Code           gen_parse_array_decl();
gen_internal gen_CodeAttributes gen_parse_attributes();
gen_internal gen_CodeComment    gen_parse_comment();
gen_internal gen_Code           gen_parse_complicated_definition(gen_TokType which);
gen_internal gen_CodeBody       gen_parse_class_struct_body(gen_TokType which, gen_Token name);
gen_internal gen_Code           gen_parse_class_struct(gen_TokType which, bool inplace_def);
gen_internal gen_Code           gen_parse_expression();
gen_internal gen_Code           gen_parse_forward_or_definition(gen_TokType which, bool is_inplace);
gen_internal gen_CodeFn         gen_parse_function_after_name(
            gen_ModuleFlag     mflags,
            gen_CodeAttributes attributes,
            gen_CodeSpecifiers specifiers,
            gen_CodeTypename   ret_type,
            gen_Token          name
        );
gen_internal gen_Code        gen_parse_function_body();
gen_internal gen_CodeBody    gen_parse_global_nspace(gen_CodeType which);
gen_internal gen_Code        gen_parse_global_nspace_constructor_destructor(gen_CodeSpecifiers specifiers);
gen_internal gen_Token       gen_parse_identifier(bool* possible_member_function);
gen_internal gen_CodeInclude gen_parse_include();
gen_internal gen_Code        gen_parse_macro_as_definiton(gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers);
gen_internal gen_CodeOperator
    gen_parse_operator_after_ret_type(gen_ModuleFlag mflags, gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers, gen_CodeTypename ret_type);
gen_internal gen_Code       gen_parse_operator_function_or_variable(bool expects_function, gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers);
gen_internal gen_CodePragma gen_parse_pragma();
gen_internal gen_CodeParams gen_parse_params(bool use_template_capture);
gen_internal gen_CodePreprocessCond gen_parse_preprocess_cond();
gen_internal gen_Code               gen_parse_simple_preprocess(gen_TokType which);
gen_internal gen_Code               gen_parse_static_assert();
gen_internal void                   gen_parse_template_args(gen_Token* token);
gen_internal gen_CodeVar
    gen_parse_variable_after_name(gen_ModuleFlag mflags, gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers, gen_CodeTypename type, gen_Str name);
gen_internal gen_CodeVar gen_parse_variable_declaration_list();

gen_internal gen_CodeClass       gen_parser_parse_class(bool inplace_def);
gen_internal gen_CodeConstructor gen_parser_parse_constructor(gen_CodeSpecifiers specifiers);
gen_internal gen_CodeDefine      gen_parser_parse_define();
gen_internal gen_CodeDestructor  gen_parser_parse_destructor(gen_CodeSpecifiers specifiers);
gen_internal gen_CodeEnum        gen_parser_parse_enum(bool inplace_def);
gen_internal gen_CodeBody        gen_parser_parse_export_body();
gen_internal gen_CodeBody        gen_parser_parse_extern_link_body();
gen_internal gen_CodeExtern      gen_parser_parse_extern_link();
gen_internal gen_CodeFriend      gen_parser_parse_friend();
gen_internal gen_CodeFn          gen_parser_parse_function();
gen_internal gen_CodeNS          gen_parser_parse_namespace();
gen_internal gen_CodeOpCast      gen_parser_parse_operator_cast(gen_CodeSpecifiers specifiers);
gen_internal gen_CodeStruct      gen_parser_parse_struct(bool inplace_def);
gen_internal gen_CodeVar         gen_parser_parse_variable();
gen_internal gen_CodeTemplate    gen_parser_parse_template();
gen_internal gen_CodeTypename    gen_parser_parse_type(bool from_template, bool* is_function);
gen_internal gen_CodeTypedef     gen_parser_parse_typedef();
gen_internal gen_CodeUnion       gen_parser_parse_union(bool inplace_def);
gen_internal gen_CodeUsing       gen_parser_parse_using();

#define gen_parser_inplace_def         (bool)true
#define gen_parser_not_inplace_def     (bool)false
#define gen_parser_dont_consume_braces (bool)true
#define gen_parser_consume_braces      (bool)false
#define gen_parser_not_from_template   (bool)false

#define gen_parser_use_parenthesis     (bool)false

// Internal parsing functions

#define gen_parser_strip_formatting_dont_preserve_newlines (bool)false

/*
	This function was an attempt at stripping formatting from any c++ code.
	It has edge case failures that prevent it from being used in function bodies.
*/
gen_internal gen_StrBuilder gen_parser_strip_formatting(gen_Str raw_text, bool preserve_newlines)
{
	gen_StrBuilder content = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, raw_text.Len);

	if (raw_text.Len == 0)
		return content;

#define cut_length (scanner - raw_text.Ptr - last_cut)
#define cut_ptr    (raw_text.Ptr + last_cut)
#define pos        (gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr))
#define move_fwd() \
	do             \
	{              \
		scanner++; \
		tokleft--; \
	} while (0)

	gen_s32     tokleft  = raw_text.Len;
	gen_sptr    last_cut = 0;
	char const* scanner  = raw_text.Ptr;

	if (scanner[0] == ' ')
	{
		move_fwd();
		last_cut = 1;
	}

	bool within_string     = false;
	bool within_char       = false;
	bool must_keep_newline = false;
	while (tokleft)
	{
		// Skip over the content of string literals
		if (scanner[0] == '"')
		{
			move_fwd();

			while (tokleft && (scanner[0] != '"' || *(scanner - 1) == '\\'))
			{
				if (scanner[0] == '\\' && tokleft > 1)
				{
					scanner += 2;
					tokleft -= 2;
				}
				else
				{
					move_fwd();
				}
			}

			// Skip the closing "
			if (tokleft)
				move_fwd();

			gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);
			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		// Skip over the content of character literals
		if (scanner[0] == '\'')
		{
			move_fwd();

			while (tokleft && (scanner[0] != '\'' || (*(scanner - 1) == '\\')))
			{
				move_fwd();
			}

			// Skip the closing '
			if (tokleft)
				move_fwd();

			gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);
			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		// Block comments
		if (tokleft > 1 && scanner[0] == '/' && scanner[1] == '*')
		{
			while (tokleft > 1 && ! (scanner[0] == '*' && scanner[1] == '/'))
				move_fwd();

			scanner += 2;
			tokleft -= 2;

			gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);
			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		// Line comments
		if (tokleft > 1 && scanner[0] == '/' && scanner[1] == '/')
		{
			must_keep_newline  = true;

			scanner           += 2;
			tokleft           -= 2;

			while (tokleft && scanner[0] != '\n')
				move_fwd();

			if (tokleft)
				move_fwd();

			gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);
			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		// Tabs
		if (scanner[0] == '\t')
		{
			if (pos > last_cut)
				gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);

			if (*gen_strbuilder_back(content) != ' ')
				gen_strbuilder_append_char(&content, ' ');

			move_fwd();
			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		if (tokleft > 1 && scanner[0] == '\r' && scanner[1] == '\n')
		{
			if (must_keep_newline || preserve_newlines)
			{
				must_keep_newline  = false;

				scanner           += 2;
				tokleft           -= 2;

				gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);
				last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
				continue;
			}

			if (pos > last_cut)
				gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);

			// Replace with a space
			if (*gen_strbuilder_back(content) != ' ')
				gen_strbuilder_append_char(&content, ' ');

			scanner  += 2;
			tokleft  -= 2;

			last_cut  = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		if (scanner[0] == '\n')
		{
			if (must_keep_newline || preserve_newlines)
			{
				must_keep_newline = false;

				move_fwd();

				gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);
				last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
				continue;
			}

			if (pos > last_cut)
				gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);

			// Replace with a space
			if (*gen_strbuilder_back(content) != ' ')
				gen_strbuilder_append_char(&content, ' ');

			move_fwd();

			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		// Escaped newlines
		if (scanner[0] == '\\')
		{
			gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);

			gen_s32 amount_to_skip = 1;
			if (tokleft > 1 && scanner[1] == '\n')
			{
				amount_to_skip = 2;
			}
			else if (tokleft > 2 && scanner[1] == '\r' && scanner[2] == '\n')
			{
				amount_to_skip = 3;
			}

			if (amount_to_skip > 1 && pos == last_cut)
			{
				scanner += amount_to_skip;
				tokleft -= amount_to_skip;
			}
			else
				move_fwd();

			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);
			continue;
		}

		// Consectuive spaces
		if (tokleft > 1 && gen_char_is_space(scanner[0]) && gen_char_is_space(scanner[1]))
		{
			gen_strbuilder_append_c_str_len(&content, cut_ptr, cut_length);
			do
			{
				move_fwd();
			} while (tokleft && gen_char_is_space(scanner[0]));

			last_cut = gen_rcast(gen_sptr, scanner) - gen_rcast(gen_sptr, raw_text.Ptr);

			// Preserve only 1 space of formattting
			char* last = gen_strbuilder_back(content);
			if (last == gen_nullptr || *last != ' ')
				gen_strbuilder_append_char(&content, ' ');

			continue;
		}

		move_fwd();
	}

	if (last_cut < raw_text.Len)
	{
		gen_strbuilder_append_c_str_len(&content, cut_ptr, raw_text.Len - last_cut);
	}

#undef cut_ptr
#undef cut_length
#undef pos
#undef move_fwd

	return content;
}

gen_internal gen_Code gen_parse_array_decl()
{
	push_scope();

	if (check(Tok_Operator) && currtok.Text.Ptr[0] == '[' && currtok.Text.Ptr[1] == ']')
	{
		gen_Code gen_array_expr = gen_untyped_str(gen_txt(" "));
		eat(Tok_Operator);
		// []

		gen_parser_pop(&gen__ctx->parser);
		return gen_array_expr;
	}

	if (check(Tok_BraceSquare_Open))
	{
		eat(Tok_BraceSquare_Open);
		// [

		if (left == 0)
		{
			gen_log_failure("Error, unexpected end of array declaration ( '[]' scope started )\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		if (currtok.Type == Tok_BraceSquare_Close)
		{
			gen_log_failure("Error, empty array expression in definition\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		gen_Token gen_untyped_tok = currtok;

		while (left && currtok.Type != Tok_BraceSquare_Close)
		{
			eat(currtok.Type);
		}

		gen_untyped_tok.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)gen_untyped_tok.Text.Ptr;

		gen_Code gen_array_expr  = gen_untyped_str(gen_untyped_tok.Text);
		// [ <Content>

		if (left == 0)
		{
			gen_log_failure("Error, unexpected end of array declaration, expected ]\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		if (currtok.Type != Tok_BraceSquare_Close)
		{
			gen_log_failure(
			    "%s: Error, expected ] in array declaration, not %s\n%s",
			    gen_toktype_to_str(currtok.Type),
			    gen_parser_to_strbuilder(gen__ctx->parser)
			);
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		eat(Tok_BraceSquare_Close);
		// [ <Content> ]

		// Its a multi-dimensional array
		if (check(Tok_BraceSquare_Open))
		{
			gen_Code adjacent_arr_expr = gen_parse_array_decl();
			// [ <Content> ][ <Content> ]...

			gen_array_expr->Next = adjacent_arr_expr;
		}

		gen_parser_pop(&gen__ctx->parser);
		return gen_array_expr;
	}

	gen_parser_pop(&gen__ctx->parser);
	return gen_NullCode;
}

gen_internal inline gen_CodeAttributes gen_parse_attributes()
{
	push_scope();

	gen_Token start = currtok;
	gen_s32   len   = 0;

	// There can be more than one attribute. If there is flatten them to a single string.
	// TODO(Ed): Support chaining attributes (Use parameter linkage pattern)
	while (left && gen_tok_is_attribute(currtok))
	{
		if (check(Tok_Attribute_Open))
		{
			eat(Tok_Attribute_Open);
			// [[

			while (left && currtok.Type != Tok_Attribute_Close)
			{
				eat(currtok.Type);
			}
			// [[ <Content>

			eat(Tok_Attribute_Close);
			// [[ <Content> ]]

			len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)start.Text.Ptr;
		}
		else if (check(Tok_Decl_GNU_Attribute))
		{
			eat(Tok_Decl_GNU_Attribute);
			eat(Tok_Paren_Open);
			eat(Tok_Paren_Open);
			// __attribute__((

			while (left && currtok.Type != Tok_Paren_Close)
			{
				eat(currtok.Type);
			}
			// __attribute__(( <Content>

			eat(Tok_Paren_Close);
			eat(Tok_Paren_Close);
			// __attribute__(( <Content> ))

			len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)start.Text.Ptr;
		}
		else if (check(Tok_Decl_MSVC_Attribute))
		{
			eat(Tok_Decl_MSVC_Attribute);
			eat(Tok_Paren_Open);
			// __declspec(

			while (left && currtok.Type != Tok_Paren_Close)
			{
				eat(currtok.Type);
			}
			// __declspec( <Content>

			eat(Tok_Paren_Close);
			// __declspec( <Content> )

			len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)start.Text.Ptr;
		}
		else if (gen_tok_is_attribute(currtok))
		{
			eat(currtok.Type);
			// <Attribute>

			// If its a macro based attribute, this could be a functional macro such as Unreal's UE_DEPRECATED(...)
			if (check(Tok_Paren_Open))
			{
				eat(Tok_Paren_Open);

				gen_s32 level = 0;
				while (left && currtok.Type != Tok_Paren_Close && level == 0)
				{
					if (currtok.Type == Tok_Paren_Open)
						++level;
					if (currtok.Type == Tok_Paren_Close)
						--level;
					eat(currtok.Type);
				}
				eat(Tok_Paren_Close);
			}

			len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)start.Text.Ptr;
			// <Attribute> ( ... )
		}
	}

	if (len > 0)
	{
		gen_Str attribute_txt = { start.Text.Ptr, len };
		gen_parser_pop(&gen__ctx->parser);

		gen_StrBuilder name_stripped = gen_parser_strip_formatting(attribute_txt, gen_parser_strip_formatting_dont_preserve_newlines);

		gen_Code result              = gen_make_code();
		result->Type                 = CT_PlatformAttributes;
		result->Name                 = gen_cache_str(gen_strbuilder_to_str(name_stripped));
		result->Content              = result->Name;
		// result->gen_Token   =

		return (gen_CodeAttributes)result;
	}

	gen_parser_pop(&gen__ctx->parser);
	return gen_NullCode;
}

gen_internal gen_Code gen_parse_class_struct(gen_TokType which, bool inplace_def)
{
	if (which != Tok_Decl_Class && which != Tok_Decl_Struct)
	{
		gen_log_failure("Error, expected class or struct, not %s\n%s", gen_toktype_to_str(which), gen_parser_to_strbuilder(gen__ctx->parser));
		return gen_InvalidCode;
	}

	gen_Token name                = gen_NullToken;

	gen_AccessSpec     access     = AccessSpec_Default;
	gen_CodeTypename   parent     = { gen_nullptr };
	gen_CodeBody       body       = { gen_nullptr };
	gen_CodeAttributes attributes = { gen_nullptr };
	gen_ModuleFlag     mflags     = ModuleFlag_None;

	gen_Code result               = gen_InvalidCode;

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <ModuleFlags>

	eat(which);
	// <ModuleFlags> <class/struct>

	attributes = gen_parse_attributes();
	// <ModuleFlags> <class/struct> <Attributes>

	if (check(Tok_Identifier))
	{
		name                         = gen_parse_identifier(gen_nullptr);
		gen__ctx->parser.Scope->Name = name.Text;
	}
	// <ModuleFlags> <class/struct> <Attributes> <Name>

	gen_CodeSpecifiers specifiers = gen_NullCode;
	if (check(Tok_Spec_Final))
	{
		specifiers = gen_def_specifier(Spec_Final);
		eat(Tok_Spec_Final);
	}
	// <ModuleFlags> <class/struct> <Attributes> <Name> <final>

	gen_local_persist char interface_arr_mem[gen_kilobytes(4)] = { 0 };
	gen_Array(gen_CodeTypename) interfaces;
	{
		gen_Arena arena = gen_arena_init_from_memory(interface_arr_mem, gen_kilobytes(4));
		interfaces      = gen_array_init_reserve(gen_CodeTypename, gen_arena_allocator_info(&arena), 4);
	}

	// TODO(Ed) : Make an gen_AST_DerivedType, we'll store any arbitary derived type into there as a linear linked list of them.
	if (check(Tok_Assign_Classifer))
	{
		eat(Tok_Assign_Classifer);
		// <ModuleFlags> <class/struct> <Attributes> <Name> <final> :

		if (gen_tok_is_access_specifier(currtok))
		{
			access = gen_tok_to_access_specifier(currtok);
			// <ModuleFlags> <class/struct> <Attributes> <Name> <final> : <Access gen_Specifier>
			eat(currtok.Type);
		}

		gen_Token parent_tok = gen_parse_identifier(gen_nullptr);
		parent               = gen_def_type(parent_tok.Text);
		// <ModuleFlags> <class/struct> <Attributes> <Name> <final> : <Access gen_Specifier> <Parent/Interface Name>

		while (check(Tok_Comma))
		{
			eat(Tok_Comma);
			// <ModuleFlags> <class/struct> <Attributes> <Name> <final> : <Access gen_Specifier> <Name>,

			if (gen_tok_is_access_specifier(currtok))
			{
				eat(currtok.Type);
			}
			gen_Token interface_tok = gen_parse_identifier(gen_nullptr);

			gen_array_append(interfaces, gen_def_type(interface_tok.Text));
			// <ModuleFlags> <class/struct> <Attributes> <Name> <final> : <Access gen_Specifier> <Name>, ...
		}
	}

	if (check(Tok_BraceCurly_Open))
	{
		body = gen_parse_class_struct_body(which, name);
	}
	// <ModuleFlags> <class/struct> <Attributes> <Name> <final> : <Access gen_Specifier> <Name>, ... { <Body> }

	gen_CodeComment inline_cmt = gen_NullCode;
	if (! inplace_def)
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// <ModuleFlags> <class/struct> <Attributes> <Name> <final> : <Access gen_Specifier> <Name>, ... { <Body> };

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// <ModuleFlags> <class/struct> <Attributes> <Name> <final> : <Access gen_Specifier> <Name>, ... { <Body> }; <InlineCmt>
	}

	if (which == Tok_Decl_Class)
		result = gen_cast(
		    gen_Code,
		    gen_def_class(
		        name.Text,
		        gen_def_assign(body, parent, access, attributes, interfaces, gen_scast(gen_s32, gen_array_num(interfaces)), specifiers, mflags)
		    )
		);

	else
		result = gen_cast(
		    gen_Code,
		    gen_def_struct(
		        name.Text,
		        gen_def_assign(
		            body,
		            (gen_CodeTypename)parent,
		            access,
		            attributes,
		            interfaces,
		            gen_scast(gen_s32, gen_array_num(interfaces)),
		            specifiers,
		            mflags
		        )
		    )
		);

	if (inline_cmt)
		result->InlineCmt = gen_cast(gen_Code, inline_cmt);

	gen_array_free(interfaces);
	return result;
}

gen_internal gen_neverinline gen_CodeBody gen_parse_class_struct_body(gen_TokType which, gen_Token name)
{
	push_scope();

	eat(Tok_BraceCurly_Open);
	// {

	gen_CodeBody result = (gen_CodeBody)gen_make_code();

	if (which == Tok_Decl_Class)
		result->Type = CT_Class_Body;

	else
		result->Type = CT_Struct_Body;

	while (left && currtok_noskip.Type != Tok_BraceCurly_Close)
	{
		gen_Code           member     = gen_Code_Invalid;
		gen_CodeAttributes attributes = { gen_nullptr };
		gen_CodeSpecifiers specifiers = { gen_nullptr };

		bool expects_function         = false;

		// gen__ctx->parser.Scope->Start = currtok_noskip;

		if (currtok_noskip.Type == Tok_Preprocess_Hash)
			eat(Tok_Preprocess_Hash);

		switch (currtok_noskip.Type)
		{
			case Tok_Statement_End:
			{
				// TODO(Ed): Convert this to a general warning procedure
				gen_log_fmt("Dangling end statement found %SB\n", gen_tok_to_strbuilder(currtok_noskip));
				eat(Tok_Statement_End);
				continue;
			}
			case Tok_NewLine:
			{
				member = gen_fmt_newline;
				eat(Tok_NewLine);
				break;
			}
			case Tok_Comment:
			{
				member = gen_cast(gen_Code, gen_parse_comment());
				break;
			}
			case Tok_Access_Public:
			{
				member = gen_access_public;
				eat(Tok_Access_Public);
				eat(Tok_Assign_Classifer);
				// public:
				break;
			}
			case Tok_Access_Protected:
			{
				member = gen_access_protected;
				eat(Tok_Access_Protected);
				eat(Tok_Assign_Classifer);
				// protected:
				break;
			}
			case Tok_Access_Private:
			{
				member = gen_access_private;
				eat(Tok_Access_Private);
				eat(Tok_Assign_Classifer);
				// private:
				break;
			}
			case Tok_Decl_Class:
			{
				member = gen_parse_complicated_definition(Tok_Decl_Class);
				// class
				break;
			}
			case Tok_Decl_Enum:
			{
				member = gen_parse_complicated_definition(Tok_Decl_Enum);
				// enum
				break;
			}
			case Tok_Decl_Friend:
			{
				member = gen_cast(gen_Code, gen_parser_parse_friend());
				// friend
				break;
			}
			case Tok_Decl_Operator:
			{
				member = gen_cast(gen_Code, gen_parser_parse_operator_cast(gen_NullCode));
				// operator <Type>()
				break;
			}
			case Tok_Decl_Struct:
			{
				member = gen_parse_complicated_definition(Tok_Decl_Struct);
				// struct
				break;
			}
			case Tok_Decl_Template:
			{
				member = gen_cast(gen_Code, gen_parser_parse_template());
				// template< ... >
				break;
			}
			case Tok_Decl_Typedef:
			{
				member = gen_cast(gen_Code, gen_parser_parse_typedef());
				// typedef
				break;
			}
			case Tok_Decl_Union:
			{
				member = gen_parse_complicated_definition(Tok_Decl_Union);
				// union
				break;
			}
			case Tok_Decl_Using:
			{
				member = gen_cast(gen_Code, gen_parser_parse_using());
				// using
				break;
			}
			case Tok_Operator:
			{
				//if ( currtok.Text[0] != '~' )
				//{
				//	gen_log_failure( "gen_Operator token found in gen_global body but not gen_destructor_ unary negation\n%s", to_strbuilder(gen__ctx->parser) );
				//	return gen_InvalidCode;
				//}

				member = gen_cast(gen_Code, gen_parser_parse_destructor(gen_NullCode));
				// ~<Name>()
				break;
			}
			case Tok_Preprocess_Define:
			{
				member = gen_cast(gen_Code, gen_parser_parse_define());
				// #define
				break;
			}
			case Tok_Preprocess_Include:
			{
				member = gen_cast(gen_Code, gen_parse_include());
				// #include
				break;
			}

			case Tok_Preprocess_If:
			case Tok_Preprocess_IfDef:
			case Tok_Preprocess_IfNotDef:
			case Tok_Preprocess_ElIf:
				member = gen_cast(gen_Code, gen_parse_preprocess_cond());
				// #<Condition>
				break;

			case Tok_Preprocess_Else:
			{
				member = gen_cast(gen_Code, gen_preprocess_else);
				eat(Tok_Preprocess_Else);
				// #else
				break;
			}
			case Tok_Preprocess_EndIf:
			{
				member = gen_cast(gen_Code, gen_preprocess_endif);
				eat(Tok_Preprocess_EndIf);
				// #endif
				break;
			}

			case Tok_Preprocess_Macro_Stmt:
			{
				member = gen_cast(gen_Code, gen_parse_simple_preprocess(Tok_Preprocess_Macro_Stmt));
				break;
			}

				// case Tok_Preprocess_Macro:
				// 	// <gen_Macro>
				// 	gen_macro_found = true;
				// 	goto Preprocess_Macro_Bare_In_Body;
				// break;

			case Tok_Preprocess_Pragma:
			{
				member = gen_cast(gen_Code, gen_parse_pragma());
				// #pragma
				break;
			}

			case Tok_Preprocess_Unsupported:
			{
				member = gen_cast(gen_Code, gen_parse_simple_preprocess(Tok_Preprocess_Unsupported));
				// #<UNKNOWN>
				break;
			}

			case Tok_StaticAssert:
			{
				member = gen_parse_static_assert();
				// gen_static_assert
				break;
			}

			case Tok_Preprocess_Macro_Expr:
			{
				if (! gen_tok_is_attribute(currtok))
				{
					gen_log_failure("Unbounded macro expression residing in class/struct body\n%S", gen_parser_to_strbuilder(gen__ctx->parser));
					return gen_InvalidCode;
				}
			}
			//! Fallthrough intended
			case Tok_Attribute_Open:
			case Tok_Decl_GNU_Attribute:
			case Tok_Decl_MSVC_Attribute:
#define Entry(attribute, str) case attribute:
				GEN_DEFINE_ATTRIBUTE_TOKENS
#undef Entry
				{
					attributes = gen_parse_attributes();
					// <Attributes>
				}
			//! Fallthrough intended
			GEN_PARSER_CLASS_STRUCT_BODY_ALLOWED_MEMBER_TOK_SPECIFIER_CASES:
			{
				gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
				gen_s32       NumSpecifiers   = 0;

				while (left && gen_tok_is_specifier(currtok))
				{
					gen_Specifier spec  = gen_str_to_specifier(currtok.Text);

					gen_b32 ignore_spec = false;

					switch (spec)
					{
					GEN_PARSER_CLASS_STRUCT_BODY_ALLOWED_MEMBER_SPECIFIER_CASES:
						break;

						case Spec_Consteval:
							expects_function = true;
							break;

						case Spec_Const:
							ignore_spec = true;
							break;

						default:
							gen_log_failure(
							    "Invalid specifier %S for class/struct member\n%S",
							    gen_spec_to_str(spec),
							    gen_strbuilder_to_str(gen_parser_to_strbuilder(gen__ctx->parser))
							);
							gen_parser_pop(&gen__ctx->parser);
							return gen_InvalidCode;
					}

					// Every specifier after would be considered part of the type type signature
					if (ignore_spec)
						break;

					specs_found[NumSpecifiers] = spec;
					NumSpecifiers++;
					eat(currtok.Type);
				}

				if (NumSpecifiers)
				{
					specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
				}
				// <Attributes> <Specifiers>

				if (gen_tok_is_attribute(currtok))
				{
					// Unfortuantely Unreal has code where there is attirbutes before specifiers
					gen_CodeAttributes more_attributes = gen_parse_attributes();

					if (attributes)
					{
						gen_StrBuilder fused = gen_strbuilder_make_reserve(gen__ctx->Allocator_Temp, attributes->Content.Len + more_attributes->Content.Len);
						gen_strbuilder_append_fmt(&fused, "%SB %SB", attributes->Content, more_attributes->Content);

						gen_Str attrib_name = gen_strbuilder_to_str(fused);
						attributes->Name    = gen_cache_str(attrib_name);
						attributes->Content = attributes->Name;
						// <Attributes> <Specifiers> <Attributes>
					}

					attributes = more_attributes;
				}

				if (currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '~')
				{
					member = gen_cast(gen_Code, gen_parser_parse_destructor(specifiers));
					// <Attribute> <Specifiers> ~<Name>()
					break;
				}

				if (currtok.Type == Tok_Decl_Operator)
				{
					member = gen_cast(gen_Code, gen_parser_parse_operator_cast(specifiers));
					// <Attributes> <Specifiers> operator <Type>()
					break;
				}
			}
			//! Fallthrough intentional
			case Tok_Identifier:
			case Tok_Preprocess_Macro_Typename:
			case Tok_Spec_Const:
			case Tok_Type_Unsigned:
			case Tok_Type_Signed:
			case Tok_Type_Short:
			case Tok_Type_Long:
			case Tok_Type_bool:
			case Tok_Type_char:
			case Tok_Type_int:
			case Tok_Type_double:
			{
				if (nexttok.Type == Tok_Paren_Open && name.Text.Len && currtok.Type == Tok_Identifier)
				{
					if (gen_c_str_compare_len(name.Text.Ptr, currtok.Text.Ptr, name.Text.Len) == 0)
					{
						member = gen_cast(gen_Code, gen_parser_parse_constructor(specifiers));
						// <Attributes> <Specifiers> <Name>()
						break;
					}
				}

				member = gen_parse_operator_function_or_variable(expects_function, attributes, specifiers);
				// <Attributes> <Specifiers> operator <Op> ...
				// or
				// <Attributes> <Specifiers> <Name> ...
			}
			break;

			default:
				gen_Token gen_untyped_tok = currtok;
				while (left && currtok.Type != Tok_BraceCurly_Close)
				{
					gen_untyped_tok.Text.Len = ((gen_sptr)currtok.Text.Ptr + currtok.Text.Len) - (gen_sptr)gen_untyped_tok.Text.Ptr;
					eat(currtok.Type);
				}
				member = gen_untyped_str(gen_untyped_tok.Text);
				// Something unknown
				break;
		}

		if (member == gen_Code_Invalid)
		{
			gen_log_failure("Failed to parse member\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		gen_body_append(result, member);
	}

	eat(Tok_BraceCurly_Close);
	// { <Members> }
	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeComment gen_parse_comment()
{
	push_scope();

	gen_CodeComment result = (gen_CodeComment)gen_make_code();
	result->Type           = CT_Comment;
	result->Content        = gen_cache_str(currtok_noskip.Text);
	// result->gen_Token   = currtok_noskip;
	eat(Tok_Comment);

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_Code gen_parse_complicated_definition(gen_TokType which)
{
	push_scope();

	gen_b32 is_inplace  = false;
	gen_b32 is_fn_def   = false;

	gen_TokArray tokens = gen__ctx->parser.Tokens;

	gen_s32 idx         = tokens.Idx;
	gen_s32 level       = 0;
	gen_b32 had_def     = false;
	gen_b32 had_paren   = false;
	for (; idx < gen_array_num(tokens.Arr); idx++)
	{
		if (tokens.Arr[idx].Type == Tok_BraceCurly_Open)
			level++;

		if (tokens.Arr[idx].Type == Tok_BraceCurly_Close)
		{
			level--;
			had_def = level == 0;
		}

		gen_b32 found_fn_def = had_def && had_paren;

		if (level == 0 && (tokens.Arr[idx].Type == Tok_Statement_End || found_fn_def))
			break;
	}

	is_fn_def = had_def && had_paren;
	if (is_fn_def)
	{
		// Function definition with <which> on return type
		gen_Code result = gen_parse_operator_function_or_variable(false, gen_NullCode, gen_NullCode);
		// <which> <typename>(...) ... { ... }
		gen_parser_pop(&gen__ctx->parser);
		return result;
	}

	if ((idx - 2) == tokens.Idx)
	{
		// It's a forward declaration only
		gen_Code result = gen_parse_forward_or_definition(which, is_inplace);
		// <class, enum, struct, or union> <Name>;
		gen_parser_pop(&gen__ctx->parser);
		return result;
	}

	gen_Token tok = tokens.Arr[idx - 1];
	if (gen_tok_is_specifier(tok) && gen_spec_is_trailing(gen_str_to_specifier(tok.Text)))
	{
		// <which> <type_identifier>(...) <specifier> ...;

		gen_s32   gen_spec_idx = idx - 1;
		gen_Token spec         = tokens.Arr[gen_spec_idx];
		while (gen_tok_is_specifier(spec) && gen_spec_is_trailing(gen_str_to_specifier(spec.Text)))
		{
			--gen_spec_idx;
			spec = tokens.Arr[gen_spec_idx];
		}

		if (tokens.Arr[gen_spec_idx].Type == Tok_Paren_Close)
		{
			// Forward declaration with trailing specifiers for a procedure
			tok             = tokens.Arr[gen_spec_idx];

			gen_Code result = gen_parse_operator_function_or_variable(false, gen_NullCode, gen_NullCode);
			// <Attributes> <Specifiers> <ReturnType/ValueType> <operator <Op>, or Name> ...
			gen_parser_pop(&gen__ctx->parser);
			return result;
		}

		gen_log_failure("Unsupported or bad member definition after %s declaration\n%s", gen_toktype_to_str(which), gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	if (tok.Type == Tok_Identifier)
	{
		tok                 = tokens.Arr[idx - 2];
		bool is_indirection = tok.Type == Tok_Ampersand || tok.Type == Tok_Star;
		bool ok_to_parse    = false;

		if (tok.Type == Tok_BraceCurly_Close)
		{
			// Its an inplace definition
			// <which> <type_identifier ?> { ... } <identifier>;
			ok_to_parse           = true;
			is_inplace            = true;

			gen_CodeTypename type = gen_cast(gen_CodeTypename, gen_parse_forward_or_definition(which, is_inplace));

			// Should be a name right after the type.
			gen_Token name               = gen_parse_identifier(gen_nullptr);
			gen__ctx->parser.Scope->Name = name.Text;

			gen_CodeVar result           = gen_parse_variable_after_name(ModuleFlag_None, gen_NullCode, gen_NullCode, type, name.Text);
			gen_parser_pop(&gen__ctx->parser);
			return (gen_Code)result;
		}
		else if (tok.Type == Tok_Identifier && tokens.Arr[idx - 3].Type == which)
		{
			// Its a variable with type ID using <which> namespace.
			// <which> <type_identifier> <identifier>;
			ok_to_parse = true;
		}
		else if (tok.Type == Tok_Assign_Classifer
		         && ((tokens.Arr[idx - 5].Type == which && tokens.Arr[idx - 4].Type == Tok_Decl_Class) || (tokens.Arr[idx - 4].Type == which)))
		{
			// Its a forward declaration of an enum
			// <enum>         <type_identifier> : <identifier>;
			// <enum> <class> <type_identifier> : <identifier>;
			ok_to_parse     = true;
			gen_Code result = gen_cast(gen_Code, gen_parser_parse_enum(! gen_parser_inplace_def));
			gen_parser_pop(&gen__ctx->parser);
			return result;
		}
		else if (is_indirection)
		{
			// Its a indirection type with type ID using struct namespace.
			// <which> <type_identifier>* <identifier>;
			ok_to_parse = true;
		}

		if (! ok_to_parse)
		{
			gen_log_failure(
			    "Unsupported or bad member definition after %s declaration\n%s",
			    gen_toktype_to_str(which),
			    gen_parser_to_strbuilder(gen__ctx->parser)
			);
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		gen_Code result = gen_parse_operator_function_or_variable(false, gen_NullCode, gen_NullCode);
		// <Attributes> <Specifiers> <ReturnType/ValueType> <operator <Op>, or Name> ...
		gen_parser_pop(&gen__ctx->parser);
		return result;
	}
	else if (tok.Type >= Tok_Type_Unsigned && tok.Type <= Tok_Type_MS_W64)
	{
		tok = tokens.Arr[idx - 2];

		if (tok.Type != Tok_Assign_Classifer
		    || ((tokens.Arr[idx - 5].Type != which && tokens.Arr[idx - 4].Type != Tok_Decl_Class) && (tokens.Arr[idx - 4].Type != which)))
		{
			gen_log_failure(
			    "Unsupported or bad member definition after %s declaration\n%s",
			    gen_toktype_to_str(which),
			    gen_parser_to_strbuilder(gen__ctx->parser)
			);
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		// Its a forward declaration of an enum class
		// <enum>         <type_identifier> : <identifier>;
		// <enum> <class> <type_identifier> : <identifier>;
		gen_Code result = gen_cast(gen_Code, gen_parser_parse_enum(! gen_parser_inplace_def));
		gen_parser_pop(&gen__ctx->parser);
		return result;
	}
	else if (tok.Type == Tok_BraceCurly_Close)
	{
		// Its a definition
		gen_Code result = gen_parse_forward_or_definition(which, is_inplace);
		// <which> { ... };
		gen_parser_pop(&gen__ctx->parser);
		return result;
	}
	else if (tok.Type == Tok_BraceSquare_Close)
	{
		// Its an array definition
		gen_Code result = gen_parse_operator_function_or_variable(false, gen_NullCode, gen_NullCode);
		// <which> <type_identifier> <identifier> [ ... ];
		gen_parser_pop(&gen__ctx->parser);
		return result;
	}
	else
	{
		gen_log_failure(
		    "Unsupported or bad member definition after %s declaration\n%SB",
		    gen_toktype_to_str(which).Ptr,
		    gen_parser_to_strbuilder(gen__ctx->parser)
		);
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
}

gen_internal inline gen_Code gen_parse_assignment_expression()
{
	gen_Code expr = { gen_nullptr };

	eat(Tok_Operator);
	// <Attributes> <Specifiers> <ValueType> <Name> =

	gen_Token expr_tok = currtok;

	if (currtok.Type == Tok_Statement_End && currtok.Type != Tok_Comma)
	{
		gen_log_failure("Expected expression after assignment operator\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	gen_s32 level = 0;
	while (left && currtok.Type != Tok_Statement_End && (currtok.Type != Tok_Comma || level > 0))
	{
		if (currtok.Type == Tok_BraceCurly_Open)
			level++;
		if (currtok.Type == Tok_BraceCurly_Close)
			level--;
		if (currtok.Type == Tok_Paren_Open)
			level++;
		else if (currtok.Type == Tok_Paren_Close)
			level--;

		eat(currtok.Type);
	}

	if (left)
	{
		expr_tok.Text.Len = ((gen_sptr)currtok.Text.Ptr + currtok.Text.Len) - (gen_sptr)expr_tok.Text.Ptr - 1;
	}
	expr = gen_untyped_str(expr_tok.Text);
	// = <Expression>
	return expr;
}

gen_internal inline gen_Code gen_parse_forward_or_definition(gen_TokType which, bool is_inplace)
{
	gen_Code result = gen_InvalidCode;

	switch (which)
	{
		case Tok_Decl_Class:
			result = gen_cast(gen_Code, gen_parser_parse_class(is_inplace));
			return result;

		case Tok_Decl_Enum:
			result = gen_cast(gen_Code, gen_parser_parse_enum(is_inplace));
			return result;

		case Tok_Decl_Struct:
			result = gen_cast(gen_Code, gen_parser_parse_struct(is_inplace));
			return result;

		case Tok_Decl_Union:
			result = gen_cast(gen_Code, gen_parser_parse_union(is_inplace));
			return result;

		default:
			gen_log_failure(
			    "Error, wrong token type given to gen_parse_complicated_definition "
			    "(only supports class, enum, struct, union) \n%s",
			    gen_parser_to_strbuilder(gen__ctx->parser)
			);

			return gen_InvalidCode;
	}
}

// Function parsing is handled in multiple places because its initial signature is shared with variable parsing
gen_internal inline gen_CodeFn gen_parse_function_after_name(
    gen_ModuleFlag     mflags,
    gen_CodeAttributes attributes,
    gen_CodeSpecifiers specifiers,
    gen_CodeTypename   ret_type,
    gen_Token          name
)
{
	push_scope();
	gen_CodeParams params = gen_parse_params(gen_parser_use_parenthesis);
	// <Attributes> <Specifiers> <ReturnType> <Name> ( <Parameters> )

	gen_Code suffix_specs = gen_NullCode;

	// TODO(Ed), Review old comment : These have to be kept separate from the return type's specifiers.
	while (left && gen_tok_is_specifier(currtok))
	{
		// For Unreal's PURE_VIRTUAL Support
		gen_Macro* macro = lookup_macro(currtok.Text);
		if (macro && gen_tok_is_specifier(currtok))
		{
			suffix_specs = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Expr);
			continue;
		}
		if (specifiers == gen_nullptr)
		{
			specifiers = gen_def_specifier(gen_str_to_specifier(currtok.Text));
			eat(currtok.Type);
			continue;
		}

		gen_specifiers_append(specifiers, gen_str_to_specifier(currtok.Text));
		eat(currtok.Type);
	}
	// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers>

	// Check for trailing specifiers...
	gen_CodeAttributes post_rt_attributes = gen_parse_attributes();
	if (post_rt_attributes)
	{
		if (attributes)
		{
			gen_StrBuilder merged = gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "%S %S", attributes->Content, post_rt_attributes->Content);
			attributes->Content   = gen_cache_str(gen_strbuilder_to_str(merged));
		}
		else
		{
			attributes = post_rt_attributes;
		}
	}
	// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> <Attributes>

	gen_CodeBody    body       = gen_NullCode;
	gen_CodeComment inline_cmt = gen_NullCode;
	if (check(Tok_BraceCurly_Open))
	{
		body = gen_cast(gen_CodeBody, gen_parse_function_body());
		if (gen_cast(gen_Code, body) == gen_Code_Invalid)
		{
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> { <Body> }
	}
	else if (check(Tok_Operator) && currtok.Text.Ptr[0] == '=')
	{
		eat(Tok_Operator);
		if (specifiers == gen_nullptr)
		{
			specifiers       = (gen_CodeSpecifiers)gen_make_code();
			specifiers->Type = CT_Specifiers;
		}
		if (gen_str_are_equal(nexttok.Text, gen_txt("delete")))
		{
			gen_specifiers_append(specifiers, Spec_Delete);
			eat(currtok.Type);
			// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> = delete
		}
		else
		{
			gen_specifiers_append(specifiers, Spec_Pure);

			eat(Tok_Number);
			// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> = 0
		}
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> < = 0 or delete > ; <InlineCmt>
	}


	if (body == gen_nullptr)
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> <Attributes> < = 0 or delete > ;

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// <Attributes> <Specifiers> <ReturnType> <Name> ( <Paraemters> ) <Specifiers> < = 0 or delete > ; <InlineCmt>
	}

	gen_StrBuilder name_stripped = gen_strbuilder_make_str(gen__ctx->Allocator_Temp, name.Text);
	gen_strbuilder_strip_space(name_stripped);

	gen_CodeFn result   = (gen_CodeFn)gen_make_code();
	result->Name        = gen_cache_str(gen_strbuilder_to_str(name_stripped));
	result->ModuleFlags = mflags;

	if (body)
	{
		switch (body->Type)
		{
			case CT_Function_Body:
			case CT_Untyped:
				break;

			default:
			{
				gen_log_failure(
				    "Body must be either of Function_Body or Untyped type, %s\n%s",
				    gen_code_debug_str(body),
				    gen_parser_to_strbuilder(gen__ctx->parser)
				);
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}
		}

		result->Type = CT_Function;
		result->Body = body;
	}
	else
	{
		result->Type = CT_Function_Fwd;
	}

	if (attributes)
		result->Attributes = attributes;

	if (specifiers)
		result->Specs = specifiers;

	if (suffix_specs)
		result->SuffixSpecs = suffix_specs;

	result->ReturnType = ret_type;

	if (params)
		result->Params = params;

	if (inline_cmt)
		result->InlineCmt = inline_cmt;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_Code gen_parse_function_body()
{
	push_scope();

	eat(Tok_BraceCurly_Open);

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = CT_Function_Body;

	// TODO : Support actual parsing of function body
	gen_Token start = currtok_noskip;

	gen_s32 level   = 0;
	while (left && (currtok_noskip.Type != Tok_BraceCurly_Close || level > 0))
	{
		if (currtok_noskip.Type == Tok_BraceCurly_Open)
			level++;

		else if (currtok_noskip.Type == Tok_BraceCurly_Close && level > 0)
			level--;

		eat(currtok_noskip.Type);
	}

	gen_Token past = prevtok;

	gen_s32 len    = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)start.Text.Ptr;

	if (len > 0)
	{
		gen_Str str = { start.Text.Ptr, len };
		gen_body_append(result, gen_cast(gen_Code, gen_def_execution(str)));
	}

	eat(Tok_BraceCurly_Close);

	gen_parser_pop(&gen__ctx->parser);
	return gen_cast(gen_Code, result);
}

gen_internal gen_neverinline gen_CodeBody gen_parse_global_nspace(gen_CodeType which)
{
	push_scope();

	if (which != CT_Namespace_Body && which != CT_Global_Body && which != CT_Export_Body && which != CT_Extern_Linkage_Body)
		return gen_InvalidCode;

	if (which != CT_Global_Body)
		eat(Tok_BraceCurly_Open);
	// {

	gen_CodeBody result = (gen_CodeBody)gen_make_code();
	result->Type        = which;

	while (left && currtok_noskip.Type != Tok_BraceCurly_Close)
	{
		gen_Code           member     = gen_Code_Invalid;
		gen_CodeAttributes attributes = { gen_nullptr };
		gen_CodeSpecifiers specifiers = { gen_nullptr };

		bool expects_function         = false;

		// gen__ctx->parser.Scope->Start = currtok_noskip;

		if (currtok_noskip.Type == Tok_Preprocess_Hash)
			eat(Tok_Preprocess_Hash);

		gen_b32 gen_macro_found = false;

		switch (currtok_noskip.Type)
		{
			case Tok_Comma:
			{
				gen_log_failure("Dangling comma found: %SB\nContext:\n%SB", gen_tok_to_strbuilder(currtok), gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}
			break;
			case Tok_Statement_End:
			{
				// TODO(Ed): Convert this to a general warning procedure
				gen_log_fmt("Dangling end statement found %SB\n", gen_tok_to_strbuilder(currtok_noskip));
				eat(Tok_Statement_End);
				continue;
			}
			case Tok_NewLine:
				// Empty lines are auto skipped by Tokens.current()
				member = gen_fmt_newline;
				eat(Tok_NewLine);
				break;

			case Tok_Comment:
				member = gen_cast(gen_Code, gen_parse_comment());
				break;

			case Tok_Decl_Class:
				member = gen_parse_complicated_definition(Tok_Decl_Class);
				// class
				break;

			case Tok_Decl_Enum:
				member = gen_parse_complicated_definition(Tok_Decl_Enum);
				// enum
				break;

			case Tok_Decl_Extern_Linkage:
				if (which == CT_Extern_Linkage_Body)
					gen_log_failure("Nested extern linkage\n%s", gen_parser_to_strbuilder(gen__ctx->parser));

				member = gen_cast(gen_Code, gen_parser_parse_extern_link());
				// extern "..." { ... }
				break;

			case Tok_Decl_Namespace:
				member = gen_cast(gen_Code, gen_parser_parse_namespace());
				// namespace <Name> { ... }
				break;

			case Tok_Decl_Struct:
				member = gen_parse_complicated_definition(Tok_Decl_Struct);
				// struct ...
				break;

			case Tok_Decl_Template:
				member = gen_cast(gen_Code, gen_parser_parse_template());
				// template<...> ...
				break;

			case Tok_Decl_Typedef:
				member = gen_cast(gen_Code, gen_parser_parse_typedef());
				// typedef ...
				break;

			case Tok_Decl_Union:
				member = gen_parse_complicated_definition(Tok_Decl_Union);
				// union ...
				break;

			case Tok_Decl_Using:
				member = gen_cast(gen_Code, gen_parser_parse_using());
				// using ...
				break;

			case Tok_Preprocess_Define:
				member = gen_cast(gen_Code, gen_parser_parse_define());
				// #define ...
				break;

			case Tok_Preprocess_Include:
				member = gen_cast(gen_Code, gen_parse_include());
				// #include ...
				break;

			case Tok_Preprocess_If:
			case Tok_Preprocess_IfDef:
			case Tok_Preprocess_IfNotDef:
			case Tok_Preprocess_ElIf:
				member = gen_cast(gen_Code, gen_parse_preprocess_cond());
				// #<Conditional> ...
				break;

			case Tok_Preprocess_Else:
				member = gen_cast(gen_Code, gen_preprocess_else);
				eat(Tok_Preprocess_Else);
				// #else
				break;

			case Tok_Preprocess_EndIf:
				member = gen_cast(gen_Code, gen_preprocess_endif);
				eat(Tok_Preprocess_EndIf);
				// #endif
				break;

			case Tok_Preprocess_Macro_Stmt:
			{
				member = gen_cast(gen_Code, gen_parse_simple_preprocess(Tok_Preprocess_Macro_Stmt));
				break;
			}

			case Tok_Preprocess_Pragma:
			{
				member = gen_cast(gen_Code, gen_parse_pragma());
				// #pragma ...
			}
			break;

			case Tok_Preprocess_Unsupported:
			{
				member = gen_cast(gen_Code, gen_parse_simple_preprocess(Tok_Preprocess_Unsupported));
				// #<UNSUPPORTED> ...
			}
			break;

			case Tok_StaticAssert:
			{
				member = gen_cast(gen_Code, gen_parse_static_assert());
				// gen_static_assert( <Conditional Expression>, ... );
			}
			break;

			case Tok_Module_Export:
			{
				if (which == CT_Export_Body)
					gen_log_failure("Nested export declaration\n%s", gen_parser_to_strbuilder(gen__ctx->parser));

				member = gen_cast(gen_Code, gen_parser_parse_export_body());
				// export { ... }
			}
			break;

			case Tok_Module_Import:
			{
				// import ...
				gen_log_failure("gen::%s: This function is not implemented");
				return gen_InvalidCode;
			}
			break;

			case Tok_Preprocess_Macro_Expr:
			{
				if (! gen_tok_is_attribute(currtok))
				{
					gen_log_failure("Unbounded macro expression residing in class/struct body\n%S", gen_parser_to_strbuilder(gen__ctx->parser));
					return gen_InvalidCode;
				}
			}
			//! Fallthrough intentional
			case Tok_Attribute_Open:
			case Tok_Decl_GNU_Attribute:
			case Tok_Decl_MSVC_Attribute:
#define Entry(attribute, str) case attribute:
				GEN_DEFINE_ATTRIBUTE_TOKENS
#undef Entry
				{
					attributes = gen_parse_attributes();
					// <Attributes>
				}
			//! Fallthrough intentional
			GEN_PARSER_CLASS_GLOBAL_NSPACE_ALLOWED_MEMBER_TOK_SPECIFIER_CASES:
			{
				gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
				gen_s32       NumSpecifiers   = 0;

				while (left && gen_tok_is_specifier(currtok))
				{
					gen_Specifier spec = gen_str_to_specifier(currtok.Text);

					bool ignore_spec   = false;

					switch (spec)
					{
					GEN_PARSER_CLASS_GLOBAL_NSPACE_ALLOWED_MEMBER_SPECIFIER_CASES:
						break;

						case Spec_Consteval:
							expects_function = true;
							break;

						case Spec_Const:
							ignore_spec = true;
							break;

						default:
							gen_Str gen_spec_str = gen_spec_to_str(spec);

							gen_log_failure(
							    "Invalid specifier %S for variable\n%S",
							    gen_spec_str,
							    gen_strbuilder_to_str(gen_parser_to_strbuilder(gen__ctx->parser))
							);
							gen_parser_pop(&gen__ctx->parser);
							return gen_InvalidCode;
					}

					if (ignore_spec)
						break;

					specs_found[NumSpecifiers] = spec;
					NumSpecifiers++;
					eat(currtok.Type);
				}

				if (NumSpecifiers)
				{
					specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
				}
				// <Attributes> <Specifiers>
			}
			//! Fallthrough intentional
			case Tok_Identifier:
			case Tok_Preprocess_Macro_Typename:
			case Tok_Spec_Const:
			case Tok_Type_Long:
			case Tok_Type_Short:
			case Tok_Type_Signed:
			case Tok_Type_Unsigned:
			case Tok_Type_bool:
			case Tok_Type_char:
			case Tok_Type_double:
			case Tok_Type_int:
			{
				// This s only in a scope so that Preprocess_Macro_Bare_In_Body works without microsoft extension warnings
				{
					gen_Code gen_constructor__destructor = gen_parse_global_nspace_constructor_destructor(specifiers);
					// Possible gen_constructor_ implemented at gen_global file scope.
					if (gen_constructor__destructor)
					{
						member = gen_constructor__destructor;
						break;
					}

					bool    found_operator_cast_outside_class_implmentation = false;
					gen_s32 idx                                             = gen__ctx->parser.Tokens.Idx;

					for (; idx < gen_array_num(gen__ctx->parser.Tokens.Arr); idx++)
					{
						gen_Token tok = gen__ctx->parser.Tokens.Arr[idx];

						if (tok.Type == Tok_Identifier)
						{
							idx++;
							tok = gen__ctx->parser.Tokens.Arr[idx];
							if (tok.Type == Tok_Access_StaticSymbol)
								continue;

							break;
						}

						if (tok.Type == Tok_Decl_Operator)
							found_operator_cast_outside_class_implmentation = true;

						break;
					}

					if (found_operator_cast_outside_class_implmentation)
					{
						member = gen_cast(gen_Code, gen_parser_parse_operator_cast(specifiers));
						// <Attributes> <Specifiers> <Name>::operator <Type>() { ... }
						break;
					}
				}

				member = gen_parse_operator_function_or_variable(expects_function, attributes, specifiers);
				// <Attributes> <Specifiers> ...
			}
		}

		if (member == gen_Code_Invalid)
		{
			gen_log_failure(
			    "Failed to parse member\nToken: %SB\nContext:\n%SB",
			    gen_tok_to_strbuilder(currtok_noskip),
			    gen_parser_to_strbuilder(gen__ctx->parser)
			);
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		// gen_log_fmt("Global Body Member: %s", member->debug_str());
		gen_body_append(result, member);
	}

	if (which != CT_Global_Body)
		eat(Tok_BraceCurly_Close);
	// { <Body> }

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal inline gen_Code gen_parse_global_nspace_constructor_destructor(gen_CodeSpecifiers specifiers)
{
	gen_Code result = { gen_nullptr };

	/*
		To check if a definition is for a gen_constructor_ we can go straight to the opening parenthesis for its parameters
		From There we work backwards to see if we come across two identifiers with the same name between an member access
		:: operator, there can be template parameters on the left of the :: so we ignore those.
		Whats important is that	its back to back.

		This has multiple possible faults. What we parse using this method may not filter out if something has a "return type"
		This is bad since technically you could have a namespace nested into another namespace with the same name.
		If this awful pattern is done the only way to distiguish with this coarse parse is to know there is no return type defined.

		TODO(Ed): We could fix this by attempting to parse a type, but we would have to have a way to have it soft fail and rollback.
	*/
	gen_TokArray tokens = gen__ctx->parser.Tokens;

	gen_s32   idx       = tokens.Idx;
	gen_Token nav       = tokens.Arr[idx];
	for (; idx < gen_array_num(tokens.Arr); idx++, nav = tokens.Arr[idx])
	{
		if (nav.Text.Ptr[0] == '<')
		{
			// Skip templated expressions as they mey have expressions with the () operators
			gen_s32 capture_level      = 0;
			gen_s32 gen_template_level = 0;
			for (; idx < gen_array_num(tokens.Arr); idx++, nav = tokens.Arr[idx])
			{
				if (nav.Text.Ptr[0] == '<')
					++gen_template_level;

				if (nav.Text.Ptr[0] == '>')
					--gen_template_level;
				if (nav.Type == Tok_Operator && nav.Text.Ptr[1] == '>')
					--gen_template_level;

				if (nav.Type == Tok_Paren_Open)
				{
					if (gen_template_level != 0)
						++capture_level;
					else
						break;
				}

				if (gen_template_level != 0 && nav.Type == Tok_Paren_Close)
					--capture_level;
			}
		}

		if (nav.Type == Tok_Paren_Open)
			break;
	}

	--idx;
	gen_Token gen_tok_right = tokens.Arr[idx];
	gen_Token gen_tok_left  = gen_NullToken;

	if (gen_tok_right.Type != Tok_Identifier)
	{
		// We're not dealing with a gen_constructor_ if there is no identifier right before the opening of a parameter's scope.
		return result;
	}

	--idx;
	gen_tok_left = tokens.Arr[idx];
	// <Attributes> <Specifiers> ... <Identifier>

	bool possible_destructor = false;
	if (gen_tok_left.Type == Tok_Operator && gen_tok_left.Text.Ptr[0] == '~')
	{
		possible_destructor = true;
		--idx;
		gen_tok_left = tokens.Arr[idx];
	}

	if (gen_tok_left.Type != Tok_Access_StaticSymbol)
		return result;

	--idx;
	gen_tok_left = tokens.Arr[idx];
	// <Attributes> <Specifiers> ... :: <Identifier>

	// We search toward the left until we find the next valid identifier
	gen_s32 capture_level      = 0;
	gen_s32 gen_template_level = 0;
	while (idx != tokens.Idx)
	{
		if (gen_tok_left.Text.Ptr[0] == '<')
			++gen_template_level;

		if (gen_tok_left.Text.Ptr[0] == '>')
			--gen_template_level;
		if (gen_tok_left.Type == Tok_Operator && gen_tok_left.Text.Ptr[1] == '>')
			--gen_template_level;

		if (gen_template_level != 0 && gen_tok_left.Type == Tok_Paren_Open)
			++capture_level;

		if (gen_template_level != 0 && gen_tok_left.Type == Tok_Paren_Close)
			--capture_level;

		if (capture_level == 0 && gen_template_level == 0 && gen_tok_left.Type == Tok_Identifier)
			break;

		--idx;
		gen_tok_left = tokens.Arr[idx];
	}

	bool is_same = gen_c_str_compare_len(gen_tok_right.Text.Ptr, gen_tok_left.Text.Ptr, gen_tok_right.Text.Len) == 0;
	if (gen_tok_left.Type == Tok_Identifier && is_same)
	{
		// We have found the pattern we desired
		if (possible_destructor)
		{
			// <Name> :: ~<Name> (
			result = gen_cast(gen_Code, gen_parser_parse_destructor(specifiers));
		}
		else
		{
			// <Name> :: <Name> (
			result = gen_cast(gen_Code, gen_parser_parse_constructor(specifiers));
		}
	}

	return result;
}

// TODO(Ed): I want to eventually change the identifier to its own gen_AST type.
// This would allow distinction of the qualifier for a symbol <qualifier>::<nested symboL>
// This would also allow
gen_internal gen_Token gen_parse_identifier(bool* possible_member_function)
{
	push_scope();

	gen_Token name                  = currtok;
	gen__ctx->parser.Scope->Name    = name.Text;

	gen_Macro* macro                = lookup_macro(currtok.Text);
	gen_b32    accept_as_identifier = macro && gen_bitfield_is_set(gen_MacroFlags, macro->Flags, MF_Allow_As_Identifier);
	gen_b32    is_decarator         = macro && gen_bitfield_is_set(gen_MacroFlags, macro->Flags, MF_Identifier_Decorator);

	// Typename can be: '::' <name>
	// If that is the case first  option will be Tok_Access_StaticSymbol below
	if (check(Tok_Identifier) || accept_as_identifier)
	{
		if (is_decarator)
		{
			gen_Code name_macro = gen_parse_simple_preprocess(currtok.Type);
			name.Text.Len       = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)name.Text.Ptr;
		}
		else
		{
			eat(Tok_Identifier);
		}
	}
	// <Name>

	gen_parse_template_args(&name);
	// <Name><Template Args>

	while (check(Tok_Access_StaticSymbol))
	{
		eat(Tok_Access_StaticSymbol);
		// <Qualifier Name> <Template Args> ::

		gen_Token invalid = gen_NullToken;
		if (left == 0)
		{
			gen_log_failure("Error, unexpected end of static symbol identifier\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return invalid;
		}

		if (currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '~')
		{
			bool is_destructor = gen_str_are_equal(gen__ctx->parser.Scope->Prev->ProcName, gen_txt("gen_parser_parse_destructor"));
			if (is_destructor)
			{
				name.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)name.Text.Ptr;
				gen_parser_pop(&gen__ctx->parser);
				return name;
			}

			gen_log_failure(
			    "Error, had a ~ operator after %SB but not a gen_destructor_\n%s",
			    gen_toktype_to_str(prevtok.Type),
			    gen_parser_to_strbuilder(gen__ctx->parser)
			);
			gen_parser_pop(&gen__ctx->parser);
			return invalid;
		}

		if (currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '*' && currtok.Text.Len == 1)
		{
			if (possible_member_function)
				*possible_member_function = true;

			else
			{
				gen_log_failure(
				    "Found a member function pointer identifier but the parsing context did not expect it\n%s",
				    gen_parser_to_strbuilder(gen__ctx->parser)
				);
				gen_parser_pop(&gen__ctx->parser);
				return invalid;
			}
		}

		if (currtok.Type != Tok_Identifier)
		{
			gen_log_failure(
			    "Error, expected static symbol identifier, not %s\n%s",
			    gen_toktype_to_str(currtok.Type),
			    gen_parser_to_strbuilder(gen__ctx->parser)
			);
			gen_parser_pop(&gen__ctx->parser);
			return invalid;
		}

		name.Text.Len = ((gen_sptr)currtok.Text.Ptr + currtok.Text.Len) - (gen_sptr)name.Text.Ptr;
		eat(Tok_Identifier);
		// <Qualifier Name> <Template Args> :: <Name>

		gen_parse_template_args(&name);
		// <Qualifier Name> <Template Args> :: <Name> <Template Args>
	}
	// <Qualifier Name> <Template Args> :: <Name> <Template Args> ...

	gen_parser_pop(&gen__ctx->parser);
	return name;
}

gen_internal gen_CodeInclude gen_parse_include()
{
	push_scope();

	gen_CodeInclude include = (gen_CodeInclude)gen_make_code();
	include->Type           = CT_Preprocess_Include;
	eat(Tok_Preprocess_Include);
	// #include

	if (! check(Tok_String))
	{
		gen_log_failure("Error, expected include string after #include\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	gen__ctx->parser.Scope->Name = currtok.Text;
	include->Content             = gen_cache_str(currtok.Text);
	eat(Tok_String);
	// #include <Path> or "Path"

	gen_parser_pop(&gen__ctx->parser);
	return include;
}

gen_internal gen_CodeOperator
    gen_parse_operator_after_ret_type(gen_ModuleFlag mflags, gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers, gen_CodeTypename ret_type)
{
	push_scope();

	gen_Token nspace = gen_NullToken;
	if (check(Tok_Identifier))
	{
		nspace = currtok;
		while (left && currtok.Type == Tok_Identifier)
		{
			eat(Tok_Identifier);

			if (currtok.Type == Tok_Access_StaticSymbol)
				eat(Tok_Access_StaticSymbol);
		}

		nspace.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)nspace.Text.Ptr;
	}
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...>

	eat(Tok_Decl_Operator);
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator

	if (! left && currtok.Type != Tok_Operator && currtok.Type != Tok_Star && currtok.Type != Tok_Ampersand && currtok.Type != Tok_Ampersand_DBL)
	{
		gen_log_failure("Expected operator after 'operator' keyword\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	gen__ctx->parser.Scope->Name = currtok.Text;

	bool was_new_or_delete       = false;

	gen_Operator op              = Op_Invalid;
	switch (currtok.Text.Ptr[0])
	{
		case '+':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_Add;

			else if (currtok.Text.Ptr[1] == '+')
				op = Op_Increment;

			else
				op = Op_Add;
		}
		break;
		case '-':
		{
			if (currtok.Text.Ptr[1] == '>')
			{
				if (currtok.Text.Ptr[2] == '*')
					op = Op_MemberOfPointer;

				else
					op = Op_MemberOfPointer;

				break;
			}

			else if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_Subtract;

			else
				op = Op_Subtract;
		}
		break;
		case '*':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_Multiply;

			else
			{
				gen_Token finder = prevtok;
				while (finder.Type != Tok_Decl_Operator)
				{
					if (finder.Type == Tok_Identifier)
					{
						op = Op_Indirection;
						break;
					}
				}

				if (op == Op_Invalid)
					op = Op_Multiply;
			}
		}
		break;
		case '/':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_Divide;

			else
				op = Op_Divide;
		}
		break;
		case '%':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_Modulo;

			else
				op = Op_Modulo;
		}
		break;
		case '&':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_BAnd;

			else if (currtok.Text.Ptr[1] == '&')
				op = Op_LAnd;

			else
			{


				if (op == Op_Invalid)
					op = Op_BAnd;
			}
		}
		break;
		case '|':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_BOr;

			else if (currtok.Text.Ptr[1] == '|')
				op = Op_LOr;

			else
				op = Op_BOr;
		}
		break;
		case '^':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_Assign_BXOr;

			else
				op = Op_BXOr;
		}
		break;
		case '~':
		{
			op = Op_BNot;
		}
		break;
		case '!':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_LNot;

			else
				op = Op_UnaryNot;
		}
		break;
		case '=':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_LEqual;

			else
				op = Op_Assign;
		}
		break;
		case '<':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_LesserEqual;

			else if (currtok.Text.Ptr[1] == '<')
			{
				if (currtok.Text.Ptr[2] == '=')
					op = Op_Assign_LShift;

				else
					op = Op_LShift;
			}
			else
				op = Op_Lesser;
		}
		break;
		case '>':
		{
			if (currtok.Text.Ptr[1] == '=')
				op = Op_GreaterEqual;

			else if (currtok.Text.Ptr[1] == '>')
			{
				if (currtok.Text.Ptr[2] == '=')
					op = Op_Assign_RShift;

				else
					op = Op_RShift;
			}
			else
				op = Op_Greater;
		}
		break;
		case '(':
		{
			if (currtok.Text.Ptr[1] == ')')
				op = Op_FunctionCall;

			else
				op = Op_Invalid;
		}
		break;
		case '[':
		{
			if (currtok.Text.Ptr[1] == ']')
				op = Op_Subscript;

			else
				op = Op_Invalid;
		}
		break;
		default:
		{
			gen_Str gen_c_str_new    = gen_operator_to_str(Op_New);
			gen_Str gen_c_str_delete = gen_operator_to_str(Op_Delete);
			if (gen_c_str_compare_len(currtok.Text.Ptr, gen_c_str_new.Ptr, gen_max(gen_c_str_new.Len - 1, currtok.Text.Len)) == 0)
			{
				op = Op_New;
				eat(Tok_Identifier);
				was_new_or_delete = true;

				gen_s32 idx       = gen__ctx->parser.Tokens.Idx + 1;
				{
					while (gen__ctx->parser.Tokens.Arr[idx].Type == Tok_NewLine)
						idx++;
				}
				gen_Token next = gen__ctx->parser.Tokens.Arr[idx];
				if (currtok.Type == Tok_Operator && gen_c_str_compare_len(currtok.Text.Ptr, "[]", 2) == 0)
				{
					eat(Tok_Operator);
					op = Op_NewArray;
				}
				else if (currtok.Type == Tok_BraceSquare_Open && next.Type == Tok_BraceSquare_Close)
				{
					eat(Tok_BraceSquare_Open);
					eat(Tok_BraceSquare_Close);
					op = Op_NewArray;
				}
			}
			else if (gen_c_str_compare_len(currtok.Text.Ptr, gen_c_str_delete.Ptr, gen_max(gen_c_str_delete.Len - 1, currtok.Text.Len)) == 0)
			{
				op = Op_Delete;
				eat(Tok_Identifier);
				was_new_or_delete = true;

				gen_s32 idx       = gen__ctx->parser.Tokens.Idx + 1;
				{
					while (gen__ctx->parser.Tokens.Arr[idx].Type == Tok_NewLine)
						idx++;
				}
				gen_Token next = gen__ctx->parser.Tokens.Arr[idx];
				if (currtok.Type == Tok_Operator && gen_c_str_compare_len(currtok.Text.Ptr, "[]", 2) == 0)
				{
					eat(Tok_Operator);
					op = Op_DeleteArray;
				}
				else if (currtok.Type == Tok_BraceSquare_Open && next.Type == Tok_BraceSquare_Close)
				{
					eat(Tok_BraceSquare_Open);
					eat(Tok_BraceSquare_Close);
					op = Op_DeleteArray;
				}
			}
			else
			{
				if (op == Op_Invalid)
				{
					gen_log_failure("Invalid operator '%s'\n%s", prevtok.Text, gen_parser_to_strbuilder(gen__ctx->parser));
					gen_parser_pop(&gen__ctx->parser);
					return gen_InvalidCode;
				}
			}
		}
	}

	if (op == Op_Invalid)
	{
		gen_log_failure("Invalid operator '%s'\n%s", currtok.Text, gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	if (! was_new_or_delete)
		eat(currtok.Type);
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op>

	// Parse Params
	gen_CodeParams params = gen_parse_params(gen_parser_use_parenthesis);
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> )

	if (params == gen_nullptr && op == Op_Multiply)
		op = Op_MemberOfPointer;

	while (left && gen_tok_is_specifier(currtok))
	{
		if (specifiers == gen_nullptr)
		{
			specifiers = gen_def_specifier(gen_str_to_specifier(currtok.Text));
			eat(currtok.Type);
			continue;
		}

		gen_specifiers_append(specifiers, gen_str_to_specifier(currtok.Text));
		eat(currtok.Type);
	}
	// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers>

	// TODO(Ed): Add proper "delete" and "new" awareness
	// We're dealing with either a "= delete" or operator deletion
	if (check(Tok_Operator) && currtok.Text.Ptr[0] == '=')
	{
		eat(currtok.Type);
		if (! gen_str_are_equal(currtok.Text, gen_txt("delete")))
		{
			gen_log_failure("Expected delete after = in operator forward instead found \"%S\"\n%SB", currtok.Text, gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		if (specifiers == gen_nullptr)
			specifiers = gen_def_specifier(Spec_Delete);
		else
			gen_specifiers_append(specifiers, Spec_Delete);
		eat(currtok.Type);
		// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers> = delete
	}

	// Parse Body
	gen_CodeBody    body       = { gen_nullptr };
	gen_CodeComment inline_cmt = gen_NullCode;
	if (check(Tok_BraceCurly_Open))
	{
		body = gen_cast(gen_CodeBody, gen_parse_function_body());
		if (gen_cast(gen_Code, body) == gen_Code_Invalid)
		{
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers> { ... }
	}
	else
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers>;

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// <ExportFlag> <Attributes> <Specifiers> <ReturnType> <Qualifier::...> operator <Op> ( <Parameters> ) <Specifiers>; <InlineCmt>
	}

	// OpValidateResult check_result = gen_operator__validate( op, params, ret_type, specifiers );
	gen_CodeOperator result = gen_def_operator(op, nspace.Text, gen_def_assign(params, ret_type, body, specifiers, attributes, mflags));

	if (inline_cmt)
		result->InlineCmt = inline_cmt;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_Code gen_parse_operator_function_or_variable(bool expects_function, gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers)
{
	push_scope();

	gen_Code result         = gen_InvalidCode;

	gen_Code gen_macro_stmt = gen_parse_macro_as_definiton(attributes, specifiers);
	if (gen_macro_stmt)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_macro_stmt;
	}

	gen_CodeTypename type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
	// <Attributes> <Specifiers> <ReturnType/ValueType>

	// Thanks Unreal
	gen_CodeAttributes post_rt_attributes = gen_parse_attributes();
	if (post_rt_attributes)
	{
		if (attributes)
		{
			gen_StrBuilder merged = gen_strbuilder_fmt_buf(gen__ctx->Allocator_Temp, "%S %S", attributes->Content, post_rt_attributes->Content);
			attributes->Content   = gen_cache_str(gen_strbuilder_to_str(merged));
		}
		else
		{
			attributes = post_rt_attributes;
		}
		// <Attributes> <Specifiers> <ReturnType/ValueType> <Attributes>
		// CONVERTED TO:
		// <Attributes> <Specifiers> <ReturnType/ValueType>
	}

	if (type == gen_InvalidCode)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	bool    found_operator = false;
	gen_s32 idx            = gen__ctx->parser.Tokens.Idx;

	for (; idx < gen_array_num(gen__ctx->parser.Tokens.Arr); idx++)
	{
		gen_Token tok = gen__ctx->parser.Tokens.Arr[idx];

		if (tok.Type == Tok_Identifier)
		{
			idx++;
			tok = gen__ctx->parser.Tokens.Arr[idx];
			if (tok.Type == Tok_Access_StaticSymbol)
				continue;

			break;
		}

		if (tok.Type == Tok_Decl_Operator)
			found_operator = true;

		break;
	}

	if (found_operator)
	{
		// Dealing with an operator overload
		result = gen_cast(gen_Code, gen_parse_operator_after_ret_type(ModuleFlag_None, attributes, specifiers, type));
		// <Attributes> <Specifiers> <ReturnType> operator ...
	}
	else
	{
		gen_Token name               = gen_parse_identifier(gen_nullptr);
		gen__ctx->parser.Scope->Name = name.Text;

		bool detected_capture        = check(Tok_Paren_Open);

		// Check three tokens ahead to make sure that were not dealing with a gen_constructor_ initialization...
		//                  (         350.0f    ,         <---  Could be the scenario
		// Example : <Capture_Start> <Value> <Comma>
		//                 idx         +1      +2
		bool detected_comma                         = gen__ctx->parser.Tokens.Arr[gen__ctx->parser.Tokens.Idx + 2].Type == Tok_Comma;

		gen_b32 detected_non_varadic_unpaired_param = detected_comma && nexttok.Type != Tok_Varadic_Argument;
		if (! detected_non_varadic_unpaired_param && nexttok.Type == Tok_Preprocess_Macro_Expr)
			for (gen_s32 break_scope = 0; break_scope == 0; ++break_scope)
			{
				gen_Macro* macro = lookup_macro(nexttok.Text);
				if (macro == gen_nullptr || ! gen_macro_is_functional(*macro))
					break;

				// (   <Macro_Expr> (
				// Idx      +1     +2
				gen_s32 idx   = gen__ctx->parser.Tokens.Idx + 1;
				gen_s32 level = 0;

				// Find end of the token expression
				for (; idx < gen_array_num(gen__ctx->parser.Tokens.Arr); idx++)
				{
					gen_Token tok = gen__ctx->parser.Tokens.Arr[idx];

					if (tok.Type == Tok_Paren_Open)
						level++;
					else if (tok.Type == Tok_Paren_Close && level > 0)
						level--;
					if (level == 0 && tok.Type == Tok_Paren_Close)
						break;
				}
				++idx;    // Will incremnt to possible comma position

				if (gen__ctx->parser.Tokens.Arr[idx].Type != Tok_Comma)
					break;

				detected_non_varadic_unpaired_param = true;
			}

		if (detected_capture && ! detected_non_varadic_unpaired_param)
		{
			// Dealing with a function
			result = gen_cast(gen_Code, gen_parse_function_after_name(ModuleFlag_None, attributes, specifiers, type, name));
			// <Attributes> <Specifiers> <ReturnType> <Name> ( ...
		}
		else
		{
			if (expects_function)
			{
				gen_log_failure("Expected function declaration (consteval was used)\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}
			// Dealing with a variable
			result = gen_cast(gen_Code, gen_parse_variable_after_name(ModuleFlag_None, attributes, specifiers, type, name.Text));
			// <Attributes> <Specifiers> <ValueType> <Name> ...
		}
	}

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_Code gen_parse_macro_as_definiton(gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers)
{
	push_scope();

	if (currtok.Type != Tok_Preprocess_Macro_Stmt)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_NullCode;
	}
	gen_Macro* macro                     = lookup_macro(currtok.Text);
	gen_b32    can_resolve_to_definition = macro && gen_bitfield_is_set(gen_MacroFlags, macro->Flags, MF_Allow_As_Definition);
	if (! can_resolve_to_definition)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_NullCode;
	}

	// TODO(Ed): When gen_AST_Macro is made, have it support attributs and specifiers for when its behaving as a declaration/definition.
	gen_Code code = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Stmt);

	// Attributes and sepcifiers will be collapsed into the macro's serialization.
	gen_StrBuilder resolved_definition = gen_strbuilder_fmt_buf(
	    gen__ctx->Allocator_Temp,
	    "%S %S %S",
	    attributes ? gen_strbuilder_to_str(gen_attributes_to_strbuilder(attributes)) : gen_txt(""),
	    specifiers ? gen_strbuilder_to_str(gen_specifiers_to_strbuilder(specifiers)) : gen_txt(""),
	    code->Content
	);
	gen_Code result = gen_untyped_str(gen_strbuilder_to_str(resolved_definition));
	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodePragma gen_parse_pragma()
{
	push_scope();

	gen_CodePragma pragma = (gen_CodePragma)gen_make_code();
	pragma->Type          = CT_Preprocess_Pragma;
	eat(Tok_Preprocess_Pragma);
	// #pragma

	if (! check(Tok_Preprocess_Content))
	{
		gen_log_failure("Error, expected content after #pragma\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	gen__ctx->parser.Scope->Name = currtok.Text;

	pragma->Content              = gen_cache_str(currtok.Text);
	eat(Tok_Preprocess_Content);
	// #pragma <Content>

	gen_parser_pop(&gen__ctx->parser);
	return pragma;
}

gen_internal inline gen_CodeParams gen_parse_params(bool use_template_capture)
{
	push_scope();

	if (! use_template_capture)
	{
		eat(Tok_Paren_Open);
		// (
	}
	else
	{
		if (check(Tok_Operator) && currtok.Text.Ptr[0] == '<')
			eat(Tok_Operator);
		// <
	}

	if (! use_template_capture && check(Tok_Paren_Close))
	{
		eat(Tok_Paren_Close);
		// )
		gen_parser_pop(&gen__ctx->parser);
		return gen_NullCode;
	}
	else if (check(Tok_Operator) && currtok.Text.Ptr[0] == '>')
	{
		eat(Tok_Operator);
		// >
		gen_parser_pop(&gen__ctx->parser);
		return gen_NullCode;
	}

	gen_Code         macro           = { gen_nullptr };
	gen_CodeTypename type            = { gen_nullptr };
	gen_Code         value           = { gen_nullptr };
	gen_Token        name            = gen_NullToken;
	gen_Code         post_name_macro = { gen_nullptr };

	if (check(Tok_Varadic_Argument))
	{
		eat(Tok_Varadic_Argument);
		// ( or <  ...

		gen_parser_pop(&gen__ctx->parser);
		return gen_param_varadic;
		// ( ... )
		// or < ... >
	}

#define CheckEndParams() (use_template_capture ? (currtok.Text.Ptr[0] != '>') : (currtok.Type != Tok_Paren_Close))

	// TODO(Ed): Use expression macros or this? macro as attribute?
	// Ex: Unreal has this type of macro:                 vvvvvvvvv
	// COREUOBJECT_API void CallFunction( FFrame& Stack, RESULT_DECL, UFunction* Function );
	// and:                 vvvv
	// AddComponentByClass(UPARAM(meta = (AllowAbstract = "false")) TSubclassOf<UActorComponent> Class, bool bManualAttachment, ...
	if (check(Tok_Preprocess_Macro_Expr))
	{
		macro = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Expr);
		// ( <gen_Macro>
	}
	if (currtok.Type != Tok_Comma)
	{
		type = gen_parser_parse_type(use_template_capture, gen_nullptr);
		if (gen_cast(gen_Code, type) == gen_Code_Invalid)
		{
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		// ( <gen_Macro> <ValueType>

		if (check(Tok_Identifier) || gen_bitfield_is_set(gen_u32, currtok.Flags, TF_Identifier))
		{
			name = currtok;
			eat(currtok.Type);
			// ( <gen_Macro> <ValueType> <Name>
		}

		// TODO(Ed): Use expression macro for this?
		// Unreal has yet another type of macro:
		// template<class T UE_REQUIRES(TPointerIsConvertibleFromTo<T, UInterface>::Value)>
		// class T ... and then ^this^ UE_REQUIRES shows up
		// So we need to consume that.
		if (check(Tok_Preprocess_Macro_Expr))
		{
			post_name_macro = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Expr);
			// ( <gen_Macro> <ValueType> <Name> <PostNameMacro>
		}

		// C++ allows typename = expression... so anything goes....
		if (gen_bitfield_is_set(gen_u32, currtok.Flags, TF_Assign))
		{
			eat(Tok_Operator);
			// ( <gen_Macro> <ValueType> <Name>  =

			gen_Token value_tok = currtok;

			if (currtok.Type == Tok_Comma)
			{
				gen_log_failure("Expected value after assignment operator\n%s.", gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}

			gen_s32 capture_level      = 0;
			gen_s32 gen_template_level = 0;
			while ((left && (currtok.Type != Tok_Comma) && gen_template_level >= 0 && CheckEndParams()) || (capture_level > 0 || gen_template_level > 0))
			{
				if (currtok.Text.Ptr[0] == '<')
					++gen_template_level;

				if (currtok.Text.Ptr[0] == '>')
					--gen_template_level;
				if (currtok.Type == Tok_Operator && currtok.Text.Ptr[1] == '>')
					--gen_template_level;

				if (currtok.Type == Tok_Paren_Open)
					++capture_level;

				if (currtok.Type == Tok_Paren_Close)
					--capture_level;

				value_tok.Text.Len = ((gen_sptr)currtok.Text.Ptr + currtok.Text.Len) - (gen_sptr)value_tok.Text.Ptr;
				eat(currtok.Type);
			}

			value = gen_untyped_str(gen_strbuilder_to_str(gen_parser_strip_formatting(value_tok.Text, gen_parser_strip_formatting_dont_preserve_newlines)));
			// ( <gen_Macro> <ValueType> <Name> = <Expression>
		}
	}

	gen_CodeParams result = (gen_CodeParams)gen_make_code();
	result->Type          = CT_Parameters;

	result->gen_Macro     = macro;

	if (name.Text.Len > 0)
		result->Name = gen_cache_str(name.Text);

	result->ValueType = type;

	if (value)
		result->Value = value;

	result->NumEntries++;

	while (check(Tok_Comma))
	{
		eat(Tok_Comma);
		// ( <gen_Macro> <ValueType> <Name> = <Expression>,

		gen_Code type  = { gen_nullptr };
		gen_Code value = { gen_nullptr };

		if (check(Tok_Varadic_Argument))
		{
			eat(Tok_Varadic_Argument);
			gen_params_append(result, gen_param_varadic);
			continue;
			// ( <gen_Macro> <ValueType> <Name> = <Expression>, ...
		}

		// Ex: Unreal has this type of macro:                 vvvvvvvvv
		// COREUOBJECT_API void CallFunction( FFrame& Stack, RESULT_DECL, UFunction* Function );
		// and:                 vvvv
		// AddComponentByClass(UPARAM(meta = (AllowAbstract = "false")) TSubclassOf<UActorComponent> Class, bool bManualAttachment, ...
		if (check(Tok_Preprocess_Macro_Expr))
		{
			macro = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Expr);
			// ( <gen_Macro> <ValueType> <Name> = <Expression>, <gen_Macro>
		}
		if (currtok.Type != Tok_Comma)
		{
			type = gen_cast(gen_Code, gen_parser_parse_type(use_template_capture, gen_nullptr));
			if (type == gen_Code_Invalid)
			{
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}
			// ( <gen_Macro> <ValueType> <Name> = <Expression>, <gen_Macro> <ValueType>

			name = gen_NullToken;

			if (check(Tok_Identifier) || gen_bitfield_is_set(gen_u32, currtok.Flags, TF_Identifier))
			{
				name = currtok;
				eat(currtok.Type);
				// ( <gen_Macro> <ValueType> <Name> = <Expression>, <gen_Macro> <ValueType> <Name>
			}

			// Unreal has yet another type of macro:
			// template<class T UE_REQUIRES(TPointerIsConvertibleFromTo<T, UInterface>::Value)>
			// class T ... and then ^this^ UE_REQUIRES shows up
			// So we need to consume that.
			if (check(Tok_Preprocess_Macro_Expr))
			{
				post_name_macro = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Expr);
				// ( <gen_Macro> <ValueType> <Name> = <Expression>, <gen_Macro> <ValueType> <PostNameMacro>
			}

			/// C++ allows typename = expression... so anything goes....
			if (gen_bitfield_is_set(gen_u32, currtok.Flags, TF_Assign))
			{
				eat(Tok_Operator);
				// ( <gen_Macro> <ValueType> <Name> = <Expression>, <gen_Macro> <ValueType> <Name> <PostNameMacro> =

				gen_Token value_tok = currtok;

				if (currtok.Type == Tok_Comma)
				{
					gen_log_failure("Expected value after assignment operator\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
					gen_parser_pop(&gen__ctx->parser);
					return gen_InvalidCode;
				}

				gen_s32 capture_level      = 0;
				gen_s32 gen_template_level = 0;
				while ((left && currtok.Type != Tok_Comma && gen_template_level >= 0 && CheckEndParams()) || (capture_level > 0 || gen_template_level > 0))
				{
					if (currtok.Text.Ptr[0] == '<')
						++gen_template_level;

					if (currtok.Text.Ptr[0] == '>')
						--gen_template_level;
					if (currtok.Type == Tok_Operator && currtok.Text.Ptr[1] == '>')
						--gen_template_level;

					if (currtok.Type == Tok_Paren_Open)
						++capture_level;

					if (currtok.Type == Tok_Paren_Close)
						--capture_level;

					value_tok.Text.Len = ((gen_sptr)currtok.Text.Ptr + currtok.Text.Len) - (gen_sptr)value_tok.Text.Ptr;
					eat(currtok.Type);
				}

				value = gen_untyped_str(gen_strbuilder_to_str(gen_parser_strip_formatting(value_tok.Text, gen_parser_strip_formatting_dont_preserve_newlines)));
				// ( <gen_Macro> <ValueType> <Name> = <Expression>, <gen_Macro> <ValueType> <Name> <PostNameMacro> = <Expression>
			}
			// ( <gen_Macro> <ValueType> <Name> = <Expression>, <gen_Macro> <ValueType> <Name> <PostNameMacro> = <Expression>, ..
		}

		gen_CodeParams param = (gen_CodeParams)gen_make_code();
		param->Type          = CT_Parameters;

		param->gen_Macro     = macro;

		if (name.Text.Len > 0)
			param->Name = gen_cache_str(name.Text);

		param->PostNameMacro = post_name_macro;
		param->ValueType     = gen_cast(gen_CodeTypename, type);

		if (value)
			param->Value = value;

		gen_params_append(result, param);
	}

	if (! use_template_capture)
	{
		eat(Tok_Paren_Close);
		// ( <gen_Macro> <ValueType> <Name> <PostNameMacro> = <Expression>, <gen_Macro> <ValueType> <Name> <PostNameMacro> = <Expression>, .. )
	}
	else
	{
		if (! check(Tok_Operator) || currtok.Text.Ptr[0] != '>')
		{
			gen_log_failure("Expected '<' after 'template' keyword\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		eat(Tok_Operator);
		// < <gen_Macro> <ValueType> <Name> <PostNameMacro> = <Expression>, <gen_Macro> <ValueType> <Name> <PostNameMacro> = <Expression>, .. >
	}
	gen_parser_pop(&gen__ctx->parser);
	return result;
#undef context
}

gen_internal gen_CodePreprocessCond gen_parse_preprocess_cond()
{
	push_scope();

	if (! gen_tok_is_preprocess_cond(currtok))
	{
		gen_log_failure("Error, expected preprocess conditional\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	gen_CodePreprocessCond cond = (gen_CodePreprocessCond)gen_make_code();
	cond->Type                  = gen_scast(gen_CodeType, currtok.Type - (Tok_Preprocess_If - CT_Preprocess_If));
	eat(currtok.Type);
	// #<Conditional>

	if (! check(Tok_Preprocess_Content))
	{
		gen_log_failure("Error, expected content after #define\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	gen__ctx->parser.Scope->Name = currtok.Text;
	cond->Content                = gen_cache_str(currtok.Text);
	eat(Tok_Preprocess_Content);
	// #<Conditiona> <Content>

	gen_parser_pop(&gen__ctx->parser);
	return cond;
}

gen_internal gen_Code gen_parse_simple_preprocess(gen_TokType which)
{
	// TODO(Ed): We can handle a macro a gen_bit better than this. It's gen_AST can be made more robust..
	// Make an gen_AST_Macro, it should have an Name be the macro itself, with the function body being an optional function body node.
	// If we want it to terminate or have an inline comment we can possbily use its parent typedef for that info...
	push_scope();

	gen_Token full_macro = currtok;
	eat(which);
	// <gen_Macro>

	gen_Macro* macro = lookup_macro(full_macro.Text);
	if (which != Tok_Preprocess_Unsupported && macro == gen_nullptr)
	{
		gen_log_failure("Expected the macro %S to be registered\n%S", full_macro, gen_parser_to_strbuilder(gen__ctx->parser));
		return gen_NullCode;
	}

	// TODO(Ed) : Parse this properly later (expression and statement support)
	if (macro && gen_macro_is_functional(*macro))
	{
		eat(Tok_Paren_Open);

		gen_s32 level = 0;
		while (left && (currtok.Type != Tok_Paren_Close || level > 0))
		{
			if (currtok.Type == Tok_Paren_Open)
				level++;

			else if (currtok.Type == Tok_Paren_Close && level > 0)
				level--;

			eat(currtok.Type);
		}
		eat(Tok_Paren_Close);
		// <gen_Macro> ( <params> )

		full_macro.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)full_macro.Text.Ptr;
	}

	if (macro && gen_macro_expects_body(*macro) && peektok.Type == Tok_BraceCurly_Open)
	{
		// Eat the block scope right after the macro. Were assuming the macro defines a function definition's signature
		eat(Tok_BraceCurly_Open);
		// <gen_Macro> {

		// TODO(Ed) : Parse this properly later (expression and statement support)
		gen_s32 level = 0;
		while (left && (currtok.Type != Tok_BraceCurly_Close || level > 0))
		{
			if (currtok.Type == Tok_BraceCurly_Open)
				level++;

			else if (currtok.Type == Tok_BraceCurly_Close && level > 0)
				level--;

			eat(currtok.Type);
		}
		eat(Tok_BraceCurly_Close);
		// <gen_Macro> { <Body> }

		// TODO(Ed): Review this?
		gen_Str prev_proc = gen__ctx->parser.Scope->Prev->ProcName;
		if (macro->Type == MT_Typename && gen_c_str_compare_len(prev_proc.Ptr, "gen_parser_parse_typedef", prev_proc.Len) != 0)
		{
			if (check(Tok_Statement_End))
			{
				gen_Token stmt_end = currtok;
				eat(Tok_Statement_End);
				// <gen_Macro> { <Content> };

				if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
					eat(Tok_Comment);
				// <gen_Macro> { <Content> }; <InlineCmt>
			}
		}

		full_macro.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)full_macro.Text.Ptr;
	}
	else
	{
		// If the macro is just a macro in the body of an gen_AST it may have a semi-colon for the user to close on purpsoe
		// (especially for functional macros)
		gen_Str calling_proc = gen__ctx->parser.Scope->Prev->ProcName;

		if (gen_str_contains(gen__ctx->parser.Scope->Prev->ProcName, gen_txt("gen_parser_parse_enum")))
		{
			// Do nothing
			goto Leave_Scope_Early;
		}
		else if (macro && macro->Type == MT_Typename && gen_str_contains(gen__ctx->parser.Scope->Prev->ProcName, gen_txt("gen_parser_parse_typedef")))
		{
			if (peektok.Type == Tok_Statement_End)
			{
				gen_Token stmt_end = currtok;
				eat(Tok_Statement_End);
				// <gen_Macro>;

				full_macro.Text.Len += prevtok.Text.Len;

				// TODO(Ed): Reveiw the context for this? (ESPECIALLY THIS)
				if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
				{
					eat(Tok_Comment);
					// <gen_Macro>; <InlineCmt>

					full_macro.Text.Len += prevtok.Text.Len;
				}
			}
		}
		else if (gen_str_contains(calling_proc, gen_txt("gen_parse_global_nspace")) || gen_str_contains(calling_proc, gen_txt("gen_parse_class_struct_body")))
		{
			if (left && peektok.Type == Tok_Statement_End)
			{
				gen_Token stmt_end = currtok;
				eat(Tok_Statement_End);
				// <gen_Macro>;
				full_macro.Text.Len += prevtok.Text.Len;
			}
		}
	}

Leave_Scope_Early:
	gen_Code result              = gen_untyped_str(full_macro.Text);
	gen__ctx->parser.Scope->Name = full_macro.Text;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_Code gen_parse_static_assert()
{
	push_scope();

	gen_Code assert              = gen_make_code();
	assert->Type                 = CT_Untyped;

	gen_Token content            = currtok;

	gen__ctx->parser.Scope->Name = content.Text;

	eat(Tok_StaticAssert);
	eat(Tok_Paren_Open);
	// gen_static_assert(

	// TODO(Ed) : Parse this properly.
	gen_s32 level = 0;
	while (left && (currtok.Type != Tok_Paren_Close || level > 0))
	{
		if (currtok.Type == Tok_Paren_Open)
			level++;
		else if (currtok.Type == Tok_Paren_Close)
			level--;

		eat(currtok.Type);
	}
	eat(Tok_Paren_Close);
	eat(Tok_Statement_End);
	// gen_static_assert( <Content> );

	content.Text.Len        = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)content.Text.Ptr;

	char const* str         = gen_c_str_fmt_buf("%.*s\n", content.Text.Len, content.Text.Ptr);
	gen_Str     content_str = { str, content.Text.Len + 1 };
	assert->Content         = gen_cache_str(content_str);
	assert->Name            = assert->Content;

	gen_parser_pop(&gen__ctx->parser);
	return assert;
}

/*
	This a brute-froce make all the arguments part of the token provided.
	Can have in-place function signatures, regular identifiers, in-place typenames, compile-time expressions, parameter-pack expansion, etc.
	This means that validation can only go so far, and so if there is any different in formatting
	passed the basic stripping supported it report a soft failure.
*/
gen_internal inline void gen_parse_template_args(gen_Token* token)
{
	if (currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '<' && currtok.Text.Len == 1)
	{
		eat(Tok_Operator);
		// <

		gen_s32 level = 0;
		while (left && level >= 0 && (currtok.Text.Ptr[0] != '>' || level > 0))
		{
			if (currtok.Text.Ptr[0] == '<')
				level++;

			if (currtok.Text.Ptr[0] == '>')
				level--;
			if (currtok.Type == Tok_Operator && currtok.Text.Ptr[1] == '>')
				level--;

			eat(currtok.Type);
		}
		// < <Content>

		// Due to the >> token, this could have been eaten early...
		if (level == 0)
			eat(Tok_Operator);
		// < <Content> >

		// Extend length of name to last token
		token->Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)token->Text.Ptr;
	}
}

// Variable parsing is handled in multiple places because its initial signature is shared with function parsing
gen_internal gen_CodeVar
    gen_parse_variable_after_name(gen_ModuleFlag mflags, gen_CodeAttributes attributes, gen_CodeSpecifiers specifiers, gen_CodeTypename type, gen_Str name)
{
	push_scope();

	gen_Code gen_array_expr                   = gen_parse_array_decl();
	gen_Code expr                             = gen_NullCode;
	gen_Code bitfield_expr                    = gen_NullCode;

	gen_b32 gen_using_constructor_initializer = false;

	if (gen_bitfield_is_set(gen_u32, currtok.Flags, TF_Assign))
	{
		// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>
		expr = gen_parse_assignment_expression();
	}

	if (currtok.Type == Tok_BraceCurly_Open)
	{
		gen_Token expr_tok = currtok;

		eat(Tok_BraceCurly_Open);
		// <Attributes> <Specifiers> <ValueType> <Name> {

		gen_s32 level = 0;
		while (left && (currtok.Type != Tok_BraceCurly_Close || level > 0))
		{
			if (currtok.Type == Tok_BraceCurly_Open)
				level++;

			else if (currtok.Type == Tok_BraceCurly_Close && level > 0)
				level--;

			eat(currtok.Type);
		}
		eat(Tok_BraceCurly_Close);

		expr_tok.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)expr_tok.Text.Ptr;
		expr              = gen_untyped_str(expr_tok.Text);
		// <Attributes> <Specifiers> <ValueType> <Name> = { <Expression> }
	}

	if (currtok.Type == Tok_Paren_Open)
	{
		eat(Tok_Paren_Open);
		// <Attributes> <Specifiers> <ValueType> <Name> (

		gen_Token expr_token              = currtok;

		gen_using_constructor_initializer = true;

		gen_s32 level                     = 0;
		while (left && (currtok.Type != Tok_Paren_Close || level > 0))
		{
			if (currtok.Type == Tok_Paren_Open)
				level++;

			else if (currtok.Type == Tok_Paren_Close && level > 0)
				level--;

			eat(currtok.Type);
		}

		expr_token.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)expr_token.Text.Ptr;
		expr                = gen_untyped_str(expr_token.Text);
		eat(Tok_Paren_Close);
		// <Attributes> <Specifiers> <ValueType> <Name> ( <Expression> )
	}

	if (currtok.Type == Tok_Assign_Classifer)
	{
		eat(Tok_Assign_Classifer);
		// <Attributes> <Specifiers> <ValueType> <Name> :

		gen_Token expr_tok = currtok;

		if (currtok.Type == Tok_Statement_End)
		{
			gen_log_failure("Expected expression after bitfield \n%SB", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		while (left && currtok.Type != Tok_Statement_End)
		{
			eat(currtok.Type);
		}

		expr_tok.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)expr_tok.Text.Ptr;
		bitfield_expr     = gen_untyped_str(expr_tok.Text);
		// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>
	}

	gen_CodeVar     gen_next_var = gen_NullCode;
	gen_Token       stmt_end     = gen_NullToken;
	gen_CodeComment inline_cmt   = gen_NullCode;
	if (type)
	{
		if (currtok.Type == Tok_Comma)
		{
			// Were dealing with a statement with more than one declaration
			// This is only handled this way if its the first declaration
			// Otherwise its looped through in gen_parse_variable_declaration_list
			gen_next_var = gen_parse_variable_declaration_list();
			// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>, ...
			// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>, ...
			// <Attributes> <Specifiers> <ValueType> <Name> { <Expression> }, ...
		}

		// If we're dealing with a "comma-procedding then we cannot expect a statement end or inline comment
		// Any comma procedding variable will not have a type provided so it can act as a indicator to skip this
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>, ...;
		// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>, ...;
		// <Attributes> <Specifiers> <ValueType> <Name> { <Expression> }, ...;

		// Check for inline comment : <type> <identifier> = <expression>; // <inline comment>
		if (left && (currtok_noskip.Type == Tok_Comment) && currtok_noskip.Line == stmt_end.Line)
		{
			inline_cmt = gen_parse_comment();
			// <Attributes> <Specifiers> <ValueType> <Name> : <Expression>, ...; <InlineCmt>
			// <Attributes> <Specifiers> <ValueType> <Name> = <Expression>, ...; <InlineCmt>
			// <Attributes> <Specifiers> <ValueType> <Name> { <Expression> }, ...; <InlineCmt>
		}
	}

	gen_CodeVar result   = (gen_CodeVar)gen_make_code();
	result->Type         = CT_Variable;
	result->Name         = gen_cache_str(name);
	result->ModuleFlags  = mflags;
	result->ValueType    = type;
	result->BitfieldSize = bitfield_expr;
	result->Attributes   = attributes;
	result->Specs        = specifiers;
	result->Value        = expr;
	result->InlineCmt    = inline_cmt;

	if (gen_array_expr)
		type->ArrExpr = gen_array_expr;

	if (gen_next_var)
	{
		result->NextVar         = gen_next_var;
		result->NextVar->Parent = gen_cast(gen_Code, result);
	}
	result->VarParenthesizedInit = gen_using_constructor_initializer;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

/*
	Note(Ed): This does not support the following:
	* Function Pointers
*/
gen_internal gen_CodeVar gen_parse_variable_declaration_list()
{
	push_scope();

	gen_CodeVar result   = gen_NullCode;
	gen_CodeVar last_var = gen_NullCode;
	while (check(Tok_Comma))
	{
		eat(Tok_Comma);
		// ,

		gen_CodeSpecifiers specifiers = gen_NullCode;

		while (left && gen_tok_is_specifier(currtok))
		{
			gen_Specifier spec = gen_str_to_specifier(currtok.Text);

			switch (spec)
			{
				case Spec_Const:
					if (specifiers->NumEntries && specifiers->ArrSpecs[specifiers->NumEntries - 1] != Spec_Ptr)
					{
						gen_log_failure(
						    "Error, const specifier must come after pointer specifier for variable declaration proceeding comma\n"
						    "(Parser will add and continue to specifiers, but will most likely fail to compile)\n%SB",
						    gen_parser_to_strbuilder(gen__ctx->parser)
						);

						gen_specifiers_append(specifiers, spec);
					}
					break;

				case Spec_Ptr:
				case Spec_Ref:
				case Spec_RValue:
					break;

				default:
				{
					gen_log_failure(
					    "Error, invalid specifier '%S' proceeding comma\n"
					    "(Parser will add and continue to specifiers, but will most likely fail to compile)\n%S",
					    currtok.Text,
					    gen_strbuilder_to_str(gen_parser_to_strbuilder(gen__ctx->parser))
					);
					continue;
				}
				break;
			}

			if (specifiers)
				gen_specifiers_append(specifiers, spec);
			else
				specifiers = gen_def_specifier(spec);

			eat(currtok.Type);
		}
		// , <Specifiers>

		gen_Str name = currtok.Text;
		eat(Tok_Identifier);
		// , <Specifiers> <Name>

		gen_CodeVar var = gen_parse_variable_after_name(ModuleFlag_None, gen_NullCode, specifiers, gen_NullCode, name);
		// , <Specifiers> <Name> ...

		if (! result)
		{
			result   = var;
			last_var = var;
		}
		else
		{
			last_var->NextVar         = var;
			last_var->NextVar->Parent = gen_cast(gen_Code, var);
			last_var                  = var;
		}
	}

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeClass gen_parser_parse_class(bool inplace_def)
{
	push_scope();
	gen_CodeClass result = (gen_CodeClass)gen_parse_class_struct(Tok_Decl_Class, inplace_def);
	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeConstructor gen_parser_parse_constructor(gen_CodeSpecifiers specifiers)
{
	push_scope();

	gen_Token      identifier = gen_parse_identifier(gen_nullptr);
	gen_CodeParams params     = gen_parse_params(gen_parser_not_from_template);
	// <Name> ( <Parameters> )

	gen_Code        initializer_list = gen_NullCode;
	gen_CodeBody    body             = gen_NullCode;
	gen_CodeComment inline_cmt       = gen_NullCode;

	// TODO(Ed) : Need to support postfix specifiers

	if (check(Tok_Assign_Classifer))
	{
		eat(Tok_Assign_Classifer);
		// <Name> ( <Parameters> ) :

		gen_Token initializer_list_tok = currtok;

		gen_s32 level                  = 0;
		while (left && (currtok.Type != Tok_BraceCurly_Open || level > 0))
		{
			if (currtok.Type == Tok_Paren_Open)
				level++;
			else if (currtok.Type == Tok_Paren_Close)
				level--;

			eat(currtok.Type);
		}

		initializer_list_tok.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)initializer_list_tok.Text.Ptr;
		// <Name> ( <Parameters> ) : <InitializerList>

		initializer_list = gen_untyped_str(initializer_list_tok.Text);

		// TODO(Ed): Constructors can have post-fix specifiers

		body = gen_cast(gen_CodeBody, gen_parse_function_body());
		// <Name> ( <Parameters> ) : <InitializerList> { <Body> }
	}
	else if (check(Tok_BraceCurly_Open))
	{
		body = gen_cast(gen_CodeBody, gen_parse_function_body());
		// <Name> ( <Parameters> ) { <Body> }
	}
	// TODO(Ed): Add support for detecting gen_constructor_ deletion
	else if (check(Tok_Operator) && currtok.Text.Ptr[0] == '=')
	{
		body = gen_cast(gen_CodeBody, gen_parse_assignment_expression());
	}
	else
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// <Name> ( <Parameters> );

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// <Name> ( <Parameters> ); <InlineCmt>
	}

	gen_CodeConstructor result = (gen_CodeConstructor)gen_make_code();

	result->Name               = gen_cache_str(identifier.Text);

	result->Specs              = specifiers;

	if (params)
		result->Params = params;

	if (initializer_list)
		result->InitializerList = initializer_list;

	if (body && body->Type == CT_Function_Body)
	{
		result->Body = gen_cast(gen_Code, body);
		result->Type = CT_Constructor;
	}
	else
		result->Type = CT_Constructor_Fwd;

	if (inline_cmt)
		result->InlineCmt = inline_cmt;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal inline gen_CodeDefine gen_parser_parse_define()
{
	push_scope();
	if (check(Tok_Preprocess_Hash))
	{
		// If gen_parse_define is called by the user the gen_hash reach here.
		eat(Tok_Preprocess_Hash);
	}

	eat(Tok_Preprocess_Define);
	// #define

	gen_CodeDefine define = (gen_CodeDefine)gen_make_code();
	define->Type          = CT_Preprocess_Define;
	if (! check(Tok_Identifier))
	{
		gen_log_failure("Error, expected identifier after #define\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	gen__ctx->parser.Scope->Name = currtok.Text;
	define->Name                 = gen_cache_str(currtok.Text);
	eat(Tok_Identifier);
	// #define <Name>

	gen_Macro* macro = lookup_macro(define->Name);
	if (gen_macro_is_functional(*macro))
	{
		eat(Tok_Paren_Open);
		// #define <Name> (

		// We provide the define params even if empty to make sure '()' are serialized.
		gen_CodeDefineParams params = (gen_CodeDefineParams)gen_make_code();
		params->Type                = CT_Parameters_Define;

		if (left && currtok.Type != Tok_Paren_Close)
		{
			params->Name = currtok.Text;
			params->NumEntries++;

			eat(Tok_Preprocess_Define_Param);
			// #define <Name> ( <param>
		}

		while (left && currtok.Type != Tok_Paren_Close)
		{
			eat(Tok_Comma);
			// #define <Name> ( <param>,

			gen_CodeDefineParams gen_next_param = (gen_CodeDefineParams)gen_make_code();
			gen_next_param->Type                = CT_Parameters_Define;
			gen_next_param->Name                = currtok.Text;
			gen_define_params_append(params, gen_next_param);

			// #define  <Name> (  <param>, <gen_next_param> ...
			eat(Tok_Preprocess_Define_Param);
		}

		eat(Tok_Paren_Close);
		// #define <Name> ( <params> )

		define->Params = params;
	}

	if (! check(Tok_Preprocess_Content))
	{
		gen_log_failure("Error, expected content after #define %s\n%s", define->Name, gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	if (currtok.Text.Len == 0)
	{
		define->Body = gen_untyped_str(gen_txt("\n"));
		eat(Tok_Preprocess_Content);
		// #define <Name> ( <params> ) <Content>

		gen_parser_pop(&gen__ctx->parser);
		return define;
	}

	define->Body = gen_untyped_str(gen_strbuilder_to_str(gen_parser_strip_formatting(currtok.Text, gen_parser_strip_formatting_dont_preserve_newlines)));
	eat(Tok_Preprocess_Content);
	// #define <Name> ( <params> ) <Content>

	gen_parser_pop(&gen__ctx->parser);
	return define;
}

gen_internal gen_CodeDestructor gen_parser_parse_destructor(gen_CodeSpecifiers specifiers)
{
	push_scope();

	bool has_context         = gen__ctx->parser.Scope && gen__ctx->parser.Scope->Prev;
	bool is_in_global_nspace = has_context && gen_str_are_equal(gen__ctx->parser.Scope->Prev->ProcName, gen_txt("gen_parse_global_nspace"));

	if (check(Tok_Spec_Virtual))
	{
		if (specifiers)
			gen_specifiers_append(specifiers, Spec_Virtual);
		else
			specifiers = gen_def_specifier(Spec_Virtual);
		eat(Tok_Spec_Virtual);
	}
	// <Virtual gen_Specifier>

	gen_Token prefix_identifier = gen_NullToken;
	if (is_in_global_nspace)
		prefix_identifier = gen_parse_identifier(gen_nullptr);

	if (left && currtok.Text.Ptr[0] == '~')
		eat(Tok_Operator);
	else
	{
		gen_log_failure("Expected gen_destructor_ '~' token\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	// <Virtual gen_Specifier> ~

	gen_Token       identifier = gen_parse_identifier(gen_nullptr);
	gen_CodeBody    body       = { gen_nullptr };
	gen_CodeComment inline_cmt = gen_NullCode;
	// <Virtual gen_Specifier> ~<Name>

	eat(Tok_Paren_Open);
	eat(Tok_Paren_Close);
	// <Virtual gen_Specifier> ~<Name>()

	bool pure_virtual = false;

	if (check(Tok_Operator) && currtok.Text.Ptr[0] == '=')
	{
		// <Virtual gen_Specifier> ~<Name>() =

		bool      skip_formatting = true;
		gen_Token upcoming        = nexttok;
		if (left && upcoming.Text.Ptr[0] == '0')
		{
			eat(Tok_Operator);
			eat(Tok_Number);
			// <Virtual gen_Specifier> ~<Name>() = 0

			gen_specifiers_append(specifiers, Spec_Pure);
		}
		else if (left && gen_c_str_compare_len(upcoming.Text.Ptr, "default", sizeof("default") - 1) == 0)
		{
			body = gen_cast(gen_CodeBody, gen_parse_assignment_expression());
			// <Virtual gen_Specifier> ~<
		}
		else
		{
			gen_log_failure("Pure or default specifier expected due to '=' token\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		pure_virtual = true;
	}

	if (! pure_virtual && check(Tok_BraceCurly_Open))
	{
		body = gen_cast(gen_CodeBody, gen_parse_function_body());
		// <Virtual gen_Specifier> ~<Name>() { ... }
	}
	else
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// <Virtual gen_Specifier> ~<Name>() <Pure gen_Specifier>;

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// <Virtual gen_Specifier> ~<Name>() <Pure gen_Specifier>; <InlineCmt>
	}

	gen_CodeDestructor result = (gen_CodeDestructor)gen_make_code();

	if (gen_tok_is_valid(prefix_identifier))
	{
		prefix_identifier.Text.Len += 1 + identifier.Text.Len;
		result->Name                = gen_cache_str(prefix_identifier.Text);
	}

	if (specifiers)
		result->Specs = specifiers;

	if (body && body->Type == CT_Function_Body)
	{
		result->Body = gen_cast(gen_Code, body);
		result->Type = CT_Destructor;
	}
	else
		result->Type = CT_Destructor_Fwd;

	if (inline_cmt)
		result->InlineCmt = inline_cmt;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeEnum gen_parser_parse_enum(bool inplace_def)
{
	push_scope();

	gen_Specifier specs_found[16]   = { Spec_NumSpecifiers };
	gen_s32       NumSpecifiers     = 0;

	gen_CodeAttributes attributes   = { gen_nullptr };

	gen_Token        name           = gen_NullToken;
	gen_Code         gen_array_expr = { gen_nullptr };
	gen_CodeTypename type           = { gen_nullptr };

	bool is_enum_class              = false;

	eat(Tok_Decl_Enum);
	// enum

	if (currtok.Type == Tok_Decl_Class)
	{
		eat(Tok_Decl_Class);
		is_enum_class = true;
		// enum class
	}

	attributes = gen_parse_attributes();
	// enum <class> <Attributes>

	if (check(Tok_Identifier))
	{
		name                         = currtok;
		gen__ctx->parser.Scope->Name = currtok.Text;
		eat(Tok_Identifier);
	}
	// enum <class> <Attributes> <Name>

	gen_b32  use_macro_underlying = false;
	gen_Code underlying_macro     = { gen_nullptr };
	if (currtok.Type == Tok_Assign_Classifer)
	{
		eat(Tok_Assign_Classifer);
		// enum <class> <Attributes> <Name> :

		type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
		if (gen_cast(gen_Code, type) == gen_Code_Invalid)
		{
			gen_log_failure("Failed to parse enum classifier\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		// enum <class> <Attributes> <Name> : <UnderlyingType>
	}
	else if (currtok.Type == Tok_Preprocess_Macro_Expr)
	{
		// We'll support the gen_enum_underlying macro
		if (gen_str_contains(currtok.Text, gen_enum_underlying_macro.Name))
		{
			use_macro_underlying = true;
			underlying_macro     = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Expr);
		}
	}

	gen_CodeBody body = { gen_nullptr };

	if (currtok.Type == Tok_BraceCurly_Open)
	{
		body       = (gen_CodeBody)gen_make_code();
		body->Type = CT_Enum_Body;

		eat(Tok_BraceCurly_Open);
		// enum <class> <Attributes> <Name> : <UnderlyingType> {

		gen_Code member    = gen_InvalidCode;

		bool expects_entry = true;
		while (left && currtok_noskip.Type != Tok_BraceCurly_Close)
		{
			if (! expects_entry)
			{
				gen_log_failure("Did not expect an entry after last member of enum body.\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				break;
			}

			if (currtok_noskip.Type == Tok_Preprocess_Hash)
				eat(Tok_Preprocess_Hash);

			switch (currtok_noskip.Type)
			{
				case Tok_NewLine:
					member = gen_untyped_str(currtok_noskip.Text);
					eat(Tok_NewLine);
					break;

				case Tok_Comment:
					member = gen_cast(gen_Code, gen_parse_comment());
					break;

				case Tok_Preprocess_Define:
					member = gen_cast(gen_Code, gen_parser_parse_define());
					// #define
					break;

				case Tok_Preprocess_If:
				case Tok_Preprocess_IfDef:
				case Tok_Preprocess_IfNotDef:
				case Tok_Preprocess_ElIf:
					member = gen_cast(gen_Code, gen_parse_preprocess_cond());
					// #<if, ifdef, ifndef, elif> ...
					break;

				case Tok_Preprocess_Else:
					member = gen_cast(gen_Code, gen_preprocess_else);
					eat(Tok_Preprocess_Else);
					break;

				case Tok_Preprocess_EndIf:
					member = gen_cast(gen_Code, gen_preprocess_endif);
					eat(Tok_Preprocess_EndIf);
					break;

				case Tok_Preprocess_Macro_Stmt:
				{
					member = gen_cast(gen_Code, gen_parse_simple_preprocess(Tok_Preprocess_Macro_Stmt));
					// <gen_Macro>
					break;
				}

				case Tok_Preprocess_Pragma:
					member = gen_cast(gen_Code, gen_parse_pragma());
					// #pragma
					break;

				case Tok_Preprocess_Unsupported:
					member = gen_cast(gen_Code, gen_parse_simple_preprocess(Tok_Preprocess_Unsupported));
					// #<UNSUPPORTED>
					break;

				default:
				{
					gen_Token entry = currtok;

					eat(Tok_Identifier);
					// <Name>

					if (currtok.Type == Tok_Operator && currtok.Text.Ptr[0] == '=')
					{
						eat(Tok_Operator);
						// <Name> =

						while (currtok.Type != Tok_Comma && currtok.Type != Tok_BraceCurly_Close)
						{
							eat(currtok.Type);
						}
					}
					// <Name> = <Expression>

					// Unreal UMETA macro support
					if (currtok.Type == Tok_Preprocess_Macro_Expr)
					{
						gen_Code macro = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Expr);
						// <Name> = <Expression> <gen_Macro>

						// We're intentially ignoring this code as its going to be serialized as an untyped string with the rest of the enum "entry".
						// TODO(Ed): We need a CodeEnumEntry, gen_AST_EnumEntry types
					}

					if (currtok.Type == Tok_Comma)
					{
						//gen_Token prev = * previous(gen__ctx->parser.Tokens, dont_skip_formatting);
						//entry.Length = ( (gen_sptr)prev.Text + prev.Length ) - (gen_sptr)entry.Text;

						eat(Tok_Comma);
						// <Name> = <Expression> <gen_Macro>,
					}

					// Consume inline comments
					// if ( currtok.Type == Tok_Comment && prevtok.Line == currtok.Line )
					// {
					// eat( Tok_Comment );
					// <Name> = <Expression> <gen_Macro>, // <Inline Comment>
					// }

					gen_Token prev = *gen_lex_previous(gen__ctx->parser.Tokens, gen_lex_dont_skip_formatting);
					entry.Text.Len = ((gen_sptr)prev.Text.Ptr + prev.Text.Len) - (gen_sptr)entry.Text.Ptr;

					member         = gen_untyped_str(entry.Text);
				}
				break;
			}

			if (member == gen_Code_Invalid)
			{
				gen_log_failure("Failed to parse member\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}

			gen_body_append(body, member);
		}

		eat(Tok_BraceCurly_Close);
		// enum <class> <Attributes> <Name> : <UnderlyingType> { <Body> }
	}

	gen_CodeComment inline_cmt = gen_NullCode;

	if (! inplace_def)
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// enum <class> <Attributes> <Name> : <UnderlyingType> { <Body> };

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// enum <class> <Attributes> <Name> : <UnderlyingType> { <Body> }; <InlineCmt>
	}

	gen_CodeEnum result = (gen_CodeEnum)gen_make_code();

	if (body)
	{
		result->Type = is_enum_class ? CT_Enum_Class : CT_Enum;
		result->Body = body;
	}
	else
	{
		result->Type = is_enum_class ? CT_Enum_Class_Fwd : CT_Enum_Fwd;
	}

	result->Name = gen_cache_str(name.Text);

	if (attributes)
		result->Attributes = attributes;

	result->UnderlyingTypeMacro = underlying_macro;
	result->UnderlyingType      = type;

	if (inline_cmt)
		result->InlineCmt = inline_cmt;


	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal inline gen_CodeBody gen_parser_parse_export_body()
{
	push_scope();
	gen_CodeBody result = gen_parse_global_nspace(CT_Export_Body);
	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal inline gen_CodeBody gen_parser_parse_extern_link_body()
{
	push_scope();
	gen_CodeBody result = gen_parse_global_nspace(CT_Extern_Linkage_Body);
	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeExtern gen_parser_parse_extern_link()
{
	push_scope();

	eat(Tok_Decl_Extern_Linkage);
	// extern

	gen_Token name = currtok;
	eat(Tok_String);
	// extern "<Name>"

	name.Text.Ptr         += 1;
	name.Text.Len         -= 1;

	gen_CodeExtern result  = (gen_CodeExtern)gen_make_code();
	result->Type           = CT_Extern_Linkage;
	result->Name           = gen_cache_str(name.Text);

	gen_CodeBody entry     = gen_parser_parse_extern_link_body();
	if (gen_cast(gen_Code, entry) == gen_Code_Invalid)
	{
		gen_log_failure("Failed to parse body\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return result;
	}
	// extern "<Name>" { <Body> }

	result->Body = entry;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeFriend gen_parser_parse_friend()
{
	push_scope();

	eat(Tok_Decl_Friend);
	// friend

	gen_CodeFn         function   = { gen_nullptr };
	gen_CodeOperator   op         = { gen_nullptr };
	gen_CodeSpecifiers specifiers = { gen_nullptr };

	// Specifiers Parsing
	{
		gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
		gen_s32       NumSpecifiers   = 0;

		while (left && gen_tok_is_specifier(currtok))
		{
			gen_Specifier spec = gen_str_to_specifier(currtok.Text);

			switch (spec)
			{
			GEN_PARSER_FRIEND_ALLOWED_SPECIFIER_CASES:
				break;

				default:
					gen_log_failure(
					    "Invalid specifier %S for friend definition\n%S",
					    gen_spec_to_str(spec),
					    gen_strbuilder_to_str(gen_parser_to_strbuilder(gen__ctx->parser))
					);
					gen_parser_pop(&gen__ctx->parser);
					return gen_InvalidCode;
			}

			// Ignore const it will be handled by the type
			if (spec == Spec_Const)
				break;

			specs_found[NumSpecifiers] = spec;
			NumSpecifiers++;
			eat(currtok.Type);
		}

		if (NumSpecifiers)
		{
			specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
		}
		// <friend> <specifiers>
	}

	// Type declaration or return type
	gen_CodeTypename type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
	if (gen_cast(gen_Code, type) == gen_Code_Invalid)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	// friend <Type>

	// Funciton declaration
	if (currtok.Type == Tok_Identifier)
	{
		// Name
		gen_Token name               = gen_parse_identifier(gen_nullptr);
		gen__ctx->parser.Scope->Name = name.Text;
		// friend <ReturnType> <Name>

		function = gen_parse_function_after_name(ModuleFlag_None, gen_NullCode, specifiers, type, name);

		// Parameter list
		// gen_CodeParams params = gen_parse_params();
		// friend <ReturnType> <Name> ( <Parameters> )

		// function             = gen_make_code();
		// function->Type       = Function_Fwd;
		// function->Name       = gen_cache_str( name );
		// function->ReturnType = type;

		// if ( params )
		// function->Params = params;
	}

	// gen_Operator declaration or definition
	if (currtok.Type == Tok_Decl_Operator)
	{
		op = gen_parse_operator_after_ret_type(ModuleFlag_None, gen_NullCode, specifiers, type);
	}

	gen_CodeComment inline_cmt = gen_NullCode;
	if (function && function->Type == CT_Function_Fwd)
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// friend <Type>;
		// friend <ReturnType> <Name> ( <Parameters> );

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// friend <Type>; <InlineCmt>
		// friend <ReturnType> <Name> ( <Parameters> ); <InlineCmt>
	}

	gen_CodeFriend result = (gen_CodeFriend)gen_make_code();
	result->Type          = CT_Friend;

	if (function)
		result->Declaration = gen_cast(gen_Code, function);
	else if (op)
		result->Declaration = gen_cast(gen_Code, op);
	else
		result->Declaration = gen_cast(gen_Code, type);

	if (inline_cmt)
		result->InlineCmt = inline_cmt;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeFn gen_parser_parse_function()
{
	push_scope();

	gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
	gen_s32       NumSpecifiers   = 0;

	gen_CodeAttributes attributes = { gen_nullptr };
	gen_CodeSpecifiers specifiers = { gen_nullptr };
	gen_ModuleFlag     mflags     = ModuleFlag_None;

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <export>

	attributes = gen_parse_attributes();
	// <export> <Attributes>

	while (left && gen_tok_is_specifier(currtok))
	{
		gen_Specifier spec = gen_str_to_specifier(currtok.Text);

		switch (spec)
		{
		GEN_PARSER_FUNCTION_ALLOWED_SPECIFIER_CASES:
			break;

			default:
				gen_log_failure("Invalid specifier %S for functon\n%SB", gen_spec_to_str(spec), gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
		}

		if (spec == Spec_Const)
			continue;

		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat(currtok.Type);
	}

	if (NumSpecifiers)
	{
		specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
	}
	// <export> <Attributes> <Specifiers>

	gen_CodeTypename ret_type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
	if (gen_cast(gen_Code, ret_type) == gen_Code_Invalid)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	// <export> <Attributes> <Specifiers> <ReturnType>

	gen_Token name               = gen_parse_identifier(gen_nullptr);
	gen__ctx->parser.Scope->Name = name.Text;
	if (! gen_tok_is_valid(name))
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	// <export> <Attributes> <Specifiers> <ReturnType> <Name>

	gen_CodeFn result = gen_parse_function_after_name(mflags, attributes, specifiers, ret_type, name);
	// <export> <Attributes> <Specifiers> <ReturnType> <Name> ...

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeNS gen_parser_parse_namespace()
{
	push_scope();

	eat(Tok_Decl_Namespace);
	// namespace

	gen_Token name               = gen_parse_identifier(gen_nullptr);
	gen__ctx->parser.Scope->Name = name.Text;
	// namespace <Name>

	gen_CodeBody body = gen_parse_global_nspace(CT_Namespace_Body);
	if (gen_cast(gen_Code, body) == gen_Code_Invalid)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	// namespace <Name> { <Body> }

	gen_CodeNS result = (gen_CodeNS)gen_make_code();
	result->Type      = CT_Namespace;
	result->Name      = gen_cache_str(name.Text);

	result->Body      = body;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeOperator gen_parser_parse_operator()
{
	push_scope();

	gen_CodeAttributes attributes = { gen_nullptr };
	gen_CodeSpecifiers specifiers = { gen_nullptr };
	gen_ModuleFlag     mflags     = ModuleFlag_None;

	gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
	gen_s32       NumSpecifiers   = 0;

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <export>

	attributes = gen_parse_attributes();
	// <export> <Attributes>

	while (left && gen_tok_is_specifier(currtok))
	{
		gen_Specifier spec = gen_str_to_specifier(currtok.Text);

		switch (spec)
		{
		GEN_PARSER_OPERATOR_ALLOWED_SPECIFIER_CASES:
			break;

			default:
				gen_log_failure(
				    "Invalid specifier "
				    "%S"
				    " for operator\n%SB",
				    gen_spec_to_str(spec),
				    gen_parser_to_strbuilder(gen__ctx->parser)
				);
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
		}

		if (spec == Spec_Const)
			continue;

		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat(currtok.Type);
	}

	if (NumSpecifiers)
	{
		specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
	}
	// <export> <Attributes> <Specifiers>

	// Parse Return Type
	gen_CodeTypename ret_type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
	// <export> <Attributes> <Specifiers> <ReturnType>

	gen_CodeOperator result = gen_parse_operator_after_ret_type(mflags, attributes, specifiers, ret_type);
	// <export> <Attributes> <Specifiers> <ReturnType> ...

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeOpCast gen_parser_parse_operator_cast(gen_CodeSpecifiers specifiers)
{
	push_scope();

	// gen_Operator's namespace if not within same class.
	gen_Token name = gen_NullToken;
	if (check(Tok_Identifier))
	{
		name = currtok;
		while (left && currtok.Type == Tok_Identifier)
		{
			eat(Tok_Identifier);
			// <Specifiers> <Qualifier>

			if (currtok.Type == Tok_Access_StaticSymbol)
				eat(Tok_Access_StaticSymbol);
			// <Specifiers> <Qualifier> ::
		}
		// <Specifiers> <Qualifier> :: ...

		name.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)name.Text.Ptr;
	}

	eat(Tok_Decl_Operator);
	// <Specifiers> <Qualifier> :: ... operator

	gen_CodeTypename type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
	// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>

	gen_Str   scope_name         = { type->Name.Ptr, type->Name.Len };
	gen_Token scope_name_tok     = { scope_name, Tok_Identifier, 0, 0, TF_Null };
	gen__ctx->parser.Scope->Name = scope_name_tok.Text;

	eat(Tok_Paren_Open);
	eat(Tok_Paren_Close);
	// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>()

	// TODO(Ed) : operator gen_cast can have const, volatile, l-value, r-value noexecept qualifying specifiers.
	if (check(Tok_Spec_Const))
	{
		if (specifiers == gen_nullptr)
			specifiers = gen_def_specifier(Spec_Const);

		else
			gen_specifiers_append(specifiers, Spec_Const);

		eat(Tok_Spec_Const);
	}
	// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const>

	gen_Code        body       = gen_NullCode;
	gen_CodeComment inline_cmt = gen_NullCode;

	if (check(Tok_BraceCurly_Open))
	{
		eat(Tok_BraceCurly_Open);
		// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const> {

		gen_Token gen_body_str = currtok;

		gen_s32 level          = 0;
		while (left && (currtok.Type != Tok_BraceCurly_Close || level > 0))
		{
			if (currtok.Type == Tok_BraceCurly_Open)
				level++;

			else if (currtok.Type == Tok_BraceCurly_Close)
				level--;

			eat(currtok.Type);
		}
		gen_body_str.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)gen_body_str.Text.Ptr;

		eat(Tok_BraceCurly_Close);
		// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const> { <Body> }

		body = gen_untyped_str(gen_body_str.Text);
	}
	else
	{
		gen_Token stmt_end = currtok;
		eat(Tok_Statement_End);
		// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const>;

		if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
			inline_cmt = gen_parse_comment();
		// <Specifiers> <Qualifier> :: ... operator <UnderlyingType>() <const>; <InlineCmt>
	}

	gen_CodeOpCast result = (gen_CodeOpCast)gen_make_code();

	if (gen_tok_is_valid(name))
		result->Name = gen_cache_str(name.Text);

	if (body)
	{
		result->Type = CT_Operator_Cast;
		result->Body = gen_cast(gen_CodeBody, body);
	}
	else
	{
		result->Type = CT_Operator_Cast_Fwd;
	}

	if (specifiers)
		result->Specs = specifiers;

	result->ValueType = gen_cast(gen_CodeTypename, type);

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal inline gen_CodeStruct gen_parser_parse_struct(bool inplace_def)
{
	push_scope();
	gen_CodeStruct result = (gen_CodeStruct)gen_parse_class_struct(Tok_Decl_Struct, inplace_def);
	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeTemplate gen_parser_parse_template()
{
#define UseTemplateCapture true

	push_scope();

	gen_ModuleFlag mflags = ModuleFlag_None;

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <export> template

	eat(Tok_Decl_Template);
	// <export> template

	gen_CodeParams params = gen_parse_params(UseTemplateCapture);
	if (gen_cast(gen_Code, params) == gen_Code_Invalid)
	{
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}
	// <export> template< <Parameters> >

	gen_Code definition = { gen_nullptr };

	while (left)
	{
		if (check(Tok_Decl_Class))
		{
			definition = gen_cast(gen_Code, gen_parser_parse_class(gen_parser_not_inplace_def));
			// <export> template< <Parameters> > class ...
			break;
		}

		if (check(Tok_Decl_Struct))
		{
			definition = gen_cast(gen_Code, gen_parser_parse_struct(gen_parser_not_inplace_def));
			// <export> template< <Parameters> > struct ...
			break;
		}

		if (check(Tok_Decl_Union))
		{
			definition = gen_cast(gen_Code, gen_parser_parse_union(gen_parser_not_inplace_def));
			// <export> template< <Parameters> > union ...
			break;
		}

		if (check(Tok_Decl_Using))
		{
			definition = gen_cast(gen_Code, gen_parser_parse_using());
			// <export> template< <Parameters> > using ...
			break;
		}

		// Its either a function or a variable
		gen_Token name                = gen_NullToken;

		gen_CodeAttributes attributes = { gen_nullptr };
		gen_CodeSpecifiers specifiers = { gen_nullptr };

		bool expects_function         = false;

		gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
		gen_s32       NumSpecifiers   = 0;

		attributes                    = gen_parse_attributes();
		// <export> template< <Parameters> > <Attributes>

		// Specifiers Parsing
		{
			while (left && gen_tok_is_specifier(currtok))
			{
				gen_Specifier spec = gen_str_to_specifier(currtok.Text);

				switch (spec)
				{
				GEN_PARSER_TEMPLATE_ALLOWED_SPECIFIER_CASES:
					break;

					case Spec_Consteval:
						expects_function = true;
						break;

					default:
						gen_log_failure(
						    "Invalid specifier %S for variable or function\n%SB",
						    gen_spec_to_str(spec),
						    gen_parser_to_strbuilder(gen__ctx->parser)
						);
						gen_parser_pop(&gen__ctx->parser);
						return gen_InvalidCode;
				}

				// Ignore const it will be handled by the type
				if (spec == Spec_Const)
					break;

				specs_found[NumSpecifiers] = spec;
				NumSpecifiers++;
				eat(currtok.Type);
			}

			if (NumSpecifiers)
			{
				specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
			}
			// <export> template< <Parameters> > <Attributes> <Specifiers>
		}


		bool has_context         = gen__ctx->parser.Scope && gen__ctx->parser.Scope->Prev;
		bool is_in_global_nspace = has_context && gen_str_are_equal(gen__ctx->parser.Scope->Prev->ProcName, gen_txt("gen_parse_global_nspace"));
		// Possible gen_constructor_ implemented at gen_global file scope.
		if (is_in_global_nspace)
		{
			gen_Code gen_constructor__destructor = gen_parse_global_nspace_constructor_destructor(specifiers);
			if (gen_constructor__destructor)
			{
				definition = gen_constructor__destructor;
				// <Attributes> <Specifiers> <Name> :: <Name> <Type> () { ... }
				break;
			}
		}

		// Possible user Defined operator casts
		if (is_in_global_nspace)
		{
			bool    found_operator_cast_outside_class_implmentation = false;
			gen_s32 idx                                             = gen__ctx->parser.Tokens.Idx;

			for (; idx < gen_array_num(gen__ctx->parser.Tokens.Arr); idx++)
			{
				gen_Token tok = gen__ctx->parser.Tokens.Arr[idx];

				if (tok.Type == Tok_Identifier)
				{
					idx++;
					tok = gen__ctx->parser.Tokens.Arr[idx];
					if (tok.Type == Tok_Access_StaticSymbol)
						continue;

					break;
				}

				if (tok.Type == Tok_Decl_Operator)
					found_operator_cast_outside_class_implmentation = true;

				break;
			}

			if (found_operator_cast_outside_class_implmentation)
			{
				definition = gen_cast(gen_Code, gen_parser_parse_operator_cast(specifiers));
				// <Attributes> <Specifiers> <Name> :: operator <Type> () { ... }
				break;
			}
		}

		definition = gen_parse_operator_function_or_variable(expects_function, attributes, specifiers);
		// <export> template< <Parameters> > <Attributes> <Specifiers> ...
		break;
	}

	gen_CodeTemplate result = (gen_CodeTemplate)gen_make_code();
	result->Type            = CT_Template;
	result->Params          = params;
	result->Declaration     = definition;
	result->ModuleFlags     = mflags;
	// result->Name        = definition->Name;

	gen_parser_pop(&gen__ctx->parser);
	return result;
#undef UseTemplateCapture
}

/*
	This is a mess, but it works
	Parsing typename is arguably one of the worst aspects of C/C++.
	This is an effort to parse it without a full blown or half-blown compliant parser.

	Recursive function typenames are not supported, if they are used expect it to serailize just fine, but validation with gen_AST::is_equal
	will not be possible if two ASTs share the same definiton but the formatting is slightly different:
	gen_AST_1->Name: (* A    ( int   (*)    (short a,unsigned b,long c) ) )
	gen_AST_2->Name: (* A       ( int(*)(short a, unsigned b, long c) ) )

	The excess whitespace cannot be stripped however, because there is no semantic awareness within the first capture group.
*/
gen_internal gen_CodeTypename gen_parser_parse_type(bool from_template, bool* gen_typedef_is_function)
{
	push_scope();

	gen_Token context_tok         = prevtok;

	gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
	gen_s32       NumSpecifiers   = 0;

	gen_Token name                = gen_NullToken;

	gen_ETypenameTag tag          = Tag_None;

	// Attributes are assumed to be before the type signature
	gen_CodeAttributes attributes = gen_parse_attributes();
	// <Attributes>

	// Prefix specifiers
	while (left && gen_tok_is_specifier(currtok))
	{
		gen_Specifier spec = gen_str_to_specifier(currtok.Text);

		if (spec != Spec_Const)
		{
			gen_log_failure("Error, invalid specifier used in type definition: %S\n%SB", currtok.Text, gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat(currtok.Type);
	}
	// <Attributes> <Specifiers>

	if (left == 0)
	{
		gen_log_failure("Error, unexpected end of type definition\n%SB", gen_parser_to_strbuilder(gen__ctx->parser));
		gen_parser_pop(&gen__ctx->parser);
		return gen_InvalidCode;
	}

	if (from_template && currtok.Type == Tok_Decl_Class)
	{
		// If a value's type is being parsed from a template, class can be used instead of typename.
		name = currtok;
		eat(Tok_Decl_Class);
		// <class>
	}

	// All kinds of nonsense can makeup a type signature, first we check for a in-place definition of a class, enum, struct, or union
	else if (currtok.Type == Tok_Decl_Class || currtok.Type == Tok_Decl_Enum || currtok.Type == Tok_Decl_Struct || currtok.Type == Tok_Decl_Union)
	{
		gen_Token next = nexttok;

		if (next.Type == Tok_Identifier)
		{
			switch (currtok.Type)
			{
				case Tok_Decl_Class:
					tag = Tag_Class;
					break;
				case Tok_Decl_Enum:
					tag = Tag_Enum;
					break;
				case Tok_Decl_Struct:
					tag = Tag_Struct;
					break;
				case Tok_Decl_Union:
					tag = Tag_Union;
					break;
				default:
					break;
			}
			eat(currtok.Type);
			// <Attributes> <Specifiers> <class, enum, struct, union>

			name = gen_parse_identifier(gen_nullptr);
			// <Attributes> <Specifiers> <class, enum, struct, union> <Name>
		}
		else if (next.Type == Tok_BraceCurly_Open)
		{
			name = currtok;
			// We have an inplace definition, we need to consume that...

			// TODO(Ed): we need to add a way for gen_AST_CodeTypename to track an implace definition..
			gen_b32 const inplace      = true;
			gen_Code      indplace_def = gen_cast(gen_Code, gen_parser_parse_struct(inplace));

			// For now we lose the structural information,
			name.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)name.Text.Ptr;
			// <Attributes> <Specifiers> <class, enum, struct, union> <inplace def>
		}
		gen__ctx->parser.Scope->Name = name.Text;
		// <Attributes> <Specifiers> <class, enum, struct, union> <Name>
	}

// Decltype draft implementation
#if 0
else if ( currtok.Type == Tok_DeclType )
{
	// Will have a capture and its own parsing rules, were going to just shove everything in a string (for now).
	name = currtok;
	eat( Tok_DeclType );
	// <Attributes> <Specifiers> decltype

	eat( Tok_Paren_Open );
	while ( left && currtok.Type != Tok_Paren_Close )
	{
		if ( currtok.Type == Tok_Paren_Open )
			level++;

		if ( currtok.Type == Tok_Paren_Close )
			level--;

		eat( currtok.Type );
	}
	eat( Tok_Paren_Close );

	name.Length = ( (gen_sptr)currtok.Text + currtok.Length ) - (gen_sptr)name.Text;
	gen__ctx->parser.Scope->Name = name;
	// <Attributes> <Specifiers> decltype( <Expression > )
}
#endif

	// Check if native type keywords are used, eat them for the signature.
	// <attributes> <specifiers> <native types ...> ...
	else if (currtok.Type >= Tok_Type_Unsigned && currtok.Type <= Tok_Type_MS_W64)
	{
		// TODO(Ed) : Review this... Its necessary for parsing however the algo's path to this is lost...
		name = currtok;
		eat(currtok.Type);

		while (left && currtok.Type >= Tok_Type_Unsigned && currtok.Type <= Tok_Type_MS_W64)
		{
			eat(currtok.Type);
		}

		name.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)name.Text.Ptr;
		// <Attributes> <Specifiers> <Compound type expression>
	}
	else if (currtok.Type == Tok_Type_Typename)
	{
		name = currtok;
		eat(Tok_Type_Typename);
		// <typename>

		if (! from_template)
		{
			name                         = gen_parse_identifier(gen_nullptr);
			gen__ctx->parser.Scope->Name = name.Text;
			if (! gen_tok_is_valid(name))
			{
				gen_log_failure("Error, failed to type signature\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}
		}
	}
	else if (currtok.Type == Tok_Preprocess_Macro_Typename)
	{
		// Typename is a macro
		// name = currtok;
		// eat(Tok_Preprocess_Macro_Typename);
		gen_Code macro = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Typename);
		name.Text      = macro->Content;
	}

	// The usual Identifier type signature that may have namespace qualifiers
	else
	{
		name                         = gen_parse_identifier(gen_nullptr);
		gen__ctx->parser.Scope->Name = name.Text;
		if (! gen_tok_is_valid(name))
		{
			gen_log_failure("Error, failed to type signature\n%s", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}
		// <Attributes> <Specifiers> <Qualifier ::> <Identifier>
		// <Attributes> <Specifiers> <Identifier>
	}

	// Suffix specifiers for typename.
	while (left && gen_tok_is_specifier(currtok))
	{
		gen_Specifier spec = gen_str_to_specifier(currtok.Text);

		switch (spec)
		{
		GEN_PARSER_TYPENAME_ALLOWED_SUFFIX_SPECIFIER_CASES:
			break;

			default:
			{
				gen_log_failure("Error, invalid specifier used in type definition: %S\n%SB", currtok.Text, gen_parser_to_strbuilder(gen__ctx->parser));
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}
		}
		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat(currtok.Type);
	}

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	if (NumSpecifiers)
	{
		specifiers    = gen_def_specifiers_arr(NumSpecifiers, specs_found);
		NumSpecifiers = 0;
	}
#endif
	// <Attributes> <Specifiers> <Identifier> <Specifiers>

	// For function type signatures
	gen_CodeTypename return_type = gen_NullCode;
	gen_CodeParams   params      = gen_NullCode;

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	gen_CodeParams gen_params_nested = gen_NullCode;
#endif

	bool       is_function_typename = false;
	gen_Token* last_capture         = gen_nullptr;
	{
		gen_Token* scanner = gen__ctx->parser.Tokens.Arr + gen__ctx->parser.Tokens.Idx;

		// An identifier being within a typename's signature only occurs if were parsing a typename for a typedef.
		if (gen_typedef_is_function && scanner->Type == Tok_Identifier)
		{
			is_function_typename = true;
			++scanner;
		}
		is_function_typename     = scanner->Type == Tok_Paren_Open;

		gen_Token* first_capture = scanner;
		if (is_function_typename)
		{
			// Go to the end of the signature
			while (scanner->Type != Tok_Statement_End && scanner->Type != Tok_BraceCurly_Open)
				++scanner;

			// Go back to the first capture start found
			while (scanner->Type != Tok_Paren_Open)
				--scanner;

			last_capture = scanner;
		}

		bool has_context   = gen__ctx->parser.Scope && gen__ctx->parser.Scope->Prev;
		bool is_for_opcast = has_context && gen_str_are_equal(gen__ctx->parser.Scope->Prev->ProcName, gen_txt("gen_parser_parse_operator_cast"));
		if (is_for_opcast && is_function_typename && last_capture)
		{
			// If we're parsing for an operator gen_cast, having one capture start is not enough
			// we need to make sure that the capture is not for the gen_cast definition.
			is_function_typename = false;

			if (last_capture == first_capture)
			{
				// The capture start in question is the first capture start, this is not a function typename.
				is_function_typename = false;
			}
		}
	}

	if (is_function_typename)
	{
		// We're dealing with a function typename.
		// By this point, decltype should have been taken care of for return type, along with any all its specifiers

		// The previous information with exception to attributes will be considered the return type.
		return_type       = (gen_CodeTypename)gen_make_code();
		return_type->Type = CT_Typename;

		// gen_StrBuilder
		// name_stripped = gen_StrBuilder::make( FallbackAllocator, name );
		// name_stripped.strip_space();
		return_type->Name = gen_cache_str(name.Text);

#ifdef GEN_USE_NEW_TYPENAME_PARSING
		if (specifiers)
		{
			return_type->Specs = specifiers;
			specifiers         = gen_nullptr;
		}

#else
		if (NumSpecifiers)
			return_type->Specs = gen_def_specifiers_arr(NumSpecifiers, (gen_Specifier*)specs_found);

		// Reset specifiers, the function itself will have its own suffix specifiers possibly.
		NumSpecifiers = 0;
#endif
		// <Attributes> <ReturnType>
		name = gen_NullToken;

		// The next token can either be a capture for the identifier or it could be the identifier exposed.
		if (! check(Tok_Paren_Open))
		{
			// Started with an identifier immeidately, which means its of the format: <ReturnType> <identifier> <capture>;
			name = gen_parse_identifier(gen_nullptr);
		}
		// <Attributes> <ReturnType> <Identifier>

		// If the next token is a capture start and is not the last capture, then we're dealing with function typename whoose identifier is within the
		// capture.
		else if ((gen__ctx->parser.Tokens.Arr + gen__ctx->parser.Tokens.Idx) != last_capture)
		{
			// WIP : Possible alternative without much pain...
			// If this were to be parsed properly...
			// Eat Capture Start
			// Deal with possible binding specifiers (*, &, &&) and modifiers on those bindings (const, volatile)
			// Parse specifiers for the typename with an optional identifier,
			// we can shove these specific specifiers into a specs, and then leave the suffix ones for a separate member of the gen_AST.
			// Parse immeidate capture which would be with gen_parse_params()
			// Eat Capture End
#ifdef GEN_USE_NEW_TYPENAME_PARSING
			eat(Tok_Paren_Open);
			// <Attributes> <ReturnType> (

			// Binding specifiers
			while (left && currtok.is_specifier())
			{
				gen_Specifier spec = to_type(currtok);

				if (spec != Spec_Ptr && spec != Spec_Ref && spec != Spec_RValue)
				{
					gen_log_failure("Error, invalid specifier used in type definition: %S\n%SB", gen_toktype_to_str(currtok), to_strbuilder(gen__ctx->parser));
					pop(&gen__ctx->parser);
					return gen_InvalidCode;
				}

				specs_found[NumSpecifiers] = spec;
				NumSpecifiers++;
				eat(currtok.Type);
			}

			if (NumSpecifiers)
			{
				specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
			}
			NumSpecifiers = 0;
			// <Attributes> <ReturnType> ( <Specifiers>

			if (check(Tok_Identifier))
				name = gen_parse_identifier();
			// <Attributes> <ReturnType> ( <Specifiers> <Identifier>

			// Immeidate parameters

			if (check(Tok_Paren_Open))
				gen_params_nested = gen_parse_params();
			// <Attributes> <ReturnType> ( <Specifiers> <Identifier> ( <Parameters> )

			eat(Tok_Paren_Close);
			// <Attributes> <ReturnType> ( <Specifiers> <Identifier> ( <Parameters> ) )

#else
			// Starting immediatley with a capture, most likely declaring a typename for a member function pointer.
			// Everything within this capture will just be shoved into the name field including the capture tokens themselves.
			name = currtok;

			eat(Tok_Paren_Open);
			// <Attributes> <ReturnType> (

			gen_s32 level = 0;
			while (left && (currtok.Type != Tok_Paren_Close || level > 0))
			{
				if (currtok.Type == Tok_Paren_Open)
					level++;

				if (currtok.Type == Tok_Paren_Close)
					level--;

				eat(currtok.Type);
			}
			eat(Tok_Paren_Close);
			// <Attributes> <ReturnType> ( <Expression> )

			name.Text.Len = ((gen_sptr)prevtok.Text.Ptr + prevtok.Text.Len) - (gen_sptr)name.Text.Ptr;
#endif
		}

		// Were now dealing with the parameters of the function
		params = gen_parse_params(gen_parser_use_parenthesis);
		// <Attributes> <ReturnType> <All Kinds of nonsense> ( <Parameters> )

		// Look for suffix specifiers for the function
		while (left && gen_tok_is_specifier(currtok))
		{
			gen_Specifier spec = gen_str_to_specifier(currtok.Text);

			if (spec != Spec_Const
			    // TODO : Add support for NoExcept, l-value, volatile, l-value, etc
			    // && spec != Spec_NoExcept
			    && spec != Spec_RValue)
			{
				gen_log_failure(
				    "Error, invalid specifier used in type definition: %S\n%S",
				    currtok.Text,
				    gen_strbuilder_to_str(gen_parser_to_strbuilder(gen__ctx->parser))
				);
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
			}

			specs_found[NumSpecifiers] = spec;
			NumSpecifiers++;
			eat(currtok.Type);
		}

#ifdef GEN_USE_NEW_TYPENAME_PARSING
		if (NumSpecifiers)
		{
			func_suffix_specs = gen_def_specifiers_arr(NumSpecifiers, specs_found);
			NumSpecifiers     = 0;
		}
#endif
		// <Attributes> <ReturnType> <All Kinds of nonsense> ( <Parameters> ) <Specifiers>
	}
	// <Attributes> <All Kinds of nonsense>

	bool is_param_pack = false;
	if (check(Tok_Varadic_Argument))
	{
		is_param_pack = true;
		eat(Tok_Varadic_Argument);
		// <Attributes> <All kinds of nonsense> ...
	}

	gen_CodeTypename result = (gen_CodeTypename)gen_make_code();
	result->Type            = CT_Typename;
	// result->gen_Token = gen__ctx->parser.Scope->Start;

	// Need to wait until were using the new parsing method to do this.
	gen_StrBuilder name_stripped = gen_parser_strip_formatting(name.Text, gen_parser_strip_formatting_dont_preserve_newlines);

	// name_stripped.strip_space();

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	if (gen_params_nested)
	{
		name_stripped.append(gen_params_nested->to_strbuilder());
	}
#endif

	result->Name = gen_cache_str(gen_strbuilder_to_str(name_stripped));

	if (attributes)
		result->Attributes = attributes;

#ifdef GEN_USE_NEW_TYPENAME_PARSING
	if (specifiers)
	{
		result->Specs = specifiers;
	}

	if (func_suffix_specs)
	{
		result->FuncSuffixSpecs = func_suffix_specs;
	}
#else
	if (NumSpecifiers)
	{
		gen_CodeSpecifiers specifiers = gen_def_specifiers_arr(NumSpecifiers, (gen_Specifier*)specs_found);
		result->Specs                 = specifiers;
	}
#endif

	if (is_param_pack)
		result->IsParamPack = true;

	// These following are only populated if its a function typename
	if (return_type)
	{
		result->ReturnType = return_type;

		if (gen_typedef_is_function)
			*gen_typedef_is_function = true;
	}

	if (params)
		result->Params = params;

	result->TypeTag = tag;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeTypedef gen_parser_parse_typedef()
{
	push_scope();

	bool      is_function    = false;
	gen_Token name           = gen_NullToken;
	gen_Code  gen_array_expr = { gen_nullptr };
	gen_Code  type           = { gen_nullptr };

	gen_ModuleFlag mflags    = ModuleFlag_None;

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <ModuleFlags>

	eat(Tok_Decl_Typedef);
	// <ModuleFlags> typedef

	const bool from_typedef = true;

	// TODO(Ed): UPDATE MACRO USAGE HERE
#if GEN_PARSER_DISABLE_MACRO_TYPEDEF
	if (false)
#else
	gen_b32 valid_macro  = false;
	valid_macro         |= left && currtok.Type == Tok_Preprocess_Macro_Typename;
	valid_macro         |= left && currtok.Type == Tok_Preprocess_Macro_Stmt;
	// if (currtok.Type == Tok_Preprocess_Macro_Stmt)
	// {
	// PreprocessMacro* macro = lookup_macro(currtok.Text);
	// valid_macro |= macro && gen_macro_expects_body(* macro));
	// }

	if (valid_macro)
#endif
	{
		type                         = gen_cast(gen_Code, gen_t_empty);
		name                         = currtok;
		gen_Code macro               = gen_parse_simple_preprocess(currtok.Type);
		name.Text.Len                = macro->Content.Len;
		gen__ctx->parser.Scope->Name = name.Text;
		// <ModuleFalgs> typedef <Preprocessed_Macro>

		if (currtok.Type == Tok_Identifier)
		{
			type = macro;
			name = currtok;
			eat(Tok_Identifier);
			// <ModuleFalgs> typedef <Preprocessed_Macro> <Identifier>
		}
	}
	else
	{
		bool is_complicated =
		    currtok.Type == Tok_Decl_Enum || currtok.Type == Tok_Decl_Class || currtok.Type == Tok_Decl_Struct || currtok.Type == Tok_Decl_Union;


		// This code is highly correlated with gen_parse_complicated_definition
		if (is_complicated)
		{
			gen_TokArray tokens = gen__ctx->parser.Tokens;
			gen_TokType  which  = currtok.Type;

			gen_s32 idx         = tokens.Idx;
			gen_s32 level       = 0;
			for (; idx < gen_array_num(tokens.Arr); idx++)
			{
				if (tokens.Arr[idx].Type == Tok_BraceCurly_Open)
					level++;

				if (tokens.Arr[idx].Type == Tok_BraceCurly_Close)
					level--;

				if (level == 0 && tokens.Arr[idx].Type == Tok_Statement_End)
					break;
			}

			gen_Token pre_foward_tok = currtok;
			if ((idx - 3) == tokens.Idx)
			{
				// Its a forward declaration only
				type = gen_parse_forward_or_definition(which, from_typedef);
				// <ModuleFalgs> typedef <UnderlyingType: Forward Decl>
			}
			else
			{
				gen_Token tok = tokens.Arr[idx - 1];
				if (tok.Type == Tok_Identifier)
				{
					gen_log_fmt("Found id\n");
					tok                 = tokens.Arr[idx - 2];

					bool is_indirection = tok.Type == Tok_Ampersand || tok.Type == Tok_Star;

					bool ok_to_parse    = false;

					gen_Token temp_3    = tokens.Arr[idx - 3];

					if (tok.Type == Tok_BraceCurly_Close)
					{
						// Its an inplace definition
						// typedef <which> <type_identifier> { ... } <identifier>;
						ok_to_parse = true;
					}
					else if (tok.Type == Tok_Identifier && tokens.Arr[idx - 3].Type == which)
					{
						// Its a variable with type ID using which namespace.
						// typedef <which> <type_identifier> <identifier>;
						ok_to_parse = true;
					}
					else if (is_indirection)
					{
						// Its a indirection type with type ID using struct namespace.
						// typedef <which> <type_identifier>* <identifier>;
						ok_to_parse = true;
					}

					if (! ok_to_parse)
					{
						gen_log_failure("Unsupported or bad member definition after struct declaration\n%SB", gen_parser_to_strbuilder(gen__ctx->parser));
						gen_parser_pop(&gen__ctx->parser);
						return gen_InvalidCode;
					}

					// TODO(Ed) : I'm not sure if I have to use gen_parser_parse_type here, I'd rather not as that would complicate gen_parser_parse_type.
					// type = gen_parser_parse_type();
					type = gen_parse_forward_or_definition(which, from_typedef);
					// <ModuleFalgs> typedef <UnderlyingType>
				}
				else if (tok.Type == Tok_BraceCurly_Close)
				{
					// Its a definition
					// <which> { ... };
					type = gen_parse_forward_or_definition(currtok.Type, from_typedef);
					// <ModuleFalgs> typedef <UnderlyingType>
				}
				else if (tok.Type == Tok_BraceSquare_Close)
				{
					// Its an array definition
					// <which> <type_identifier> <identifier> [ ... ];
					type = gen_cast(gen_Code, gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr));
					// <ModuleFalgs> typedef <UnderlyingType>
				}
				else
				{
					gen_log_failure("Unsupported or bad member definition after struct declaration\n%SB", gen_parser_to_strbuilder(gen__ctx->parser));
					gen_parser_pop(&gen__ctx->parser);
					return gen_InvalidCode;
				}
			}
		}
		else
		{
			bool from_template = false;
			type               = gen_cast(gen_Code, gen_parser_parse_type(from_template, &is_function));
			// <ModuleFalgs> typedef <UnderlyingType>
		}

		if (check(Tok_Identifier))
		{
			name = currtok;
			eat(Tok_Identifier);
			// <ModuleFalgs> typedef <UnderlyingType> <Name>
		}
		else if (! is_function)
		{
			gen_log_failure("Error, expected identifier for typedef\n%SB", gen_parser_to_strbuilder(gen__ctx->parser));
			gen_parser_pop(&gen__ctx->parser);
			return gen_InvalidCode;
		}

		gen_array_expr = gen_parse_array_decl();
		// <UnderlyingType> + <ArrayExpr>
	}

	gen_Token stmt_end = currtok;
	eat(Tok_Statement_End);
	// <ModuleFalgs> typedef <UnderlyingType> <Name>;

	gen_CodeComment inline_cmt = gen_NullCode;
	if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
		inline_cmt = gen_parse_comment();
	// <ModuleFalgs> typedef <UnderlyingType> <Name> <ArrayExpr>; <InlineCmt>

	gen_CodeTypedef result = (gen_CodeTypedef)gen_make_code();
	result->Type           = CT_Typedef;
	result->ModuleFlags    = mflags;

	if (is_function)
	{
		result->Name       = type->Name;
		result->IsFunction = true;
	}
	else
	{
		result->Name       = gen_cache_str(name.Text);
		result->IsFunction = false;
	}

	if (type)
	{
		result->UnderlyingType         = type;
		result->UnderlyingType->Parent = gen_cast(gen_Code, result);
	}
	// Type needs to be aware of its parent so that it can be serialized properly.

	if (type->Type == CT_Typename && gen_array_expr && gen_array_expr->Type != CT_Invalid)
		gen_cast(gen_CodeTypename, type)->ArrExpr = gen_array_expr;

	if (inline_cmt)
		result->InlineCmt = inline_cmt;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_neverinline gen_CodeUnion gen_parser_parse_union(bool inplace_def)
{
	push_scope();

	gen_ModuleFlag mflags = ModuleFlag_None;

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <ModuleFlags>

	eat(Tok_Decl_Union);
	// <ModuleFlags> union

	gen_CodeAttributes attributes = gen_parse_attributes();
	// <ModuleFlags> union <Attributes>

	gen_Str name = { gen_nullptr, 0 };
	if (check(Tok_Identifier))
	{
		name                         = currtok.Text;
		gen__ctx->parser.Scope->Name = currtok.Text;
		eat(Tok_Identifier);
	}
	// <ModuleFlags> union <Attributes> <Name>

	gen_CodeBody body = { gen_nullptr };

	if (! inplace_def || ! check(Tok_Identifier))
	{
		eat(Tok_BraceCurly_Open);
		// <ModuleFlags> union <Attributes> <Name> {

		body       = gen_cast(gen_CodeBody, gen_make_code());
		body->Type = CT_Union_Body;

		while (! check_noskip(Tok_BraceCurly_Close))
		{
			if (currtok_noskip.Type == Tok_Preprocess_Hash)
				eat(Tok_Preprocess_Hash);

			gen_Code member = { gen_nullptr };
			switch (currtok_noskip.Type)
			{
				case Tok_NewLine:
					member = gen_fmt_newline;
					eat(Tok_NewLine);
					break;

				case Tok_Comment:
					member = gen_cast(gen_Code, gen_parse_comment());
					break;

					// TODO(Ed) : Unions can have gen_constructor_s and gen_destructor_s

				case Tok_Decl_Class:
					member = gen_parse_complicated_definition(Tok_Decl_Class);
					break;

				case Tok_Decl_Enum:
					member = gen_parse_complicated_definition(Tok_Decl_Enum);
					break;

				case Tok_Decl_Struct:
					member = gen_parse_complicated_definition(Tok_Decl_Struct);
					break;

				case Tok_Decl_Union:
					member = gen_parse_complicated_definition(Tok_Decl_Union);
					break;

				case Tok_Preprocess_Define:
					member = gen_cast(gen_Code, gen_parser_parse_define());
					break;

				case Tok_Preprocess_If:
				case Tok_Preprocess_IfDef:
				case Tok_Preprocess_IfNotDef:
				case Tok_Preprocess_ElIf:
					member = gen_cast(gen_Code, gen_parse_preprocess_cond());
					break;

				case Tok_Preprocess_Else:
					member = gen_cast(gen_Code, gen_preprocess_else);
					eat(Tok_Preprocess_Else);
					break;

				case Tok_Preprocess_EndIf:
					member = gen_cast(gen_Code, gen_preprocess_endif);
					eat(Tok_Preprocess_EndIf);
					break;

				case Tok_Preprocess_Macro_Typename:
					// Its a variable with a macro typename
					member = gen_cast(gen_Code, gen_parser_parse_variable());
					break;

				case Tok_Preprocess_Macro_Stmt:
					member = gen_parse_simple_preprocess(Tok_Preprocess_Macro_Stmt);
					break;

				case Tok_Preprocess_Pragma:
					member = gen_cast(gen_Code, gen_parse_pragma());
					break;

				case Tok_Preprocess_Unsupported:
					member = gen_parse_simple_preprocess(Tok_Preprocess_Unsupported);
					break;

				default:
					member = gen_cast(gen_Code, gen_parser_parse_variable());
					break;
			}

			if (member)
				gen_body_append(body, member);
		}
		// <ModuleFlags> union <Attributes> <Name> { <Body>

		eat(Tok_BraceCurly_Close);
		// <ModuleFlags> union <Attributes> <Name> { <Body> }
	}

	if (! inplace_def)
		eat(Tok_Statement_End);
	// <ModuleFlags> union <Attributes> <Name> { <Body> };

	gen_CodeUnion result = (gen_CodeUnion)gen_make_code();
	result->Type         = body ? CT_Union : CT_Union_Fwd;
	result->ModuleFlags  = mflags;

	if (name.Len)
		result->Name = gen_cache_str(name);

	result->Body       = body;
	result->Attributes = attributes;

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeUsing gen_parser_parse_using()
{
	push_scope();

	gen_Specifier specs_found[16]   = { Spec_Invalid };
	gen_s32       NumSpecifiers     = 0;

	gen_Token        name           = gen_NullToken;
	gen_Code         gen_array_expr = { gen_nullptr };
	gen_CodeTypename type           = { gen_nullptr };

	bool is_namespace               = false;

	gen_ModuleFlag     mflags       = ModuleFlag_None;
	gen_CodeAttributes attributes   = { gen_nullptr };

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <ModuleFlags>

	eat(Tok_Decl_Using);
	// <ModuleFlags> using

	if (currtok.Type == Tok_Decl_Namespace)
	{
		is_namespace = true;
		eat(Tok_Decl_Namespace);
		// <ModuleFlags> using namespace
	}

	name                         = currtok;
	gen__ctx->parser.Scope->Name = name.Text;
	eat(Tok_Identifier);
	// <ModuleFlags> using <namespace> <Name>

	if (! is_namespace)
	{
		attributes = gen_parse_attributes();
		// <ModuleFlags> using <Name> <Attributes>

		if (gen_bitfield_is_set(gen_u32, currtok.Flags, TF_Assign))
		{
			attributes = gen_parse_attributes();
			// <ModuleFlags> using <Name> <Attributes>

			eat(Tok_Operator);
			// <ModuleFlags> using <Name> <Attributes> =

			type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
			// <ModuleFlags> using <Name> <Attributes> = <UnderlyingType>

			gen_array_expr = gen_parse_array_decl();
			// <UnderlyingType> + <ArrExpr>
		}
	}

	gen_Token stmt_end = currtok;
	eat(Tok_Statement_End);
	// <ModuleFlags> using <namespace> <Attributes> <Name> = <UnderlyingType>;

	gen_CodeComment inline_cmt = gen_NullCode;
	if (currtok_noskip.Type == Tok_Comment && currtok_noskip.Line == stmt_end.Line)
	{
		inline_cmt = gen_parse_comment();
	}
	// <ModuleFlags> using <namespace> <Attributes> <Name> = <UnderlyingType>; <InlineCmt>

	gen_CodeUsing result = (gen_CodeUsing)gen_make_code();
	result->Name         = gen_cache_str(name.Text);
	result->ModuleFlags  = mflags;

	if (is_namespace)
	{
		result->Type = CT_Using_Namespace;
	}
	else
	{
		result->Type = CT_Using;

		if (type)
			result->UnderlyingType = type;

		if (gen_array_expr)
			type->ArrExpr = gen_array_expr;

		if (attributes)
			result->Attributes = attributes;

		if (inline_cmt)
			result->InlineCmt = inline_cmt;
	}

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeVar gen_parser_parse_variable()
{
	push_scope();

	gen_Specifier specs_found[16] = { Spec_NumSpecifiers };
	gen_s32       NumSpecifiers   = 0;

	gen_ModuleFlag     mflags     = ModuleFlag_None;
	gen_CodeAttributes attributes = { gen_nullptr };
	gen_CodeSpecifiers specifiers = { gen_nullptr };

	if (check(Tok_Module_Export))
	{
		mflags = ModuleFlag_Export;
		eat(Tok_Module_Export);
	}
	// <ModuleFlags>

	attributes = gen_parse_attributes();
	// <ModuleFlags> <Attributes>

	while (left && gen_tok_is_specifier(currtok))
	{
		gen_Specifier spec = gen_str_to_specifier(currtok.Text);
		switch (spec)
		{
		GEN_PARSER_VARIABLE_ALLOWED_SPECIFIER_CASES:
			break;

			default:
				gen_log_failure(
				    "Invalid specifier %S for variable\n%S",
				    gen_spec_to_str(spec),
				    gen_strbuilder_to_str(gen_parser_to_strbuilder(gen__ctx->parser))
				);
				gen_parser_pop(&gen__ctx->parser);
				return gen_InvalidCode;
		}

		// Ignore const specifiers, they're handled by the type
		if (spec == Spec_Const)
			break;

		specs_found[NumSpecifiers] = spec;
		NumSpecifiers++;
		eat(currtok.Type);
	}

	if (NumSpecifiers)
	{
		specifiers = gen_def_specifiers_arr(NumSpecifiers, specs_found);
	}
	// <ModuleFlags> <Attributes> <Specifiers>

	gen_CodeTypename type = gen_parser_parse_type(gen_parser_not_from_template, gen_nullptr);
	// <ModuleFlags> <Attributes> <Specifiers> <ValueType>

	if (gen_cast(gen_Code, type) == gen_Code_Invalid)
		return gen_InvalidCode;

	gen__ctx->parser.Scope->Name = gen_parse_identifier(gen_nullptr).Text;
	// <ModuleFlags> <Attributes> <Specifiers> <ValueType> <Name>

	gen_CodeVar result = gen_parse_variable_after_name(mflags, attributes, specifiers, type, gen__ctx->parser.Scope->Name);
	// Regular  : <ModuleFlags> <Attributes> <Specifiers> <ValueType> <Name>                  = <Value>; <InlineCmt>
	// Bitfield : <ModuleFlags> <Attributes> <Specifiers> <ValueType> <Name> : <BitfieldSize> = <Value>; <InlineCmt>

	gen_parser_pop(&gen__ctx->parser);
	return result;
}

gen_internal gen_CodeTypename gen_parser_parse_type_alt(bool from_template, bool* gen_typedef_is_functon)
{
	return gen_InvalidCode;
}

#ifdef CHECK_WAS_DEFINED

#pragma pop_macro("check")

#endif

// Publically Exposed Interface

gen_CodeClass gen_parse_class( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	push_scope();
	gen_CodeClass result = (gen_CodeClass) gen_parse_class_struct( Tok_Decl_Class, gen_parser_not_inplace_def );
	gen_parser_pop(& gen__ctx->parser);
	return result;
}

gen_CodeConstructor gen_parse_constructor( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	// TODO(Ed): Constructors can have prefix attributes

	gen_CodeSpecifiers specifiers = gen_NullCode;
	gen_Specifier      specs_found[ 16 ] = { Spec_NumSpecifiers };
	gen_s32            NumSpecifiers = 0;

	while ( left && gen_tok_is_specifier(currtok) )
	{
		gen_Specifier spec = gen_str_to_specifier( currtok.Text );

		gen_b32 ignore_spec = false;

		switch ( spec )
		{
			case Spec_Constexpr :
			case Spec_Explicit:
			case Spec_Inline :
			case Spec_ForceInline :
			case Spec_NeverInline :
				break;

			case Spec_Const :
				ignore_spec = true;
				break;

			default :
				gen_log_failure( "Invalid specifier %s for variable\n%S", gen_spec_to_str( spec ), gen_parser_to_strbuilder(gen__ctx->parser) );
				gen_parser_pop(& gen__ctx->parser);
				return gen_InvalidCode;
		}

		// Every specifier after would be considered part of the type type signature
		if (ignore_spec)
			break;

		specs_found[ NumSpecifiers ] = spec;
		NumSpecifiers++;
		eat( currtok.Type );
	}

	if ( NumSpecifiers )
	{
		specifiers = gen_def_specifiers_arr( NumSpecifiers, specs_found );
		// <specifiers> ...
	}

	gen__ctx->parser.Tokens         = toks;
	gen_CodeConstructor result = gen_parser_parse_constructor( specifiers );
	return result;
}

gen_CodeDefine gen_parse_define( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	push_scope();
	gen_CodeDefine result = gen_parser_parse_define();
	gen_parser_pop(& gen__ctx->parser);
	return result;
}

gen_CodeDestructor gen_parse_destructor( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	// TODO(Ed): Destructors can have prefix attributes
	// TODO(Ed): Destructors can have virtual

	gen__ctx->parser.Tokens        = toks;
	gen_CodeDestructor result = gen_parser_parse_destructor(gen_NullCode);
	return result;
}

gen_CodeEnum gen_parse_enum( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
	{
		gen_parser_pop(& gen__ctx->parser);
		return gen_InvalidCode;
	}

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_enum( gen_parser_not_inplace_def);
}

gen_CodeBody gen_parse_export_body( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_export_body();
}

gen_CodeExtern gen_parse_extern_link( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_extern_link();
}

gen_CodeFriend gen_parse_friend( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_friend();
}

gen_CodeFn gen_parse_function( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return (gen_CodeFn) gen_parser_parse_function();
}

gen_CodeBody gen_parse_global_body( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	push_scope();
	gen_CodeBody result = gen_parse_global_nspace( CT_Global_Body );
	gen_parser_pop(& gen__ctx->parser);
	return result;
}

gen_CodeNS gen_parse_namespace( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_namespace();
}

gen_CodeOperator gen_parse_operator( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return (gen_CodeOperator) gen_parser_parse_operator();
}

gen_CodeOpCast gen_parse_operator_cast( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_operator_cast(gen_NullCode);
}

gen_CodeStruct gen_parse_struct( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	push_scope();
	gen_CodeStruct result = (gen_CodeStruct) gen_parse_class_struct( Tok_Decl_Struct, gen_parser_not_inplace_def );
	gen_parser_pop(& gen__ctx->parser);
	return result;
}

gen_CodeTemplate gen_parse_template( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_template();
}

gen_CodeTypename gen_parse_type( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_type( gen_parser_not_from_template, gen_nullptr);
}

gen_CodeTypedef gen_parse_typedef( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_typedef();
}

gen_CodeUnion gen_parse_union( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_union( gen_parser_not_inplace_def);
}

gen_CodeUsing gen_parse_using( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_using();
}

gen_CodeVar gen_parse_variable( gen_Str def )
{
	check_parse_args( def );

	gen_TokArray toks = gen_lex( def );
	if ( toks.Arr == gen_nullptr )
		return gen_InvalidCode;

	gen__ctx->parser.Tokens = toks;
	return gen_parser_parse_variable();
}

// Undef helper macros
#undef check_parse_args
#undef currtok_noskip
#undef currtok
#undef peektok
#undef prevtok
#undef nexttok
#undef nexttok_noskip
#undef eat
#undef left
#undef check
#undef push_scope
#undef gen_def_assign

// Here for C Variant
#undef gen_lex_dont_skip_formatting
#undef gen_lex_skip_formatting

#undef gen_parser_inplace_def
#undef gen_parser_not_inplace_def
#undef gen_parser_dont_consume_braces
#undef gen_parser_consume_braces
#undef gen_parser_not_from_template
#undef gen_parser_use_parenthesis
#undef gen_parser_strip_formatting_dont_preserve_newlines

#pragma endregion Parsing

gen_ssize gen_token_fmt_va( char* buf, gen_usize buf_size, gen_s32 num_tokens, va_list va )
{
	char const* buf_begin = buf;
	gen_ssize       remaining = buf_size;

	if (gen__ctx->gen_token_fmt_map.Hashes == gen_nullptr) {
		gen__ctx->gen_token_fmt_map = gen_hashtable_init(gen_Str, gen__ctx->Allocator_DyanmicContainers );
	}
	// Populate token pairs
	{
		gen_s32 left = num_tokens - 1;

		while ( left-- )
		{
			char const* token = va_arg( va, char const* );
			gen_Str         value = va_arg( va, gen_Str );

			gen_u32 key = gen_crc32( token, gen_c_str_len(token) );
			gen_hashtable_set( gen__ctx->gen_token_fmt_map, key, value );
		}
	}

	char const* fmt     = va_arg( va, char const* );
	char        current = *fmt;

	while ( current )
	{
		gen_ssize len = 0;

		while ( current && current != '<' && remaining )
		{
			* buf = * fmt;
			buf++;
			fmt++;
			remaining--;

			current = * fmt;
		}

		if ( current == '<' )
		{
			char const* scanner = fmt + 1;

			gen_s32 gen_tok_len = 0;

			while ( *scanner != '>' )
			{
				gen_tok_len++;
				scanner++;
			}

			char const* token = fmt + 1;

			gen_u32      key   = gen_crc32( token, gen_tok_len );
			gen_Str*     value = gen_hashtable_get(gen__ctx->gen_token_fmt_map, key );

			if ( value )
			{
				gen_ssize          left = value->Len;
				char const* str  = value->Ptr;

				while ( left-- )
				{
					* buf = * str;
					buf++;
					str++;
					remaining--;
				}

				scanner++;
				fmt     = scanner;
				current = * fmt;
				continue;
			}

			* buf = * fmt;
			buf++;
			fmt++;
			remaining--;

			current = * fmt;
		}
	}
	gen_hashtable_clear(gen__ctx->gen_token_fmt_map);
	gen_ssize result = buf_size - remaining;
	return result;
}

gen_Code gen_untyped_str( gen_Str content )
{
	if ( content.Len == 0 )
	{
		gen_log_failure( "gen_untyped_str: empty string" );
		return gen_InvalidCode;
	}

	gen_Code
	result          = gen_make_code();
	result->Name    = gen_cache_str( content );
	result->Type    = CT_Untyped;
	result->Content = result->Name;

	if ( result->Name.Len == 0 )
	{
		gen_log_failure( "gen_untyped_str: could not cache string" );
		return gen_InvalidCode;
	}

	return result;
}

gen_Code gen_untyped_fmt( char const* fmt, ...)
{
	if ( fmt == gen_nullptr )
	{
		gen_log_failure( "gen_untyped_fmt: null format string" );
		return gen_InvalidCode;
	}

	gen_local_persist gen_thread_local
	char buf[GEN_PRINTF_MAXLEN] = { 0 };

	va_list va;
	va_start(va, fmt);
	gen_ssize length = gen_c_str_fmt_va(buf, GEN_PRINTF_MAXLEN, fmt, va);
	va_end(va);
    gen_Str content = { buf, length };

	gen_Code
	result          = gen_make_code();
	result->Type    = CT_Untyped;
	result->Content = gen_cache_str( content );

	if ( result->Name.Len == 0 )
	{
		gen_log_failure( "gen_untyped_fmt: could not cache string" );
		return gen_InvalidCode;
	}

	return result;
}

gen_Code gen_untyped_token_fmt( gen_s32 num_tokens, char const* fmt, ... )
{
	if ( num_tokens == 0 )
	{
		gen_log_failure( "gen_untyped_token_fmt: zero tokens" );
		return gen_InvalidCode;
	}

	gen_local_persist gen_thread_local
	char buf[GEN_PRINTF_MAXLEN] = { 0 };

	va_list va;
	va_start(va, fmt);
	gen_ssize length = gen_token_fmt_va(buf, GEN_PRINTF_MAXLEN, num_tokens, va);
	va_end(va);

	gen_Str buf_str = { buf, length };

	gen_Code
	result          = gen_make_code();
	result->Type    = CT_Untyped;
	result->Content = gen_cache_str( buf_str );

	if ( result->Name.Len == 0 )
	{
		gen_log_failure( "gen_untyped_fmt: could not cache string" );
		return gen_InvalidCode;
	}

	return result;
}

#pragma endregion Interface
#pragma region gen_Builder

gen_Builder gen_builder_open(char const* path)
{
	gen_Builder result;

	gen_FileError error = gen_file_open_mode(&result.File, EFileMode_WRITE, path);
	if (error != EFileError_NONE)
	{
		gen_log_failure("gen::File::open - Could not open file: %s", path);
		return result;
	}

	gen_Context* ctx = gen_get_context();
	GEN_ASSERT_NOT_NULL(ctx);
	result.Buffer = gen_strbuilder_make_reserve(ctx->Allocator_Temp, ctx->InitSize_BuilderBuffer);

	// gen_log_fmt("$gen_Builder - Opened file: %s\n", result.File.filename );
	return result;
}

void gen_builder_pad_lines(gen_Builder* builder, gen_s32 num)
{
	gen_strbuilder_append_str(&builder->Buffer, gen_txt("\n"));
}

void gen_builder__print(gen_Builder* builder, gen_Code code)
{
	gen_StrBuilder str = gen_code_to_strbuilder(code);
	// const gen_ssize len = str.length();
	// gen_log_fmt( "%s - print: %.*s\n", File.filename, len > 80 ? 80 : len, str.Data );
	gen_strbuilder_append_string(&builder->Buffer, str);
}

void gen_builder_print_fmt_va(gen_Builder* builder, char const* fmt, va_list va)
{
	gen_ssize res;
	char      buf[GEN_PRINTF_MAXLEN] = { 0 };

	res                              = gen_c_str_fmt_va(buf, gen_count_of(buf) - 1, fmt, va) - 1;

	gen_strbuilder_append_c_str_len((gen_StrBuilder*)&(builder->Buffer), (char const*)buf, res);
}

void gen_builder_write(gen_Builder* builder)
{
	gen_b32 result = gen_file_write(&builder->File, builder->Buffer, gen_strbuilder_length(builder->Buffer));

	if (result == false)
		gen_log_failure("gen::File::write - Failed to write to file: %s\n", gen_file_name(&builder->File));

	gen_log_fmt("Generated: %s\n", builder->File.filename);
	gen_file_close(&builder->File);
	gen_strbuilder_free(&builder->Buffer);
}

#pragma endregion gen_Builder

#pragma region Scanner

gen_Code gen_scan_file(char const* path)
{
	gen_FileInfo file;

	gen_FileError error = gen_file_open_mode(&file, EFileMode_READ, path);
	if (error != EFileError_NONE)
	{
		GEN_FATAL("gen_scan_file: Could not open: %s", path);
	}

	gen_ssize fsize = gen_file_size(&file);
	if (fsize <= 0)
	{
		GEN_FATAL("gen_scan_file: %s is empty", path);
	}

	gen_StrBuilder str = gen_strbuilder_make_reserve(gen_get_context()->Allocator_Temp, fsize);
	gen_file_read(&file, str, fsize);
	gen_strbuilder_get_header(str)->Length = fsize;

	// Skip INTELLISENSE_DIRECTIVES preprocessor blocks
	// Its designed so that the directive should be the first thing in the file.
	// Anything that comes before it will also be omitted.
	{
#define current (*scanner)
#define matched 0
#define move_fwd() \
	do             \
	{              \
		++scanner; \
		--left;    \
	} while (0)
		const gen_Str directive_start      = gen_txt("ifdef");
		const gen_Str directive_end        = gen_txt("endif");
		const gen_Str gen_def_intellisense = gen_txt("INTELLISENSE_DIRECTIVES");

		bool        found_directive        = false;
		char const* scanner                = (char const*)str;
		gen_s32     left                   = fsize;
		while (left)
		{
			// Processing directive.
			if (current == '#')
			{
				move_fwd();
				while (left && gen_char_is_space(current))
					move_fwd();

				if (! found_directive)
				{
					if (left && gen_c_str_compare_len(scanner, directive_start.Ptr, directive_start.Len) == matched)
					{
						scanner += directive_start.Len;
						left    -= directive_start.Len;

						while (left && gen_char_is_space(current))
							move_fwd();

						if (left && gen_c_str_compare_len(scanner, gen_def_intellisense.Ptr, gen_def_intellisense.Len) == matched)
						{
							scanner         += gen_def_intellisense.Len;
							left            -= gen_def_intellisense.Len;

							found_directive  = true;
						}
					}

					// Skip to end of line
					while (left && current != '\r' && current != '\n')
						move_fwd();
					move_fwd();

					if (left && current == '\n')
						move_fwd();

					continue;
				}

				if (left && gen_c_str_compare_len(scanner, directive_end.Ptr, directive_end.Len) == matched)
				{
					scanner += directive_end.Len;
					left    -= directive_end.Len;

					// Skip to end of line
					while (left && current != '\r' && current != '\n')
						move_fwd();
					move_fwd();

					if (left && current == '\n')
						move_fwd();

					// gen_sptr skip_size = fsize - left;
					if ((scanner + 2) >= ((char const*)str + fsize))
					{
						gen_mem_move(str, scanner, left);
						gen_strbuilder_get_header(str)->Length = left;
						break;
					}

					gen_mem_move(str, scanner, left);
					gen_strbuilder_get_header(str)->Length = left;

					break;
				}
			}

			move_fwd();
		}
#undef move_fwd
#undef matched
#undef current
	}

	gen_file_close(&file);
	return gen_untyped_str(gen_strbuilder_to_str(str));
}

gen_CodeBody gen_parse_file(const char* path)
{
	FileContents file    = gen_file_read_contents(gen_get_context()->Allocator_Temp, true, path);
	gen_Str      content = { (char const*)file.data, file.size };
	gen_CodeBody code    = gen_parse_global_body(content);
	gen_log_fmt("\nParsed: %s\n", path);
	return code;
}

gen_CSV_Column gen_parse_csv_one_column(gen_AllocatorInfo allocator, char const* path)
{
	FileContents content       = gen_file_read_contents(allocator, gen_file_zero_terminate, path);
	gen_Arena    gen_csv_arena = gen_arena_init_from_memory(content.data, content.size);

	gen_CSV_Column result;
	gen_csv_parse(&result.ADT, gen_rcast(char*, content.data), allocator, false);
	result.Content = result.ADT.nodes[0].nodes;
	return result;
}

gen_CSV_Columns2 gen_parse_csv_two_columns(gen_AllocatorInfo allocator, char const* path)
{
	FileContents content       = gen_file_read_contents(allocator, gen_file_zero_terminate, path);
	gen_Arena    gen_csv_arena = gen_arena_init_from_memory(content.data, content.size);

	gen_CSV_Columns2 result;
	gen_csv_parse(&result.ADT, gen_rcast(char*, content.data), allocator, false);
	result.Col_1 = result.ADT.nodes[0].nodes;
	result.Col_2 = result.ADT.nodes[1].nodes;
	return result;
}

#pragma endregion Scanner

GEN_API_C_END
GEN_NS_END

#endif
#pragma endregion GENCPP IMPLEMENTATION GUARD
#ifdef __clang__
#	pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#	pragma GCC diagnostic pop
#endif
