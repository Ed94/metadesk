#ifdef INTELLISENSE_DIRECTIVES
#	include "memory.h"
#	include "memory_substrate.h"
#	include "../os/os.h"
#endif

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

void* heap_allocator_proc( void* allocator_data, AllocType type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags )
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
		case EAllocation_FREE :
			{
				if ( ! old_memory )
					break;
				_heap_alloc_info* alloc_info  = rcast( _heap_alloc_info*, old_memory) - 1;
				_heap_stats_info.used_memory -= alloc_info->size;
				_heap_stats_info.alloc_count--;
				old_memory = alloc_info->physical_start;
			}
			break;
		case EAllocation_ALLOC :
			{
				size += track_size;
			}
			break;
		default :
			break;
	}
#endif

	switch ( type )
	{
#if defined( COMPILER_MSVC ) || ( defined( COMPILER_GCC ) && defined( OS_WINDOWS ) ) || ( defined( COMPILER_TINYC ) && defined( OS_WINDOWS ) )
		case EAllocType_ALLOC :
			ptr = _aligned_malloc( size, alignment );
			if ( flags & ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				zero_size( ptr, size );
			break;
		case EAllocType_FREE :
			_aligned_free( old_memory );
			break;
		case EAllocType_RESIZE :
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

		case EAllocation_FREE :
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

		case EAllocType_FREE_ALL :
			break;
	}

#ifdef GEN_HEAP_ANALYSIS
	if ( type == EAllocation_ALLOC )
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

void* vm_allocator_proc(void* allocator_data, AllocType type, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags)
{

}

