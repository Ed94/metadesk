#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "platform.h"
#	include "base_types.h"
#	include "memory.h"
#endif

// This provides an alterntive memory strategy to HMH/Casey Muratori/RJF styled arenas
// The library is derived from zpl-c which in-turn 
// is related to the gb headers an thus the Odin-lang memory strategy
// Users can override the underlying memory allocator used, even for the HMH arena memory strategy.

#define MD__ONES          ( scast( GEN_NS usize, - 1) / MD_U8_MAX )
#define MD__HIGHS         ( MD__ONES * ( MD_U8_MAX / 2 + 1 ) )
#define MD__HAS_ZERO( x ) ( ( ( x ) - MD__ONES ) & ~( x ) & MD__HIGHS )

typedef U32 AllocType;
enum AllocType enum_underlying(U32)
{
	EAllocType_ALLOC,
	EAllocType_FREE,
	EAllocType_FREE_ALL,
	EAllocType_RESIZE,
};

typedef void*(AllocatorProc)( void* allocator_data, AllocType type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags );

typedef struct AllocatorInfo;
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
#define heap() (AllocatorInfo){ AllocatorInfo allocator = { heap_allocator_proc, nullptr }; return allocator; }

//! Helper to allocate memory using heap allocator.
#define malloc( sz ) alloc( heap(), sz )

//! Helper to free memory allocated by heap allocator.
#define mfree( ptr ) free( heap(), ptr )

typedef struct VMem;
struct VMem {
	U32 cmt_size;
	U32 res_size;
	U64 base_pos;
};

AllocatorInfo vm_allocator(VMem* vm) {
	AllocatorInfo info = { vm_allocator_proc, vm }
	return info
}

MD_API void* vm_allocator_proc(void * allocator_data, AllocType type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);
