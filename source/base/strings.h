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

typedef struct MD_String8 MD_String8;
struct MD_String8
{
	MD_U8* str;
	MD_U64 size;
};

typedef struct MD_String16 MD_String16;
struct MD_String16
{
	MD_U16* str;
	MD_U64  size;
};

typedef struct MD_String32 MD_String32;
struct MD_String32
{
	MD_U32* str;
	MD_U64  size;
};

////////////////////////////////
//~ rjf: String List & Array Types

typedef struct MD_String8Node MD_String8Node;
struct MD_String8Node
{
	MD_String8Node* next;
	MD_String8      string;
};

typedef struct MD_String8MetaNode MD_String8MetaNode;
struct MD_String8MetaNode
{
	MD_String8MetaNode* next;
	MD_String8Node*     node;
};

typedef struct MD_String8List MD_String8List;
struct MD_String8List
{
	MD_String8Node* first;
	MD_String8Node* last;
	MD_U64 node_count;
	MD_U64 total_size;
};

typedef struct MD_String8Array MD_String8Array;
struct MD_String8Array
{
	MD_String8* v;
	MD_U64      count;
};

////////////////////////////////
//~ rjf: String Matching, Splitting, & Joining Types

typedef MD_U32 MD_StringMatchFlags;
enum
{
	MD_StringMatchFlag_CaseInsensitive  = (1 << 0),
	MD_StringMatchFlag_RightSideSloppy  = (1 << 1),
	MD_StringMatchFlag_SlashInsensitive = (1 << 2),
};

typedef MD_U32 MD_StringSplitFlags;
enum
{
	MD_StringSplitFlag_KeepEmpties = (1 << 0),
};

typedef enum MD_PathStyle
{
	MD_PathStyle_Relative,
	MD_PathStyle_WindowsAbsolute,
	MD_PathStyle_UnixAbsolute,
  
#if MD_OS_WINDOWS
	MD_PathStyle_SystemAbsolute = MD_PathStyle_WindowsAbsolute
#elif MD_OS_LINUX
	MD_PathStyle_SystemAbsolute = MD_PathStyle_UnixAbsolute
#else
#	error "absolute path style is undefined for this OS"
#endif
}
MD_PathStyle;

typedef struct MD_StringJoin MD_StringJoin;
struct MD_StringJoin
{
	MD_String8 pre;
	MD_String8 sep;
	MD_String8 post;
};

////////////////////////////////
//~ rjf: UTF Decoding Types

typedef struct MD_UnicodeDecode MD_UnicodeDecode;
struct MD_UnicodeDecode
{
	MD_U32 inc;
	MD_U32 codepoint;
};

////////////////////////////////
//~ rjf: String Fuzzy Matching Types

typedef struct MD_FuzzyMatchRangeNode MD_FuzzyMatchRangeNode;
struct MD_FuzzyMatchRangeNode
{
	MD_FuzzyMatchRangeNode* next;
	MD_Rng1U64 range;
};

typedef struct MD_FuzzyMatchRangeList MD_FuzzyMatchRangeList;
struct MD_FuzzyMatchRangeList
{
	MD_FuzzyMatchRangeNode* first;
	MD_FuzzyMatchRangeNode* last;
	MD_U64 count;
	MD_U64 needle_part_count;
	MD_U64 total_dim;
};

////////////////////////////////
//~ NOTE(allen): String <-> Integer Tables

inline MD_U8
md_integer_symbols(MD_U8 value) {
	md_local_persist
	MD_U8 lookup_table[16] = {
		'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',
	};
	return lookup_table[value];
}

inline MD_U8
md_base64(MD_U8 value) {
	md_local_persist
	MD_U8 lookup_table[64] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
		'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'_', '$',
	};
	return lookup_table[value];
}

inline MD_U8
md_base64_reverse(MD_U8 value) {
	md_local_persist
	MD_U8 lookup_table[128] = {
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
inline MD_U8
md_integer_symbol_reverse(MD_U8 value) {
	md_local_persist 
	MD_U8 lookup_table[128] = {
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

inline MD_B32 md_char_is_space        (MD_U8 c) { return(c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v'); }
inline MD_B32 md_char_is_upper        (MD_U8 c) { return('A' <= c && c <= 'Z'); }
inline MD_B32 md_char_is_lower        (MD_U8 c) { return('a' <= c && c <= 'z'); }
inline MD_B32 md_char_is_alpha        (MD_U8 c) { return(md_char_is_upper(c) || md_char_is_lower(c)); }
inline MD_B32 md_char_is_slash        (MD_U8 c) { return(c == '/' || c == '\\'); }
inline MD_U8  md_char_to_lower        (MD_U8 c) { if (md_char_is_upper(c)) { c += ('a' - 'A'); } return(c); }
inline MD_U8  md_char_to_upper        (MD_U8 c) { if (md_char_is_lower(c)) { c += ('A' - 'a'); } return(c); }
inline MD_U8  md_char_to_correct_slash(MD_U8 c) { if (md_char_is_slash(c)) { c = '/';          } return(c); }

inline MD_B32
md_char_is_digit(MD_U8 c, MD_U32 base) {
	MD_B32 result = 0;
	if (0 < base && base <= 16) {
		MD_U8 val = md_integer_symbol_reverse(c);
		if (val < base) {
			result = 1;
		}
	}
	return(result);
}

////////////////////////////////
//~ rjf: C-String Measurement

inline MD_U64 md_cstring8_length (MD_U8*  c) { MD_U8*  p = c; for (; *p != 0; p += 1); return(p - c); }
inline MD_U64 md_cstring16_length(MD_U16* c) { MD_U16* p = c; for (; *p != 0; p += 1); return(p - c); }
inline MD_U64 md_cstring32_length(MD_U32* c) { MD_U32* p = c; for (; *p != 0; p += 1); return(p - c); }

#define md_cstring_length(c) _Generic(c, MD_U8*: md_cstring8_length, MD_U16*: md_cstring16_length, MD_U32*: md_cstring32_length)(c)

////////////////////////////////
//~ rjf: String Constructors

#define md_str8_lit(S)      md_str8((MD_U8*)(S), sizeof(S) - 1)
#define md_str8_lit_comp(S) { (MD_U8*)(S), sizeof(S) - 1, }
#define md_str8_varg(S)     (int)((S).size), ((S).str)

#define md_str8_array(S, C)    md_str8((MD_U8*)(S), sizeof(*(S)) * (C))
#define md_str8_array_fixed(S) md_str8((MD_U8*)(S), sizeof(S))
#define md_str8_struct(S)      md_str8((MD_U8*)(S), sizeof(*(S)))

inline MD_String8  md_str8         (MD_U8* str,   MD_U64 size)            { MD_String8  result = {str,   size};                                 return(result); } 
inline MD_String8  md_str8_range   (MD_U8* first, MD_U8* one_past_last)   { MD_String8  result = {first, (MD_U64)(one_past_last - first)};      return(result); }
inline MD_String8  md_str8_zero    (void)                                 { MD_String8  result = {0};                                           return(result); }
inline MD_String16 md_str16        (MD_U16* str,   MD_U64  size)          { MD_String16 result = {str,   size};                                 return(result); }
inline MD_String16 md_str16_range  (MD_U16* first, MD_U16* one_past_last) { MD_String16 result = {first, (MD_U64)(one_past_last - first)};      return(result); }
inline MD_String16 md_str16_zero   (void)                                 { MD_String16 result = {0};                                           return(result); }
inline MD_String32 md_str32        (MD_U32* str,   MD_U64  size)          { MD_String32 result = {str,   size};                                 return(result); }
inline MD_String32 md_str32_range  (MD_U32* first, MD_U32* one_past_last) { MD_String32 result = {first, (MD_U64)(one_past_last - first)};      return(result); }
inline MD_String32 md_str32_zero   (void)                                 { MD_String32 result = {0};                                           return(result); }
inline MD_String8  md_str8_cstring (char* c)                              { MD_String8  result = {(MD_U8*) c, md_cstring8_length ((MD_U8*) c)}; return(result); }
inline MD_String16 md_str16_cstring(MD_U16*  c)                           { MD_String16 result = {(MD_U16*)c, md_cstring16_length((MD_U16*)c)}; return(result); }
inline MD_String32 md_str32_cstring(MD_U32*  c)                           { MD_String32 result = {(MD_U32*)c, md_cstring32_length((MD_U32*)c)}; return(result); }

#define md_str_range(str, one_past_last) _Generic(str, MD_U8: md_str8_range,   MD_U16: md_str16_range,   MD_U32: md_str32_range  )(str, one_past_last)
#define md_str_cstring(c)                _Generic(c,   MD_U8: md_str8_cstring, MD_U16: md_str16_cstring, MD_U32: md_str32_cstring)(c)

MD_String8  md_str8_cstring_capped        (void* cstr,      void* cap);
MD_String16 md_str16_cstring_capped       (void* cstr,      void* cap);
MD_String8  md_str8_cstring_capped_reverse(void* raw_start, void* raw_cap);

inline MD_String8
md_str8_cstring_capped(void* cstr, void* cap) {
	char* ptr = (char*)cstr;
	char* opl = (char*)cap;
	for (;ptr < opl && *ptr != 0; ptr += 1);
	MD_U64     size   = (MD_U64)(ptr - (char *)cstr);
	MD_String8 result = {(MD_U8*)cstr, size};
	return  result;
}

inline MD_String16
md_str16_cstring_capped(void* cstr, void* cap)
{
	MD_U16* ptr = (MD_U16*)cstr;
	MD_U16* opl = (MD_U16*)cap;
	for (;ptr < opl && *ptr != 0; ptr += 1);
	MD_U64      size   = (MD_U64)(ptr - (MD_U16 *)cstr);
	MD_String16 result = md_str16(cstr, size);
	return   result;
}

inline MD_String8
md_str8_cstring_capped_reverse(void* raw_start, void* raw_cap)
{
	MD_U8* start = raw_start;
	MD_U8* ptr   = raw_cap;
	for (; ptr > start; )
	{
		ptr -= 1;
		if (*ptr == '\0') { 
			break;
		}
	}
	MD_U64     size    = (MD_U64)(ptr - start);
	MD_String8 result  = md_str8(start, size);
	return  result;
}

////////////////////////////////
//~ rjf: String Formatting & Copying

MD_String8 md_str8_cat__arena (MD_Arena* arena, MD_String8 s1, MD_String8 s2);
MD_String8 md_str8_copy__arena(MD_Arena* arena, MD_String8 s);
MD_String8 md_str8fv__arena   (MD_Arena* arena, char* fmt, va_list args);
MD_String8 md_str8f__arena    (MD_Arena* arena, char* fmt, ...);

MD_API MD_String8 md_str8_cat__ainfo (MD_AllocatorInfo ainfo, MD_String8 s1, MD_String8 s2);
MD_API MD_String8 md_str8_copy__ainfo(MD_AllocatorInfo ainfo, MD_String8 s);
MD_API MD_String8 md_str8fv__ainfo   (MD_AllocatorInfo ainfo, char* fmt, va_list args);
       MD_String8 md_str8f__ainfo    (MD_AllocatorInfo ainfo, char* fmt, ...);

#define md_str8_cat(allocator, s1, s2)  _Generic(allocator, MD_Arena*: md_str8_cat__arena,  MD_AllocatorInfo: md_str8_cat__ainfo,  default: md_assert_generic_sel_fail) md_generic_call(allocator, s1, s2)
#define md_str8_copy(allocator, s)      _Generic(allocator, MD_Arena*: md_str8_copy__arena, MD_AllocatorInfo: md_str8_copy__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, s)
#define md_str8fv(allocator, fmt, args) _Generic(allocator, MD_Arena*: md_str8fv__arena,    MD_AllocatorInfo: md_str8fv__ainfo ,   default: md_assert_generic_sel_fail) md_generic_call(allocator, fmt, args)
#define md_str8f(allocator, fmt, ...)   _Generic(allocator, MD_Arena*: md_str8f__arena,     MD_AllocatorInfo: md_str8f__ainfo ,    default: md_assert_generic_sel_fail) md_generic_call(allocator, fmt, __VA_ARGS__)

md_force_inline MD_String8 md_str8_cat__arena (MD_Arena* arena, MD_String8 s1, MD_String8 s2) { return md_str8_cat__ainfo (md_arena_allocator(arena), s1, s2); }
md_force_inline MD_String8 md_str8_copy__arena(MD_Arena* arena, MD_String8 s)                 { return md_str8_copy__ainfo(md_arena_allocator(arena), s); }
md_force_inline MD_String8 md_str8fv__arena   (MD_Arena* arena, char* fmt, va_list args)      { return md_str8fv__ainfo   (md_arena_allocator(arena), fmt, args); }

inline MD_String8
md_str8f__arena(MD_Arena *arena, char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8 result = md_str8fv(arena, fmt, args);
	va_end(args);
	return(result);
}

inline MD_String8
md_str8f__ainfo(MD_AllocatorInfo ainfo, char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8 result = md_str8fv(ainfo, fmt, args);
	va_end(args);
	return(result);
}

////////////////////////////////
//~ rjf: String Slicing

MD_API MD_String8 md_str8_skip_chop_whitespace(MD_String8 string);

MD_String8 md_str8_substr (MD_String8 str, MD_Rng1U64 range);
MD_String8 md_str8_prefix (MD_String8 str, MD_U64     size);
MD_String8 md_str8_skip   (MD_String8 str, MD_U64     amt);
MD_String8 md_str8_postfix(MD_String8 str, MD_U64     size);
MD_String8 md_str8_chop   (MD_String8 str, MD_U64     amt);

inline MD_String8
md_str8_substr(MD_String8 str, MD_Rng1U64 range){
	range.md_min = md_clamp_top(range.md_min, str.size);
	range.md_max = md_clamp_top(range.md_max, str.size);
	str.str  += range.md_min;
	str.size  = md_dim_1u64(range);
	return(str);
}

inline MD_String8 
md_str8_prefix(MD_String8 str, MD_U64 size){
	str.size = md_clamp_top(size, str.size);
	return(str);
}

inline MD_String8
md_str8_skip(MD_String8 str, MD_U64 amt){
	amt       = md_clamp_top(amt, str.size);
	str.str  += amt;
	str.size -= amt;
	return(str);
}

inline MD_String8
md_str8_postfix(MD_String8 str, MD_U64 size){
	size     = md_clamp_top(size, str.size);
	str.str  = (str.str + str.size) - size;
	str.size = size;
	return(str);
}

inline MD_String8
md_str8_chop(MD_String8 str, MD_U64 amt){
	amt       = md_clamp_top(amt, str.size);
	str.size -= amt;
	return(str);
}

////////////////////////////////
//~ rjf: String Stylization

inline MD_String8 md_upper_fromstr8__arena       (MD_Arena* arena, MD_String8 string) { string = md_str8_copy(arena, string); for(MD_U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = md_char_to_upper(string.str[idx]);                          } return string; }
inline MD_String8 md_lower_fromstr8__arena       (MD_Arena* arena, MD_String8 string) { string = md_str8_copy(arena, string); for(MD_U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = md_char_to_lower(string.str[idx]);                          } return string; }
inline MD_String8 md_backslashed_from_str8__arena(MD_Arena* arena, MD_String8 string) { string = md_str8_copy(arena, string); for(MD_U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = md_char_is_slash(string.str[idx]) ? '\\' : string.str[idx]; } return string; }

inline MD_String8 md_upper_fromstr8__ainfo       (MD_AllocatorInfo ainfo, MD_String8 string) { string = md_str8_copy(ainfo, string); for(MD_U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = md_char_to_upper(string.str[idx]);                          } return string; }
inline MD_String8 md_lower_fromstr8__ainfo       (MD_AllocatorInfo ainfo, MD_String8 string) { string = md_str8_copy(ainfo, string); for(MD_U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = md_char_to_lower(string.str[idx]);                          } return string; }
inline MD_String8 md_backslashed_from_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8 string) { string = md_str8_copy(ainfo, string); for(MD_U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = md_char_is_slash(string.str[idx]) ? '\\' : string.str[idx]; } return string; }

#define md_upper_fromstr8(allocator, string)        _Generic(allocator, MD_Arena*: md_upper_fromstr8__arena,        MD_AllocatorInfo: md_upper_fromstr8__ainfo,         default: md_assert_generic_sel_fail) md_generic_call(allocator, string)
#define md_lower_fromstr8(allocator, string)        _Generic(allocator, MD_Arena*: md_lower_fromstr8__arena,        MD_AllocatorInfo: md_lower_fromstr8__ainfo,         default: md_assert_generic_sel_fail) md_generic_call(allocator, string)
#define md_backslashed_from_str8(allocator, string) _Generic(allocator, MD_Arena*: md_backslashed_from_str8__arena, MD_AllocatorInfo: md_backslashed_from_str8__ainfo , default: md_assert_generic_sel_fail) md_generic_call(allocator, string)

////////////////////////////////
//~ rjf: String Matching

#define md_str8_match_lit(a_lit, b, flags)            md_str8_match(md_str8_lit(a_lit), (b), (flags))
#define md_str8_match_cstr(a_cstr, b, flags)          md_str8_match(md_str8_cstring(a_cstr), (b), (flags))
#define md_str8_ends_with_lit(string, end_lit, flags) md_str8_ends_with((string), md_str8_lit(end_lit), (flags))

MD_API MD_B32 md_str8_match              (MD_String8 a, MD_String8 b,                             MD_StringMatchFlags flags);
MD_API MD_U64 md_str8_find_needle        (MD_String8 string, MD_U64 start_pos, MD_String8 needle, MD_StringMatchFlags flags);
MD_API MD_U64 md_str8_find_needle_reverse(MD_String8 string, MD_U64 start_pos, MD_String8 needle, MD_StringMatchFlags flags);
       MD_B32 md_str8_ends_with          (MD_String8 string, MD_String8 end,                      MD_StringMatchFlags flags);

inline MD_B32
md_str8_ends_with(MD_String8 string, MD_String8 end, MD_StringMatchFlags flags) {
	MD_String8 postfix  = md_str8_postfix(string, end.size);
	MD_B32     is_match = md_str8_match(end, postfix, flags);
	return  is_match;
}

////////////////////////////////
//~ rjf: String <=> Integer Conversions

//- rjf: string -> integer
MD_API MD_S64 md_sign_from_str8 (MD_String8 string, MD_String8* md_string_tail);
MD_API MD_B32 md_str8_is_integer(MD_String8 string, MD_U32      radix);
       MD_U64 md_u64_from_str8  (MD_String8 string, MD_U32      radix);
       MD_S64 md_s64_from_str8  (MD_String8 string, MD_U32      radix);
       MD_U32 md_u32_from_str8  (MD_String8 string, MD_U32 radix);
       MD_S32 s32_from_str8     (MD_String8 string, MD_U32 radix);

MD_API MD_B32 md_try_u64_from_str8_c_rules(MD_String8 string, MD_U64* x);
       MD_B32 md_try_s64_from_str8_c_rules(MD_String8 string, MD_S64* x);

//- rjf: integer -> string
MD_String8 md_str8_from_memory_size__arena(MD_Arena* arena, MD_SSIZE z);
MD_String8 md_str8_from_u64__arena        (MD_Arena* arena, MD_U64 u64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator);
MD_String8 md_str8_from_s64__arena        (MD_Arena* arena, MD_S64 s64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator);

MD_String8 md_str8_from_bits_u32__arena(MD_Arena* arena, MD_U32 x);
MD_String8 md_str8_from_bits_u64__arena(MD_Arena* arena, MD_U64 x);

MD_API MD_String8 md_str8_from_memory_size__ainfo(MD_AllocatorInfo ainfo, MD_SSIZE z);
MD_API MD_String8 md_str8_from_u64__ainfo        (MD_AllocatorInfo ainfo, MD_U64 u64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator);
MD_API MD_String8 md_str8_from_s64__ainfo        (MD_AllocatorInfo ainfo, MD_S64 u64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator);

MD_String8 md_str8_from_bits_u32__ainfo(MD_AllocatorInfo ainfo, MD_U32 x);
MD_String8 md_str8_from_bits_u64__ainfo(MD_AllocatorInfo ainfo, MD_U64 x);

#define md_str8_from_memory_size(allocator, z)                                     _Generic(allocator, MD_Arena*: md_str8_from_memory_size__arena, MD_AllocatorInfo: md_str8_from_memory_size__ainfo,  default: md_assert_generic_sel_fail) md_generic_call(allocator, z)
#define md_str8_from_u64(allocator, u64, radix, min_digits, digit_group_separator) _Generic(allocator, MD_Arena*: md_str8_from_u64__arena,         MD_AllocatorInfo: md_str8_from_u64__ainfo,          default: md_assert_generic_sel_fail) md_generic_call(allocator, u64, radix, min_digits, digit_group_separator)
#define md_str8_from_s64(allocator, s64, radix, min_digits, digit_group_separator) _Generic(allocator, MD_Arena*: md_str8_from_s64__arena,         MD_AllocatorInfo: md_str8_from_s64__ainfo ,         default: md_assert_generic_sel_fail) md_generic_call(allocator, s64, radix, min_digits, digit_group_separator)

#define md_str8_from_bits_u32(allocator, x) _Generic(allocator, MD_Arena*: md_str8_from_bits_u32__arena, MD_AllocatorInfo: md_str8_from_bits_u32__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, x)
#define md_str8_from_bits_u64(allocator, x) _Generic(allocator, MD_Arena*: md_str8_from_bits_u64__arena, MD_AllocatorInfo: md_str8_from_bits_u64__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, x)

md_force_inline MD_String8 md_str8_from_memory_size__arena(MD_Arena* arena, MD_SSIZE z)                                                              { return md_str8_from_memory_size__ainfo(md_arena_allocator(arena), z); }
md_force_inline MD_String8 md_str8_from_u64__arena        (MD_Arena* arena, MD_U64 u64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator) { return md_str8_from_u64__ainfo        (md_arena_allocator(arena), u64, radix, min_digits, digit_group_separator); }
md_force_inline MD_String8 md_str8_from_s64__arena        (MD_Arena* arena, MD_S64 s64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator) { return md_str8_from_s64__ainfo        (md_arena_allocator(arena), s64, radix, min_digits, digit_group_separator); }

inline MD_S64
md_s64_from_str8(MD_String8 string, MD_U32 radix) {
	MD_S64 sign = md_sign_from_str8(string, &string);
	MD_S64 x    = (MD_S64)md_u64_from_str8(string, radix) * sign;
	return(x);
}

inline MD_U64
md_u64_from_str8(MD_String8 string, MD_U32 radix) {
	MD_U64 x = 0;
	if (1 < radix && radix <= 16) {
		for (MD_U64 i = 0; i < string.size; i += 1) {
			x *= radix;
			x += md_integer_symbol_reverse(string.str[i]&0x7F);
		}
	}
	return(x);
}

inline MD_U32
md_u32_from_str8(MD_String8 string, MD_U32 radix) {
	MD_U64 x64 = md_u64_from_str8(string, radix);
	MD_U32 x32 = md_safe_cast_u32(x64);
	return x32;
}

inline MD_S32
s32_from_str8(MD_String8 string, MD_U32 radix) {
	MD_S64 x64 = md_s64_from_str8(string, radix);
	MD_S32 x32 = md_safe_cast_s32(x64);
	return x32;
}

inline MD_B32
md_try_s64_from_str8_c_rules(MD_String8 string, MD_S64* x) {
  MD_String8 md_string_tail = {0};
  MD_S64     sign           = md_sign_from_str8(string, &md_string_tail);
  MD_U64     x_u64          = 0;
  MD_B32     is_integer     = md_try_u64_from_str8_c_rules(md_string_tail, &x_u64);
  *x = x_u64*sign;
  return(is_integer);
}

inline MD_String8
md_str8_from_bits_u32__arena(MD_Arena* arena, MD_U32 x)
{
	MD_U8 c0 = 'a' + ((x >> 28) & 0xf);
	MD_U8 c1 = 'a' + ((x >> 24) & 0xf);
	MD_U8 c2 = 'a' + ((x >> 20) & 0xf);
	MD_U8 c3 = 'a' + ((x >> 16) & 0xf);
	MD_U8 c4 = 'a' + ((x >> 12) & 0xf);
	MD_U8 c5 = 'a' + ((x >>  8) & 0xf);
	MD_U8 c6 = 'a' + ((x >>  4) & 0xf);
	MD_U8 c7 = 'a' + ((x >>  0) & 0xf);
	MD_String8 result = md_str8f(arena, "%c%c%c%c%c%c%c%c", c0, c1, c2, c3, c4, c5, c6, c7);
	return result;
}

inline MD_String8
md_str8_from_bits_u64__arena(MD_Arena* arena, MD_U64 x)
{
	MD_U8 c0 = 'a' + ((x >> 60) & 0xf);
	MD_U8 c1 = 'a' + ((x >> 56) & 0xf);
	MD_U8 c2 = 'a' + ((x >> 52) & 0xf);
	MD_U8 c3 = 'a' + ((x >> 48) & 0xf);
	MD_U8 c4 = 'a' + ((x >> 44) & 0xf);
	MD_U8 c5 = 'a' + ((x >> 40) & 0xf);
	MD_U8 c6 = 'a' + ((x >> 36) & 0xf);
	MD_U8 c7 = 'a' + ((x >> 32) & 0xf);
	MD_U8 c8 = 'a' + ((x >> 28) & 0xf);
	MD_U8 c9 = 'a' + ((x >> 24) & 0xf);
	MD_U8 ca = 'a' + ((x >> 20) & 0xf);
	MD_U8 cb = 'a' + ((x >> 16) & 0xf);
	MD_U8 cc = 'a' + ((x >> 12) & 0xf);
	MD_U8 cd = 'a' + ((x >>  8) & 0xf);
	MD_U8 ce = 'a' + ((x >>  4) & 0xf);
	MD_U8 cf = 'a' + ((x >>  0) & 0xf);
	MD_String8 result = md_str8f(arena,
		"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, ca, cb, cc, cd, ce, cf
	);
	return result;
}

inline MD_String8
md_str8_from_bits_u32__ainfo(MD_AllocatorInfo ainfo, MD_U32 x)
{
	MD_U8 c0 = 'a' + ((x >> 28) & 0xf);
	MD_U8 c1 = 'a' + ((x >> 24) & 0xf);
	MD_U8 c2 = 'a' + ((x >> 20) & 0xf);
	MD_U8 c3 = 'a' + ((x >> 16) & 0xf);
	MD_U8 c4 = 'a' + ((x >> 12) & 0xf);
	MD_U8 c5 = 'a' + ((x >>  8) & 0xf);
	MD_U8 c6 = 'a' + ((x >>  4) & 0xf);
	MD_U8 c7 = 'a' + ((x >>  0) & 0xf);
	MD_String8 result = md_str8f(ainfo, "%c%c%c%c%c%c%c%c", c0, c1, c2, c3, c4, c5, c6, c7);
	return result;
}

inline MD_String8
md_str8_from_bits_u64__ainfo(MD_AllocatorInfo ainfo, MD_U64 x)
{
	MD_U8 c0 = 'a' + ((x >> 60) & 0xf);
	MD_U8 c1 = 'a' + ((x >> 56) & 0xf);
	MD_U8 c2 = 'a' + ((x >> 52) & 0xf);
	MD_U8 c3 = 'a' + ((x >> 48) & 0xf);
	MD_U8 c4 = 'a' + ((x >> 44) & 0xf);
	MD_U8 c5 = 'a' + ((x >> 40) & 0xf);
	MD_U8 c6 = 'a' + ((x >> 36) & 0xf);
	MD_U8 c7 = 'a' + ((x >> 32) & 0xf);
	MD_U8 c8 = 'a' + ((x >> 28) & 0xf);
	MD_U8 c9 = 'a' + ((x >> 24) & 0xf);
	MD_U8 ca = 'a' + ((x >> 20) & 0xf);
	MD_U8 cb = 'a' + ((x >> 16) & 0xf);
	MD_U8 cc = 'a' + ((x >> 12) & 0xf);
	MD_U8 cd = 'a' + ((x >>  8) & 0xf);
	MD_U8 ce = 'a' + ((x >>  4) & 0xf);
	MD_U8 cf = 'a' + ((x >>  0) & 0xf);
	MD_String8 result = md_str8f(ainfo,
		"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
		c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, ca, cb, cc, cd, ce, cf
	);
	return result;
}

////////////////////////////////
//~ rjf: String <=> Float Conversions

MD_F64 md_f64_from_str8(MD_String8 string);

////////////////////////////////
//~ rjf: String List Construction Functions

#define md_str8_list_first(list) ((list)->first ? (list)->first->string : md_str8_zero())

MD_String8Node* md_str8_list_push_node                 (MD_String8List* list, MD_String8Node* node);
MD_String8Node* md_str8_list_push_node_set_string      (MD_String8List* list, MD_String8Node* node, MD_String8 string);
MD_String8Node* md_str8_list_push_node_front           (MD_String8List* list, MD_String8Node* node);
MD_String8Node* md_str8_list_push_node_front_set_string(MD_String8List* list, MD_String8Node* node, MD_String8 string);
void         md_str8_list_concat_in_place              (MD_String8List* list, MD_String8List* to_push);

inline MD_String8Node*
md_str8_list_push_node(MD_String8List* list, MD_String8Node* node) {
	md_sll_queue_push(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += node->string.size;
	return(node);
}

inline MD_String8Node*
md_str8_list_push_node_set_string(MD_String8List* list, MD_String8Node* node, MD_String8 string) {
	md_sll_queue_push(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += string.size;
	node->string      = string;
	return(node);
}

inline MD_String8Node*
md_str8_list_push_node_front(MD_String8List* list, MD_String8Node* node) {
	md_sll_queue_push_front(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += node->string.size;
	return(node);
}

inline MD_String8Node*
md_str8_list_push_node_front_set_string(MD_String8List* list, MD_String8Node* node, MD_String8 string) {
	md_sll_queue_push_front(list->first, list->last, node);
	list->node_count += 1;
	list->total_size += string.size;
	node->string = string;
	return(node);
}

MD_String8Node* md_str8_list_aligner__arena(MD_Arena* arena, MD_String8List* list, MD_U64 md_min, MD_U64 align);
MD_String8List  md_str8_list_copy__arena   (MD_Arena* arena, MD_String8List* list);

MD_String8Node* md_str8_list_push__arena       (MD_Arena* arena, MD_String8List* list, MD_String8 string);
MD_String8Node* md_str8_list_push_front__arena (MD_Arena* arena, MD_String8List* list, MD_String8 string);
MD_String8Node* md_str8_list_pushf__arena      (MD_Arena* arena, MD_String8List* list, char* fmt, ...);
MD_String8Node* md_str8_list_push_frontf__arena(MD_Arena* arena, MD_String8List* list, char* fmt, ...);

MD_API MD_String8Node* md_str8_list_aligner__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, MD_U64 md_min, MD_U64 align);
MD_API MD_String8List  md_str8_list_copy__ainfo   (MD_AllocatorInfo ainfo, MD_String8List* list);

MD_String8Node* md_str8_list_push__ainfo       (MD_AllocatorInfo ainfo, MD_String8List* list, MD_String8 string);
MD_String8Node* md_str8_list_push_front__ainfo (MD_AllocatorInfo ainfo, MD_String8List* list, MD_String8 string);
MD_String8Node* md_str8_list_pushf__ainfo      (MD_AllocatorInfo ainfo, MD_String8List* list, char* fmt, ...);
MD_String8Node* md_str8_list_push_frontf__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, char* fmt, ...);

#define md_str8_list_aligner(allocator, list, md_min, align) _Generic(allocator, MD_Arena*: md_str8_list_aligner__arena,     MD_AllocatorInfo: md_str8_list_aligner__ainfo,     default: md_assert_generic_sel_fail) md_generic_call(allocator, list, md_min, align)
#define md_str8_list_copy(allocator, list)                   _Generic(allocator, MD_Arena*: md_str8_list_copy__arena,        MD_AllocatorInfo: md_str8_list_copy__ainfo,        default: md_assert_generic_sel_fail) md_generic_call(allocator, list)
#define md_str8_list_push(allocator, list, string)           _Generic(allocator, MD_Arena*: md_str8_list_push__arena,        MD_AllocatorInfo: md_str8_list_push__ainfo,        default: md_assert_generic_sel_fail) md_generic_call(allocator, list, string)
#define md_str8_list_push_front(allocator, list, string)     _Generic(allocator, MD_Arena*: md_str8_list_push_front__arena,  MD_AllocatorInfo: md_str8_list_push_front__ainfo,  default: md_assert_generic_sel_fail) md_generic_call(allocator, list, string)
#define md_str8_list_pushf(allocator, list, fmt, ...)        _Generic(allocator, MD_Arena*: md_str8_list_pushf__arena,       MD_AllocatorInfo: md_str8_list_pushf__ainfo,       default: md_assert_generic_sel_fail) md_generic_call(allocator, list, fmt, __VA_ARGS__)
#define md_str8_list_push_frontf(allocaotr, list, fmt, ...)  _Generic(allocator, MD_Arena*: md_str8_list_push_frontf__arena, MD_AllocatorInfo: md_str8_list_push_frontf__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, list, fmt, __VA_ARGS__)

md_force_inline MD_String8Node* md_str8_list_ligner__arena    (MD_Arena* arena, MD_String8List* list, MD_U64 md_min, MD_U64 align) { return md_str8_list_aligner__ainfo   (md_arena_allocator(arena), list, md_min, align); }
md_force_inline MD_String8List  md_str8_list_copy__arena      (MD_Arena* arena, MD_String8List* list)                              { return md_str8_list_copy__ainfo      (md_arena_allocator(arena), list); }
md_force_inline MD_String8Node* md_str8_list_push__arena      (MD_Arena* arena, MD_String8List* list, MD_String8 string)           { return md_str8_list_push__ainfo      (md_arena_allocator(arena), list, string); }
md_force_inline MD_String8Node* md_str8_list_push_front__arena(MD_Arena* arena, MD_String8List* list, MD_String8 string)           { return md_str8_list_push_front__ainfo(md_arena_allocator(arena), list, string); }

inline MD_String8Node*
md_str8_list_pushf__arena(MD_Arena *arena, MD_String8List *list, char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8      string = md_str8fv(arena, fmt, args);
	MD_String8Node* result = md_str8_list_push(arena, list, string);
	va_end(args);
	return(result);
}

inline MD_String8Node*
md_str8_list_push_frontf__arena(MD_Arena *arena, MD_String8List *list, char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8      string = md_str8fv(arena, fmt, args);
	MD_String8Node* result = md_str8_list_push_front(arena, list, string);
	va_end(args);
	return(result);
}

inline MD_String8Node*
md_str8_list_push__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, MD_String8 string) {
  MD_String8Node* node = md_alloc_array_no_zero(ainfo, MD_String8Node, 1);
  md_str8_list_push_node_set_string(list, node, string);
  return(node);
}

inline MD_String8Node*
md_str8_list_push_front__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, MD_String8 string) {
  MD_String8Node *node = md_alloc_array_no_zero(ainfo, MD_String8Node, 1);
  md_str8_list_push_node_front_set_string(list, node, string);
  return(node);
}

inline MD_String8Node*
md_str8_list_pushf__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8      string = md_str8fv(ainfo, fmt, args);
	MD_String8Node* result = md_str8_list_push(ainfo, list, string);
	va_end(args);
	return(result);
}

inline MD_String8Node*
md_str8_list_push_frontf__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8      string = md_str8fv(ainfo, fmt, args);
	MD_String8Node* result = md_str8_list_push_front(ainfo, list, string);
	va_end(args);
	return(result);
}

////////////////////////////////
//~ rjf: String Splitting & Joining

       MD_String8List md_str8_split__arena(MD_Arena*        arena, MD_String8 string, MD_U8* split_chars, MD_U64 split_char_count, MD_StringSplitFlags flags);
MD_API MD_String8List md_str8_split__ainfo(MD_AllocatorInfo ainfo, MD_String8 string, MD_U8* split_chars, MD_U64 split_char_count, MD_StringSplitFlags flags);

MD_String8List  md_str8_split_by_string_chars__arena     (MD_Arena*        arena, MD_String8      string, MD_String8 split_chars, MD_StringSplitFlags flags);
MD_String8List  md_str8_split_by_string_chars__ainfo     (MD_AllocatorInfo ainfo, MD_String8      string, MD_String8 split_chars, MD_StringSplitFlags flags);
MD_String8List  md_str8_list_split_by_string_chars__arena(MD_Arena*        arena, MD_String8List  list,   MD_String8 split_chars, MD_StringSplitFlags flags);
MD_String8List  md_str8_list_split_by_string_chars__ainfo(MD_AllocatorInfo ainfo, MD_String8List  list,   MD_String8 split_chars, MD_StringSplitFlags flags);

       MD_String8 md_str8_list_join__arena   (MD_Arena*        arena, MD_String8List* list, MD_StringJoin* optional_params);
MD_API MD_String8 md_str8_list_join__ainfo   (MD_AllocatorInfo ainfo, MD_String8List* list, MD_StringJoin* optional_params);
       void    md_str8_list_from_flags__arena(MD_Arena*        arena, MD_String8List* list, MD_U32 flags, MD_String8* flag_string_table, MD_U32 flag_string_count);
       void    md_str8_list_from_flags__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, MD_U32 flags, MD_String8* flag_string_table, MD_U32 flag_string_count);

#define md_str8_split(allocator, string, split_chars, split_char_count, flags)  _Generic(allocator, MD_Arena*: md_str8_split__arena,                      MD_AllocatorInfo: md_str8_split__ainfo,                      default: md_assert_generic_sel_fail) md_generic_call(allocator, string, split_chars, split_char_count, flags)
#define md_str8_split_by_string_chars(allocator, string, split_chars, flags)    _Generic(allocator, MD_Arena*: md_str8_split_by_string_chars__arena,      MD_AllocatorInfo: md_str8_split_by_string_chars__ainfo,      default: md_assert_generic_sel_fail) md_generic_call(allocator, string, split_chars, flags)
#define md_str8_list_split_by_string_chars(allocator, list, split_chars, flags) _Generic(allocator, MD_Arena*: md_str8_list_split_by_string_chars__arena, MD_AllocatorInfo: md_str8_list_split_by_string_chars__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, list,   split_chars, flags)
#define md_str8_list_join(allocator, list, params)                              _Generic(allocator, MD_Arena*: md_str8_list_join__arena,                  MD_AllocatorInfo: md_str8_list_join__ainfo,                  default: md_assert_generic_sel_fail) md_generic_call(allocator, list, params )

md_force_inline MD_String8List md_str8_split__arena                     (MD_Arena* arena, MD_String8 string,    MD_U8*     split_chars, MD_U64 split_char_count, MD_StringSplitFlags flags) { return md_str8_split__ainfo                      (md_arena_allocator(arena), string, split_chars, split_char_count, flags); }
md_force_inline MD_String8List md_str8_split_by_string_chars__arena     (MD_Arena* arena, MD_String8 string,    MD_String8 split_chars, MD_StringSplitFlags flags)                          { return md_str8_split_by_string_chars__ainfo      (md_arena_allocator(arena), string, split_chars, flags); }
md_force_inline MD_String8List md_str8_list_split_by_string_chars__arena(MD_Arena* arena, MD_String8List  list, MD_String8 split_chars, MD_StringSplitFlags flags)                          { return md_str8_list_split_by_string_chars__ainfo (md_arena_allocator(arena), list,   split_chars, flags); }
md_force_inline void        md_str8_list_from_flags__arena              (MD_Arena* arena, MD_String8List* list, MD_U32 flags, MD_String8* flag_string_table, MD_U32 flag_string_count)      {        md_str8_list_from_flags__ainfo            (md_arena_allocator(arena), list, flags, flag_string_table, flag_string_count); }
md_force_inline MD_String8     md_str8_list_join__arena                 (MD_Arena* arena, MD_String8List* list, MD_StringJoin* optional_params)                                             { return md_str8_list_join__ainfo                  (md_arena_allocator(arena), list, optional_params); }

inline MD_String8List
md_str8_split_by_string_chars__ainfo(MD_AllocatorInfo ainfo, MD_String8 string, MD_String8 split_chars, MD_StringSplitFlags flags) {
	MD_String8List list = md_str8_split(ainfo, string, split_chars.str, split_chars.size, flags);
	return list;
}

inline MD_String8List
md_str8_list_split_by_string_chars__ainfo(MD_AllocatorInfo ainfo, MD_String8List list, MD_String8 split_chars, MD_StringSplitFlags flags) {
	MD_String8List result = {0};
	for (MD_String8Node *node = list.first; node != 0; node = node->next){
		MD_String8List split = md_str8_split_by_string_chars(ainfo, node->string, split_chars, flags);
		md_str8_list_concat_in_place(&result, &split);
	}
	return result;
}

void
md_str8_list_from_flags__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, MD_U32 flags, MD_String8* flag_string_table, MD_U32 flag_string_count) {
	for (MD_U32 i = 0; i < flag_string_count; i += 1) {
		MD_U32 flag = (1 << i);
		if (flags & flag) {
			md_str8_list_push(ainfo, list, flag_string_table[i]);
		}
	}
}

////////////////////////////////
//~ rjf; String Arrays

#define md_str8_array_from_list(allocator, list) _Generic(allocator, MD_Arena*: md_str8_array_from_list__arena, MD_AllocatorInfo: md_str8_array_from_list__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, list)
#define md_str8_array_reserve(allocator, count)  _Generic(allocaotr, MD_Arena*: md_str8_array_reserve__arena    MD_AllocatorInfo: md_str8_array_reserve__ainfo,   default: md_assert_generic_sel_fail) md_generic_call(allocator, count)

inline MD_String8Array
md_str8_array_from_list__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list) {
	MD_String8Array array;
	array.count = list->node_count;
	array.v     = md_alloc_array_no_zero(ainfo, MD_String8, array.count);
	MD_U64 idx = 0;
	for(MD_String8Node *n = list->first; n != 0; n = n->next, idx += 1) {
		array.v[idx] = n->string;
	}
	return array;
}

inline MD_String8Array
md_str8_array_reserve__ainfo(MD_AllocatorInfo ainfo, MD_U64 count) {
	MD_String8Array arr;
	arr.count = 0;
	arr.v     = md_alloc_array(ainfo, MD_String8, count);
	return arr;
}

md_force_inline MD_String8Array md_str8_array_from_list__arena(MD_Arena* arena, MD_String8List* list) { return md_str8_array_from_list__ainfo(md_arena_allocator(arena), list); }
md_force_inline MD_String8Array md_str8_array_reserve__arena  (MD_Arena* arena, MD_U64 count)         { return md_str8_array_reserve__ainfo  (md_arena_allocator(arena), count); }

////////////////////////////////
//~ rjf: String Path Helpers

MD_API MD_String8 md_str8_chop_last_slash(MD_String8 string);
MD_API MD_String8 md_str8_skip_last_slash(MD_String8 string);
MD_API MD_String8 md_str8_chop_last_dot  (MD_String8 string);
MD_API MD_String8 md_str8_skip_last_dot  (MD_String8 string);

md_force_inline MD_String8List md_str8_split_path__arena(MD_Arena*        arena, MD_String8 string) { MD_String8List result = md_str8_split(arena, string, (MD_U8*)"/\\", 2, 0); return(result); }
md_force_inline MD_String8List md_str8_split_path__ainfo(MD_AllocatorInfo ainfo, MD_String8 string) { MD_String8List result = md_str8_split(ainfo, string, (MD_U8*)"/\\", 2, 0); return(result); }

#define md_str8_split_path(allocator, string) _Generic(allocator, MD_Arena*: md_str8_split_path__arena, MD_AllocatorInfo: md_str8_split_path__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, string)

MD_API MD_PathStyle   md_path_style_from_str8                (MD_String8 string);
MD_API void        md_str8_path_list_resolve_dots_in_place(                     MD_String8List* path, MD_PathStyle style);
MD_API MD_String8     md_str8_path_list_join_by_style__arena (MD_Arena*        arena, MD_String8List* path, MD_PathStyle style);
MD_API MD_String8     md_str8_path_list_join_by_style__ainfo (MD_AllocatorInfo ainfo, MD_String8List* path, MD_PathStyle style);

#define md_str8_path_list_join_by_style(allocator, path, style) _Generic(allocator, MD_Arena*: md_str8_path_list_join_by_style__arena, MD_AllocatorInfo: md_str8_split_path__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, path, style)

md_force_inline MD_String8 md_str8_path_list_join_by_style__arena(MD_Arena* arena, MD_String8List* path, MD_PathStyle style) { return md_str8_path_list_join_by_style__ainfo(md_arena_allocator(arena), path, style); }

////////////////////////////////
//~ rjf: UTF-8 & UTF-16 Decoding/Encoding

inline MD_U8
md_utf8_class(MD_U8 value)
{
	md_local_persist
	MD_U8 lookup_table[32] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,2,2,2,3,3,4,5,
	};
	return lookup_table[value];
}

MD_API MD_UnicodeDecode md_utf8_decode           (MD_U8*  str,    MD_U64 md_max);
       MD_UnicodeDecode md_utf16_decode          (MD_U16* str,    MD_U64 md_max);
MD_API MD_U32           md_utf8_encode           (MD_U8*  str,    MD_U32 codepoint);
MD_API MD_U32           md_utf16_encode          (MD_U16* str,    MD_U32 codepoint);
       MD_U32           md_utf8_from_utf32_single(MD_U8*  buffer, MD_U32 character);

inline MD_UnicodeDecode
md_utf16_decode(MD_U16* str, MD_U64 md_max) {
	MD_UnicodeDecode result = {1, MD_MAX_U32};
	result.codepoint = str[0];
	result.inc       = 1;
	if (md_max > 1 && 0xD800 <= str[0] && str[0] < 0xDC00 && 0xDC00 <= str[1] && str[1] < 0xE000) {
		result.codepoint = ((str[0] - 0xD800) << 10) | 
		                   ((str[1] - 0xDC00) + 0x10000);
		result.inc = 2;
	}
	return(result);
}

inline MD_U32 md_utf8_from_utf32_single(MD_U8* buffer, MD_U32 character){ return(md_utf8_encode(buffer, character)); }

////////////////////////////////
//~ rjf: Unicode String Conversions

#define md_str8_from_str16(allocator, md_string_in) _Generic(allocator, MD_Arena*: md_str8_from_str16__arena, MD_AllocatorInfo: md_str8_from_str16__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, md_string_in)
#define md_str16_from_str8(allocator, md_string_in) _Generic(allocator, MD_Arena*: md_str16_from_str8__arena, MD_AllocatorInfo: md_str16_from_str8__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, md_string_in)
#define md_str8_from_str32(allocator, md_string_in) _Generic(allocator, MD_Arena*: md_str8_from_str32__arena, MD_AllocatorInfo: md_str8_from_str32__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, md_string_in)
#define md_str32_from_str8(allocator, md_string_in) _Generic(allocator, MD_Arena*: md_str32_from_str8__arena, MD_AllocatorInfo: md_str32_from_str8__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, md_string_in)

MD_API MD_String8  md_str8_from_str16__arena(MD_Arena* arena, MD_String16 in);
MD_API MD_String16 md_str16_from_str8__arena(MD_Arena* arena, MD_String8  in);
MD_API MD_String8  md_str8_from_str32__arena(MD_Arena* arena, MD_String32 in);
MD_API MD_String32 md_str32_from_str8__arena(MD_Arena* arena, MD_String8  in);

MD_API MD_String8  md_str8_from_str16__ainfo(MD_AllocatorInfo ainfo, MD_String16 in);
MD_API MD_String16 md_str16_from_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8  in);
MD_API MD_String8  md_str8_from_str32__ainfo(MD_AllocatorInfo ainfo, MD_String32 in);
MD_API MD_String32 md_str32_from_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8  in);

////////////////////////////////
//~ String -> Enum Conversions

MD_API MD_OperatingSystem md_operating_system_from_string(MD_String8 string);

////////////////////////////////
//~ rjf: Basic Types & Space Enum -> String Conversions

MD_String8 md_string_from_dimension       (MD_Dimension       dimension);
MD_String8 md_string_from_side            (MD_Side            side);
MD_String8 md_string_from_operating_system(MD_OperatingSystem os);
MD_String8 md_string_from_architecture    (MD_Arch            arch);

inline MD_String8
md_string_from_dimension(MD_Dimension dimension) {
	md_local_persist MD_String8 strings[] = {
		md_str8_lit_comp("X"),
		md_str8_lit_comp("Y"),
		md_str8_lit_comp("Z"),
		md_str8_lit_comp("W"),
	};
	MD_String8 result = md_str8_lit("error");
	if ((MD_U32)dimension < 4) {
		result = strings[dimension];
	}
	return(result);
}

inline MD_String8
md_string_from_side(MD_Side side) {
	md_local_persist MD_String8 strings[] = {
		md_str8_lit_comp("Min"),
		md_str8_lit_comp("Max"),
	};
	MD_String8 result = md_str8_lit("error");
	if ((MD_U32)side < 2) {
		result = strings[side];
	}
	return(result);
}

inline MD_String8
md_string_from_operating_system(MD_OperatingSystem os) {
	md_local_persist MD_String8 strings[] = {
		md_str8_lit_comp("Null"),
		md_str8_lit_comp("Windows"),
		md_str8_lit_comp("Linux"),
		md_str8_lit_comp("Mac"),
	};
	MD_String8 result = md_str8_lit("error");
	if (os < MD_OperatingSystem_COUNT) {
		result = strings[os];
	}
	return(result);
}

inline MD_String8
md_string_from_architecture(MD_Arch arch) {
	md_local_persist MD_String8 strings[] = {
		md_str8_lit_comp("Null"),
		md_str8_lit_comp("x64"),
		md_str8_lit_comp("x86"),
		md_str8_lit_comp("arm64"),
		md_str8_lit_comp("arm32"),
	};
	MD_String8 result = md_str8_lit("error");
	if (arch < MD_Arch_COUNT) {
		result = strings[arch];
	}
	return(result);
}

////////////////////////////////
//~ rjf: Time Types -> String

inline MD_String8
md_string_from_week_day(MD_WeekDay week_day)
{
	md_local_persist MD_String8 strings[] = {
		md_str8_lit_comp("Sun"),
		md_str8_lit_comp("Mon"),
		md_str8_lit_comp("Tue"),
		md_str8_lit_comp("Wed"),
		md_str8_lit_comp("Thu"),
		md_str8_lit_comp("Fri"),
		md_str8_lit_comp("Sat"),
	};
	MD_String8 result = md_str8_lit("Err");
	if ((MD_U32)week_day < MD_WeekDay_COUNT) {
		result = strings[week_day];
	}
	return(result);
}

inline MD_String8
md_string_from_month(MD_Month month) 
{
	md_local_persist MD_String8 strings[] = {
		md_str8_lit_comp("Jan"),
		md_str8_lit_comp("Feb"),
		md_str8_lit_comp("Mar"),
		md_str8_lit_comp("Apr"),
		md_str8_lit_comp("May"),
		md_str8_lit_comp("Jun"),
		md_str8_lit_comp("Jul"),
		md_str8_lit_comp("Aug"),
		md_str8_lit_comp("Sep"),
		md_str8_lit_comp("Oct"),
		md_str8_lit_comp("Nov"),
		md_str8_lit_comp("Dec"),
	};
	MD_String8 result = md_str8_lit("Err");
	if ((MD_U32)month < MD_Month_COUNT) {
		result = strings[month];
	}
	return(result);
}

MD_String8 md_date_time_string__arena          (MD_Arena* arena, MD_DateTime* date_time);
MD_String8 md_file_name_date_time_string__arena(MD_Arena* arena, MD_DateTime* date_time);
MD_String8 md_string_from_elapsed_time__arena  (MD_Arena* arena, MD_DateTime  date_time);

MD_API MD_String8 md_date_time_string__ainfo          (MD_AllocatorInfo ainfo, MD_DateTime* date_time);
MD_API MD_String8 md_file_name_date_time_string__ainfo(MD_AllocatorInfo ainfo, MD_DateTime* date_time);
MD_API MD_String8 md_string_from_elapsed_time__ainfo  (MD_AllocatorInfo ainfo, MD_DateTime  date_time);

#define md_date_time_string(allocator, date_time)         _Generic(allocator, MD_Arena*: md_date_time_string__arena,           MD_AllocatorInfo: md_date_time_string__ainfo,           default: md_assert_generic_sel_fail) md_generic_call(allocator, date_time)
#define file_name_date_time_string(allocator, date_time)  _Generic(allocator, MD_Arena*: md_file_name_date_time_string__arena, MD_AllocatorInfo: md_file_name_date_time_string__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, date_time)
#define md_string_from_elapsed_time(allocator, date_time) _Generic(allocator, MD_Arena*: md_string_from_elapsed_time__arena,   MD_AllocatorInfo: md_string_from_elapsed_time__ainfo,   default: md_assert_generic_sel_fail) md_generic_call(allocator, date_time)

md_force_inline MD_String8 md_push_date_time_string__arena          (MD_Arena* arena, MD_DateTime* date_time) { return md_date_time_string__ainfo          (md_arena_allocator(arena), date_time); }
md_force_inline MD_String8 md_push_file_name_date_time_string__arena(MD_Arena* arena, MD_DateTime* date_time) { return md_file_name_date_time_string__ainfo(md_arena_allocator(arena), date_time); }
md_force_inline MD_String8 md_string_from_elapsed_time__arena       (MD_Arena* arena, MD_DateTime  dt)        { return md_string_from_elapsed_time__ainfo  (md_arena_allocator(arena), dt); }

////////////////////////////////
//~ Globally Unique Ids

inline MD_String8
md_string_from_guid__arena(MD_Arena* arena, MD_Guid guid) {
	MD_String8 result = md_str8f(arena, 
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

inline MD_String8
md_string_from_guid__ainfo(MD_AllocatorInfo ainfo, MD_Guid guid) {
	MD_String8 result = md_str8f(ainfo, 
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

#define md_string_from_guid(allocator, guid) _Generic(allocator, MD_Arena*: md_string_from_guid__arena, MD_AllocatorInfo: md_string_from_guid__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, guid)

inline MD_Guid md_guid_from_string(MD_String8 string) { MD_Guid guid = {0}; md_try_guid_from_string(string, &guid); return guid; }

MD_API MD_B32 md_try_guid_from_string(MD_String8 string, MD_Guid* md_guid_out);

////////////////////////////////
//~ rjf: Basic Text Indentation

       MD_String8 md_indented_from_string__arena(MD_Arena*        arena, MD_String8 string);
MD_API MD_String8 md_indented_from_string__ainfo(MD_AllocatorInfo arena, MD_String8 string);

#define md_indented_from_string(allocator, string) _Generic(allocator, MD_Arena*: md_indented_from_string__arena, MD_AllocatorInfo: md_indented_from_string__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, string)

md_force_inline MD_String8 md_indented_from_string__arena(MD_Arena* arena, MD_String8 string) { return md_indented_from_string__ainfo(md_arena_allocator(arena), string); }

////////////////////////////////
//~ rjf: Text Escaping

       MD_String8 md_escaped_from_raw_str8__arena(MD_Arena*        arena, MD_String8 string);
MD_API MD_String8 md_escaped_from_raw_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8 string);
       MD_String8 md_raw_from__escaped_str8__arena(MD_Arena*        arena, MD_String8 string);
MD_API MD_String8 md_raw_from__escaped_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8 string);

#define md_escaped_from_raw_str8(allocator, string)  _Generic(allocator, MD_Arena*: md_escaped_from_raw_str8__arena,  MD_AllocatorInfo: md_escaped_from_raw_str8__ainfo,  default: md_assert_generic_sel_fail) md_generic_call(allocator, string)
#define md_raw_from__escaped_str8(allocator, string) _Generic(allocator, MD_Arena*: md_raw_from__escaped_str8__arena, MD_AllocatorInfo: md_raw_from__escaped_str8__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, string)

md_force_inline MD_String8 md_escaped_from_raw_str8__arena (MD_Arena* arena, MD_String8 string) { return md_escaped_from_raw_str8__ainfo (md_arena_allocator(arena), string); }
md_force_inline MD_String8 md_raw_from__escaped_str8__arena(MD_Arena* arena, MD_String8 string) { return md_raw_from__escaped_str8__ainfo(md_arena_allocator(arena), string); }

////////////////////////////////
//~ rjf: Text Wrapping

       MD_String8List md_wrapped_lines_from_string__arena(MD_Arena*        arena, MD_String8 string, MD_U64 first_line_max_width, MD_U64 max_width, MD_U64 wrap_indent);
MD_API MD_String8List md_wrapped_lines_from_string__ainfo(MD_AllocatorInfo ainfo, MD_String8 string, MD_U64 first_line_max_width, MD_U64 max_width, MD_U64 wrap_indent);

#define md_wrapped_lines_from_string(allocator, string, first_line_max_width, max_width, wrap_indent) \
_Generic(allocator, MD_Arena*: md_wrapped_lines_from_string__arena, MD_AllocatorInfo: md_wrapped_lines_from_string__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, string, first_line_max_width, max_width, wrap_indent)

md_force_inline MD_String8List md_wrapped_lines_from_string__arena(MD_Arena* arena, MD_String8 string, MD_U64 first_line_max_width, MD_U64 max_width, MD_U64 wrap_indent) 
{ return md_wrapped_lines_from_string__ainfo(md_arena_allocator(arena), string, first_line_max_width, max_width, wrap_indent); }

////////////////////////////////
//~ rjf: String <-> Color

inline MD_String8 md_hex_string_from_rgba_4f32__arena(MD_Arena*        arena, MD_Vec4F32 rgba) { MD_String8 hex_string = md_str8f(arena, "%02x%02x%02x%02x", (MD_U8)(rgba.x * 255.f), (MD_U8)(rgba.y * 255.f), (MD_U8)(rgba.z * 255.f), (MD_U8)(rgba.w * 255.f));  return hex_string; }
inline MD_String8 md_hex_string_from_rgba_4f32__ainfo(MD_AllocatorInfo ainfo, MD_Vec4F32 rgba) { MD_String8 hex_string = md_str8f(ainfo, "%02x%02x%02x%02x", (MD_U8)(rgba.x * 255.f), (MD_U8)(rgba.y * 255.f), (MD_U8)(rgba.z * 255.f), (MD_U8)(rgba.w * 255.f));  return hex_string; }

#define md_hex_string_from_rgba_4f32(allocator, rgba) _Generic(allocator, MD_Arena*: md_hex_string_from_rgba_4f32__arena, MD_AllocatorInfo: md_hex_string_from_rgba_4f32__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, rgba)

MD_API MD_Vec4F32 md_rgba_from_hex_string_4f32(MD_String8 hex_string);

////////////////////////////////
//~ rjf: String Fuzzy Matching

       MD_FuzzyMatchRangeList md_fuzzy_match_find__arena           (MD_Arena*        arena, MD_String8 needle, MD_String8 haystack);
MD_API MD_FuzzyMatchRangeList md_fuzzy_match_find__ainfo           (MD_AllocatorInfo ainfo, MD_String8 needle, MD_String8 haystack);
       MD_FuzzyMatchRangeList md_fuzzy_match_range_list_copy__arena(MD_Arena*        arena, MD_FuzzyMatchRangeList* src);
MD_API MD_FuzzyMatchRangeList md_fuzzy_match_range_list_copy__ainfo(MD_AllocatorInfo ainfo, MD_FuzzyMatchRangeList* src);

#define md_fuzzy_match_find(allocator, needle, haystack) _Generic(allocator, MD_Arena*: md_fuzzy_match_find__arena,            MD_AllocatorInfo: md_fuzzy_match_find__ainfo)            md_generic_call(allocator, needle, haystack)
#define md_fuzzy_match_range_list_copy(allocator, src)   _Generic(allocator, MD_Arena*: md_fuzzy_match_range_list_copy__arena, MD_AllocatorInfo: md_fuzzy_match_range_list_copy__ainfo) md_generic_call(allocator, src)

md_force_inline MD_FuzzyMatchRangeList md_fuzzy_match_find__arena           (MD_Arena *arena, MD_String8 needle, MD_String8 haystack) { return md_fuzzy_match_find__ainfo           (md_arena_allocator(arena), needle, haystack); }
md_force_inline MD_FuzzyMatchRangeList md_fuzzy_match_range_list_copy__arena(MD_Arena* arena, MD_FuzzyMatchRangeList* src)            { return md_fuzzy_match_range_list_copy__ainfo(md_arena_allocator(arena), src); }

////////////////////////////////
//~ NOTE(allen): Serialization Helpers

#define md_str8_serial_push_array(allocator, srl, ptr, count) md_str8_serial_push_data (allocator, srl, ptr, sizeof(*(ptr)) * (count))
#define md_str8_serial_push_struct(allocator, srl, ptr)       md_str8_serial_push_array(allocator, srl, ptr, 1)

inline void
md_str8_serial_write_to_dst(MD_String8List* srl, void* out) {
	MD_U8* ptr = (MD_U8*)out;
	for (MD_String8Node *node = srl->first; node != 0; node = node->next) 
	{
		MD_U64 size = node->string.size;
		md_memory_copy(ptr, node->string.str, size);
		ptr += size;
	}
}

void       md_str8_serial_begin__arena         (MD_Arena* arena, MD_String8List* srl);
MD_String8 md_str8_serial_end__arena           (MD_Arena* arena, MD_String8List* srl);
MD_U64     md_str8_serial_push_align__arena    (MD_Arena* arena, MD_String8List* srl, MD_U64 align);
void*      md_str8_serial_push_size__arena     (MD_Arena* arena, MD_String8List* srl, MD_U64 size);
void*      md_str8_serial_push_data__arena     (MD_Arena* arena, MD_String8List* srl, void* data, MD_U64 size);
void       md_str8_serial_push_data_list__arena(MD_Arena* arena, MD_String8List* srl, MD_String8Node* first);
void       md_str8_serial_push_u64__arena      (MD_Arena* arena, MD_String8List* srl, MD_U64 x);
void       md_str8_serial_push_u32__arena      (MD_Arena* arena, MD_String8List* srl, MD_U32 x);
void       md_str8_serial_push_u16__arena      (MD_Arena* arena, MD_String8List* srl, MD_U16 x);
void       md_str8_serial_push_u8__arena       (MD_Arena* arena, MD_String8List* srl, MD_U8 x);
void       md_str8_serial_push_cstr__arena     (MD_Arena* arena, MD_String8List* srl, MD_String8 str);
void       md_str8_serial_push_string__arena   (MD_Arena* arena, MD_String8List* srl, MD_String8 str);

       void       md_str8_serial_begin__ainfo         (MD_AllocatorInfo ainfo, MD_String8List* srl);
       MD_String8 md_str8_serial_end__ainfo           (MD_AllocatorInfo ainfo, MD_String8List* srl);
MD_API MD_U64     md_str8_serial_push_align__ainfo    (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U64 align);
MD_API void*      md_str8_serial_push_size__ainfo     (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U64 size);
       void*      md_str8_serial_push_data__ainfo     (MD_AllocatorInfo ainfo, MD_String8List* srl, void* data, MD_U64 size);
       void       md_str8_serial_push_data_list__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, MD_String8Node* first);
MD_API void       md_str8_serial_push_u64__ainfo      (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U64 x);
MD_API void       md_str8_serial_push_u32__ainfo      (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U32 x);
       void       md_str8_serial_push_u16__ainfo      (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U16 x);
       void       md_str8_serial_push_u8__ainfo       (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U8 x);
       void       md_str8_serial_push_cstr__ainfo     (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_String8 str);
       void       md_str8_serial_push_string__ainfo   (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_String8 str);

#define md_str8_serial_begin(allocator, srl)                  _Generic(allocator, MD_Arena*: md_str8_serial_begin__arena,          MD_AllocatorInfo: md_str8_serial_begin__ainfo,          default: md_assert_generic_sel_fail) md_generic_call(allocator, srl)
#define md_str8_serial_end(allocator, srl)                    _Generic(allocator, MD_Arena*: md_str8_serial_end__arena,            MD_AllocatorInfo: md_str8_serial_end__ainfo,            default: md_assert_generic_sel_fail) md_generic_call(allocator, srl)
#define md_str8_serial_push_align(allocator, srl, align)      _Generic(allocator, MD_Arena*: md_str8_serial_push_align__arena,     MD_AllocatorInfo: md_str8_serial_push_align__ainfo,     default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, align)
#define md_str8_serial_push_size(allocator, srl, size)        _Generic(allocator, MD_Arena*: md_str8_serial_push_size__arena,      MD_AllocatorInfo: md_str8_serial_push_size__ainfo,      default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, size)
#define md_str8_serial_push_data(allocator, srl, data, usize) _Generic(allocator, MD_Arena*: md_str8_serial_push_data__arena,      MD_AllocatorInfo: md_str8_serial_push_data__ainfo,      default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, data, usize)
#define md_str8_serial_push_data_list(allocator, srl, first)  _Generic(allocator, MD_Arena*: md_str8_serial_push_data_list__arena, MD_AllocatorInfo: md_str8_serial_push_data_list__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, first)
#define md_str8_serial_push_u64(allocator, srl, x)            _Generic(allocator, MD_Arena*: md_str8_serial_push_u64__arena,       MD_AllocatorInfo: md_str8_serial_push_u64__ainfo,       default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, x)
#define md_str8_serial_push_u32(allocator, srl, x)            _Generic(allocator, MD_Arena*: md_str8_serial_push_u32__arena,       MD_AllocatorInfo: md_str8_serial_push_u32__ainfo,       default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, x)
#define md_str8_serial_push_u16(allocator, srl, x)            _Generic(allocator, MD_Arena*: md_str8_serial_push_u16__arena,       MD_AllocatorInfo: md_str8_serial_push_u16__ainfo,       default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, x)
#define md_str8_serial_push_u8(allocator, srl, x)             _Generic(allocator, MD_Arena*: md_str8_serial_push_u8__arena,        MD_AllocatorInfo: md_str8_serial_push_u8__ainfo,        default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, x)
#define md_str8_serial_push_cstr(allocator, srl, str)         _Generic(allocator, MD_Arena*: md_str8_serial_push_cstr__arena,      MD_AllocatorInfo: md_str8_serial_push_cstr__ainfo,      default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, str)
#define md_str8_serial_push_string(allocator, slr, str)       _Generic(allocator, MD_Arena*: md_str8_serial_push_string__arena,    MD_AllocatorInfo: md_str8_serial_push_string__ainfo,    default: md_assert_generic_sel_fail) md_generic_call(allocator, srl, str)

md_force_inline MD_U64 md_str8_serial_push_align__arena(MD_Arena* arena, MD_String8List* srl, MD_U64 align) { return md_str8_serial_push_align__ainfo(md_arena_allocator(arena), srl, align); }
md_force_inline void*  md_str8_serial_push_size__arena (MD_Arena* arena, MD_String8List* srl, MD_U64 size)  { return md_str8_serial_push_size__ainfo (md_arena_allocator(arena), srl, size); }
md_force_inline void   md_str8_serial_push_u64__arena  (MD_Arena* arena, MD_String8List* srl, MD_U64 x)     {        md_str8_serial_push_u64__ainfo  (md_arena_allocator(arena), srl, x); }
md_force_inline void   md_str8_serial_push_u32__arena  (MD_Arena* arena, MD_String8List* srl, MD_U32 x)     {        md_str8_serial_push_u32__ainfo  (md_arena_allocator(arena), srl, x); }

md_force_inline void       md_str8_serial_begin__arena(MD_Arena* arena, MD_String8List* srl) {        md_str8_serial_begin__ainfo(md_arena_allocator(arena), srl); }
md_force_inline MD_String8 md_str8_serial_end__arena  (MD_Arena* arena, MD_String8List* srl) { return md_str8_serial_end__ainfo  (md_arena_allocator(arena), srl); }

md_force_inline void md_str8_serial_push_u16__arena   (MD_Arena* arena, MD_String8List* srl, MD_U16     x)   { md_str8_serial_push_data__arena(arena, srl, &x, sizeof(x)); }
md_force_inline void md_str8_serial_push_u8__arena    (MD_Arena* arena, MD_String8List* srl, MD_U8      x)   { md_str8_serial_push_data__arena(arena, srl, &x, sizeof(x)); }
md_force_inline void md_str8_serial_push_cstr__arena  (MD_Arena* arena, MD_String8List* srl, MD_String8 str) { md_str8_serial_push_data__arena(arena, srl, str.str, str.size);  md_str8_serial_push_u8__arena(arena, srl, 0); } 
md_force_inline void md_str8_serial_push_string__arena(MD_Arena* arena, MD_String8List* srl, MD_String8 str) { md_str8_serial_push_data__arena(arena, srl, str.str, str.size); }

md_force_inline void md_str8_serial_push_u16__ainfo   (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U16     x)   { md_str8_serial_push_data__ainfo(ainfo, srl, &x, sizeof(x)); }
md_force_inline void md_str8_serial_push_u8__ainfo    (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U8      x)   { md_str8_serial_push_data__ainfo(ainfo, srl, &x, sizeof(x)); }
md_force_inline void md_str8_serial_push_cstr__ainfo  (MD_AllocatorInfo ainfo, MD_String8List* srl, MD_String8 str) { md_str8_serial_push_data__ainfo(ainfo, srl, str.str, str.size);  md_str8_serial_push_u8__ainfo(ainfo, srl, 0); } 
md_force_inline void md_str8_serial_push_string__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, MD_String8 str) { md_str8_serial_push_data__ainfo(ainfo, srl, str.str, str.size); }
	
inline void*
md_str8_serial_push_data__arena(MD_Arena* arena, MD_String8List* srl, void* data, MD_U64 size) {
	void* result = md_str8_serial_push_size__arena(arena, srl, size);
	if(result != 0) {
		md_memory_copy(result, data, size);
	}
	return result;
}

inline void
md_str8_serial_push_data_list__arena(MD_Arena* arena, MD_String8List* srl, MD_String8Node* first) {
	for (MD_String8Node* node = first; node != 0; node = node->next) {
		md_str8_serial_push_data__arena(arena, srl, node->string.str, node->string.size);
	}
}

inline void 
md_str8_serial_begin__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl) { 
	MD_String8Node* node = md_alloc_array(ainfo, MD_String8Node, 1);
	node->string.str = md_alloc_array_no_zero(ainfo, MD_U8, 0);
	srl->first       = srl->last = node;
	srl->node_count  = 1;
	srl->total_size  = 0;
}

inline MD_String8
md_str8_serial_end_alloc(MD_AllocatorInfo ainfo, MD_String8List* srl) {
	MD_U64 size = srl->total_size;
	MD_U8* out  = md_alloc_array_no_zero(ainfo, MD_U8, size);
	md_str8_serial_write_to_dst(srl, out);
	MD_String8 result = md_str8(out, size);
	return  result;
}

inline void*
md_str8_serial_push_data__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, void* data, MD_U64 size) {
	void* result = md_str8_serial_push_size(ainfo, srl, size);
	if(result != 0) {
		md_memory_copy(result, data, size);
	}
	return result;
}

inline void
md_str8_serial_push_data_list__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, MD_String8Node* first) {
	for (MD_String8Node* node = first; node != 0; node = node->next) {
		md_str8_serial_push_data__ainfo(ainfo, srl, node->string.str, node->string.size);
	}
}

////////////////////////////////
//~ rjf: Deserialization Helpers

#define md_str8_deserial_read_array(string, off, ptr, count) md_str8_deserial_read((string), (off), (ptr), sizeof(*(ptr)) * (count), sizeof( *(ptr)))
#define md_str8_deserial_read_struct(string, off, ptr)       md_str8_deserial_read((string), (off), (ptr), sizeof(*(ptr)), sizeof( *(ptr)))

MD_API MD_U64   md_str8_deserial_read                       (MD_String8 string, MD_U64 off, void* read_dst, MD_U64 read_size, MD_U64 granularity);
MD_API MD_U64   md_str8_deserial_find_first_match           (MD_String8 string, MD_U64 off, MD_U16 scan_val);
       void* md_str8_deserial_get_raw_ptr                   (MD_String8 string, MD_U64 off, MD_U64 size);
MD_API MD_U64   md_str8_deserial_read_cstr                  (MD_String8 string, MD_U64 off, MD_String8* cstr_out);
MD_API MD_U64   md_str8_deserial_read_windows_utf16_string16(MD_String8 string, MD_U64 off, MD_String16* md_str_out);
       MD_U64   md_str8_deserial_read_block                 (MD_String8 string, MD_U64 off, MD_U64 size, MD_String8* block_out);
MD_API MD_U64   md_str8_deserial_read_uleb128               (MD_String8 string, MD_U64 off, MD_U64* value_out);
MD_API MD_U64   md_str8_deserial_read_sleb128               (MD_String8 string, MD_U64 off, MD_S64* value_out);

inline void*  md_str8_deserial_get_raw_ptr(MD_String8 string, MD_U64 off, MD_U64 size)                        { void* raw_ptr = 0; if (off + size <= string.size) { raw_ptr = string.str + off; }   return raw_ptr; }
inline MD_U64 md_str8_deserial_read_block (MD_String8 string, MD_U64 off, MD_U64 size, MD_String8* block_out) { MD_Rng1U64 range = md_rng_1u64(off, off + size); *block_out = md_str8_substr(string, range); return block_out->size; }

////////////////////////////////
// Second-order Generic Selectors

#define md_str8_from(allocator, in) \
_Generic((in),                      \
    MD_SSIZE   : _Generic(allocator, MD_Arena*: md_str8_from_memory_size__arena, MD_AllocatorInfo: md_str8_from_memory_size__ainfo, default: md_assert_generic_sel_fail), \
_Generic_L2((in),                                                                                                                                             \
	MD_String16: _Generic(allocator, MD_Arena*: md_str8_from_str16__arena, MD_AllocatorInfo: md_str8_from_str16__ainfo, default: md_assert_generic_sel_fail), \
	MD_String32: _Generic(allocator, MD_Arena*: md_str8_from_str32__arena, MD_AllocatorInfo: md_str8_from_str32__ainfo, default: md_assert_generic_sel_fail), \
	MD_U64     : _Generic(allocator, MD_Arena*: md_str8_from_u64__arena,   MD_AllocatorInfo: md_str8_from_u64__ainfo,   default: md_assert_generic_sel_fail), \
	MD_S64     : _Generic(allocator, MD_Arena*: md_str8_from_s64__arena,   MD_AllocatorInfo: md_str8_from_s64__ainfo,   default: md_assert_generic_sel_fail), \
	default : md_assert_generic_sel_fail)                                                                                                                     \
) md_generic_call(allocator, in)

#define md_str16_from(allocator, in) \
_Generic(in,                         \
	MD_String8: _Generic(allocator, MD_Arena*: md_str16_from_str8__arena, MD_AllocatorInfo: md_str16_from_str8__ainfo,  default: md_assert_generic_sel_fail), \
	default:  md_assert_generic_sel_fail \
) md_generic_call(allocator, in)
