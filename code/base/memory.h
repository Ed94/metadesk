#if MD_INTELLISENE_DIRECTIVES
#pragma once
#include "base_types.h"
#endif

#define MD_KILOBYTES( x ) (             ( x ) * ( S64 )( 1024 ) )
#define MD_MEGABYTES( x ) ( MD_KILOBYTES( x ) * ( S64 )( 1024 ) )
#define MD_GIGABYTES( x ) ( MD_MEGABYTES( x ) * ( S64 )( 1024 ) )
#define MD_TERABYTES( x ) ( MD_GIGABYTES( x ) * ( S64 )( 1024 ) )

#define MD__ONES          ( scast( GEN_NS usize, - 1) / MD_U8_MAX )
#define MD__HIGHS         ( MD__ONES * ( MD_U8_MAX / 2 + 1 ) )
#define MD__HAS_ZERO( x ) ( ( ( x ) - MD__ONES ) & ~( x ) & MD__HIGHS )

#define swap(a, b)     \
do                     \
{                      \
	typeof(a) tmp = a; \
	a = b;             \
	b = tmp;           \
}                      \
while (0)

#if MD_COMPILER_MSVC || (MD_COMPILER_CLANG && MD_OS_WINDOWS)
# pragma section(".rdata$", read)
# define read_only __declspec(allocate(".rdata$"))
#elif (MD_COMPILER_CLANG && MD_OS_LINUX)
# define read_only __attribute__((section(".rodata")))
#else
// NOTE(rjf): I don't know of a useful way to do this in GCC land.
// __attribute__((section(".rodata"))) looked promising, but it introduces a
// strange warning about malformed section attributes, and it doesn't look
// like writing to that section reliably produces access violations, strangely
// enough. (It does on Clang)
# define read_only
#endif


