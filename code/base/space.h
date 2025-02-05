#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#endif

////////////////////////////////
//~ rjf: Basic Types & Spaces

typedef enum Dimension
{
  Dimension_X,
  Dimension_Y,
  Dimension_Z,
  Dimension_W,
}
Dimension;

typedef enum Side
{
  Side_Invalid = -1,
  Side_Min,
  Side_Max,
  Side_COUNT,
}
Side;
#define side_flip(s) ((Side)(!(s)))

typedef enum Axis2
{
  Axis2_Invalid = -1,
  Axis2_X,
  Axis2_Y,
  Axis2_COUNT,
}
Axis2;
#define axis2_flip(a) ((Axis2)(!(a)))

typedef enum Corner
{
  Corner_Invalid = -1,
  Corner_00,
  Corner_01,
  Corner_10,
  Corner_11,
  Corner_COUNT
}
Corner;

typedef enum Dir2
{
  Dir2_Invalid = -1,
  Dir2_Left,
  Dir2_Up,
  Dir2_Right,
  Dir2_Down,
  Dir2_COUNT
}
Dir2;

#define axis2_from_dir2(d) (((d) & 1) ? Axis2_Y : Axis2_X)
#define side_from_dir2(d)  (((d) < Dir2_Right) ? Side_Min : Side_Max)
