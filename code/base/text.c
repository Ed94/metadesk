#ifdef INTELLISENSE_DIRECTIVES
#	include "text.h"
#endif

////////////////////////////////
//~ rjf: Text 2D Coordinate/Range Functions

internal TxtPt
txt_pt(S64 line, S64 column)
{
  TxtPt p = {0};
  p.line = line;
  p.column = column;
  return p;
}

internal B32
txt_pt_match(TxtPt a, TxtPt b)
{
  return a.line == b.line && a.column == b.column;
}

internal B32
txt_pt_less_than(TxtPt a, TxtPt b)
{
  B32 result = 0;
  if(a.line < b.line)
  {
    result = 1;
  }
  else if(a.line == b.line)
  {
    result = a.column < b.column;
  }
  return result;
}

internal TxtPt
txt_pt_min(TxtPt a, TxtPt b)
{
  TxtPt result = b;
  if(txt_pt_less_than(a, b))
  {
    result = a;
  }
  return result;
}

internal TxtPt
txt_pt_max(TxtPt a, TxtPt b)
{
  TxtPt result = a;
  if(txt_pt_less_than(a, b))
  {
    result = b;
  }
  return result;
}

internal TxtRng
txt_rng(TxtPt min, TxtPt max)
{
  TxtRng range = {0};
  if(txt_pt_less_than(min, max))
  {
    range.min = min;
    range.max = max;
  }
  else
  {
    range.min = max;
    range.max = min;
  }
  return range;
}

internal TxtRng
txt_rng_intersect(TxtRng a, TxtRng b)
{
  TxtRng result = {0};
  result.min = txt_pt_max(a.min, b.min);
  result.max = txt_pt_min(a.max, b.max);
  if(txt_pt_less_than(result.max, result.min))
  {
    MemoryZeroStruct(&result);
  }
  return result;
}

internal TxtRng
txt_rng_union(TxtRng a, TxtRng b)
{
  TxtRng result = {0};
  result.min = txt_pt_min(a.min, b.min);
  result.max = txt_pt_max(a.max, b.max);
  return result;
}

internal B32
txt_rng_contains(TxtRng r, TxtPt pt)
{
  B32 result = ((txt_pt_less_than(r.min, pt) || txt_pt_match(r.min, pt)) &&
                txt_pt_less_than(pt, r.max));
  return result;
}

////////////////////////////////
//~ rjf: Text Path Helpers

internal String8TxtPtPair
str8_txt_pt_pair_from_string(String8 string)
{
	String8TxtPtPair pair = {0};
	{
		String8 file_part = {0};
		String8 line_part = {0};
		String8 col_part  = {0};
    
		// rjf: grab file part
		for(U64 idx = 0; idx <= string.size; idx += 1)
		{
			U8 byte      =  (idx     < string.size) ? (string.str[idx    ]) : 0;
			U8 next_byte = ((idx + 1 < string.size) ? (string.str[idx + 1]) : 0);
			if(byte == ':' && next_byte != '/' && next_byte != '\\') {
				file_part = str8_prefix(string, idx);
				line_part = str8_skip(string, idx+1);
				break;
			}
			else if(byte == 0) {
				file_part = string;
				break;
			}
		}
		// rjf: grab line/column
		{
			U64 colon_pos = str8_find_needle(line_part, 0, str8_lit(":"), 0);
			if(colon_pos < line_part.size) {
				col_part  = str8_skip  (line_part, colon_pos + 1);
				line_part = str8_prefix(line_part, colon_pos);
			}
		}
    
		// rjf: convert line/column strings to numerics
		U64 line   = 0;
		U64 column = 0;
		try_u64_from_str8_c_rules(line_part, &line);
		try_u64_from_str8_c_rules(col_part,  &column);
    
		// rjf: fill
		pair.string = file_part;
		pair.pt     = txt_pt((S64)line, (S64)column);
		if(pair.pt.line   == 0) { pair.pt.line   = 1; }
		if(pair.pt.column == 0) { pair.pt.column = 1; }
	}
	return pair;
}
