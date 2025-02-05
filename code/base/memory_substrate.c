#ifdef INTELLISENSE_DIRECTIVES
#	include "memory.h"
#	include "memory_substrate.h"
#	include "../os/os.h"
#endif

#ifdef MD_HEAP_ANALYSIS
#define GEN_HEAP_STATS_MAGIC 0xDEADC0DE

typedef struct _heap_stats _heap_stats;
struct _heap_stats
{
	U32   magic;
	SSIZE used_memory;
	SSIZE alloc_count;
};

global _heap_stats _heap_stats_info;

void heap_stats_init( void )
{
	memory_zero_struct( &_heap_stats_info );
	_heap_stats_info.magic = GEN_HEAP_STATS_MAGIC;
}

SSIZE heap_stats_used_memory( void )
{
	assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	return _heap_stats_info.used_memory;
}

SSIZE heap_stats_alloc_count( void )
{
	assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	return _heap_stats_info.alloc_count;
}

void heap_stats_check( void )
{
	assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	assert( _heap_stats_info.used_memory == 0 );
	assert( _heap_stats_info.alloc_count == 0 );
}

typedef struct _heap_alloc_info _heap_alloc_info;
struct _heap_alloc_info
{
	SSIZE size;
	void* physical_start;
};
#endif

void* heap_allocator_proc( void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags )
{
	void* ptr = nullptr;
	// unused( allocator_data );
	// unused( old_size );
	if ( ! alignment )
		alignment = MD_DEFAULT_MEMORY_ALIGNMENT;

#ifdef MD_HEAP_ANALYSIS
	ssize alloc_info_size      = size_of( _heap_alloc_info );
	ssize alloc_info_remainder = ( alloc_info_size % alignment );
	ssize track_size           = max( alloc_info_size, alignment ) + alloc_info_remainder;
	switch ( type )
	{
		case EAllocatorMode_FREE :
			{
				if ( ! old_memory )
					break;
				_heap_alloc_info* alloc_info  = rcast( _heap_alloc_info*, old_memory) - 1;
				_heap_stats_info.used_memory -= alloc_info->size;
				_heap_stats_info.alloc_count--;
				old_memory = alloc_info->physical_start;
			}
			break;
		case EAllocatorMode_ALLOC :
			{
				size += track_size;
			}
			break;
		default :
			break;
	}
#endif

	switch ( mode )
	{
	#if defined( COMPILER_MSVC ) || ( defined( COMPILER_GCC ) && defined( OS_WINDOWS ) ) || ( defined( COMPILER_TINYC ) && defined( OS_WINDOWS ) )
		case AllocatorMode_Alloc:
		{
			ptr = _aligned_malloc( size, alignment );
			if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				zero_size( ptr, size );
		}
		break;

		case AllocatorMode_Free:
			_aligned_free( old_memory );
		break;

		case AllocatorMode_Reisze:
		{
			AllocatorInfo a = heap();
			ptr             = default_resize_align( a, old_memory, old_size, size, alignment );
		}
		break;

	#elif defined( OS_LINUX ) && ! defined( CPU_ARM ) && ! defined( COMPILER_TINYC )
		case EAllocation_ALLOC :
		{
			ptr = aligned_alloc( alignment, ( size + alignment - 1 ) & ~( alignment - 1 ) );

			if ( flags & GEN_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
			{
				zero_size( ptr, size );
			}
		}
		break;

		case EAllocation_Freee :
		{
			free( old_memory );
		}
		break;

		case EAllocation_RESIZE :
		{
			AllocatorInfo a = heap();
			ptr             = default_resize_align( a, old_memory, old_size, size, alignment );
		}
		break;
	#else
		case EAllocType_ALLOC :
		{
			posix_memalign( &ptr, alignment, size );

			if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
			{
				zero_size( ptr, size );
			}
		}
		break;

		case EAllocType_FREE :
		{
			free( old_memory );
		}
		break;

		case EAllocType_RESIZE :
		{
			AllocatorInfo a = heap();
			ptr             = default_resize_align( a, old_memory, old_size, size, alignment );
		}
		break;
	#endif

		case AllocatorMode_FreeAll:
			break;

		case AllocatorMode_QueryType:
			return (void*) AllocatorType_Heap;

		case AllocatorMode_QuerySupport:
			return (void*) (
				AllocatorQuery_Alloc | AllocatorQuery_Free | AllocatorQuery_Reisze
			);
	}

#ifdef GEN_HEAP_ANALYSIS
	if ( type == AllocatorMode_Alloc )
	{
		_heap_alloc_info* alloc_info = rcast( _heap_alloc_info*, rcast( char*, ptr) + alloc_info_remainder );
		zero_item( alloc_info );
		alloc_info->size              = size - track_size;
		alloc_info->physical_start    = ptr;
		ptr                           = rcast( void*, alloc_info + 1 );
		_heap_stats_info.used_memory += alloc_info->size;
		_heap_stats_info.alloc_count++;
	}
#endif

	return ptr;
}

VArena varena__alloc(VArenaParams params)
{
	// rjf: round up reserve/commit sizes
	U64 reserve_size = params.reserve_size;
	U64 commit_size  = params.commit_size;

	void* base = nullptr;
	if (params.flags & VArenaFlag_LargePages)
	{
		reserve_size = align_pow2(reserve_size, os_get_system_info()->large_page_size);
		commit_size  = align_pow2(commit_size,  os_get_system_info()->large_page_size);

		base = os_reserve_large(reserve_size);
		os_commit_large(base, commit_size);
		asan_poison_memory_region(base, params.commit_size);
	}
	else
	{
		reserve_size = align_pow2(reserve_size, os_get_system_info()->page_size);
		commit_size  = align_pow2(commit_size,  os_get_system_info()->page_size);

		base = os_reserve(reserve_size);
		os_commit(base, commit_size);
		asan_poison_memory_region(base, params.commit_size);
	}

	// NOTE(Ed): Panic on varena creation failure
	#if OS_FEATURE_GRAPHICAL
		if(unlikely(base == 0))
		{
			os_graphical_message(1, str8_lit("Fatal Allocation Failure"), str8_lit("Unexpected memory allocation failure."));
			os_abort(1);
		}
	#endif

	SPTR header_size = size_of(VArena);

	VArena* vm = (VArena* ) base;
	vm->reserve       = reserve_size;
	vm->committed     = commit_size;
	vm->reserve_start = (SPTR)base + header_size;
	vm->flags         = params.flags;
	vm->commit_used   = 0;
	asan_unpoison_memory_region(vm, header_size);
}

void Varena_release(VArena* arena)
{


	arena = nullptr;
}

void* varena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags)
{
	void* result = nullptr;

	switch (mode)
	{
		case AllocatorMode_Alloc:

		break;

		case AllocatorMode_Free:
		break;

		case AllocatorMode_FreeAll:
		break;

		case AllocatorMode_Reisze:
		break;

		case AllocatorMode_QueryType:
		break;

		case AllocatorMode_QuerySupport:
		break;
	}



	return result;
}

