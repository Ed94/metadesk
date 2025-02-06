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
typedef U64 AllocatorType;
enum
{
	AllocatorType_Heap   = 0, // Genreal heap allocator
	AllocatorType_VArena = 1, // Arena allocator backed by virtual address space
	AllocatorType_FArena = 2, // Fixed arena backed back by a fixed size block of memory (usually a byte slice)
	AllocatorType_Arena  = 3, // Composite arena used originally by RAD Debugger & Metadesk
};

typedef U32 AllocatorMode;
enum AllocatorMode
{
	AllocatorMode_Alloc,
	AllocatorMode_Free,
	AllocatorMode_FreeAll,
	AllocatorMode_Resize,
	// AllocatorMode_Pop,
	// AllocatorMode_Pop_To,
	AllocatorMode_QueryType,
	AllocatorMode_QuerySupport,
};
typedef U64 AllocatorQueryFlags;
enum
{
	AllocatorQuery_Alloc   = (1 << 0),
	AllocatorQuery_Free    = (1 << 1),
	AllocatorQuery_FreeAll = (1 << 2),
	AllocatorQuery_Resize  = (1 << 3),
	// AllocatorQuery_Pop     = (1 << 3),
	// AllocatorQuery_Pop_To  = (1 << 3),
};

typedef void*(AllocatorProc)( void* allocator_data, AllocatorMode type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags );

typedef struct AllocatorInfo AllocatorInfo;
struct AllocatorInfo
{
	AllocatorProc* proc;
	void*          data;
};

// Overridable by the user by defining MD_OVERRIDE_DEFAULT_ALLOCATOR
AllocatorInfo default_allocator();

enum AllocFlag
{
	ALLOCATOR_FLAG_CLEAR_TO_ZERO = (1 << 0),
};

#ifndef MD_DEFAULT_MEMORY_ALIGNMENT
#	define MD_DEFAULT_MEMORY_ALIGNMENT ( 2 * size_of( void* ) )
#endif

#ifndef MD_DEFAULT_ALLOCATOR_FLAGS
#	define MD_DEFAULT_ALLOCATOR_FLAGS ( ALLOCATOR_FLAG_CLEAR_TO_ZERO )
#endif

// Retrieve which type of allocator
AllocatorType allocator_type(AllocatorInfo a);
// Retreive which modes the allocator supports
AllocatorQueryFlags allocator_query_support(AllocatorInfo a);

// Allocate memory with default alignment.
void* alloc( AllocatorInfo a, SSIZE size );

// Allocate memory with specified alignment.
void* alloc_align( AllocatorInfo a, SSIZE size, SSIZE alignment );

// Free allocated memory.
void alloc_free( AllocatorInfo a, void* ptr );

// void alloc_pop(AllocatorInfo a, SSIZE amt);

// void alloc_pop_to(AllocatorInfo a, SSIZE pos);

// Free all memory allocated by an allocator.
void free_all( AllocatorInfo a );

// Resize an allocated memory.
void* resize( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size );

// Resize an allocated memory with specified alignment.
void* resize_align( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size, SSIZE alignment );

#ifndef alloc_item
// Allocate memory for an item.
#define alloc_item(allocator, Type)           (Type*)memory_zero(alloc(allocator,  size_of(Type)), size_of(Type))
// Allocate memory for an item.
#define alloc_item_no_zero( allocator, Type ) (Type*)            alloc(allocator,  size_of(Type))
#endif

#ifndef alloc_array
// Allocate memory for an array of items.
#define alloc_array( allocator_, Type, count )         (Type*)memory_zero(alloc( allocator_, size_of(Type) * (count) ), size_of(Type) * (count))
#define alloc_array_no_zero( allocator_, Type, count ) (Type*)            alloc( allocator_, size_of(Type) * (count) )
#endif

// Allocate/Resize memory using default options.

// Use this if you don't need a "fancy" resize allocation
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
// The heap allocator backed by the platform vendor's malloc & free.
#define heap() (AllocatorInfo){ heap_allocator_proc, nullptr }
#endif

#ifndef md_malloc
// Helper to allocate memory using heap allocator.
#define md_malloc( sz ) alloc( heap(), sz )
#endif

#ifndef md_free
// Helper to free memory allocated by heap allocator.
#define md_free( ptr ) alloc_free( heap(), ptr )
#endif

/* Virtual Memory Arena
	This is separate from the composite arena used by HMH/Casey Muratori/RJF
	This arena stricly manages one reservation of the process's virtual address space.

	Segregating this from composite Arena style causes more moremoy to be used for "allocator headers", however it allows
	users of a library to have greater control over the allocation strategy used from their side instead of the library itself.

	Like with the composite Arena, the VArena has its struct as the header of the reserve of memory.
*/

#ifndef VARENA_DEFUALT_RESERVE
#define VARENA_DEFAULT_RESERVE MB(64)
#endif
#ifndef VARENA_DEFUALT_COMMIT
#define VARENA_DEFAULT_COMMIT  KB(64)
#endif

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
	SSIZE reserve_start;
	SSIZE reserve;
	SSIZE commit_size;
	SSIZE committed;
	SSIZE commit_used;
	VArenaFlags flags;
};

MD_API VArena* varena__alloc(VArenaParams params PARAM_DEFAULT);
#define varena_alloc(...) varena__alloc( (VArenaParams){__VA_ARGS__} )

MD_API void         varena_commit   (VArena vm, SSIZE commit_size);
MD_API VArenaParams varena_release  (VArena vm);

MD_API void* varena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);

#define varena_allocator(vm) (AllocatorInfo) { varena_allocator_proc, vm }

typedef struct ByteSlice ByteSlice;
struct ByteSlice
{
	U8*   data;
	SSIZE len;
};

#define mem_to_byteslice(data, len) (ByteSlice){ (U8*)(data), (SSIZE)(len) }

// Fixed size arena
typedef struct FArena FArena;
struct FArena
{
	ByteSlice slice;
	SSIZE     used;
};

#define farena_from_byteslice(slice)  (FArena) { slice, 0 }
#define farena_from_memory(data, len) (FArena) { mem_to_byteslice(data, len), 0 }

MD_API void* farena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags);

#define farena_allocator(arena) (AllocatorInfo){ farena_allocator_proc, & arena }

// Inlines

inline
AllocatorType allocator_type(AllocatorInfo a) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	return (AllocatorType) a.proc(a.data, AllocatorMode_QueryType, 0, 0, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS);
}

inline
AllocatorQueryFlags allocator_query_support(AllocatorInfo a) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	return (AllocatorType) a.proc(a.data, AllocatorMode_QuerySupport, 0, 0, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS);
}

inline
void* alloc_align( AllocatorInfo a, SSIZE size, SSIZE alignment ) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	return a.proc( a.data, AllocatorMode_Alloc, size, alignment, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* alloc( AllocatorInfo a, SSIZE size ) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	return alloc_align( a, size, MD_DEFAULT_MEMORY_ALIGNMENT );
}

inline
void alloc_free( AllocatorInfo a, void* ptr ) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	if ( ptr != nullptr ) {
		a.proc( a.data, AllocatorMode_Free, 0, 0, ptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
	}
}

// inline
// void alloc_pop(AllocatorInfo a, SSIZE amt) {
// 	if (a.proc == nullptr) {
// 		a = default_allocator();
// 	}
// 	a.proc(a.data, AllocatorMode_Pop, amt, 0, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS);
// }

// inline
// void alloc_pop_to(AllocatorInfo a, SSIZE pos) {
// 	if (a.proc == nullptr) {
// 		a = default_allocator();
// 	}
// 	a.proc(a.data, AllocatorMode_Pop_To, 0, 0, rcast(void*, pos), 0, 0);
// }

inline
void free_all( AllocatorInfo a ) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	a.proc( a.data, AllocatorMode_FreeAll, 0, 0, nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* resize( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size ) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	return resize_align( a, ptr, old_size, new_size, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline
void* resize_align( AllocatorInfo a, void* ptr, SSIZE old_size, SSIZE new_size, SSIZE alignment ) {
	if (a.proc == nullptr) {
		a = default_allocator();
	}
	return a.proc( a.data, AllocatorMode_Resize, new_size, alignment, ptr, old_size, MD_DEFAULT_ALLOCATOR_FLAGS );
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
