#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Non-Fancy Ring Buffer Reads/Writes

internal U64 ring_write(U8 *ring_base, U64 ring_size, U64 ring_pos, void *src_data, U64 src_data_size);
internal U64 ring_read(U8 *ring_base, U64 ring_size, U64 ring_pos, void *dst_data, U64 read_size);
#define ring_write_struct(ring_base, ring_size, ring_pos, ptr) ring_write((ring_base), (ring_size), (ring_pos), (ptr), sizeof(*(ptr)))
#define ring_read_struct(ring_base, ring_size, ring_pos, ptr) ring_read((ring_base), (ring_size), (ring_pos), (ptr), sizeof(*(ptr)))
