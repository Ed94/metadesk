#ifdef INTELLISENSE_DIRECTIVES
#	include "text.h"
#endif

////////////////////////////////
//~ rjf: Text Path Helpers

MD_String8TxtPtPair
md_str8_txt_pt_pair_from_string(MD_String8 string)
{
	MD_String8TxtPtPair pair = {0};
	{
		MD_String8 file_part = {0};
		MD_String8 line_part = {0};
		MD_String8 col_part  = {0};
    
		// rjf: grab file part
		for(MD_U64 idx = 0; idx <= string.size; idx += 1)
		{
			MD_U8 byte      =  (idx     < string.size) ? (string.str[idx    ]) : 0;
			MD_U8 next_byte = ((idx + 1 < string.size) ? (string.str[idx + 1]) : 0);
			if(byte == ':' && next_byte != '/' && next_byte != '\\') {
				file_part = md_str8_prefix(string, idx);
				line_part = md_str8_skip(string, idx+1);
				break;
			}
			else if(byte == 0) {
				file_part = string;
				break;
			}
		}
		// rjf: grab line/column
		{
			MD_U64 colon_pos = md_str8_find_needle(line_part, 0, md_str8_lit(":"), 0);
			if(colon_pos < line_part.size) {
				col_part  = md_str8_skip  (line_part, colon_pos + 1);
				line_part = md_str8_prefix(line_part, colon_pos);
			}
		}
    
		// rjf: convert line/column strings to numerics
		MD_U64 line   = 0;
		MD_U64 column = 0;
		md_try_u64_from_str8_c_rules(line_part, &line);
		md_try_u64_from_str8_c_rules(col_part,  &column);
    
		// rjf: fill
		pair.string = file_part;
		pair.pt     = md_txt_pt((MD_S64)line, (MD_S64)column);
		if(pair.pt.line   == 0) { pair.pt.line   = 1; }
		if(pair.pt.column == 0) { pair.pt.column = 1; }
	}
	return pair;
}
