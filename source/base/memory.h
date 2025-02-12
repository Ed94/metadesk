#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "platform.h"
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Units

#ifndef KILOBTYES
#define MD_KILOBYTES( x ) (             ( x ) * ( MD_S64 )( 1024 ) )
#endif
#ifndef MD_MEGABYTES
#define MD_MEGABYTES( x ) ( MD_KILOBYTES( x ) * ( MD_S64 )( 1024 ) )
#endif
#ifndef MD_GIGABYTES
#define MD_GIGABYTES( x ) ( MD_MEGABYTES( x ) * ( MD_S64 )( 1024 ) )
#endif
#ifndef MD_TERABYTES
#define MD_TERABYTES( x ) ( MD_GIGABYTES( x ) * ( MD_S64 )( 1024 ) )
#endif

#ifndef MD_KB
#define MD_KB(n)  (((MD_U64)(n)) << 10)
#endif
#ifndef MD_MB
#define MD_MB(n)  (((MD_U64)(n)) << 20)
#endif
#ifndef MD_GB
#define MD_GB(n)  (((MD_U64)(n)) << 30)
#endif
#ifndef MD_TB
#define MD_TB(n)  (((MD_U64)(n)) << 40)
#endif

#ifndef md_thousand
#define thousand(n)   ((n) * 1000)
#endif
#ifndef md_million
#define md_million(n)    ((n) * 1000000)
#endif
#ifndef md_billion
#define md_billion(n)    ((n) * 1000000000)
#endif

////////////////////////////////
//~ rjf: Type -> Alignment

#ifndef md_align_of
#	if MD_COMPILER_MSVC
#		define md_align_of(T) __alignof(T)
#	elif MD_COMPILER_CLANG
#		define md_align_of(T) __alignof(T)
#	elif MD_COMPILER_GCC
#		define md_align_of(T) __alignof__(T)
#	else
#		error AlignOf not defined for this compiler.
#	endif
#endif

////////////////////////////////
//~ rjf: Member Offsets

#ifndef md_member
#define md_member(T, m)                    ( ((T*) 0)->m )
#endif
#ifndef md_offset_of
#define md_offset_of(T, m)                 md_int_from_ptr(& md_member(T, m))
#endif
#ifndef md_member_from_offset
#define md_member_from_offset(T, ptr, off) (T)  ((((MD_U8 *) ptr) + (off)))
#endif
#ifndef md_cast_from_member
#define md_cast_from_member(T, m, ptr)     (T*) (((MD_U8*)ptr) - md_offset_of(T, m))
#endif

////////////////////////////////
//~ rjf: Memory Operation Macros


// TODO(Ed): Review usage of memmove here...(I guess wanting to avoid overlap faults..)
#ifndef md_memory_copy
#	if MD_OS_WINDOWS
		void* memcpy_intrinsic(void* dest, const void* src, size_t count) 
		{
			if (dest == NULL || src == NULL || count == 0) {
				return NULL;
			}

			__movsb((unsigned char*)dest, (const unsigned char*)src, count);
			return dest;
		}
#		define md_memory_copy(dst, src, size) memcpy_intrinsic((dst), (src), (size))
#	else
#		define md_memory_copy(dst, src, size) memmove((dst), (src), (size))
#	endif
#endif
#ifndef md_memory_set
#	if USE_VENDOR_MEMORY_OPS
#		define md_memory_set(dst, byte, size) memset((dst), (byte), (size))
#	else
#		define md_memory_set(dst, byte, size) md_mem_set((dst), (byte), (size))
#	endif
#endif
#ifndef md_memory_compare
#define md_memory_compare(a, b, size)     memcmp((a), (b), (size))
#endif
#ifndef md_memory_str_len
#define md_memory_str_len(ptr)            cstr_len(ptr)
#endif

#ifndef md_memory_copy_struct
#define md_memory_copy_struct(d, s)       md_memory_copy((d), (s), sizeof( *(d)))
#endif
#ifndef md_memory_copy_array
#define md_memory_copy_array(d, s)        md_memory_copy((d), (s), sizeof( d))
#endif
#ifndef md_memory_copy_type
#define md_memory_copy_type(d, s, c)      md_memory_copy((d), (s), sizeof( *(d)) * (c))
#endif

#ifndef md_memory_zero
#define md_memory_zero(s,z)               md_memory_set((s), 0, (z))
#endif
#ifndef md_memory_zero_struct
#define md_memory_zero_struct(s)          md_memory_zero((s), sizeof( *(s)))
#endif
#ifndef md_memory_zero_array
#define md_memory_zero_array(a)           md_memory_zero((a), sizeof(a))
#endif
#ifndef md_memory_zero_type
#define md_memory_zero_type(m, c)         md_memory_zero((m), sizeof( *(m)) * (c))
#endif

#ifndef md_memory_match
#define md_memory_match(a, b, z)          (md_memory_compare((a), (b), (z)) == 0)
#endif
#ifndef md_memory_match_struct
#define md_memory_match_struct(a, b)      md_memory_match((a), (b), sizeof(*(a)))
#endif
#ifndef md_memory_match_array
#define md_memory_match_array(a, b)       md_memory_match((a), (b), sizeof(a))
#endif

#ifndef md_memory_read
#define md_memory_read(T, p, e)          ( ((p) + sizeof(T) <= (e)) ? ( *(T*)(p)) : (0) )
#endif
#ifndef md_memory_consume
#define md_memory_consume(T, p, e)       ( ((p) + sizeof(T) <= (e)) ? ((p) += sizeof(T), *(T*)((p) - sizeof(T))) : ((p) = (e),0) )
#endif

////////////////////////////////
//~ rjf: Memory Functions

inline MD_B32
md_memory_is_zero(void* ptr, MD_U64 size)
{
	MD_B32 result = 1;
	
	// break down size
	MD_U64 extra  = (size & 0x7);
	MD_U64 count8 = (size >> 3);
	
	// check with 8-byte stride
	MD_U64* p64 = (MD_U64*)ptr;
	if (result)
	{
		for (MD_U64 i = 0; i < count8; i += 1, p64 += 1) {
			if (*p64 != 0){
				result = 0;
				goto done;
			}
		}
	}
	
	// check extra
	if (result)
	{
		MD_U8* p8 = (MD_U8*)p64;
		for (MD_U64 i = 0; i < extra; i += 1, p8 += 1) {
			if (*p8 != 0) {
				result = 0;
				goto done;
			}
		}
	}
	
done:;
	return(result);
}

inline
void* md_mem_move( void* destination, void const* source, MD_SSIZE byte_count )
{
	if ( destination == NULL )
	{
		return NULL;
	}

	MD_U8*       dest_ptr = md_rcast( MD_U8*, destination);
	MD_U8 const* src_ptr  = md_rcast( MD_U8 const*, source);

	if ( dest_ptr == src_ptr )
		return dest_ptr;

	// NOTE: Non-overlapping
	if ( src_ptr + byte_count <= dest_ptr || dest_ptr + byte_count <= src_ptr )  {  
		return md_memory_copy( dest_ptr, src_ptr, byte_count );
	}

	if ( dest_ptr < src_ptr )
	{
		if ( md_scast(MD_UPTR, src_ptr) % size_of( MD_SSIZE ) == md_scast(MD_UPTR, dest_ptr) % size_of( MD_SSIZE ) )
		{
			while ( md_pcast( MD_UPTR, dest_ptr) % size_of( MD_SSIZE ) )
			{
				if ( ! byte_count-- )
					return destination;

				*dest_ptr++ = *src_ptr++;
			}
			while ( byte_count >= size_of( MD_SSIZE ) )
			{
				* md_rcast(MD_SSIZE*, dest_ptr)  = * md_rcast(MD_SSIZE const*, src_ptr);
				byte_count -= size_of( MD_SSIZE );
				dest_ptr   += size_of( MD_SSIZE );
				src_ptr    += size_of( MD_SSIZE );
			}
		}
		for ( ; byte_count; byte_count-- )
			*dest_ptr++ = *src_ptr++;
	}
	else
	{
		if ( ( md_scast(MD_UPTR, src_ptr) % size_of( MD_SSIZE ) ) == ( md_scast(MD_UPTR, dest_ptr) % size_of( MD_SSIZE ) ) )
		{
			while ( md_scast(MD_UPTR, dest_ptr + byte_count ) % size_of( MD_SSIZE ) )
			{
				if ( ! byte_count-- )
					return destination;

				dest_ptr[ byte_count ] = src_ptr[ byte_count ];
			}
			while ( byte_count >= size_of( MD_SSIZE ) )
			{
				byte_count                              -= size_of( MD_SSIZE );
				* md_rcast(MD_SSIZE*, dest_ptr + byte_count )  = * md_rcast( MD_SSIZE const*, src_ptr + byte_count );
			}
		}
		while ( byte_count )
			byte_count--, dest_ptr[ byte_count ] = src_ptr[ byte_count ];
	}

	return destination;
}

inline
void* md_mem_set( void* destination, MD_U8 fill_byte, MD_SSIZE byte_count )
{
	if ( destination == NULL )
	{
		return NULL;
	}

	MD_SSIZE align_offset;
	MD_U8*   dest_ptr  = md_rcast( MD_U8*, destination);
	MD_U32   fill_word = ( ( MD_U32 )-1 ) / 255 * fill_byte;

	if ( byte_count == 0 )
		return destination;

	dest_ptr[ 0 ] = dest_ptr[ byte_count - 1 ] = fill_byte;
	if ( byte_count < 3 )
		return destination;

	dest_ptr[ 1 ] = dest_ptr[ byte_count - 2 ] = fill_byte;
	dest_ptr[ 2 ] = dest_ptr[ byte_count - 3 ] = fill_byte;
	if ( byte_count < 7 )
		return destination;

	dest_ptr[ 3 ] = dest_ptr[ byte_count - 4 ] = fill_byte;
	if ( byte_count < 9 )
		return destination;

	align_offset  = -md_scast(MD_SPTR, dest_ptr ) & 3;
	dest_ptr     += align_offset;
	byte_count   -= align_offset;
	byte_count   &= -4;

	* md_rcast( MD_U32*, ( dest_ptr + 0              ) ) = fill_word;
	* md_rcast( MD_U32*, ( dest_ptr + byte_count - 4 ) ) = fill_word;
	if ( byte_count < 9 )
		return destination;

	* md_rcast( MD_U32*, dest_ptr + 4 )               = fill_word;
	* md_rcast( MD_U32*, dest_ptr + 8 )               = fill_word;
	* md_rcast( MD_U32*, dest_ptr + byte_count - 12 ) = fill_word;
	* md_rcast( MD_U32*, dest_ptr + byte_count - 8 )  = fill_word;
	if ( byte_count < 25 )
		return destination;

	* md_rcast( MD_U32*, dest_ptr + 12 )              = fill_word;
	* md_rcast( MD_U32*, dest_ptr + 16 )              = fill_word;
	* md_rcast( MD_U32*, dest_ptr + 20 )              = fill_word;
	* md_rcast( MD_U32*, dest_ptr + 24 )              = fill_word;
	* md_rcast( MD_U32*, dest_ptr + byte_count - 28 ) = fill_word;
	* md_rcast( MD_U32*, dest_ptr + byte_count - 24 ) = fill_word;
	* md_rcast( MD_U32*, dest_ptr + byte_count - 20 ) = fill_word;
	* md_rcast( MD_U32*, dest_ptr + byte_count - 16 ) = fill_word;

	align_offset  = 24 + md_scast(MD_UPTR, dest_ptr ) & 4;
	dest_ptr     += align_offset;
	byte_count   -= align_offset;

	{
		MD_U64 fill_doubleword = ( md_scast( MD_U64, fill_word) << 32 ) | fill_word;
		while ( byte_count > 31 )
		{
			* md_rcast( MD_U64*, dest_ptr + 0 )  = fill_doubleword;
			* md_rcast( MD_U64*, dest_ptr + 8 )  = fill_doubleword;
			* md_rcast( MD_U64*, dest_ptr + 16 ) = fill_doubleword;
			* md_rcast( MD_U64*, dest_ptr + 24 ) = fill_doubleword;

			byte_count -= 32;
			dest_ptr += 32;
		}
	}

	return destination;
}

////////////////////////////////
//~ rjf: Atomic Operations

#ifndef md_ins_atomic_u64_eval
#	if MD_OS_WINDOWS
#		if MD_ARCH_X64
#			define md_ins_atomic_u64_eval(x)                 InterlockedAdd64((volatile __int64 *)(x), 0)
#			define md_ins_atomic_u64_inc_eval(x)             InterlockedIncrement64((volatile __int64 *)(x))
#			define md_ins_atomic_u64_dec_eval(x)             InterlockedDecrement64((volatile __int64 *)(x))
#			define md_ins_atomic_u64_eval_assign(x,c)        InterlockedExchange64((volatile __int64 *)(x), (c))
#			define md_ins_atomic_u64_add_eval(x,c)           InterlockedAdd64((volatile __int64 *)(x), c)
#			define md_ins_atomic_u64_eval_cond_assign(x,k,c) InterlockedCompareExchange64((volatile __int64 *)(x), (k), (c))
#			define md_ins_atomic_u32_eval(x,c)               InterlockedAdd((volatile LONG *)(x), 0)
#			define md_ins_atomic_u32_eval_assign(x,c)        InterlockedExchange((volatile LONG *)(x), (c))
#			define md_ins_atomic_u32_eval_cond_assign(x,k,c) InterlockedCompareExchange((volatile LONG *)(x), (k), (c))
#			define md_ins_atomic_ptr_eval_assign(x,c)        (void*) md_ins_atomic_u64_eval_assign((volatile __int64 *)(x), (__int64)(c))
#		else
#			error Atomic intrinsics not defined for this operating system / architecture combination.
#		endif
#	elif MD_OS_LINUX
#		if MD_ARCH_X64
#			define md_ins_atomic_u64_inc_eval(x) __sync_fetch_and_add((volatile MD_U64 *)(x), 1)
#		else
#			error Atomic intrinsics not defined for this operating system / architecture combination.
#		endif
#	else
#		error Atomic intrinsics not defined for this operating system.
#	endif
#endif

////////////////////////////////
//~ rjf: Linked List Building Macros

//- rjf: linked list macro helpers

#ifndef md_check_nil
#define md_check_nil(nil, p) ((p) == 0 || (p) == nil)
#endif
#ifndef md_set_nil
#define md_set_nil(nil, p)   ((p) = nil)
#endif

//- rjf: doubly-linked-lists

#ifndef MD_LINKED_LIST_PURE_MACRO
#define MD_LINKED_LIST_PURE_MACRO 0
#endif

#ifndef md_dll_insert_npz
// TODO(Ed): Review...
inline void
md_dll__insert_npz(
	void*  nil, 
	void** f, void** f_prev,
	void** l, void** l_next,
	void*  p, void** p_next, void** p_next_prev,
	void*  n, void** n_prev, void** n_next 
)
{
	if (md_check_nil(nil, *f)) {
		*f      = n;
		*l      = n;
		*n_prev = nil;
		*n_next = nil;
	}
	else
	{
		if (md_check_nil(nil, p)) {
			*n_next = *f;
			*f_prev =  n;
			*f      =  n;
			*n_prev =  nil;
		}
		else 
		{
			if (p == *l) {
				*l_next =  n;
				*n_prev = *l;
				*l      =  n;
				*n_next =  nil;
			}
			else
			{
				if ( ! md_check_nil(nil, p) && md_check_nil(nil, *p_next)) {
					*p_next_prev = n;
				}
				*n_next = *p_next;
				*p_next =  n;
				*n_prev =  p;
			}
		}
	}
}

#if ! MD_LINKED_LIST_PURE_MACRO
// insert next-previous with nil
#define md_dll_insert_npz(nil, f, l, p, n, next, prev) md_dll__insert_npz(nil, &f, &f->prev, &l, &l->next, p, &p->next, &p->next->prev, n, &n->prev, &n->next)
#else
// insert next-previous with nil
#define md_dll_insert_npz(nil, f, l, p, n, next, prev)                     \
(                                                                          \
	md_check_nil(nil, f) ? (                                               \
		(f) = (l) = (n),                                                   \
		md_set_nil(nil, (n)->next),                                        \
		md_set_nil(nil, (n)->prev)                                         \
	)                                                                      \
	: (                                                                    \
		md_check_nil(nil, p) ? (                                           \
			(n)->next = (f),                                               \
			(f)->prev = (n),                                               \
			(f) = (n),                                                     \
			md_set_nil(nil,(n)->prev)                                      \
		)                                                                  \
		: ((p) == (l)) ? (                                                 \
				(l)->next = (n),                                           \
				(n)->prev = (l),                                           \
				(l) = (n),                                                 \
				md_set_nil(nil, (n)->next)                                 \
			)                                                              \
			: (                                                            \
				( 	                                                       \
					( ! md_check_nil(nil, p) && md_check_nil(nil, (p)->next) ) ? \
						(0)                                                \
					:	( (p)->next->prev = (n) )                          \
				),                                                         \
				((n)->next = (p)->next),                                   \
				((p)->next = (n)),                                         \
				((n)->prev = (p))                                          \
			)                                                              \
	)                                                                      \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
// md_dll_insert_npz
#endif

#ifndef md_dll_push_back_npz
// push-back next-previous with nil
#define md_dll_push_back_npz(nil, f, l, n, next, prev)  md_dll_insert_npz(nil, f, l, l, n, next, prev)
#endif

#ifndef md_dll_push_front_npz
// push-fornt next-previous with nil
#define md_dll_push_front_npz(nil, f, l, n, next, prev) md_dll_insert_npz(nil, l, f, f, n, prev, next)
#endif

#ifndef md_dll_remove_npz
inline void
md_dll__remove_npz(
	void*  nil, 
	void** f, 
	void** l, void* l_prev, 
	void*  n, void* n_next, void** n_next_prev, 
	          void* n_prev, void** n_prev_next
)
{
	if (n == *f) {
		*f = n_next;
	}
	if (n == *l) {
		*l = l_prev;
	}
	if (md_check_nil(nil, n_prev)) {
		*n_prev_next = n_next;
	}
	if (! md_check_nil(nil, n_next)) {
		*n_next_prev = n_prev;
	}
}

#if ! MD_LINKED_LIST_PURE_MACRO
// remove next-previous with nil
#define md_dll_remove_npz(nil, f, l, n, next, prev) md_dll__remove_npz(nil, &f, &l, l->prev, n, n->next, &n->next->prev, n->prev, &n->prev->next)
#else
// remove next-previous with nil
#define md_dll_remove_npz(nil, f, l, n, next, prev) \
(                                                   \
	(                                               \
		(n) == (f) ?                                \
			(f) = (n)->next                         \
		: 	(0)                                     \
	),                                              \
	(                                               \
		(n) == (l) ?                                \
			(l) = (l)->prev                         \
		:	(0)                                     \
	),                                              \
	(                                               \
		md_check_nil(nil,(n)->prev) ?               \
			(0)                                     \
		: 	((n)->prev->next = (n)->next)           \
	),                                              \
	(                                               \
		md_check_nil(nil,(n)->next) ?               \
			(0)                                     \
		:	((n)->next->prev = (n)->prev)           \
	)                                               \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
// md_dll_remove_npz
#endif

//- rjf: singly-linked, doubly-headed lists (queues)

#ifndef md_sll_queue_push_nz
inline void
md_sll__queue_push_nz(
	void* nil, 
	void** f, 
	void** l, void** l_next, 
	void*  n, void** n_next
) 
{
	if (md_check_nil(nil, *f)) {
		*f      = n;
		*l      = n;
		*n_next = nil;
	}
	else {
		*l_next = n;
		*l      = n;
		*n_next = nil;
	}
}

// queue-push next with nil
#if ! MD_LINKED_LIST_PURE_MACRO
#define md_sll_queue_push_nz(nil, f, l, n, next) md_sll__queue_push_nz(nil, &f, &l, &l->next, n, &n->next)
#else
#define md_sll_queue_push_nz(nil, f, l, n, next) \
(                                                \
	md_check_nil(nil, f) ? (                     \
		(f) = (l) = (n),                         \
		md_set_nil(nil, (n)->next)               \
	)                                            \
	: (                                          \
		(l)->next=(n),                           \
		(l) = (n),                               \
		md_set_nil(nil,(n)->next)                \
	)                                            \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
// md_sll_queue_push_nz
#endif

#ifndef md_sll_queue_push_front_nz
inline void
md_sll__queue_push_front_nz(void* nil, void** f, void** l, void* n, void** n_next) {
	if (md_check_nil(nil, *f)) {
		*f      = n;
		*l      = n;
		*n_next = nil;
	}
	else {
		*n_next = f;
		*f      = n;
	}
}

// queue-push-front next with nil
#if ! MD_LINKED_LIST_PURE_MACRO
#define md_sll_queue_push_front_nz(nil, f, l, n, next) md_sll__queue_push_front_nz(nil, &f, &l, n, &n->next)
#else
#define md_sll_queue_push_front_nz(nil, f, l, n, next) \
(                                                      \
	md_check_nil(nil, f) ? (                           \
		(f) = (l) = (n),                               \
		md_set_nil(nil,(n)->next)                      \
	)                                                  \
	: (                                                \
		(n)->next = (f),                               \
		(f) = (n)                                      \
	)                                                  \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
#endif

#ifndef md_sll_queue_pop_nz
inline void
md_sll__queue_pop_nz(void* nil, void** f, void* f_next, void** l)
{
	if (*f == *l) {
		*f = nil;
		*l = nil;
	}
	else {
		*f = f_next;
	}
}

// queue-pop next with nil
#if ! MD_LINKED_LIST_PURE_MACRO
#define md_sll_queue_pop_nz(nil, f, l, next) md_sll__queue_pop_nz(nil, &f, f->next, &l)
#else
#define md_sll_queue_pop_nz(nil, f, l, next) \
(                                            \
	(f) == (l) ? (                           \
		md_set_nil(nil,f),                   \
		md_set_nil(nil,l)                    \
	)                                        \
	: (                                      \
		(f)=(f)->next                        \
	)                                        \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
// md_sll_queue_pop_nz
#endif

//- rjf: singly-linked, singly-headed lists (stacks)

#ifndef md_sll_stack_push_n
#define md_sll_stack_push_n(f,n,next) ( (n)->next = (f), (f) = (n) )
#endif
#ifndef md_sll_stack_pop_n
#define md_sll_stack_pop_n(f,next)    ( (f) = (f)->next )
#endif

//- rjf: doubly-linked-list helpers

#ifndef md_dll_insert_np
#define md_dll_insert_np(f, l, p, n, next, prev)  md_dll_insert_npz    (0, f, l, p, n,    next, prev)
#endif
#ifndef md_dll_push_back_np
#define md_dll_push_back_np(f, l, n, next, prev)  md_dll_push_back_npz (0, f, l, n, next,       prev)
#endif
#ifndef md_dll_push_front_np
#define md_dll_push_front_np(f, l, n, next, prev) md_dll_push_front_npz(0, f, l, n, next,       prev)
#endif
#ifndef md_dll_remove_np
#define md_dll_remove_np(f, l, n, next, prev)     md_dll_remove_npz    (0, f, l, n, next,       prev)
#endif
#ifndef md_dll_insert
#define md_dll_insert(f, l, p, n)                 md_dll_insert_npz    (0, f, l, p, n,    next, prev)
#endif
#ifndef md_dll_push_back
#define md_dll_push_back(f, l, n)                 md_dll_push_back_npz (0, f, l, n, next, prev)
#endif
#ifndef md_dll_push_front
#define md_dll_push_front(f, l, n)                md_dll_push_front_npz(0, f, l, n, next, prev)
#endif
#ifndef md_dll_remove
#define md_dll_remove(f, l, n)                    md_dll_remove_npz    (0, f, l, n, next, prev)
#endif

//- rjf: singly-linked, doubly-headed list helpers

#ifndef md_sll_queue_push_n
#define md_sll_queue_push_n(f, l, n, next)       md_sll_queue_push_nz      (0, f, l, n, next)
#endif
#ifndef md_sll_queue_push_front_n
#define md_sll_queue_push_front_n(f, l, n, next) md_sll_queue_push_front_nz(0, f, l, n, next)
#endif
#ifndef md_sll_queue_pop_n
#define md_sll_queue_pop_n(f, l, next)           md_sll_queue_pop_nzs      (0, f, l,    next)
#endif
#ifndef md_sll_queue_push
#define md_sll_queue_push(f, l, n)               md_sll_queue_push_nz      (0, f, l, n, next)
#endif
#ifndef md_sll_queue_push_front
#define md_sll_queue_push_front(f, l ,n)         md_sll_queue_push_front_nz(0, f, l, n, next)
#endif
#ifndef md_sll_queue_pop
#define md_sll_queue_pop(f, l)                   md_sll_queue_pop_nz       (0, f, l,    next)
#endif

//- rjf: singly-linked, singly-headed list helpers

#ifndef md_sll_stack_push
#define md_sll_stack_push(f, n) md_sll_stack_push_n(f, n, next)
#endif
#ifndef md_sll_stack_pop
#define md_sll_stack_pop(f)     md_sll_stack_pop_n (f,    next)
#endif

////////////////////////////////
//~ rjf: Address Sanitizer Markup

#ifndef MD_NO_ASAN
#	if MD_COMPILER_MSVC
#		if defined(__SANITIZE_ADDRESS__)
#			define MD_ASAN_ENABLED 1
#	 		define MD_NO_ASAN __declspec(no_sanitize_address)
#		else
#	 		define MD_NO_ASAN
#		endif
#	elif MD_COMPILER_CLANG
#		if defined(__has_feature)
#	 		if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#	  		define MD_ASAN_ENABLED 1
#	 		endif
#		endif
#		define MD_NO_ASAN __attribute__((no_sanitize("address")))
#	else
#		define MD_NO_ASAN
#	endif
#endif

#ifndef md_asan_poison_memory_region
#	if MD_ASAN_ENABLED
#		pragma comment(lib, "clang_rt.asan-x86_64.lib")
		MD_C_API void __asan_poison_memory_region(void const volatile *addr, size_t size);
		MD_C_API void __asan_unpoison_memory_region(void const volatile *addr, size_t size);

#		define md_asan_poison_memory_region(addr, size)   __asan_poison_memory_region((addr), (size))
#		define md_asan_unpoison_memory_region(addr, size) __asan_unpoison_memory_region((addr), (size))
#	else
#		define md_asan_poison_memory_region(addr, size)   ((void)(addr), (void)(size))
#		define md_asan_unpoison_memory_region(addr, size) ((void)(addr), (void)(size))
#	endif
#endif

////////////////////////////////
//~ rjf: Misc. Helper Macros

#ifndef md_array_count
#define md_array_count(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef md_ceil_integer_div
#define md_ceil_integer_div(a,b) (((a) + (b) - 1) / (b))
#endif

#ifndef md_swap
#define md_swap(T, a, b) do { T t__ = a; a = b; b = t__; } while(0)
#endif

#ifndef md_int_from_ptr
#	if MD_ARCH_64BIT
#		define md_int_from_ptr(ptr) ((MD_U64)(ptr))
#	elif MD_ARCH_32BIT
#		define md_int_from_ptr(ptr) ((MD_U32)(ptr))
#	else
#		error Missing pointer-to-integer cast for this architecture.
#	endif
#endif

#ifndef md_ptr_from_int
#define md_ptr_from_int(i) (void*)((MD_U8*)0 + (i))
#endif

#ifndef md_compose_64bit
#define md_compose_64bit(a,b)   ((((MD_U64)a) << 32) | ((MD_U64)b));
#endif
#ifndef md_align_pow2
#define md_align_pow2(x,b)      (((x) + (b) - 1) & ( ~((b) - 1)))
#endif
#ifndef md_align_down_pow2
#define md_align_down_pow2(x,b) ((x) & (~((b) - 1)))
#endif
#ifndef md_align_pad_pow2
#define md_align_pad_pow2(x,b)  ((0-(x)) & ((b) - 1))
#endif
#ifndef md_is_pow2
#define md_is_pow2(x)           ((x) != 0 && ((x ) & ((x) - 1)) == 0)
#endif
#ifndef md_is_pow2_or_zero
#define md_is_pow2_or_zero(x)   ((((x) - 1) & (x)) == 0)
#endif

#ifndef md_extract_bit
#define md_extract_bit(word, idx) (((word) >> (idx)) & 1)
#endif

#ifndef md_zero_struct
#	if MD_LANG_CPP
#		define md_zero_struct {}
#	else
#		define md_zero_struct {0}
#	endif
#endif

#ifndef md_this_function_name
#	if MD_COMPILER_MSVC && MD_COMPILER_MSVC_YEAR < 2015
#		define md_this_function_name "unknown"
#	else
#		define md_this_function_name __func__
#	endif
#endif

#ifndef md_read_only
#	if MD_COMPILER_MSVC || (MD_COMPILER_CLANG && MD_OS_WINDOWS)
#		pragma section(".rdata$", read)
#		define md_read_only __declspec(allocate(".rdata$"))
#	elif (MD_COMPILER_CLANG && MD_OS_LINUX)
#		define md_read_only __attribute__((section(".rodata")))
#	else
		// NOTE(rjf): I don't know of a useful way to do this in GCC land.
		// __attribute__((section(".rodata"))) looked promising, but it introduces a
		// strange warning about malformed section attributes, and it doesn't look
		// like writing to that section reliably produces access violations, strangely
		// enough. (It does on Clang)
#		define md_read_only
#	endif
#endif

#ifndef md_local_persist
#define md_local_persist static
#endif

#if MD_COMPILER_MSVC
#	define md_thread_static __declspec(thread)
#elif MD_COMPILER_CLANG || MD_COMPILER_GCC
#	define md_thread_static __thread
#endif

#if MD_COMPILER_CPP
// Already Defined
#elif MD_COMPILER_C && __STDC_VERSION__ >= 201112L
#	define md_thread_local _Thread_local
#elif MD_COMPILER_MSVC
#	define md_thread_local __declspec(thread)
#elif MD_COMPILER_CLANG
#	define md_thread_local __thread
#else
#	error "No thread local support"
#endif


////////////////////////////////
//~ rjf: Safe Casts

inline MD_U16
md_safe_cast_u16(MD_U32 x) {
	md_assert_always(x <= MD_MAX_U16);
	MD_U16 result = (MD_U16)x;
	return result;
}

inline MD_U32
md_safe_cast_u32(MD_U64 x) {
	md_assert_always(x <= MD_MAX_U32);
	MD_U32 result = (MD_U32)x;
	return result;
}

inline MD_S32
md_safe_cast_s32(MD_S64 x) {
	md_assert_always(x <= MD_MAX_S32);
	MD_S32 result = (MD_S32)x;
	return result;
}

////////////////////////////////
//~ rjf: Large Base Type Functions

inline U128 md_u128_zero (void)           { U128 v = {0};      return v; }
inline U128 md_u128_make (MD_U64 v0, MD_U64 v1) { U128 v = {v0, v1}; return v; }
inline MD_B32  md_u128_match(U128 a, U128 b) { return md_memory_match_struct(&a, &b); }

////////////////////////////////
//~ rjf: Bit Patterns

inline MD_U32 md_u32_from_u64_saturate(MD_U64 x) { MD_U32 x32 = (x > MD_MAX_U32) ? MD_MAX_U32 : (MD_U32)x; return(x32); }

inline MD_U64
md_u64_up_to_pow2(MD_U64 x) {
	if (x == 0) {
		x = 1;
	}
	else {
		x -= 1;
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		x |= (x >> 32);
		x += 1;
	}
	return(x);
}

inline MD_S32
md_extend_sign32(MD_U32 x, MD_U32 size) {
	MD_U32 high_bit = size * 8;
	MD_U32 shift    = 32 - high_bit;
	MD_S32 result   = ((MD_S32)x << shift) >> shift;
	return result;
}

inline MD_S64
md_extend_sign64(MD_U64 x, MD_U64 size) {
	MD_U64 high_bit = size * 8;
	MD_U64 shift    = 64 - high_bit;
	MD_S64 result   = ((MD_S64)x << shift) >> shift;
	return result;
}

inline MD_F32 md_inf32    (void) { union { MD_U32 u; MD_F32 f; } x; x.u = MD_EXPONENT32;          return(x.f); }
inline MD_F32 md_neg_inf32(void) { union { MD_U32 u; MD_F32 f; } x; x.u = MD_SIGN32 | MD_EXPONENT32; return(x.f); }

inline MD_U16
md_bswap_u16(MD_U16 x)
{
	MD_U16 result = (((x & 0xFF00) >> 8) |
					 ((x & 0x00FF) << 8));
	return result;
}

inline MD_U32
md_bswap_u32(MD_U32 x)
{
	MD_U32 result = (((x & 0xFF000000) >> 24) |
					 ((x & 0x00FF0000) >> 8)  |
					 ((x & 0x0000FF00) << 8)  |
					 ((x & 0x000000FF) << 24));
	return result;
}

inline MD_U64
md_bswap_u64(MD_U64 x)
{
  // TODO(nick): naive bswap, replace with something that is faster like an intrinsic
  MD_U64 result = (((x & 0xFF00000000000000ULL) >> 56) |
                   ((x & 0x00FF000000000000ULL) >> 40) |
                   ((x & 0x0000FF0000000000ULL) >> 24) |
                   ((x & 0x000000FF00000000ULL) >> 8)  |
                   ((x & 0x00000000FF000000ULL) << 8)  |
                   ((x & 0x0000000000FF0000ULL) << 24) |
                   ((x & 0x000000000000FF00ULL) << 40) |
                   ((x & 0x00000000000000FFULL) << 56));
  return result;
}

#if MD_ARCH_LITTLE_ENDIAN
# define md_from_be_u16(x) md_bswap_u16(x)
# define md_from_be_u32(x) md_bswap_u32(x)
# define md_from_be_u64(x) md_bswap_u64(x)
#else
# define md_from_be_u16(x) (x)
# define md_from_be_u32(x) (x)
# define md_from_be_u64(x) (x)
#endif

#if MD_COMPILER_MSVC || (MD_COMPILER_CLANG && MD_OS_WINDOWS)
	inline MD_U64 md_count_bits_set16(MD_U16 val) { return __popcnt16(val); }
	inline MD_U64 md_count_bits_set32(MD_U32 val) { return __popcnt  (val); }
	inline MD_U64 md_count_bits_set64(MD_U64 val) { return __popcnt64(val); }

	inline MD_U64 md_ctz32(MD_U32 mask) { unsigned long idx; _BitScanForward  (&idx, mask); return idx; }
	inline MD_U64 md_ctz64(MD_U64 mask) { unsigned long idx; _BitScanForward64(&idx, mask); return idx; }
	inline MD_U64 md_clz32(MD_U32 mask) { unsigned long idx; _BitScanReverse  (&idx, mask); return 31 - idx; }
	inline MD_U64 md_clz64(MD_U64 mask) { unsigned long idx; _BitScanReverse64(&idx, mask); return 63 - idx; }
#elif MD_COMPILER_CLANG || MD_COMPILER_GCC
	inline MD_U64 md_count_bits_set16(MD_U16 val) { NotImplemented; return 0; }
	inline MD_U64 md_count_bits_set32(MD_U32 val) { NotImplemented; return 0; }
	inline MD_U64 md_count_bits_set64(MD_U64 val) { NotImplemented; return 0; }

	inline MD_U64 md_ctz32(MD_U32 val) { NotImplemented; return 0; }
	inline MD_U64 md_ctz64(MD_U32 val) { NotImplemented; return 0; }
	inline MD_U64 md_clz32(MD_U32 val) { NotImplemented; return 0; }
	inline MD_U64 md_clz64(MD_U64 val) { NotImplemented; return 0; }
#else
#	error "Bit intrinsic functions not defined for this compiler."
#endif
