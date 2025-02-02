#ifdef MD_INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "platform.h"
#endif

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
