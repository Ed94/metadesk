#ifdef MD_INTELLISENSE_DIRECTIVES
#pragma once
#include "base_types.h"
#include "macros.h"
#include "linkage.h"
#endif

// This provides an alterntive memory strategy to HMH/Casey Muratori/RJF styled arenas
// The library is derived from zpl-c which in-turn 
// is related to the gb headers an thus the Odin-lang memory strategy
// Users can override the underlying memory allocator used, even for the HMH arena memory strategy.

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

enum AllocType : u8
{
	EAllocType_ALLOC,
	EAllocType_FREE,
	EAllocType_FREE_ALL,
	EAllocType_RESIZE,
};

typedef void*(AllocatorProc)( void* allocator_data, AllocType type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags );

struct AllocatorInfo
{
	AllocatorProc* proc;
	void*          data;
};

enum AllocFlag
{
	ALLOCATOR_FLAG_CLEAR_TO_ZERO = 0,
};

#ifndef MD_DEFAULT_MEMORY_ALIGNMENT
#	define MD_DEFAULT_MEMORY_ALIGNMENT ( 2 * size_of( void* ) )
#endif

#ifndef MD_DEFAULT_ALLOCATOR_FLAGS
#	define MD_DEFAULT_ALLOCATOR_FLAGS ( ALLOCATOR_FLAG_CLEAR_TO_ZERO )
#endif

//! Allocate memory with default alignment.
void* alloc( AllocatorInfo a, SSIZE size );

//! Allocate memory with specified alignment.
void* alloc_align( AllocatorInfo a, SSIZE size, SSIZE alignment );

//! Free allocated memory.
void alloc_free( AllocatorInfo a, void* ptr );

//! Free all memory allocated by an allocator.
void free_all( AllocatorInfo a );

//! Resize an allocated memory.
void* resize( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size );

//! Resize an allocated memory with specified alignment.
void* resize_align( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size, SSIZE alignment );

//! Allocate memory for an item.
#define alloc_item( allocator_, Type ) ( Type* )alloc( allocator_, size_of( Type ) )

//! Allocate memory for an array of items.
#define alloc_array( allocator_, Type, count ) ( Type* )alloc( allocator_, size_of( Type ) * ( count ) )

/* heap memory analysis tools */
/* define GEN_HEAP_ANALYSIS to enable this feature */
/* call zpl_heap_stats_init at the beginning of the entry point */
/* you can call zpl_heap_stats_check near the end of the execution to validate any possible leaks */
MD_API void  heap_stats_init( void );
MD_API SSIZE heap_stats_used_memory( void );
MD_API SSIZE heap_stats_alloc_count( void );
MD_API void  heap_stats_check( void );

//! Allocate/Resize memory using default options.

//! Use this if you don't need a "fancy" resize allocation
void* default_resize_align( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size, SSIZE alignment );

MD_API void* heap_allocator_proc( void* allocator_data, AllocType type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags );

//! The heap allocator backed by operating system's memory manager.
constexpr AllocatorInfo heap( void ) { AllocatorInfo allocator = { heap_allocator_proc, nullptr }; return allocator; }

//! Helper to allocate memory using heap allocator.
#define malloc( sz ) alloc( heap(), sz )

//! Helper to free memory allocated by heap allocator.
#define mfree( ptr ) free( heap(), ptr )

