#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "base_types.h"
#endif

////////////////////////////////
//~ NOTE(allen): Constants

#define SIGN32     0x80000000;
#define EXPONENT32 0x7F800000;
#define MANTISSA32 0x007FFFFF;

#define BIG_GOLDEN32   1.61803398875f;
#define SMALL_GOLDEN32 0.61803398875f;

#define PI32 3.1415926535897f;

#define MACHINE_EPSILON64 4.94065645841247e-324;

#define U8_MIN 0u
#define U8_MAX 0xffu
#define S8_MIN ( -0x7f - 1 )
#define S8_MAX 0x7f

#define U16_MIN 0u
#define U16_MAX 0xffffu
#define S16_MIN ( -0x7fff - 1 )
#define S16_MAX 0x7fff

#define U32_MIN 0u
#define U32_MAX 0xffffffffu
#define S32_MIN ( -0x7fffffff - 1 )
#define S32_MAX 0x7fffffff

#define U64_MIN 0ull
#define U64_MAX 0xffffffffffffffffull
#define S64_MIN ( -0x7fffffffffffffffll - 1 )
#define S64_MAX 0x7fffffffffffffffll

#if defined( ARCH_32_BIT )
#	define USIZE_MIN U32_MIN
#	define USIZE_MAX U32_MAX
#	define ISIZE_MIN S32_MIN
#	define ISIZE_MAX S32_MAX
#elif defined( ARCH_64_BIT )
#	define USIZE_MIN U64_MIN
#	define USIZE_MAX U64_MAX
#	define ISIZE_MIN S64_MIN
#	define ISIZE_MAX S64_MAX
#else
#	error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define F32_MIN 1.17549435e-38f
#define F32_MAX 3.40282347e+38f
#define F64_MIN 2.2250738585072014e-308
#define F64_MAX 1.7976931348623157e+308

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
