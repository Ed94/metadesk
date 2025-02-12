#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "strings.h"
#endif

////////////////////////////////
//~ rjf: Text 2D Coordinates & Ranges

typedef struct MD_TxtPt MD_TxtPt;
struct MD_TxtPt
{
	MD_S64 line;
	MD_S64 column;
};

typedef struct MD_TxtRng MD_TxtRng;
struct MD_TxtRng
{
	MD_TxtPt md_min;
	MD_TxtPt md_max;
};

////////////////////////////////
//~ rjf: String Pair Types

typedef struct MD_String8TxtPtPair MD_String8TxtPtPair;
struct MD_String8TxtPtPair
{
	MD_String8 string;
	MD_TxtPt   pt;
};

////////////////////////////////
//~ rjf: Text Path Helpers

MD_API MD_String8TxtPtPair md_str8_txt_pt_pair_from_string(MD_String8 string);

////////////////////////////////
//~ rjf: Text 2D Coordinate/Range Functions

inline MD_TxtPt md_txt_pt(MD_S64 line, MD_S64 column) { MD_TxtPt p  = { line, column }; return p; }

inline MD_B32   md_txt_pt_match(MD_TxtPt a, MD_TxtPt b) { return a.line == b.line && a.column == b.column; }
inline MD_TxtPt md_txt_pt_min  (MD_TxtPt a, MD_TxtPt b) { MD_TxtPt result = b; if (md_txt_pt_less_than(a, b)) { result = a; } return result; }
inline MD_TxtPt md_txt_pt_max  (MD_TxtPt a, MD_TxtPt b) { MD_TxtPt result = a; if (md_txt_pt_less_than(a, b)) { result = b; } return result; }

MD_B32    md_txt_pt_less_than (MD_TxtPt  a,   MD_TxtPt  b);
MD_TxtRng md_txt_rng          (MD_TxtPt  md_min, MD_TxtPt  md_max);
MD_TxtRng md_txt_rng_intersect(MD_TxtRng a,   MD_TxtRng b);
MD_TxtRng md_txt_rng_union    (MD_TxtRng a,   MD_TxtRng b);
MD_B32    md_txt_rng_contains (MD_TxtRng r,   MD_TxtPt  pt);

inline MD_B32
md_txt_pt_less_than(MD_TxtPt a, MD_TxtPt b)
{
	MD_B32 result = 0;
	if (a.line < b.line) {
		result = 1;
	}
	else if (a.line == b.line) {
		result = a.column < b.column;
	}
	return result;
}

inline MD_TxtRng
md_txt_rng(MD_TxtPt md_min, MD_TxtPt md_max)
{
	MD_TxtRng range = {0};
	if(md_txt_pt_less_than(md_min, md_max)) {
		range.md_min = md_min;
		range.md_max = md_max;
	}
	else {
		range.md_min = md_max;
		range.md_max = md_min;
	}
	return range;
}

inline MD_TxtRng
md_txt_rng_intersect(MD_TxtRng a, MD_TxtRng b)
{
	MD_TxtRng result = {0};
	result.md_min = md_txt_pt_max(a.md_min, b.md_min);
	result.md_max = md_txt_pt_min(a.md_max, b.md_max);
	if (md_txt_pt_less_than(result.md_max, result.md_min)) {
		MemoryZeroStruct(&result);
	}
	return result;
}

inline MD_TxtRng
md_txt_rng_union(MD_TxtRng a, MD_TxtRng b)
{
	MD_TxtRng result = {0};
	result.md_min = md_txt_pt_min(a.md_min, b.md_min);
	result.md_max = md_txt_pt_max(a.md_max, b.md_max);
	return result;
}

inline MD_B32
md_txt_rng_contains(MD_TxtRng r, MD_TxtPt pt) {
	MD_B32    result = ((md_txt_pt_less_than(r.md_min, pt) || md_txt_pt_match(r.md_min, pt)) && md_txt_pt_less_than(pt, r.md_max));
	return result;
}
