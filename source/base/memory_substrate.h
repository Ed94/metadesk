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
#define MD__ONES          ( md_scast( GEN_NS usize, - 1) / MD_U8_MAX )
#define MD__HIGHS         ( MD__ONES * ( MD_U8_MAX / 2 + 1 ) )
#define MD__HAS_ZERO( x ) ( ( ( x ) - MD__ONES ) & ~( x ) & MD__HIGHS )
#endif

// Return value of md_allocator_type
typedef MD_U64 MD_AllocatorType;
enum
{
	MD_AllocatorType_Heap   = 0, // Genreal md_heap allocator
	MD_AllocatorType_VArena = 1, // MD_Arena allocator backed by virtual address space
	MD_AllocatorType_FArena = 2, // Fixed arena backed back by a fixed size block of memory (usually a byte slice)
	MD_AllocatorType_Arena  = 3, // Composite arena used originally by RAD Debugger & Metadesk
};

typedef MD_U32 MD_AllocatorMode;
enum MD_AllocatorMode
{
	MD_AllocatorMode_Alloc,
	MD_AllocatorMode_Free,
	MD_AllocatorMode_FreeAll,
	MD_AllocatorMode_Resize,
	MD_AllocatorMode_QueryType,
	MD_AllocatorMode_QuerySupport,
};
typedef MD_U64 MD_AllocatorQueryFlags;
enum
{
	MD_AllocatorQuery_Alloc         = (1 << 0),
	MD_AllocatorQuery_Free          = (1 << 1),
	MD_AllocatorQuery_FreeAll       = (1 << 2),
	MD_AllocatorQuery_Resize        = (1 << 3), // Supports both grow and shrink
	MD_AllocatorQuery_ResizeShrink  = (1 << 4),
	MD_AllocatorQuery_ResizeGrow    = (1 << 5),
};

typedef void*(MD_AllocatorProc)( void* allocator_data, MD_AllocatorMode type, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags );

typedef struct MD_AllocatorInfo MD_AllocatorInfo;
struct MD_AllocatorInfo
{
	MD_AllocatorProc* proc;
	void*          data;
};

// Overridable by the user by defining MD_OVERRIDE_DEFAULT_ALLOCATOR
MD_AllocatorInfo md_default_allocator();

enum AllocFlag
{
	MD_ALLOCATOR_FLAG_CLEAR_TO_ZERO = (1 << 0),
};

#ifndef MD_DEFAULT_MEMORY_ALIGNMENT
#	define MD_DEFAULT_MEMORY_ALIGNMENT ( 2 * size_of( void* ) )
#endif

#ifndef MD_DEFAULT_ALLOCATOR_FLAGS
#	define MD_DEFAULT_ALLOCATOR_FLAGS ( MD_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
#endif

// Retrieve which type of allocator
MD_AllocatorType md_allocator_type(MD_AllocatorInfo a);
// Retreive which modes the allocator supports
MD_AllocatorQueryFlags md_allocator_query_support(MD_AllocatorInfo a);

// Allocate memory with default alignment.
void* md_alloc( MD_AllocatorInfo a, MD_SSIZE size );

// Allocate memory with specified alignment.
void* md_alloc_align( MD_AllocatorInfo a, MD_SSIZE size, MD_SSIZE alignment );

// Free allocated memory.
void md_alloc_free( MD_AllocatorInfo a, void* ptr );

// Free all memory allocated by an allocator.
void md_free_all( MD_AllocatorInfo a );

// Resize an allocated memory.
void* md_resize( MD_AllocatorInfo a, void* ptr, MD_SSIZE old_size, MD_SSIZE new_size );

// Resize an allocated memory with specified alignment.
void* md_resize_align( MD_AllocatorInfo a, void* ptr, MD_SSIZE old_size, MD_SSIZE new_size, MD_SSIZE alignment );

#ifndef md_alloc_item
// Allocate memory for an item.
#define md_alloc_item(allocator, Type)           (Type*)md_memory_zero(md_alloc(allocator,  size_of(Type)), size_of(Type))
// Allocate memory for an item.
#define md_alloc_item_no_zero( allocator, Type ) (Type*)               md_alloc(allocator,  size_of(Type))
#endif

#ifndef md_alloc_array
// Allocate memory for an array of items.
#define md_alloc_array( allocator_, Type, count )         (Type*)md_memory_zero(md_alloc( allocator_, size_of(Type) * (count) ), size_of(Type) * (count))
// Allocate memory for an array of items. (Don't zero initialize)
#define md_alloc_array_no_zero( allocator_, Type, count ) (Type*)               md_alloc( allocator_, size_of(Type) * (count) )
#endif

// Allocate/Resize memory using default options.

// Use this if you don't need a "fancy" md_resize allocation
void* md_default_resize_align( MD_AllocatorInfo a, void* ptr, MD_SSIZE old_size, MD_SSIZE new_size, MD_SSIZE alignment );

#ifdef MD_HEAP_ANALYSIS
/* md_heap memory analysis tools */
/* define GEN_HEAP_ANALYSIS to enable this feature */
/* call zpl_heap_stats_init at the beginning of the entry point */
/* you can call zpl_heap_stats_check near the end of the execution to validate any possible leaks */
MD_API void  md_heap_stats_init( void );
MD_API MD_SSIZE heap_stats_used_memory( void );
MD_API MD_SSIZE md_heap_stats_alloc_count( void );
MD_API void  md_heap_stats_check( void );
#endif

MD_API void* md_heap_allocator_proc( void* allocator_data, MD_AllocatorMode mode, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags );

#ifndef md_heap
// The md_heap allocator backed by the platform vendor's md_malloc & free.
#define md_heap() (MD_AllocatorInfo){ md_heap_allocator_proc, md_nullptr }
#endif

#ifndef md_malloc
// Helper to allocate memory using md_heap allocator.
#define md_malloc( sz ) md_alloc( md_heap(), sz )
#endif

#ifndef md_free
// Helper to free memory allocated by md_heap allocator.
#define md_free( ptr ) md_alloc_free( md_heap(), ptr )
#endif

/* Virtual Memory MD_Arena
	This is separate from the composite arena used by HMH/Casey Muratori/RJF
	This arena stricly manages one reservation of the process's virtual address space.

	Segregating this from composite MD_Arena style causes more moremoy to be used for "allocator headers", however it allows
	users of a library to have greater control over the allocation strategy used from their side instead of the library itself.

	Like with the composite MD_Arena, the MD_VArena has its struct as the header of the reserve of memory.
*/

#ifndef MD_VARENA_DEFUALT_RESERVE
#define MD_VARENA_DEFAULT_RESERVE MD_MB(64)
#endif
#ifndef MD_VARENA_DEFUALT_COMMIT
#define MD_VARENA_DEFAULT_COMMIT  MD_KB(64)
#endif

typedef MD_U32 MD_VArenaFlags;
enum
{
	MD_VArenaFlag_LargePages = (1 << 0),
};

typedef struct MD_VArenaParams MD_VArenaParams;
struct MD_VArenaParams
{
	MD_U64         base_addr;
	MD_VArenaFlags flags;
	MD_U64         reserve_size;
	MD_U64         commit_size;
};

typedef struct MD_VArena MD_VArena;
struct MD_VArena
{
	MD_SSIZE reserve_start;
	MD_SSIZE reserve;
	MD_SSIZE commit_size;
	MD_SSIZE committed;
	MD_SSIZE commit_used;
	MD_VArenaFlags flags;
};

MD_API MD_VArena* md_varena__alloc(MD_VArenaParams params MD_PARAM_DEFAULT);
#define md_varena_alloc(...) md_varena__alloc( (MD_VArenaParams){__VA_ARGS__} )

MD_API void md_varena_commit (MD_VArena* vm, MD_SSIZE commit_size);
MD_API void md_varena_release(MD_VArena* vm);

md_force_inline void varena_rewind(MD_VArena* vm, MD_SSIZE pos) { vm->commit_used = pos; }

MD_API void* md_varena_allocator_proc(void* allocator_data, MD_AllocatorMode mode, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags);

#define md_varena_allocator(vm) (MD_AllocatorInfo) { md_varena_allocator_proc, vm }

typedef struct MD_ByteSlice MD_ByteSlice;
struct MD_ByteSlice
{
	MD_U8*   data;
	MD_SSIZE len;
};

#define md_mem_to_byteslice(data, len) (MD_ByteSlice){ (MD_U8*)(data), (MD_SSIZE)(len) }

// Fixed size arena
typedef struct MD_FArena MD_FArena;
struct MD_FArena
{
	MD_ByteSlice slice;
	MD_SSIZE     used;
};

#define md_farena_from_byteslice(slice)  (MD_FArena) { slice, 0 }
#define md_farena_from_memory(data, len) (MD_FArena) { md_mem_to_byteslice(data, len), 0 }

MD_API void* farena_allocator_proc(void* allocator_data, MD_AllocatorMode mode, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags);

#define farena_allocator(arena) (MD_AllocatorInfo){ farena_allocator_proc, & arena }

// Inlines

inline MD_AllocatorType
md_allocator_type(MD_AllocatorInfo a) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	return (MD_AllocatorType) a.proc(a.data, MD_AllocatorMode_QueryType, 0, 0, md_nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS);
}

inline MD_AllocatorQueryFlags
md_allocator_query_support(MD_AllocatorInfo a) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	return (MD_AllocatorType) a.proc(a.data, MD_AllocatorMode_QuerySupport, 0, 0, md_nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS);
}

inline void* 
md_alloc_align( MD_AllocatorInfo a, MD_SSIZE size, MD_SSIZE alignment ) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	return a.proc( a.data, MD_AllocatorMode_Alloc, size, alignment, md_nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline void*
md_alloc( MD_AllocatorInfo a, MD_SSIZE size ) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	return md_alloc_align( a, size, MD_DEFAULT_MEMORY_ALIGNMENT );
}

inline void
md_alloc_free( MD_AllocatorInfo a, void* ptr ) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	if ( ptr != md_nullptr ) {
		a.proc( a.data, MD_AllocatorMode_Free, 0, 0, ptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
	}
}

inline void
md_free_all( MD_AllocatorInfo a ) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	a.proc( a.data, MD_AllocatorMode_FreeAll, 0, 0, md_nullptr, 0, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline void*
md_resize( MD_AllocatorInfo a, void* ptr, MD_SSIZE old_size, MD_SSIZE new_size ) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	return md_resize_align( a, ptr, old_size, new_size, MD_DEFAULT_ALLOCATOR_FLAGS );
}

inline void*
md_resize_align( MD_AllocatorInfo a, void* ptr, MD_SSIZE old_size, MD_SSIZE new_size, MD_SSIZE alignment ) {
	if (a.proc == md_nullptr) {
		a = md_default_allocator();
	}
	return a.proc( a.data, MD_AllocatorMode_Resize, new_size, alignment, ptr, old_size, MD_DEFAULT_ALLOCATOR_FLAGS );
}
