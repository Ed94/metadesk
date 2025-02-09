#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "macros.h"
#	include "debug.h"
#	include "strings.h"
#	include "thread_context.h"
////////////////////////////////
//~ rjf: Third Party Includes
#if !BUILD_SUPPLEMENTARY_UNIT
#	define STB_SPRINTF_IMPLEMENTATION
#	define STB_SPRINTF_STATIC
#	include "third_party/stb/stb_sprintf.h"
// Note(Ed): We should inject when generating the library segmented or singleheader
#endif

#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

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

U64
str8_find_needle_reverse(String8 string, U64 start_pos, String8 needle, StringMatchFlags flags) {
	U64 result = 0;
	for (S64 i = string.size - start_pos - needle.size; i >= 0; --i)
	{
		String8 haystack = str8_substr(string, rng_1u64(i, i + needle.size));
		if (str8_match(haystack, needle, flags)) {
			result = (U64)i + needle.size;
			break;
		}
	}
	return result;
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
	return alloc_str8_cat(arena_allocator(arena), s1, s2);
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
	return alloc_str8_copy(arena_allocator(arena), s);
#endif
}

String8
push_str8fv(Arena* arena, char* fmt, va_list args){
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	va_list args2;
	va_copy(args2, args);
	U32     needed_bytes = md_vsnprintf(0, 0, fmt, args) + 1;
	String8 result       = {0};
	result.str  = push_array_no_zero(arena, U8, needed_bytes);
	result.size = md_vsnprintf((char*)result.str, needed_bytes, fmt, args2);
	result.str[result.size] = 0;
	va_end(args2);
	return(result);
#else
	return alloc_str8fv(arena_allocator(arena), fmt, args);
#endif
}

String8
alloc_str8_cat(AllocatorInfo ainfo, String8 s1, String8 s2)
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
alloc_str8_copy(AllocatorInfo ainfo, String8 s)
{
	String8 str;
	str.size = s.size;
	str.str  = alloc_array_no_zero(ainfo, U8, str.size + 1);
	memory_copy(str.str, s.str, s.size);
	str.str[str.size] = 0;
	return(str);
}

String8
alloc_str8fv(AllocatorInfo ainfo, char *fmt, va_list args){
	va_list args2;
	va_copy(args2, args);
	U32     needed_bytes = md_vsnprintf(0, 0, fmt, args) + 1;
	String8 result       = {0};
	result.str  = alloc_array_no_zero(ainfo, U8, needed_bytes);
	result.size = md_vsnprintf((char*)result.str, needed_bytes, fmt, args2);
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
			if ( ! (c < 0x80) || integer_symbol_reverse(c) >= radix){
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
		result = alloc_str8f(ainfo, "%llu  b", z);
	}
	else if (z < MB(1)) {
		result = alloc_str8f(ainfo, "%llu.%02llu Kb", z/KB(1), ((100*z)/KB(1))%100);
	}
	else if (z < GB(1)) {
		result = alloc_str8f(ainfo, "%llu.%02llu Mb", z/MB(1), ((100*z)/MB(1))%100);
	}
	else{
		result = alloc_str8f(ainfo, "%llu.%02llu Gb", z/GB(1), ((100*z)/GB(1))%100);
	}
	return(result);
}

String8
str8_from_u64(Arena* arena, U64 u64, U32 radix, U8 min_digits, U8 digit_group_separator)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
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
				needed_separators = (needed_digits + needed_leading_0s) / digit_group_size;
				if(needed_separators > 0 && (needed_digits + needed_leading_0s) % digit_group_size == 0)
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
					digits_until_separator = digit_group_size + 1;
				}
				else {
					result.str[result.size - idx - 1] = char_to_lower(integer_symbols(u64_reduce % radix));
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
#else
	return str8_from_allocator_u64(arena_allocator(arena), u64, radix, min_digits, digit_group_separator);
#endif
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
				needed_separators = (needed_digits + needed_leading_0s) / digit_group_size;
				if(needed_separators > 0 && (needed_digits + needed_leading_0s) % digit_group_size == 0)
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
					digits_until_separator = digit_group_size + 1;
				}
				else {
					result.str[result.size - idx - 1] = char_to_lower(integer_symbols(u64_reduce % radix));
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
		// TODO(Ed): Review, we should just keep using thread scratch arenas (and provide them to teh context)
		U8      bytes[KB(8)];
		FArena  scratch      = farena_from_memory(bytes, size_of(bytes));
		String8 numeric_part = str8_from_allocator_u64(farena_allocator(scratch), (U64)(-s64), radix, min_digits, digit_group_separator);
		result = alloc_str8f(ainfo, "-%S", numeric_part);
	}
	else {
		result = str8_from_allocator_u64(ainfo, (U64)s64, radix, min_digits, digit_group_separator);
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
str8_list_alloc_aligner(AllocatorInfo ainfo, String8List* list, U64 min, U64 align) {
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
	return str8_list_alloc_aligner(arena_allocator(arena), list, min, align);
#endif
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
str8_list_alloc_copy(AllocatorInfo ainfo, String8List* list) {
	String8List result = {0};
	for (String8Node* node = list->first; node != 0; node = node->next) {
		String8Node* new_node   = alloc_array_no_zero(ainfo, String8Node, 1);
		String8      new_string = alloc_str8_copy(ainfo, node->string);
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
		memory_copy_struct(&join, optional_params);
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
	// TODO(Ed): Review
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

UnicodeDecode
utf8_decode(U8* str, U64 max)
{
	UnicodeDecode result = {1, MAX_U32};
	U8 byte       = str[0];
	U8 byte_class = utf8_class(byte >> 3);
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
				if (utf8_class(cont_byte >> 3) == 0)
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
				if (utf8_class(cont_byte[0] >> 3) == 0 &&
					utf8_class(cont_byte[1] >> 3) == 0)
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
				if (utf8_class(cont_byte[0] >> 3) == 0 &&
					utf8_class(cont_byte[1] >> 3) == 0 &&
					utf8_class(cont_byte[2] >> 3) == 0)
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

String8
str8_from_16(Arena* arena, String16 in) {
	U64  cap  = in.size * 3;
	U8*  str  = push_array_no_zero(arena, U8, cap + 1);
	U16* ptr  = in.str;
	U16* opl  = ptr + in.size;
	U64  size = 0;
	UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = utf16_decode(ptr, opl - ptr);
		size   += utf8_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	arena_pop(arena, (cap - size));
	return(str8(str, size));
}

String16
str16_from_8(Arena* arena, String8 in) {
	U64  cap  = in.size * 2;
	U16* str  = push_array_no_zero(arena, U16, cap + 1);
	U8*  ptr  = in.str;
	U8*  opl  = ptr + in.size;
	U64  size = 0;
	UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = utf8_decode(ptr, opl - ptr);
		size   += utf16_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	arena_pop(arena, (cap - size)*2);
	return(str16(str, size));
}

String8
str8_from_32(Arena *arena, String32 in){
	U64  cap  = in.size * 4;
	U8*  str  = push_array_no_zero(arena, U8, cap + 1);
	U32* ptr  = in.str;
	U32* opl  = ptr + in.size;
	U64  size = 0;
	for (;ptr < opl; ptr += 1){
		size += utf8_encode(str + size, *ptr);
	}
	str[size] = 0;
	arena_pop(arena, (cap - size));
	return(str8(str, size));
}

String32
str32_from_8(Arena *arena, String8 in){
	U64  cap  = in.size;
	U32* str  = push_array_no_zero(arena, U32, cap + 1);
	U8*  ptr  = in.str;
	U8*  opl  = ptr + in.size;
	U64  size = 0;
	UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc){
		consume   = utf8_decode(ptr, opl - ptr);
		str[size] = consume.codepoint;
		size     += 1;
	}
	str[size] = 0;
	arena_pop(arena, (cap - size)*4);
	return(str32(str, size));
}

String8
str8_from_16_alloc(AllocatorInfo ainfo, String16 in) {
	U64  cap  = in.size * 3;
	U8*  str  = alloc_array_no_zero(ainfo, U8, cap + 1);
	U16* ptr  = in.str;
	U16* opl  = ptr + in.size;
	U64  size = 0;
	UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = utf16_decode(ptr, opl - ptr);
		size   += utf8_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	resize(ainfo, str, cap + 1, (cap - size));
	return(str8(str, size));
}

String16
str16_from_8_alloc(AllocatorInfo ainfo, String8 in) {
	U64  cap  = in.size * 2;
	U16* str  = alloc_array_no_zero(ainfo, U16, cap + 1);
	U8*  ptr  = in.str;
	U8*  opl  = ptr + in.size;
	U64  size = 0;
	UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = utf8_decode(ptr, opl - ptr);
		size   += utf16_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	resize(ainfo, str, cap + 1, (cap - size));
	return(str16(str, size));
}

String8
str8_from_32_alloc(AllocatorInfo ainfo, String32 in){
	U64  cap  = in.size * 4;
	U8*  str  = alloc_array_no_zero(ainfo, U8, cap + 1);
	U32* ptr  = in.str;
	U32* opl  = ptr + in.size;
	U64  size = 0;
	for (;ptr < opl; ptr += 1){
		size += utf8_encode(str + size, *ptr);
	}
	str[size] = 0;
	resize(ainfo, str, cap + 1, (cap - size));
	return(str8(str, size));
}

String32
str32_from_8_alloc(AllocatorInfo ainfo, String8 in){
	U64  cap  = in.size;
	U32* str  = alloc_array_no_zero(ainfo, U32, cap + 1);
	U8*  ptr  = in.str;
	U8*  opl  = ptr + in.size;
	U64  size = 0;
	UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc){
		consume   = utf8_decode(ptr, opl - ptr);
		str[size] = consume.codepoint;
		size     += 1;
	}
	str[size] = 0;
	resize(ainfo, str, cap + 1, (cap - size));
	return(str32(str, size));
}

////////////////////////////////
//~ String -> Enum Conversions

typedef struct OS_EnumMap OS_EnumMap;
struct OS_EnumMap
{
	String8         string;
	OperatingSystem os;
};

read_only global OS_EnumMap g_os_enum_map[] =
{
	{ str8_lit_comp(""),        OperatingSystem_Null     },
	{ str8_lit_comp("Windows"), OperatingSystem_Windows, },
	{ str8_lit_comp("Linux"),   OperatingSystem_Linux,   },
	{ str8_lit_comp("Mac"),     OperatingSystem_Mac,     },
};
md_static_assert(array_count(g_os_enum_map) == OperatingSystem_COUNT, g_os_enum_map_count_check);

OperatingSystem
operating_system_from_string(String8 string)
{
	for (U64 i = 0; i < array_count(g_os_enum_map); ++i) {
		if(str8_match(g_os_enum_map[i].string, string, StringMatchFlag_CaseInsensitive)) {
			return g_os_enum_map[i].os;
		}
	}
	return OperatingSystem_Null;
}

////////////////////////////////
//~ rjf: Time Types -> String

String8
push_date_time_string(Arena* arena, DateTime* date_time) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	char* mon_str = (char*)string_from_month(date_time->month).str;

	U32 adjusted_hour = date_time->hour % 12;
	if (adjusted_hour == 0){
		adjusted_hour = 12;
	}

	char* ampm = "am";
	if (date_time->hour >= 12){
		ampm = "pm";
	}

	String8 result = push_str8f(arena, 
		"%d %s %d, %02d:%02d:%02d %s",
		date_time->day, mon_str, date_time->year,
		adjusted_hour, date_time->min, date_time->sec, ampm
	);
	return(result);
#else
	return alloc_date_time_string(arena_allocator(arena), date_time);
#endif
}

String8
push_file_name_date_time_string(Arena* arena, DateTime* date_time) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	char* mon_str = (char*)string_from_month(date_time->month).str;

	String8 result = push_str8f(arena, 
		"%d-%s-%0d--%02d-%02d-%02d",
		date_time->year, mon_str, date_time->day,
		date_time->hour, date_time->min, date_time->sec
	);
	return(result);
#else
	return alloc_file_name_date_time_string(arena_allocator(arena), date_time);
#endif
}

String8
string_from_elapsed_time(Arena* arena, DateTime dt) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	TempArena   scratch = scratch_begin(&arena, 1);
	String8List list    = {0};
	if (dt.year) {
		str8_list_pushf(scratch.arena, &list, "%dy", dt.year);
		str8_list_pushf(scratch.arena, &list, "%um", dt.mon);
		str8_list_pushf(scratch.arena, &list, "%ud", dt.day);
	} else if (dt.mon) {
		str8_list_pushf(scratch.arena, &list, "%um", dt.mon);
		str8_list_pushf(scratch.arena, &list, "%ud", dt.day);
	} else if (dt.day) {
		str8_list_pushf(scratch.arena, &list, "%ud", dt.day);
	}
	str8_list_pushf(scratch.arena, &list, "%u:%u:%u:%u ms", dt.hour, dt.min, dt.sec, dt.msec);
	StringJoin join   = { str8_lit_comp(""), str8_lit_comp(" "), str8_lit_comp("") };
	String8    result = str8_list_join(arena, &list, &join);
	scratch_end(scratch);
	return(result);
#else
	return string_from_elapsed_time_alloc(arena_allocator(arena), dt);
#endif
}

String8
alloc_date_time_string(AllocatorInfo ainfo, DateTime* date_time) {
	char* mon_str = (char*)string_from_month(date_time->month).str;

	U32 adjusted_hour = date_time->hour % 12;
	if (adjusted_hour == 0){
		adjusted_hour = 12;
	}

	char* ampm = "am";
	if (date_time->hour >= 12){
		ampm = "pm";
	}

	String8 result = alloc_str8f(ainfo, 
		"%d %s %d, %02d:%02d:%02d %s",
		date_time->day, mon_str, date_time->year,
		adjusted_hour, date_time->min, date_time->sec, ampm
	);
	return(result);
}

String8
alloc_file_name_date_time_string(AllocatorInfo ainfo, DateTime* date_time) {
	char* mon_str = (char*)string_from_month(date_time->month).str;

	String8 result = alloc_str8f(ainfo, 
		"%d-%s-%0d--%02d-%02d-%02d",
		date_time->year, mon_str, date_time->day,
		date_time->hour, date_time->min, date_time->sec
	);
	return(result);
}

String8
string_from_elapsed_time_alloc(AllocatorInfo ainfo, DateTime dt) {
	U8            bytes[KB(8)];
	FArena        arena   = farena_from_memory(bytes, size_of(bytes));
	AllocatorInfo scratch = farena_allocator(arena);

	String8List list = {0};
	if (dt.year) {
		str8_list_allocf(scratch, &list, "%dy", dt.year);
		str8_list_allocf(scratch, &list, "%um", dt.mon);
		str8_list_allocf(scratch, &list, "%ud", dt.day);
	} else if (dt.mon) {
		str8_list_allocf(scratch, &list, "%um", dt.mon);
		str8_list_allocf(scratch, &list, "%ud", dt.day);
	} else if (dt.day) {
		str8_list_allocf(scratch, &list, "%ud", dt.day);
	}
	str8_list_allocf(scratch, &list, "%u:%u:%u:%u ms", dt.hour, dt.min, dt.sec, dt.msec);

	StringJoin join   = { str8_lit_comp(""), str8_lit_comp(" "), str8_lit_comp("") };
	String8    result = str8_list_join_alloc(ainfo, &list, &join);
	return(result);
}

////////////////////////////////
//~ Globally UNique Ids

B32
try_guid_from_string(String8 string, Guid *guid_out)
{
	TempArena scratch = scratch_begin(0,0);
	B32 is_parsed = 0;

	String8List list = str8_split_by_string_chars(scratch.arena, string, str8_lit("-"), StringSplitFlag_KeepEmpties);
	if(list.node_count == 5)
	{
		String8 data1_str    = list.first->string;
		String8 data2_str    = list.first->next->string;
		String8 data3_str    = list.first->next->next->string;
		String8 data4_hi_str = list.first->next->next->next->string;
		String8 data4_lo_str = list.first->next->next->next->next->string;
		if( str8_is_integer(data1_str,    16) && 
			str8_is_integer(data2_str,    16) &&
			str8_is_integer(data3_str,    16) &&
			str8_is_integer(data4_hi_str, 16) &&
			str8_is_integer(data4_lo_str, 16)   )
		{
			U64 data1    = u64_from_str8(data1_str,    16);
			U64 data2    = u64_from_str8(data2_str,    16);
			U64 data3    = u64_from_str8(data3_str,    16);
			U64 data4_hi = u64_from_str8(data4_hi_str, 16);
			U64 data4_lo = u64_from_str8(data4_lo_str, 16);
			if( data1    <= MAX_U32 &&
				data2    <= MAX_U16 &&
				data3    <= MAX_U16 &&
				data4_hi <= MAX_U16 &&
				data4_lo <= 0xffffffffffff )
			{
				guid_out->data1 = (U32)data1;
				guid_out->data2 = (U16)data2;
				guid_out->data3 = (U16)data3;

				U64 data4 = (data4_hi << 48) | data4_lo;
				memory_copy(&guid_out->data4[0], &data4, sizeof(data4));
				is_parsed = 1;
			}
		}
	}
	scratch_end(scratch);
	return is_parsed;
}

////////////////////////////////
//~ rjf: Basic Text Indentation

String8
indented_from_string(Arena* arena, String8 string)
{
#if 1 // Better than enforcing abstract allocator for this case.
  TempArena scratch = scratch_begin(&arena, 1);

  read_only local_persist U8 indentation_bytes[] = "                                                                                                                                ";
  String8List indented_strings = {0};

  S64 depth          = 0;
  S64 next_depth     = 0;
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
			if(line.size != 0) {
				str8_list_pushf(scratch.arena, &indented_strings, "%.*s%S\n", (int)depth * 2, indentation_bytes, line);
			}
			line_begin_off = off + 1;
			depth          = next_depth;
		}break;
		}
  }
  String8 result = str8_list_join(arena, &indented_strings, 0);
  scratch_end(scratch);
  return result;
#else
  return indented_from_string_alloc(arena_allocator(arena), string);
#endif
}

String8
indented_from_string_alloc(AllocatorInfo ainfo, String8 string)
{
	// TODO(Ed): Review, we should just keep using thread scratch arenas (and provide them to teh context)
	Arena*    arena   = arena_alloc(.backing = ainfo, .block_size = MB(1));
	TempArena scratch = scratch_begin(&arena, 1);

	read_only local_persist U8 indentation_bytes[] = "                                                                                                                                ";
	String8List indented_strings = {0};

	S64 depth          = 0;
	S64 next_depth     = 0;
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
				if(line.size != 0) {
					str8_list_pushf(scratch.arena, &indented_strings, "%.*s%S\n", (int)depth * 2, indentation_bytes, line);
				}
				line_begin_off = off + 1;
				depth          = next_depth;
			}break;
		}
	}

	String8 result = str8_list_join_alloc(ainfo, &indented_strings, 0);
	scratch_end(scratch);
	arena_release(arena);
	return result;
}

////////////////////////////////
//~ rjf: Text Escaping

String8
escaped_from_raw_str8(Arena* arena, String8 string)
{
#if 1 // Better than enforcing abstract allocator for this case.
	TempArena scratch = scratch_begin(&arena, 1);

	String8List parts   = {0};
	U64 start_split_idx = 0;
	for(U64 idx = 0; idx <= string.size; idx += 1)
	{
		U8  byte  = (idx < string.size) ? string.str[idx] : 0;
		B32 split = 1;
		String8 separator_replace = {0};
		switch (byte)
		{
			default: { split = 0; } break;

			case 0:    {} break;
			case '\a': { separator_replace = str8_lit("\\a" ); } break;
			case '\b': { separator_replace = str8_lit("\\b" ); } break;
			case '\f': { separator_replace = str8_lit("\\f" ); } break;
			case '\n': { separator_replace = str8_lit("\\n" ); } break;
			case '\r': { separator_replace = str8_lit("\\r" ); } break;
			case '\t': { separator_replace = str8_lit("\\t" ); } break;
			case '\v': { separator_replace = str8_lit("\\v" ); } break;
			case '\\': { separator_replace = str8_lit("\\\\"); } break;
			case '"':  { separator_replace = str8_lit("\\\""); } break;
			case '?':  { separator_replace = str8_lit("\\?" ); } break;
		}
		if (split)
		{
			String8 substr  = str8_substr(string, r1u64(start_split_idx, idx));
			start_split_idx = idx + 1;
			str8_list_push(scratch.arena, &parts, substr);
			if(separator_replace.size != 0) {
				str8_list_push(scratch.arena, &parts, separator_replace);
			}
		}
	}

	StringJoin join   = {0};
	String8    result = str8_list_join(arena, &parts, &join);

	scratch_end(scratch);
	return result;
#else
	return escaped_from_raw_str8_alloc(arena_allocator(arena), string);
#endif
}

String8
escaped_from_raw_str8_alloc(AllocatorInfo ainfo, String8 string)
{
	// TODO(Ed): Review, we should just keep using thread scratch arenas (and provide them to teh context)
	Arena*    arena   = arena_alloc(.backing = ainfo, .block_size = MB(1));
	TempArena scratch = scratch_begin(&arena, 1);

	String8List parts   = {0};
	U64 start_split_idx = 0;
	for(U64 idx = 0; idx <= string.size; idx += 1)
	{
		U8  byte  = (idx < string.size) ? string.str[idx] : 0;
		B32 split = 1;
		String8 separator_replace = {0};
		switch (byte)
		{
			default: { split = 0; } break;

			case 0:    {} break;
			case '\a': { separator_replace = str8_lit("\\a" ); } break;
			case '\b': { separator_replace = str8_lit("\\b" ); } break;
			case '\f': { separator_replace = str8_lit("\\f" ); } break;
			case '\n': { separator_replace = str8_lit("\\n" ); } break;
			case '\r': { separator_replace = str8_lit("\\r" ); } break;
			case '\t': { separator_replace = str8_lit("\\t" ); } break;
			case '\v': { separator_replace = str8_lit("\\v" ); } break;
			case '\\': { separator_replace = str8_lit("\\\\"); } break;
			case '"':  { separator_replace = str8_lit("\\\""); } break;
			case '?':  { separator_replace = str8_lit("\\?" ); } break;
		}
		if (split)
		{
			String8 substr  = str8_substr(string, r1u64(start_split_idx, idx));
			start_split_idx = idx + 1;
			str8_list_push(scratch.arena, &parts, substr);
			if(separator_replace.size != 0) {
				str8_list_push(scratch.arena, &parts, separator_replace);
			}
		}
	}

	StringJoin join   = {0};
	String8    result = str8_list_join_alloc(ainfo, &parts, &join);

	scratch_end(scratch);
	arena_release(arena);
	return result;
}

String8
raw_from_escaped_str8(Arena* arena, String8 string)
{
#if 1 // Better than enforcing abstract allocator for this case.
	TempArena scratch = scratch_begin(&arena, 1);
	String8List strs  = {0};
	U64         start = 0;
	for (U64 idx = 0; idx <= string.size; idx += 1)
	{
		if (idx == string.size || string.str[idx] == '\\' || string.str[idx] == '\r') {
			String8 str = str8_substr(string, r1u64(start, idx));
			if (str.size != 0) {
				str8_list_push(scratch.arena, &strs, str);
			}
			start = idx + 1;
		}
		if (idx < string.size && string.str[idx] == '\\') {
			U8 next_char    = string.str[idx+1];
			U8 replace_byte = 0;
			switch(next_char)
			{
				default: {} break;
				case 'a':  replace_byte = 0x07; break;
				case 'b':  replace_byte = 0x08; break;
				case 'e':  replace_byte = 0x1b; break;
				case 'f':  replace_byte = 0x0c; break;
				case 'n':  replace_byte = 0x0a; break;
				case 'r':  replace_byte = 0x0d; break;
				case 't':  replace_byte = 0x09; break;
				case 'v':  replace_byte = 0x0b; break;
				case '\\': replace_byte = '\\'; break;
				case '\'': replace_byte = '\''; break;
				case '"':  replace_byte = '"';  break;
				case '?':  replace_byte = '?';  break;
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
#else
	return raw_from_escaped_str8(arena_allocator(arena), string);
#endif
}

String8
raw_from_escaped_str8_alloc(AllocatorInfo ainfo, String8 string)
{
	// TODO(Ed): Review, we should just keep using thread scratch arenas (and provide them to teh context)
	Arena*    arena   = arena_alloc(.backing = ainfo, .block_size = MB(1));
	TempArena scratch = scratch_begin(&arena, 1);

	String8List strs  = {0};
	U64         start = 0;
	for (U64 idx = 0; idx <= string.size; idx += 1)
	{
		if (idx == string.size || string.str[idx] == '\\' || string.str[idx] == '\r') {
			String8 str = str8_substr(string, r1u64(start, idx));
			if (str.size != 0) {
				str8_list_push(scratch.arena, &strs, str);
			}
			start = idx + 1;
		}
		if (idx < string.size && string.str[idx] == '\\') {
			U8 next_char    = string.str[idx+1];
			U8 replace_byte = 0;
			switch(next_char)
			{
				default: {} break;
				case 'a':  replace_byte = 0x07; break;
				case 'b':  replace_byte = 0x08; break;
				case 'e':  replace_byte = 0x1b; break;
				case 'f':  replace_byte = 0x0c; break;
				case 'n':  replace_byte = 0x0a; break;
				case 'r':  replace_byte = 0x0d; break;
				case 't':  replace_byte = 0x09; break;
				case 'v':  replace_byte = 0x0b; break;
				case '\\': replace_byte = '\\'; break;
				case '\'': replace_byte = '\''; break;
				case '"':  replace_byte = '"';  break;
				case '?':  replace_byte = '?';  break;
			}

			String8 replace_string = push_str8_copy(scratch.arena, str8(&replace_byte, 1));
			str8_list_push(scratch.arena, &strs, replace_string);
			idx += 1;
			start += 1;
		}
	}

	String8 result = str8_list_join_alloc(ainfo, &strs, 0);
	scratch_end(scratch);
	arena_release(arena);
	return result;
}

////////////////////////////////
//~ rjf: Text Wrapping

String8List
wrapped_lines_from_string(Arena* arena, String8 string, U64 first_line_max_width, U64 max_width, U64 wrap_indent)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
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
#else
	return wrapped_lines_from_string_alloc(arena_allocator(arena), string, first_line_max_width, max_width, wrap_indent);
#endif
}

String8List
wrapped_lines_from_string_alloc(AllocatorInfo ainfo, String8 string, U64 first_line_max_width, U64 max_width, U64 wrap_indent)
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
			str8_list_alloc(ainfo, &list, substr);
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
					line = alloc_str8f(ainfo, "%.*s%S", wrapped_indent_level, spaces, line);
				}
				str8_list_alloc(ainfo, &list, line);
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
			line = alloc_str8f(ainfo, "%.*s%S", wrapped_indent_level, spaces, line);
		}
		str8_list_alloc(ainfo, &list, line);
	}
	return list;
}

////////////////////////////////
//~ rjf: String <-> Color

Vec4F32
rgba_from_hex_string_4f32(String8 hex_string)
{
	U8  byte_text[8]  = {0};
	U64 byte_text_idx = 0;
	for(U64 idx = 0; idx < hex_string.size && byte_text_idx < array_count(byte_text); idx += 1)
	{
		if(char_is_digit(hex_string.str[idx], 16)) {
			byte_text[byte_text_idx] = char_to_lower(hex_string.str[idx]);
			byte_text_idx           += 1;
		}
	}
	U8 byte_vals[4] = {0};
	for(U64 idx = 0; idx < 4; idx += 1) {
		byte_vals[idx] = (U8)u64_from_str8(str8(&byte_text[idx*2], 2), 16);
	}
	Vec4F32 rgba = v4f32(byte_vals[0] / 255.f, byte_vals[1] / 255.f, byte_vals[2] / 255.f, byte_vals[3] / 255.f);
	return  rgba;
}

////////////////////////////////
//~ rjf: String Fuzzy Matching

FuzzyMatchRangeList
fuzzy_match_find(Arena *arena, String8 needle, String8 haystack)
{
#if 1 // Better than enforcing abstract allocator for this case.
	TempArena   scratch = scratch_begin(&arena, 1);
	String8List needles = str8_split(scratch.arena, needle, (U8*)" ", 1, 0);

	FuzzyMatchRangeList 
	result = {0};
	result.needle_part_count = needles.node_count;
	for(String8Node* needle_n = needles.first; needle_n != 0; needle_n = needle_n->next)
	{
		U64 find_pos = 0;
		for(;find_pos < haystack.size;)
		{
			find_pos = str8_find_needle(haystack, find_pos, needle_n->string, StringMatchFlag_CaseInsensitive);
			B32 is_in_gathered_ranges = 0;
			for (FuzzyMatchRangeNode* n = result.first; n != 0; n = n->next)
			{
				if (n->range.min <= find_pos && find_pos < n->range.max) {
					is_in_gathered_ranges = 1;
					find_pos              = n->range.max;
					break;
				}
			}
			if( ! is_in_gathered_ranges) {
				break;
			}
		}
		if (find_pos < haystack.size) {
			Rng1U64             range = r1u64(find_pos, find_pos+needle_n->string.size);
			FuzzyMatchRangeNode* n    = push_array(arena, FuzzyMatchRangeNode, 1);
			n->range = range;
			sll_queue_push(result.first, result.last, n);
			result.count     += 1;
			result.total_dim += dim_1u64(range);
		}
	}
	scratch_end(scratch);
	return result;
#else
	return fuzzy_match_find_alloc(arena_allocator(arena), needle, haystack);
#endif
}

FuzzyMatchRangeList
fuzzy_match_find_alloc(AllocatorInfo ainfo, String8 needle, String8 haystack)
{
	// TODO(Ed): Review, we should just keep using thread scratch arenas (and provide them to teh context)
	Arena*    arena   = arena_alloc(.backing = ainfo, .block_size = MB(1));
	TempArena scratch = temp_begin(&arena, 1);

	String8List needles = str8_split(scratch.arena, needle, (U8*)" ", 1, 0);
	FuzzyMatchRangeList 
	result = {0};
	result.needle_part_count = needles.node_count;
	for(String8Node* needle_n = needles.first; needle_n != 0; needle_n = needle_n->next)
	{
		U64 find_pos = 0;
		for(;find_pos < haystack.size;)
		{
			find_pos = str8_find_needle(haystack, find_pos, needle_n->string, StringMatchFlag_CaseInsensitive);
			B32 is_in_gathered_ranges = 0;
			for (FuzzyMatchRangeNode* n = result.first; n != 0; n = n->next)
			{
				if (n->range.min <= find_pos && find_pos < n->range.max) {
					is_in_gathered_ranges = 1;
					find_pos              = n->range.max;
					break;
				}
			}
			if( ! is_in_gathered_ranges) {
				break;
			}
		}
		if (find_pos < haystack.size) {
			Rng1U64             range = r1u64(find_pos, find_pos+needle_n->string.size);
			FuzzyMatchRangeNode* n    = push_array(arena, FuzzyMatchRangeNode, 1);
			n->range = range;
			sll_queue_push(result.first, result.last, n);
			result.count     += 1;
			result.total_dim += dim_1u64(range);
		}
	}
	temp_end(scratch);
	arena_release(arena);
	return result;
}

FuzzyMatchRangeList
fuzzy_match_range_list_copy(Arena* arena, FuzzyMatchRangeList* src)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	FuzzyMatchRangeList dst = {0};
	for(FuzzyMatchRangeNode* src_n = src->first; src_n != 0; src_n = src_n->next)
	{
		FuzzyMatchRangeNode* dst_n = push_array(arena, FuzzyMatchRangeNode, 1);
		sll_queue_push(dst.first, dst.last, dst_n);
		dst_n->range = src_n->range;
	}
	dst.count             = src->count;
	dst.needle_part_count = src->needle_part_count;
	dst.total_dim         = src->total_dim;
	return dst;
#else
	return fuzzy_match_range_list_copy_alloc(arena_allocator(arena), src);
#endif
}

FuzzyMatchRangeList
fuzzy_match_range_list_copy_alloc(AllocatorInfo ainfo, FuzzyMatchRangeList* src)
{
	FuzzyMatchRangeList dst = {0};
	for(FuzzyMatchRangeNode* src_n = src->first; src_n != 0; src_n = src_n->next)
	{
		FuzzyMatchRangeNode* dst_n = alloc_array(ainfo, FuzzyMatchRangeNode, 1);
		sll_queue_push(dst.first, dst.last, dst_n);
		dst_n->range = src_n->range;
	}
	dst.count             = src->count;
	dst.needle_part_count = src->needle_part_count;
	dst.total_dim         = src->total_dim;
	return dst;
}

////////////////////////////////
//~ NOTE(allen): Serialization Helpers

U64
str8_serial_push_align(Arena* arena, String8List* srl, U64 align) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	Assert(is_pow2(align));
	U64 pos     = srl->total_size;
	U64 new_pos = align_pow2(pos, align);
	U64 size    = (new_pos - pos);
	
	if(size != 0)
	{
		U8* buf = push_array(arena, U8, size);
		
		String8* str = &srl->last->string;
		if (str->str + str->size == buf) {
			srl->last->string.size += size;
			srl->total_size        += size;
		}
		else {
			str8_list_push(arena, srl, str8(buf, size));
		}
	}
	return size;
#else
	return str8_serial_alloc_align(arena_allocator(arena), srl, align);
#endif
}

void*
str8_serial_push_size(Arena* arena, String8List* srl, U64 size) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	void* result = 0;
	if(size != 0)
	{
		U8*      buf = push_array_no_zero(arena, U8, size);
		String8* str = &srl->last->string;
		if (str->str + str->size == buf) {
			srl->last->string.size += size;
			srl->total_size += size;
		}
		else {
			str8_list_push(arena, srl, str8(buf, size));
		}
		result = buf;
	}
	return result;
#else
	return str8_serial_alloc_size(arena_allocator(arena), srl, size);
#endif
}

void
str8_serial_push_u64(Arena* arena, String8List* srl, U64 x) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	U8* buf = push_array_no_zero(arena, U8, 8);
	memory_copy(buf, &x, 8);
	String8* str = &srl->last->string;
	if (str->str + str->size == buf) {
		srl->last->string.size += 8;
		srl->total_size        += 8;
	}
	else {
		str8_list_push(arena, srl, str8(buf, 8));
	}
#else
	str8_serial_alloc_u64(arena_allocator(arena), srl, x);
#endif
}

void
str8_serial_push_u32(Arena* arena, String8List* srl, U32 x) {
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	U8* buf = push_array_no_zero(arena, U8, 4);
	memory_copy(buf, &x, 4);
	String8 *str = &srl->last->string;
	if (str->str + str->size == buf) {
		srl->last->string.size += 4;
		srl->total_size += 4;
	}
	else {
		str8_list_push(arena, srl, str8(buf, 4));
	}
#else
	str8_serial_alloc_u32(arena_allocator(arena), srl, x);
#endif
}

U64
str8_serial_alloc_align(AllocatorInfo ainfo, String8List* srl, U64 align) {
	Assert(is_pow2(align));
	U64 pos     = srl->total_size;
	U64 new_pos = align_pow2(pos, align);
	U64 size    = (new_pos - pos);
	
	if(size != 0)
	{
		U8* buf = alloc_array(ainfo, U8, size);
		
		String8* str = &srl->last->string;
		if (str->str + str->size == buf) {
			srl->last->string.size += size;
			srl->total_size        += size;
		}
		else {
			str8_list_alloc(ainfo, srl, str8(buf, size));
		}
	}
	return size;
}

void*
str8_serial_alloc_size(AllocatorInfo ainfo, String8List* srl, U64 size) {
	void* result = 0;
	if(size != 0)
	{
		U8*      buf = alloc_array_no_zero(ainfo, U8, size);
		String8* str = &srl->last->string;
		if (str->str + str->size == buf) {
			srl->last->string.size += size;
			srl->total_size += size;
		}
		else {
			str8_list_alloc(ainfo, srl, str8(buf, size));
		}
		result = buf;
	}
	return result;
}

void
str8_serial_alloc_u64(AllocatorInfo ainfo, String8List* srl, U64 x) {
	U8* buf = alloc_array_no_zero(ainfo, U8, 8);
	memory_copy(buf, &x, 8);
	String8* str = &srl->last->string;
	if (str->str + str->size == buf) {
		srl->last->string.size += 8;
		srl->total_size        += 8;
	}
	else {
		str8_list_alloc(ainfo, srl, str8(buf, 8));
	}
}

void
str8_serial_push_u32(AllocatorInfo ainfo, String8List* srl, U32 x) {
	U8* buf = alloc_array_no_zero(ainfo, U8, 4);
	memory_copy(buf, &x, 4);
	String8 *str = &srl->last->string;
	if (str->str + str->size == buf) {
		srl->last->string.size += 4;
		srl->total_size += 4;
	}
	else {
		str8_list_alloc(ainfo, srl, str8(buf, 4));
	}
}

////////////////////////////////
//~ rjf: Deserialization Helpers

U64
str8_deserial_read(String8 string, U64 off, void *read_dst, U64 read_size, U64 granularity) {
	U64 bytes_left             = string.size - min(off, string.size);
	U64 actually_readable_size = min(bytes_left, read_size);
	U64 legally_readable_size  = actually_readable_size - actually_readable_size % granularity;
	if(legally_readable_size > 0)
	{
		memory_copy(read_dst, string.str + off, legally_readable_size);
	}
	return legally_readable_size;
}

U64
str8_deserial_find_first_match(String8 string, U64 off, U16 scan_val) {
	U64 cursor = off;
	for (;;) 
	{
		U16 val = 0;
		str8_deserial_read_struct(string, cursor, &val);
		if (val == scan_val) {
			break;
		}
		cursor += sizeof(val);
	}
	return cursor;
}

U64
str8_deserial_read_cstr(String8 string, U64 off, String8* cstr_out) {
	U64 cstr_size = 0;
	if (off < string.size) {
		U8* ptr   = string.str + off;
		U8* cap   = string.str + string.size;
		*cstr_out = str8_cstring_capped(ptr, cap);
		cstr_size = (cstr_out->size + 1);
	}
	return cstr_size;
}

U64
str8_deserial_read_windows_utf16_string16(String8 string, U64 off, String16* str_out)
{
	U64 null_off = str8_deserial_find_first_match(string, off, 0);
	U64 size     = null_off - off;
	U16 *str     = (U16*)str8_deserial_get_raw_ptr(string, off, size);
	U64 count    = size / sizeof(*str);
	
	*str_out = str16(str, count);
	
	U64    read_size_with_null = size + sizeof(*str);
	return read_size_with_null;
}

U64
str8_deserial_read_uleb128(String8 string, U64 off, U64 *value_out) {
	U64 value  = 0;
	U64 shift  = 0;
	U64 cursor = off;
	for(;;)
	{
		U8  byte       = 0;
		U64 bytes_read = str8_deserial_read_struct(string, cursor, &byte);
		if (bytes_read != sizeof(byte)) { 
			break;
		}

		U8 val = byte & 0x7fu;
		value |= ((U64)val) << shift;

		cursor += bytes_read;
		shift  += 7u;
		if ((byte & 0x80u) == 0) {
			break;
		}
	}
	if(value_out != 0) { *value_out = value; }

	U64    bytes_read = cursor - off;
	return bytes_read;
}

U64
str8_deserial_read_sleb128(String8 string, U64 off, S64 *value_out)
{
	U64 value  = 0;
	U64 shift  = 0;
	U64 cursor = off;
	for(;;)
	{
		U8  byte;
		U64 bytes_read = str8_deserial_read_struct(string, cursor, &byte);
		if (bytes_read != sizeof(byte)) {
			break;
		}

		U8 val = byte & 0x7fu;
		value |= ((U64)val) << shift;

		cursor += bytes_read;
		shift  += 7u;

		if ((byte & 0x80u) == 0)
		{
			if (shift < sizeof(value) * 8 && (byte & 0x40u) != 0) {
				value |= -(S64)(1ull << shift);
			}
			break;
		}
	}
	if (value_out != 0) { *value_out = value; }

	U64    bytes_read = cursor - off;
	return bytes_read;
}
