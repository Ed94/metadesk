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

#ifndef MD__ONES
#define MD__ONES          ( scast( GEN_NS usize, - 1) / MD_U8_MAX )
#define MD__HIGHS         ( MD__ONES * ( MD_U8_MAX / 2 + 1 ) )
#define MD__HAS_ZERO( x ) ( ( ( x ) - MD__ONES ) & ~( x ) & MD__HIGHS )
#endif

// Return value of allocator_type
typedef U32 AllocatorType;
enum
{
	AllocatorType_Heap   = 0, // Genreal heap allocator
	AllocatorType_VArena = 1, // Arena allocator backed by virtual address space
	AllocatorType_FArena = 2, // Fixed arena backed back by a fixed size block of memory (usually a byte slice)
	AllocatorType_Arena  = 3, // Composite arena used originally by RAD Debugger & Metadesk
};

typedef U32 AllocatorMode;
enum AllocatorMode enum_underlying(U32)
{
	AllocatorMode_ALLOC,
	AllocatorMode_FREE,
	AllocatorMode_FREE_ALL,
	AllocatorMode_RESIZE,
	AllocatorMode_QueryType,
};

typedef void*(AllocatorProc)( void* allocator_data, AllocatorMode type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags );

typedef struct AllocatorInfo AllocatorInfo;
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

// Allows the user to retrieve which type of allocator is being used.
AllocatorType allocator_type(AllocatorInfo a);

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

#ifndef alloc_item
//! Allocate memory for an item.
#define alloc_item( allocator_, Type ) ( Type* )alloc( allocator_, size_of( Type ) )
#endif

#ifndef alloc_array
//! Allocate memory for an array of items.
#define alloc_array( allocator_, Type, count ) ( Type* )alloc( allocator_, size_of( Type ) * ( count ) )
#endif

//! Allocate/Resize memory using default options.

//! Use this if you don't need a "fancy" resize allocation
void* default_resize_align( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size, SSIZE alignment );

#ifdef MD_HEAP_ANALYSIS
/* heap memory analysis tools */
/* define GEN_HEAP_ANALYSIS to enable this feature */
/* call zpl_heap_stats_init at the beginning of the entry point */
/* you can call zpl_heap_stats_check near the end of the execution to validate any possible leaks */
MD_API void  heap_stats_init( void );
MD_API SSIZE heap_stats_used_memory( void );
MD_API SSIZE heap_stats_alloc_count( void );
MD_API void  heap_stats_check( void );
#endif

MD_API void* heap_allocator_proc( void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags );

#ifndef heap
//! The heap allocator backed by operating system's memory manager.
#define heap() (AllocatorInfo){ AllocatorInfo allocator = { heap_allocator_proc, nullptr }; return allocator; }
#endif

#ifndef malloc
//! Helper to allocate memory using heap allocator.
#define malloc( sz ) alloc( heap(), sz )
#endif

#ifndef mfree
//! Helper to free memory allocated by heap allocator.
#define mfree( ptr ) free( heap(), ptr )
#endif

/* Virtual Memory Arena
	This is separate from the composite arena used by HMH/Casey Muratori/RJF
	This arena stricly manages one reservation of the process's virtual address space.

	Segregating this from composite Arena style causes more moremoy to be used for "allocator headers", however it allows
	users of a library to have greater control over the allocation strategy used from their side instead of the library itself.

	Like with the composite Arena, the VArena has its struct as the header of the reserve of memory.
*/

typedef U32 VArenaFlags;
enum
{
	VArenaFlag_LargePages = (1 << 0),
};

typedef struct VArenaParams VArenaParams;
struct VArenaParams
{
	U64         base_addr;
	VArenaFlags flags;
	U64         reserve_size;
	U64         commit_size;
};

typedef struct VArena VArena;
struct VArena
{
	VArenaFlags flags;
	U64 base_pos;
	U64 cmt;
	U64 res;
	U64 res_size;
	U64 cmt_size;
};

AllocatorInfo vm_allocator(VArena* vm) {
	AllocatorInfo info = { vm_allocator_proc, vm };
	return info;
}

VArena*      varena_alloc    (VArenaParams params);
void         varena_commit   (VArena vm, SSIZE commit_size);
VArenaParams varena_free     (VArena vm);
SSIZE        varena_page_size(SSIZE* alignment_out);

MD_API void* varena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);

typedef struct ByteSlice ByteSlice;
struct ByteSlice
{
	U8*   data;
	SSIZE len;
};

// Fixed size arena
typedef struct FArena FArena;
struct FArena
{
	ByteSlice slice;
	SSIZE     pos;
};

FArena farena_from_byteslice(ByteSlice slice);

MD_API void* farena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);

// Inlines

inline
AllocatorType allocator_type(AllocatorInfo a) {
	return (AllocatorType) a.proc(a.data, AllocatorMode_QueryType, 0, 0, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS);
}

inline
void* alloc_align( AllocatorInfo a, SSIZE size, SSIZE alignment ) {
	return a.proc( a.data, AllocatorMode_ALLOC, size, alignment, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* alloc( AllocatorInfo a, SSIZE size ) {
	return alloc_align( a, size, MD_DEFAULT_MEMORY_ALIGNMENT );
}

inline
void allocator_free( AllocatorInfo a, void* ptr ) {
	if ( ptr != nullptr )
		a.proc( a.data, AllocatorMode_FREE, 0, 0, ptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void free_all( AllocatorInfo a ) {
	a.proc( a.data, AllocatorMode_FREE_ALL, 0, 0, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* resize( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size ) {
	return resize_align( a, ptr, old_size, new_size, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* resize_align( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size, SSIZE alignment ) {
	return a.proc( a.data, AllocatorMode_RESIZE, new_size, alignment, ptr, old_size, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* default_resize_align( AllocatorInfo a, void* old_memory, SSIZE old_size, SSIZE new_size, SSIZE alignment )
{
	if ( ! old_memory )
		return alloc_align( a, new_size, alignment );

	if ( new_size == 0 )
	{
		allocator_free( a, old_memory );
		return nullptr;
	}

	if ( new_size < old_size )
		new_size = old_size;

	if ( old_size == new_size )
	{
		return old_memory;
	}
	else
	{
		void*  new_memory = alloc_align( a, new_size, alignment );
		if ( ! new_memory )
			return nullptr;

		mem_move( new_memory, old_memory, min( new_size, old_size ) );
		allocator_free( a, old_memory );
		return new_memory;
	}
}
