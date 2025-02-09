#ifdef INTELLISENSE_DIRECTIVES
#	include "os.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Filesystem Helpers (Helpers, Implemented Once)

B32
os_write_data_to_file_path(String8 path, String8 data)
{
	B32       good = 0;
	OS_Handle file = os_file_open(OS_AccessFlag_Write, path);
	if(! os_handle_match(file, os_handle_zero()))
	{
		good = 1;
		os_file_write(file, r1u64(0, data.size), data.str);
		os_file_close(file);
	}
	return good;
}

B32
os_write_data_list_to_file_path(String8 path, String8List list)
{
	B32       good = 0;
	OS_Handle file = os_file_open(OS_AccessFlag_Write, path);
	if( ! os_handle_match(file, os_handle_zero()))
	{
		    good = 1;
		U64 off  = 0;
		for(String8Node* n = list.first; n != 0; n = n->next)
		{
			os_file_write(file, r1u64(off, off+n->string.size), n->string.str);
			off += n->string.size;
		}
		os_file_close(file);
	}
	return good;
}

B32
os_append_data_to_file_path(String8 path, String8 data)
{
	B32 good = 0;
	if(data.size != 0)
	{
		OS_Handle file = os_file_open(OS_AccessFlag_Write|OS_AccessFlag_Append, path);
		if( ! os_handle_match(file, os_handle_zero()))
		{
			good = 1;
			U64 pos = os_properties_from_file(file).size;
			os_file_write(file, r1u64(pos, pos+data.size), data.str);
			os_file_close(file);
		}
	}
	return good;
}

String8
os_string_from_file_range(Arena* arena, OS_Handle file, Rng1U64 range)
{
	U64 pre_pos = arena_pos(arena);
	String8 result;
	result.size = dim_1u64(range);
	result.str = push_array_no_zero(arena, U8, result.size);
	U64 actual_read_size = os_file_read(file, range, result.str);
	if(actual_read_size < result.size)
	{
		arena_pop_to(arena, pre_pos + actual_read_size);
		result.size = actual_read_size;
	}
	return result;
}

String8
os_string_from_file_range_alloc(AllocatorInfo ainfo, OS_Handle file, Rng1U64 range) {
	String8 result;
	result.size = dim_1u64(range);
	result.str  = alloc_array_no_zero(ainfo, U8, result.size);
	U64 actual_read_size = os_file_read(file, range, result.str);
	if(actual_read_size < result.size)
	{
		// TODO(Ed): It may be better to actually wrap the alloation in an arena and then rewind it.
		// This would ensure resize isn't doing an expensive shrink (from a bad heap realloc, or something else)
		// That or we just leave it up to the user to make sure to pass in an arena.
		resize(ainfo, result.str, result.size, scast(U64, result.str) + actual_read_size);
		result.size = actual_read_size;
	}
	return result;
}
