#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "strings.h"
#	include "thread_context.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Third Party Includes

// #if !BUILD_SUPPLEMENTARY_UNIT
// # define STB_SPRINTF_IMPLEMENTATION
// # define STB_SPRINTF_STATIC
// # include "third_party/stb/stb_sprintf.h"
// #endif

////////////////////////////////
//~ NOTE(allen): String <-> Integer Tables

read_only global U8 integer_symbols[16] = {
	'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',
};

// NOTE(allen): Includes reverses for uppercase and lowercase hex.
read_only global U8 integer_symbol_reverse[128] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};

read_only global U8 base64[64] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'_', '$',
};

read_only global U8 base64_reverse[128] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
	0xFF,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,
	0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0xFF,0xFF,0xFF,0xFF,0x3E,
	0xFF,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
	0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0xFF,0xFF,0xFF,0xFF,0xFF,
};

////////////////////////////////
//~ rjf: Character Classification & Conversion Functions

B32 char_is_digit(U8 c, U32 base) {
	B32 result = 0;
	if (0 < base && base <= 16) {
		U8 val = integer_symbol_reverse[c];
		if (val < base) {
			result = 1;
		}
	}
	return(result);
}

////////////////////////////////
//~ rjf: String Matching

B32
str8_match(String8 a, String8 b, StringMatchFlags flags)
{
	B32 result = 0;
	if (a.size == b.size || (flags & StringMatchFlag_RightSideSloppy))
	{
		B32 case_insensitive  = (flags & StringMatchFlag_CaseInsensitive);
		B32 slash_insensitive = (flags & StringMatchFlag_SlashInsensitive);
		U64 size              = min(a.size, b.size);
		result = 1;
		for (U64 i = 0; i < size; i += 1)
		{
			U8 at = a.str[i];
			U8 bt = b.str[i];
			if (case_insensitive) {
				at = char_to_upper(at);
				bt = char_to_upper(bt);
			}
			if (slash_insensitive) {
				at = char_to_correct_slash(at);
				bt = char_to_correct_slash(bt);
			}
			if (at != bt) {
				result = 0;
				break;
			}
		}
	}
	return(result);
}

U64
str8_find_needle(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags)
{
	U8* p           = string.str + start_pos;
	U64 stop_offset = max(string.size + 1, needle.size) - needle.size;
	U8* stop_p      = string.str + stop_offset;
	if (needle.size > 0)
	{
		U8*              string_opl     = string.str + string.size;
		String8          needle_tail    = str8_skip(needle, 1);
		StringMatchFlags adjusted_flags = flags | StringMatchFlag_RightSideSloppy;
		U8 needle_first_char_adjusted   = needle.str[0];
		if (adjusted_flags & StringMatchFlag_CaseInsensitive) {
			needle_first_char_adjusted = char_to_upper(needle_first_char_adjusted);
		}
		for (;p < stop_p; p += 1)
		{
			U8 haystack_char_adjusted = *p;
			if(adjusted_flags & StringMatchFlag_CaseInsensitive) {
				haystack_char_adjusted = char_to_upper(haystack_char_adjusted);
			}
			if (haystack_char_adjusted == needle_first_char_adjusted) {
				if (str8_match(str8_range(p + 1, string_opl), needle_tail, adjusted_flags)){
					break;
				}
			}
		}
	}
	U64 result = string.size;
	if (p < stop_p){
		result = (U64)(p - string.str);
	}
	return(result);
}

////////////////////////////////
//~ rjf: String Slicing

String8
str8_skip_chop_whitespace(String8 string)
{
	U8* first = string.str;
	U8* opl   = first + string.size;
	for (; first < opl; first += 1) {
		if ( ! char_is_space(*first)) { break;}
	}
	for (; opl > first; ){
		opl -= 1;
		if ( ! char_is_space(*opl)) { opl += 1; break; }
	}
	String8 result = str8_range(first, opl);
	return(result);
}

////////////////////////////////
//~ rjf: String Formatting & Copying

String8
push_str8_cat(Arena* arena, String8 s1, String8 s2)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8 str;
	str.size = s1.size + s2.size;
	str.str  = push_array_no_zero(arena, U8, str.size + 1);
	memory_copy(str.str, s1.str, s1.size);
	memory_copy(str.str + s1.size, s2.str, s2.size);
	str.str[str.size] = 0;
	return(str);
#else
	return str8_cat(arena_allocator(arena), s1, s2);
#endif
}

String8
push_str8_copy(Arena* arena, String8 s){
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8 str;
	str.size = s.size;
	str.str  = push_array_no_zero(arena, U8, str.size + 1);
	memory_copy(str.str, s.str, s.size);
	str.str[str.size] = 0;
	return(str);
#else
	return str8_copy(arena_allocator(arena), s);
#endif
}

String8
push_str8fv(Arena* arena, char* fmt, va_list args){
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	va_list args2;
	va_copy(args2, args);
	U32     needed_bytes = raddbg_vsnprintf(0, 0, fmt, args) + 1;
	String8 result       = {0};
	result.str  = push_array_no_zero(arena, U8, needed_bytes);
	result.size = raddbg_vsnprintf((char*)result.str, needed_bytes, fmt, args2);
	result.str[result.size] = 0;
	va_end(args2);
	return(result);
#else
	return str8fv(arena_allocator(arena), fmt, args);
#endif
}

String8
str8_cat(AllocatorInfo ainfo, String8 s1, String8 s2)
{
	String8 str;
	str.size = s1.size + s2.size;
	str.str  = alloc_array_no_zero(ainfo, U8, str.size + 1);
	memory_copy(str.str,           s1.str, s1.size);
	memory_copy(str.str + s1.size, s2.str, s2.size);
	str.str[str.size] = 0;
	return str;
}

String8
str8_copy(AllocatorInfo ainfo, String8 s)
{
	String8 str;
	str.size = s.size;
	str.str  = alloc_array_no_zero(ainfo, U8, str.size + 1);
	memory_copy(str.str, s.str, s.size);
	str.str[str.size] = 0;
	return(str);
}

String8
str8fv(AllocatorInfo ainfo, char *fmt, va_list args){
	va_list args2;
	va_copy(args2, args);
	U32     needed_bytes = raddbg_vsnprintf(0, 0, fmt, args) + 1;
	String8 result       = {0};
	result.str  = alloc_array_no_zero(ainfo, U8, needed_bytes);
	result.size = raddbg_vsnprintf((char*)result.str, needed_bytes, fmt, args2);
	result.str[result.size] = 0;
	va_end(args2);
	return(result);
}

////////////////////////////////
//~ rjf: String <=> Integer Conversions

//- rjf: string -> integer

S64
sign_from_str8(String8 string, String8* string_tail)
{
	// count negative signs
	U64 neg_count = 0;
	U64 i         = 0;
	for (; i < string.size; i += 1){
		if (string.str[i] == '-') {
			neg_count += 1;
		}
		else if (string.str[i] != '+') {
			break;
		}
	}
	// output part of string after signs
	*string_tail = str8_skip(string, i);
	// output integer sign
	S64 sign = (neg_count & 1)?-1:+1;
	return(sign);
}

B32
str8_is_integer(String8 string, U32 radix){
	B32 result = 0;
	if (string.size > 0 && (1 < radix && radix <= 16)) {
		result = 1;
		for (U64 i = 0; i < string.size; i += 1) {
			U8 c = string.str[i];
			if ( ! (c < 0x80) || integer_symbol_reverse[c] >= radix){
				result = 0;
				break;
			}
		}
	}
	return(result);
}

B32
try_u64_from_str8_c_rules(String8 string, U64 *x) 
{
	B32 is_integer = 0;
	if (str8_is_integer(string, 10)) {
		is_integer = 1;
		*x = u64_from_str8(string, 10);
	}
	else
	{
		String8 hex_string = str8_skip(string, 2);
		if (str8_match(str8_prefix(string, 2), str8_lit("0x"), 0) &&
			str8_is_integer(hex_string, 0x10))
		{
			is_integer = 1;
			*x = u64_from_str8(hex_string, 0x10);
		}
		else if (str8_match(str8_prefix(string, 2), str8_lit("0b"), 0) &&
				 str8_is_integer(hex_string, 2))
		{
			is_integer = 1;
			*x = u64_from_str8(hex_string, 2);
		}
		else
		{
			String8 oct_string = str8_skip(string, 1);
			if (str8_match(str8_prefix(string, 1), str8_lit("0"), 0) &&
				str8_is_integer(hex_string, 010))
			{
				is_integer = 1;
				*x = u64_from_str8(oct_string, 010);
			}
		}
	}
	return(is_integer);
}

//- rjf: integer -> string

String8
str8_from_memory_size(Arena* arena, U64 z) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8 result = {0};
	if (z < KB(1)) {
		result = push_str8f(arena, "%llu  b", z);
	}
	else if (z < MB(1)) {
		result = push_str8f(arena, "%llu.%02llu Kb", z/KB(1), ((100*z)/KB(1))%100);
	}
	else if (z < GB(1)) {
		result = push_str8f(arena, "%llu.%02llu Mb", z/MB(1), ((100*z)/MB(1))%100);
	}
	else{
		result = push_str8f(arena, "%llu.%02llu Gb", z/GB(1), ((100*z)/GB(1))%100);
	}
	return(result);
#else
	return str8_from_allocator_size(arena_allocator(arena), z);
#endif
}

String8
str8_from_allocator_size(AllocatorInfo ainfo, U64 z) {
	String8 result = {0};
	if (z < KB(1)) {
		result = str8f(ainfo, "%llu  b", z);
	}
	else if (z < MB(1)) {
		result = str8f(ainfo, "%llu.%02llu Kb", z/KB(1), ((100*z)/KB(1))%100);
	}
	else if (z < GB(1)) {
		result = str8f(ainfo, "%llu.%02llu Mb", z/MB(1), ((100*z)/MB(1))%100);
	}
	else{
		result = str8f(ainfo, "%llu.%02llu Gb", z/GB(1), ((100*z)/GB(1))%100);
	}
	return(result);
}

String8
str8_from_u64(Arena* arena, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator)
{
	#if 0
	String8 result = {0};
	{
		// rjf: prefix
		String8 prefix = {0};
		switch(radix)
		{
			case 16:{prefix = str8_lit("0x");}break;
			case 8: {prefix = str8_lit("0o");}break;
			case 2: {prefix = str8_lit("0b");}break;
		}
		
		// rjf: determine # of chars between separators
		U8 digit_group_size = 3;
		switch(radix)
		{
			default:break;
			case 2:
			case 8:
			case 16:
				{digit_group_size = 4;} break;
		}
		
		// rjf: prep
		U64 needed_leading_0s = 0;
		{
			U64 needed_digits = 1;
			{
				U64 u64_reduce = u64;
				for(;;) 
				{
					u64_reduce /= radix;
					if(u64_reduce == 0) {
						break;
					}
					needed_digits += 1;
				}
			}
			    needed_leading_0s = (min_digits > needed_digits) ? min_digits - needed_digits : 0;
			U64 needed_separators = 0;
			if (digit_group_separator != 0)
			{
				needed_separators = (needed_digits+needed_leading_0s)/digit_group_size;
				if(needed_separators > 0 && (needed_digits+needed_leading_0s)%digit_group_size == 0)
				{
					needed_separators -= 1;
				}
			}
			result.size = prefix.size + needed_leading_0s + needed_separators + needed_digits;
			result.str  = push_array_no_zero(arena, U8, result.size + 1);
			result.str[result.size] = 0;
		}
		
		// rjf: fill contents
		{
			U64 u64_reduce             = u64;
			U64 digits_until_separator = digit_group_size;
			for (U64 idx = 0; idx < result.size; idx += 1)
			{
				if(digits_until_separator == 0 && digit_group_separator != 0) {
					result.str[result.size - idx - 1] = digit_group_separator;
					digits_until_separator = digit_group_size+1;
				}
				else {
					result.str[result.size - idx - 1] = char_to_lower(integer_symbols[u64_reduce%radix]);
					u64_reduce /= radix;
				}
				digits_until_separator -= 1;
				if(u64_reduce == 0) {
					break;
				}
			}
			for(U64 leading_0_idx = 0; leading_0_idx < needed_leading_0s; leading_0_idx += 1)
			{
				result.str[prefix.size + leading_0_idx] = '0';
			}
		}
		
		// rjf: fill prefix
		if(prefix.size != 0)
		{
			memory_copy(result.str, prefix.str, prefix.size);
		}
	}
	return result;
	#endif
	return str8_from_allocator_u64(arena_allocator(arena), u64, radix, min_digits, digit_group_separator);
}

String8
str8_from_allocator_u64(AllocatorInfo ainfo, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator)
{
	String8 result = {0};
	{
		// rjf: prefix
		String8 prefix = {0};
		switch(radix)
		{
			case 16:{prefix = str8_lit("0x");}break;
			case 8: {prefix = str8_lit("0o");}break;
			case 2: {prefix = str8_lit("0b");}break;
		}
		
		// rjf: determine # of chars between separators
		U8 digit_group_size = 3;
		switch(radix)
		{
			default:break;
			case 2:
			case 8:
			case 16:
				{digit_group_size = 4;} break;
		}
		
		// rjf: prep
		U64 needed_leading_0s = 0;
		{
			U64 needed_digits = 1;
			{
				U64 u64_reduce = u64;
				for(;;) 
				{
					u64_reduce /= radix;
					if(u64_reduce == 0) {
						break;
					}
					needed_digits += 1;
				}
			}
			    needed_leading_0s = (min_digits > needed_digits) ? min_digits - needed_digits : 0;
			U64 needed_separators = 0;
			if (digit_group_separator != 0)
			{
				needed_separators = (needed_digits+needed_leading_0s)/digit_group_size;
				if(needed_separators > 0 && (needed_digits+needed_leading_0s)%digit_group_size == 0)
				{
					needed_separators -= 1;
				}
			}
			result.size = prefix.size + needed_leading_0s + needed_separators + needed_digits;
			result.str  = alloc_array_no_zero(ainfo, U8, result.size + 1);
			result.str[result.size] = 0;
		}
		
		// rjf: fill contents
		{
			U64 u64_reduce             = u64;
			U64 digits_until_separator = digit_group_size;
			for (U64 idx = 0; idx < result.size; idx += 1)
			{
				if(digits_until_separator == 0 && digit_group_separator != 0) {
					result.str[result.size - idx - 1] = digit_group_separator;
					digits_until_separator = digit_group_size+1;
				}
				else {
					result.str[result.size - idx - 1] = char_to_lower(integer_symbols[u64_reduce%radix]);
					u64_reduce /= radix;
				}
				digits_until_separator -= 1;
				if(u64_reduce == 0) {
					break;
				}
			}
			for(U64 leading_0_idx = 0; leading_0_idx < needed_leading_0s; leading_0_idx += 1)
			{
				result.str[prefix.size + leading_0_idx] = '0';
			}
		}
		
		// rjf: fill prefix
		if(prefix.size != 0)
		{
			memory_copy(result.str, prefix.str, prefix.size);
		}
	}
	return result;
}

String8
str8_from_s64(Arena *arena, S64 s64, U32 radix, U8 min_digits, U8 digit_group_separator)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8 result = {0};
	// TODO(rjf): preeeeetty sloppy...
	if(s64 < 0) {
		TempArena scratch      = scratch_begin( & arena, 1);
		String8   numeric_part = str8_from_u64(scratch.arena, (U64)(-s64), radix, min_digits, digit_group_separator);
		result = push_str8f(arena, "-%S", numeric_part);
		scratch_end(scratch);
	}
	else {
		result = str8_from_u64(arena, (U64)s64, radix, min_digits, digit_group_separator);
	}
	return result;
#else
	return str8_from_alloctor_s64(arena_allocator(arena), s64, radix, min_digits, digit_group_separator);
#endif
}

String8
str8_from_alloctor_s64(AllocatorInfo ainfo, S64 s64, U32 radix, U8 min_digits, U8 digit_group_separator)
{
	String8 result = {0};
	if(s64 < 0) {
		U8      bytes[KB(8)];
		FArena  scratch = farena_from_memory(bytes, size_of(bytes));
		String8 numeric_part = str8__from_allocator_u64(farena_allocator(scratch), (U64)(-s64), radix, min_digits, digit_group_separator);
		result = str8f(ainfo, "-%S", numeric_part);
	}
	else {
		result = str8__from_allocator_u64(ainfo, (U64)s64, radix, min_digits, digit_group_separator);
	}
	return result;
}

////////////////////////////////
//~ rjf: String <=> Float Conversions

F64
f64_from_str8(String8 string)
{
	// TODO(rjf): crappy implementation for now that just uses atof.
	F64 result = 0;
	if (string.size > 0)
	{
		// rjf: find starting pos of numeric string, as well as sign
		F64 sign = +1.0;
		//U64 first_numeric = 0;
		if(string.str[0] == '-')
		{
			//first_numeric = 1;
			sign = -1.0;
		}
		else if(string.str[0] == '+')
		{
			//first_numeric = 1;
			sign = 1.0;
		}
		
		// rjf: gather numerics
		U64  num_valid_chars = 0;
		char buffer[64];
		for(U64 idx = 0; idx < string.size && num_valid_chars < sizeof(buffer)-1; idx += 1)
		{
			if(char_is_digit(string.str[idx], 10) || string.str[idx] == '.')
			{
				buffer[num_valid_chars] = string.str[idx];
				num_valid_chars += 1;
			}
		}
		
		// rjf: null-terminate (the reason for all of this!!!!!!)
		buffer[num_valid_chars] = 0;
		
		// rjf: do final conversion
		result = sign * atof(buffer);
	}
	return result;
}

////////////////////////////////
//~ rjf: String List Construction Functions

void
str8_list_concat_in_place(String8List* list, String8List* to_push) {
	if(to_push->node_count != 0) 
	{
		if (list->last) {
			list->node_count += to_push->node_count;
			list->total_size += to_push->total_size;
			list->last->next  = to_push->first;
			list->last        = to_push->last;
		}
		else {
			*list = *to_push;
		}
		memory_zero_struct(to_push);
	}
}

String8Node*
str8_list_push_aligner(Arena* arena, String8List* list, U64 min, U64 align)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8Node* node = push_array_no_zero(arena, String8Node, 1);
	U64 new_size = list->total_size + min;
	U64 increase_size = 0;

	if (align > 1) {
		// NOTE(allen): assert is power of 2
		assert(((align - 1) & align) == 0);
		U64 mask = align - 1;
		new_size += mask;
		new_size &= (~mask);
		increase_size = new_size - list->total_size;
	}

	local_persist const U8 zeroes_buffer[64] = {0};
	assert(increase_size <= array_count(zeroes_buffer));

	sll_queue_push(list->first, list->last, node);
	list->node_count += 1;
	list->total_size  = new_size;
	node->string.str  = (U8*)zeroes_buffer;
	node->string.size = increase_size;
	return(node);
#else
	return str8_list_aligner(arena_allocator(arena), list, min, align);
#endif
}

String8Node*
str8_list_aligner(AllocatorInfo ainfo, String8List* list, U64 min, U64 align) {
	String8Node* node = alloc_array_no_zero(ainfo, String8Node, 1);
	U64 new_size = list->total_size + min;
	U64 increase_size = 0;

	if (align > 1) {
		// NOTE(allen): assert is power of 2
		assert(((align - 1) & align) == 0);
		U64 mask = align - 1;
		new_size += mask;
		new_size &= (~mask);
		increase_size = new_size - list->total_size;
	}

	local_persist const U8 zeroes_buffer[64] = {0};
	assert(increase_size <= array_count(zeroes_buffer));

	sll_queue_push(list->first, list->last, node);
	list->node_count += 1;
	list->total_size  = new_size;
	node->string.str  = (U8*)zeroes_buffer;
	node->string.size = increase_size;
	return(node);
}

String8List
str8_list_copy(Arena *arena, String8List *list) {
#if MD_DONT_MAP_ANREA_TO_ALLOCATOR_IMPL
	String8List result = {0};
	for (String8Node* node = list->first;
		node != 0;
		node = node->next)
	{
		String8Node* new_node   = push_array_no_zero(arena, String8Node, 1);
		String8      new_string = push_str8_copy(arena, node->string);
		str8_list_push_node_set_string(&result, new_node, new_string);
	}
	return(result);
#else
	return str8_list_alloc_copy(arena_allocator(arena), list);
#endif
}

String8List
str8_list_alloc_copy(AllocatorInfo ainfo, String8List* list)
{
	String8List result = {0};
	for (String8Node* node = list->first;
		node != 0;
		node = node->next)
	{
		String8Node* new_node   = alloc_array_no_zero(ainfo, String8Node, 1);
		String8      new_string = str8_copy(ainfo, node->string);
		str8_list_push_node_set_string(&result, new_node, new_string);
	}
	return(result);
}

////////////////////////////////
//~ rjf: String Splitting & Joining

String8List
str8_split(Arena* arena, String8 string, U8* split_chars, U64 split_char_count, StringSplitFlags flags) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	String8List list = {0};
  
	B32 keep_empties = (flags & StringSplitFlag_KeepEmpties);
  
	U8* ptr = string.str;
	U8* opl = string.str + string.size;
	for (;ptr < opl;)
	{
		U8* first = ptr;
		for (;ptr < opl; ptr += 1)
		{
			U8  c        = *ptr;
			B32 is_split = 0;
			for (U64 i = 0; i < split_char_count; i += 1) {
				if (split_chars[i] == c) {
					is_split = 1;
					break;
				}
			}
			if (is_split){
				break;
			}
		}
    
		String8 string = str8_range(first, ptr);
		if (keep_empties || string.size > 0){
			str8_list_push(arena, &list, string);
		}
		ptr += 1;
	}
	return(list);
#else
	return str8_split_alloc(arena_allocator(arena), string, split_chars, split_char_count, flags);
#endif
}

String8List
str8_split_alloc(AllocatorInfo ainfo, String8 string, U8* split_chars, U64 split_char_count, StringSplitFlags flags) {
	String8List list = {0};
  
	B32 keep_empties = (flags & StringSplitFlag_KeepEmpties);
  
	U8* ptr = string.str;
	U8* opl = string.str + string.size;
	for (;ptr < opl;)
	{
		U8* first = ptr;
		for (;ptr < opl; ptr += 1)
		{
			U8  c        = *ptr;
			B32 is_split = 0;
			for (U64 i = 0; i < split_char_count; i += 1) {
				if (split_chars[i] == c) {
					is_split = 1;
					break;
				}
			}
			if (is_split){
				break;
			}
		}
    
		String8 string = str8_range(first, ptr);
		if (keep_empties || string.size > 0){
			str8_list_alloc(ainfo, &list, string);
		}
		ptr += 1;
	}
	return(list);
}

String8
str8_list_join(Arena* arena, String8List* list, StringJoin* optional_params) 
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	StringJoin join = {0};
	if (optional_params != 0){
		memory_copy_struct(&join, optional_params);
	}
	
	U64 sep_count = 0;
	if (list->node_count > 0){
		sep_count = list->node_count - 1;
	}
	
	String8 result;
	result.size = join.pre.size + join.post.size + sep_count*join.sep.size + list->total_size;
	U8 *ptr = result.str = push_array_no_zero(arena, U8, result.size + 1);
	
	MemoryCopy(ptr, join.pre.str, join.pre.size);
	ptr += join.pre.size;
	for (String8Node *node = list->first; node != 0; node = node->next)
	{
		memory_copy(ptr, node->string.str, node->string.size);
		ptr += node->string.size;
		if (node->next != 0) {
			memory_copy(ptr, join.sep.str, join.sep.size);
			ptr += join.sep.size;
		}
	}
	memory_copy(ptr, join.post.str, join.post.size);
	ptr += join.post.size;
	*ptr = 0;
	return(result);
#else
	return str8_list_join_alloc(arena_allocator(arena), list, optional_params);
#endif
}

String8
str8_list_join_alloc(AllocatorInfo ainfo, String8List* list, StringJoin* optional_params) 
{
	StringJoin join = {0};
	if (optional_params != 0){
		MemoryCopyStruct(&join, optional_params);
	}
	
	U64 sep_count = 0;
	if (list->node_count > 0){
		sep_count = list->node_count - 1;
	}
	
	String8 result;
	result.size = join.pre.size + join.post.size + sep_count*join.sep.size + list->total_size;
	U8* ptr = result.str = alloc_array_no_zero(ainfo, U8, result.size + 1);
	
	memory_copy(ptr, join.pre.str, join.pre.size);
	ptr += join.pre.size;
	for (String8Node *node = list->first; node != 0; node = node->next)
	{
		memory_copy(ptr, node->string.str, node->string.size);
		ptr += node->string.size;
		if (node->next != 0) {
			memory_copy(ptr, join.sep.str, join.sep.size);
			ptr += join.sep.size;
		}
	}
	memory_copy(ptr, join.post.str, join.post.size);
	ptr += join.post.size;
	*ptr = 0;
	return(result);
}

////////////////////////////////
//~ rjf: String Path Helpers

String8
str8_chop_last_slash(String8 string) {
	if (string.size > 0)
	{
		U8* ptr = string.str + string.size - 1;
		for (;ptr >= string.str; ptr -= 1) {
			if (*ptr == '/' || *ptr == '\\') break;
		}
		if (ptr >= string.str) {
			string.size = (U64)(ptr - string.str);
		}
		else {
			string.size = 0;
		}
	}
	return(string);
}

String8
str8_skip_last_slash(String8 string) {
	if (string.size > 0)
	{
		U8* ptr = string.str + string.size - 1;
		for (;ptr >= string.str; ptr -= 1) {
			if (*ptr == '/' || *ptr == '\\') break;
		}
		if (ptr >= string.str) {
			ptr += 1;
			string.size = (U64)(string.str + string.size - ptr);
			string.str = ptr;
		}
	}
	return(string);
}

String8
str8_chop_last_dot(String8 string) {
	String8 result = string;
	U64 p = string.size;
	for (;p > 0;) {
		p -= 1;
		if (string.str[p] == '.') {
			result = str8_prefix(string, p);
			break;
		}
	}
	return(result);
}

String8
str8_skip_last_dot(String8 string) {
	String8 result = string;
	U64 p = string.size;
	for (;p > 0;) {
		p -= 1;
		if (string.str[p] == '.') {
			result = str8_skip(string, p + 1);
			break;
		}
	}
	return(result);
}

PathStyle
path_style_from_str8(String8 string) {
	PathStyle result = PathStyle_Relative;
	if (string.size >= 1 && string.str[0] == '/') {
		result = PathStyle_UnixAbsolute;
	}
	else if (string.size >= 2 && char_is_alpha(string.str[0]) && string.str[1] == ':')
	{
		if (string.size == 2 || char_is_slash(string.str[2])) {
			result = PathStyle_WindowsAbsolute;
		}
	}
	return(result);
}

void
str8_path_list_resolve_dots_in_place(String8List* path, PathStyle style)
{
	TempArena scratch = scratch_begin(0, 0);
	String8MetaNode* stack          = 0;
	String8MetaNode* free_meta_node = 0;
	String8Node*     first          = path->first;
  
	memory_zero_struct(path);
	for (String8Node* node = first, *next = 0; node != 0; node = next)
	{
		// save next now
		next = node->next;
		
		// cases:
		if (node == first && style == PathStyle_WindowsAbsolute){
			goto save_without_stack;
		}

		if (node->string.size == 1 && node->string.str[0] == '.') {
			goto do_nothing;
		}
		if (node->string.size == 2 && node->string.str[0] == '.' && node->string.str[1] == '.') {
			if (stack != 0) {
				goto eliminate_stack_top;
			}
			else{
				goto save_without_stack;
			}
		}
		goto save_with_stack;
		
		
		// handlers:
		save_with_stack:
		{
			str8_list_push_node(path, node);
				
			String8MetaNode *stack_node = free_meta_node;
			if (stack_node != 0){
				sll_stack_pop(free_meta_node);
			}
			else{
				stack_node = push_array_no_zero(scratch.arena, String8MetaNode, 1);
			}
			sll_stack_push(stack, stack_node);
			stack_node->node = node;
			continue;
		}
			
		save_without_stack:
		{
			str8_list_push_node(path, node);
			continue;
		}
		
		eliminate_stack_top:
		{
			path->node_count -= 1;
			path->total_size -= stack->node->string.size;

			sll_stack_pop(stack);
			if (stack == 0) {
				path->last = path->first;
			}
			else {
				path->last = stack->node;
			}
			continue;
		}
		do_nothing: continue;
	}
	scratch_end(scratch);
}

String8
str8_path_list_join_by_style(Arena* arena, String8List* path, PathStyle style) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	StringJoin params = {0};
	switch (style)
	{
		case PathStyle_Relative:
		case PathStyle_WindowsAbsolute:
		{
			params.sep = str8_lit("/");
		}
		break;
		
		case PathStyle_UnixAbsolute:
		{
			params.pre = str8_lit("/");
			params.sep = str8_lit("/");
		}
		break;
	}
	String8 result = str8_list_join(arena, path, &params);
	return(result);
#else
	return str8_path_list_join_by_style_alloc(arena_allocator(arena), path, style);
#endif
}

String8
str8_path_list_join_by_style_alloc(AllocatorInfo ainfo, String8List* path, PathStyle style) {
	StringJoin params = {0};
	switch (style)
	{
		case PathStyle_Relative:
		case PathStyle_WindowsAbsolute:
		{
			params.sep = str8_lit("/");
		}
		break;
		
		case PathStyle_UnixAbsolute:
		{
			params.pre = str8_lit("/");
			params.sep = str8_lit("/");
		}
		break;
	}
	String8 result = str8_list_join_alloc(ainfo, path, &params);
	return(result);
}

////////////////////////////////
//~ rjf: UTF-8 & UTF-16 Decoding/Encoding

read_only global U8 utf8_class[32] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,2,2,2,2,3,3,4,5,
};

UnicodeDecode
utf8_decode(U8* str, U64 max)
{
	UnicodeDecode result = {1, MAX_U32};
	U8 byte       = str[0];
	U8 byte_class = utf8_class[byte >> 3];
	switch (byte_class)
	{
		case 1:
		{
			result.codepoint = byte;
		}
		break;

		case 2:
		{
			if (2 < max)
			{
				U8 cont_byte = str[1];
				if (utf8_class[cont_byte >> 3] == 0)
				{
					result.codepoint =   (byte      & BITMASK5) << 6;
					result.codepoint |=  (cont_byte & BITMASK6);
					result.inc = 2;
				}
			}
		}
		break;

		case 3:
		{
			if (2 < max)
			{
				U8 cont_byte[2] = {str[1], str[2]};
				if (utf8_class[cont_byte[0] >> 3] == 0 &&
					utf8_class[cont_byte[1] >> 3] == 0)
				{
					result.codepoint =   (byte         & BITMASK4) << 12;
					result.codepoint |= ((cont_byte[0] & BITMASK6) << 6);
					result.codepoint |=  (cont_byte[1] & BITMASK6);
					result.inc = 3;
				}
			}
		}
		break;

		case 4:
		{
			if (3 < max)
			{
				U8 cont_byte[3] = {str[1], str[2], str[3]};
				if (utf8_class[cont_byte[0] >> 3] == 0 &&
					utf8_class[cont_byte[1] >> 3] == 0 &&
					utf8_class[cont_byte[2] >> 3] == 0)
				{
					result.codepoint =   (byte         & BITMASK3) << 18;
					result.codepoint |= ((cont_byte[0] & BITMASK6) << 12);
					result.codepoint |= ((cont_byte[1] & BITMASK6) <<  6);
					result.codepoint |=  (cont_byte[2] & BITMASK6);
					result.inc = 4;
				}
			}
		}
	}
	return(result);
}

U32
utf8_encode(U8* str, U32 codepoint)
{
	U32 inc = 0;
	if (codepoint <= 0x7F){
		str[0] = (U8)codepoint;
		inc    = 1;
	}
	else if (codepoint <= 0x7FF){
		str[0] = (BITMASK2 << 6) | ((codepoint >> 6) & BITMASK5);
		str[1] = BIT8 | (codepoint & BITMASK6);
		inc    = 2;
	}
	else if (codepoint <= 0xFFFF){
		str[0] = (BITMASK3 << 5) | ((codepoint >> 12) & BITMASK4);
		str[1] = BIT8 | ((codepoint >> 6) & BITMASK6);
		str[2] = BIT8 | ( codepoint       & BITMASK6);
		inc    = 3;
	}
	else if (codepoint <= 0x10FFFF){
		str[0] = (BITMASK4 << 4) | ((codepoint >> 18) & BITMASK3);
		str[1] = BIT8 | ((codepoint >> 12) & BITMASK6);
		str[2] = BIT8 | ((codepoint >>  6) & BITMASK6);
		str[3] = BIT8 | ( codepoint        & BITMASK6);
		inc    = 4;
	}
	else{
		str[0] = '?';
		inc    = 1;
	}
	return(inc);
}

U32
utf16_encode(U16 *str, U32 codepoint) {
	U32 inc = 1;
	if (codepoint == MAX_U32) {
		str[0] = (U16)'?';
	}
	else if (codepoint < 0x10000) {
		str[0] = (U16)codepoint;
	}
	else {
		U32 v = codepoint - 0x10000;
		str[0] = safe_cast_u16(0xD800 + (v >> 10));
		str[1] = safe_cast_u16(0xDC00 + (v & BITMASK10));
		inc    = 2;
	}
	return(inc);
}

////////////////////////////////
//~ rjf: Unicode String Conversions

internal String8
str8_from_16(Arena *arena, String16 in){
  U64 cap = in.size*3;
  U8 *str = push_array_no_zero(arena, U8, cap + 1);
  U16 *ptr = in.str;
  U16 *opl = ptr + in.size;
  U64 size = 0;
  UnicodeDecode consume;
  for (;ptr < opl; ptr += consume.inc){
    consume = utf16_decode(ptr, opl - ptr);
    size += utf8_encode(str + size, consume.codepoint);
  }
  str[size] = 0;
  arena_pop(arena, (cap - size));
  return(str8(str, size));
}

internal String16
str16_from_8(Arena *arena, String8 in){
  U64 cap = in.size*2;
  U16 *str = push_array_no_zero(arena, U16, cap + 1);
  U8 *ptr = in.str;
  U8 *opl = ptr + in.size;
  U64 size = 0;
  UnicodeDecode consume;
  for (;ptr < opl; ptr += consume.inc){
    consume = utf8_decode(ptr, opl - ptr);
    size += utf16_encode(str + size, consume.codepoint);
  }
  str[size] = 0;
  arena_pop(arena, (cap - size)*2);
  return(str16(str, size));
}

internal String8
str8_from_32(Arena *arena, String32 in){
  U64 cap = in.size*4;
  U8 *str = push_array_no_zero(arena, U8, cap + 1);
  U32 *ptr = in.str;
  U32 *opl = ptr + in.size;
  U64 size = 0;
  for (;ptr < opl; ptr += 1){
    size += utf8_encode(str + size, *ptr);
  }
  str[size] = 0;
  arena_pop(arena, (cap - size));
  return(str8(str, size));
}

internal String32
str32_from_8(Arena *arena, String8 in){
  U64 cap = in.size;
  U32 *str = push_array_no_zero(arena, U32, cap + 1);
  U8 *ptr = in.str;
  U8 *opl = ptr + in.size;
  U64 size = 0;
  UnicodeDecode consume;
  for (;ptr < opl; ptr += consume.inc){
    consume = utf8_decode(ptr, opl - ptr);
    str[size] = consume.codepoint;
    size += 1;
  }
  str[size] = 0;
  arena_pop(arena, (cap - size)*4);
  return(str32(str, size));
}

////////////////////////////////
//~ rjf: Basic Types & Space Enum -> String Conversions

internal String8
string_from_dimension(Dimension dimension){
  local_persist String8 strings[] = {
    str8_lit_comp("X"),
    str8_lit_comp("Y"),
    str8_lit_comp("Z"),
    str8_lit_comp("W"),
  };
  String8 result = str8_lit("error");
  if ((U32)dimension < 4){
    result = strings[dimension];
  }
  return(result);
}

internal String8
string_from_side(Side side){
  local_persist String8 strings[] = {
    str8_lit_comp("Min"),
    str8_lit_comp("Max"),
  };
  String8 result = str8_lit("error");
  if ((U32)side < 2){
    result = strings[side];
  }
  return(result);
}

internal String8
string_from_operating_system(OperatingSystem os){
  local_persist String8 strings[] = {
    str8_lit_comp("Null"),
    str8_lit_comp("Windows"),
    str8_lit_comp("Linux"),
    str8_lit_comp("Mac"),
  };
  String8 result = str8_lit("error");
  if (os < OperatingSystem_COUNT){
    result = strings[os];
  }
  return(result);
}

internal String8
string_from_architecture(Architecture arch){
  local_persist String8 strings[] = {
    str8_lit_comp("Null"),
    str8_lit_comp("x64"),
    str8_lit_comp("x86"),
    str8_lit_comp("arm64"),
    str8_lit_comp("arm32"),
  };
  String8 result = str8_lit("error");
  if (arch < Architecture_COUNT){
    result = strings[arch];
  }
  return(result);
}

////////////////////////////////
//~ rjf: Time Types -> String

internal String8
string_from_week_day(WeekDay week_day){
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
  if ((U32)week_day < WeekDay_COUNT){
    result = strings[week_day];
  }
  return(result);
}

internal String8
string_from_month(Month month){
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
  if ((U32)month < Month_COUNT){
    result = strings[month];
  }
  return(result);
}

internal String8
push_date_time_string(Arena *arena, DateTime *date_time){
  char *mon_str = (char*)string_from_month(date_time->month).str;
  U32 adjusted_hour = date_time->hour%12;
  if (adjusted_hour == 0){
    adjusted_hour = 12;
  }
  char *ampm = "am";
  if (date_time->hour >= 12){
    ampm = "pm";
  }
  String8 result = push_str8f(arena, "%d %s %d, %02d:%02d:%02d %s",
                              date_time->day, mon_str, date_time->year,
                              adjusted_hour, date_time->min, date_time->sec, ampm);
  return(result);
}

internal String8
push_file_name_date_time_string(Arena *arena, DateTime *date_time){
  char *mon_str = (char*)string_from_month(date_time->month).str;
  String8 result = push_str8f(arena, "%d-%s-%0d--%02d-%02d-%02d",
                              date_time->year, mon_str, date_time->day,
                              date_time->hour, date_time->min, date_time->sec);
  return(result);
}

////////////////////////////////
//~ rjf: Text Escaping

internal String8
escaped_from_raw_str8(Arena *arena, String8 string)
{
  TempArena scratch = scratch_begin(&arena, 1);
  String8List parts = {0};
  U64 start_split_idx = 0;
  for(U64 idx = 0; idx <= string.size; idx += 1)
  {
    U8 byte = (idx < string.size) ? string.str[idx] : 0;
    B32 split = 1;
    String8 separator_replace = {0};
    switch(byte)
    {
      default:{split = 0;}break;
      case 0:    {}break;
      case '\a': {separator_replace = str8_lit("\\a");}break;
      case '\b': {separator_replace = str8_lit("\\b");}break;
      case '\f': {separator_replace = str8_lit("\\f");}break;
      case '\n': {separator_replace = str8_lit("\\n");}break;
      case '\r': {separator_replace = str8_lit("\\r");}break;
      case '\t': {separator_replace = str8_lit("\\t");}break;
      case '\v': {separator_replace = str8_lit("\\v");}break;
      case '\\': {separator_replace = str8_lit("\\\\");}break;
      case '"':  {separator_replace = str8_lit("\\\"");}break;
      case '?':  {separator_replace = str8_lit("\\?");}break;
    }
    if(split)
    {
      String8 substr = str8_substr(string, r1u64(start_split_idx, idx));
      start_split_idx = idx+1;
      str8_list_push(scratch.arena, &parts, substr);
      if(separator_replace.size != 0)
      {
        str8_list_push(scratch.arena, &parts, separator_replace);
      }
    }
  }
  StringJoin join = {0};
  String8 result = str8_list_join(arena, &parts, &join);
  scratch_end(scratch);
  return result;
}

internal String8
raw_from_escaped_str8(Arena *arena, String8 string)
{
  TempArena scratch = scratch_begin(&arena, 1);
  String8List strs = {0};
  U64 start = 0;
  for(U64 idx = 0; idx <= string.size; idx += 1)
  {
    if(idx == string.size || string.str[idx] == '\\' || string.str[idx] == '\r')
    {
      String8 str = str8_substr(string, r1u64(start, idx));
      if(str.size != 0)
      {
        str8_list_push(scratch.arena, &strs, str);
      }
      start = idx+1;
    }
    if(idx < string.size && string.str[idx] == '\\')
    {
      U8 next_char = string.str[idx+1];
      U8 replace_byte = 0;
      switch(next_char)
      {
        default:{}break;
        case 'a': replace_byte = 0x07; break;
        case 'b': replace_byte = 0x08; break;
        case 'e': replace_byte = 0x1b; break;
        case 'f': replace_byte = 0x0c; break;
        case 'n': replace_byte = 0x0a; break;
        case 'r': replace_byte = 0x0d; break;
        case 't': replace_byte = 0x09; break;
        case 'v': replace_byte = 0x0b; break;
        case '\\':replace_byte = '\\'; break;
        case '\'':replace_byte = '\''; break;
        case '"': replace_byte = '"';  break;
        case '?': replace_byte = '?';  break;
      }
      String8 replace_string = push_str8_copy(scratch.arena, str8(&replace_byte, 1));
      str8_list_push(scratch.arena, &strs, replace_string);
      idx += 1;
      start += 1;
    }
  }
  String8 result = str8_list_join(arena, &strs, 0);
  scratch_end(scratch);
  return result;
}

////////////////////////////////
//~ rjf: Basic Text Indentation

internal String8
indented_from_string(Arena *arena, String8 string)
{
  TempArena scratch = scratch_begin(&arena, 1);
  read_only local_persist U8 indentation_bytes[] = "                                                                                                                                ";
  String8List indented_strings = {0};
  S64 depth = 0;
  S64 next_depth = 0;
  U64 line_begin_off = 0;
  for(U64 off = 0; off <= string.size; off += 1)
  {
    U8 byte = off<string.size ? string.str[off] : 0;
    switch(byte)
    {
      default:{}break;
      case '{':case '[':case '(':{next_depth += 1; next_depth = max(0, next_depth);}break;
      case '}':case ']':case ')':{next_depth -= 1; next_depth = max(0, next_depth); depth = next_depth;}break;
      case '\n':
      case 0:
      {
        String8 line = str8_skip_chop_whitespace(str8_substr(string, r1u64(line_begin_off, off)));
        if(line.size != 0)
        {
          str8_list_pushf(scratch.arena, &indented_strings, "%.*s%S\n", (int)depth*2, indentation_bytes, line);
        }
        line_begin_off = off+1;
        depth = next_depth;
      }break;
    }
  }
  String8 result = str8_list_join(arena, &indented_strings, 0);
  scratch_end(scratch);
  return result;
}

////////////////////////////////
//~ rjf: Text Wrapping

internal String8List
wrapped_lines_from_string(Arena* arena, String8 string, U64 first_line_max_width, U64 max_width, U64 wrap_indent)
{
	String8List list       = {0};
	Rng1U64     line_range = r1u64(0, 0);

	U64 wrapped_indent_level = 0;
	static char* spaces = "                                                                ";

	for (U64 idx = 0; idx <= string.size; idx += 1)
	{
		U8 chr = idx < string.size ? string.str[idx] : 0;
		if (chr == '\n')
		{
			Rng1U64
			candidate_line_range     = line_range;
			candidate_line_range.max = idx;
			// NOTE(nick): when wrapping is interrupted with \n we emit a string without including \n
			// because later tool_fprint_list inserts separator after each node
			// except for last node, so don't strip last \n.
			if (idx + 1 == string.size){
				candidate_line_range.max += 1;
			}
			String8 substr = str8_substr(string, candidate_line_range);
			str8_list_push(arena, &list, substr);
			line_range = r1u64(idx + 1,idx + 1);
		}
		else
		if (char_is_space(chr) || chr == 0)
		{
			Rng1U64 
			candidate_line_range     = line_range;
			candidate_line_range.max = idx;

			String8 substr          = str8_substr(string, candidate_line_range);
			U64     width_this_line = max_width-wrapped_indent_level;

			if (list.node_count == 0) {
				width_this_line = first_line_max_width;
			}
			if (substr.size > width_this_line) 
			{
				String8 line = str8_substr(string, line_range);
				if (wrapped_indent_level > 0){
					line = push_str8f(arena, "%.*s%S", wrapped_indent_level, spaces, line);
				}
				str8_list_push(arena, &list, line);
				line_range           = r1u64(line_range.max + 1, candidate_line_range.max);
				wrapped_indent_level = clamp_top(64, wrap_indent);
			}
			else{
				line_range = candidate_line_range;
			}
		}
	}
	if (line_range.min < string.size && line_range.max > line_range.min) {
		String8 line = str8_substr(string, line_range);
		if (wrapped_indent_level > 0) {
			line = push_str8f(arena, "%.*s%S", wrapped_indent_level, spaces, line);
		}
		str8_list_push(arena, &list, line);
	}
	return list;
}

////////////////////////////////
//~ rjf: String <-> Color

internal String8
hex_string_from_rgba_4f32(Arena *arena, Vec4F32 rgba)
{
  String8 hex_string = push_str8f(arena, "%02x%02x%02x%02x", (U8)(rgba.x*255.f), (U8)(rgba.y*255.f), (U8)(rgba.z*255.f), (U8)(rgba.w*255.f));
  return hex_string;
}

internal Vec4F32
rgba_from_hex_string_4f32(String8 hex_string)
{
  U8 byte_text[8] = {0};
  U64 byte_text_idx = 0;
  for(U64 idx = 0; idx < hex_string.size && byte_text_idx < ArrayCount(byte_text); idx += 1)
  {
    if(char_is_digit(hex_string.str[idx], 16))
    {
      byte_text[byte_text_idx] = char_to_lower(hex_string.str[idx]);
      byte_text_idx += 1;
    }
  }
  U8 byte_vals[4] = {0};
  for(U64 idx = 0; idx < 4; idx += 1)
  {
    byte_vals[idx] = (U8)u64_from_str8(str8(&byte_text[idx*2], 2), 16);
  }
  Vec4F32 rgba = v4f32(byte_vals[0]/255.f, byte_vals[1]/255.f, byte_vals[2]/255.f, byte_vals[3]/255.f);
  return rgba;
}

////////////////////////////////
//~ rjf: String Fuzzy Matching

internal FuzzyMatchRangeList
fuzzy_match_find(Arena *arena, String8 needle, String8 haystack)
{
  FuzzyMatchRangeList result = {0};
  TempArena scratch = scratch_begin(&arena, 1);
  String8List needles = str8_split(scratch.arena, needle, (U8*)" ", 1, 0);
  result.needle_part_count = needles.node_count;
  for(String8Node *needle_n = needles.first; needle_n != 0; needle_n = needle_n->next)
  {
    U64 find_pos = 0;
    for(;find_pos < haystack.size;)
    {
      find_pos = str8_find_needle(haystack, find_pos, needle_n->string, StringMatchFlag_CaseInsensitive);
      B32 is_in_gathered_ranges = 0;
      for(FuzzyMatchRangeNode *n = result.first; n != 0; n = n->next)
      {
        if(n->range.min <= find_pos && find_pos < n->range.max)
        {
          is_in_gathered_ranges = 1;
          find_pos = n->range.max;
          break;
        }
      }
      if(!is_in_gathered_ranges)
      {
        break;
      }
    }
    if(find_pos < haystack.size)
    {
      Rng1U64 range = r1u64(find_pos, find_pos+needle_n->string.size);
      FuzzyMatchRangeNode *n = push_array(arena, FuzzyMatchRangeNode, 1);
      n->range = range;
      SLLQueuePush(result.first, result.last, n);
      result.count += 1;
      result.total_dim += dim_1u64(range);
    }
  }
  scratch_end(scratch);
  return result;
}

internal FuzzyMatchRangeList
fuzzy_match_range_list_copy(Arena *arena, FuzzyMatchRangeList *src)
{
  FuzzyMatchRangeList dst = {0};
  for(FuzzyMatchRangeNode *src_n = src->first; src_n != 0; src_n = src_n->next)
  {
    FuzzyMatchRangeNode *dst_n = push_array(arena, FuzzyMatchRangeNode, 1);
    SLLQueuePush(dst.first, dst.last, dst_n);
    dst_n->range = src_n->range;
  }
  dst.count = src->count;
  dst.needle_part_count = src->needle_part_count;
  dst.total_dim = src->total_dim;
  return dst;
}

////////////////////////////////
//~ NOTE(allen): Serialization Helpers

internal void
str8_serial_begin(Arena *arena, String8List *srl){
  String8Node *node = push_array(arena, String8Node, 1);
  node->string.str = push_array_no_zero(arena, U8, 0);
  srl->first = srl->last = node;
  srl->node_count = 1;
  srl->total_size = 0;
}

internal String8
str8_serial_end(Arena *arena, String8List *srl){
  U64 size = srl->total_size;
  U8 *out = push_array_no_zero(arena, U8, size);
  str8_serial_write_to_dst(srl, out);
  String8 result = str8(out, size);
  return result;
}

internal void
str8_serial_write_to_dst(String8List *srl, void *out){
  U8 *ptr = (U8*)out;
  for (String8Node *node = srl->first;
       node != 0;
       node = node->next){
    U64 size = node->string.size;
    MemoryCopy(ptr, node->string.str, size);
    ptr += size;
  }
}

internal U64
str8_serial_push_align(Arena *arena, String8List *srl, U64 align){
  Assert(IsPow2(align));
  
  U64 pos = srl->total_size;
  U64 new_pos = AlignPow2(pos, align);
  U64 size = (new_pos - pos);
  
  if(size != 0)
  {
    U8 *buf = push_array(arena, U8, size);
    
    String8 *str = &srl->last->string;
    if (str->str + str->size == buf){
      srl->last->string.size += size;
      srl->total_size += size;
    }
    else{
      str8_list_push(arena, srl, str8(buf, size));
    }
  }
  return size;
}

internal void *
str8_serial_push_size(Arena *arena, String8List *srl, U64 size)
{
  void *result = 0;
  if(size != 0)
  {
    U8 *buf = push_array_no_zero(arena, U8, size);
    String8 *str = &srl->last->string;
    if (str->str + str->size == buf){
      srl->last->string.size += size;
      srl->total_size += size;
    }
    else{
      str8_list_push(arena, srl, str8(buf, size));
    }
    result = buf;
  }
  return result;
}

internal void *
str8_serial_push_data(Arena *arena, String8List *srl, void *data, U64 size){
  void *result = str8_serial_push_size(arena, srl, size);
  if(result != 0)
  {
    MemoryCopy(result, data, size);
  }
  return result;
}

internal void
str8_serial_push_data_list(Arena *arena, String8List *srl, String8Node *first){
  for (String8Node *node = first;
       node != 0;
       node = node->next){
    str8_serial_push_data(arena, srl, node->string.str, node->string.size);
  }
}

internal void
str8_serial_push_u64(Arena *arena, String8List *srl, U64 x){
  U8 *buf = push_array_no_zero(arena, U8, 8);
  MemoryCopy(buf, &x, 8);
  String8 *str = &srl->last->string;
  if (str->str + str->size == buf){
    srl->last->string.size += 8;
    srl->total_size += 8;
  }
  else{
    str8_list_push(arena, srl, str8(buf, 8));
  }
}

internal void
str8_serial_push_u32(Arena *arena, String8List *srl, U32 x){
  U8 *buf = push_array_no_zero(arena, U8, 4);
  MemoryCopy(buf, &x, 4);
  String8 *str = &srl->last->string;
  if (str->str + str->size == buf){
    srl->last->string.size += 4;
    srl->total_size += 4;
  }
  else{
    str8_list_push(arena, srl, str8(buf, 4));
  }
}

internal void
str8_serial_push_u16(Arena *arena, String8List *srl, U16 x){
  str8_serial_push_data(arena, srl, &x, sizeof(x));
}

internal void
str8_serial_push_u8(Arena *arena, String8List *srl, U8 x){
  str8_serial_push_data(arena, srl, &x, sizeof(x));
}

internal void
str8_serial_push_cstr(Arena *arena, String8List *srl, String8 str){
  str8_serial_push_data(arena, srl, str.str, str.size);
  str8_serial_push_u8(arena, srl, 0);
}

internal void
str8_serial_push_string(Arena *arena, String8List *srl, String8 str){
  str8_serial_push_data(arena, srl, str.str, str.size);
}

////////////////////////////////
//~ rjf: Deserialization Helpers

internal U64
str8_deserial_read(String8 string, U64 off, void *read_dst, U64 read_size, U64 granularity)
{
  U64 bytes_left = string.size-Min(off, string.size);
  U64 actually_readable_size = Min(bytes_left, read_size);
  U64 legally_readable_size = actually_readable_size - actually_readable_size%granularity;
  if(legally_readable_size > 0)
  {
    MemoryCopy(read_dst, string.str+off, legally_readable_size);
  }
  return legally_readable_size;
}

internal U64
str8_deserial_find_first_match(String8 string, U64 off, U16 scan_val)
{
  U64 cursor = off;
  for (;;) {
    U16 val = 0;
    str8_deserial_read_struct(string, cursor, &val);
    if (val == scan_val) {
      break;
    }
    cursor += sizeof(val);
  }
  return cursor;
}

internal void *
str8_deserial_get_raw_ptr(String8 string, U64 off, U64 size)
{
  void *raw_ptr = 0;
  if (off + size <= string.size) {
    raw_ptr = string.str + off;
  }
  return raw_ptr;
}

internal U64
str8_deserial_read_cstr(String8 string, U64 off, String8 *cstr_out)
{
  U64 cstr_size = 0;
  if (off < string.size) {
    U8 *ptr = string.str + off;
    U8 *cap = string.str + string.size;
    *cstr_out = str8_cstring_capped(ptr, cap);
    cstr_size = (cstr_out->size + 1);
  }
  return cstr_size;
}

internal U64
str8_deserial_read_windows_utf16_string16(String8 string, U64 off, String16 *str_out)
{
  U64 null_off = str8_deserial_find_first_match(string, off, 0);
  U64 size = null_off - off;
  U16 *str = (U16 *)str8_deserial_get_raw_ptr(string, off, size);
  U64 count = size / sizeof(*str);
  *str_out = str16(str, count);
  
  U64 read_size_with_null = size + sizeof(*str);
  return read_size_with_null;
}

internal U64
str8_deserial_read_block(String8 string, U64 off, U64 size, String8 *block_out)
{
  Rng1U64 range = rng_1u64(off, off + size);
  *block_out = str8_substr(string, range);
  return block_out->size;
}
