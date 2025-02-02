#ifdef MD_INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "base_types.h"
#endif

////////////////////////////////
//~ NOTE(allen): Constants

#define MD_SIGN32     0x80000000;
#define MD_EXPONENT32 0x7F800000;
#define MD_MANTISSA32 0x007FFFFF;

#define MD_BIG_GOLDEN32   1.61803398875f;
#define MD_SMALL_GOLDEN32 0.61803398875f;

#define MD_PI32 3.1415926535897f;

#define MD_MACHINE_EPSILON64 4.94065645841247e-324;

#define MD_U8_MIN 0u
#define MD_U8_MAX 0xffu
#define MD_S8_MIN ( -0x7f - 1 )
#define MD_S8_MAX 0x7f

#define MD_U16_MIN 0u
#define MD_U16_MAX 0xffffu
#define MD_S16_MIN ( -0x7fff - 1 )
#define MD_S16_MAX 0x7fff

#define MD_U32_MIN 0u
#define MD_U32_MAX 0xffffffffu
#define MD_S32_MIN ( -0x7fffffff - 1 )
#define MD_S32_MAX 0x7fffffff

#define MD_U64_MIN 0ull
#define MD_U64_MAX 0xffffffffffffffffull
#define MD_S64_MIN ( -0x7fffffffffffffffll - 1 )
#define MD_S64_MAX 0x7fffffffffffffffll

#if defined( MD_ARCH_32_BIT )
#	define MD_USIZE_MIN MD_U32_MIN
#	define MD_USIZE_MAX MD_U32_MAX
#	define MD_ISIZE_MIN MD_S32_MIN
#	define MD_ISIZE_MAX MD_S32_MAX
#elif defined( MD_ARCH_64_BIT )
#	define MD_USIZE_MIN MD_U64_MIN
#	define MD_USIZE_MAX MD_U64_MAX
#	define MD_ISIZE_MIN MD_S64_MIN
#	define MD_ISIZE_MAX MD_S64_MAX
#else
#	error Unknown architecture size. This library only supports 32 bit and 64 bit architectures.
#endif

#define MD_F32_MIN 1.17549435e-38f
#define MD_F32_MAX 3.40282347e+38f
#define MD_F64_MIN 2.2250738585072014e-308
#define MD_F64_MAX 1.7976931348623157e+308

#define MD_BITMASK1  0x00000001
#define MD_BITMASK2  0x00000003
#define MD_BITMASK3  0x00000007
#define MD_BITMASK4  0x0000000f
#define MD_BITMASK5  0x0000001f
#define MD_BITMASK6  0x0000003f
#define MD_BITMASK7  0x0000007f
#define MD_BITMASK8  0x000000ff
#define MD_BITMASK9  0x000001ff
#define MD_BITMASK10 0x000003ff
#define MD_BITMASK11 0x000007ff
#define MD_BITMASK12 0x00000fff
#define MD_BITMASK13 0x00001fff
#define MD_BITMASK14 0x00003fff
#define MD_BITMASK15 0x00007fff
#define MD_BITMASK16 0x0000ffff
#define MD_BITMASK17 0x0001ffff
#define MD_BITMASK18 0x0003ffff
#define MD_BITMASK19 0x0007ffff
#define MD_BITMASK20 0x000fffff
#define MD_BITMASK21 0x001fffff
#define MD_BITMASK22 0x003fffff
#define MD_BITMASK23 0x007fffff
#define MD_BITMASK24 0x00ffffff
#define MD_BITMASK25 0x01ffffff
#define MD_BITMASK26 0x03ffffff
#define MD_BITMASK27 0x07ffffff
#define MD_BITMASK28 0x0fffffff
#define MD_BITMASK29 0x1fffffff
#define MD_BITMASK30 0x3fffffff
#define MD_BITMASK31 0x7fffffff
#define MD_BITMASK32 0xffffffff

#define MD_BITMASK33 0x00000001ffffffffull
#define MD_BITMASK34 0x00000003ffffffffull
#define MD_BITMASK35 0x00000007ffffffffull
#define MD_BITMASK36 0x0000000fffffffffull
#define MD_BITMASK37 0x0000001fffffffffull
#define MD_BITMASK38 0x0000003fffffffffull
#define MD_BITMASK39 0x0000007fffffffffull
#define MD_BITMASK40 0x000000ffffffffffull
#define MD_BITMASK41 0x000001ffffffffffull
#define MD_BITMASK42 0x000003ffffffffffull
#define MD_BITMASK43 0x000007ffffffffffull
#define MD_BITMASK44 0x00000fffffffffffull
#define MD_BITMASK45 0x00001fffffffffffull
#define MD_BITMASK46 0x00003fffffffffffull
#define MD_BITMASK47 0x00007fffffffffffull
#define MD_BITMASK48 0x0000ffffffffffffull
#define MD_BITMASK49 0x0001ffffffffffffull
#define MD_BITMASK50 0x0003ffffffffffffull
#define MD_BITMASK51 0x0007ffffffffffffull
#define MD_BITMASK52 0x000fffffffffffffull
#define MD_BITMASK53 0x001fffffffffffffull
#define MD_BITMASK54 0x003fffffffffffffull
#define MD_BITMASK55 0x007fffffffffffffull
#define MD_BITMASK56 0x00ffffffffffffffull
#define MD_BITMASK57 0x01ffffffffffffffull
#define MD_BITMASK58 0x03ffffffffffffffull
#define MD_BITMASK59 0x07ffffffffffffffull
#define MD_BITMASK60 0x0fffffffffffffffull
#define MD_BITMASK61 0x1fffffffffffffffull
#define MD_BITMASK62 0x3fffffffffffffffull
#define MD_BITMASK63 0x7fffffffffffffffull
#define MD_BITMASK64 0xffffffffffffffffull

#define MD_BIT1  (1 << 0)
#define MD_BIT2  (1 << 1)
#define MD_BIT3  (1 << 2)
#define MD_BIT4  (1 << 3)
#define MD_BIT5  (1 << 4)
#define MD_BIT6  (1 << 5)
#define MD_BIT7  (1 << 6)
#define MD_BIT8  (1 << 7)
#define MD_BIT9  (1 << 8)
#define MD_BIT10 (1 << 9)
#define MD_BIT11 (1 << 10)
#define MD_BIT12 (1 << 11)
#define MD_BIT13 (1 << 12)
#define MD_BIT14 (1 << 13)
#define MD_BIT15 (1 << 14)
#define MD_BIT16 (1 << 15)
#define MD_BIT17 (1 << 16)
#define MD_BIT18 (1 << 17)
#define MD_BIT19 (1 << 18)
#define MD_BIT20 (1 << 19)
#define MD_BIT21 (1 << 20)
#define MD_BIT22 (1 << 21)
#define MD_BIT23 (1 << 22)
#define MD_BIT24 (1 << 23)
#define MD_BIT25 (1 << 24)
#define MD_BIT26 (1 << 25)
#define MD_BIT27 (1 << 26)
#define MD_BIT28 (1 << 27)
#define MD_BIT29 (1 << 28)
#define MD_BIT30 (1 << 29)
#define MD_BIT31 (1 << 30)
#define MD_BIT32 (1 << 31)

#define MD_BIT33 (1ull << 32)
#define MD_BIT34 (1ull << 33)
#define MD_BIT35 (1ull << 34)
#define MD_BIT36 (1ull << 35)
#define MD_BIT37 (1ull << 36)
#define MD_BIT38 (1ull << 37)
#define MD_BIT39 (1ull << 38)
#define MD_BIT40 (1ull << 39)
#define MD_BIT41 (1ull << 40)
#define MD_BIT42 (1ull << 41)
#define MD_BIT43 (1ull << 42)
#define MD_BIT44 (1ull << 43)
#define MD_BIT45 (1ull << 44)
#define MD_BIT46 (1ull << 45)
#define MD_BIT47 (1ull << 46)
#define MD_BIT48 (1ull << 47)
#define MD_BIT49 (1ull << 48)
#define MD_BIT50 (1ull << 49)
#define MD_BIT51 (1ull << 50)
#define MD_BIT52 (1ull << 51)
#define MD_BIT53 (1ull << 52)
#define MD_BIT54 (1ull << 53)
#define MD_BIT55 (1ull << 54)
#define MD_BIT56 (1ull << 55)
#define MD_BIT57 (1ull << 56)
#define MD_BIT58 (1ull << 57)
#define MD_BIT59 (1ull << 58)
#define MD_BIT60 (1ull << 59)
#define MD_BIT61 (1ull << 60)
#define MD_BIT62 (1ull << 61)
#define MD_BIT63 (1ull << 62)
#define MD_BIT64 (1ull << 63)
