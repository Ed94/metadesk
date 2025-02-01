#if MD_INTELLISENSE_DIRECTIVES
#include "cstd.h"
#endif

#ifndef MD_API
#if MD_COMPILER_MSVC
#	ifdef MD_DYN_LINK
#		ifdef MD_DYN_EXPORT
#			define MD_API __declspec(dllexport)
#		else
#			define MD_API __declspec(dllimport)
#		endif
#	else
#		define MD_API  // Empty for static builds
#	endif
#else
#	ifdef MD_DYN_LINK
#		define MD_API __attribute__((visibility("default")))
#	else
#		define MD_API  // Empty for static builds
#	endif
#endif
#endif // GEN_API

#ifndef internal
#define internal static
#endif

#ifndef local_persist
#define local_persist static
#endif

#if MD_COMPILER_MSVC
# define thread_static __declspec(thread)
#elif MD_COMPILER_CLANG || MD_COMPILER_GCC
# define thread_static __thread
#endif

////////////////////////////////
//~ rjf: Branch Predictor Hints

#if defined(__clang__)
# define expect(expr, val) __builtin_expect((expr), (val))
#else
# define expect(expr, val) (expr)
#endif

#define likely(expr)            expect(expr,1)
#define unlikely(expr)          expect(expr,0)

#if MD_COMPILER_CPP
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

#if ! defined(typeof) && (!MD_COMPILER_C || __STDC_VERSION__ < 202311L)
#	if ! MD_COMPILER_C
#		define typeof decltype
#	elif defined(_MSC_VER)
#		define typeof __typeof__
#	elif defined(__GNUC__) || defined(__clang__)
#		define typeof __typeof__
#	else
#		error "Compiler not supported"
#	endif
#endif

#ifndef MD_API_C_BEGIN
#	if MD_COMPILER_C
#		define MD_API_C_BEGIN
#		define MD_API_C_END
#	else
#		define MD_API_C_BEGIN extern "C" {
#		define MD_API_C_END   }
#	endif
#endif

#if MD_COMPILER_C
#	if __STDC_VERSION__ >= 202311L
#		define enum_underlying(type) : type
#	else
#		define enum_underlying(type)
#   endif
#else
#	define enum_underlying(type) : type
#endif

#if MD_COMPILER_C
#	ifndef nullptr
#		define nullptr NULL
#	endif

#	ifndef MD_REMOVE_PTR
#		define MD_REMOVE_PTR(type) typeof(* ( (type) NULL) )
#	endif
#endif

#if ! defined(MD_PARAM_DEFAULT) && MD_COMPILER_CPP
#	define MD_PARAM_DEFAULT = {}
#else
#	define MD_PARAM_DEFAULT
#endif

#if MD_COMPILER_C
#ifndef static_assert
#undef  static_assert
    #if MD_COMPILER_C && __STDC_VERSION__ >= 201112L
        #define static_assert(condition, message) _Static_assert(condition, message)
    #else
        #define static_assert(condition, message) typedef char static_assertion_##__LINE__[(condition)?1:-1]
	#endif
#endif
#endif

#if MD_DONT_USE_NAMESPACE || MD_COMPILER_C
#	if MD_COMPILER_C
#		define MD_NS
#		define MD_NS_BEGIN
#		define MD_NS_END
#	else
#		define MD_NS              ::
#		define MD_NS_BEGIN
#		define MD_NS_END
#	endif
#else
	namespace MD {}
	namespace md = MD;
#	define MD_NS              MD::
#	define MD_NS_BEGIN        namespace MD {
#	define MD_NS_END          }
#endif
