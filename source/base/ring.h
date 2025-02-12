#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Non-Fancy Ring Buffer Reads/Writes

U64 ring_write(U8* ring_base, U64 ring_size, U64 ring_pos, void* src_data, U64 src_data_size);
U64 ring_read (U8* ring_base, U64 ring_size, U64 ring_pos, void* dst_data, U64 read_size);

#define ring_write_struct(ring_base, ring_size, ring_pos, ptr) ring_write((ring_base), (ring_size), (ring_pos), (ptr), sizeof(*(ptr)))
#define ring_read_struct(ring_base, ring_size, ring_pos, ptr)  ring_read ((ring_base), (ring_size), (ring_pos), (ptr), sizeof(*(ptr)))

////////////////////////////////
//~ rjf: Non-Fancy Ring Buffer Reads/Writes

inline U64
ring_write(U8* ring_base, U64 ring_size, U64 ring_pos, void* src_data, U64 src_data_size) {
	assert(src_data_size <= ring_size);
	{
		U64   ring_off           = ring_pos % ring_size;
		U64   bytes_before_split = ring_size - ring_off;
		U64   pre_split_bytes    = min(bytes_before_split, src_data_size);
		U64   pst_split_bytes    = src_data_size - pre_split_bytes;
		void* pre_split_data     = src_data;
		void* pst_split_data     = ((U8*)src_data + pre_split_bytes);
		memory_copy(ring_base + ring_off, pre_split_data, pre_split_bytes);
		memory_copy(ring_base + 0, pst_split_data, pst_split_bytes);
	}
	return src_data_size;
}

inline U64
ring_read(U8* ring_base, U64 ring_size, U64 ring_pos, void* dst_data, U64 read_size) {
	assert(read_size <= ring_size);
	{
		U64 ring_off           = ring_pos % ring_size;
		U64 bytes_before_split = ring_size-ring_off;
		U64 pre_split_bytes    = min(bytes_before_split, read_size);
		U64 pst_split_bytes    = read_size - pre_split_bytes;
		memory_copy(dst_data, ring_base+ring_off, pre_split_bytes);
		memory_copy((U8*)dst_data + pre_split_bytes, ring_base + 0, pst_split_bytes);
	}
	return read_size;
}
