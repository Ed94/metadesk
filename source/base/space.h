#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ rjf: Basic Types & Spaces

typedef enum MD_Dimension MD_Dimension;
enum MD_Dimension
{
	MD_Dimension_X,
	MD_Dimension_Y,
	MD_Dimension_Z,
	MD_Dimension_W,
};

typedef enum MD_Side MD_Side;
enum MD_Side
{
	MD_Side_Invalid = -1,
	MD_Side_Min,
	MD_Side_Max,
	MD_Side_COUNT,
};
#define side_flip(s) ((MD_Side)(!(s)))

typedef enum MD_Axis2 MD_Axis2;
enum MD_Axis2
{
	MD_Axis2_Invalid = -1,
	MD_Axis2_X,
	MD_Axis2_Y,
	MD_Axis2_COUNT,
};
#define axis2_flip(a) ((MD_Axis2)(!(a)))

typedef enum MD_corner MD_corner;
enum MD_corner
{
	MD_Corner_Invalid = -1,
	MD_Corner_00,
	MD_Corner_01,
	MD_Corner_10,
	MD_Corner_11,
	MD_Corner_COUNT
};

typedef enum MD_Dir2 MD_Dir2;
enum MD_Dir2
{
	MD_Dir2_Invalid = -1,
	MD_Dir2_Left,
	MD_Dir2_Up,
	MD_Dir2_Right,
	MD_Dir2_Down,
	MD_Dir2_COUNT
};

#define md_axis2_from_dir2(d) (((d) & 1)             ? MD_Axis2_Y  : MD_Axis2_X)
#define md_side_from_dir2(d)  (((d) < MD_Dir2_Right) ? MD_Side_Min : MD_Side_Max)

////////////////////////////////
//~ rjf: Enum -> Sign

inline MD_S32 md_sign_from_side_S32(MD_Side side) { return((side == MD_Side_Min) ? -1   : 1  ); }
inline MD_F32 md_sign_from_side_F32(MD_Side side) { return((side == MD_Side_Min) ? -1.f : 1.f); }
