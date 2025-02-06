#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "platform.h"
#	include "macros.h"
#	include "generic_macros.h"
#	include "base_types.h"
#	include "constants.h"
#	include "arena.h"
#	include "space.h"
#	include "math.h"
#	include "toolchain.h"
#	include "time.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Third Party Includes

#define STB_SPRINTF_DECORATE(name) raddbg_##name
#include "third_party/stb/stb_sprintf.h"

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
//~ rjf: Character Classification & Conversion Functions

inline B32 char_is_space        (U8 c) { return(c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v'); }
inline B32 char_is_upper        (U8 c) { return('A' <= c && c <= 'Z'); }
inline B32 char_is_lower        (U8 c) { return('a' <= c && c <= 'z'); }
inline B32 char_is_alpha        (U8 c) { return(char_is_upper(c) || char_is_lower(c)); }
inline B32 char_is_slash        (U8 c) { return(c == '/' || c == '\\'); }
inline U8  char_to_lower        (U8 c) { if (char_is_upper(c)) { c += ('a' - 'A'); } return(c); }
inline U8  char_to_upper        (U8 c) { if (char_is_lower(c)) { c += ('A' - 'a'); } return(c); }
inline U8  char_to_correct_slash(U8 c) { if (char_is_slash(c)) { c = '/';          } return(c); }

B32 char_is_digit(U8 c, U32 base);

////////////////////////////////
//~ rjf: C-String Measurement

inline U64 cstring8_length (U8*  c) { U8*  p = c; for (; *p != 0; p += 1); return(p - c); }
inline U64 cstring16_length(U16* c) { U16* p = c; for (; *p != 0; p += 1); return(p - c); }
inline U64 cstring32_length(U32 *c) { U32* p = c; for (; *p != 0; p += 1); return(p - c); }

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

inline String8
str8_cstring_capped(void *cstr, void *cap) {
  char *ptr = (char*)cstr;
  char *opl = (char*)cap;
  for (;ptr < opl && *ptr != 0; ptr += 1);
  U64 size = (U64)(ptr - (char *)cstr);
  String8 result = {(U8*)cstr, size};
  return(result);
}

////////////////////////////////
//~ rjf: String Matching

MD_API B32 str8_match      (String8 a, String8 b,                          StringMatchFlags flags);
MD_API U64 str8_find_needle(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags);
       B32 str8_ends_with  (String8 string, String8 end,                   StringMatchFlags flags);

inline B32
str8_ends_with(String8 string, String8 end, StringMatchFlags flags) {
  String8 postfix  = str8_postfix(string, end.size);
  B32     is_match = str8_match(end, postfix, flags);
  return  is_match;
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
//~ rjf: String Formatting & Copying

MD_API String8 push_str8_cat (Arena* arena, String8 s1, String8 s2);
MD_API String8 push_str8_copy(Arena* arena, String8 s);
MD_API String8 push_str8fv   (Arena* arena, char* fmt, va_list args);
       String8 push_str8f    (Arena* arena, char* fmt, ...);

MD_API String8 str8_cat (AllocatorInfo ainfo, String8 s1, String8 s2);
MD_API String8 str8_copy(AllocatorInfo ainfo, String8 s);
MD_API String8 str8fv   (AllocatorInfo ainfo, char* fmt, va_list args);
       String8 str8f    (AllocatorInfo ainfo, char* fmt, ...);

inline String8
push_str8f(Arena *arena, char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	String8 result = push_str8fv(arena, fmt, args);
	va_end(args);
	return(result);
}

inline String8
str8f(AllocatorInfo ainfo, char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	String8 result = str8fv(ainfo, fmt, args);
	va_end(args);
	return(result);
}

////////////////////////////////
//~ rjf: String Stylization

inline String8 upper_from_str8      (Arena* arena, String8 string) { string = push_str8_copy(arena, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_to_upper(string.str[idx]);                          } return string; }
inline String8 lower_from_str8      (Arena* arena, String8 string) { string = push_str8_copy(arena, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_to_lower(string.str[idx]);                          } return string; }
inline String8 backslashed_from_str8(Arena *arena, String8 string) { string = push_str8_copy(arena, string); for(U64 idx = 0; idx < string.size; idx += 1) { string.str[idx] = char_is_slash(string.str[idx]) ? '\\' : string.str[idx]; } return string; }

////////////////////////////////
//~ rjf: String <=> Integer Conversions

//- rjf: string -> integer
MD_API S64 sign_from_str8           (String8 string, String8* string_tail);
MD_API B32 str8_is_integer          (String8 string, U32      radix);
MD_API U64 u64_from_str8            (String8 string, U32      radix);
       S64 s64_from_str8            (String8 string, U32      radix);
MD_API B32 try_u64_from_str8_c_rules(String8 string, U64*     x);
       B32 try_s64_from_str8_c_rules(String8 string, S64*     x);

//- rjf: integer -> string
MD_API String8 str8_from_memory_size(Arena *arena, U64 z);
MD_API String8 str8_from_u64        (Arena *arena, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);
MD_API String8 str8_from_s64        (Arena *arena, S64 s64, U32 radix, U8 min_digits, U8 digit_group_separator);

String8 str8_from_allocator_size(U64 z, AllocatorInfo ainfo);
String8 str8_from_allocator_u64 (AllocatorInfo ainfo, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);
String8 str8_from_alloctor_s64  (AllocatorInfo ainfo, S64 u64, U32 radix, U8 min_digits, U8 digit_group_separator);

inline U64
u64_from_str8(String8 string, U32 radix) {
	U64 x = 0;
	if (1 < radix && radix <= 16) {
		for (U64 i = 0; i < string.size; i += 1) {
			x *= radix;
			x += integer_symbol_reverse[string.str[i]&0x7F];
		}
	}
	return(x);
}

inline S64
s64_from_str8(String8 string, U32 radix) {
	S64 sign = sign_from_str8(string, &string);
	S64 x    = (S64)u64_from_str8(string, radix) * sign;
	return(x);
}

inline B32
try_s64_from_str8_c_rules(String8 string, S64 *x) {
  String8 string_tail = {0};
  S64     sign        = sign_from_str8(string, &string_tail);
  U64     x_u64       = 0;
  B32     is_integer  = try_u64_from_str8_c_rules(string_tail, &x_u64);
  *x = x_u64*sign;
  return(is_integer);
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
void         str8_list_concat_in_place           (String8List *list, String8List* to_push);

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

String8Node* str8_list_push        (Arena* arena, String8List* list, String8 string);
String8Node* str8_list_push_front  (Arena* arena, String8List* list, String8 string);
String8Node* str8_list_push_aligner(Arena* arena, String8List* list, U64 min, U64 align);
String8Node* str8_list_pushf       (Arena* arena, String8List* list, char* fmt, ...);
String8Node* str8_list_push_frontf (Arena* arena, String8List* list, char* fmt, ...);
String8List  str8_list_copy        (Arena* arena, String8List* list);

String8Node* str8_list_alloc        (AllocatorInfo ainfo, String8List* list, String8 string);
String8Node* str8_list_alloc_front  (AllocatorInfo ainfo, String8List* list, String8 string);
String8Node* str8_list_alloc_aligner(AllocatorInfo ainfo, String8List* list, U64 min, U64 align);

inline String8Node*
str8_list_push(Arena* arena, String8List* list, String8 string) {
	String8Node* node = push_array_no_zero(arena, String8Node, 1);
	str8_list_push_node_set_string(list, node, string);
	return(node);
}

inline String8Node*
str8_list_push_front(Arena* arena, String8List* list, String8 string) {
  String8Node *node = push_array_no_zero(arena, String8Node, 1);
  str8_list_push_node_front_set_string(list, node, string);
  return(node);
}

////////////////////////////////
//~ rjf: String Splitting & Joining

String8List str8_split_arena(Arena *arena, String8 string, U8 *split_chars, U64 split_char_count, StringSplitFlags flags);

// Example pattern for general allocator variants

// String8List  str8__split_ainfo(String8 string, U8* split_chars, U64 split_char_count, StringSplitFlags flags, AllocatorInfo allocator);
// #define      str8_split_ainfo(string, split_chars, split_char_count, flags, ...) str8__split_ainfo(string, split_chars, split_char_count, flags, (AllocatorInfo){__VA_ARGS__});

internal String8List  str8_split_by_string_chars     (Arena *arena, String8 string, String8 split_chars, StringSplitFlags flags);
internal String8List  str8_list_split_by_string_chars(Arena *arena, String8List list, String8 split_chars, StringSplitFlags flags);
internal String8      str8_list_join                 (Arena *arena, String8List *list, StringJoin *optional_params);
internal void         str8_list_from_flags           (Arena *arena, String8List *list, U32 flags, String8 *flag_string_table, U32 flag_string_count);

////////////////////////////////
//~ rjf; String Arrays

internal String8Array str8_array_from_list(Arena *arena, String8List *list);
internal String8Array str8_array_reserve  (Arena *arena, U64 count);

////////////////////////////////
//~ rjf: String Path Helpers

internal String8 str8_chop_last_slash(String8 string);
internal String8 str8_skip_last_slash(String8 string);
internal String8 str8_chop_last_dot  (String8 string);
internal String8 str8_skip_last_dot  (String8 string);

internal PathStyle   path_style_from_str8                (String8 string);
internal String8List str8_split_path                     (Arena *arena, String8 string);
internal void        str8_path_list_resolve_dots_in_place(String8List *path, PathStyle style);
internal String8     str8_path_list_join_by_style        (Arena *arena, String8List *path, PathStyle style);

////////////////////////////////
//~ rjf: UTF-8 & UTF-16 Decoding/Encoding

internal UnicodeDecode utf8_decode           (U8*  str,    U64 max);
internal UnicodeDecode utf16_decode          (U16* str,    U64 max);
internal U32           utf8_encode           (U8*  str,    U32 codepoint);
internal U32           utf16_encode          (U16* str,    U32 codepoint);
internal U32           utf8_from_utf32_single(U8*  buffer, U32 character);

////////////////////////////////
//~ rjf: Unicode String Conversions

internal String8  str8_from_16(Arena* arena, String16 in);
internal String16 str16_from_8(Arena* arena, String8  in);
internal String8  str8_from_32(Arena* arena, String32 in);
internal String32 str32_from_8(Arena* arena, String8  in);

////////////////////////////////
//~ rjf: Basic Types & Space Enum -> String Conversions

internal String8 string_from_dimension(Dimension dimension);
internal String8 string_from_side(Side side);
internal String8 string_from_operating_system(OperatingSystem os);
internal String8 string_from_architecture(Architecture arch);

////////////////////////////////
//~ rjf: Time Types -> String

internal String8 string_from_week_day           (WeekDay week_day);
internal String8 string_from_month              (Month month);
internal String8 push_date_time_string          (Arena* arena, DateTime* date_time);
internal String8 push_file_name_date_time_string(Arena* arena, DateTime* date_time);
internal String8 string_from_elapsed_time       (Arena* arena, DateTime dt);

////////////////////////////////
//~ rjf: Basic Text Indentation

internal String8 indented_from_string(Arena *arena, String8 string);

////////////////////////////////
//~ rjf: String <-> Color

internal String8 hex_string_from_rgba_4f32(Arena *arena, Vec4F32 rgba);
internal Vec4F32 rgba_from_hex_string_4f32(String8 hex_string);

////////////////////////////////
//~ rjf: String Fuzzy Matching

internal FuzzyMatchRangeList fuzzy_match_find           (Arena *arena, String8 needle, String8 haystack);
internal FuzzyMatchRangeList fuzzy_match_range_list_copy(Arena *arena, FuzzyMatchRangeList *src);

////////////////////////////////
//~ NOTE(allen): Serialization Helpers

internal void    str8_serial_begin         (Arena *arena, String8List *srl);
internal String8 str8_serial_end           (Arena *arena, String8List *srl);
internal void    str8_serial_write_to_dst  (String8List *srl, void *out);
internal U64     str8_serial_push_align    (Arena *arena, String8List *srl, U64 align);
internal void*   str8_serial_push_size     (Arena *arena, String8List *srl, U64 size);
internal void*   str8_serial_push_data     (Arena *arena, String8List *srl, void *data, U64 size);
internal void    str8_serial_push_data_list(Arena *arena, String8List *srl, String8Node *first);
internal void    str8_serial_push_u64      (Arena *arena, String8List *srl, U64 x);
internal void    str8_serial_push_u32      (Arena *arena, String8List *srl, U32 x);
internal void    str8_serial_push_u16      (Arena *arena, String8List *srl, U16 x);
internal void    str8_serial_push_u8       (Arena *arena, String8List *srl, U8 x);
internal void    str8_serial_push_cstr     (Arena *arena, String8List *srl, String8 str);
internal void    str8_serial_push_string   (Arena *arena, String8List *srl, String8 str);

#define str8_serial_push_array(arena, srl, ptr, count) str8_serial_push_data (arena, srl, ptr, sizeof(*(ptr)) * (count))
#define str8_serial_push_struct(arena, srl, ptr)       str8_serial_push_array(arena, srl, ptr, 1)

////////////////////////////////
//~ rjf: Deserialization Helpers

internal U64   str8_deserial_read                       (String8 string, U64 off, void* read_dst, U64 read_size, U64 granularity);
internal U64   str8_deserial_find_first_match           (String8 string, U64 off, U16 scan_val);
internal void* str8_deserial_get_raw_ptr                (String8 string, U64 off, U64 size);
internal U64   str8_deserial_read_cstr                  (String8 string, U64 off, String8* cstr_out);
internal U64   str8_deserial_read_windows_utf16_string16(String8 string, U64 off, String16* str_out);
internal U64   str8_deserial_read_block                 (String8 string, U64 off, U64 size, String8* block_out);

#define str8_deserial_read_array(string, off, ptr, count) str8_deserial_read((string), (off), (ptr), sizeof(*(ptr)) * (count), sizeof( *(ptr)))
#define str8_deserial_read_struct(string, off, ptr)       str8_deserial_read((string), (off), (ptr), sizeof(*(ptr)), sizeof( *(ptr)))

////////////////////////////////
//~ rjf: Text 2D Coordinates & Ranges

typedef struct TxtPt TxtPt;
struct TxtPt
{
  S64 line;
  S64 column;
};

typedef struct TxtRng TxtRng;
struct TxtRng
{
  TxtPt min;
  TxtPt max;
};

////////////////////////////////
//~ rjf: String Pair Types

typedef struct String8TxtPtPair String8TxtPtPair;
struct String8TxtPtPair
{
  String8 string;
  TxtPt   pt;
};

////////////////////////////////
//~ rjf: Text Path Helpers

String8TxtPtPair str8_txt_pt_pair_from_string(String8 string);

////////////////////////////////
//~ rjf: Text Wrapping

String8List wrapped_lines_from_string(Arena *arena, String8 string, U64 first_line_max_width, U64 max_width, U64 wrap_indent);
