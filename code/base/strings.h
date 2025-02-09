#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "platform.h"
#	include "macros.h"
#	include "generic_macros.h"
#	include "base_types.h"
#	include "arena.h"
#	include "space.h"
#	include "math.h"
#	include "toolchain.h"
#	include "time.h"
////////////////////////////////
//~ rjf: Third Party Includes
#	define STB_SPRINTF_DECORATE(name) md_##name
#	include "third_party/stb/stb_sprintf.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: String Types

typedef struct String8 String8;
struct String8
{
	U8* str;
	U64 size;
};

typedef struct String16 String16;
struct String16
{
	U16* str;
	U64  size;
};

typedef struct String32 String32;
struct String32
{
	U32* str;
	U64  size;
};

////////////////////////////////
//~ rjf: String List & Array Types

typedef struct String8Node String8Node;
struct String8Node
{
	String8Node* next;
	String8      string;
};

typedef struct String8MetaNode String8MetaNode;
struct String8MetaNode
{
	String8MetaNode* next;
	String8Node*     node;
};

typedef struct String8List String8List;
struct String8List
{
	String8Node* first;
	String8Node* last;
	U64 node_count;
	U64 total_size;
};

typedef struct String8Array String8Array;
struct String8Array
{
	String8* v;
	U64      count;
};

////////////////////////////////
//~ rjf: String Matching, Splitting, & Joining Types

typedef U32 StringMatchFlags;
enum
{
	StringMatchFlag_CaseInsensitive  = (1 << 0),
	StringMatchFlag_RightSideSloppy  = (1 << 1),
	StringMatchFlag_SlashInsensitive = (1 << 2),
};

typedef U32 StringSplitFlags;
enum
{
	StringSplitFlag_KeepEmpties = (1 << 0),
};

typedef enum PathStyle
{
	PathStyle_Relative,
	PathStyle_WindowsAbsolute,
	PathStyle_UnixAbsolute,
  
#if OS_WINDOWS
	PathStyle_SystemAbsolute = PathStyle_WindowsAbsolute
#elif OS_LINUX
	PathStyle_SystemAbsolute = PathStyle_UnixAbsolute
#else
#	error "absolute path style is undefined for this OS"
#endif
}
PathStyle;

typedef struct StringJoin StringJoin;
struct StringJoin
{
	String8 pre;
	String8 sep;
	String8 post;
};

////////////////////////////////
//~ rjf: UTF Decoding Types

typedef struct UnicodeDecode UnicodeDecode;
struct UnicodeDecode
{
	U32 inc;
	U32 codepoint;
};

////////////////////////////////
//~ rjf: String Fuzzy Matching Types

typedef struct FuzzyMatchRangeNode FuzzyMatchRangeNode;
struct FuzzyMatchRangeNode
{
	FuzzyMatchRangeNode* next;
	Rng1U64 range;
};

typedef struct FuzzyMatchRangeList FuzzyMatchRangeList;
struct FuzzyMatchRangeList
{
	FuzzyMatchRangeNode* first;
	FuzzyMatchRangeNode* last;
	U64 count;
	U64 needle_part_count;
	U64 total_dim;
};

////////////////////////////////
//~ NOTE(allen): String <-> Integer Tables

inline U8
integer_symbols(U8 value) {
	read_only local_persist
	U8 lookup_table[16] = {
		'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',
	};
	return lookup_table[value];
}

inline U8
base64(U8 value) {
	read_only local_persist
	U8 lookup_table[64] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
		'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'_', '$',
	};
	return lookup_table[value];
}

inline U8
base64_reverse(U8 value) {
	read_only local_persist
	U8 lookup_table[128] = {
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
		0xFF,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,
		0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0xFF,0xFF,0xFF,0xFF,0x3E,
		0xFF,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
		0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0xFF,0xFF,0xFF,0xFF,0xFF,
	};
	return lookup_table[value];
}

// NOTE(allen): Includes reverses for uppercase and lowercase hex.
inline U8
integer_symbol_reverse(U8 value) {
	read_only local_persist
	lookup_table[128] = {
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	};
	return lookup_table[value];
}

////////////////////////////////
//~ rjf: Character Classification & Conversion Functions

inline B32 char_is_space        (U8 c) { return(c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v'); }
inline B32 char_is_upper        (U8 c) { return('A' <= c && c <= 'Z'); }
inline B32 char_is_lower        (U8 c) { return('a' <= c && c <= 'z'); }
inline B32 char_is_alpha        (U8 c) { return(char_is_upper(c) || char_is_lower(c)); }
inline B32 char_is_slash        (U8 c) { return(c == '/' || c == '\\'); }
inline U8  char_to_lower        (U8 c) { if (char_is_upper(c)) { c += ('a' - 'A'); } return(c); }
inline U8  char_to_upper        (U8 c) { if (char_is_lower(c)) { c += ('A' - 'a'); } return(c); }
inline U8  char_to_correct_slash(U8 c) { if (char_is_slash(c)) { c = '/';          } return(c); }

inline B32
char_is_digit(U8 c, U32 base) {
	B32 result = 0;
	if (0 < base && base <= 16) {
		U8 val = integer_symbol_reverse(c);
		if (val < base) {
			result = 1;
		}
	}
	return(result);
}

////////////////////////////////
//~ rjf: C-String Measurement

inline U64 cstring8_length (U8*  c) { U8*  p = c; for (; *p != 0; p += 1); return(p - c); }
inline U64 cstring16_length(U16* c) { U16* p = c; for (; *p != 0; p += 1); return(p - c); }
inline U64 cstring32_length(U32 *c) { U32* p = c; for (; *p != 0; p += 1); return(p - c); }

#define cstring_length(c) _Generic(c, U8: cstring8_length, U16: cstring16_length, U32: cstring32_length)(c)

////////////////////////////////
//~ rjf: String Constructors

#define str8_lit(S)      str8((U8*)(S), sizeof(S) - 1)
#define str8_lit_comp(S) { (U8*)(S), sizeof(S) - 1, }
#define str8_varg(S)     (int)((S).size), ((S).str)

#define str8_array(S, C)    str8((U8*)(S), sizeof(*(S)) * (C))
#define str8_array_fixed(S) str8((U8*)(S), sizeof(S))
#define str8_struct(S)      str8((U8*)(S), sizeof(*(S)))

inline String8  str8         (U8* str,   U64 size)            { String8  result = {str,   size};                         return(result); } 
inline String8  str8_range   (U8* first, U8* one_past_last)   { String8  result = {first, (U64)(one_past_last - first)}; return(result); }
inline String8  str8_zero    (void)                           { String8  result = {0};                                   return(result); }
inline String16 str16        (U16* str,   U64  size)          { String16 result = {str,   size};                         return(result); }
inline String16 str16_range  (U16* first, U16* one_past_last) { String16 result = {first, (U64)(one_past_last - first)}; return(result); }
inline String16 str16_zero   (void)                           { String16 result = {0};                                   return(result); }
inline String32 str32        (U32* str,   U64  size)          { String32 result = {str,   size};                         return(result); }
inline String32 str32_range  (U32* first, U32* one_past_last) { String32 result = {first, (U64)(one_past_last - first)}; return(result); }
inline String32 str32_zero   (void)                           { String32 result = {0};                                   return(result); }
inline String8  str8_cstring (char* c)                        { String8  result = {(U8*) c, cstring8_length ((U8*) c)};  return(result); }
inline String16 str16_cstring(U16*  c)                        { String16 result = {(U16*)c, cstring16_length((U16*)c)};  return(result); }
inline String32 str32_cstring(U32*  c)                        { String32 result = {(U32*)c, cstring32_length((U32*)c)};  return(result); }

#define str_range(str, one_past_last) _Generic(str, U8: str8_range,   U16: str16_range,   U32: str32_range  )(str, one_past_last)
#define str_cstring(c)                _Generic(c,   U8: str8_cstring, U16: str16_cstring, U32: str32_cstring)(c)

String8  str8_cstring_capped        (void* cstr,      void* cap);
String16 str16_cstring_capped       (void* cstr,      void* cap);
String8  str8_cstring_capped_reverse(void* raw_start, void* raw_cap);

inline String8
str8_cstring_capped(void* cstr, void* cap) {
	char* ptr = (char*)cstr;
	char* opl = (char*)cap;
	for (;ptr < opl && *ptr != 0; ptr += 1);
	U64     size   = (U64)(ptr - (char *)cstr);
	String8 result = {(U8*)cstr, size};
	return  result;
}

inline String16
str16_cstring_capped(void* cstr, void* cap)
{
	U16* ptr = (U16*)cstr;
	U16* opl = (U16*)cap;
	for (;ptr < opl && *ptr != 0; ptr += 1);
	U64      size   = (U64)(ptr - (U16 *)cstr);
	String16 result = str16(cstr, size);
	return   result;
}

inline String8
str8_cstring_capped_reverse(void* raw_start, void* raw_cap)
{
	U8* start = raw_start;
	U8* ptr   = raw_cap;
	for (; ptr > start; )
	{
		ptr -= 1;
		if (*ptr == '\0') { 
			break;
		}
	}
	U64     size    = (U64)(ptr - start);
	String8 result  = str8(start, size);
	return  result;
}

////////////////////////////////
//~ rjf: String Formatting & Copying

MD_API String8 push_str8_cat (Arena* arena, String8 s1, String8 s2);
MD_API String8 push_str8_copy(Arena* arena, String8 s);
MD_API String8 push_str8fv   (Arena* arena, char* fmt, va_list args);
       String8 push_str8f    (Arena* arena, char* fmt, ...);

MD_API String8 alloc_str8_cat (AllocatorInfo ainfo, String8 s1, String8 s2);
MD_API String8 alloc_str8_copy(AllocatorInfo ainfo, String8 s);
MD_API String8 alloc_str8fv   (AllocatorInfo ainfo, char* fmt, va_list args);
       String8 alloc_str8f    (AllocatorInfo ainfo, char* fmt, ...);

inline String8
push_str8f(Arena *arena, char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	String8 result = push_str8fv(arena, fmt, args);
	va_end(args);
	return(result);
}

inline String8
alloc_str8f(AllocatorInfo ainfo, char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	String8 result = alloc_str8fv(ainfo, fmt, args);
	va_end(args);
	return(result);
}

////////////////////////////////
//~ rjf: String Slicing

MD_API String8 str8_skip_chop_whitespace(String8 string);

String8 str8_substr (String8 str, Rng1U64 range);
String8 str8_prefix (String8 str, U64     size);
String8 str8_skip   (String8 str, U64     amt);
String8 str8_postfix(String8 str, U64     size);
String8 str8_chop   (String8 str, U64     amt);

inline String8
str8_substr(String8 str, Rng1U64 range){
	range.min = clamp_top(range.min, str.size);
	range.max = clamp_top(range.max, str.size);
	str.str  += range.min;
	str.size  = dim_1u64(range);
	return(str);
}

inline String8 
str8_prefix(String8 str, U64 size){
	str.size = clamp_top(size, str.size);
	return(str);
}

inline String8
str8_skip(String8 str, U64 amt){
	amt       = clamp_top(amt, str.size);
	str.str  += amt;
	str.size -= amt;
	return(str);
}

inline String8
str8_postfix(String8 str, U64 size){
	size     = clamp_top(size, str.size);
	str.str  = (str.str + str.size) - size;
	str.size = size;
	return(str);
}

inline String8
str8_chop(String8 str, U64 amt){
	amt       = clamp_top(amt, str.size);
	str.size -= amt;
	return(str);
}

////////////////////////////////
//~ rjf: String Stylization

inline String8 upper_from_str8      (Arena* arena, String8 string) { string = push_str8_copy(arena, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_to_upper(string.str[idx]);                          } return string; }
inline String8 lower_from_str8      (Arena* arena, String8 string) { string = push_str8_copy(arena, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_to_lower(string.str[idx]);                          } return string; }
inline String8 backslashed_from_str8(Arena *arena, String8 string) { string = push_str8_copy(arena, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_is_slash(string.str[idx]) ? '\\' : string.str[idx]; } return string; }

inline String8 upper_from_str8_alloc      (AllocatorInfo ainfo, String8 string) { string = alloc_str8_copy(ainfo, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_to_upper(string.str[idx]);                          } return string; }
inline String8 lower_from_str8_alloc      (AllocatorInfo ainfo, String8 string) { string = alloc_str8_copy(ainfo, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_to_lower(string.str[idx]);                          } return string; }
inline String8 backslashed_from_str8_alloc(AllocatorInfo ainfo, String8 string) { string = alloc_str8_copy(ainfo, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_is_slash(string.str[idx]) ? '\\' : string.str[idx]; } return string; }

////////////////////////////////
//~ rjf: String Matching

#define str8_match_lit(a_lit, b, flags)   str8_match(str8_lit(a_lit), (b), (flags))
#define str8_match_cstr(a_cstr, b, flags) str8_match(str8_cstring(a_cstr), (b), (flags))
#define str8_ends_with_lit(string, end_lit, flags) str8_ends_with((string), str8_lit(end_lit), (flags))

MD_API B32 str8_match              (String8 a, String8 b,                          StringMatchFlags flags);
MD_API U64 str8_find_needle        (String8 string, U64 start_pos, String8 needle, StringMatchFlags flags);
MD_API U64 str8_find_needle_reverse(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags);
       B32 str8_ends_with          (String8 string, String8 end,                   StringMatchFlags flags);

inline B32
str8_ends_with(String8 string, String8 end, StringMatchFlags flags) {
	String8 postfix  = str8_postfix(string, end.size);
	B32     is_match = str8_match(end, postfix, flags);
	return  is_match;
}

////////////////////////////////
//~ rjf: String <=> Integer Conversions

//- rjf: string -> integer
MD_API S64 sign_from_str8 (String8 string, String8* string_tail);
MD_API B32 str8_is_integer(String8 string, U32      radix);
       U64 u64_from_str8  (String8 string, U32      radix);
       S64 s64_from_str8  (String8 string, U32      radix);
       U32 u32_from_str8  (String8 string, U32 radix);
       S32 s32_from_str8  (String8 string, U32 radix);

MD_API B32 try_u64_from_str8_c_rules(String8 string, U64* x);
       B32 try_s64_from_str8_c_rules(String8 string, S64* x);

//- rjf: integer -> string
MD_API String8 str8_from_memory_size(Arena* arena, U64 z);
MD_API String8 str8_from_u64        (Arena* arena, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);
MD_API String8 str8_from_s64        (Arena* arena, S64 s64, U32 radix, U8 min_digits, U8 digit_group_separator);

String8 str8_from_bits_u32(Arena* arena, U32 x);
String8 str8_from_bits_u64(Arena* arena, U64 x);

MD_API String8 str8_from_allocator_size(AllocatorInfo ainfo, U64 z);
MD_API String8 str8_from_allocator_u64 (AllocatorInfo ainfo, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);
MD_API String8 str8_from_alloctor_s64  (AllocatorInfo ainfo, S64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);

inline S64
s64_from_str8(String8 string, U32 radix) {
	S64 sign = sign_from_str8(string, &string);
	S64 x    = (S64)u64_from_str8(string, radix) * sign;
	return(x);
}

inline U64
u64_from_str8(String8 string, U32 radix) {
	U64 x = 0;
	if (1 < radix && radix <= 16) {
		for (U64 i = 0; i < string.size; i += 1) {
			x *= radix;
			x += integer_symbol_reverse(string.str[i]&0x7F);
		}
	}
	return(x);
}

inline U32
u32_from_str8(String8 string, U32 radix) {
	U64 x64 = u64_from_str8(string, radix);
	U32 x32 = safe_cast_u32(x64);
	return x32;
}

inline S32
s32_from_str8(String8 string, U32 radix) {
	S64 x64 = s64_from_str8(string, radix);
	S32 x32 = safe_cast_s32(x64);
	return x32;
}

inline B32
try_s64_from_str8_c_rules(String8 string, S64* x) {
  String8 string_tail = {0};
  S64     sign        = sign_from_str8(string, &string_tail);
  U64     x_u64       = 0;
  B32     is_integer  = try_u64_from_str8_c_rules(string_tail, &x_u64);
  *x = x_u64*sign;
  return(is_integer);
}

inline String8
str8_from_bits_u32(Arena* arena, U32 x)
{
	U8 c0 = 'a' + ((x >> 28) & 0xf);
	U8 c1 = 'a' + ((x >> 24) & 0xf);
	U8 c2 = 'a' + ((x >> 20) & 0xf);
	U8 c3 = 'a' + ((x >> 16) & 0xf);
	U8 c4 = 'a' + ((x >> 12) & 0xf);
	U8 c5 = 'a' + ((x >>  8) & 0xf);
	U8 c6 = 'a' + ((x >>  4) & 0xf);
	U8 c7 = 'a' + ((x >>  0) & 0xf);
	String8 result = push_str8f(arena, "%c%c%c%c%c%c%c%c", c0, c1, c2, c3, c4, c5, c6, c7);
	return result;
}

inline String8
str8_from_bits_u64(Arena* arena, U64 x)
{
	U8 c0 = 'a' + ((x >> 60) & 0xf);
	U8 c1 = 'a' + ((x >> 56) & 0xf);
	U8 c2 = 'a' + ((x >> 52) & 0xf);
	U8 c3 = 'a' + ((x >> 48) & 0xf);
	U8 c4 = 'a' + ((x >> 44) & 0xf);
	U8 c5 = 'a' + ((x >> 40) & 0xf);
	U8 c6 = 'a' + ((x >> 36) & 0xf);
	U8 c7 = 'a' + ((x >> 32) & 0xf);
	U8 c8 = 'a' + ((x >> 28) & 0xf);
	U8 c9 = 'a' + ((x >> 24) & 0xf);
	U8 ca = 'a' + ((x >> 20) & 0xf);
	U8 cb = 'a' + ((x >> 16) & 0xf);
	U8 cc = 'a' + ((x >> 12) & 0xf);
	U8 cd = 'a' + ((x >>  8) & 0xf);
	U8 ce = 'a' + ((x >>  4) & 0xf);
	U8 cf = 'a' + ((x >>  0) & 0xf);
	String8 result = push_str8f(arena,
		"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, ca, cb, cc, cd, ce, cf
	);
	return result;
}

////////////////////////////////
//~ rjf: String <=> Float Conversions

F64 f64_from_str8(String8 string);

////////////////////////////////
//~ rjf: String List Construction Functions

#define str8_list_first(list) ((list)->first ? (list)->first->string : str8_zero())

String8Node* str8_list_push_node                 (String8List* list, String8Node* node);
String8Node* str8_list_push_node_set_string      (String8List* list, String8Node* node, String8 string);
String8Node* str8_list_push_node_front           (String8List* list, String8Node* node);
String8Node* str8_list_push_node_front_set_string(String8List* list, String8Node* node, String8 string);
void         str8_list_concat_in_place           (String8List* list, String8List* to_push);

inline String8Node*
str8_list_push_node(String8List* list, String8Node* node){
	sll_queue_push(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += node->string.size;
	return(node);
}

inline String8Node*
str8_list_push_node_set_string(String8List* list, String8Node* node, String8 string) {
	sll_queue_push(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += string.size;
	node->string      = string;
	return(node);
}

inline String8Node*
str8_list_push_node_front(String8List* list, String8Node* node) {
	sll_queue_push_front(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += node->string.size;
	return(node);
}

inline String8Node*
str8_list_push_node_front_set_string(String8List* list, String8Node* node, String8 string) {
	sll_queue_push_front(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += string.size;
	node->string = string;
	return(node);
}

MD_API String8Node* str8_list_push_aligner(Arena* arena, String8List* list, U64 min, U64 align);
MD_API String8List  str8_list_copy        (Arena* arena, String8List* list);

String8Node* str8_list_push        (Arena* arena, String8List* list, String8 string);
String8Node* str8_list_push_front  (Arena* arena, String8List* list, String8 string);
String8Node* str8_list_pushf       (Arena* arena, String8List* list, char* fmt, ...);
String8Node* str8_list_push_frontf (Arena* arena, String8List* list, char* fmt, ...);

MD_API String8Node* str8_list_alloc_aligner(AllocatorInfo ainfo, String8List* list, U64 min, U64 align);
MD_API String8List  str8_list_alloc_copy   (AllocatorInfo ainfo, String8List* list);

String8Node* str8_list_alloc       (AllocatorInfo ainfo, String8List* list, String8 string);
String8Node* str8_list_alloc_front (AllocatorInfo ainfo, String8List* list, String8 string);
String8Node* str8_list_allocf      (AllocatorInfo ainfo, String8List* list, char* fmt, ...);
String8Node* str8_list_alloc_frontf(AllocatorInfo ainfo, String8List* list, char* fmt, ...);

inline String8Node*
str8_list_push(Arena* arena, String8List* list, String8 string) {
#if MD_DONT_MAP_ANREA_TO_ALLOCATOR_IMPL
	String8Node* node = push_array_no_zero(arena, String8Node, 1);
	str8_list_push_node_set_string(list, node, string);
	return(node);
#else
	return str8_list_alloc(arena_allocator(arena), list, string);
#endif
}

inline String8Node*
str8_list_push_front(Arena* arena, String8List* list, String8 string) {
#if MD_DONT_MAP_ANREA_TO_ALLOCATOR_IMPL
	String8Node *node = push_array_no_zero(arena, String8Node, 1);
	str8_list_push_node_front_set_string(list, node, string);
	return(node);
#else
	return str8_list_alloc_front(arena_allocator(arena), list, string);
#endif
}

inline String8Node*
str8_list_pushf(Arena *arena, String8List *list, char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String8      string = push_str8fv(arena, fmt, args);
	String8Node* result = str8_list_push(arena, list, string);
	va_end(args);
	return(result);
}

inline String8Node*
str8_list_push_frontf(Arena *arena, String8List *list, char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String8      string = push_str8fv(arena, fmt, args);
	String8Node* result = str8_list_push_front(arena, list, string);
	va_end(args);
	return(result);
}

inline String8Node*
str8_list_alloc(AllocatorInfo ainfo, String8List* list, String8 string) {
  String8Node *node = alloc_array_no_zero(ainfo, String8Node, 1);
  str8_list_push_node_front_set_string(list, node, string);
  return(node);
}

inline String8Node*
str8_list_alloc_front(AllocatorInfo ainfo, String8List* list, String8 string) {
  String8Node *node = alloc_array_no_zero(ainfo, String8Node, 1);
  str8_list_push_node_front_set_string(list, node, string);
  return(node);
}

inline String8Node*
str8_list_allocf(AllocatorInfo ainfo, String8List* list, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String8      string = alloc_str8fv(ainfo, fmt, args);
	String8Node* result = str8_list_alloc(ainfo, list, string);
	va_end(args);
	return(result);
}

inline String8Node*
str8_list_alloc_frontf(AllocatorInfo ainfo, String8List* list, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String8      string = alloc_str8fv(ainfo, fmt, args);
	String8Node* result = str8_list_alloc_front(ainfo, list, string);
	va_end(args);
	return(result);
}

////////////////////////////////
//~ rjf: String Splitting & Joining

MD_API String8List str8_split      (Arena*        arena, String8 string, U8* split_chars, U64 split_char_count, StringSplitFlags flags);
MD_API String8List str8_split_alloc(AllocatorInfo ainfo, String8 string, U8* split_chars, U64 split_char_count, StringSplitFlags flags);

String8List  str8_split_by_string_chars           (Arena*        arena, String8      string, String8 split_chars, StringSplitFlags flags);
String8List  str8_split_by_string_chars_alloc     (AllocatorInfo ainfo, String8      string, String8 split_chars, StringSplitFlags flags);
String8List  str8_list_split_by_string_chars      (Arena*        arena, String8List  list,   String8 split_chars, StringSplitFlags flags);
String8List  str8_list_split_by_string_chars_alloc(AllocatorInfo ainfo, String8List  list,   String8 split_chars, StringSplitFlags flags);

MD_API String8 str8_list_join            (Arena*        arena, String8List* list, StringJoin* optional_params);
MD_API String8 str8_list_join_alloc      (AllocatorInfo ainfo, String8List* list, StringJoin* optional_params);
       void    str8_list_from_flags      (Arena*        arena, String8List* list, U32 flags, String8* flag_string_table, U32 flag_string_count);
       void    str8_list_from_flags_alloc(AllocatorInfo ainfo, String8List* list, U32 flags, String8* flag_string_table, U32 flag_string_count);

inline String8List
str8_split_by_string_chars(Arena *arena, String8 string, String8 split_chars, StringSplitFlags flags) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8List list = str8_split(arena, string, split_chars.str, split_chars.size, flags);
	return list;
#else
	return str8_split_by_string_chars_alloc(arena_allocator(arena), string, split_chars, flags);
#endif
}

inline String8List
str8_split_by_string_chars_alloc(AllocatorInfo ainfo, String8 string, String8 split_chars, StringSplitFlags flags) {
	String8List list = str8_split_alloc(ainfo, string, split_chars.str, split_chars.size, flags);
	return list;
}

inline String8List
str8_list_split_by_string_chars(Arena* arena, String8List list, String8 split_chars, StringSplitFlags flags) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8List result = {0};
	for (String8Node *node = list.first; node != 0; node = node->next){
		String8List split = str8_split_by_string_chars(arena, node->string, split_chars, flags);
		str8_list_concat_in_place(&result, &split);
	}
	return result;
#else
	return str8_list_split_by_string_chars_alloc(arena_allocator(arena), list, split_chars, flags);
#endif
}

inline String8List
str8_list_split_by_string_chars_alloc(AllocatorInfo ainfo, String8List list, String8 split_chars, StringSplitFlags flags) {
	String8List result = {0};
	for (String8Node *node = list.first; node != 0; node = node->next){
		String8List split = str8_split_by_string_chars_alloc(ainfo, node->string, split_chars, flags);
		str8_list_concat_in_place(&result, &split);
	}
	return result;
}

void
str8_list_from_flags(Arena* arena, String8List* list, U32 flags, String8* flag_string_table, U32 flag_string_count) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	for (U32 i = 0; i < flag_string_count; i += 1) {
		U32 flag = (1 << i);
		if (flags & flag) {
			str8_list_push(arena, list, flag_string_table[i]);
		}
	}
#else
	str8_list_from_flags_alloc(arena_allocator(arena), list, flags, flag_string_table, flag_string_count);
#endif
}

void
str8_list_from_flags_alloc(AllocatorInfo ainfo, String8List* list, U32 flags, String8* flag_string_table, U32 flag_string_count) {
	for (U32 i = 0; i < flag_string_count; i += 1) {
		U32 flag = (1 << i);
		if (flags & flag) {
			str8_list_alloc(ainfo, list, flag_string_table[i]);
		}
	}
}

////////////////////////////////
//~ rjf; String Arrays

inline String8Array
str8_array_from_list_alloc(AllocatorInfo ainfo, String8List* list) {
	String8Array array;
	array.count = list->node_count;
	array.v     = alloc_array_no_zero(ainfo, String8, array.count);
	U64 idx = 0;
	for(String8Node *n = list->first; n != 0; n = n->next, idx += 1) {
		array.v[idx] = n->string;
	}
	return array;
}

inline String8Array
str8_array_reserve_alloc(AllocatorInfo ainfo, U64 count) {
	String8Array arr;
	arr.count = 0;
	arr.v     = alloc_array(ainfo, String8, count);
	return arr;
}

inline String8Array
str8_array_from_list(Arena* arena, String8List* list) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8Array array;
	array.count = list->node_count;
	array.v     = push_array_no_zero(arena, String8, array.count);
	U64 idx = 0;
	for(String8Node *n = list->first; n != 0; n = n->next, idx += 1) {
		array.v[idx] = n->string;
	}
	return array;
#else
	return str8_array_from_list_alloc(arena_allocator(arena), list);
#endif
}

inline String8Array
str8_array_reserve(Arena *arena, U64 count) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8Array arr;
	arr.count = 0;
	arr.v     = push_array(arena, String8, count);
	return arr;
#else
	return str8_array_reserve_alloc(arena_allocator(arena), count);
#endif
}

////////////////////////////////
//~ rjf: String Path Helpers

MD_API String8 str8_chop_last_slash(String8 string);
MD_API String8 str8_skip_last_slash(String8 string);
MD_API String8 str8_chop_last_dot  (String8 string);
MD_API String8 str8_skip_last_dot  (String8 string);

inline String8List str8_split_path(Arena* arena, String8 string) { String8List result = str8_split(arena, string, (U8*)"/\\", 2, 0); return(result); }

MD_API PathStyle   path_style_from_str8                (String8 string);
MD_API void        str8_path_list_resolve_dots_in_place(                     String8List* path, PathStyle style);
MD_API String8     str8_path_list_join_by_style        (Arena*        arena, String8List* path, PathStyle style);
MD_API String8     str8_path_list_join_by_style_alloc  (AllocatorInfo ainfo, String8List* path, PathStyle style);

////////////////////////////////
//~ rjf: UTF-8 & UTF-16 Decoding/Encoding

inline U8
utf8_class(U8 value)
{
	read_only local_persist
	U8 lookup_table[32] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,2,2,2,3,3,4,5,
	};
	return lookup_table[value];
}

MD_API UnicodeDecode utf8_decode           (U8*  str,    U64 max);
       UnicodeDecode utf16_decode          (U16* str,    U64 max);
MD_API U32           utf8_encode           (U8*  str,    U32 codepoint);
MD_API U32           utf16_encode          (U16* str,    U32 codepoint);
       U32           utf8_from_utf32_single(U8*  buffer, U32 character);

inline UnicodeDecode
utf16_decode(U16* str, U64 max) {
	UnicodeDecode result = {1, MAX_U32};
	result.codepoint = str[0];
	result.inc       = 1;
	if (max > 1 && 0xD800 <= str[0] && str[0] < 0xDC00 && 0xDC00 <= str[1] && str[1] < 0xE000) {
		result.codepoint = ((str[0] - 0xD800) << 10) | 
		                   ((str[1] - 0xDC00) + 0x10000);
		result.inc = 2;
	}
	return(result);
}

inline U32 utf8_from_utf32_single(U8* buffer, U32 character){ return(utf8_encode(buffer, character)); }

////////////////////////////////
//~ rjf: Unicode String Conversions

MD_API String8  str8_from_16(Arena* arena, String16 in);
MD_API String16 str16_from_8(Arena* arena, String8  in);
MD_API String8  str8_from_32(Arena* arena, String32 in);
MD_API String32 str32_from_8(Arena* arena, String8  in);

MD_API String8  str8_from_16_alloc(AllocatorInfo ainfo, String16 in);
MD_API String16 str16_from_8_alloc(AllocatorInfo ainfo, String8  in);
MD_API String8  str8_from_32_alloc(AllocatorInfo ainfo, String32 in);
MD_API String32 str32_from_8_alloc(AllocatorInfo ainfo, String8  in);

////////////////////////////////
//~ String -> Enum Conversions

MD_API OperatingSystem operating_system_from_string(String8 string);

////////////////////////////////
//~ rjf: Basic Types & Space Enum -> String Conversions

inline String8
string_from_dimension(Dimension dimension) {
	local_persist String8 strings[] = {
		str8_lit_comp("X"),
		str8_lit_comp("Y"),
		str8_lit_comp("Z"),
		str8_lit_comp("W"),
	};
	String8 result = str8_lit("error");
	if ((U32)dimension < 4) {
		result = strings[dimension];
	}
	return(result);
}

inline String8
string_from_side(Side side) {
	local_persist String8 strings[] = {
		str8_lit_comp("Min"),
		str8_lit_comp("Max"),
	};
	String8 result = str8_lit("error");
	if ((U32)side < 2) {
		result = strings[side];
	}
	return(result);
}

inline String8
string_from_operating_system(OperatingSystem os) {
	local_persist String8 strings[] = {
		str8_lit_comp("Null"),
		str8_lit_comp("Windows"),
		str8_lit_comp("Linux"),
		str8_lit_comp("Mac"),
	};
	String8 result = str8_lit("error");
	if (os < OperatingSystem_COUNT) {
		result = strings[os];
	}
	return(result);
}

inline String8
string_from_architecture(Arch arch) {
	local_persist String8 strings[] = {
		str8_lit_comp("Null"),
		str8_lit_comp("x64"),
		str8_lit_comp("x86"),
		str8_lit_comp("arm64"),
		str8_lit_comp("arm32"),
	};
	String8 result = str8_lit("error");
	if (arch < Arch_COUNT) {
		result = strings[arch];
	}
	return(result);
}

////////////////////////////////
//~ rjf: Time Types -> String

inline String8
string_from_week_day(WeekDay week_day)
{
	local_persist String8 strings[] = {
		str8_lit_comp("Sun"),
		str8_lit_comp("Mon"),
		str8_lit_comp("Tue"),
		str8_lit_comp("Wed"),
		str8_lit_comp("Thu"),
		str8_lit_comp("Fri"),
		str8_lit_comp("Sat"),
	};
	String8 result = str8_lit("Err");
	if ((U32)week_day < WeekDay_COUNT) {
		result = strings[week_day];
	}
	return(result);
}

inline String8
string_from_month(Month month) 
{
	local_persist String8 strings[] = {
		str8_lit_comp("Jan"),
		str8_lit_comp("Feb"),
		str8_lit_comp("Mar"),
		str8_lit_comp("Apr"),
		str8_lit_comp("May"),
		str8_lit_comp("Jun"),
		str8_lit_comp("Jul"),
		str8_lit_comp("Aug"),
		str8_lit_comp("Sep"),
		str8_lit_comp("Oct"),
		str8_lit_comp("Nov"),
		str8_lit_comp("Dec"),
	};
	String8 result = str8_lit("Err");
	if ((U32)month < Month_COUNT) {
		result = strings[month];
	}
	return(result);
}

MD_API String8 push_date_time_string          (Arena* arena, DateTime* date_time);
MD_API String8 push_file_name_date_time_string(Arena* arena, DateTime* date_time);
MD_API String8 string_from_elapsed_time       (Arena* arena, DateTime  date_time);

MD_API String8 alloc_date_time_string          (AllocatorInfo ainfo, DateTime* date_time);
MD_API String8 alloc_file_name_date_time_string(AllocatorInfo ainfo, DateTime* date_time);
MD_API String8 string_from_elapsed_time_alloc  (AllocatorInfo ainfo, DateTime  date_time);

////////////////////////////////
//~ Globally Unique Ids

inline String8
string_from_guid(Arena* arena, Guid guid) {
	String8 result = push_str8f(arena, 
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.data1,
		guid.data2,
		guid.data3,
		guid.data4[0],
		guid.data4[1],
		guid.data4[2],
		guid.data4[3],
		guid.data4[4],
		guid.data4[5],
		guid.data4[6],
		guid.data4[7]
	);
	return result;
}

inline Guid guid_from_string(String8 string) { Guid guid = {0}; try_guid_from_string(string, &guid); return guid; }

MD_API B32 try_guid_from_string(String8 string, Guid* guid_out);

////////////////////////////////
//~ rjf: Basic Text Indentation

MD_API String8 indented_from_string      (Arena*        arena, String8 string);
MD_API String8 indented_from_string_alloc(AllocatorInfo arena, String8 string);

////////////////////////////////
//~ rjf: Text Escaping

MD_API String8 escaped_from_raw_str8      (Arena*        arena, String8 string);
MD_API String8 escaped_from_raw_str8_alloc(AllocatorInfo ainfo, String8 string);
MD_API String8 raw_from_escaped_str8      (Arena*        arena, String8 string);
MD_API String8 raw_from_escaped_str8_alloc(AllocatorInfo ainfo, String8 string);

////////////////////////////////
//~ rjf: Text Wrapping

MD_API String8List wrapped_lines_from_string      (Arena*        arena, String8 string, U64 first_line_max_width, U64 max_width, U64 wrap_indent);
MD_API String8List wrapped_lines_from_string_alloc(AllocatorInfo ainfo, String8 string, U64 first_line_max_width, U64 max_width, U64 wrap_indent);

////////////////////////////////
//~ rjf: String <-> Color

inline String8 hex_string_from_rgba_4f32      (Arena*        arena, Vec4F32 rgba) { String8 hex_string = push_str8f(arena, "%02x%02x%02x%02x", (U8)(rgba.x*255.f), (U8)(rgba.y*255.f), (U8)(rgba.z*255.f), (U8)(rgba.w*255.f));  return hex_string; }
inline String8 hex_string_from_rgba_4f32_alloc(AllocatorInfo ainfo, Vec4F32 rgba) { String8 hex_string =      alloc_str8f(ainfo, "%02x%02x%02x%02x", (U8)(rgba.x*255.f), (U8)(rgba.y*255.f), (U8)(rgba.z*255.f), (U8)(rgba.w*255.f));  return hex_string; }

MD_API Vec4F32 rgba_from_hex_string_4f32(String8 hex_string);

////////////////////////////////
//~ rjf: String Fuzzy Matching

MD_API FuzzyMatchRangeList fuzzy_match_find                 (Arena*        arena, String8 needle, String8 haystack);
MD_API FuzzyMatchRangeList fuzzy_match_find_alloc           (AllocatorInfo ainfo, String8 needle, String8 haystack);
MD_API FuzzyMatchRangeList fuzzy_match_range_list_copy      (Arena*        arena, FuzzyMatchRangeList* src);
MD_API FuzzyMatchRangeList fuzzy_match_range_list_copy_alloc(AllocatorInfo ainfo, FuzzyMatchRangeList* src);

////////////////////////////////
//~ NOTE(allen): Serialization Helpers

#define str8_serial_push_array(arena, srl, ptr, count) str8_serial_push_data (arena, srl, ptr, sizeof(*(ptr)) * (count))
#define str8_serial_push_struct(arena, srl, ptr)       str8_serial_push_array(arena, srl, ptr, 1)

inline void
str8_serial_write_to_dst(String8List* srl, void* out) {
	U8* ptr = (U8*)out;
	for (String8Node *node = srl->first; node != 0; node = node->next) 
	{
		U64 size = node->string.size;
		memory_copy(ptr, node->string.str, size);
		ptr += size;
	}
}


       void    str8_serial_begin_alloc    (AllocatorInfo ainfo, String8List* srl);
       String8 str8_serial_end_alloc      (AllocatorInfo ainfo, String8List* srl);
MD_API U64     str8_serial_alloc_align    (AllocatorInfo ainfo, String8List* srl, U64 align);
MD_API void*   str8_serial_alloc_size     (AllocatorInfo ainfo, String8List* srl, U64 size);
       void*   str8_serial_alloc_data     (AllocatorInfo ainfo, String8List* srl, void* data, U64 size);
       void    str8_serial_alloc_data_list(AllocatorInfo ainfo, String8List* srl, String8Node* first);
MD_API void    str8_serial_alloc_u64      (AllocatorInfo ainfo, String8List* srl, U64 x);
MD_API void    str8_serial_alloc_u32      (AllocatorInfo ainfo, String8List* srl, U32 x);
       void    str8_serial_alloc_u16      (AllocatorInfo ainfo, String8List* srl, U16 x);
       void    str8_serial_alloc_u8       (AllocatorInfo ainfo, String8List* srl, U8 x);
       void    str8_serial_alloc_cstr     (AllocatorInfo ainfo, String8List* srl, String8 str);
       void    str8_serial_alloc_string   (AllocatorInfo ainfo, String8List* srl, String8 str);

inline void 
str8_serial_begin_alloc(AllocatorInfo ainfo, String8List* srl) {
	String8Node* node = alloc_array(ainfo, String8Node, 1);
	node->string.str = alloc_array_no_zero(ainfo, U8, 0);
	srl->first       = srl->last = node;
	srl->node_count  = 1;
	srl->total_size  = 0;
}

inline String8
str8_serial_end_alloc(AllocatorInfo ainfo, String8List* srl) {
	U64 size = srl->total_size;
	U8* out  = alloc_array_no_zero(ainfo, U8, size);
	str8_serial_write_to_dst(srl, out);
	String8 result = str8(out, size);
	return  result;
}

inline void*
str8_serial_alloc_data(AllocatorInfo ainfo, String8List* srl, void* data, U64 size){
	void* result = str8_serial_alloc_size(ainfo, srl, size);
	if(result != 0) {
		memory_copy(result, data, size);
	}
	return result;
}

inline void
str8_serial_alloc_data_list(AllocatorInfo ainfo, String8List* srl, String8Node* first){
	for (String8Node* node = first; node != 0; node = node->next) {
		str8_serial_alloc_data(ainfo, srl, node->string.str, node->string.size);
	}
}

inline void str8_serial_alloc_u16(AllocatorInfo ainfo, String8List* srl, U16 x) { str8_serial_alloc_data(ainfo, srl, &x, sizeof(x)); }
inline void str8_serial_alloc_u8 (AllocatorInfo ainfo, String8List* srl, U8  x) { str8_serial_alloc_data(ainfo, srl, &x, sizeof(x)); }

inline void str8_serial_alloc_cstr  (AllocatorInfo ainfo, String8List* srl, String8 str) { str8_serial_alloc_data(ainfo, srl, str.str, str.size);  str8_serial_alloc_u8(ainfo, srl, 0); } 
inline void str8_serial_alloc_string(AllocatorInfo ainfo, String8List* srl, String8 str) { str8_serial_alloc_data(ainfo, srl, str.str, str.size); }


       void    str8_serial_begin         (Arena* arena, String8List* srl);
       String8 str8_serial_end           (Arena* arena, String8List* srl);
MD_API U64     str8_serial_push_align    (Arena* arena, String8List* srl, U64 align);
MD_API void*   str8_serial_push_size     (Arena* arena, String8List* srl, U64 size);
       void*   str8_serial_push_data     (Arena* arena, String8List* srl, void* data, U64 size);
       void    str8_serial_push_data_list(Arena* arena, String8List* srl, String8Node* first);
MD_API void    str8_serial_push_u64      (Arena* arena, String8List* srl, U64 x);
MD_API void    str8_serial_push_u32      (Arena* arena, String8List* srl, U32 x);
       void    str8_serial_push_u16      (Arena* arena, String8List* srl, U16 x);
       void    str8_serial_push_u8       (Arena* arena, String8List* srl, U8 x);
       void    str8_serial_push_cstr     (Arena* arena, String8List* srl, String8 str);
       void    str8_serial_push_string   (Arena* arena, String8List* srl, String8 str);

inline void 
str8_serial_begin(Arena* arena, String8List* srl) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8Node* node = push_array(arena, String8Node, 1);
	node->string.str = push_array_no_zero(arena, U8, 0);
	srl->first       = srl->last = node;
	srl->node_count  = 1;
	srl->total_size  = 0;
#else
	str8_serial_begin_alloc(arena_allocator(arena), srl);
#endif
}

inline String8
str8_serial_end(Arena* arena, String8List* srl) {
	U64 size = srl->total_size;
	U8* out  = push_array_no_zero(arena, U8, size);
	str8_serial_write_to_dst(srl, out);
	String8 result = str8(out, size);
	return  result;
}

inline void*
str8_serial_push_data(Arena* arena, String8List* srl, void* data, U64 size){
	void* result = str8_serial_push_size(arena, srl, size);
	if(result != 0) {
		memory_copy(result, data, size);
	}
	return result;
}

inline void
str8_serial_push_data_list(Arena* arena, String8List* srl, String8Node* first){
	for (String8Node* node = first; node != 0; node = node->next) {
		str8_serial_push_data(arena, srl, node->string.str, node->string.size);
	}
}

inline void str8_serial_push_u16(Arena* arena, String8List* srl, U16 x) { str8_serial_push_data(arena, srl, &x, sizeof(x)); }
inline void str8_serial_push_u8 (Arena* arena, String8List* srl, U8  x) { str8_serial_push_data(arena, srl, &x, sizeof(x)); }

inline void str8_serial_push_cstr  (Arena* arena, String8List* srl, String8 str) { str8_serial_push_data(arena, srl, str.str, str.size);  str8_serial_push_u8(arena, srl, 0); } 
inline void str8_serial_push_string(Arena* arena, String8List* srl, String8 str) { str8_serial_push_data(arena, srl, str.str, str.size); }

////////////////////////////////
//~ rjf: Deserialization Helpers

#define str8_deserial_read_array(string, off, ptr, count) str8_deserial_read((string), (off), (ptr), sizeof(*(ptr)) * (count), sizeof( *(ptr)))
#define str8_deserial_read_struct(string, off, ptr)       str8_deserial_read((string), (off), (ptr), sizeof(*(ptr)), sizeof( *(ptr)))

MD_API U64   str8_deserial_read                       (String8 string, U64 off, void* read_dst, U64 read_size, U64 granularity);
MD_API U64   str8_deserial_find_first_match           (String8 string, U64 off, U16 scan_val);
       void* str8_deserial_get_raw_ptr                (String8 string, U64 off, U64 size);
MD_API U64   str8_deserial_read_cstr                  (String8 string, U64 off, String8* cstr_out);
MD_API U64   str8_deserial_read_windows_utf16_string16(String8 string, U64 off, String16* str_out);
       U64   str8_deserial_read_block                 (String8 string, U64 off, U64 size, String8* block_out);
MD_API U64   str8_deserial_read_uleb128               (String8 string, U64 off, U64* value_out);
MD_API U64   str8_deserial_read_sleb128               (String8 string, U64 off, S64* value_out);

inline void* str8_deserial_get_raw_ptr(String8 string, U64 off, U64 size)                      { void* raw_ptr = 0; if (off + size <= string.size) { raw_ptr = string.str + off; }   return raw_ptr; }
inline U64   str8_deserial_read_block (String8 string, U64 off, U64 size, String8* block_out)  { Rng1U64 range = rng_1u64(off, off + size); *block_out = str8_substr(string, range); return block_out->size; }
