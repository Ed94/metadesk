#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "strings.h"
#endif

////////////////////////////////
//~ rjf: Text 2D Coordinates & Ranges

typedef struct TxtPt TxtPt;
struct TxtPt
{
	S64 line;
	S64 column;
};

typedef struct TxtRng TxtRng;
struct TxtRng
{
	TxtPt min;
	TxtPt max;
};

////////////////////////////////
//~ rjf: String Pair Types

typedef struct String8TxtPtPair String8TxtPtPair;
struct String8TxtPtPair
{
	String8 string;
	TxtPt   pt;
};

////////////////////////////////
//~ rjf: Text Path Helpers

MD_API String8TxtPtPair str8_txt_pt_pair_from_string(String8 string);

////////////////////////////////
//~ rjf: Text 2D Coordinate/Range Functions

inline TxtPt txt_pt(S64 line, S64 column) { TxtPt p  = { line, column }; return p; }

inline B32   txt_pt_match(TxtPt a, TxtPt b) { return a.line == b.line && a.column == b.column; }
inline TxtPt txt_pt_min  (TxtPt a, TxtPt b) { TxtPt result = b; if (txt_pt_less_than(a, b)) { result = a; } return result; }
inline TxtPt txt_pt_max  (TxtPt a, TxtPt b) { TxtPt result = a; if (txt_pt_less_than(a, b)) { result = b; } return result; }

B32    txt_pt_less_than (TxtPt  a,   TxtPt  b);
TxtRng txt_rng          (TxtPt  min, TxtPt  max);
TxtRng txt_rng_intersect(TxtRng a,   TxtRng b);
TxtRng txt_rng_union    (TxtRng a,   TxtRng b);
B32    txt_rng_contains (TxtRng r,   TxtPt  pt);

inline B32
txt_pt_less_than(TxtPt a, TxtPt b)
{
	B32 result = 0;
	if (a.line < b.line) {
		result = 1;
	}
	else if (a.line == b.line) {
		result = a.column < b.column;
	}
	return result;
}

inline TxtRng
txt_rng(TxtPt min, TxtPt max)
{
	TxtRng range = {0};
	if(txt_pt_less_than(min, max)) {
		range.min = min;
		range.max = max;
	}
	else {
		range.min = max;
		range.max = min;
	}
	return range;
}

inline TxtRng
txt_rng_intersect(TxtRng a, TxtRng b)
{
	TxtRng result = {0};
	result.min = txt_pt_max(a.min, b.min);
	result.max = txt_pt_min(a.max, b.max);
	if (txt_pt_less_than(result.max, result.min)) {
		MemoryZeroStruct(&result);
	}
	return result;
}

inline TxtRng
txt_rng_union(TxtRng a, TxtRng b)
{
	TxtRng result = {0};
	result.min = txt_pt_min(a.min, b.min);
	result.max = txt_pt_max(a.max, b.max);
	return result;
}

inline B32
txt_rng_contains(TxtRng r, TxtPt pt) {
	B32    result = ((txt_pt_less_than(r.min, pt) || txt_pt_match(r.min, pt)) && txt_pt_less_than(pt, r.max));
	return result;
}
