#ifdef MD_INTELLISENSE_DIRECTIVES
#pragma once
#include "cracking_arch.h"
#include "macros.h"
#endif

////////////////////////////////
//~ rjf: Units

#define KB(n)  (((U64)(n)) << 10)
#define MB(n)  (((U64)(n)) << 20)
#define GB(n)  (((U64)(n)) << 30)
#define TB(n)  (((U64)(n)) << 40)

#define thousand(n)   ((n) * 1000)
#define million(n)    ((n) * 1000000)
#define billion(n)    ((n) * 1000000000)

////////////////////////////////
//~ rjf: Clamps, Mins, Maxes

#define min(A,B) (((A) < (B)) ? (A) : (B))
#define max(A,B) (((A) > (B)) ? (A) : (B))

#define clamp_top(A,X) Min(A, X)
#define clamp_bot(X,B) Max(X, B)

#define clamp(A,X,B) (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))

////////////////////////////////
//~ rjf: Type -> Alignment

#if MD_COMPILER_MSVC
#	define align_of(T) __alignof(T)
#elif MD_COMPILER_CLANG
#	define align_of(T) __alignof(T)
#elif MD_COMPILER_GCC
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

#if MD_COMPILER_MSVC
#	define trap() __debugbreak()
#elif MD_COMPILER_CLANG || COMPILER_GCC
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

#if MD_OS_WINDOWS

// #	include <windows.h>
// #	include <tmmintrin.h>
// #	include <wmmintrin.h>
// #	include <intrin.h>

#	if MD_ARCH_X64
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
#elif MD_OS_LINUX
#	if MD_ARCH_X64
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
#define dll_insert_npz(nil, f, l, p, n, next, prev) (                      \
	check_nil(nil, f) ? (                                                  \
			(f) = (l) = (n),                                               \
			set_nil(nil, (n)->next),                                       \
			set_nil(nil, (n)->prev)                                        \
		)                                                                  \
	: (                                                                    \
		check_nil(nil,p) ? (                                               \
			(n)->next = (f),                                               \
			(f)->prev = (n),                                               \
			(f) = (n),                                                     \
			set_nil(nil,(n)->prev)                                         \
		)                                                                  \
		: ((p) == (l)) ? (                                                 \
					(l)->next = (n),                                       \
					(n)->prev = (l),                                       \
					(l) = (n),                                             \
					set_nil(nil, (n)->next)                                \
				)                                                          \
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
)                                                                          \

#define dll_push_back_npz(nil, f, l, n, next, prev)  dll_insert_npz(nil, f, l, l, n, next, prev)
#define dll_push_front_npz(nil, f, l, n, next, prev) dll_insert_npz(nil, l, f, f, n, prev, next)
#define dll_remove_npz(nil,f,l,n,next,prev) \
(                                           \
	(                                       \
		(n) == (f) ?                        \
			(f) = (n)->next                 \
		: 	(0)                             \
	),                                      \
	(                                       \
		(n) == (l) ?                        \
			(l) = (l)->prev                 \
		:	(0)                             \
	),                                      \
	(                                       \
		CheckNil(nil,(n)->prev) ?           \
			(0)                             \
		: 	((n)->prev->next = (n)->next)   \
	),                                      \
	(                                       \
		CheckNil(nil,(n)->next) ?           \
			(0)                             \
		:	((n)->next->prev = (n)->prev)   \
	)                                       \
)

//- rjf: singly-linked, doubly-headed lists (queues)
#define SLLQueuePush_NZ(nil,f,l,n,next) ( \
	CheckNil(nil,f)?\
((f)=(l)=(n),SetNil(nil,(n)->next)):\
((l)->next=(n),(l)=(n),SetNil(nil,(n)->next)))
#define SLLQueuePushFront_NZ(nil,f,l,n,next) (CheckNil(nil,f)?\
((f)=(l)=(n),SetNil(nil,(n)->next)):\
((n)->next=(f),(f)=(n)))
#define SLLQueuePop_NZ(nil,f,l,next) ((f)==(l)?\
(SetNil(nil,f),SetNil(nil,l)):\
((f)=(f)->next))

//- rjf: singly-linked, singly-headed lists (stacks)
#define sll_stack_push_n(f,n,next) ( (n)->next = (f), (f) = (n) )
#define sll_stack_pop_n(f,next)    ( (f) = (f)->next )

//- rjf: doubly-linked-list helpers
#define dll_insert_np(f, l, p, n, next, prev)  DLLInsert_NPZ   (0, f, l, p, n,    next, prev)
#define dll_push_back_np(f, l, n, next, prev)  DLLPushBack_NPZ (0, f, l, n, next,       prev)
#define dll_push_front_np(f, l, n, next, prev) DLLPushFront_NPZ(0, f, l, n, next,       prev)
#define dll_remove_np(f, l, n, next, prev)     DLLRemove_NPZ   (0, f, l, n, next,       prev)
#define dll_insert(f, l, p, n)                 DLLInsert_NPZ   (0, f, l, p, n,    next, prev)
#define dll_push_back(f, l, n)                 DLLPushBack_NPZ (0, f, l, n, next, prev)
#define dll_push_front(f, l, n)                DLLPushFront_NPZ(0, f, l, n, next, prev)
#define dll_remove(f, l, n)                    DLLRemove_NPZ   (0, f, l, n, next, prev)

//- rjf: singly-linked, doubly-headed list helpers
#define SLLQueuePush_N(f,l,n,next) SLLQueuePush_NZ(0,f,l,n,next)
#define SLLQueuePushFront_N(f,l,n,next) SLLQueuePushFront_NZ(0,f,l,n,next)
#define SLLQueuePop_N(f,l,next) SLLQueuePop_NZ(0,f,l,next)
#define SLLQueuePush(f,l,n) SLLQueuePush_NZ(0,f,l,n,next)
#define SLLQueuePushFront(f,l,n) SLLQueuePushFront_NZ(0,f,l,n,next)
#define SLLQueuePop(f,l) SLLQueuePop_NZ(0,f,l,next)

//- rjf: singly-linked, singly-headed list helpers
#define SLLStackPush(f,n) SLLStackPush_N(f,n,next)
#define SLLStackPop(f) SLLStackPop_N(f,next)

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

#if ASAN_ENABLED
#pragma comment(lib, "clang_rt.asan-x86_64.lib")
MD_C_API void __asan_poison_memory_region(void const volatile *addr, size_t size);
MD_C_API void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
# define AsanPoisonMemoryRegion(addr, size)   __asan_poison_memory_region((addr), (size))
# define AsanUnpoisonMemoryRegion(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
# define AsanPoisonMemoryRegion(addr, size)   ((void)(addr), (void)(size))
# define AsanUnpoisonMemoryRegion(addr, size) ((void)(addr), (void)(size))
#endif

////////////////////////////////
//~ rjf: Misc. Helper Macros

#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)

#define Glue_(A,B) A##B
#define Glue(A,B) Glue_(A,B)

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define CeilIntegerDiv(a,b) (((a) + (b) - 1)/(b))

#define Swap(T,a,b) do{T t__ = a; a = b; b = t__;}while(0)

#if ARCH_64BIT
# define IntFromPtr(ptr) ((U64)(ptr))
#elif ARCH_32BIT
# define IntFromPtr(ptr) ((U32)(ptr))
#else
# error Missing pointer-to-integer cast for this architecture.
#endif
#define PtrFromInt(i) (void*)((U8*)0 + (i))

#define Compose64Bit(a,b)  ((((U64)a) << 32) | ((U64)b));
#define AlignPow2(x,b)     (((x) + (b) - 1)&(~((b) - 1)))
#define AlignDownPow2(x,b) ((x)&(~((b) - 1)))
#define AlignPadPow2(x,b)  ((0-(x)) & ((b) - 1))
#define IsPow2(x)          ((x)!=0 && ((x)&((x)-1))==0)
#define IsPow2OrZero(x)    ((((x) - 1)&(x)) == 0)

#define ExtractBit(word, idx) (((word) >> (idx)) & 1)

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


