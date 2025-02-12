#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Non-Fancy Ring Buffer Reads/Writes

MD_U64 md_ring_write(MD_U8* md_ring_base, MD_U64 md_ring_size, MD_U64 md_ring_pos, void* src_data, MD_U64 src_data_size);
MD_U64 md_ring_read (MD_U8* md_ring_base, MD_U64 md_ring_size, MD_U64 md_ring_pos, void* dst_data, MD_U64 read_size);

#define md_ring_write_struct(md_ring_base, md_ring_size, md_ring_pos, ptr) md_ring_write((md_ring_base), (md_ring_size), (md_ring_pos), (ptr), sizeof(*(ptr)))
#define md_ring_read_struct(md_ring_base, md_ring_size, md_ring_pos, ptr)  md_ring_read ((md_ring_base), (md_ring_size), (md_ring_pos), (ptr), sizeof(*(ptr)))

////////////////////////////////
//~ rjf: Non-Fancy Ring Buffer Reads/Writes

inline MD_U64
md_ring_write(MD_U8* md_ring_base, MD_U64 md_ring_size, MD_U64 md_ring_pos, void* src_data, MD_U64 src_data_size) {
	md_assert(src_data_size <= md_ring_size);
	{
		MD_U64   md_ring_off        = md_ring_pos % md_ring_size;
		MD_U64   bytes_before_split = md_ring_size - md_ring_off;
		MD_U64   pre_split_bytes    = md_min(bytes_before_split, src_data_size);
		MD_U64   pst_split_bytes    = src_data_size - pre_split_bytes;
		void* pre_split_data        = src_data;
		void* pst_split_data        = ((MD_U8*)src_data + pre_split_bytes);
		md_memory_copy(md_ring_base + md_ring_off, pre_split_data, pre_split_bytes);
		md_memory_copy(md_ring_base + 0, pst_split_data, pst_split_bytes);
	}
	return src_data_size;
}

inline MD_U64
md_ring_read(MD_U8* md_ring_base, MD_U64 md_ring_size, MD_U64 md_ring_pos, void* dst_data, MD_U64 read_size) {
	md_assert(read_size <= md_ring_size);
	{
		MD_U64 md_ring_off        = md_ring_pos % md_ring_size;
		MD_U64 bytes_before_split = md_ring_size-md_ring_off;
		MD_U64 pre_split_bytes    = md_min(bytes_before_split, read_size);
		MD_U64 pst_split_bytes    = read_size - pre_split_bytes;
		md_memory_copy(dst_data, md_ring_base+md_ring_off, pre_split_bytes);
		md_memory_copy((MD_U8*)dst_data + pre_split_bytes, md_ring_base + 0, pst_split_bytes);
	}
	return read_size;
}
