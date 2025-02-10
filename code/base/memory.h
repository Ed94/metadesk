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
#define KILOBYTES( x ) (             ( x ) * ( S64 )( 1024 ) )
#endif
#ifndef MEGABYTES
#define MEGABYTES( x ) ( MD_KILOBYTES( x ) * ( S64 )( 1024 ) )
#endif
#ifndef GIGABYTES
#define GIGABYTES( x ) ( MD_MEGABYTES( x ) * ( S64 )( 1024 ) )
#endif
#ifndef TERABYTES
#define TERABYTES( x ) ( MD_GIGABYTES( x ) * ( S64 )( 1024 ) )
#endif

#ifndef KB
#define KB(n)  (((U64)(n)) << 10)
#endif
#ifndef MB
#define MB(n)  (((U64)(n)) << 20)
#endif
#ifndef GB
#define GB(n)  (((U64)(n)) << 30)
#endif
#ifndef TB
#define TB(n)  (((U64)(n)) << 40)
#endif

#ifndef thosuand
#define thousand(n)   ((n) * 1000)
#endif
#ifndef million
#define million(n)    ((n) * 1000000)
#endif
#ifndef billion
#define billion(n)    ((n) * 1000000000)
#endif

////////////////////////////////
//~ rjf: Type -> Alignment

#ifndef align_of
#	if COMPILER_MSVC
#		define align_of(T) __alignof(T)
#	elif COMPILER_CLANG
#		define align_of(T) __alignof(T)
#	elif COMPILER_GCC
#		define align_of(T) __alignof__(T)
#	else
#		error AlignOf not defined for this compiler.
#	endif
#endif

////////////////////////////////
//~ rjf: Member Offsets

#ifndef membeMD_DYN_LINKr
#define member(T, m)                    ( ((T*) 0)->m )
#endif
#ifndef offset_of
#define offset_of(T, m)                 int_from_ptr(& member(T, m))
#endif
#ifndef member_from_offset
#define member_from_offset(T, ptr, off) (T)  ((((U8 *) ptr) + (off)))
#endif
#ifndef cast_from_member
#define cast_from_member(T, m, ptr)     (T*) (((U8*)ptr) - offset_of(T, m))
#endif

////////////////////////////////
//~ rjf: Memory Operation Macros


// TODO(Ed): Review usage of memmove here...(I guess wanting to avoid overlap faults..)
#ifndef memory_copy
#	if OS_WINDOWS
		void* memcpy_intrinsic(void* dest, const void* src, size_t count) 
		{
			if (dest == NULL || src == NULL || count == 0) {
				return NULL;
			}

			__movsb((unsigned char*)dest, (const unsigned char*)src, count);
			return dest;
		}
#		define memory_copy(dst, src, size)        memcpy_intrinsic((dst), (src), (size))
#	else
#		define memory_copy(dst, src, size)        memmove((dst), (src), (size))
#	endif
#endif
#ifndef memory_set
#	if USE_VENDOR_MEMORY_OPS
#		define memory_set(dst, byte, size)    memset((dst), (byte), (size))
#	else
#		define memory_set(dst, byte, size)    mem_set((dst), (byte), (size))
#	endif
#endif
#ifndef memory_compare
#define memory_compare(a, b, size)     memcmp((a), (b), (size))
#endif
#ifndef memory_str_len
#define memory_str_len(ptr)            cstr_len(ptr)
#endif

#ifndef memory_copy_struct
#define memory_copy_struct(d, s)       memory_copy((d), (s), sizeof( *(d)))
#endif
#ifndef memory_copy_array
#define memory_copy_array(d, s)        memory_copy((d), (s), sizeof( d))
#endif
#ifndef memory_copy_type
#define memory_copy_type(d, s, c)      memory_copy((d), (s), sizeof( *(d)) * (c))
#endif

#ifndef memory_zero
#define memory_zero(s,z)               memory_set((s), 0, (z))
#endif
#ifndef memory_zero_struct
#define memory_zero_struct(s)          memory_zero((s), sizeof( *(s)))
#endif
#ifndef memory_zero_array
#define memory_zero_array(a)           memory_zero((a), sizeof(a))
#endif
#ifndef memory_zero_type
#define memory_zero_type(m, c)         memory_zero((m), sizeof( *(m)) * (c))
#endif

#ifndef memory_match
#define memory_match(a, b, z)          (memory_compare((a), (b), (z)) == 0)
#endif
#ifndef memory_match_struct
#define memory_match_struct(a, b)      memory_match((a), (b), sizeof(*(a)))
#endif
#ifndef memory_match_array
#define memory_match_array(a, b)       memory_match((a), (b), sizeof(a))
#endif

#ifndef memory_read
#define memory_read(T, p, e)          ( ((p) + sizeof(T) <= (e)) ? ( *(T*)(p)) : (0) )
#endif
#ifndef memory_consume
#define memory_consume(T, p, e)       ( ((p) + sizeof(T) <= (e)) ? ((p) += sizeof(T), *(T*)((p) - sizeof(T))) : ((p) = (e),0) )
#endif

////////////////////////////////
//~ rjf: Memory Functions

inline B32
memory_is_zero(void* ptr, U64 size)
{
	B32 result = 1;
	
	// break down size
	U64 extra  = (size & 0x7);
	U64 count8 = (size >> 3);
	
	// check with 8-byte stride
	U64* p64 = (U64*)ptr;
	if (result)
	{
		for (U64 i = 0; i < count8; i += 1, p64 += 1) {
			if (*p64 != 0){
				result = 0;
				goto done;
			}
		}
	}
	
	// check extra
	if (result)
	{
		U8* p8 = (U8*)p64;
		for (U64 i = 0; i < extra; i += 1, p8 += 1) {
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
void* mem_move( void* destination, void const* source, SSIZE byte_count )
{
	if ( destination == NULL )
	{
		return NULL;
	}

	U8*       dest_ptr = rcast( U8*, destination);
	U8 const* src_ptr  = rcast( U8 const*, source);

	if ( dest_ptr == src_ptr )
		return dest_ptr;

	// NOTE: Non-overlapping
	if ( src_ptr + byte_count <= dest_ptr || dest_ptr + byte_count <= src_ptr )  {  
		return memory_copy( dest_ptr, src_ptr, byte_count );
	}

	if ( dest_ptr < src_ptr )
	{
		if ( scast(UPTR, src_ptr) % size_of( SSIZE ) == scast(UPTR, dest_ptr) % size_of( SSIZE ) )
		{
			while ( pcast( UPTR, dest_ptr) % size_of( SSIZE ) )
			{
				if ( ! byte_count-- )
					return destination;

				*dest_ptr++ = *src_ptr++;
			}
			while ( byte_count >= size_of( SSIZE ) )
			{
				* rcast(SSIZE*, dest_ptr)  = * rcast(SSIZE const*, src_ptr);
				byte_count -= size_of( SSIZE );
				dest_ptr   += size_of( SSIZE );
				src_ptr    += size_of( SSIZE );
			}
		}
		for ( ; byte_count; byte_count-- )
			*dest_ptr++ = *src_ptr++;
	}
	else
	{
		if ( ( scast(UPTR, src_ptr) % size_of( SSIZE ) ) == ( scast(UPTR, dest_ptr) % size_of( SSIZE ) ) )
		{
			while ( scast(UPTR, dest_ptr + byte_count ) % size_of( SSIZE ) )
			{
				if ( ! byte_count-- )
					return destination;

				dest_ptr[ byte_count ] = src_ptr[ byte_count ];
			}
			while ( byte_count >= size_of( SSIZE ) )
			{
				byte_count                              -= size_of( SSIZE );
				* rcast(SSIZE*, dest_ptr + byte_count )  = * rcast( SSIZE const*, src_ptr + byte_count );
			}
		}
		while ( byte_count )
			byte_count--, dest_ptr[ byte_count ] = src_ptr[ byte_count ];
	}

	return destination;
}

inline
void* mem_set( void* destination, U8 fill_byte, SSIZE byte_count )
{
	if ( destination == NULL )
	{
		return NULL;
	}

	SSIZE align_offset;
	U8*   dest_ptr  = rcast( U8*, destination);
	U32   fill_word = ( ( U32 )-1 ) / 255 * fill_byte;

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

	align_offset  = -scast(SPTR, dest_ptr ) & 3;
	dest_ptr     += align_offset;
	byte_count   -= align_offset;
	byte_count   &= -4;

	* rcast( U32*, ( dest_ptr + 0              ) ) = fill_word;
	* rcast( U32*, ( dest_ptr + byte_count - 4 ) ) = fill_word;
	if ( byte_count < 9 )
		return destination;

	* rcast( U32*, dest_ptr + 4 )               = fill_word;
	* rcast( U32*, dest_ptr + 8 )               = fill_word;
	* rcast( U32*, dest_ptr + byte_count - 12 ) = fill_word;
	* rcast( U32*, dest_ptr + byte_count - 8 )  = fill_word;
	if ( byte_count < 25 )
		return destination;

	* rcast( U32*, dest_ptr + 12 )              = fill_word;
	* rcast( U32*, dest_ptr + 16 )              = fill_word;
	* rcast( U32*, dest_ptr + 20 )              = fill_word;
	* rcast( U32*, dest_ptr + 24 )              = fill_word;
	* rcast( U32*, dest_ptr + byte_count - 28 ) = fill_word;
	* rcast( U32*, dest_ptr + byte_count - 24 ) = fill_word;
	* rcast( U32*, dest_ptr + byte_count - 20 ) = fill_word;
	* rcast( U32*, dest_ptr + byte_count - 16 ) = fill_word;

	align_offset  = 24 + scast(UPTR, dest_ptr ) & 4;
	dest_ptr     += align_offset;
	byte_count   -= align_offset;

	{
		U64 fill_doubleword = ( scast( U64, fill_word) << 32 ) | fill_word;
		while ( byte_count > 31 )
		{
			* rcast( U64*, dest_ptr + 0 )  = fill_doubleword;
			* rcast( U64*, dest_ptr + 8 )  = fill_doubleword;
			* rcast( U64*, dest_ptr + 16 ) = fill_doubleword;
			* rcast( U64*, dest_ptr + 24 ) = fill_doubleword;

			byte_count -= 32;
			dest_ptr += 32;
		}
	}

	return destination;
}

////////////////////////////////
//~ rjf: Atomic Operations

#ifndef ins_atomic_u64_eval
#	if OS_WINDOWS
#		if ARCH_X64
#			define ins_atomic_u64_eval(x)                 InterlockedAdd64((volatile __int64 *)(x), 0)
#			define ins_atomic_u64_inc_eval(x)             InterlockedIncrement64((volatile __int64 *)(x))
#			define ins_atomic_u64_dec_eval(x)             InterlockedDecrement64((volatile __int64 *)(x))
#			define ins_atomic_u64_eval_assign(x,c)        InterlockedExchange64((volatile __int64 *)(x), (c))
#			define ins_atomic_u64_add_eval(x,c)           InterlockedAdd64((volatile __int64 *)(x), c)
#			define ins_atomic_u64_eval_cond_assign(x,k,c) InterlockedCompareExchange64((volatile __int64 *)(x), (k), (c))
#			define ins_atomic_u32_eval(x,c)               InterlockedAdd((volatile LONG *)(x), 0)
#			define ins_atomic_u32_eval_assign(x,c)        InterlockedExchange((volatile LONG *)(x), (c))
#			define ins_atomic_u32_eval_cond_assign(x,k,c) InterlockedCompareExchange((volatile LONG *)(x), (k), (c))
#			define ins_atomic_ptr_eval_assign(x,c)        (void*) ins_atomic_u64_eval_assign((volatile __int64 *)(x), (__int64)(c))
#		else
#			error Atomic intrinsics not defined for this operating system / architecture combination.
#		endif
#	elif OS_LINUX
#		if ARCH_X64
#			define ins_atomic_u64_inc_eval(x) __sync_fetch_and_add((volatile U64 *)(x), 1)
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

#ifndef check_nil
#define check_nil(nil, p) ((p) == 0 || (p) == nil)
#endif
#ifndef set_nil
#define set_nil(nil, p)   ((p) = nil)
#endif

//- rjf: doubly-linked-lists

#ifndef MD_LINKED_LIST_PURE_MACRO
#define MD_LINKED_LIST_PURE_MACRO 0
#endif

#ifndef dll_insert_npz
// TODO(Ed): Review...
inline void
dll__insert_npz(
	void*  nil, 
	void** f, void** f_prev,
	void** l, void** l_next,
	void*  p, void** p_next, void** p_next_prev,
	void*  n, void** n_prev, void** n_next 
)
{
	if (check_nil(nil, *f)) {
		*f      = n;
		*l      = n;
		*n_prev = nil;
		*n_next = nil;
	}
	else
	{
		if (check_nil(nil, p)) {
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
				if ( ! check_nil(nil, p) && check_nil(nil, *p_next)) {
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
#define dll_insert_npz(nil, f, l, p, n, next, prev) dll__insert_npz(nil, &f, &f->prev, &l, &l->next, p, &p->next, &p->next->prev, n, &n->prev, &n->next)
#else
// insert next-previous with nil
#define dll_insert_npz(nil, f, l, p, n, next, prev)                        \
(                                                                          \
	check_nil(nil, f) ? (                                                  \
		(f) = (l) = (n),                                                   \
		set_nil(nil, (n)->next),                                           \
		set_nil(nil, (n)->prev)                                            \
	)                                                                      \
	: (                                                                    \
		check_nil(nil, p) ? (                                              \
			(n)->next = (f),                                               \
			(f)->prev = (n),                                               \
			(f) = (n),                                                     \
			set_nil(nil,(n)->prev)                                         \
		)                                                                  \
		: ((p) == (l)) ? (                                                 \
				(l)->next = (n),                                           \
				(n)->prev = (l),                                           \
				(l) = (n),                                                 \
				set_nil(nil, (n)->next)                                    \
			)                                                              \
			: (                                                            \
				( 	                                                       \
					( ! check_nil(nil, p) && check_nil(nil, (p)->next) ) ? \
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
// dll_insert_npz
#endif

#ifndef dll_push_back_npz
// push-back next-previous with nil
#define dll_push_back_npz(nil, f, l, n, next, prev)  dll_insert_npz(nil, f, l, l, n, next, prev)
#endif

#ifndef dll_push_front_npz
// push-fornt next-previous with nil
#define dll_push_front_npz(nil, f, l, n, next, prev) dll_insert_npz(nil, l, f, f, n, prev, next)
#endif

#ifndef dll_remove_npz
inline void
dll__remove_npz(
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
	if (check_nil(nil, n_prev)) {
		*n_prev_next = n_next;
	}
	if (! check_nil(nil, n_next)) {
		*n_next_prev = n_prev;
	}
}

#if ! MD_LINKED_LIST_PURE_MACRO
// remove next-previous with nil
#define dll_remove_npz(nil, f, l, n, next, prev) dll__remove_npz(nil, &f, &l, l->prev, n, n->next, &n->next->prev, n->prev, &n->prev->next)
#else
// remove next-previous with nil
#define dll_remove_npz(nil, f, l, n, next, prev) \
(                                                \
	(                                            \
		(n) == (f) ?                             \
			(f) = (n)->next                      \
		: 	(0)                                  \
	),                                           \
	(                                            \
		(n) == (l) ?                             \
			(l) = (l)->prev                      \
		:	(0)                                  \
	),                                           \
	(                                            \
		check_nil(nil,(n)->prev) ?               \
			(0)                                  \
		: 	((n)->prev->next = (n)->next)        \
	),                                           \
	(                                            \
		check_nil(nil,(n)->next) ?               \
			(0)                                  \
		:	((n)->next->prev = (n)->prev)        \
	)                                            \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
// dll_remove_npz
#endif

//- rjf: singly-linked, doubly-headed lists (queues)

#ifndef sll_queue_push_nz
inline void
sll__queue_push_nz(
	void* nil, 
	void** f, 
	void** l, void** l_next, 
	void*  n, void** n_next
) 
{
	if (check_nil(nil, *f)) {
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
#define sll_queue_push_nz(nil, f, l, n, next) sll__queue_push_nz(nil, &f, &l, &l->next, n, &n->next)
#else
#define sll_queue_push_nz(nil, f, l, n, next) \
(                                             \
	check_nil(nil, f) ? (                     \
		(f) = (l) = (n),                      \
		set_nil(nil, (n)->next)               \
	)                                         \
	: (                                       \
		(l)->next=(n),                        \
		(l) = (n),                            \
		set_nil(nil,(n)->next)                \
	)                                         \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
// sll_queue_push_nz
#endif

#ifndef sll_queue_push_front_nz
inline void
sll__queue_push_front_nz(void* nil, void** f, void** l, void* n, void** n_next) {
	if (check_nil(nil, *f)) {
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
#define sll_queue_push_front_nz(nil, f, l, n, next) sll__queue_push_front_nz(nil, &f, &l, n, &n->next)
#else
#define sll_queue_push_front_nz(nil, f, l, n, next) \
(                                                   \
	check_nil(nil, f) ? (                           \
		(f) = (l) = (n),                            \
		set_nil(nil,(n)->next)                      \
	)                                               \
	: (                                             \
		(n)->next = (f),                            \
		(f) = (n)                                   \
	)                                               \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
#endif

#ifndef sll_queue_pop_nz
inline void
sll__queue_pop_nz(void* nil, void** f, void* f_next, void** l)
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
#define sll_queue_pop_nz(nil, f, l, next) sll__queue_pop_nz(nil, &f, f->next, &l)
#else
#define sll_queue_pop_nz(nil, f, l, next) \
(                                         \
	(f) == (l) ? (                        \
		set_nil(nil,f),                   \
		set_nil(nil,l)                    \
	)                                     \
	: (                                   \
		(f)=(f)->next                     \
	)                                     \
)
// ! MD_LINKED_LIST_PURE_MACRO
#endif
// sll_queue_pop_nz
#endif

//- rjf: singly-linked, singly-headed lists (stacks)

#ifndef sll_stack_push_n
#define sll_stack_push_n(f,n,next) ( (n)->next = (f), (f) = (n) )
#endif
#ifndef sll_stack_pop_n
#define sll_stack_pop_n(f,next)    ( (f) = (f)->next )
#endif

//- rjf: doubly-linked-list helpers

#ifndef dll_insert_np
#define dll_insert_np(f, l, p, n, next, prev)  dll_insert_npz    (0, f, l, p, n,    next, prev)
#endif
#ifndef dll_push_back_np
#define dll_push_back_np(f, l, n, next, prev)  dll_push_back_npz (0, f, l, n, next,       prev)
#endif
#ifndef dll_push_front_np
#define dll_push_front_np(f, l, n, next, prev) dll_push_front_npz(0, f, l, n, next,       prev)
#endif
#ifndef dll_remove_np
#define dll_remove_np(f, l, n, next, prev)     dll_remove_npz    (0, f, l, n, next,       prev)
#endif
#ifndef dll_insert
#define dll_insert(f, l, p, n)                 dll_insert_npz    (0, f, l, p, n,    next, prev)
#endif
#ifndef dll_push_back
#define dll_push_back(f, l, n)                 dll_push_back_npz (0, f, l, n, next, prev)
#endif
#ifndef dll_push_front
#define dll_push_front(f, l, n)                dll_push_front_npz(0, f, l, n, next, prev)
#endif
#ifndef dll_remove
#define dll_remove(f, l, n)                    dll_remove_npz    (0, f, l, n, next, prev)
#endif

//- rjf: singly-linked, doubly-headed list helpers

#ifndef sll_queue_push_n
#define sll_queue_push_n(f, l, n, next)       sll_queue_push_nz      (0, f, l, n, next)
#endif
#ifndef sll_queue_push_front_n
#define sll_queue_push_front_n(f, l, n, next) sll_queue_push_front_nz(0, f, l, n, next)
#endif
#ifndef sll_queue_pop_n
#define sll_queue_pop_n(f, l, next)           sll_queue_pop_nzs      (0, f, l,    next)
#endif
#ifndef sll_queue_push
#define sll_queue_push(f, l, n)               sll_queue_push_nz      (0, f, l, n, next)
#endif
#ifndef sll_queue_push_front
#define sll_queue_push_front(f, l ,n)         sll_queue_push_front_nz(0, f, l, n, next)
#endif
#ifndef sll_queue_pop
#define sll_queue_pop(f, l)                   sll_queue_pop_nz       (0, f, l,    next)
#endif

//- rjf: singly-linked, singly-headed list helpers

#ifndef sll_stack_push
#define sll_stack_push(f, n) sll_stack_push_n(f, n, next)
#endif
#ifndef sll_stack_pop
#define sll_stack_pop(f)     sll_stack_pop_n (f,    next)
#endif

////////////////////////////////
//~ rjf: Address Sanitizer Markup

#ifndef NO_ASAN
#	if COMPILER_MSVC
#		if defined(__SANITIZE_ADDRESS__)
#			define ASAN_ENABLED 1
#	 		define NO_ASAN __declspec(no_sanitize_address)
#		else
#	 		define NO_ASAN
#		endif
#	elif COMPILER_CLANG
#		if defined(__has_feature)
#	 		if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#	  		define ASAN_ENABLED 1
#	 		endif
#		endif
#		define NO_ASAN __attribute__((no_sanitize("address")))
#	else
#		define NO_ASAN
#	endif
#endif

#ifndef asan_poison_memory_region
#	if MD_ASAN_ENABLED
#		pragma comment(lib, "clang_rt.asan-x86_64.lib")
		MD_C_API void __asan_poison_memory_region(void const volatile *addr, size_t size);
		MD_C_API void __asan_unpoison_memory_region(void const volatile *addr, size_t size);

#		define asan_poison_memory_region(addr, size)   __asan_poison_memory_region((addr), (size))
#		define asan_unpoison_memory_region(addr, size) __asan_unpoison_memory_region((addr), (size))
#	else
#		define asan_poison_memory_region(addr, size)   ((void)(addr), (void)(size))
#		define asan_unpoison_memory_region(addr, size) ((void)(addr), (void)(size))
#	endif
#endif

////////////////////////////////
//~ rjf: Misc. Helper Macros

#ifndef array_count
#define array_count(a) (sizeof(a) / sizeof((a)[0]))
#endif

#ifndef ceil_integer_div
#define ceil_integer_div(a,b) (((a) + (b) - 1) / (b))
#endif

#ifndef swap
#define swap(T, a, b) do { T t__ = a; a = b; b = t__; } while(0)
#endif

#ifndef int_from_ptr
#	if ARCH_64BIT
#		define int_from_ptr(ptr) ((U64)(ptr))
#	elif ARCH_32BIT
#		define int_from_ptr(ptr) ((U32)(ptr))
#	else
#		error Missing pointer-to-integer cast for this architecture.
#	endif
#endif

#ifndef ptr_from_int
#define ptr_from_int(i) (void*)((U8*)0 + (i))
#endif

#ifndef compose_64bit
#define compose_64bit(a,b)    ((((U64)a) << 32) | ((U64)b));
#endif
#ifndef align_pow2
#define align_pow2(x,b)      (((x) + (b) - 1) & ( ~((b) - 1)))
#endif
#ifndef align_down_pow2
#define align_down_pow2(x,b) ((x) & (~((b) - 1)))
#endif
#ifndef align_pad_pow2
#define align_pad_pow2(x,b)  ((0-(x)) & ((b) - 1))
#endif
#ifndef is_pow2
#define is_pow2(x)           ((x) != 0 && ((x ) & ((x) - 1)) == 0)
#endif
#ifndef is_pow2_or_zero
#define is_pow2_or_zero(x)   ((((x) - 1) & (x)) == 0)
#endif

#ifndef extract_bit
#define extract_bit(word, idx) (((word) >> (idx)) & 1)
#endif

#ifndef zero_struct
#	if LANG_CPP
#		define zero_struct {}
#	else
#		define zero_struct {0}
#	endif
#endif

#ifndef this_function_name
#	if COMPILER_MSVC && COMPILER_MSVC_YEAR < 2015
#		define this_function_name "unknown"
#	else
#		define this_function_name __func__
#	endif
#endif

#ifndef read_only
#	if COMPILER_MSVC || (COMPILER_CLANG && OS_WINDOWS)
#		pragma section(".rdata$", read)
#		define read_only __declspec(allocate(".rdata$"))
#	elif (COMPILER_CLANG && OS_LINUX)
#		define read_only __attribute__((section(".rodata")))
#	else
		// NOTE(rjf): I don't know of a useful way to do this in GCC land.
		// __attribute__((section(".rodata"))) looked promising, but it introduces a
		// strange warning about malformed section attributes, and it doesn't look
		// like writing to that section reliably produces access violations, strangely
		// enough. (It does on Clang)
#		define read_only
#	endif
#endif

#ifndef local_persist
#define local_persist static
#endif

#if COMPILER_MSVC
#	define thread_static __declspec(thread)
#elif COMPILER_CLANG || COMPILER_GCC
#	define thread_static __thread
#endif

#if COMPILER_CPP
// Already Defined
#elif COMPILER_C && __STDC_VERSION__ >= 201112L
#	define thread_local _Thread_local
#elif COMPILER_MSVC
#	define thread_local __declspec(thread)
#elif COMPILER_CLANG
#	define thread_local __thread
#else
#	error "No thread local support"
#endif


////////////////////////////////
//~ rjf: Safe Casts

inline U16
safe_cast_u16(U32 x) {
	assert_always(x <= MAX_U16);
	U16 result = (U16)x;
	return result;
}

inline U32
safe_cast_u32(U64 x) {
	assert_always(x <= MAX_U32);
	U32 result = (U32)x;
	return result;
}

inline S32
safe_cast_s32(S64 x) {
	assert_always(x <= MAX_S32);
	S32 result = (S32)x;
	return result;
}

////////////////////////////////
//~ rjf: Large Base Type Functions

inline U128 u128_zero (void)           { U128 v = {0};      return v; }
inline U128 u128_make (U64 v0, U64 v1) { U128 v = {v0, v1}; return v; }
inline B32  u128_match(U128 a, U128 b) { return memory_match_struct(&a, &b); }

////////////////////////////////
//~ rjf: Bit Patterns

inline U32 u32_from_u64_saturate(U64 x) { U32 x32 = (x > MAX_U32) ? MAX_U32 : (U32)x; return(x32); }

inline U64
u64_up_to_pow2(U64 x) {
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

inline S32
extend_sign32(U32 x, U32 size) {
  U32 high_bit = size * 8;
  U32 shift    = 32 - high_bit;
  S32 result   = ((S32)x << shift) >> shift;
  return result;
}

inline S64
extend_sign64(U64 x, U64 size) {
  U64 high_bit = size * 8;
  U64 shift    = 64 - high_bit;
  S64 result   = ((S64)x << shift) >> shift;
  return result;
}

inline F32 inf32    (void) { union { U32 u; F32 f; } x; x.u = EXPONENT32;          return(x.f); }
inline F32 neg_inf32(void) { union { U32 u; F32 f; } x; x.u = SIGN32 | EXPONENT32; return(x.f); }

inline U16
bswap_u16(U16 x)
{
  U16 result = (((x & 0xFF00) >> 8) |
                ((x & 0x00FF) << 8));
  return result;
}

inline U32
bswap_u32(U32 x)
{
  U32 result = (((x & 0xFF000000) >> 24) |
                ((x & 0x00FF0000) >> 8)  |
                ((x & 0x0000FF00) << 8)  |
                ((x & 0x000000FF) << 24));
  return result;
}

inline U64
bswap_u64(U64 x)
{
  // TODO(nick): naive bswap, replace with something that is faster like an intrinsic
  U64 result = (((x & 0xFF00000000000000ULL) >> 56) |
                ((x & 0x00FF000000000000ULL) >> 40) |
                ((x & 0x0000FF0000000000ULL) >> 24) |
                ((x & 0x000000FF00000000ULL) >> 8)  |
                ((x & 0x00000000FF000000ULL) << 8)  |
                ((x & 0x0000000000FF0000ULL) << 24) |
                ((x & 0x000000000000FF00ULL) << 40) |
                ((x & 0x00000000000000FFULL) << 56));
  return result;
}

#if ARCH_LITTLE_ENDIAN
# define from_be_u16(x) bswap_u16(x)
# define from_be_u32(x) bswap_u32(x)
# define from_be_u64(x) bswap_u64(x)
#else
# define from_be_u16(x) (x)
# define from_be_u32(x) (x)
# define from_be_u64(x) (x)
#endif

#if COMPILER_MSVC || (COMPILER_CLANG && OS_WINDOWS)
	inline U64 count_bits_set16(U16 val) { return __popcnt16(val); }
	inline U64 count_bits_set32(U32 val) { return __popcnt  (val); }
	inline U64 count_bits_set64(U64 val) { return __popcnt64(val); }

	inline U64 ctz32(U32 mask) { unsigned long idx; _BitScanForward  (&idx, mask); return idx; }
	inline U64 ctz64(U64 mask) { unsigned long idx; _BitScanForward64(&idx, mask); return idx; }
	inline U64 clz32(U32 mask) { unsigned long idx; _BitScanReverse  (&idx, mask); return 31 - idx; }
	inline U64 clz64(U64 mask) { unsigned long idx; _BitScanReverse64(&idx, mask); return 63 - idx; }
#elif COMPILER_CLANG || COMPILER_GCC
	inline U64 count_bits_set16(U16 val) { NotImplemented; return 0; }
	inline U64 count_bits_set32(U32 val) { NotImplemented; return 0; }
	inline U64 count_bits_set64(U64 val) { NotImplemented; return 0; }

	inline U64 ctz32(U32 val) { NotImplemented; return 0; }
	inline U64 ctz64(U32 val) { NotImplemented; return 0; }
	inline U64 clz32(U32 val) { NotImplemented; return 0; }
	inline U64 clz64(U64 val) { NotImplemented; return 0; }
#else
#	error "Bit intrinsic functions not defined for this compiler."
#endif
