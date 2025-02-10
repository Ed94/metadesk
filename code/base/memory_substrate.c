#ifdef INTELLISENSE_DIRECTIVES
#	include "memory.h"
#	include "memory_substrate.h"
#	include "../os/os.h"
#endif

void*
default_resize_align( AllocatorInfo a, void* old_memory, SSIZE old_size, SSIZE new_size, SSIZE alignment )
{
	if ( ! old_memory )
		return alloc_align( a, new_size, alignment );

	if ( new_size == 0 )
	{
		alloc_free( a, old_memory );
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

		mem_move( new_memory, old_memory, md_min( new_size, old_size ) );
		alloc_free( a, old_memory );
		return new_memory;
	}
}

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

void
heap_stats_init( void )
{
	memory_zero_struct( &_heap_stats_info );
	_heap_stats_info.magic = GEN_HEAP_STATS_MAGIC;
}

SSIZE
heap_stats_used_memory( void )
{
	assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	return _heap_stats_info.used_memory;
}

SSIZE
heap_stats_alloc_count( void )
{
	assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call heap_stats_init first!" );
	return _heap_stats_info.alloc_count;
}

void
heap_stats_check( void )
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

void*
heap_allocator_proc( void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags )
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
				memory_zero( ptr, size );
		}
		break;

		case AllocatorMode_Free:
			_aligned_free( old_memory );
		break;

		case AllocatorMode_Resize:
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
				AllocatorQuery_Alloc | AllocatorQuery_Free | AllocatorQuery_Resize | AllocatorQuery_ResizeGrow | AllocatorQuery_ResizeShrink
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

VArena*
varena__alloc(VArenaParams params)
{
	if (params.reserve_size == 0) {
		params.reserve_size = VARENA_DEFAULT_RESERVE;
	}
	if (params.commit_size == 0) {
		params.commit_size = VARENA_DEFAULT_COMMIT;
	}

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

	SPTR header_size = align_pow2(size_of(VArena), MD_DEFAULT_MEMORY_ALIGNMENT);
	asan_unpoison_memory_region(base, header_size);

	VArena* vm        = rcast(VArena*, base);
	vm->reserve_start = rcast(SPTR,    base) + header_size;
	vm->reserve       = reserve_size;
	vm->commit_size   = params.commit_size;
	vm->committed     = commit_size;
	vm->commit_used   = 0;
	vm->flags         = params.flags;
	return vm;
}

void
varena_release(VArena* arena)
{
	os_release(arena, arena->reserve);
	arena = nullptr;
}

void*
varena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE requested_size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags)
{
	OS_SystemInfo const* info = os_get_system_info();

	VArena* vm = rcast(VArena*, allocator_data);

	void* allocated_mem = nullptr;
	switch (mode)
	{
		case AllocatorMode_Alloc:
		{
			assert(requested_size != 0);

			requested_size = align_pow2(requested_size, alignment);

			UPTR current_offset   = vm->reserve_start + vm->commit_used;
			UPTR size_to_allocate = requested_size;
			UPTR to_be_used       = vm->commit_used + size_to_allocate;
			assert(to_be_used < vm->reserve);

			UPTR header_offset = vm->reserve_start - scast(UPTR, vm);

			UPTR commit_left         = vm->committed - vm->commit_used - header_offset;
			B32  needs_more_commited = commit_left < size_to_allocate;
			if (needs_more_commited) 
			{
				SPTR reserve_left     = vm->reserve - vm->committed;
				UPTR next_commit_size;
				if (vm->flags & VArenaFlag_LargePages) {
					next_commit_size = reserve_left > 0 ? md_max(vm->commit_size, size_to_allocate) : scast(UPTR, align_pow2( -reserve_left, os_get_system_info()->large_page_size));
				}
				else {
					next_commit_size = reserve_left > 0 ? md_max(vm->commit_size, size_to_allocate) : scast(UPTR, align_pow2(abs(reserve_left), os_get_system_info()->page_size));
				} 	 
				if (next_commit_size) {
					B32 commit_result = os_commit(vm, next_commit_size);
					if (commit_result == false) {
						break;
					}
				}
			}

			allocated_mem    = rcast(void*, current_offset);
			vm->commit_used += size_to_allocate;
		}
		break;

		case AllocatorMode_Free:
		{
		}
		break;

		case AllocatorMode_FreeAll:
		{
			vm->commit_used = 0;
		}
		break;

		case AllocatorMode_Resize:
		{
			assert(old_memory != nullptr);
			assert(old_size > 0);
			assert_msg(old_size == requested_size, "Requested resize when none needed");

			requested_size = align_pow2(requested_size, alignment);
			old_size       = align_pow2(old_size, alignment);

			UPTR old_memory_offset = scast(UPTR, old_memory)        + scast(UPTR, old_size);
			UPTR current_offset    = scast(UPTR, vm->reserve_start) + scast(UPTR, vm->commit_used);

			assert_msg(old_memory_offset == current_offset, "Cannot resize existing allocation in VArena unless it was the last allocated");

			B32 requested_shrink = requested_size >= old_size;
			if (requested_shrink) {
				vm->commit_used -= rcast(UPTR, align_pow2(requested_size, alignment));
				allocated_mem    = old_memory;
				break;
			}

			UPTR size_to_allocate = requested_size - old_size, alignment;

			UPTR header_offset       = vm->reserve_start - scast(UPTR, vm);
			UPTR commit_left         = vm->committed - vm->commit_used - header_offset;
			B32  needs_more_commited = commit_left < size_to_allocate;
			if (needs_more_commited) 
			{
				SPTR reserve_left     = vm->reserve - vm->committed;
				UPTR next_commit_size;
				if (vm->flags & VArenaFlag_LargePages) {
					next_commit_size = reserve_left > 0 ? vm->commit_size : scast(UPTR, align_pow2( -reserve_left, os_get_system_info()->large_page_size));
				}
				else {
					next_commit_size = reserve_left > 0 ? vm->commit_size : scast(UPTR, align_pow2(abs(reserve_left), os_get_system_info()->page_size));
				}
				if (next_commit_size) {
					B32 commit_result = os_commit(vm, next_commit_size);
					if (commit_result == false) {
						break;
					}
				}
			}

			allocated_mem    = old_memory;
			vm->commit_used += size_to_allocate;
		}
		break;

		// case AllocatorMode_Pop:
		// break;
		// case AllocatorMode_Pop_To:
		// break;

		case AllocatorMode_QueryType:
		{
			return (void*) AllocatorType_VArena;
		}
		break;

		case AllocatorMode_QuerySupport:
		{
			return (void*) (
				AllocatorQuery_Alloc | AllocatorQuery_FreeAll | AllocatorQuery_Resize | AllocatorQuery_ResizeGrow | AllocatorQuery_ResizeShrink
			);
		}
		break;
	}
	return allocated_mem;
}

void*
farena_allocator_proc(void* allocator_data, AllocatorMode mode, SSIZE size, SSIZE alignment, void* old_memory, SSIZE old_size, U64 flags)
{
	FArena* arena = rcast(FArena*, allocator_data);

	void* allocated_mem = nullptr;
	switch (mode)
	{
		case AllocatorMode_Alloc:
		{
			SPTR  end        = scast(SPTR, arena->slice.data) + arena->used;
			SSIZE total_size = align_pow2(size, alignment);

			if (arena->used + total_size > arena->slice.len ) {
				// Out of memory
				return allocated_mem;
			}

			allocated_mem = scast(void*, end);
			arena->used  += total_size;
		}
		break;

		case AllocatorMode_Free:
		{
		}
		break;

		case AllocatorMode_FreeAll:
		{
			arena->used = 0;
		}
		break;

		case AllocatorMode_Resize:
		{
			assert(old_memory != nullptr);
			assert(old_size > 0);
			assert_msg(old_size == size, "Requested resize when none needed");

			size     = align_pow2(size, alignment);
			old_size = align_pow2(size, alignment);

			SPTR old_memory_offset = scast(SPTR, old_memory)        + old_size;
			SPTR current_offset    = scast(SPTR, arena->slice.data) + arena->used;

			assert_msg(old_memory_offset == current_offset, "Cannot resize existing allocation in VArena unless it was the last allocated");

			B32 requested_shrink = size >= old_size;
			if (requested_shrink) {

				arena->used    -= size;
				allocated_mem   = old_memory;
				break;
			}

			allocated_mem = old_memory;
			arena->used  += size;
		}
		break;

		// case AllocatorMode_Pop:
		// break;
		// case AllocatorMode_Pop_To:
		// break;

		case AllocatorMode_QueryType:
			return (void*) AllocatorType_FArena;
		break;

		case AllocatorMode_QuerySupport:
			return (void*) (AllocatorQuery_Alloc | AllocatorQuery_FreeAll | AllocatorQuery_Resize 
				// | AllocatorQuery_Pop | AllocatorQuery_Pop_To
			);
		break;
	}
	return allocated_mem;
}
