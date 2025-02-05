#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "platform.h"
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
//~ rjf: Clamps, Mins, Maxes

#ifndef min
#define min(A,B) (((A) < (B)) ? (A) : (B))
#endif
#ifndef max
#define max(A,B) (((A) > (B)) ? (A) : (B))
#endif

#ifndef clamp_top
#define clamp_top(A,X) Min(A, X)
#endif
#ifndef clamp_bot
#define clamp_bot(X,B) Max(X, B)
#endif

#define clamp(A,X,B) (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))

////////////////////////////////
//~ rjf: Type -> Alignment

#if COMPILER_MSVC
#	define align_of(T) __alignof(T)
#elif COMPILER_CLANG
#	define align_of(T) __alignof(T)
#elif COMPILER_GCC
#	define align_of(T) __alignof__(T)
#else
#	error AlignOf not defined for this compiler.
#endif

////////////////////////////////
//~ rjf: Member Offsets

#define member(T, m)                    ( ((T*) 0)->m )
#define offset_of(T, m)                 int_from_ptr(& member(T, m))
#define member_from_offset(T, ptr, off) (T)  ((((U8 *) ptr) + (off)))
#define cast_from_member(T, m, ptr)     (T*) (((U8*)ptr) - offset_of(T, m))

////////////////////////////////
//~ rjf: For-Loop Construct Macros

#define defer_loop(begin, end)           for (int _i_ =       ((begin), 0); ! _i_;                           _i_ += 1, (end))
#define defer_loop_checked(begin, end)   for (int _i_ = 2 * ! (begin);       (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))

#define each_enum_val(type, it)          type it = (type) 0; it < type ## _COUNT; it = (type)( it + 1 )
#define each_non_zero_enum_val(type, it) type it = (type) 1; it < type ## _COUNT; it = (type)( it + 1 )

////////////////////////////////
//~ rjf: Memory Operation Macros

#define memory_copy(dst, src, size)    memmove((dst), (src), (size))
#define memory_set(dst, byte, size)    memset((dst), (byte), (size))
#define memory_compare(a, b, size)     memcmp((a), (b), (size))
#define memory_str_len(ptr)            cstr_len(ptr)

#define memory_copy_struct(d,s)        memory_copy((d), (s), sizeof( *(d)))
#define memory_copy_array(d,s)         memory_copy((d), (s), sizeof( d))
#define memory_copy_type(d,s,c)        memory_copy((d), (s), sizeof( *(d)) * (c))

#define memory_zero(s,z)               mem_set((s), 0, (z))
#define memory_zero_struct(s)          memory_zero((s), sizeof( *(s)))
#define memory_zero_array(a)           memroy_zero((a), sizeof(a))
#define memory_zero_type(m,c)          memroy_zero((m), sizeof( *(m)) * (c))

#define memory_match(a,b,z)            (memory_compare((a), (b), (z)) == 0)
#define memory_match_struct(a,b)       memory_match((a), (b), sizeof(*(a)))
#define memory_match_array(a,b)        memory_match((a), (b), sizeof(a))

#define memory_read(T,p,e)            ( ((p) + sizeof(T) <= (e)) ? ( *(T*)(p)) : (0) )
#define memory_consume(T,p,e)         ( ((p) + sizeof(T) <= (e)) ? ((p) += sizeof(T), *(T*)((p) - sizeof(T))) : ((p) = (e),0) )

////////////////////////////////
//~ rjf: Asserts

#if COMPILER_MSVC
#	define trap() __debugbreak()
#elif COMPILER_CLANG || COMPILER_GCC
#	define trap() __builtin_trap()
#else
#	error Unknown trap intrinsic for this compiler.
#endif

#define assert_always(x) do { if ( !(x) ) { trap(); } } while(0)

#if BUILD_DEBUG
#	define assert(x) assert_always(x)
#else
#	define assert(x) (void)(x)
#endif

#define invalid_path            assert( ! "Invalid Path!")
#define not_implemented         assert( ! "Not Implemented!")
#define no_op                   ((void)0)
#define md_static_assert(C, ID) global U8 glue(ID, __LINE__)[ (C) ? 1 : -1 ]

////////////////////////////////
//~ rjf: Atomic Operations

#if OS_WINDOWS
#	if ARCH_X64
#		define ins_atomic_u64_eval(x)                 InterlockedAdd64((volatile __int64 *)(x), 0)
#		define ins_atomic_u64_inc_eval(x)             InterlockedIncrement64((volatile __int64 *)(x))
#		define ins_atomic_u64_dec_eval(x)             InterlockedDecrement64((volatile __int64 *)(x))
#		define ins_atomic_u64_eval_assign(x,c)        InterlockedExchange64((volatile __int64 *)(x), (c))
#		define ins_atomic_u64_add_eval(x,c)           InterlockedAdd64((volatile __int64 *)(x), c)
#		define ins_atomic_u64_eval_cond_assign(x,k,c) InterlockedCompareExchange64((volatile __int64 *)(x), (k), (c))
#		define ins_atomic_u32_eval(x,c)               InterlockedAdd((volatile LONG *)(x), 0)
#		define ins_atomic_u32_eval_assign(x,c)        InterlockedExchange((volatile LONG *)(x), (c))
#		define ins_atomic_u32_eval_cond_assign(x,k,c) InterlockedCompareExchange((volatile LONG *)(x), (k), (c))
#		define ins_atomic_ptr_eval_assign(x,c)        (void*) ins_atomic_u64_eval_assign((volatile __int64 *)(x), (__int64)(c))
#	else
#		error Atomic intrinsics not defined for this operating system / architecture combination.
#	endif
#elif OS_LINUX
#	if ARCH_X64
#		define ins_atomic_u64_inc_eval(x) __sync_fetch_and_add((volatile U64 *)(x), 1)
#	else
#		error Atomic intrinsics not defined for this operating system / architecture combination.
#	endif
#else
#	error Atomic intrinsics not defined for this operating system.
#endif

////////////////////////////////
//~ rjf: Linked List Building Macros

//- rjf: linked list macro helpers

#define check_nil(nil,p) ((p) == 0 || (p) == nil)
#define set_nil(nil,p)   ((p) = nil)

//- rjf: doubly-linked-lists

// insert next-previous with nil
#define dll_insert_npz(nil, f, l, p, n, next, prev) (                      \
	check_nil(nil, f) ? (                                                  \
		(f) = (l) = (n),                                                   \
		set_nil(nil, (n)->next),                                           \
		set_nil(nil, (n)->prev)                                            \
	)                                                                      \
	: (                                                                    \
		check_nil(nil,p) ? (                                               \
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
// push-back next-previous with nil
#define dll_push_back_npz(nil, f, l, n, next, prev)  dll_insert_npz(nil, f, l, l, n, next, prev)
// push-fornt next-previous with nil
#define dll_push_front_npz(nil, f, l, n, next, prev) dll_insert_npz(nil, l, f, f, n, prev, next)
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

//- rjf: singly-linked, doubly-headed lists (queues)

// queue-push next with nil
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
// queue-push-front next with nil
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
// queue-pop next with nil
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

//- rjf: singly-linked, singly-headed lists (stacks)

#define sll_stack_push_n(f,n,next) ( (n)->next = (f), (f) = (n) )
#define sll_stack_pop_n(f,next)    ( (f) = (f)->next )

//- rjf: doubly-linked-list helpers

#define dll_insert_np(f, l, p, n, next, prev)  dll_insert_npz    (0, f, l, p, n,    next, prev)
#define dll_push_back_np(f, l, n, next, prev)  dll_push_back_npz (0, f, l, n, next,       prev)
#define dll_push_front_np(f, l, n, next, prev) dll_push_front_npz(0, f, l, n, next,       prev)
#define dll_remove_np(f, l, n, next, prev)     dll_remove_npz    (0, f, l, n, next,       prev)
#define dll_insert(f, l, p, n)                 dll_insert_npz    (0, f, l, p, n,    next, prev)
#define dll_push_back(f, l, n)                 dll_push_back_npz (0, f, l, n, next, prev)
#define dll_push_front(f, l, n)                dll_push_front_npz(0, f, l, n, next, prev)
#define dll_remove(f, l, n)                    dll_remove_npz    (0, f, l, n, next, prev)

//- rjf: singly-linked, doubly-headed list helpers

#define sll_queue_push_n(f, l, n, next)       sll_queue_push_nz      (0, f, l, n, next)
#define sll_queue_push_fornt_n(f, l, n, next) sll_queue_push_front_nz(0, f, l, n, next)
#define sll_queue_pop_n(f, l, next)           sll_queue_pop_nzs      (0, f, l,    next)
#define sll_queue_push(f, l, n)               sll_queue_push_nz      (0, f, l, n, next)
#define sll_queue_push_front(f, l ,n)         sll_queue_push_front_nz(0, f, l, n, next)
#define sll_queue_pop(f, l)                   sll_queue_pop_nz       (0, f, l,    next)

//- rjf: singly-linked, singly-headed list helpers

#define sll_stack_push(f, n) sll_stack_push_n(f, n, next)
#define sll_stack_pop(f)     sll_stack_pop_n (f,    next)

////////////////////////////////
//~ rjf: Address Sanitizer Markup

#if COMPILER_MSVC
# if defined(__SANITIZE_ADDRESS__)
#  define ASAN_ENABLED 1
#  define NO_ASAN __declspec(no_sanitize_address)
# else
#  define NO_ASAN
# endif
#elif COMPILER_CLANG
# if defined(__has_feature)
#  if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#   define ASAN_ENABLED 1
#  endif
# endif
# define NO_ASAN __attribute__((no_sanitize("address")))
#else
# define NO_ASAN
#endif

#if MD_ASAN_ENABLED
#	pragma comment(lib, "clang_rt.asan-x86_64.lib")
	MD_C_API void __asan_poison_memory_region(void const volatile *addr, size_t size);
	MD_C_API void __asan_unpoison_memory_region(void const volatile *addr, size_t size);

#	define asan_poison_memory_region(addr, size)   __asan_poison_memory_region((addr), (size))
#	define asan_unpoison_memory_region(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
#	define asan_poison_memory_region(addr, size)   ((void)(addr), (void)(size))
#	define asan_unpoison_memory_region(addr, size) ((void)(addr), (void)(size))
#endif

////////////////////////////////
//~ rjf: Misc. Helper Macros

#ifndef stringify
#define stringify_(S) #S
#define stringify(S)  stringify_(S)
#endif

#ifndef glue
#define glue_(A,B) A ## B
#define glue(A,B)  glue_(A,B)
#endif

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

#define ptr_from_int(i) (void*)((U8*)0 + (i))

#define compose_64bit(a,b)    ((((U64)a) << 32) | ((U64)b));
#define align_pow_2(x,b)      (((x) + (b) - 1)&( ~((b) - 1)))
#define align_down_pow_2(x,b) ((x) & (~((b) - 1)))
#define align_pad_pow_2(x,b)  ((0-(x)) & ((b) - 1))
#define is_pow_2(x)           ((x) != 0 && ((x )& ((x) - 1)) == 0)
#define is_pow_2_or_zero(x)   ((((x) - 1) & (x)) == 0)

#define extract_bit(word, idx) (((word) >> (idx)) & 1)

#if LANG_CPP
# define zero_struct {}
#else
# define zero_struct {0}
#endif

#if COMPILER_MSVC && COMPILER_MSVC_YEAR < 2015
# define this_function_name "unknown"
#else
# define this_function_name __func__
#endif

#if COMPILER_MSVC || (COMPILER_CLANG && OS_WINDOWS)
#	pragma section(".rdata$", read)
#	define read_only __declspec(allocate(".rdata$"))
#elif (COMPILER_CLANG && OS_LINUX)
#	define read_only __attribute__((section(".rodata")))
#else
// NOTE(rjf): I don't know of a useful way to do this in GCC land.
// __attribute__((section(".rodata"))) looked promising, but it introduces a
// strange warning about malformed section attributes, and it doesn't look
// like writing to that section reliably produces access violations, strangely
// enough. (It does on Clang)
#	define read_only
#endif
