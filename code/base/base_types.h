#ifdef MD_INTELLISENSE_DIRECTIVES
#pragma once
#include "macros.h"
#endif

#pragma region Basic Types

#define MD_U8_MIN 0u
#define MD_U8_MAX 0xffu
#define MD_I8_MIN ( -0x7f - 1 )
#define MD_I8_MAX 0x7f

#define MD_U16_MIN 0u
#define MD_U16_MAX 0xffffu
#define MD_I16_MIN ( -0x7fff - 1 )
#define MD_I16_MAX 0x7fff

#define MD_U32_MIN 0u
#define MD_U32_MAX 0xffffffffu
#define MD_I32_MIN ( -0x7fffffff - 1 )
#define MD_I32_MAX 0x7fffffff

#define MD_U64_MIN 0ull
#define MD_U64_MAX 0xffffffffffffffffull
#define MD_I64_MIN ( -0x7fffffffffffffffll - 1 )
#define MD_I64_MAX 0x7fffffffffffffffll

#if defined( MD_ARCH_32_BIT )
#	define MD_USIZE_MIN GEN_U32_MIN
#	define MD_USIZE_MAX GEN_U32_MAX
#	define MD_ISIZE_MIN GEN_S32_MIN
#	define MD_ISIZE_MAX GEN_S32_MAX
#elif defined( MD_ARCH_64_BIT )
#	define MD_USIZE_MIN GEN_U64_MIN
#	define MD_USIZE_MAX GEN_U64_MAX
#	define MD_ISIZE_MIN GEN_I64_MIN
#	define MD_ISIZE_MAX GEN_I64_MAX
#else
#	error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define MD_F32_MIN 1.17549435e-38f
#define MD_F32_MAX 3.40282347e+38f
#define MD_F64_MIN 2.2250738585072014e-308
#define MD_F64_MAX 1.7976931348623157e+308

#if defined( MD_COMPILER_MSVC )
#	if _MSC_VER < 1300
typedef unsigned char  U8;
typedef signed   char  S8;
typedef unsigned short U16;
typedef signed   short S16;
typedef unsigned int   U32;
typedef signed   int   S32;
#    else
typedef unsigned __int8  U8;
typedef signed   __int8  S8;
typedef unsigned __int16 U16;
typedef signed   __int16 S16;
typedef unsigned __int32 U32;
typedef signed   __int32 S32;
#    endif
typedef unsigned __int64 U64;
typedef signed   __int64 S64;
#else
#	include <stdint.h>

typedef uint8_t  U8;
typedef int8_t   S8;
typedef uint16_t U16;
typedef int16_t  S16;
typedef uint32_t U32;
typedef int32_t  S32;
typedef uint64_t U64;
typedef int64_t  S64;
#endif

typedef struct U128 U128;
struct U128 {
	U64 data[2];
};

static_assert( sizeof( U8 )  == sizeof( S8 ),  "sizeof(U8)  != sizeof(S8)" );
static_assert( sizeof( U16 ) == sizeof( S16 ), "sizeof(U16) != sizeof(sS6)" );
static_assert( sizeof( U32 ) == sizeof( S32 ), "sizeof(U32) != sizeof(sS2)" );
static_assert( sizeof( U64 ) == sizeof( S64 ), "sizeof(U64) != sizeof(sS4)" );

static_assert( sizeof( U8 )  == 1, "sizeof(U8) != 1" );
static_assert( sizeof( U16 ) == 2, "sizeof(U16) != 2" );
static_assert( sizeof( U32 ) == 4, "sizeof(U32) != 4" );
static_assert( sizeof( U64 ) == 8, "sizeof(U64) != 8" );

typedef size_t    USIZE;
typedef ptrdiff_t SSIZE;

static_assert( sizeof( USIZE ) == sizeof( SSIZE ), "sizeof(USIZE) != sizeof(SSIZE)" );

// NOTE: (u)zpl_intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
#if defined( _WIN64 )
typedef signed __int64   SPTR;
typedef unsigned __int64 UPTR;
#elif defined( _WIN32 )
// NOTE; To mark types changing their size, e.g. zpl_intptr
#	ifndef _W64
#		if ! defined( __midl ) && ( defined( _X86_ ) || defined( _M_IX86 ) ) && _MSC_VER >= 1300
#			define _W64 __w64
#		else
#			define _W64
#		endif
#	endifk
typedef _W64 signed int   SPTR;
typedef _W64 unsigned int UPTR;
#else
typedef uintptr_t UPTR;
typedef intptr_t  SPTR;
#endif

static_assert( sizeof( UPTR ) == sizeof( SPTR ), "sizeof(UPTR) != sizeof(SPTR)" );

typedef float  F32;
typedef double F64;

static_assert( sizeof( F32 ) == 4, "sizeof(F32) != 4" );
static_assert( sizeof( F64 ) == 8, "sizeof(F64) != 8" );

typedef S8  B8;
typedef S16 B16;
typedef S32 B32;
