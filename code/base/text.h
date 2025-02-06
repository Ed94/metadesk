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

TxtPt  txt_pt(S64 line, S64 column);
B32    txt_pt_match(TxtPt a, TxtPt b);
B32    txt_pt_less_than(TxtPt a, TxtPt b);
TxtPt  txt_pt_min(TxtPt a, TxtPt b);
TxtPt  txt_pt_max(TxtPt a, TxtPt b);
TxtRng txt_rng(TxtPt min, TxtPt max);
TxtRng txt_rng_intersect(TxtRng a, TxtRng b);
TxtRng txt_rng_union(TxtRng a, TxtRng b);
B32    txt_rng_contains(TxtRng r, TxtPt pt);

