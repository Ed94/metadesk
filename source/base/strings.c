#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "macros.h"
#	include "debug.h"
#	include "strings.h"
#	include "thread_context.h"
#	define STB_SPRINTF_IMPLEMENTATION
#	define STB_SPRINTF_STATIC
#	include "third_party/stb/stb_sprintf.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: String Matching

MD_B32
md_str8_match(MD_String8 a, MD_String8 b, MD_StringMatchFlags flags)
{
	MD_B32 result = 0;
	if (a.size == b.size || (flags & MD_StringMatchFlag_RightSideSloppy))
	{
		MD_B32 case_insensitive  = (flags & MD_StringMatchFlag_CaseInsensitive);
		MD_B32 slash_insensitive = (flags & MD_StringMatchFlag_SlashInsensitive);
		MD_U64 size              = md_min(a.size, b.size);
		result = 1;
		for (MD_U64 i = 0; i < size; i += 1)
		{
			MD_U8 at = a.str[i];
			MD_U8 bt = b.str[i];
			if (case_insensitive) {
				at = md_char_to_upper(at);
				bt = md_char_to_upper(bt);
			}
			if (slash_insensitive) {
				at = md_char_to_correct_slash(at);
				bt = md_char_to_correct_slash(bt);
			}
			if (at != bt) {
				result = 0;
				break;
			}
		}
	}
	return(result);
}

MD_U64
md_str8_find_needle(MD_String8 string, MD_U64 start_pos, MD_String8 needle, MD_StringMatchFlags flags)
{
	MD_U8* p           = string.str + start_pos;
	MD_U64 stop_offset = md_max(string.size + 1, needle.size) - needle.size;
	MD_U8* stop_p      = string.str + stop_offset;
	if (needle.size > 0)
	{
		MD_U8*              md_string_opl  = string.str + string.size;
		MD_String8          needle_tail    = md_str8_skip(needle, 1);
		MD_StringMatchFlags adjusted_flags = flags | MD_StringMatchFlag_RightSideSloppy;
		MD_U8 needle_first_char_adjusted   = needle.str[0];
		if (adjusted_flags & MD_StringMatchFlag_CaseInsensitive) {
			needle_first_char_adjusted = md_char_to_upper(needle_first_char_adjusted);
		}
		for (;p < stop_p; p += 1)
		{
			MD_U8 haystack_char_adjusted = *p;
			if(adjusted_flags & MD_StringMatchFlag_CaseInsensitive) {
				haystack_char_adjusted = md_char_to_upper(haystack_char_adjusted);
			}
			if (haystack_char_adjusted == needle_first_char_adjusted) {
				if (md_str8_match(md_str8_range(p + 1, md_string_opl), needle_tail, adjusted_flags)){
					break;
				}
			}
		}
	}
	MD_U64 result = string.size;
	if (p < stop_p){
		result = (MD_U64)(p - string.str);
	}
	return(result);
}

MD_U64
md_str8_find_needle_reverse(MD_String8 string, MD_U64 start_pos, MD_String8 needle, MD_StringMatchFlags flags) {
	MD_U64 result = 0;
	for (MD_S64 i = string.size - start_pos - needle.size; i >= 0; --i)
	{
		MD_String8 haystack = md_str8_substr(string, md_rng_1u64(i, i + needle.size));
		if (md_str8_match(haystack, needle, flags)) {
			result = (MD_U64)i + needle.size;
			break;
		}
	}
	return result;
}

////////////////////////////////
//~ rjf: String Slicing

MD_String8
md_str8_skip_chop_whitespace(MD_String8 string)
{
	MD_U8* first = string.str;
	MD_U8* opl   = first + string.size;
	for (; first < opl; first += 1) {
		if ( ! md_char_is_space(*first)) { break;}
	}
	for (; opl > first; ){
		opl -= 1;
		if ( ! md_char_is_space(*opl)) { opl += 1; break; }
	}
	MD_String8 result = md_str8_range(first, opl);
	return(result);
}

////////////////////////////////
//~ rjf: String Formatting & Copying

MD_String8
md_str8_cat__ainfo(MD_AllocatorInfo ainfo, MD_String8 s1, MD_String8 s2)
{
	MD_String8 str;
	str.size = s1.size + s2.size;
	str.str  = md_alloc_array_no_zero(ainfo, MD_U8, str.size + 1);
	md_memory_copy(str.str,           s1.str, s1.size);
	md_memory_copy(str.str + s1.size, s2.str, s2.size);
	str.str[str.size] = 0;
	return str;
}

MD_String8
md_str8_copy__ainfo(MD_AllocatorInfo ainfo, MD_String8 s)
{
	MD_String8 str;
	str.size = s.size;
	str.str  = md_alloc_array_no_zero(ainfo, MD_U8, str.size + 1);
	md_memory_copy(str.str, s.str, s.size);
	str.str[str.size] = 0;
	return(str);
}

MD_String8
md_str8fv__ainfo(MD_AllocatorInfo ainfo, char* fmt, va_list args){
	va_list args2;
	va_copy(args2, args);
	MD_U32     needed_bytes = md_vsnprintf(0, 0, fmt, args) + 1;
	MD_String8 result       = {0};
	result.str  = md_alloc_array_no_zero(ainfo, MD_U8, needed_bytes);
	result.size = md_vsnprintf((char*)result.str, needed_bytes, fmt, args2);
	result.str[result.size] = 0;
	va_end(args2);
	return(result);
}

////////////////////////////////
//~ rjf: String <=> Integer Conversions

//- rjf: string -> integer

MD_S64
md_sign_from_str8(MD_String8 string, MD_String8* md_string_tail)
{
	// count negative signs
	MD_U64 neg_count = 0;
	MD_U64 i         = 0;
	for (; i < string.size; i += 1){
		if (string.str[i] == '-') {
			neg_count += 1;
		}
		else if (string.str[i] != '+') {
			break;
		}
	}
	// output part of string after signs
	*md_string_tail = md_str8_skip(string, i);
	// output integer sign
	MD_S64 sign = (neg_count & 1)?-1:+1;
	return(sign);
}

MD_B32
md_str8_is_integer(MD_String8 string, MD_U32 radix){
	MD_B32 result = 0;
	if (string.size > 0 && (1 < radix && radix <= 16)) {
		result = 1;
		for (MD_U64 i = 0; i < string.size; i += 1) {
			MD_U8 c = string.str[i];
			if ( ! (c < 0x80) || md_integer_symbol_reverse(c) >= radix){
				result = 0;
				break;
			}
		}
	}
	return(result);
}

MD_B32
md_try_u64_from_str8_c_rules(MD_String8 string, MD_U64 *x) 
{
	MD_B32 is_integer = 0;
	if (md_str8_is_integer(string, 10)) {
		is_integer = 1;
		*x = md_u64_from_str8(string, 10);
	}
	else
	{
		MD_String8 hex_string = md_str8_skip(string, 2);
		if (md_str8_match(md_str8_prefix(string, 2), md_str8_lit("0x"), 0) &&
			md_str8_is_integer(hex_string, 0x10))
		{
			is_integer = 1;
			*x = md_u64_from_str8(hex_string, 0x10);
		}
		else if (md_str8_match(md_str8_prefix(string, 2), md_str8_lit("0b"), 0) &&
				 md_str8_is_integer(hex_string, 2))
		{
			is_integer = 1;
			*x = md_u64_from_str8(hex_string, 2);
		}
		else
		{
			MD_String8 oct_string = md_str8_skip(string, 1);
			if (md_str8_match(md_str8_prefix(string, 1), md_str8_lit("0"), 0) &&
				md_str8_is_integer(hex_string, 010))
			{
				is_integer = 1;
				*x = md_u64_from_str8(oct_string, 010);
			}
		}
	}
	return(is_integer);
}

//- rjf: integer -> string

MD_String8
md_str8_from_memory_size__ainfo(MD_AllocatorInfo ainfo, MD_SSIZE z) {
	MD_String8 result = {0};
	if (z < MD_KB(1)) {
		result = md_str8f(ainfo, "%llu  b", z);
	}
	else if (z < MD_MB(1)) {
		result = md_str8f(ainfo, "%llu.%02llu Kb", z / MD_KB(1), ((100 * z) / MD_KB(1)) % 100);
	}
	else if (z < MD_GB(1)) {
		result = md_str8f(ainfo, "%llu.%02llu Mb", z / MD_MB(1), ((100 * z) / MD_MB(1)) % 100);
	}
	else{
		result = md_str8f(ainfo, "%llu.%02llu Gb", z / MD_GB(1), ((100 * z) / MD_GB(1)) % 100);
	}
	return(result);
}

MD_String8
md_str8_from_u64__ainfo(MD_AllocatorInfo ainfo, MD_U64 u64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator)
{
	MD_String8 result = {0};
	{
		// rjf: prefix
		MD_String8 prefix = {0};
		switch(radix)
		{
			case 16:{prefix = md_str8_lit("0x");}break;
			case 8: {prefix = md_str8_lit("0o");}break;
			case 2: {prefix = md_str8_lit("0b");}break;
		}
		
		// rjf: determine # of chars between separators
		MD_U8 digit_group_size = 3;
		switch(radix)
		{
			default:break;
			case 2:
			case 8:
			case 16:
				{digit_group_size = 4;} break;
		}
		
		// rjf: prep
		MD_U64 needed_leading_0s = 0;
		{
			MD_U64 needed_digits = 1;
			{
				MD_U64 u64_reduce = u64;
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
			MD_U64 needed_separators = 0;
			if (digit_group_separator != 0)
			{
				needed_separators = (needed_digits + needed_leading_0s) / digit_group_size;
				if(needed_separators > 0 && (needed_digits + needed_leading_0s) % digit_group_size == 0)
				{
					needed_separators -= 1;
				}
			}
			result.size = prefix.size + needed_leading_0s + needed_separators + needed_digits;
			result.str  = md_alloc_array_no_zero(ainfo, MD_U8, result.size + 1);
			result.str[result.size] = 0;
		}
		
		// rjf: fill contents
		{
			MD_U64 u64_reduce             = u64;
			MD_U64 digits_until_separator = digit_group_size;
			for (MD_U64 idx = 0; idx < result.size; idx += 1)
			{
				if(digits_until_separator == 0 && digit_group_separator != 0) {
					result.str[result.size - idx - 1] = digit_group_separator;
					digits_until_separator = digit_group_size + 1;
				}
				else {
					result.str[result.size - idx - 1] = md_char_to_lower(md_integer_symbols(u64_reduce % radix));
					u64_reduce /= radix;
				}
				digits_until_separator -= 1;
				if(u64_reduce == 0) {
					break;
				}
			}
			for(MD_U64 leading_0_idx = 0; leading_0_idx < needed_leading_0s; leading_0_idx += 1)
			{
				result.str[prefix.size + leading_0_idx] = '0';
			}
		}
		
		// rjf: fill prefix
		if(prefix.size != 0)
		{
			md_memory_copy(result.str, prefix.str, prefix.size);
		}
	}
	return result;
}

MD_String8
md_str8_from_s64__ainfo(MD_AllocatorInfo ainfo, MD_S64 s64, MD_U32 radix, MD_U8 min_digits, MD_U8 digit_group_separator)
{
	MD_String8 result = {0};
	if(s64 < 0) {
		MD_U8      bytes[MD_KB(1)];
		MD_FArena  scratch      = md_farena_from_memory(bytes, size_of(bytes));
		MD_String8 numeric_part = md_str8_from_u64(farena_allocator(scratch), (MD_U64)(-s64), radix, min_digits, digit_group_separator);
		result = md_str8f(ainfo, "-%S", numeric_part);
	}
	else {
		result = md_str8_from_u64(ainfo, (MD_U64)s64, radix, min_digits, digit_group_separator);
	}
	return result;
}

////////////////////////////////
//~ rjf: String <=> Float Conversions

MD_F64
md_f64_from_str8(MD_String8 string)
{
	// TODO(rjf): crappy implementation for now that just uses atof.
	MD_F64 result = 0;
	if (string.size > 0)
	{
		// rjf: find starting pos of numeric string, as well as sign
		MD_F64 sign = +1.0;
		//MD_U64 first_numeric = 0;
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
		MD_U64  num_valid_chars = 0;
		char buffer[64];
		for(MD_U64 idx = 0; idx < string.size && num_valid_chars < sizeof(buffer)-1; idx += 1)
		{
			if(md_char_is_digit(string.str[idx], 10) || string.str[idx] == '.')
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
md_str8_list_concat_in_place(MD_String8List* list, MD_String8List* to_push) {
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
		md_memory_zero_struct(to_push);
	}
}

MD_String8Node*
md_str8_list_aligner__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, MD_U64 md_min, MD_U64 align) {
	MD_String8Node* node = md_alloc_array_no_zero(ainfo, MD_String8Node, 1);
	MD_U64 new_size = list->total_size + md_min;
	MD_U64 increase_size = 0;

	if (align > 1) {
		// NOTE(allen): md_assert is power of 2
		md_assert(((align - 1) & align) == 0);
		MD_U64 mask = align - 1;
		new_size += mask;
		new_size &= (~mask);
		increase_size = new_size - list->total_size;
	}

	md_local_persist const MD_U8 zeroes_buffer[64] = {0};
	md_assert(increase_size <= md_array_count(zeroes_buffer));

	md_sll_queue_push(list->first, list->last, node);
	list->node_count += 1;
	list->total_size  = new_size;
	node->string.str  = (MD_U8*)zeroes_buffer;
	node->string.size = increase_size;
	return(node);
}

MD_String8List
md_str8_list_copy__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list) {
	MD_String8List result = {0};
	for (MD_String8Node* node       = list->first; node != 0; node = node->next) {
		MD_String8Node*  new_node   = md_alloc_array_no_zero(ainfo, MD_String8Node, 1);
		MD_String8       new_string = md_str8_copy(ainfo, node->string);
		md_str8_list_push_node_set_string(&result, new_node, new_string);
	}
	return(result);
}

////////////////////////////////
//~ rjf: String Splitting & Joining

MD_String8List
md_str8_split__ainfo(MD_AllocatorInfo ainfo, MD_String8 string, MD_U8* split_chars, MD_U64 split_char_count, MD_StringSplitFlags flags)
{
	MD_String8List list = {0};
  
	MD_B32 keep_empties = (flags & MD_StringSplitFlag_KeepEmpties);
  
	MD_U8* ptr = string.str;
	MD_U8* opl = string.str + string.size;
	for (;ptr < opl;)
	{
		MD_U8* first = ptr;
		for (;ptr < opl; ptr += 1)
		{
			MD_U8  c        = *ptr;
			MD_B32 is_split = 0;
			for (MD_U64 i = 0; i < split_char_count; i += 1) {
				if (split_chars[i] == c) {
					is_split = 1;
					break;
				}
			}
			if (is_split){
				break;
			}
		}
    
		MD_String8 string = md_str8_range(first, ptr);
		if (keep_empties || string.size > 0){
			md_str8_list_push(ainfo, &list, string);
		}
		ptr += 1;
	}
	return(list);
}

MD_String8
md_str8_list_join__ainfo(MD_AllocatorInfo ainfo, MD_String8List* list, MD_StringJoin* optional_params) 
{
	MD_StringJoin join      = {0};
	if (optional_params != md_nullptr) {
		md_memory_copy_struct(&join, optional_params);
	}
	MD_U64        sep_count = 0;
	if (list->node_count > 0){
		sep_count = list->node_count - 1;
	}
	
	MD_String8 result;
	result.size = join.pre.size + join.post.size + sep_count * join.sep.size + list->total_size;

	MD_U8* ptr = result.str = md_alloc_array_no_zero(ainfo, MD_U8, result.size + 1);
	md_memory_copy(ptr, join.pre.str, join.pre.size);
	ptr += join.pre.size;

	for (MD_String8Node *node = list->first; node != 0; node = node->next) {
		md_memory_copy(ptr, node->string.str, node->string.size);
		ptr += node->string.size;

		if (node->next != 0) {
			md_memory_copy(ptr, join.sep.str, join.sep.size);
			ptr += join.sep.size;
		}
	}

	md_memory_copy(ptr, join.post.str, join.post.size);
	ptr  += join.post.size;
	*ptr  = 0;
	return(result);
}

////////////////////////////////
//~ rjf: String Path Helpers

MD_String8
md_str8_chop_last_slash(MD_String8 string) {
	if (string.size > 0)
	{
		MD_U8* ptr = string.str + string.size - 1;
		for (;ptr >= string.str; ptr -= 1) {
			if (*ptr == '/' || *ptr == '\\') break;
		}
		if (ptr >= string.str) {
			string.size = (MD_U64)(ptr - string.str);
		}
		else {
			string.size = 0;
		}
	}
	return(string);
}

MD_String8
md_str8_skip_last_slash(MD_String8 string) {
	if (string.size > 0)
	{
		MD_U8* ptr = string.str + string.size - 1;
		for (;ptr >= string.str; ptr -= 1) {
			if (*ptr == '/' || *ptr == '\\') break;
		}
		if (ptr >= string.str) {
			ptr += 1;
			string.size = (MD_U64)(string.str + string.size - ptr);
			string.str = ptr;
		}
	}
	return(string);
}

MD_String8
md_str8_chop_last_dot(MD_String8 string) {
	MD_String8 result = string;
	MD_U64 p = string.size;
	for (;p > 0;) {
		p -= 1;
		if (string.str[p] == '.') {
			result = md_str8_prefix(string, p);
			break;
		}
	}
	return(result);
}

MD_String8
md_str8_skip_last_dot(MD_String8 string) {
	MD_String8 result = string;
	MD_U64 p = string.size;
	for (;p > 0;) {
		p -= 1;
		if (string.str[p] == '.') {
			result = md_str8_skip(string, p + 1);
			break;
		}
	}
	return(result);
}

MD_PathStyle
md_path_style_from_str8(MD_String8 string) {
	MD_PathStyle result = MD_PathStyle_Relative;
	if (string.size >= 1 && string.str[0] == '/') {
		result = MD_PathStyle_UnixAbsolute;
	}
	else if (string.size >= 2 && md_char_is_alpha(string.str[0]) && string.str[1] == ':')
	{
		if (string.size == 2 || md_char_is_slash(string.str[2])) {
			result = MD_PathStyle_WindowsAbsolute;
		}
	}
	return(result);
}

void
md_str8_path_list_resolve_dots_in_place(MD_String8List* path, MD_PathStyle style)
{
	MD_TempArena scratch = md_scratch_begin(0, 0);
	MD_String8MetaNode* stack          = 0;
	MD_String8MetaNode* free_meta_node = 0;
	MD_String8Node*     first          = path->first;
  
	md_memory_zero_struct(path);
	for (MD_String8Node* node = first, *next = 0; node != 0; node = next)
	{
		// save next now
		next = node->next;
		
		// cases:
		if (node == first && style == MD_PathStyle_WindowsAbsolute){
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
			md_str8_list_push_node(path, node);
				
			MD_String8MetaNode *stack_node = free_meta_node;
			if (stack_node != 0){
				md_sll_stack_pop(free_meta_node);
			}
			else{
				stack_node = md_push_array__no_zero(scratch.arena, MD_String8MetaNode, 1);
			}
			md_sll_stack_push(stack, stack_node);
			stack_node->node = node;
			continue;
		}
			
		save_without_stack:
		{
			md_str8_list_push_node(path, node);
			continue;
		}
		
		eliminate_stack_top:
		{
			path->node_count -= 1;
			path->total_size -= stack->node->string.size;

			md_sll_stack_pop(stack);
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

MD_String8
md_str8_path_list_join_by_style__ainfo(MD_AllocatorInfo ainfo, MD_String8List* path, MD_PathStyle style) {
	MD_StringJoin params = {0};
	switch (style)
	{
		case MD_PathStyle_Relative:
		case MD_PathStyle_WindowsAbsolute:
		{
			params.sep = md_str8_lit("/");
		}
		break;
		
		case MD_PathStyle_UnixAbsolute:
		{
			params.pre = md_str8_lit("/");
			params.sep = md_str8_lit("/");
		}
		break;
	}
	MD_String8 result = md_str8_list_join(ainfo, path, &params );
	return(result);
}

////////////////////////////////
//~ rjf: UTF-8 & UTF-16 Decoding/Encoding

MD_UnicodeDecode
md_utf8_decode(MD_U8* str, MD_U64 md_max)
{
	MD_UnicodeDecode result = {1, MD_MAX_U32};
	MD_U8 byte       = str[0];
	MD_U8 byte_class = md_utf8_class(byte >> 3);
	switch (byte_class)
	{
		case 1:
		{
			result.codepoint = byte;
		}
		break;

		case 2:
		{
			if (2 < md_max)
			{
				MD_U8 cont_byte = str[1];
				if (md_utf8_class(cont_byte >> 3) == 0)
				{
					result.codepoint =   (byte      & MD_BITMASK5) << 6;
					result.codepoint |=  (cont_byte & MD_BITMASK6);
					result.inc = 2;
				}
			}
		}
		break;

		case 3:
		{
			if (2 < md_max)
			{
				MD_U8 cont_byte[2] = {str[1], str[2]};
				if (md_utf8_class(cont_byte[0] >> 3) == 0 &&
					md_utf8_class(cont_byte[1] >> 3) == 0)
				{
					result.codepoint =   (byte         & MD_BITMASK4) << 12;
					result.codepoint |= ((cont_byte[0] & MD_BITMASK6) << 6);
					result.codepoint |=  (cont_byte[1] & MD_BITMASK6);
					result.inc = 3;
				}
			}
		}
		break;

		case 4:
		{
			if (3 < md_max)
			{
				MD_U8 cont_byte[3] = {str[1], str[2], str[3]};
				if (md_utf8_class(cont_byte[0] >> 3) == 0 &&
					md_utf8_class(cont_byte[1] >> 3) == 0 &&
					md_utf8_class(cont_byte[2] >> 3) == 0)
				{
					result.codepoint =   (byte         & MD_BITMASK3) << 18;
					result.codepoint |= ((cont_byte[0] & MD_BITMASK6) << 12);
					result.codepoint |= ((cont_byte[1] & MD_BITMASK6) <<  6);
					result.codepoint |=  (cont_byte[2] & MD_BITMASK6);
					result.inc = 4;
				}
			}
		}
	}
	return(result);
}

MD_U32
md_utf8_encode(MD_U8* str, MD_U32 codepoint)
{
	MD_U32 inc = 0;
	if (codepoint <= 0x7F){
		str[0] = (MD_U8)codepoint;
		inc    = 1;
	}
	else if (codepoint <= 0x7FF){
		str[0] = (MD_BITMASK2 << 6) | ((codepoint >> 6) & MD_BITMASK5);
		str[1] = MD_BIT8 | (codepoint & MD_BITMASK6);
		inc    = 2;
	}
	else if (codepoint <= 0xFFFF){
		str[0] = (MD_BITMASK3 << 5) | ((codepoint >> 12) & MD_BITMASK4);
		str[1] = MD_BIT8 | ((codepoint >> 6) & MD_BITMASK6);
		str[2] = MD_BIT8 | ( codepoint       & MD_BITMASK6);
		inc    = 3;
	}
	else if (codepoint <= 0x10FFFF){
		str[0] = (MD_BITMASK4 << 4) | ((codepoint >> 18) & MD_BITMASK3);
		str[1] = MD_BIT8 | ((codepoint >> 12) & MD_BITMASK6);
		str[2] = MD_BIT8 | ((codepoint >>  6) & MD_BITMASK6);
		str[3] = MD_BIT8 | ( codepoint        & MD_BITMASK6);
		inc    = 4;
	}
	else{
		str[0] = '?';
		inc    = 1;
	}
	return(inc);
}

MD_U32
md_utf16_encode(MD_U16* str, MD_U32 codepoint) {
	MD_U32 inc = 1;
	if (codepoint == MD_MAX_U32) {
		str[0] = (MD_U16)'?';
	}
	else if (codepoint < 0x10000) {
		str[0] = (MD_U16)codepoint;
	}
	else {
		MD_U32 v = codepoint - 0x10000;
		str[0] = md_safe_cast_u16(0xD800 + (v >> 10));
		str[1] = md_safe_cast_u16(0xDC00 + (v & MD_BITMASK10));
		inc    = 2;
	}
	return(inc);
}

////////////////////////////////
//~ rjf: Unicode String Conversions

// NOTE(Ed): These utilize arena's pop rewinding, so we'll keep the codepath diverged from _alloc paths

MD_String8
md_str8_from_str16__arena(MD_Arena* arena, MD_String16 in) {
	MD_U64  cap  = in.size * 3;
	MD_U8*  str  = md_push_array__no_zero(arena, MD_U8, cap + 1);
	MD_U16* ptr  = in.str;
	MD_U16* opl  = ptr + in.size;
	MD_U64  size = 0;
	MD_UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = md_utf16_decode(ptr, opl - ptr);
		size   += md_utf8_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	md_arena_pop(arena, (cap - size));
	return(md_str8(str, size));
}

MD_String16
md_str16_from_str8__arena(MD_Arena* arena, MD_String8 in) {
	MD_U64  cap  = in.size * 2;
	MD_U16* str  = md_push_array__no_zero(arena, MD_U16, cap + 1);
	MD_U8*  ptr  = in.str;
	MD_U8*  opl  = ptr + in.size;
	MD_U64  size = 0;
	MD_UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = md_utf8_decode(ptr, opl - ptr);
		size   += md_utf16_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	md_arena_pop(arena, (cap - size)*2);
	return(md_str16(str, size));
}

MD_String8
md_str8_from_str32__arena(MD_Arena* arena, MD_String32 in){
	MD_U64  cap  = in.size * 4;
	MD_U8*  str  = md_push_array__no_zero(arena, MD_U8, cap + 1);
	MD_U32* ptr  = in.str;
	MD_U32* opl  = ptr + in.size;
	MD_U64  size = 0;
	for (;ptr < opl; ptr += 1){
		size += md_utf8_encode(str + size, *ptr);
	}
	str[size] = 0;
	md_arena_pop(arena, (cap - size));
	return(md_str8(str, size));
}

MD_String32
md_str32_from_str8__arena(MD_Arena* arena, MD_String8 in){
	MD_U64  cap  = in.size;
	MD_U32* str  = md_push_array__no_zero(arena, MD_U32, cap + 1);
	MD_U8*  ptr  = in.str;
	MD_U8*  opl  = ptr + in.size;
	MD_U64  size = 0;
	MD_UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc){
		consume   = md_utf8_decode(ptr, opl - ptr);
		str[size] = consume.codepoint;
		size     += 1;
	}
	str[size] = 0;
	md_arena_pop(arena, (cap - size)*4);
	return(str32(str, size));
}

MD_String8
md_str8_from_str16__ainfo(MD_AllocatorInfo ainfo, MD_String16 in) {
	MD_U64  cap  = in.size * 3;
	MD_U8*  str  = md_alloc_array_no_zero(ainfo, MD_U8, cap + 1);
	MD_U16* ptr  = in.str;
	MD_U16* opl  = ptr + in.size;
	MD_U64  size = 0;
	MD_UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = md_utf16_decode(ptr, opl - ptr);
		size   += md_utf8_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	if (md_allocator_query_support(ainfo) & MD_AllocatorQuery_ResizeShrink) {
		md_resize(ainfo, str, cap + 1, (cap - size));
	}
	return(md_str8(str, size));
}

MD_String16
md_str16_from_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8 in) {
	MD_U64  cap  = in.size * 2;
	MD_U16* str  = md_alloc_array_no_zero(ainfo, MD_U16, cap + 1);
	MD_U8*  ptr  = in.str;
	MD_U8*  opl  = ptr + in.size;
	MD_U64  size = 0;
	MD_UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc) {
		consume = md_utf8_decode(ptr, opl - ptr);
		size   += md_utf16_encode(str + size, consume.codepoint);
	}
	str[size] = 0;
	if (md_allocator_query_support(ainfo) & MD_AllocatorQuery_ResizeShrink) {
		md_resize(ainfo, str, cap + 1, (cap - size));
	}
	return(md_str16(str, size));
}

MD_String8
md_str8_from_str32__ainfo(MD_AllocatorInfo ainfo, MD_String32 in){
	MD_U64  cap  = in.size * 4;
	MD_U8*  str  = md_alloc_array_no_zero(ainfo, MD_U8, cap + 1);
	MD_U32* ptr  = in.str;
	MD_U32* opl  = ptr + in.size;
	MD_U64  size = 0;
	for (;ptr < opl; ptr += 1){
		size += md_utf8_encode(str + size, *ptr);
	}
	str[size] = 0;
	if (md_allocator_query_support(ainfo) & MD_AllocatorQuery_ResizeShrink) {
		md_resize(ainfo, str, cap + 1, (cap - size));
	}
	return(md_str8(str, size));
}

MD_String32
md_str32_from_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8 in){
	MD_U64  cap  = in.size;
	MD_U32* str  = md_alloc_array_no_zero(ainfo, MD_U32, cap + 1);
	MD_U8*  ptr  = in.str;
	MD_U8*  opl  = ptr + in.size;
	MD_U64  size = 0;
	MD_UnicodeDecode consume;
	for (;ptr < opl; ptr += consume.inc){
		consume   = md_utf8_decode(ptr, opl - ptr);
		str[size] = consume.codepoint;
		size     += 1;
	}
	str[size] = 0;
	if (md_allocator_query_support(ainfo) & MD_AllocatorQuery_ResizeShrink) {
		md_resize(ainfo, str, cap + 1, (cap - size));
	}
	return(str32(str, size));
}

////////////////////////////////
//~ String -> Enum Conversions

typedef struct MD_OS_EnumMap MD_OS_EnumMap;
struct MD_OS_EnumMap
{
	MD_String8         string;
	MD_OperatingSystem os;
};

md_read_only md_global MD_OS_EnumMap g_os_enum_map[] =
{
	{ md_str8_lit_comp(""),        MD_OperatingSystem_Null     },
	{ md_str8_lit_comp("Windows"), MD_OperatingSystem_Windows, },
	{ md_str8_lit_comp("Linux"),   MD_OperatingSystem_Linux,   },
	{ md_str8_lit_comp("Mac"),     MD_OperatingSystem_Mac,     },
};
md_static_assert(md_array_count(g_os_enum_map) == MD_OperatingSystem_COUNT, g_os_enum_map_count_check);

MD_OperatingSystem
md_operating_system_from_string(MD_String8 string)
{
	for (MD_U64 i = 0; i < md_array_count(g_os_enum_map); ++i) {
		if(md_str8_match(g_os_enum_map[i].string, string, MD_StringMatchFlag_CaseInsensitive)) {
			return g_os_enum_map[i].os;
		}
	}
	return MD_OperatingSystem_Null;
}

////////////////////////////////
//~ rjf: Time Types -> String

MD_String8
md_date_time_string__ainfo(MD_AllocatorInfo ainfo, MD_DateTime* date_time) {
	char* mon_str = (char*)md_string_from_month(date_time->month).str;

	MD_U32 adjusted_hour = date_time->hour % 12;
	if (adjusted_hour == 0){
		adjusted_hour = 12;
	}

	char* ampm = "am";
	if (date_time->hour >= 12){
		ampm = "pm";
	}

	MD_String8 result = md_str8f(ainfo, 
		"%d %s %d, %02d:%02d:%02d %s",
		date_time->day, mon_str, date_time->year,
		adjusted_hour, date_time->md_min, date_time->sec, ampm
	);
	return(result);
}

MD_String8
md_file_name_date_time_string__ainfo(MD_AllocatorInfo ainfo, MD_DateTime* date_time) {
	char* mon_str = (char*)md_string_from_month(date_time->month).str;

	MD_String8 result = md_str8f(ainfo, 
		"%d-%s-%0d--%02d-%02d-%02d",
		date_time->year, mon_str, date_time->day,
		date_time->hour, date_time->md_min, date_time->sec
	);
	return(result);
}

MD_String8
md_string_from_elapsed_time__ainfo(MD_AllocatorInfo ainfo, MD_DateTime dt) {
	MD_TempArena scratch = md_scratch_begin(ainfo);

	MD_String8List list = {0};
	if (dt.year) {
		md_str8_list_pushf(scratch.arena, &list, "%dy", dt.year);
		md_str8_list_pushf(scratch.arena, &list, "%um", dt.mon);
		md_str8_list_pushf(scratch.arena, &list, "%ud", dt.day);
	} else if (dt.mon) {
		md_str8_list_pushf(scratch.arena, &list, "%um", dt.mon);
		md_str8_list_pushf(scratch.arena, &list, "%ud", dt.day);
	} else if (dt.day) {
		md_str8_list_pushf(scratch.arena, &list, "%ud", dt.day);
	}
	md_str8_list_pushf(scratch.arena, &list, "%u:%u:%u:%u ms", dt.hour, dt.md_min, dt.sec, dt.msec);

	MD_StringJoin join   = { md_str8_lit_comp(""), md_str8_lit_comp(" "), md_str8_lit_comp("") };
	MD_String8    result = md_str8_list_join(ainfo, &list, &join);
	return(result);
}

////////////////////////////////
//~ Globally Unique Ids

MD_B32
md_try_guid_from_string(MD_String8 string, MD_Guid* md_guid_out)
{
	MD_TempArena scratch = md_scratch_begin(0,0);
	MD_B32 is_parsed = 0;

	MD_String8List list = md_str8_split_by_string_chars(scratch.arena, string, md_str8_lit("-"), MD_StringSplitFlag_KeepEmpties);
	if(list.node_count == 5)
	{
		MD_String8 data1_str    = list.first->string;
		MD_String8 data2_str    = list.first->next->string;
		MD_String8 data3_str    = list.first->next->next->string;
		MD_String8 data4_hi_str = list.first->next->next->next->string;
		MD_String8 data4_lo_str = list.first->next->next->next->next->string;
		if( md_str8_is_integer(data1_str,    16) && 
			md_str8_is_integer(data2_str,    16) &&
			md_str8_is_integer(data3_str,    16) &&
			md_str8_is_integer(data4_hi_str, 16) &&
			md_str8_is_integer(data4_lo_str, 16)   )
		{
			MD_U64 data1    = md_u64_from_str8(data1_str,    16);
			MD_U64 data2    = md_u64_from_str8(data2_str,    16);
			MD_U64 data3    = md_u64_from_str8(data3_str,    16);
			MD_U64 data4_hi = md_u64_from_str8(data4_hi_str, 16);
			MD_U64 data4_lo = md_u64_from_str8(data4_lo_str, 16);
			if( data1    <= MD_MAX_U32 &&
				data2    <= MD_MAX_U16 &&
				data3    <= MD_MAX_U16 &&
				data4_hi <= MD_MAX_U16 &&
				data4_lo <= 0xffffffffffff )
			{
				md_guid_out->data1 = (MD_U32)data1;
				md_guid_out->data2 = (MD_U16)data2;
				md_guid_out->data3 = (MD_U16)data3;

				MD_U64 data4 = (data4_hi << 48) | data4_lo;
				md_memory_copy(&md_guid_out->data4[0], &data4, sizeof(data4));
				is_parsed = 1;
			}
		}
	}
	scratch_end(scratch);
	return is_parsed;
}

////////////////////////////////
//~ rjf: Basic Text Indentation

MD_String8
md_indented_from_string__ainfo(MD_AllocatorInfo ainfo, MD_String8 string)
{
	MD_TempArena scratch = md_scratch_begin(ainfo);

	md_local_persist MD_U8 indentation_bytes[] = "                                                                                                                                ";
	MD_String8List indented_strings = {0};

	MD_S64 depth          = 0;
	MD_S64 next_depth     = 0;
	MD_U64 line_begin_off = 0;
	for(MD_U64 off = 0; off <= string.size; off += 1)
	{
		MD_U8 byte = off<string.size ? string.str[off] : 0;
		switch(byte)
		{
			default:{}break;
			case '{':case '[':case '(':{next_depth += 1; next_depth = md_max(0, next_depth);}break;
			case '}':case ']':case ')':{next_depth -= 1; next_depth = md_max(0, next_depth); depth = next_depth;}break;
			case '\n':
			case 0:
			{
				MD_String8 line = md_str8_skip_chop_whitespace(md_str8_substr(string, md_r1u64(line_begin_off, off)));
				if(line.size != 0) {
					md_str8_list_pushf(scratch.arena, &indented_strings, "%.*s%S\n", (int)depth * 2, indentation_bytes, line);
				}
				line_begin_off = off + 1;
				depth          = next_depth;
			}break;
		}
	}

	MD_String8 result = md_str8_list_join(ainfo, &indented_strings, 0);
	scratch_end(scratch);
	return result;
}

////////////////////////////////
//~ rjf: Text Escaping

MD_String8
md_escaped_from_raw_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8 string)
{
	MD_TempArena scratch = md_scratch_begin(ainfo);

	MD_String8List parts   = {0};
	MD_U64 start_split_idx = 0;
	for(MD_U64 idx = 0; idx <= string.size; idx += 1)
	{
		MD_U8  byte  = (idx < string.size) ? string.str[idx] : 0;
		MD_B32 split = 1;
		MD_String8 separator_replace = {0};
		switch (byte)
		{
			default: { split = 0; } break;

			case 0:    {} break;
			case '\a': { separator_replace = md_str8_lit("\\a" ); } break;
			case '\b': { separator_replace = md_str8_lit("\\b" ); } break;
			case '\f': { separator_replace = md_str8_lit("\\f" ); } break;
			case '\n': { separator_replace = md_str8_lit("\\n" ); } break;
			case '\r': { separator_replace = md_str8_lit("\\r" ); } break;
			case '\t': { separator_replace = md_str8_lit("\\t" ); } break;
			case '\v': { separator_replace = md_str8_lit("\\v" ); } break;
			case '\\': { separator_replace = md_str8_lit("\\\\"); } break;
			case '"':  { separator_replace = md_str8_lit("\\\""); } break;
			case '?':  { separator_replace = md_str8_lit("\\?" ); } break;
		}
		if (split)
		{
			MD_String8 substr  = md_str8_substr(string, md_r1u64(start_split_idx, idx));
			start_split_idx = idx + 1;
			md_str8_list_push(scratch.arena, &parts, substr);
			if(separator_replace.size != 0) {
				md_str8_list_push(scratch.arena, &parts, separator_replace);
			}
		}
	}

	MD_StringJoin join   = {0};
	MD_String8    result = md_str8_list_join(ainfo, &parts, &join);

	scratch_end(scratch);
	return result;
}

MD_String8
md_raw_from__escaped_str8__ainfo(MD_AllocatorInfo ainfo, MD_String8 string)
{
	MD_TempArena scratch = md_scratch_begin(ainfo);

	MD_String8List strs  = {0};
	MD_U64         start = 0;
	for (MD_U64 idx = 0; idx <= string.size; idx += 1)
	{
		if (idx == string.size || string.str[idx] == '\\' || string.str[idx] == '\r') {
			MD_String8 str = md_str8_substr(string, md_r1u64(start, idx));
			if (str.size != 0) {
				md_str8_list_push(scratch.arena, &strs, str);
			}
			start = idx + 1;
		}
		if (idx < string.size && string.str[idx] == '\\') {
			MD_U8 next_char    = string.str[idx+1];
			MD_U8 replace_byte = 0;
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

			MD_String8 replace_string = md_str8_copy(scratch.arena, md_str8(&replace_byte, 1));
			md_str8_list_push(scratch.arena, &strs, replace_string);
			idx += 1;
			start += 1;
		}
	}

	MD_String8 result = md_str8_list_join(ainfo, &strs, 0);
	scratch_end(scratch);
	return result;
}

////////////////////////////////
//~ rjf: Text Wrapping

MD_String8List
md_wrapped_lines_from_string__ainfo(MD_AllocatorInfo ainfo, MD_String8 string, MD_U64 first_line_max_width, MD_U64 max_width, MD_U64 wrap_indent)
{
	MD_String8List list       = {0};
	MD_Rng1U64     line_range = md_r1u64(0, 0);

	MD_U64 wrapped_indent_level = 0;
	static char* spaces = "                                                                ";

	for (MD_U64 idx = 0; idx <= string.size; idx += 1)
	{
		MD_U8 chr = idx < string.size ? string.str[idx] : 0;
		if (chr == '\n')
		{
			MD_Rng1U64
			candidate_line_range     = line_range;
			candidate_line_range.md_max = idx;
			// NOTE(nick): when wrapping is interrupted with \n we emit a string without including \n
			// because later tool_fprint_list inserts separator after each node
			// except for last node, so don't strip last \n.
			if (idx + 1 == string.size){
				candidate_line_range.md_max += 1;
			}
			MD_String8 substr = md_str8_substr(string, candidate_line_range);
			md_str8_list_push(ainfo, &list, substr);
			line_range = md_r1u64(idx + 1,idx + 1);
		}
		else
		if (md_char_is_space(chr) || chr == 0)
		{
			MD_Rng1U64 
			candidate_line_range     = line_range;
			candidate_line_range.md_max = idx;

			MD_String8 substr          = md_str8_substr(string, candidate_line_range);
			MD_U64     width_this_line = max_width-wrapped_indent_level;

			if (list.node_count == 0) {
				width_this_line = first_line_max_width;
			}
			if (substr.size > width_this_line) 
			{
				MD_String8 line = md_str8_substr(string, line_range);
				if (wrapped_indent_level > 0){
					line = md_str8f(ainfo, "%.*s%S", wrapped_indent_level, spaces, line);
				}
				md_str8_list_push(ainfo, &list, line);
				line_range           = md_r1u64(line_range.md_max + 1, candidate_line_range.md_max);
				wrapped_indent_level = md_clamp_top(64, wrap_indent);
			}
			else{
				line_range = candidate_line_range;
			}
		}
	}
	if (line_range.md_min < string.size && line_range.md_max > line_range.md_min) {
		MD_String8 line = md_str8_substr(string, line_range);
		if (wrapped_indent_level > 0) {
			line = md_str8f(ainfo, "%.*s%S", wrapped_indent_level, spaces, line);
		}
		md_str8_list_push(ainfo, &list, line);
	}
	return list;
}

////////////////////////////////
//~ rjf: String <-> Color

MD_Vec4F32
md_rgba_from_hex_string_4f32(MD_String8 hex_string)
{
	MD_U8  byte_text[8]  = {0};
	MD_U64 byte_text_idx = 0;
	for(MD_U64 idx = 0; idx < hex_string.size && byte_text_idx < md_array_count(byte_text); idx += 1)
	{
		if(md_char_is_digit(hex_string.str[idx], 16)) {
			byte_text[byte_text_idx] = md_char_to_lower(hex_string.str[idx]);
			byte_text_idx           += 1;
		}
	}
	MD_U8 byte_vals[4] = {0};
	for(MD_U64 idx = 0; idx < 4; idx += 1) {
		byte_vals[idx] = (MD_U8)md_u64_from_str8(md_str8(&byte_text[idx*2], 2), 16);
	}
	MD_Vec4F32 rgba = md_v4f32(byte_vals[0] / 255.f, byte_vals[1] / 255.f, byte_vals[2] / 255.f, byte_vals[3] / 255.f);
	return  rgba;
}

////////////////////////////////
//~ rjf: String Fuzzy Matching

MD_FuzzyMatchRangeList
md_fuzzy_match_find__ainfo(MD_AllocatorInfo ainfo, MD_String8 needle, MD_String8 haystack)
{
	MD_TempArena   scratch = md_scratch_begin(ainfo);
	MD_String8List needles = md_str8_split(scratch.arena, needle, (MD_U8*)" ", 1, 0);
	MD_FuzzyMatchRangeList 
	result = {0};
	result.needle_part_count = needles.node_count;
	for(MD_String8Node* needle_n = needles.first; needle_n != 0; needle_n = needle_n->next)
	{
		MD_U64 find_pos = 0;
		for(;find_pos < haystack.size;)
		{
			find_pos = md_str8_find_needle(haystack, find_pos, needle_n->string, MD_StringMatchFlag_CaseInsensitive);
			MD_B32 is_in_gathered_ranges = 0;
			for (MD_FuzzyMatchRangeNode* n = result.first; n != 0; n = n->next)
			{
				if (n->range.md_min <= find_pos && find_pos < n->range.md_max) {
					is_in_gathered_ranges = 1;
					find_pos              = n->range.md_max;
					break;
				}
			}
			if( ! is_in_gathered_ranges) {
				break;
			}
		}
		if (find_pos < haystack.size) {
			MD_Rng1U64             range = md_r1u64(find_pos, find_pos+needle_n->string.size);
			MD_FuzzyMatchRangeNode* n    = md_push_array(scratch.arena, MD_FuzzyMatchRangeNode, 1);
			n->range = range;
			md_sll_queue_push(result.first, result.last, n);
			result.count     += 1;
			result.total_dim += md_dim_1u64(range);
		}
	}
	md_temp_end(scratch);
	return result;
}

MD_FuzzyMatchRangeList
md_fuzzy_match_range_list_copy__ainfo(MD_AllocatorInfo ainfo, MD_FuzzyMatchRangeList* src)
{
	MD_FuzzyMatchRangeList dst = {0};
	for(MD_FuzzyMatchRangeNode* src_n = src->first; src_n != 0; src_n = src_n->next)
	{
		MD_FuzzyMatchRangeNode* dst_n = md_alloc_array(ainfo, MD_FuzzyMatchRangeNode, 1);
		md_sll_queue_push(dst.first, dst.last, dst_n);
		dst_n->range = src_n->range;
	}
	dst.count             = src->count;
	dst.needle_part_count = src->needle_part_count;
	dst.total_dim         = src->total_dim;
	return dst;
}

////////////////////////////////
//~ NOTE(allen): Serialization Helpers

MD_U64
md_str8_serial_push_align__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U64 align) {
	md_assert(md_is_pow2(align));
	MD_U64 pos     = srl->total_size;
	MD_U64 new_pos = md_align_pow2(pos, align);
	MD_U64 size    = (new_pos - pos);
	
	if(size != 0)
	{
		MD_U8* buf = md_alloc_array(ainfo, MD_U8, size);
		
		MD_String8* str = &srl->last->string;
		if (str->str + str->size == buf) {
			srl->last->string.size += size;
			srl->total_size        += size;
		}
		else {
			md_str8_list_push(ainfo, srl, md_str8(buf, size));
		}
	}
	return size;
}

void*
md_str8_serial_push_size__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U64 size) {
	void* result = 0;
	if(size != 0)
	{
		MD_U8*      buf = md_alloc_array_no_zero(ainfo, MD_U8, size);
		MD_String8* str = &srl->last->string;
		if (str->str + str->size == buf) {
			srl->last->string.size += size;
			srl->total_size += size;
		}
		else {
			md_str8_list_push(ainfo, srl, md_str8(buf, size));
		}
		result = buf;
	}
	return result;
}

void
md_str8_serial_alloc_u64__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U64 x) {
	MD_U8* buf = md_alloc_array_no_zero(ainfo, MD_U8, 8);
	md_memory_copy(buf, &x, 8);
	MD_String8* str = &srl->last->string;
	if (str->str + str->size == buf) {
		srl->last->string.size += 8;
		srl->total_size        += 8;
	}
	else {
		md_str8_list_push(ainfo, srl, md_str8(buf, 8));
	}
}

void
md_str8_serial_alloc_u32__ainfo(MD_AllocatorInfo ainfo, MD_String8List* srl, MD_U32 x) {
	MD_U8* buf = md_alloc_array_no_zero(ainfo, MD_U8, 4);
	md_memory_copy(buf, &x, 4);
	MD_String8 *str = &srl->last->string;
	if (str->str + str->size == buf) {
		srl->last->string.size += 4;
		srl->total_size += 4;
	}
	else {
		md_str8_list_push(ainfo, srl, md_str8(buf, 4));
	}
}

////////////////////////////////
//~ rjf: Deserialization Helpers

MD_U64
md_str8_deserial_read(MD_String8 string, MD_U64 off, void *read_dst, MD_U64 read_size, MD_U64 granularity) {
	MD_U64 bytes_left             = string.size - md_min(off, string.size);
	MD_U64 actually_readable_size = md_min(bytes_left, read_size);
	MD_U64 legally_readable_size  = actually_readable_size - actually_readable_size % granularity;
	if(legally_readable_size > 0)
	{
		md_memory_copy(read_dst, string.str + off, legally_readable_size);
	}
	return legally_readable_size;
}

MD_U64
md_str8_deserial_find_first_match(MD_String8 string, MD_U64 off, MD_U16 scan_val) {
	MD_U64 cursor = off;
	for (;;) 
	{
		MD_U16 val = 0;
		md_str8_deserial_read_struct(string, cursor, &val);
		if (val == scan_val) {
			break;
		}
		cursor += sizeof(val);
	}
	return cursor;
}

MD_U64
md_str8_deserial_read_cstr(MD_String8 string, MD_U64 off, MD_String8* cstr_out) {
	MD_U64 cstr_size = 0;
	if (off < string.size) {
		MD_U8* ptr  = string.str + off;
		MD_U8* cap  = string.str + string.size;
		*cstr_out   = md_str8_cstring_capped(ptr, cap);
		 cstr_size  = (cstr_out->size + 1);
	}
	return cstr_size;
}

MD_U64
md_str8_deserial_read_windows_utf16_string16(MD_String8 string, MD_U64 off, MD_String16* md_str_out)
{
	MD_U64 null_off = md_str8_deserial_find_first_match(string, off, 0);
	MD_U64 size     = null_off - off;
	MD_U16 *str     = (MD_U16*)md_str8_deserial_get_raw_ptr(string, off, size);
	MD_U64 count    = size / sizeof(*str);
	
	*md_str_out = md_str16(str, count);
	
	MD_U64    read_size_with_null = size + sizeof(*str);
	return read_size_with_null;
}

MD_U64
md_str8_deserial_read_uleb128(MD_String8 string, MD_U64 off, MD_U64 *value_out) {
	MD_U64 value  = 0;
	MD_U64 shift  = 0;
	MD_U64 cursor = off;
	for(;;)
	{
		MD_U8  byte       = 0;
		MD_U64 bytes_read = md_str8_deserial_read_struct(string, cursor, &byte);
		if (bytes_read != sizeof(byte)) { 
			break;
		}

		MD_U8 val = byte & 0x7fu;
		value |= ((MD_U64)val) << shift;

		cursor += bytes_read;
		shift  += 7u;
		if ((byte & 0x80u) == 0) {
			break;
		}
	}
	if(value_out != 0) { *value_out = value; }

	MD_U64    bytes_read = cursor - off;
	return bytes_read;
}

MD_U64
md_str8_deserial_read_sleb128(MD_String8 string, MD_U64 off, MD_S64 *value_out)
{
	MD_U64 value  = 0;
	MD_U64 shift  = 0;
	MD_U64 cursor = off;
	for(;;)
	{
		MD_U8  byte;
		MD_U64 bytes_read = md_str8_deserial_read_struct(string, cursor, &byte);
		if (bytes_read != sizeof(byte)) {
			break;
		}

		MD_U8 val = byte & 0x7fu;
		value |= ((MD_U64)val) << shift;

		cursor += bytes_read;
		shift  += 7u;

		if ((byte & 0x80u) == 0)
		{
			if (shift < sizeof(value) * 8 && (byte & 0x40u) != 0) {
				value |= -(MD_S64)(1ull << shift);
			}
			break;
		}
	}
	if (value_out != 0) { *value_out = value; }

	MD_U64    bytes_read = cursor - off;
	return bytes_read;
}
