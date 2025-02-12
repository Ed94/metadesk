
// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Filesystem Helpers (Helpers, Implemented Once)

MD_B32
md_os_write_data_to_file_path(MD_String8 path, MD_String8 data)
{
	MD_B32       good = 0;
	MD_OS_Handle file = md_os_file_open(MD_OS_AccessFlag_Write, path);
	if(! md_os_handle_match(file, md_os_handle_zero()))
	{
		good = 1;
		md_os_file_write(file, md_r1u64(0, data.size), data.str);
		md_os_file_close(file);
	}
	return good;
}

MD_B32
md_os_write_data_list_to_file_path(MD_String8 path, MD_String8List list)
{
	MD_B32       good = 0;
	MD_OS_Handle file = md_os_file_open(MD_OS_AccessFlag_Write, path);
	if( ! md_os_handle_match(file, md_os_handle_zero()))
	{
		    good = 1;
		MD_U64 off  = 0;
		for(MD_String8Node* n = list.first; n != 0; n = n->next)
		{
			md_os_file_write(file, md_r1u64(off, off+n->string.size), n->string.str);
			off += n->string.size;
		}
		md_os_file_close(file);
	}
	return good;
}

MD_B32
md_os_append_data_to_file_path(MD_String8 path, MD_String8 data)
{
	MD_B32 good = 0;
	if(data.size != 0)
	{
		MD_OS_Handle file = md_os_file_open(MD_OS_AccessFlag_Write|MD_OS_AccessFlag_Append, path);
		if( ! md_os_handle_match(file, md_os_handle_zero()))
		{
			good = 1;
			MD_U64 pos = md_os_properties_from_file(file).size;
			md_os_file_write(file, md_r1u64(pos, pos+data.size), data.str);
			md_os_file_close(file);
		}
	}
	return good;
}

MD_String8
md_os_string_from_file_range__arena(MD_Arena* arena, MD_OS_Handle file, MD_Rng1U64 range)
{
	MD_U64 pre_pos = md_arena_pos(arena);
	MD_String8 result;
	result.size = md_dim_1u64(range);
	result.str  = md_push_array__no_zero(arena, MD_U8, result.size);
	MD_U64 actual_read_size = md_os_file_read(file, range, result.str);
	if(actual_read_size < result.size) {
		md_arena_pop_to(arena, pre_pos + actual_read_size);
		result.size = actual_read_size;
	}
	return result;
}

MD_String8
md_os_string_from_file_range__ainfo(MD_AllocatorInfo ainfo, MD_OS_Handle file, MD_Rng1U64 range) {
	MD_String8 result;
	result.size = md_dim_1u64(range);
	result.str  = md_alloc_array_no_zero(ainfo, MD_U8, result.size);
	MD_U64 actual_read_size = md_os_file_read(file, range, result.str);
	if ((md_allocator_query_support(ainfo) & MD_AllocatorQuery_ResizeShrink) && actual_read_size < result.size) {
		md_resize(ainfo, result.str, result.size, actual_read_size);
		result.size = actual_read_size;
	}
	return result;
}
