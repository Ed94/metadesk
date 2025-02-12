#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "context_cracking.h"
#	include "linkage.h"
#	include "macros.h"
#	include "base_types.h"
#	include "memory.h"
#	include "memory_substrate.h"
#	include "arena.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Vector Types

//- rjf: 2-vectors

typedef union MD_Vec2F32 MD_Vec2F32;
union MD_Vec2F32
{
	struct
	{
		MD_F32 x;
		MD_F32 y;
	};
	MD_F32 v[2];
};

typedef union MD_Vec2S64 MD_Vec2S64;
union MD_Vec2S64
{
	struct
	{
		MD_S64 x;
		MD_S64 y;
	};
	MD_S64 v[2];
};

typedef union MD_Vec2S32 MD_Vec2S32;
union MD_Vec2S32
{
	struct
	{
		MD_S32 x;
		MD_S32 y;
	};
	MD_S32 v[2];
};

typedef union MD_Vec2S16 MD_Vec2S16;
union MD_Vec2S16
{
	struct
	{
		MD_S16 x;
		MD_S16 y;
	};
	MD_S16 v[2];
};

//- rjf: 3-vectors

typedef union MD_Vec3F32 MD_Vec3F32;
union MD_Vec3F32
{
	struct
	{
		MD_F32 x;
		MD_F32 y;
		MD_F32 z;
	};
	struct
	{
		MD_Vec2F32 xy;
		MD_F32 _z0;
	};
	struct
	{
		MD_F32 _x0;
		MD_Vec2F32 yz;
	};
	MD_F32 v[3];
};

typedef union MD_Vec3S32 MD_Vec3S32;
union MD_Vec3S32
{
	struct
	{
		MD_S32 x;
		MD_S32 y;
		MD_S32 z;
	};
	struct
	{
		MD_Vec2S32 xy;
		MD_S32 _z0;
	};
	struct
	{
		MD_S32 _x0;
		MD_Vec2S32 yz;
	};
	MD_S32 v[3];
};

//- rjf: 4-vectors

typedef union MD_Vec4F32 MD_Vec4F32;
union MD_Vec4F32
{
	struct
	{
		MD_F32 x;
		MD_F32 y;
		MD_F32 z;
		MD_F32 w;
	};
	struct
	{
		MD_Vec2F32 xy;
		MD_Vec2F32 zw;
	};
	struct
	{
		MD_Vec3F32 xyz;
		MD_F32 _z0;
	};
	struct
	{
		MD_F32 _x0;
		MD_Vec3F32 yzw;
	};
	MD_F32 v[4];
};

typedef union MD_Vec4S32 MD_Vec4S32;
union MD_Vec4S32
{
	struct
	{
		MD_S32 x;
		MD_S32 y;
		MD_S32 z;
		MD_S32 w;
	};
	struct
	{
		MD_Vec2S32 xy;
		MD_Vec2S32 zw;
	};
	struct
	{
		MD_Vec3S32 xyz;
		MD_S32 _z0;
	};
	struct
	{
		MD_S32 _x0;
		MD_Vec3S32 yzw;
	};
	MD_S32 v[4];
};

////////////////////////////////
//~ rjf: Matrix Types

typedef struct MD_Mat3x3F32 MD_Mat3x3F32;
struct MD_Mat3x3F32
{
	MD_F32 v[3][3];
};

typedef struct MD_Mat4x4F32 MD_Mat4x4F32;
struct MD_Mat4x4F32
{
	MD_F32 v[4][4];
};

////////////////////////////////
//~ rjf: Range Types

//- rjf: 1-range

typedef union MD_Rng1U32 MD_Rng1U32;
union MD_Rng1U32
{
	struct
	{
		MD_U32 md_min;
		MD_U32 md_max;
	};
	MD_U32 v[2];
};

typedef union MD_Rng1S32 MD_Rng1S32;
union MD_Rng1S32
{
	struct
	{
		MD_S32 md_min;
		MD_S32 md_max;
	};
	MD_S32 v[2];
};

typedef union MD_Rng1U64 MD_Rng1U64;
union MD_Rng1U64
{
	struct
	{
		MD_U64 md_min;
		MD_U64 md_max;
	};
	MD_U64 v[2];
};

typedef union Rng1S64 Rng1S64;
union Rng1S64
{
	struct
	{
		MD_S64 md_min;
		MD_S64 md_max;
	};
	MD_S64 v[2];
};

typedef union Rng1F32 Rng1F32;
union Rng1F32
{
	struct
	{
		MD_F32 md_min;
		MD_F32 md_max;
	};
	MD_F32 v[2];
};

//- rjf: 2-range (rectangles)

typedef union MD_Rng2S16 MD_Rng2S16;
union MD_Rng2S16
{
	struct
	{
		MD_Vec2S16 md_min;
		MD_Vec2S16 md_max;
	};
	struct
	{
		MD_Vec2S16 p0;
		MD_Vec2S16 p1;
	};
	struct
	{
		MD_S16 x0;
		MD_S16 y0;
		MD_S16 x1;
		MD_S16 y1;
	};
	MD_Vec2S16 v[2];
};

typedef union MD_Rng2S32 MD_Rng2S32;
union MD_Rng2S32
{
	struct
	{
		MD_Vec2S32 md_min;
		MD_Vec2S32 md_max;
	};
	struct
	{
		MD_Vec2S32 p0;
		MD_Vec2S32 p1;
	};
	struct
	{
		MD_S32 x0;
		MD_S32 y0;
		MD_S32 x1;
		MD_S32 y1;
	};
	MD_Vec2S32 v[2];
};

typedef union MD_Rng2F32 MD_Rng2F32;
union MD_Rng2F32
{
	struct
	{
		MD_Vec2F32 md_min;
		MD_Vec2F32 md_max;
	};
	struct
	{
		MD_Vec2F32 p0;
		MD_Vec2F32 p1;
	};
	struct
	{
		MD_F32 x0;
		MD_F32 y0;
		MD_F32 x1;
		MD_F32 y1;
	};
	MD_Vec2F32 v[2];
};

typedef union MD_Rng2S64 MD_Rng2S64;
union MD_Rng2S64
{
	struct
	{
		MD_Vec2S64 md_min;
		MD_Vec2S64 md_max;
	};
	struct
	{
		MD_Vec2S64 p0;
		MD_Vec2S64 p1;
	};
	struct
	{
		MD_S64 x0;
		MD_S64 y0;
		MD_S64 x1;
		MD_S64 y1;
	};
	MD_Vec2S64 v[2];
};

////////////////////////////////
//~ rjf: List Types

typedef struct MD_Rng1S64Node MD_Rng1S64Node;
struct MD_Rng1S64Node
{
	MD_Rng1S64Node* next;
	Rng1S64      v;
};

typedef struct MD_Rng1S64List MD_Rng1S64List;
struct MD_Rng1S64List
{
	MD_Rng1S64Node* first;
	MD_Rng1S64Node* last;
	MD_U64 count;
};

typedef struct MD_Rng1S64Array MD_Rng1S64Array;
struct MD_Rng1S64Array
{
	Rng1S64* v;
	MD_U64 count;
};

////////////////////////////////
//~ rjf: Clamps, Mins, Maxes

#ifndef md_min
#define md_min(A, B) (((A) < (B)) ? (A) : (B))
#endif
#ifndef md_max
#define md_max(A, B) (((A) > (B)) ? (A) : (B))
#endif

#ifndef md_clamp_top
#define md_clamp_top(A, X) md_min(A, X)
#endif
#ifndef md_clamp_bot
#define md_clamp_bot(X, B) md_max(X, B)
#endif

#define md_clamp(A, X, B) (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))

////////////////////////////////
//~ rjf: Scalar Ops

#define md_abs_s64(v) (MD_S64)llabs(v)

#define md_sqrt_f32(v)   sqrtf(v)
#define md_mod_f32(a, b) fmodf((a), (b))
#define md_pow_f32(b, e) powf((b), (e))
#define md_ceil_f32(v)   ceilf(v)
#define md_floor_f32(v)  floorf(v)
#define md_round_f32(v)  roundf(v)
#define md_abs_f32(v)    fabsf(v)

#define md_radians_from_turns_f32(v) ((v) * 2 * 3.1415926535897f)
#define md_turns_from_radians_f32(v) ((v) / 2 * 3.1415926535897f)
#define md_degrees_from_turns_f32(v) ((v) * 360.f)
#define md_turns_from_degrees_f32(v) ((v) / 360.f)

#define md_degrees_from_radians_f32(v) (md_degrees_from_turns_f32(md_turns_from_radians_f32(v)))
#define md_radians_from_degrees_f32(v) (md_radians_from_turns_f32(md_turns_from_degrees_f32(v)))

#define md_sin_f32(v)    sinf( md_radians_from_turns_f32(v) )
#define md_cos_f32(v)    cosf( md_radians_from_turns_f32(v) )
#define md_tan_f32(v)    tanf( md_radians_from_turns_f32(v) )

#define md_sqrt_f64(v)   sqrt(v)
#define md_mod_f64(a, b) fmod((a), (b))
#define md_pow_f64(b, e) pow((b), (e))
#define md_ceil_f64(v)   ceil(v)
#define md_floor_f64(v)  floor(v)
#define md_round_f64(v)  round(v)
#define md_abs_f64(v)    fabs(v)

#define md_radians_from_turns_f64(v) ((v) * 2 * 3.1415926535897)
#define md_turns_from_radians_f64(v) ((v) / 2 * 3.1415926535897)
#define md_degrees_from_turns_f64(v) ((v) * 360.0)
#define turns_from_degrees_f64(v) ((v) / 360.0)

#define md_degrees_from_radians_f64(v) (md_degrees_from_turns_f64(md_turns_from_radians_f64(v)))
#define md_radians_from_degrees_f64(v) (md_radians_from_turns_f64(turns_from_degrees_f64(v)))

#define md_sin_f64(v)    sin(md_radians_from_turns_f64(v))
#define md_cos_f64(v)    cos(md_radians_from_turns_f64(v))
#define md_tan_f64(v)    tan(md_radians_from_turns_f64(v))

inline MD_F32 md_mix_1f32(MD_F32 a, MD_F32 b, MD_F32 t) { MD_F32 c = (a + (b - a) * md_clamp(0.f, t, 1.f)); return c; }
inline MD_F64 md_mix_1f64(MD_F64 a, MD_F64 b, MD_F64 t) { MD_F64 c = (a + (b - a) * md_clamp(0.0, t, 1.0)); return c; }

////////////////////////////////
//~ rjf: Vector Ops

// ==================== 2D Vectors ====================

#define md_v2f32(x, y) md_vec_2f32((x), (y))
inline MD_Vec2F32 md_vec_2f32           (MD_F32     x, MD_F32     y)        { MD_Vec2F32 v = { x, y };                                       return v; }
inline MD_Vec2F32 md_add_2f32           (MD_Vec2F32 a, MD_Vec2F32 b)        { MD_Vec2F32 c = {a.x + b.x, a.y + b.y};                         return c; }
inline MD_Vec2F32 md_sub_2f32           (MD_Vec2F32 a, MD_Vec2F32 b)        { MD_Vec2F32 c = {a.x - b.x, a.y - b.y};                         return c; }
inline MD_Vec2F32 md_mul_2f32           (MD_Vec2F32 a, MD_Vec2F32 b)        { MD_Vec2F32 c = {a.x * b.x, a.y * b.y};                         return c; }
inline MD_Vec2F32 md_div_2f32           (MD_Vec2F32 a, MD_Vec2F32 b)        { MD_Vec2F32 c = {a.x / b.x, a.y / b.y};                         return c; }
inline MD_Vec2F32 md_scale_2f32         (MD_Vec2F32 v, MD_F32     s)        { MD_Vec2F32 c = {v.x * s,   v.y * s  };                         return c; }
inline MD_F32     md_dot_2f32           (MD_Vec2F32 a, MD_Vec2F32 b)        { MD_F32     c = a.x * b.x + a.y * b.y;                          return c; }
inline MD_F32     md_length_squared_2f32(MD_Vec2F32 v)                   { MD_F32     c = v.x * v.x + v.y * v.y;                          return c; }
inline MD_F32     md_length_2f32        (MD_Vec2F32 v)                   { MD_F32     c = md_sqrt_f32(v.x*v.x + v.y*v.y);                    return c; }
inline MD_Vec2F32 md_normalize_2f32     (MD_Vec2F32 v)                   {         v = md_scale_2f32(v, 1.f / md_length_2f32(v));            return v; }
inline MD_Vec2F32 md_mix_2f32           (MD_Vec2F32 a, MD_Vec2F32 b, MD_F32 t) { MD_Vec2F32 c = {md_mix_1f32(a.x, b.x, t), md_mix_1f32(a.y, b.y, t)}; return c; }

#define md_v2s64(x, y) md_vec_2s64((x), (y))
inline MD_Vec2S64 md_vec_2s64           (MD_S64     x, MD_S64     y)        { MD_Vec2S64 v = { x, y };                                       return v; }
inline MD_Vec2S64 md_add_2s64           (MD_Vec2S64 a, MD_Vec2S64 b)        { MD_Vec2S64 c = {a.x + b.x, a.y + b.y};                         return c; }
inline MD_Vec2S64 md_sub_2s64           (MD_Vec2S64 a, MD_Vec2S64 b)        { MD_Vec2S64 c = {a.x - b.x, a.y - b.y};                         return c; }
inline MD_Vec2S64 md_mul_2s64           (MD_Vec2S64 a, MD_Vec2S64 b)        { MD_Vec2S64 c = {a.x * b.x, a.y * b.y};                         return c; }
inline MD_Vec2S64 md_div_2s64           (MD_Vec2S64 a, MD_Vec2S64 b)        { MD_Vec2S64 c = {a.x / b.x, a.y / b.y};                         return c; }
inline MD_Vec2S64 md_scale_2s64         (MD_Vec2S64 v, MD_S64     s)        { MD_Vec2S64 c = {v.x * s,   v.y * s  };                         return c; }
inline MD_S64     md_dot_2s64           (MD_Vec2S64 a, MD_Vec2S64 b)        { MD_S64     c = a.x * b.x + a.y * b.y;                          return c; }
inline MD_S64     md_length_squared_2s64(MD_Vec2S64 v)                   { MD_S64     c = v.x * v.x + v.y * v.y;                          return c; }
inline MD_S64     md_length_2s64        (MD_Vec2S64 v)                   { MD_S64     c = (MD_S64)md_sqrt_f64((MD_F64)(v.x*v.x + v.y*v.y));        return c; }
inline MD_Vec2S64 md_normalize_2s64     (MD_Vec2S64 v)                   {         v = md_scale_2s64(v, (MD_S64)(1.f / md_length_2s64(v)));     return v; }
inline MD_Vec2S64 md_mix_2s64           (MD_Vec2S64 a, MD_Vec2S64 b, MD_F32 t) { MD_Vec2S64 c = {(MD_S64)md_mix_1f32((MD_F32)a.x, (MD_F32)b.x, t), (MD_S64)md_mix_1f32((MD_F32)a.y, (MD_F32)b.y, t)}; return c; }

#define md_v2s32(x, y) md_vec_2s32((x), (y))
inline MD_Vec2S32 md_vec_2s32           (MD_S32 x, MD_S32 y)                { MD_Vec2S32 v = { x, y };                                       return v; }
inline MD_Vec2S32 md_add_2s32           (MD_Vec2S32 a, MD_Vec2S32 b)        { MD_Vec2S32 c = {a.x + b.x, a.y + b.y};                         return c; }
inline MD_Vec2S32 md_sub_2s32           (MD_Vec2S32 a, MD_Vec2S32 b)        { MD_Vec2S32 c = {a.x - b.x, a.y - b.y};                         return c; }
inline MD_Vec2S32 md_mul_2s32           (MD_Vec2S32 a, MD_Vec2S32 b)        { MD_Vec2S32 c = {a.x * b.x, a.y * b.y};                         return c; }
inline MD_Vec2S32 md_div_2s32           (MD_Vec2S32 a, MD_Vec2S32 b)        { MD_Vec2S32 c = {a.x / b.x, a.y / b.y};                         return c; }
inline MD_Vec2S32 md_scale_2s32         (MD_Vec2S32 v, MD_S32     s)        { MD_Vec2S32 c = {v.x * s,   v.y * s  };                         return c; }
inline MD_S32     md_dot_2s32           (MD_Vec2S32 a, MD_Vec2S32 b)        { MD_S32     c = a.x * b.x + a.y * b.y;                          return c; }
inline MD_S32     md_length_squared_2s32(MD_Vec2S32 v)                   { MD_S32     c = v.x * v.x + v.y * v.y;                          return c; }
inline MD_S32     md_length_2s32        (MD_Vec2S32 v)                   { MD_S32     c = (MD_S32)md_sqrt_f32((MD_F32)v.x*(MD_F32)v.x + (MD_F32)v.y*(MD_F32)v.y); return c; }
inline MD_Vec2S32 md_normalize_2s32     (MD_Vec2S32 v)                   {         v = md_scale_2s32(v, (MD_S32)(1.f / md_length_2s32(v)));     return v; }
inline MD_Vec2S32 md_mix_2s32           (MD_Vec2S32 a, MD_Vec2S32 b, MD_F32 t) { MD_Vec2S32 c = {(MD_S32)md_mix_1f32((MD_F32)a.x, (MD_F32)b.x, t), (MD_S32)md_mix_1f32((MD_F32)a.y, (MD_F32)b.y, t)}; return c; }

#define md_v2s16(x, y) md_vec_2s16((x), (y))
inline MD_Vec2S16 md_vec_2s16           (MD_S16 x, MD_S16 y)                { MD_Vec2S16 v = { x, y };                                       return v; }
inline MD_Vec2S16 md_add_2s16           (MD_Vec2S16 a, MD_Vec2S16 b)        { MD_Vec2S16 c = {(MD_S16)(a.x + b.x), (MD_S16)(a.y + b.y)};           return c; }
inline MD_Vec2S16 md_sub_2s16           (MD_Vec2S16 a, MD_Vec2S16 b)        { MD_Vec2S16 c = {(MD_S16)(a.x - b.x), (MD_S16)(a.y - b.y)};           return c; }
inline MD_Vec2S16 md_mul_2s16           (MD_Vec2S16 a, MD_Vec2S16 b)        { MD_Vec2S16 c = {(MD_S16)(a.x * b.x), (MD_S16)(a.y * b.y)};           return c; }
inline MD_Vec2S16 md_div_2s16           (MD_Vec2S16 a, MD_Vec2S16 b)        { MD_Vec2S16 c = {(MD_S16)(a.x / b.x), (MD_S16)(a.y / b.y)};           return c; }
inline MD_Vec2S16 md_scale_2s16         (MD_Vec2S16 v, MD_S16     s)        { MD_Vec2S16 c = {(MD_S16)(v.x * s  ), (MD_S16)(v.y * s  )};           return c; }
inline MD_S16     md_dot_2s16           (MD_Vec2S16 a, MD_Vec2S16 b)        { MD_S16     c = a.x * b.x + a.y * b.y;                          return c; }
inline MD_S16     md_length_squared_2s16(MD_Vec2S16 v)                   { MD_S16     c = v.x * v.x + v.y * v.y;                          return c; }
inline MD_S16     md_length_2s16        (MD_Vec2S16 v)                   { MD_S16     c = (MD_S16)md_sqrt_f32((MD_F32)(v.x*v.x + v.y*v.y));        return c; }
inline MD_Vec2S16 md_normalize_2s16     (MD_Vec2S16 v)                   {         v = md_scale_2s16(v, (MD_S16)(1.f / md_length_2s16(v)));     return v; }
inline MD_Vec2S16 md_mix_2s16           (MD_Vec2S16 a, MD_Vec2S16 b, MD_F32 t) { MD_Vec2S16 c = {(MD_S16)md_mix_1f32((MD_F32)a.x, (MD_F32)b.x, t), (MD_S16)md_mix_1f32((MD_F32)a.y, (MD_F32)b.y, t)}; return c; }

#define md_vec2(a, b)             _Generic(a, MD_S16: md_vec_2s16,            MD_S32: md_vec_2s32,            MD_S64: md_vec_2s64,            MD_F32: md_vec_2f32           )((a), (b))
#define md_add_vec2(a, b)         _Generic(a, MD_S16: md_add_2s16,            MD_S32: md_add_2s32,            MD_S64: md_add_2s64,            MD_F32: md_add_2f32           )((a), (b))
#define md_sub_vec2(a, b)         _Generic(a, MD_S16: md_sub_2s16,            MD_S32: md_sub_2s32,            MD_S64: md_sub_2s64,            MD_F32: md_sub_2f32           )((a), (b))
#define md_mul_vec2(a, b)         _Generic(a, MD_S16: md_mul_2s16,            MD_S32: md_mul_2s32,            MD_S64: md_mul_2s64,            MD_F32: md_mul_2f32           )((a), (b))
#define md_div_vec2(a, b)         _Generic(a, MD_S16: md_div_2s16,            MD_S32: md_div_2s32,            MD_S64: md_div_2s64,            MD_F32: md_div_2f32           )((a), (b))
#define md_scale_vec2(v, s)       _Generic(v, MD_S16: md_scale_2s16,          MD_S32: md_scale_2s32,          MD_S64: md_scale_2s64,          MD_F32: md_scale_2f32         )((v), (s))
#define md_dot_vec2(a, b)         _Generic(a, MD_S16: md_dot_2s16,            MD_S32: md_dot_2s32,            MD_S64: md_dot_2s64,            MD_F32: md_dot_2f32           )((a), (b))
#define md_length_squared_vec2(v) _Generic(v, MD_S16: md_length_squared_2s16, MD_S32: md_length_squared_2s32, MD_S64: md_length_squared_2s64, MD_F32: md_length_squared_2f32)((v))
#define md_length_vec2(v)         _Generic(v, MD_S16: md_length_2s16,         MD_S32: md_length_2s32,         MD_S64: md_length_2s64,         MD_F32: md_length_2f32        )((v))
#define md_normalize_vec2(v)      _Generic(v, MD_S16: md_normalize_2s16,      MD_S32: md_normalize_2s32,      MD_S64: md_normalize_2s64,      MD_F32: md_normalize_2f32     )((v))
#define md_mix_vec2(a, b, t)      _Generic(a, MD_S16: md_mix_2s16,            MD_S32: md_mix_2s32,            MD_S64: md_mix_2s64,            MD_F32: md_mix_2f32           )((a), (b), (t))

// ==================== 3D Vectors ====================

#define md_v3f32(x, y, z) md_vec_3f32((x), (y), (z))
inline MD_Vec3F32 md_vec_3f32           (MD_F32 x, MD_F32 y, MD_F32 z)         { MD_Vec3F32 v = {x, y, z}; return v; }
inline MD_Vec3F32 md_add_3f32           (MD_Vec3F32 a, MD_Vec3F32 b)        { MD_Vec3F32 c = {a.x + b.x, a.y + b.y, a.z + b.z};           return c; }
inline MD_Vec3F32 md_sub_3f32           (MD_Vec3F32 a, MD_Vec3F32 b)        { MD_Vec3F32 c = {a.x - b.x, a.y - b.y, a.z - b.z};           return c; }
inline MD_Vec3F32 md_mul_3f32           (MD_Vec3F32 a, MD_Vec3F32 b)        { MD_Vec3F32 c = {a.x * b.x, a.y * b.y, a.z * b.z};           return c; }
inline MD_Vec3F32 md_div_3f32           (MD_Vec3F32 a, MD_Vec3F32 b)        { MD_Vec3F32 c = {a.x / b.x, a.y / b.y, a.z / b.z};           return c; }
inline MD_Vec3F32 md_scale_3f32         (MD_Vec3F32 v, MD_F32     s)        { MD_Vec3F32 c = {v.x * s,   v.y * s,   v.z * s};             return c; }
inline MD_F32     md_dot_3f32           (MD_Vec3F32 a, MD_Vec3F32 b)        { MD_F32     c = a.x * b.x + a.y * b.y + a.z * b.z;           return c; }
inline MD_F32     md_length_squared_3f32(MD_Vec3F32 v)                   { MD_F32     c = v.x * v.x + v.y * v.y + v.z * v.z;           return c; }
inline MD_F32     md_length_3f32        (MD_Vec3F32 v)                   { MD_F32     c = md_sqrt_f32(v.x * v.x + v.y * v.y + v.z * v.z); return c; }
inline MD_Vec3F32 md_normalize_3f32     (MD_Vec3F32 v)                   {         v = md_scale_3f32(v, 1.f / md_length_3f32(v));         return v; }
inline MD_Vec3F32 md_mix_3f32           (MD_Vec3F32 a, MD_Vec3F32 b, MD_F32 t) { MD_Vec3F32 c = {md_mix_1f32(a.x, b.x, t), md_mix_1f32(a.y, b.y, t), md_mix_1f32(a.z, b.z, t)}; return c; }
inline MD_Vec3F32 md_cross_3f32         (MD_Vec3F32 a, MD_Vec3F32 b)        { MD_Vec3F32 c = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; return c; }

#define md_v3s32(x, y, z) md_vec_3s32((x), (y), (z))
inline MD_Vec3S32 md_vec_3s32           (MD_S32 x, MD_S32 y, MD_S32 z)         { MD_Vec3S32 v = {x, y, z}; return v; }
inline MD_Vec3S32 md_add_3s32           (MD_Vec3S32 a, MD_Vec3S32 b)        { MD_Vec3S32 c = {a.x + b.x, a.y + b.y, a.z + b.z};                       return c; }
inline MD_Vec3S32 md_sub_3s32           (MD_Vec3S32 a, MD_Vec3S32 b)        { MD_Vec3S32 c = {a.x - b.x, a.y - b.y, a.z - b.z};                       return c; }
inline MD_Vec3S32 md_mul_3s32           (MD_Vec3S32 a, MD_Vec3S32 b)        { MD_Vec3S32 c = {a.x * b.x, a.y * b.y, a.z * b.z};                       return c; }
inline MD_Vec3S32 md_div_3s32           (MD_Vec3S32 a, MD_Vec3S32 b)        { MD_Vec3S32 c = {a.x / b.x, a.y / b.y, a.z / b.z};                       return c; }
inline MD_Vec3S32 md_scale_3s32         (MD_Vec3S32 v, MD_S32     s)        { MD_Vec3S32 c = {v.x * s,   v.y * s,   v.z * s  };                       return c; }
inline MD_S32     md_dot_3s32           (MD_Vec3S32 a, MD_Vec3S32 b)        { MD_S32     c = a.x * b.x + a.y * b.y + a.z * b.z;                       return c; }
inline MD_S32     md_length_squared_3s32(MD_Vec3S32 v)                   { MD_S32     c = v.x * v.x + v.y * v.y + v.z * v.z;                       return c; }
inline MD_S32     md_length_3s32        (MD_Vec3S32 v)                   { MD_S32     c = (MD_S32)md_sqrt_f32((MD_F32)(v.x * v.x + v.y * v.y + v.z * v.z)); return c; }
inline MD_Vec3S32 md_normalize_3s32     (MD_Vec3S32 v)                   {         v = md_scale_3s32(v, (MD_S32)(1.f / md_length_3s32(v)));              return v; }
inline MD_Vec3S32 md_mix_3s32           (MD_Vec3S32 a, MD_Vec3S32 b, MD_F32 t) { MD_Vec3S32 c = {(MD_S32)md_mix_1f32((MD_F32)a.x, (MD_F32)b.x, t), (MD_S32)md_mix_1f32((MD_F32)a.y, (MD_F32)b.y, t), (MD_S32)md_mix_1f32((MD_F32)a.z, (MD_F32)b.z, t)}; return c; }
inline MD_Vec3S32 md_cross_3s32         (MD_Vec3S32 a, MD_Vec3S32 b)        { MD_Vec3S32 c = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; return c; }

#define md_vec3(a, b, c)          _Generic(a, MD_S32: md_vec_3s32,            MD_F32: md_vec_3f32           )((a), (b), (c))
#define md_add_vec3(a, b)         _Generic(a, MD_S32: md_add_3s32,            MD_F32: md_add_3f32           )((a), (b))
#define md_sub_vec3(a, b)         _Generic(a, MD_S32: md_sub_3s32,            MD_F32: md_sub_3f32           )((a), (b))
#define md_mul_vec3(a, b)         _Generic(a, MD_S32: md_mul_3s32,            MD_F32: md_mul_3f32           )((a), (b))
#define md_div_vec3(a, b)         _Generic(a, MD_S32: md_div_3s32,            MD_F32: md_div_3f32           )((a), (b))
#define md_scale_vec3(v, s)       _Generic(v, MD_S32: md_scale_3s32,          MD_F32: md_scale_3f32         )((v), (s))
#define md_dot_vec3(a, b)         _Generic(a, MD_S32: md_dot_3s32,            MD_F32: md_dot_3f32           )((a), (b))
#define md_length_squared_vec3(v) _Generic(v, MD_S32: md_length_squared_3s32, MD_F32: md_length_squared_3f32)((v))
#define md_length_vec3(v)         _Generic(v, MD_S32: md_length_3s32,         MD_F32: md_length_3f32        )((v))
#define md_normalize_vec3(v)      _Generic(v, MD_S32: md_normalize_3s32,      MD_F32: md_normalize_3f32     )((v))
#define md_mix_vec3(a, b, t)      _Generic(a, MD_S32: md_mix_3s32,            MD_F32: md_mix_3f32           )((a), (b), (t))
#define md_cross_vec3(a, b)       _Generic(a, MD_S32: md_cross_3s32,          MD_F32: md_cross_3f32         )((a), (b))

// ==================== 4D Vectors ====================

#define md_v4f32(x, y, z, w) md_vec_4f32((x), (y), (z), (w))
inline MD_Vec4F32 md_vec_4f32           (MD_F32 x, MD_F32 y, MD_F32 z, MD_F32 w)  { MD_Vec4F32 v = {x, y, z, w}; return v; }
inline MD_Vec4F32 md_add_4f32           (MD_Vec4F32 a, MD_Vec4F32 b)        { MD_Vec4F32 c = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};            return c; }
inline MD_Vec4F32 md_sub_4f32           (MD_Vec4F32 a, MD_Vec4F32 b)        { MD_Vec4F32 c = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};            return c; }
inline MD_Vec4F32 md_mul_4f32           (MD_Vec4F32 a, MD_Vec4F32 b)        { MD_Vec4F32 c = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};            return c; }
inline MD_Vec4F32 md_div_4f32           (MD_Vec4F32 a, MD_Vec4F32 b)        { MD_Vec4F32 c = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};            return c; }
inline MD_Vec4F32 md_scale_4f32         (MD_Vec4F32 v, MD_F32     s)        { MD_Vec4F32 c = {v.x * s,   v.y * s,   v.z * s,   v.w * s  };            return c; }
inline MD_F32     md_dot_4f32           (MD_Vec4F32 a, MD_Vec4F32 b)        { MD_F32     c = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;           return c; }
inline MD_F32     md_length_squared_4f32(MD_Vec4F32 v)                   { MD_F32     c = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;           return c; }
inline MD_F32     md_length_4f32        (MD_Vec4F32 v)                   { MD_F32     c = md_sqrt_f32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); return c; }
inline MD_Vec4F32 md_normalize_4f32     (MD_Vec4F32 v)                   {         v = md_scale_4f32(v, 1.f / md_length_4f32(v));                     return v; }
inline MD_Vec4F32 md_mix_4f32           (MD_Vec4F32 a, MD_Vec4F32 b, MD_F32 t) { MD_Vec4F32 c = {md_mix_1f32(a.x, b.x, t), md_mix_1f32(a.y, b.y, t), md_mix_1f32(a.z, b.z, t), md_mix_1f32(a.w, b.w, t)}; return c; }

#define v4s32(x, y, z, w) md_vec_4s32((x), (y), (z), (w))
inline MD_Vec4S32 md_vec_4s32           (MD_S32 x, MD_S32 y, MD_S32 z, MD_S32 w)  { MD_Vec4S32 v = {x, y, z, w}; return v; }
inline MD_Vec4S32 md_add_4s32           (MD_Vec4S32 a, MD_Vec4S32 b)        { MD_Vec4S32 c = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};                        return c; }
inline MD_Vec4S32 md_sub_4s32           (MD_Vec4S32 a, MD_Vec4S32 b)        { MD_Vec4S32 c = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};                        return c; }
inline MD_Vec4S32 md_mul_4s32           (MD_Vec4S32 a, MD_Vec4S32 b)        { MD_Vec4S32 c = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};                        return c; }
inline MD_Vec4S32 md_div_4s32           (MD_Vec4S32 a, MD_Vec4S32 b)        { MD_Vec4S32 c = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};                        return c; }
inline MD_Vec4S32 md_scale_4s32         (MD_Vec4S32 v, MD_S32 s)            { MD_Vec4S32 c = {v.x * s,   v.y * s,   v.z * s,   v.w * s  };                        return c; }
inline MD_S32     md_dot_4s32           (MD_Vec4S32 a, MD_Vec4S32 b)        { MD_S32     c = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;                       return c; }
inline MD_S32     md_length_squared_4s32(MD_Vec4S32 v)                   { MD_S32     c = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;                       return c; }
inline MD_S32     md_length_4s32        (MD_Vec4S32 v)                   { MD_S32     c = (MD_S32)md_sqrt_f32((MD_F32)(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w)); return c; }
inline MD_Vec4S32 md_normalize_4s32     (MD_Vec4S32 v)                   {         v = md_scale_4s32(v, (MD_S32)(1.f / md_length_4s32(v)));                          return v; }
inline MD_Vec4S32 md_mix_4s32           (MD_Vec4S32 a, MD_Vec4S32 b, MD_F32 t) { MD_Vec4S32 c = {(MD_S32)md_mix_1f32((MD_F32)a.x, (MD_F32)b.x, t), (MD_S32)md_mix_1f32((MD_F32)a.y, (MD_F32)b.y, t), (MD_S32)md_mix_1f32((MD_F32)a.z, (MD_F32)b.z, t), (MD_S32)md_mix_1f32((MD_F32)a.w, (MD_F32)b.w, t)}; return c; }

#define md_vec4(a, b, c, d)       _Generic(a, MD_S32: md_vec_4s32,            MD_F32: md_vec_4f32           )((a), (b), (c), (d))
#define md_add_vec4(a, b)         _Generic(a, MD_S32: md_add_4s32,            MD_F32: md_add_4f32           )((a), (b))
#define md_sub_vec4(a, b)         _Generic(a, MD_S32: md_sub_4s32,            MD_F32: md_sub_4f32           )((a), (b))
#define md_mul_vec4(a, b)         _Generic(a, MD_S32: md_mul_4s32,            MD_F32: md_mul_4f32           )((a), (b))
#define md_div_vec4(a, b)         _Generic(a, MD_S32: md_div_4s32,            MD_F32: md_div_4f32           )((a), (b))
#define md_scale_vec4(v, s)       _Generic(v, MD_S32: md_scale_4s32,          MD_F32: md_scale_4f32         )((v), (s))
#define md_dot_vec4(a, b)         _Generic(a, MD_S32: md_dot_4s32,            MD_F32: md_dot_4f32           )((a), (b))
#define md_length_squared_vec4(v) _Generic(v, MD_S32: md_length_squared_4s32, MD_F32: md_length_squared_4f32)((v))
#define md_length_vec4(v)         _Generic(v, MD_S32: md_length_4s32,         MD_F32: md_length_4f32        )((v))
#define md_normalize_vec4(v)      _Generic(v, MD_S32: md_normalize_4s32,      MD_F32: md_normalize_4f32     )((v))
#define md_mix_vec4(a, b, t)      _Generic(a, MD_S32: md_mix_4s32,            MD_F32: md_mix_4f32           )((a), (b), (t))

////////////////////////////////
//~ rjf: Matrix Ops

// ==================== 3x3 Matrix ====================

inline MD_Mat3x3F32 md_mat_3x3f32           (MD_F32     diag)             { MD_Mat3x3F32 res = {0};             res.v[0][0] = diag;    res.v[1][1] = diag;    res.v[2][2] = diag; return res; }
inline MD_Mat3x3F32 md_make_translate_3x3f32(MD_Vec2F32 delta)            { MD_Mat3x3F32 mat = md_mat_3x3f32(1.f); mat.v[2][0] = delta.x; mat.v[2][1] = delta.y;                     return mat; }
inline MD_Mat3x3F32 md_make_scale_3x3f32    (MD_Vec2F32 scale)            { MD_Mat3x3F32 mat = md_mat_3x3f32(1.f); mat.v[0][0] = scale.x; mat.v[1][1] = scale.y;                     return mat; }
inline MD_Mat3x3F32 md_mul_3x3f32           (MD_Mat3x3F32 a, MD_Mat3x3F32 b) { MD_Mat3x3F32 c = {0}; for(int j = 0; j < 3; j += 1) for(int i = 0; i < 3; i += 1) { c.v[i][j] = (a.v[0][j]*b.v[i][0] + a.v[1][j] * b.v[i][1] + a.v[2][j] * b.v[i][2]); } return c; }

inline MD_Mat4x4F32 md_mat_4x4f32           (MD_F32     diag)  { MD_Mat4x4F32 res = {0};             res.v[0][0] = diag;    res.v[1][1] = diag;    res.v[2][2] = diag;    res.v[3][3] = diag; return res; }
inline MD_Mat4x4F32 md_make_translate_4x4f32(MD_Vec3F32 delta) { MD_Mat4x4F32 res = md_mat_4x4f32(1.f); res.v[3][0] = delta.x; res.v[3][1] = delta.y; res.v[3][2] = delta.z;                     return res; }
inline MD_Mat4x4F32 md_make_scale_4x4f32    (MD_Vec3F32 scale) { MD_Mat4x4F32 res = md_mat_4x4f32(1.f); res.v[0][0] = scale.x; res.v[1][1] = scale.y; res.v[2][2] = scale.z;                     return res; }

// ==================== 4x4 Matrix ====================

MD_Mat4x4F32 md_make_perspective_4x4f32 (MD_F32 fov,  MD_F32 aspect_ratio,               MD_F32 near_z, MD_F32 far_z);
MD_Mat4x4F32 md_make_orthographic_4x4f32(MD_F32 left, MD_F32 right, MD_F32 bottom, MD_F32 top, MD_F32 near_z, MD_F32 far_z);
MD_Mat4x4F32 md_make_look_at_4x4f32     (MD_Vec3F32 eye,  MD_Vec3F32 center, MD_Vec3F32 up);
MD_Mat4x4F32 md_make_rotate_4x4f32      (MD_Vec3F32 axis, MD_F32 turns);

MD_Mat4x4F32 md_mul_4x4f32     (MD_Mat4x4F32 a, MD_Mat4x4F32 b);
MD_Mat4x4F32 md_scale_4x4f32   (MD_Mat4x4F32 m, MD_F32       scale);
MD_Mat4x4F32 md_inverse_4x4f32 (MD_Mat4x4F32 m);
MD_Mat4x4F32 md_derotate_4x4f32(MD_Mat4x4F32 mat);

inline MD_Mat4x4F32 
md_make_perspective_4x4f32(MD_F32 fov, MD_F32 aspect_ratio, MD_F32 near_z, MD_F32 far_z) {
	MD_F32 tan_theta_over_2   = md_tan_f32(fov / 2);
	MD_F32 q_tan_theta_over_2 = 1 / tan_theta_over_2;
	MD_F32 q_near_far_z       = 1 / (near_z - far_z);
	MD_Mat4x4F32 
	result         = md_mat_4x4f32(1.f);
	result.v[0][0] =                q_tan_theta_over_2;
	result.v[1][1] = aspect_ratio * q_tan_theta_over_2;
	result.v[2][3] = 1.f;
	result.v[2][2] = -(      near_z + far_z) * q_near_far_z;
	result.v[3][2] =  (2.f * near_z * far_z) * q_near_far_z;
	result.v[3][3] = 0.f;
	return result;
}

inline MD_Mat4x4F32 
md_make_orthographic_4x4f32(MD_F32 left, MD_F32 right, MD_F32 bottom, MD_F32 top, MD_F32 near_z, MD_F32 far_z) {
	MD_Mat4x4F32 result = md_mat_4x4f32(1.f);
	
	result.v[0][0] = 2.f / (right - left);
	result.v[1][1] = 2.f / (top   - bottom);
	result.v[2][2] = 2.f / (far_z - near_z);
	result.v[3][3] = 1.f;
	
	result.v[3][0] = (left   + right) / (left   - right);
	result.v[3][1] = (bottom + top  ) / (bottom - top  );
	result.v[3][2] = (near_z + far_z) / (near_z - far_z);
	
	return result;
}

inline MD_Mat4x4F32 
md_make_look_at_4x4f32(MD_Vec3F32 eye, MD_Vec3F32 center, MD_Vec3F32 up) {
	MD_Mat4x4F32 result;
	MD_Vec3F32 f = md_normalize_3f32(md_sub_3f32(eye, center));
	MD_Vec3F32 s = md_normalize_3f32(md_cross_3f32(f, up));
	MD_Vec3F32 u = md_cross_3f32(s, f);
	result.v[0][0] =  s.x;              result.v[0][1] =  u.x;              result.v[0][2] = -f.x;              result.v[0][3] = 0.0f; 
	result.v[1][0] =  s.y;              result.v[1][1] =  u.y;              result.v[1][2] = -f.y;              result.v[1][3] = 0.0f;
	result.v[2][0] =  s.z;              result.v[2][1] =  u.z;              result.v[2][2] = -f.z;              result.v[2][3] = 0.0f;
	result.v[3][0] = -md_dot_3f32(s, eye); result.v[3][1] = -md_dot_3f32(u, eye); result.v[3][2] =  md_dot_3f32(f, eye); result.v[3][3] = 1.0f;
	return result;
}

inline MD_Mat4x4F32 
md_make_rotate_4x4f32(MD_Vec3F32 axis, MD_F32 turns) {
		axis       = md_normalize_3f32(axis);
	MD_F32 sin_theta  = md_sin_f32(turns);
	MD_F32 cos_theta  = md_cos_f32(turns);
	MD_F32 cos_value  = 1.f - cos_theta;
	MD_F32 mul_x_sint = axis.x * sin_theta;
	MD_F32 mul_y_sint = axis.y * sin_theta;
	MD_F32 mul_z_sint = axis.z * sin_theta;
	MD_F32 mul_xx_cos = axis.x * axis.x * cos_value;
	MD_F32 mul_yy_cos = axis.y * axis.y * cos_value;
	MD_F32 mul_zz_cos = axis.z * axis.z * cos_value;
	MD_F32 mul_xy_cos = axis.x * axis.y * cos_value;
	MD_F32 mul_xz_cos = axis.x * axis.z * cos_value;
	MD_F32 mul_yz_cos = axis.y * axis.z * cos_value;
	MD_Mat4x4F32 
	result         = md_mat_4x4f32(1.f);
	result.v[0][0] = mul_xx_cos + cos_theta; 
	result.v[0][1] = mul_xy_cos + mul_z_sint;
	result.v[0][2] = mul_xz_cos - mul_y_sint;
	result.v[1][0] = mul_xy_cos - mul_z_sint;
	result.v[1][1] = mul_yy_cos + cos_theta;
	result.v[1][2] = mul_yz_cos + mul_x_sint;
	result.v[2][0] = mul_xz_cos + mul_y_sint;
	result.v[2][1] = mul_yz_cos - mul_x_sint;
	result.v[2][2] = mul_zz_cos + cos_theta;
	return result;
}

inline MD_Mat4x4F32
md_mul_4x4f32(MD_Mat4x4F32 a, MD_Mat4x4F32 b) { 
	MD_Mat4x4F32 c = {0}; 
	for(int j = 0; j < 4; j += 1) 
	for(int i = 0; i < 4; i += 1) { 
		c.v[i][j] = (a.v[0][j] * b.v[i][0] + a.v[1][j] * b.v[i][1] + a.v[2][j] * b.v[i][2] + a.v[3][j] * b.v[i][3]);
	} 
	return c; 
} 

inline MD_Mat4x4F32
md_scale_4x4f32(MD_Mat4x4F32 m, MD_F32 scale) { 
	for(int j = 0; j < 4; j += 1)
	for(int i = 0; i < 4; i += 1) { 
		m.v[i][j] *= scale;
	} 
	return m;
}

inline MD_Mat4x4F32
md_inverse_4x4f32(MD_Mat4x4F32 m) {
	MD_F32 coef00 = m.v[2][2] * m.v[3][3] - m.v[3][2] * m.v[2][3];
	MD_F32 coef02 = m.v[1][2] * m.v[3][3] - m.v[3][2] * m.v[1][3];
	MD_F32 coef03 = m.v[1][2] * m.v[2][3] - m.v[2][2] * m.v[1][3];
	MD_F32 coef04 = m.v[2][1] * m.v[3][3] - m.v[3][1] * m.v[2][3];
	MD_F32 coef06 = m.v[1][1] * m.v[3][3] - m.v[3][1] * m.v[1][3];
	MD_F32 coef07 = m.v[1][1] * m.v[2][3] - m.v[2][1] * m.v[1][3];
	MD_F32 coef08 = m.v[2][1] * m.v[3][2] - m.v[3][1] * m.v[2][2];
	MD_F32 coef10 = m.v[1][1] * m.v[3][2] - m.v[3][1] * m.v[1][2];
	MD_F32 coef11 = m.v[1][1] * m.v[2][2] - m.v[2][1] * m.v[1][2];
	MD_F32 coef12 = m.v[2][0] * m.v[3][3] - m.v[3][0] * m.v[2][3];
	MD_F32 coef14 = m.v[1][0] * m.v[3][3] - m.v[3][0] * m.v[1][3];
	MD_F32 coef15 = m.v[1][0] * m.v[2][3] - m.v[2][0] * m.v[1][3];
	MD_F32 coef16 = m.v[2][0] * m.v[3][2] - m.v[3][0] * m.v[2][2];
	MD_F32 coef18 = m.v[1][0] * m.v[3][2] - m.v[3][0] * m.v[1][2];
	MD_F32 coef19 = m.v[1][0] * m.v[2][2] - m.v[2][0] * m.v[1][2];
	MD_F32 coef20 = m.v[2][0] * m.v[3][1] - m.v[3][0] * m.v[2][1];
	MD_F32 coef22 = m.v[1][0] * m.v[3][1] - m.v[3][0] * m.v[1][1];
	MD_F32 coef23 = m.v[1][0] * m.v[2][1] - m.v[2][0] * m.v[1][1];
	
	MD_Vec4F32 fac0 = { coef00, coef00, coef02, coef03 };
	MD_Vec4F32 fac1 = { coef04, coef04, coef06, coef07 };
	MD_Vec4F32 fac2 = { coef08, coef08, coef10, coef11 };
	MD_Vec4F32 fac3 = { coef12, coef12, coef14, coef15 };
	MD_Vec4F32 fac4 = { coef16, coef16, coef18, coef19 };
	MD_Vec4F32 fac5 = { coef20, coef20, coef22, coef23 };
	
	MD_Vec4F32 vec0 = { m.v[1][0], m.v[0][0], m.v[0][0], m.v[0][0] };
	MD_Vec4F32 vec1 = { m.v[1][1], m.v[0][1], m.v[0][1], m.v[0][1] };
	MD_Vec4F32 md_vec2 = { m.v[1][2], m.v[0][2], m.v[0][2], m.v[0][2] };
	MD_Vec4F32 md_vec3 = { m.v[1][3], m.v[0][3], m.v[0][3], m.v[0][3] };
	
	MD_Vec4F32 inv0 = md_add_4f32(md_sub_4f32(md_mul_4f32(vec1, fac0), md_mul_4f32(md_vec2, fac1)), md_mul_4f32(md_vec3, fac2));
	MD_Vec4F32 inv1 = md_add_4f32(md_sub_4f32(md_mul_4f32(vec0, fac0), md_mul_4f32(md_vec2, fac3)), md_mul_4f32(md_vec3, fac4));
	MD_Vec4F32 inv2 = md_add_4f32(md_sub_4f32(md_mul_4f32(vec0, fac1), md_mul_4f32(vec1, fac3)), md_mul_4f32(md_vec3, fac5));
	MD_Vec4F32 inv3 = md_add_4f32(md_sub_4f32(md_mul_4f32(vec0, fac2), md_mul_4f32(vec1, fac4)), md_mul_4f32(md_vec2, fac5));
	
	MD_Vec4F32 sign_a = { +1, -1, +1, -1 };
	MD_Vec4F32 sign_b = { -1, +1, -1, +1 };
	
	MD_Mat4x4F32 inverse;
	for(MD_U32 i = 0; i < 4; i += 1) {
		inverse.v[0][i] = inv0.v[i] * sign_a.v[i];
		inverse.v[1][i] = inv1.v[i] * sign_b.v[i];
		inverse.v[2][i] = inv2.v[i] * sign_a.v[i];
		inverse.v[3][i] = inv3.v[i] * sign_b.v[i];
	}
	
	MD_Vec4F32 row0 = { inverse.v[0][0], inverse.v[1][0], inverse.v[2][0], inverse.v[3][0] };
	MD_Vec4F32 m0   = { m.v[0][0], m.v[0][1], m.v[0][2], m.v[0][3] };
	MD_Vec4F32 dot0 = md_mul_4f32(m0, row0);
	MD_F32     dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

	MD_F32    one_over_det = 1 / dot1; 
	return md_scale_4x4f32(inverse, one_over_det);
}

inline MD_Mat4x4F32
md_derotate_4x4f32(MD_Mat4x4F32 mat) {
	MD_Vec3F32 scale = {
		md_length_3f32(md_v3f32(mat.v[0][0], mat.v[0][1], mat.v[0][2])),
		md_length_3f32(md_v3f32(mat.v[1][0], mat.v[1][1], mat.v[1][2])),
		md_length_3f32(md_v3f32(mat.v[2][0], mat.v[2][1], mat.v[2][2])),
	};
	mat.v[0][0] = scale.x; mat.v[1][0] = 0.f;     mat.v[2][0] = 0.f; 
	mat.v[0][1] = 0.f;     mat.v[1][1] = scale.y; mat.v[2][1] = 0.f; 
	mat.v[0][2] = 0.f;     mat.v[1][2] = 0.f;     mat.v[2][2] = scale.z;
	return mat;
}

////////////////////////////////
//~ rjf: Range Ops

// ==================== 1D Ranges ====================

#define md_r1u32(md_min, md_max) md_rng_1u32((md_min), (md_max))
inline MD_Rng1U32 md_rng_1u32      (MD_U32 md_min, MD_U32 md_max)     { MD_Rng1U32 r = {md_min, md_max}; if(r.md_min > r.md_max) { md_swap(MD_U32, r.md_min, r.md_max); } return r; }
inline MD_Rng1U32 md_shift_1u32    (MD_Rng1U32 r, MD_U32 x)     {         r.md_min += x; r.md_max += x;                                        return r; }
inline MD_Rng1U32 md_pad_1u32      (MD_Rng1U32 r, MD_U32 x)     {         r.md_min -= x; r.md_max += x;                                        return r; }
inline MD_U32     md_center_1u32   (MD_Rng1U32 r)            { MD_U32     c = (r.md_min + r.md_max) / 2;                                       return c; }
inline MD_B32     md_contains_1u32 (MD_Rng1U32 r, MD_U32 x)     { MD_B32     c = (r.md_min <= x && x < r.md_max);                                 return c; }
inline MD_U32     md_dim_1u32      (MD_Rng1U32 r)            { MD_U32     c = r.md_max - r.md_min;                                             return c; }
inline MD_Rng1U32 md_union_1u32    (MD_Rng1U32 a, MD_Rng1U32 b) { MD_Rng1U32 c = {md_min(a.md_min, b.md_min), md_min(a.md_max, b.md_max)};              return c; }
inline MD_Rng1U32 md_intersect_1u32(MD_Rng1U32 a, MD_Rng1U32 b) { MD_Rng1U32 c = {md_max(a.md_min, b.md_min), md_min(a.md_max, b.md_max)};              return c; }
inline MD_U32     md_clamp_1u32    (MD_Rng1U32 r, MD_U32 v)     {         v = md_clamp(r.md_min, v, r.md_max);                                    return v; }

#define md_r1s32(md_min, md_max) md_rng_1s32((md_min), (md_max))
inline MD_Rng1S32 md_rng_1s32      (MD_S32 md_min, MD_S32 md_max)     { MD_Rng1S32 r = {md_min, md_max}; if(r.md_min > r.md_max) { md_swap(MD_S32, r.md_min, r.md_max); } return r; }
inline MD_Rng1S32 md_shift_1s32    (MD_Rng1S32 r, MD_S32 x)     {         r.md_min += x; r.md_max += x;                                        return r; }
inline MD_Rng1S32 md_pad_1s32      (MD_Rng1S32 r, MD_S32 x)     {         r.md_min -= x; r.md_max += x;                                        return r; }
inline MD_S32     md_center_1s32   (MD_Rng1S32 r)            { MD_S32     c = (r.md_min+r.md_max) / 2;                                         return c; }
inline MD_B32     md_contains_1s32 (MD_Rng1S32 r, MD_S32 x)     { MD_B32     c = (r.md_min <= x && x < r.md_max);                                 return c; }
inline MD_S32     md_dim_1s32      (MD_Rng1S32 r)            { MD_S32     c = r.md_max-r.md_min;                                               return c; }
inline MD_Rng1S32 md_union_1s32    (MD_Rng1S32 a, MD_Rng1S32 b) { MD_Rng1S32 c = {md_min(a.md_min, b.md_min), md_max(a.md_max, b.md_max)};              return c; }
inline MD_Rng1S32 md_intersect_1s32(MD_Rng1S32 a, MD_Rng1S32 b) { MD_Rng1S32 c = {md_max(a.md_min, b.md_min), md_min(a.md_max, b.md_max)};              return c; }
inline MD_S32     md_clamp_1s32    (MD_Rng1S32 r, MD_S32 v)     {         v = md_clamp(r.md_min, v, r.md_max);                                    return v; }

#define md_r1u64(md_min, md_max) md_rng_1u64((md_min), (md_max))
inline MD_Rng1U64 md_rng_1u64      (MD_U64 md_min, MD_U64 md_max)     { MD_Rng1U64 r = {md_min, md_max}; if(r.md_min > r.md_max) { md_swap(MD_U64, r.md_min, r.md_max); } return r; }
inline MD_Rng1U64 md_shift_1u64    (MD_Rng1U64 r, MD_U64 x)     {         r.md_min += x; r.md_max += x;                                        return r; }
inline MD_Rng1U64 md_pad_1u64      (MD_Rng1U64 r, MD_U64 x)     {         r.md_min -= x; r.md_max += x;                                        return r; }
inline MD_U64     md_center_1u64   (MD_Rng1U64 r)            { MD_U64     c = (r.md_min + r.md_max)/2;                                         return c; }
inline MD_B32     md_contains_1u64 (MD_Rng1U64 r, MD_U64 x)     { MD_B32     c = (r.md_min <= x && x < r.md_max);                                 return c; }
inline MD_U64     md_dim_1u64      (MD_Rng1U64 r)            { MD_U64     c = r.md_max-r.md_min;                                               return c; }
inline MD_Rng1U64 md_union_1u64    (MD_Rng1U64 a, MD_Rng1U64 b) { MD_Rng1U64 c = {md_min(a.md_min, b.md_min), md_max(a.md_max, b.md_max)};              return c; }
inline MD_Rng1U64 md_intersect_1u64(MD_Rng1U64 a, MD_Rng1U64 b) { MD_Rng1U64 c = {md_max(a.md_min, b.md_min), md_min(a.md_max, b.md_max)};              return c; }
inline MD_U64     md_clamp_1u64    (MD_Rng1U64 r, MD_U64 v)     {         v = md_clamp(r.md_min, v, r.md_max);                                    return v; }

#define md_r1s64(md_min, md_max) md_rng_1s64((md_min), (md_max))
inline Rng1S64 md_rng_1s64      (MD_S64 md_min, MD_S64 md_max)     { Rng1S64 r = {md_min, md_max}; if(r.md_min > r.md_max) { md_swap(MD_S64, r.md_min, r.md_max); } return r; }
inline Rng1S64 md_shift_1s64    (Rng1S64 r, MD_S64 x)     {         r.md_min += x; r.md_max += x;                                        return r; }
inline Rng1S64 md_pad_1s64      (Rng1S64 r, MD_S64 x)     {         r.md_min -= x; r.md_max += x;                                        return r; }
inline MD_S64     md_center_1s64   (Rng1S64 r)            { MD_S64     c = (r.md_min + r.md_max) / 2;                                       return c; }
inline MD_B32     md_contains_1s64 (Rng1S64 r, MD_S64 x)     { MD_B32     c = (r.md_min <= x && x < r.md_max);                                 return c; }
inline MD_S64     md_dim_1s64      (Rng1S64 r)            { MD_S64     c = r.md_max - r.md_min;                                             return c; }
inline Rng1S64 md_union_1s64    (Rng1S64 a, Rng1S64 b) { Rng1S64 c = {md_min(a.md_min, b.md_min), md_max(a.md_max, b.md_max)};              return c; }
inline Rng1S64 md_intersect_1s64(Rng1S64 a, Rng1S64 b) { Rng1S64 c = {md_max(a.md_min, b.md_min), md_min(a.md_max, b.md_max)};              return c; }
inline MD_S64     md_clamp_1s64    (Rng1S64 r, MD_S64 v)     {         v = md_clamp(r.md_min, v, r.md_max);                                    return v;}

#define md_r1f32(md_min, md_max) md_rng_1f32((md_min), (md_max))
inline Rng1F32 md_rng_1f32      (MD_F32 md_min, MD_F32 md_max)     { Rng1F32 r = {md_min, md_max}; if(r.md_min > r.md_max) { md_swap(MD_F32, r.md_min, r.md_max); } return r; }
inline Rng1F32 md_shift_1f32    (Rng1F32 r, MD_F32 x)     {         r.md_min += x; r.md_max += x;                                        return r; }
inline Rng1F32 md_pad_1f32      (Rng1F32 r, MD_F32 x)     {         r.md_min -= x; r.md_max += x;                                        return r; }
inline MD_F32     md_center_1f32   (Rng1F32 r)            { MD_F32     c = (r.md_min + r.md_max) / 2;                                       return c; }
inline MD_B32     md_contains_1f32 (Rng1F32 r, MD_F32 x)     { MD_B32     c = (r.md_min <= x && x < r.md_max);                                 return c; }
inline MD_F32     md_dim_1f32      (Rng1F32 r)            { MD_F32     c = r.md_max - r.md_min;                                             return c; }
inline Rng1F32 md_union_1f32    (Rng1F32 a, Rng1F32 b) { Rng1F32 c = {md_min(a.md_min, b.md_min), md_max(a.md_max, b.md_max)};              return c; }
inline Rng1F32 md_intersect_1f32(Rng1F32 a, Rng1F32 b) { Rng1F32 c = {md_max(a.md_min, b.md_min), md_min(a.md_max, b.md_max)};              return c; }
inline MD_F32     md_clamp_1f32    (Rng1F32 r, MD_F32 v)     {         v = md_clamp(r.md_min, v, r.md_max);                                    return v; }

// ==================== 2D Ranges ====================

#define md_r2s16(md_min, md_max) md_rng_2s16((md_min), (md_max))
#define md_r2s16p(x, y, z, w) md_r2s16(md_v2s16((x), (y)), md_v2s16((z), (w)))
inline MD_Rng2S16 md_rng_2s16      (MD_Vec2S16 md_min, MD_Vec2S16 md_max) { MD_Rng2S16 r = {md_min, md_max};                                                            return r; }
inline MD_Rng2S16 md_shift_2s16    (MD_Rng2S16 r, MD_Vec2S16 x)     {         r.md_min = md_add_2s16(r.md_min, x); r.md_max = md_add_2s16(r.md_max, x);                    return r; }
inline MD_Rng2S16 md_pad_2s16      (MD_Rng2S16 r, MD_S16 x)         { MD_Vec2S16 xv = {x, x}; r.md_min = md_sub_2s16(r.md_min, xv); r.md_max = md_add_2s16(r.md_max, xv);     return r; }
inline MD_Vec2S16 md_center_2s16   (MD_Rng2S16 r)                { MD_Vec2S16 c = {(MD_S16)((r.md_min.x + r.md_max.x) / 2), (MD_S16)((r.md_min.y + r.md_max.y) / 2)};      return c; }
inline MD_B32     md_contains_2s16 (MD_Rng2S16 r, MD_Vec2S16 x)     { MD_B32     c = (r.md_min.x <= x.x && x.x < r.md_max.x && r.md_min.y <= x.y && x.y < r.md_max.y);  return c; }
inline MD_Vec2S16 md_dim_2s16      (MD_Rng2S16 r)                { MD_Vec2S16 dim = {(MD_S16)(r.md_max.x - r.md_min.x), (MD_S16)(r.md_max.y - r.md_min.y)};                return dim; }
inline MD_Rng2S16 md_union_2s16    (MD_Rng2S16 a, MD_Rng2S16 b)     { MD_Rng2S16 c; c.p0.x = md_min(a.md_min.x, b.md_min.x); c.p0.y = md_min(a.md_min.y, b.md_min.y); c.p1.x = md_max(a.md_max.x, b.md_max.x); c.p1.y = md_max(a.md_max.y, b.md_max.y); return c; }
inline MD_Rng2S16 md_intersect_2s16(MD_Rng2S16 a, MD_Rng2S16 b)     { MD_Rng2S16 c; c.p0.x = md_max(a.md_min.x, b.md_min.x); c.p0.y = md_max(a.md_min.y, b.md_min.y); c.p1.x = md_min(a.md_max.x, b.md_max.x); c.p1.y = md_min(a.md_max.y, b.md_max.y); return c; }
inline MD_Vec2S16 md_clamp_2s16    (MD_Rng2S16 r, MD_Vec2S16 v)     {         v.x = md_clamp(r.md_min.x, v.x, r.md_max.x); v.y = md_clamp(r.md_min.y, v.y, r.md_max.y); return v; }

#define md_r2s32(md_min, md_max) md_rng_2s32((md_min), (md_max))
#define md_r2s32p(x, y, z, w) md_r2s32(md_v2s32((x), (y)), md_v2s32((z), (w)))
inline MD_Rng2S32 md_rng_2s32      (MD_Vec2S32 md_min, MD_Vec2S32 md_max) { MD_Rng2S32 r = {md_min, md_max};                                                           return r; }
inline MD_Rng2S32 md_shift_2s32    (MD_Rng2S32 r, MD_Vec2S32 x)     {         r.md_min = md_add_2s32(r.md_min, x); r.md_max = md_add_2s32(r.md_max, x);                   return r; }
inline MD_Rng2S32 md_pad_2s32      (MD_Rng2S32 r, MD_S32 x)         { MD_Vec2S32 xv = {x, x}; r.md_min = md_sub_2s32(r.md_min, xv); r.md_max = md_add_2s32(r.md_max, xv);    return r; }
inline MD_Vec2S32 md_center_2s32   (MD_Rng2S32 r)                { MD_Vec2S32 c = {(r.md_min.x + r.md_max.x) / 2, (r.md_min.y + r.md_max.y) / 2};                   return c; }
inline MD_B32     md_contains_2s32 (MD_Rng2S32 r, MD_Vec2S32 x)     { MD_B32     c = (r.md_min.x <= x.x && x.x < r.md_max.x && r.md_min.y <= x.y && x.y < r.md_max.y); return c; }
inline MD_Vec2S32 md_dim_2s32      (MD_Rng2S32 r)                { MD_Vec2S32 dim = {r.md_max.x - r.md_min.x, r.md_max.y - r.md_min.y};                             return dim; }
inline MD_Rng2S32 md_union_2s32    (MD_Rng2S32 a, MD_Rng2S32 b)     { MD_Rng2S32 c; c.p0.x = md_min(a.md_min.x, b.md_min.x); c.p0.y = md_min(a.md_min.y, b.md_min.y); c.p1.x = md_max(a.md_max.x, b.md_max.x); c.p1.y = md_max(a.md_max.y, b.md_max.y); return c; }
inline MD_Rng2S32 md_intersect_2s32(MD_Rng2S32 a, MD_Rng2S32 b)     { MD_Rng2S32 c; c.p0.x = md_max(a.md_min.x, b.md_min.x); c.p0.y = md_max(a.md_min.y, b.md_min.y); c.p1.x = md_min(a.md_max.x, b.md_max.x); c.p1.y = md_min(a.md_max.y, b.md_max.y); return c; }
inline MD_Vec2S32 md_clamp_2s32    (MD_Rng2S32 r, MD_Vec2S32 v)     {         v.x = md_clamp(r.md_min.x, v.x, r.md_max.x); v.y = md_clamp(r.md_min.y, v.y, r.md_max.y); return v; }

#define md_r2s64(md_min, md_max) md_rng_2s64((md_min), (md_max))
#define md_r2s64p(x, y, z, w) md_r2s64(md_v2s64((x), (y)), md_v2s64((z), (w)))
inline MD_Rng2S64 md_rng_2s64      (MD_Vec2S64 md_min, MD_Vec2S64 md_max) { MD_Rng2S64 r = {md_min, md_max};                                                           return r; }
inline MD_Rng2S64 md_shift_2s64    (MD_Rng2S64 r, MD_Vec2S64 x)     {         r.md_min = md_add_2s64(r.md_min, x); r.md_max = md_add_2s64(r.md_max, x);                   return r; }
inline MD_Rng2S64 md_pad_2s64      (MD_Rng2S64 r, MD_S64 x)         { MD_Vec2S64 xv = {x, x}; r.md_min = md_sub_2s64(r.md_min, xv); r.md_max = md_add_2s64(r.md_max, xv);    return r; }
inline MD_Vec2S64 md_center_2s64   (MD_Rng2S64 r)                { MD_Vec2S64 c = {(r.md_min.x + r.md_max.x) / 2, (r.md_min.y + r.md_max.y) / 2};                   return c; }
inline MD_B32     md_contains_2s64 (MD_Rng2S64 r, MD_Vec2S64 x)     { MD_B32     c = (r.md_min.x <= x.x && x.x < r.md_max.x && r.md_min.y <= x.y && x.y < r.md_max.y); return c; }
inline MD_Vec2S64 md_dim_2s64      (MD_Rng2S64 r)                { MD_Vec2S64 dim = {r.md_max.x - r.md_min.x, r.md_max.y - r.md_min.y};                             return dim; }
inline MD_Rng2S64 md_union_2s64    (MD_Rng2S64 a, MD_Rng2S64 b)     { MD_Rng2S64 c; c.p0.x = md_min(a.md_min.x, b.md_min.x); c.p0.y = md_min(a.md_min.y, b.md_min.y); c.p1.x = md_max(a.md_max.x, b.md_max.x); c.p1.y = md_max(a.md_max.y, b.md_max.y); return c; }
inline MD_Rng2S64 md_intersect_2s64(MD_Rng2S64 a, MD_Rng2S64 b)     { MD_Rng2S64 c; c.p0.x = md_max(a.md_min.x, b.md_min.x); c.p0.y = md_max(a.md_min.y, b.md_min.y); c.p1.x = md_min(a.md_max.x, b.md_max.x); c.p1.y = md_min(a.md_max.y, b.md_max.y); return c; }
inline MD_Vec2S64 md_clamp_2s64    (MD_Rng2S64 r, MD_Vec2S64 v)     {         v.x = md_clamp(r.md_min.x, v.x, r.md_max.x); v.y = md_clamp(r.md_min.y, v.y, r.md_max.y); return v; }

////////////////////////////////
//~ rjf: Miscellaneous Ops

MD_Vec3F32 md_hsv_from_rgb  (MD_Vec3F32 rgb);
MD_Vec3F32 md_rgb_from_hsv  (MD_Vec3F32 hsv);
MD_Vec4F32 md_hsva_from_rgba(MD_Vec4F32 rgba);
MD_Vec4F32 md_rgba_from_hsva(MD_Vec4F32 hsva);
MD_Vec4F32 md_rgba_from_u32 (MD_U32     hex);
MD_U32     md_u32_from_rgba (MD_Vec4F32 rgba);

inline MD_Vec3F32
md_hsv_from_rgb(MD_Vec3F32 rgb)
{
	MD_F32 c_max   = md_max(rgb.x, md_max(rgb.y, rgb.z));
	MD_F32 c_min   = md_min(rgb.x, md_min(rgb.y, rgb.z));
	MD_F32 delta   = c_max - c_min;
	MD_F32 q_delta = 1.0f / delta;
	MD_F32 h = ( 
		(delta == 0.f)   ? 0.f                                           :
		(c_max == rgb.x) ? md_mod_f32((rgb.y - rgb.z) * q_delta + 6.f, 6.f) :
		(c_max == rgb.y) ? (rgb.z - rgb.x)         * q_delta + 2.f       :
		(c_max == rgb.z) ? (rgb.x - rgb.y)         * q_delta + 4.f       :
		                   0.f
	);
	MD_F32 s = (c_max == 0.f) ? 0.f : (delta / c_max);
	MD_F32 v = c_max;
	MD_Vec3F32 hsv = {h * (1.0f / 6.f), s, v};
	return hsv;
}

inline MD_Vec3F32
md_rgb_from_hsv(MD_Vec3F32 hsv) {
	MD_F32 h = md_mod_f32(hsv.x * 360.f, 360.f);
	MD_F32 s = hsv.y;
	MD_F32 v = hsv.z;
	MD_F32 c = v * s;
	MD_F32 x = c * (1.f - md_abs_f32(md_mod_f32(h / 60.f, 2.f) - 1.f));
	MD_F32 m = v - c;
	MD_F32 r = 0;
	MD_F32 g = 0;
	MD_F32 b = 0;
	if      ((h >= 0.f   && h < 60.f ) || (h >= 360.f && h < 420.f)) { r = c; g = x; b = 0; }
	else if ( h >= 60.f  && h < 120.f)                               { r = x; g = c; b = 0; }
	else if ( h >= 120.f && h < 180.f)                               { r = 0; g = c; b = x; }
	else if ( h >= 180.f && h < 240.f )                              { r = 0; g = x; b = c; }
	else if ( h >= 240.f && h < 300.f )                              { r = x; g = 0; b = c; }
	else if ((h >= 300.f && h <= 360.f) || (h >= -60.f && h <= 0.f)) { r = c; g = 0; b = x; }
	MD_Vec3F32 rgb = {r + m, g + m, b + m};
	return(rgb);
}

inline MD_Vec4F32
md_hsva_from_rgba(MD_Vec4F32 rgba) {
	MD_Vec3F32 rgb  = md_v3f32(rgba.x, rgba.y, rgba.z);
	MD_Vec3F32 hsv  = md_hsv_from_rgb(rgb);
	MD_Vec4F32 hsva = md_v4f32(hsv.x, hsv.y, hsv.z, rgba.w);
	return hsva;
}

inline MD_Vec4F32
md_rgba_from_hsva(MD_Vec4F32 hsva)
{
	MD_Vec3F32 hsv  = md_v3f32(hsva.x, hsva.y, hsva.z);
	MD_Vec3F32 rgb  = md_rgb_from_hsv(hsv);
	MD_Vec4F32 rgba = md_v4f32(rgb.x, rgb.y, rgb.z, hsva.w);
	return rgba;
}

inline MD_Vec4F32
md_rgba_from_u32(MD_U32 hex)
{
	MD_Vec4F32 result = md_v4f32(
		((hex&0xff000000) >> 24) / 255.f,
		((hex&0x00ff0000) >> 16) / 255.f,
		((hex&0x0000ff00) >>  8) / 255.f,
		((hex&0x000000ff) >>  0) / 255.f);
	return result;
}

inline MD_U32
md_u32_from_rgba(MD_Vec4F32 rgba)
{
	MD_U32 result = 0;
	result |= ((MD_U32)((MD_U8)(rgba.x * 255.f))) << 24;
	result |= ((MD_U32)((MD_U8)(rgba.y * 255.f))) << 16;
	result |= ((MD_U32)((MD_U8)(rgba.z * 255.f))) <<  8;
	result |= ((MD_U32)((MD_U8)(rgba.w * 255.f))) <<  0;
	return result;
}

#define md_rgba_from_u32_lit_comp(h)       \
{                                       \
	(((h) & 0xff000000) >> 24) / 255.f, \
	(((h) & 0x00ff0000) >> 16) / 255.f, \
	(((h) & 0x0000ff00) >> 8 ) / 255.f, \
	(((h) & 0x000000ff) >> 0 ) / 255.f  \
}

////////////////////////////////
//~ rjf: List Type Functions

void         md_rng1s64_list_push__arena           (MD_Arena*        arena, MD_Rng1S64List* list, Rng1S64 rng);
void         md_rng1s64_list_push__ainfo           (MD_AllocatorInfo ainfo, MD_Rng1S64List* list, Rng1S64 rng);
MD_Rng1S64Array md_rng1s64_array_from_list_push__arena(MD_Arena*        arena, MD_Rng1S64List* list);
MD_Rng1S64Array md_rng1s64_array_from_list_push__ainfo(MD_AllocatorInfo ainfo, MD_Rng1S64List* list);

#define rng1s64_list_push(allocator, list, rng)       _Generic(allocator, MD_Arena*: md_rng1s64_list_push__arena,            MD_AllocatorInfo: md_rng1s64_list_push__ainfo,            default: md_assert_generic_sel_fail) md_generic_call(allocator, list, rng)
#define md_rng1s64_array_from_list_push(allocator, list) _Generic(allocator, MD_Arena*: md_rng1s64_array_from_list_push__arena, MD_AllocatorInfo: md_rng1s64_array_from_list_push__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, list)

md_force_inline void         md_rng1s64_list_push__arena           (MD_Arena* arena, MD_Rng1S64List* list, Rng1S64 rng) {        md_rng1s64_list_push__ainfo           (md_arena_allocator(arena), list, rng); }
md_force_inline MD_Rng1S64Array md_rng1s64_array_from_list_push__arena(MD_Arena* arena, MD_Rng1S64List* list)              { return md_rng1s64_array_from_list_push__ainfo(md_arena_allocator(arena), list); }

inline void
md_rng1s64_list_alloc(MD_AllocatorInfo ainfo, MD_Rng1S64List* list, Rng1S64 rng) {
	MD_Rng1S64Node* n = md_alloc_array(ainfo, MD_Rng1S64Node, 1);
	md_memory_copy_struct(&n->v, &rng);
	md_sll_queue_push(list->first, list->last, n);
	list->count += 1;
}

inline MD_Rng1S64Array
md_rng1s64_array_from_list_alloc(MD_AllocatorInfo ainfo, MD_Rng1S64List* list) {
	MD_Rng1S64Array 
	arr       = {0};
	arr.count = list->count;
	arr.v     = md_alloc_array_no_zero(ainfo, Rng1S64, arr.count);
	MD_U64 idx   = 0;
	for (MD_Rng1S64Node* n = list->first; n != 0; n = n->next) {
		arr.v[idx] = n->v;
		idx       += 1;
	}
	return arr;
}
