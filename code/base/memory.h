#ifdef MD_INTELLISENSE_DIRECTIVES
#pragma once
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
# define align_of(T) __alignof(T)
#elif MD_COMPILER_CLANG
# define align_of(T) __alignof(T)
#elif MD_COMPILER_GCC
# define align_of(T) __alignof__(T)
#else
# error AlignOf not defined for this compiler.
#endif

////////////////////////////////
//~ rjf: Member Offsets

#define member(T, m)                    ( ((T*) 0)->m )
#define offset_of(T, m)                 int_from_ptr(& member(T, m))
#define member_from_offset(T, ptr, off) (T)  ((((U8 *) ptr) + (off)))
#define cast_from_member(T, m, ptr)     (T*) (((U8*)ptr) - offset_of(T, m))

////////////////////////////////
//~ rjf: For-Loop Construct Macros

#define defer_loop(begin, end)         for(int _i_ =       ((begin), 0); ! _i_;                           _i_ += 1, (end))
#define defer_loop_checked(begin, end) for(int _i_ = 2 * ! (begin);       (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))

#define each_enum_val(type, it)          type it = (type) 0; it < type ## _COUNT; it = (type)( it + 1 )
#define each_non_zero_enum_val(type, it) type it = (type) 1; it < type ## _COUNT; it = (type)( it + 1 )

////////////////////////////////
//~ rjf: Memory Operation Macros

#define memory_copy(dst, src, size)    memmove((dst), (src), (size))
#define memory_set(dst, byte, size)    memset((dst), (byte), (size))
#define memory_compare(a, b, size)     memcmp((a), (b), (size))
#define memory_str_len(ptr)            cstr_len(ptr)

#define memory_copy_struct(d,s)   memory_copy((d),(s),sizeof(*(d)))
#define memory_copy_array(d,s)    memory_copy((d),(s),sizeof(d))
#define memory_copy_type(d,s,c)   memory_copy((d),(s),sizeof(*(d))*(c))

#define memory_zero(s,z)       mem_set((s),0,(z))
#define memory_zero_struct(s)  memory_zero((s),sizeof(*(s)))
#define memory_zero_array(a)   memroy_zero((a),sizeof(a))
#define memory_zero_type(m,c)  memroy_zero((m),sizeof(*(m))*(c))

#define memory_match(a,b,z)      (memory_compare((a),(b),(z)) == 0)
#define memory_match_struct(a,b) memory_match((a),(b),sizeof(*(a)))
#define memory_match_array(a,b)  memory_match((a),(b),sizeof(a))

#define memory_read(T,p,e)    ( ((p)+sizeof(T)<=(e))?(*(T*)(p)):(0) )
#define memory_consume(T,p,e) ( ((p)+sizeof(T)<=(e))?((p)+=sizeof(T),*(T*)((p)-sizeof(T))):((p)=(e),0) )


