#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "generic_macros.h"
#	include "platform.h"
#endif

#if defined( COMPILER_MSVC )
#	if _MSC_VER < 1300
typedef unsigned char,  U8;
typedef signed   char,  S8;
typedef unsigned short, U16;
typedef signed   short, S16;
typedef unsigned int,   U32;
typedef signed   int,   S32;
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

typedef uint8_t,  U8;
typedef int8_t,   S8;
typedef uint16_t, U16;
typedef int16_t,  S16;
typedef uint32_t, U32;
typedef int32_t,  S32;
typedef uint64_t, U64;
typedef int64_t,  S64;
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

#ifndef size_of
#define size_of( x ) ( SSIZE )( sizeof( x ) )
#endif

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
#	endif
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

typedef void VoidProc(void);
distinct_register_selector(VoidProc);

////////////////////////////////
//~ NOTE(allen): Constants

#define SIGN32     0x80000000
#define EXPONENT32 0x7F800000
#define MANTISSA32 0x007FFFFF

#define BIG_GOLDEN32   1.61803398875f
#define SMALL_GOLDEN32 0.61803398875f

#define PI32 3.1415926535897f

#define MACHINE_EPSILON64 4.94065645841247e-324

#define MIN_U8 0u
#define MAX_U8 0xffu
#define MIN_S8 ( -0x7f - 1 )
#define MAX_S8 0x7f

#define MIN_U16 0u
#define MAX_U16 0xffffu
#define MIN_S16 ( -0x7fff - 1 )
#define MAX_S16 0x7fff

#define MIN_U32 0u
#define MAX_U32 0xffffffffu
#define MIN_S32 ( -0x7fffffff - 1 )
#define MAX_S32 0x7fffffff

#define MIN_U64 0ull
#define MAX_U64 0xffffffffffffffffull
#define MIN_S64 ( -0x7fffffffffffffffll - 1 )
#define MAX_S64 0x7fffffffffffffffll

#if ARCH_32BIT
#	define MIN_USIZE U32_MIN
#	define MAX_USIZE U32_MAX
#	define MIN_SSIZE S32_MIN
#	define MAX_SSIZE S32_MAX
#elif ARCH_64BIT
#	define MIN_USIZE U64_MIN
#	define MAX_USIZE U64_MAX
#	define MIN_SSIZE S64_MIN
#	define MAX_SSIZE S64_MAX
#else
#	error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define MIN_F32 1.17549435e-38f
#define MAX_F32 3.40282347e+38f
#define MIN_F64 2.2250738585072014e-308
#define MAX_F64 1.7976931348623157e+308

#define BITMASK1  0x00000001
#define BITMASK2  0x00000003
#define BITMASK3  0x00000007
#define BITMASK4  0x0000000f
#define BITMASK5  0x0000001f
#define BITMASK6  0x0000003f
#define BITMASK7  0x0000007f
#define BITMASK8  0x000000ff
#define BITMASK9  0x000001ff
#define BITMASK10 0x000003ff
#define BITMASK11 0x000007ff
#define BITMASK12 0x00000fff
#define BITMASK13 0x00001fff
#define BITMASK14 0x00003fff
#define BITMASK15 0x00007fff
#define BITMASK16 0x0000ffff
#define BITMASK17 0x0001ffff
#define BITMASK18 0x0003ffff
#define BITMASK19 0x0007ffff
#define BITMASK20 0x000fffff
#define BITMASK21 0x001fffff
#define BITMASK22 0x003fffff
#define BITMASK23 0x007fffff
#define BITMASK24 0x00ffffff
#define BITMASK25 0x01ffffff
#define BITMASK26 0x03ffffff
#define BITMASK27 0x07ffffff
#define BITMASK28 0x0fffffff
#define BITMASK29 0x1fffffff
#define BITMASK30 0x3fffffff
#define BITMASK31 0x7fffffff
#define BITMASK32 0xffffffff

#define BITMASK33 0x00000001ffffffffull
#define BITMASK34 0x00000003ffffffffull
#define BITMASK35 0x00000007ffffffffull
#define BITMASK36 0x0000000fffffffffull
#define BITMASK37 0x0000001fffffffffull
#define BITMASK38 0x0000003fffffffffull
#define BITMASK39 0x0000007fffffffffull
#define BITMASK40 0x000000ffffffffffull
#define BITMASK41 0x000001ffffffffffull
#define BITMASK42 0x000003ffffffffffull
#define BITMASK43 0x000007ffffffffffull
#define BITMASK44 0x00000fffffffffffull
#define BITMASK45 0x00001fffffffffffull
#define BITMASK46 0x00003fffffffffffull
#define BITMASK47 0x00007fffffffffffull
#define BITMASK48 0x0000ffffffffffffull
#define BITMASK49 0x0001ffffffffffffull
#define BITMASK50 0x0003ffffffffffffull
#define BITMASK51 0x0007ffffffffffffull
#define BITMASK52 0x000fffffffffffffull
#define BITMASK53 0x001fffffffffffffull
#define BITMASK54 0x003fffffffffffffull
#define BITMASK55 0x007fffffffffffffull
#define BITMASK56 0x00ffffffffffffffull
#define BITMASK57 0x01ffffffffffffffull
#define BITMASK58 0x03ffffffffffffffull
#define BITMASK59 0x07ffffffffffffffull
#define BITMASK60 0x0fffffffffffffffull
#define BITMASK61 0x1fffffffffffffffull
#define BITMASK62 0x3fffffffffffffffull
#define BITMASK63 0x7fffffffffffffffull
#define BITMASK64 0xffffffffffffffffull

#define BIT1  (1 << 0)
#define BIT2  (1 << 1)
#define BIT3  (1 << 2)
#define BIT4  (1 << 3)
#define BIT5  (1 << 4)
#define BIT6  (1 << 5)
#define BIT7  (1 << 6)
#define BIT8  (1 << 7)
#define BIT9  (1 << 8)
#define BIT10 (1 << 9)
#define BIT11 (1 << 10)
#define BIT12 (1 << 11)
#define BIT13 (1 << 12)
#define BIT14 (1 << 13)
#define BIT15 (1 << 14)
#define BIT16 (1 << 15)
#define BIT17 (1 << 16)
#define BIT18 (1 << 17)
#define BIT19 (1 << 18)
#define BIT20 (1 << 19)
#define BIT21 (1 << 20)
#define BIT22 (1 << 21)
#define BIT23 (1 << 22)
#define BIT24 (1 << 23)
#define BIT25 (1 << 24)
#define BIT26 (1 << 25)
#define BIT27 (1 << 26)
#define BIT28 (1 << 27)
#define BIT29 (1 << 28)
#define BIT30 (1 << 29)
#define BIT31 (1 << 30)
#define BIT32 (1 << 31)

#define BIT33 (1ull << 32)
#define BIT34 (1ull << 33)
#define BIT35 (1ull << 34)
#define BIT36 (1ull << 35)
#define BIT37 (1ull << 36)
#define BIT38 (1ull << 37)
#define BIT39 (1ull << 38)
#define BIT40 (1ull << 39)
#define BIT41 (1ull << 40)
#define BIT42 (1ull << 41)
#define BIT43 (1ull << 42)
#define BIT44 (1ull << 43)
#define BIT45 (1ull << 44)
#define BIT46 (1ull << 45)
#define BIT47 (1ull << 46)
#define BIT48 (1ull << 47)
#define BIT49 (1ull << 48)
#define BIT50 (1ull << 49)
#define BIT51 (1ull << 50)
#define BIT52 (1ull << 51)
#define BIT53 (1ull << 52)
#define BIT54 (1ull << 53)
#define BIT55 (1ull << 54)
#define BIT56 (1ull << 55)
#define BIT57 (1ull << 56)
#define BIT58 (1ull << 57)
#define BIT59 (1ull << 58)
#define BIT60 (1ull << 59)
#define BIT61 (1ull << 60)
#define BIT62 (1ull << 61)
#define BIT63 (1ull << 62)
#define BIT64 (1ull << 63)

////////////////////////////////
//~ Globally Unique Ids

typedef union Guid Guid;
union Guid
{
	struct
	{
		U32 data1;
		U16 data2;
		U16 data3;
		U8  data4[8];
	};
	U8 v[16];
};
static_assert(size_of(Guid) == 16, "Guid is not 16 bytes");

////////////////////////////////
//~ Arrays

typedef struct U16Array U16Array;
struct U16Array
{
	U64  count;
	U16* v;
};
typedef struct U32Array U32Array;
struct U32Array
{
	U64  count;
	U32* v;
};
typedef struct U64Array U64Array;
struct U64Array
{
	U64  count;
	U64* v;
};
typedef struct U128Array U128Array;
struct U128Array
{
	U64   count;
	U128* v;
};
