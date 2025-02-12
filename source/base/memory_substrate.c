#ifdef INTELLISENSE_DIRECTIVES
#	include "memory.h"
#	include "memory_substrate.h"
#	include "../os/os.h"
#endif

void*
md_default_resize_align( MD_AllocatorInfo a, void* old_memory, MD_SSIZE old_size, MD_SSIZE new_size, MD_SSIZE alignment )
{
	if ( ! old_memory )
		return md_alloc_align( a, new_size, alignment );

	if ( new_size == 0 )
	{
		md_alloc_free( a, old_memory );
		return md_nullptr;
	}

	if ( new_size < old_size )
		new_size = old_size;

	if ( old_size == new_size )
	{
		return old_memory;
	}
	else
	{
		void*  new_memory = md_alloc_align( a, new_size, alignment );
		if ( ! new_memory )
			return md_nullptr;

		md_mem_move( new_memory, old_memory, md_min( new_size, old_size ) );
		md_alloc_free( a, old_memory );
		return new_memory;
	}
}

#ifdef MD_HEAP_ANALYSIS
#define GEN_HEAP_STATS_MAGIC 0xDEADC0DE

typedef struct _heap_stats _heap_stats;
struct _heap_stats
{
	MD_U32   magic;
	MD_SSIZE used_memory;
	MD_SSIZE md_alloc_count;
};

md_global _heap_stats _heap_stats_info;

void
md_heap_stats_init( void )
{
	md_memory_zero_struct( &_heap_stats_info );
	_heap_stats_info.magic = GEN_HEAP_STATS_MAGIC;
}

MD_SSIZE
heap_stats_used_memory( void )
{
	md_assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call md_heap_stats_init first!" );
	return _heap_stats_info.used_memory;
}

MD_SSIZE
md_heap_stats_alloc_count( void )
{
	md_assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call md_heap_stats_init first!" );
	return _heap_stats_info.md_alloc_count;
}

void
md_heap_stats_check( void )
{
	md_assert_msg( _heap_stats_info.magic == GEN_HEAP_STATS_MAGIC, "heap_stats is not initialised yet, call md_heap_stats_init first!" );
	md_assert( _heap_stats_info.used_memory == 0 );
	md_assert( _heap_stats_info.md_alloc_count == 0 );
}

typedef struct _heap_alloc_info _heap_alloc_info;
struct _heap_alloc_info
{
	MD_SSIZE size;
	void* physical_start;
};
#endif

void*
md_heap_allocator_proc( void* allocator_data, MD_AllocatorMode mode, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags )
{
	void* ptr = md_nullptr;
	// unused( allocator_data );
	// unused( old_size );
	if ( ! alignment )
		alignment = MD_DEFAULT_MEMORY_ALIGNMENT;

#ifdef MD_HEAP_ANALYSIS
	ssize md_alloc_info_size      = size_of( _heap_alloc_info );
	ssize md_alloc_info_remainder = ( md_alloc_info_size % alignment );
	ssize track_size           = md_max( md_alloc_info_size, alignment ) + md_alloc_info_remainder;
	switch ( type )
	{
		case EAllocatorMode_FREE :
			{
				if ( ! old_memory )
					break;
				_heap_alloc_info* md_alloc_info  = md_rcast( _heap_alloc_info*, old_memory) - 1;
				_heap_stats_info.used_memory -= md_alloc_info->size;
				_heap_stats_info.md_alloc_count--;
				old_memory = md_alloc_info->physical_start;
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
	#if defined( MD_COMPILER_MSVC ) || ( defined( MD_COMPILER_GCC ) && defined( MD_OS_WINDOWS ) ) || ( defined( MD_COMPILER_TINYC ) && defined( MD_OS_WINDOWS ) )
		case MD_AllocatorMode_Alloc:
		{
			ptr = _aligned_malloc( size, alignment );
			if ( flags & MD_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
				md_memory_zero( ptr, size );
		}
		break;

		case MD_AllocatorMode_Free:
			_aligned_free( old_memory );
		break;

		case MD_AllocatorMode_Resize:
		{
			MD_AllocatorInfo a = md_heap();
			ptr                = md_default_resize_align( a, old_memory, old_size, size, alignment );
		}
		break;

	#elif defined( MD_OS_LINUX ) && ! defined( CPU_ARM ) && ! defined( MD_COMPILER_TINYC )
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
			MD_AllocatorInfo a = md_heap();
			ptr             = md_default_resize_align( a, old_memory, old_size, size, alignment );
		}
		break;
	#else
		case EAllocType_ALLOC :
		{
			posix_memalign( &ptr, alignment, size );

			if ( flags & MD_ALLOCATOR_FLAG_CLEAR_TO_ZERO )
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
			MD_AllocatorInfo a = md_heap();
			ptr             = md_default_resize_align( a, old_memory, old_size, size, alignment );
		}
		break;
	#endif

		case MD_AllocatorMode_FreeAll:
			break;

		case MD_AllocatorMode_QueryType:
			return (void*) MD_AllocatorType_Heap;

		case MD_AllocatorMode_QuerySupport:
			return (void*) (
				MD_AllocatorQuery_Alloc | MD_AllocatorQuery_Free | MD_AllocatorQuery_Resize | MD_AllocatorQuery_ResizeGrow | MD_AllocatorQuery_ResizeShrink
			);
	}

#ifdef GEN_HEAP_ANALYSIS
	if ( type == MD_AllocatorMode_Alloc )
	{
		_heap_alloc_info* md_alloc_info = md_rcast( _heap_alloc_info*, md_rcast( char*, ptr) + md_alloc_info_remainder );
		zero_item( md_alloc_info );
		md_alloc_info->size              = size - track_size;
		md_alloc_info->physical_start    = ptr;
		ptr                           = md_rcast( void*, md_alloc_info + 1 );
		_heap_stats_info.used_memory += md_alloc_info->size;
		_heap_stats_info.md_alloc_count++;
	}
#endif

	return ptr;
}

MD_VArena*
md_varena__alloc(MD_VArenaParams params)
{
	if (params.reserve_size == 0) {
		params.reserve_size = MD_VARENA_DEFAULT_RESERVE;
	}
	if (params.commit_size == 0) {
		params.commit_size = MD_VARENA_DEFAULT_COMMIT;
	}

	// rjf: round up reserve/commit sizes
	MD_U64 reserve_size = params.reserve_size;
	MD_U64 commit_size  = params.commit_size;

	void* base = md_nullptr;
	if (params.flags & MD_VArenaFlag_LargePages)
	{
		reserve_size = md_align_pow2(reserve_size, md_os_get_system_info()->large_page_size);
		commit_size  = md_align_pow2(commit_size,  md_os_get_system_info()->large_page_size);

		base = md_os_reserve_large(reserve_size);
		md_os_commit_large(base, commit_size);
		md_asan_poison_memory_region(base, params.commit_size);
	}
	else
	{
		reserve_size = md_align_pow2(reserve_size, md_os_get_system_info()->page_size);
		commit_size  = md_align_pow2(commit_size,  md_os_get_system_info()->page_size);

		    base          = md_os_reserve(reserve_size);
		MD_B32 commit_result = md_os_commit(base, commit_size);
		md_assert(commit_result == 1);
		md_asan_poison_memory_region(base, params.commit_size);
	}

	// NOTE(Ed): Panic on varena creation failure
	#if MD_OS_FEATURE_GRAPHICAL
		if(md_unlikely(base == 0))
		{
			md_os_graphical_message(1, md_str8_lit("Fatal Allocation Failure"), md_str8_lit("Unexpected memory allocation failure."));
			md_os_abort(1);
		}
	#endif

	MD_SPTR header_size = md_align_pow2(size_of(MD_VArena), MD_DEFAULT_MEMORY_ALIGNMENT);
	md_asan_unpoison_memory_region(base, header_size);

	MD_VArena* vm     = md_rcast(MD_VArena*, base);
	vm->reserve_start = md_rcast(MD_SPTR,    base) + header_size;
	vm->reserve       = reserve_size;
	vm->commit_size   = params.commit_size;
	vm->committed     = commit_size;
	vm->commit_used   = 0;
	vm->flags         = params.flags;
	return vm;
}

void
md_varena_release(MD_VArena* arena)
{
	md_os_release(arena, arena->reserve);
	arena = md_nullptr;
}

void*
md_varena_allocator_proc(void* allocator_data, MD_AllocatorMode mode, MD_SSIZE requested_size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags)
{
	MD_OS_SystemInfo const* info = md_os_get_system_info();

	MD_VArena* vm = md_rcast(MD_VArena*, allocator_data);

	void* allocated_mem = md_nullptr;
	switch (mode)
	{
		case MD_AllocatorMode_Alloc:
		{
			md_assert_msg(requested_size != 0, "requested_size is 0");

			requested_size = md_align_pow2(requested_size, alignment);

			MD_UPTR current_offset   = vm->reserve_start + vm->commit_used;
			MD_UPTR size_to_allocate = requested_size;
			MD_UPTR to_be_used       = vm->commit_used + size_to_allocate;
			MD_SPTR reserve_left     = vm->reserve - vm->committed;
			md_assert(to_be_used < reserve_left);

			MD_UPTR header_offset = vm->reserve_start - md_scast(MD_UPTR, vm);

			MD_UPTR commit_left         = vm->committed - vm->commit_used - header_offset;
			MD_B32  needs_more_commited = commit_left < size_to_allocate;
			if (needs_more_commited) 
			{
				MD_UPTR next_commit_size;
				if (vm->flags & MD_VArenaFlag_LargePages) {
					next_commit_size = reserve_left > 0 ? md_max(vm->commit_size, size_to_allocate) : md_scast(MD_UPTR, md_align_pow2( abs(reserve_left), md_os_get_system_info()->large_page_size));
				}
				else {
					next_commit_size = reserve_left > 0 ? md_max(vm->commit_size, size_to_allocate) : md_scast(MD_UPTR, md_align_pow2(abs(reserve_left), md_os_get_system_info()->page_size));
				} 	 
				if (next_commit_size) {
					void* next_commit_start = md_rcast(void*, md_rcast(MD_UPTR, vm) + vm->committed);
					MD_B32   commit_result  = md_os_commit(next_commit_start, next_commit_size);
					if (commit_result == false) {
						break;
					}
					vm->committed += next_commit_size;
				}
			}

			allocated_mem    = md_rcast(void*, current_offset);
			vm->commit_used += size_to_allocate;
		}
		break;

		case MD_AllocatorMode_Free:
		{
		}
		break;

		case MD_AllocatorMode_FreeAll:
		{
			vm->commit_used = 0;
		}
		break;

		case MD_AllocatorMode_Resize:
		{
			md_assert(old_memory != md_nullptr);
			md_assert(old_size > 0);
			md_assert_msg(old_size == requested_size, "Requested md_resize when none needed");

			requested_size = md_align_pow2(requested_size, alignment);
			old_size       = md_align_pow2(old_size, alignment);

			MD_UPTR old_memory_offset = md_scast(MD_UPTR, old_memory)        + md_scast(MD_UPTR, old_size);
			MD_UPTR current_offset    = md_scast(MD_UPTR, vm->reserve_start) + md_scast(MD_UPTR, vm->commit_used);

			md_assert_msg(old_memory_offset == current_offset, "Cannot md_resize existing allocation in MD_VArena unless it was the last allocated");

			MD_B32 requested_shrink = requested_size >= old_size;
			if (requested_shrink) {
				vm->commit_used -= md_rcast(MD_UPTR, md_align_pow2(requested_size, alignment));
				allocated_mem    = old_memory;
				break;
			}

			MD_UPTR size_to_allocate = requested_size - old_size, alignment;

			MD_UPTR header_offset       = vm->reserve_start - md_scast(MD_UPTR, vm);
			MD_UPTR commit_left         = vm->committed - vm->commit_used - header_offset;
			MD_B32  needs_more_commited = commit_left < size_to_allocate;
			if (needs_more_commited) 
			{
				MD_SPTR reserve_left = vm->reserve - vm->committed;
				MD_UPTR next_commit_size;
				if (vm->flags & MD_VArenaFlag_LargePages) {
					next_commit_size = reserve_left > 0 ? vm->commit_size : md_scast(MD_UPTR, md_align_pow2( -reserve_left, md_os_get_system_info()->large_page_size));
				}
				else {
					next_commit_size = reserve_left > 0 ? vm->commit_size : md_scast(MD_UPTR, md_align_pow2(abs(reserve_left), md_os_get_system_info()->page_size));
				}
				if (next_commit_size) {
					MD_B32 commit_result = md_os_commit(vm, next_commit_size);
					if (commit_result == false) {
						break;
					}
				}
			}

			allocated_mem    = old_memory;
			vm->commit_used += size_to_allocate;
		}
		break;

		case MD_AllocatorMode_QueryType:
		{
			return (void*) MD_AllocatorType_VArena;
		}
		break;

		case MD_AllocatorMode_QuerySupport:
		{
			return (void*) (
				MD_AllocatorQuery_Alloc | MD_AllocatorQuery_FreeAll | MD_AllocatorQuery_Resize | MD_AllocatorQuery_ResizeGrow | MD_AllocatorQuery_ResizeShrink
			);
		}
		break;
	}
	return allocated_mem;
}

void*
farena_allocator_proc(void* allocator_data, MD_AllocatorMode mode, MD_SSIZE size, MD_SSIZE alignment, void* old_memory, MD_SSIZE old_size, MD_U64 flags)
{
	MD_FArena* arena = md_rcast(MD_FArena*, allocator_data);

	void* allocated_mem = md_nullptr;
	switch (mode)
	{
		case MD_AllocatorMode_Alloc:
		{
			MD_SPTR  end        = md_scast(MD_SPTR, arena->slice.data) + arena->used;
			MD_SSIZE total_size = md_align_pow2(size, alignment);

			if (arena->used + total_size > arena->slice.len ) {
				// Out of memory
				return allocated_mem;
			}

			allocated_mem = md_scast(void*, end);
			arena->used += total_size;
		}
		break;

		case MD_AllocatorMode_Free:
		{
		}
		break;

		case MD_AllocatorMode_FreeAll:
		{
			arena->used = 0;
		}
		break;

		case MD_AllocatorMode_Resize:
		{
			md_assert(old_memory != md_nullptr);
			md_assert(old_size > 0);
			md_assert_msg(old_size == size, "Requested md_resize when none needed");

			size     = md_align_pow2(size, alignment);
			old_size = md_align_pow2(size, alignment);

			MD_SPTR old_memory_offset = md_scast(MD_SPTR, old_memory)        + old_size;
			MD_SPTR current_offset    = md_scast(MD_SPTR, arena->slice.data) + arena->used;

			md_assert_msg(old_memory_offset == current_offset, "Cannot md_resize existing allocation in MD_VArena unless it was the last allocated");

			MD_B32 requested_shrink = size >= old_size;
			if (requested_shrink) {

				arena->used  -= size;
				allocated_mem = old_memory;
				break;
			}

			allocated_mem = old_memory;
			arena->used  += size;
		}
		break;

		case MD_AllocatorMode_QueryType:
			return (void*) MD_AllocatorType_FArena;
		break;

		case MD_AllocatorMode_QuerySupport:
			return (void*) (
				MD_AllocatorQuery_Alloc | MD_AllocatorQuery_FreeAll | MD_AllocatorQuery_Resize | MD_AllocatorQuery_ResizeGrow | MD_AllocatorQuery_ResizeShrink
			);
		break;
	}
	return allocated_mem;
}
