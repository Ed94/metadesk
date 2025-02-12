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

typedef union Vec2F32 Vec2F32;
union Vec2F32
{
	struct
	{
		F32 x;
		F32 y;
	};
	F32 v[2];
};

typedef union Vec2S64 Vec2S64;
union Vec2S64
{
	struct
	{
		S64 x;
		S64 y;
	};
	S64 v[2];
};

typedef union Vec2S32 Vec2S32;
union Vec2S32
{
	struct
	{
		S32 x;
		S32 y;
	};
	S32 v[2];
};

typedef union Vec2S16 Vec2S16;
union Vec2S16
{
	struct
	{
		S16 x;
		S16 y;
	};
	S16 v[2];
};

//- rjf: 3-vectors

typedef union Vec3F32 Vec3F32;
union Vec3F32
{
	struct
	{
		F32 x;
		F32 y;
		F32 z;
	};
	struct
	{
		Vec2F32 xy;
		F32 _z0;
	};
	struct
	{
		F32 _x0;
		Vec2F32 yz;
	};
	F32 v[3];
};

typedef union Vec3S32 Vec3S32;
union Vec3S32
{
	struct
	{
		S32 x;
		S32 y;
		S32 z;
	};
	struct
	{
		Vec2S32 xy;
		S32 _z0;
	};
	struct
	{
		S32 _x0;
		Vec2S32 yz;
	};
	S32 v[3];
};

//- rjf: 4-vectors

typedef union Vec4F32 Vec4F32;
union Vec4F32
{
	struct
	{
		F32 x;
		F32 y;
		F32 z;
		F32 w;
	};
	struct
	{
		Vec2F32 xy;
		Vec2F32 zw;
	};
	struct
	{
		Vec3F32 xyz;
		F32 _z0;
	};
	struct
	{
		F32 _x0;
		Vec3F32 yzw;
	};
	F32 v[4];
};

typedef union Vec4S32 Vec4S32;
union Vec4S32
{
	struct
	{
		S32 x;
		S32 y;
		S32 z;
		S32 w;
	};
	struct
	{
		Vec2S32 xy;
		Vec2S32 zw;
	};
	struct
	{
		Vec3S32 xyz;
		S32 _z0;
	};
	struct
	{
		S32 _x0;
		Vec3S32 yzw;
	};
	S32 v[4];
};

////////////////////////////////
//~ rjf: Matrix Types

typedef struct Mat3x3F32 Mat3x3F32;
struct Mat3x3F32
{
	F32 v[3][3];
};

typedef struct Mat4x4F32 Mat4x4F32;
struct Mat4x4F32
{
	F32 v[4][4];
};

////////////////////////////////
//~ rjf: Range Types

//- rjf: 1-range

typedef union Rng1U32 Rng1U32;
union Rng1U32
{
	struct
	{
		U32 min;
		U32 max;
	};
	U32 v[2];
};

typedef union Rng1S32 Rng1S32;
union Rng1S32
{
	struct
	{
		S32 min;
		S32 max;
	};
	S32 v[2];
};

typedef union Rng1U64 Rng1U64;
union Rng1U64
{
	struct
	{
		U64 min;
		U64 max;
	};
	U64 v[2];
};

typedef union Rng1S64 Rng1S64;
union Rng1S64
{
	struct
	{
		S64 min;
		S64 max;
	};
	S64 v[2];
};

typedef union Rng1F32 Rng1F32;
union Rng1F32
{
	struct
	{
		F32 min;
		F32 max;
	};
	F32 v[2];
};

//- rjf: 2-range (rectangles)

typedef union Rng2S16 Rng2S16;
union Rng2S16
{
	struct
	{
		Vec2S16 min;
		Vec2S16 max;
	};
	struct
	{
		Vec2S16 p0;
		Vec2S16 p1;
	};
	struct
	{
		S16 x0;
		S16 y0;
		S16 x1;
		S16 y1;
	};
	Vec2S16 v[2];
};

typedef union Rng2S32 Rng2S32;
union Rng2S32
{
	struct
	{
		Vec2S32 min;
		Vec2S32 max;
	};
	struct
	{
		Vec2S32 p0;
		Vec2S32 p1;
	};
	struct
	{
		S32 x0;
		S32 y0;
		S32 x1;
		S32 y1;
	};
	Vec2S32 v[2];
};

typedef union Rng2F32 Rng2F32;
union Rng2F32
{
	struct
	{
		Vec2F32 min;
		Vec2F32 max;
	};
	struct
	{
		Vec2F32 p0;
		Vec2F32 p1;
	};
	struct
	{
		F32 x0;
		F32 y0;
		F32 x1;
		F32 y1;
	};
	Vec2F32 v[2];
};

typedef union Rng2S64 Rng2S64;
union Rng2S64
{
	struct
	{
		Vec2S64 min;
		Vec2S64 max;
	};
	struct
	{
		Vec2S64 p0;
		Vec2S64 p1;
	};
	struct
	{
		S64 x0;
		S64 y0;
		S64 x1;
		S64 y1;
	};
	Vec2S64 v[2];
};

////////////////////////////////
//~ rjf: List Types

typedef struct Rng1S64Node Rng1S64Node;
struct Rng1S64Node
{
	Rng1S64Node* next;
	Rng1S64      v;
};

typedef struct Rng1S64List Rng1S64List;
struct Rng1S64List
{
	Rng1S64Node* first;
	Rng1S64Node* last;
	U64 count;
};

typedef struct Rng1S64Array Rng1S64Array;
struct Rng1S64Array
{
	Rng1S64* v;
	U64 count;
};

////////////////////////////////
//~ rjf: Clamps, Mins, Maxes

#ifndef md_min
#define md_min(A, B) (((A) < (B)) ? (A) : (B))
#endif
#ifndef md_max
#define md_max(A, B) (((A) > (B)) ? (A) : (B))
#endif

#ifndef clamp_top
#define clamp_top(A, X) md_min(A, X)
#endif
#ifndef clamp_bot
#define clamp_bot(X, B) md_max(X, B)
#endif

#define clamp(A, X, B) (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))

////////////////////////////////
//~ rjf: Scalar Ops

#define abs_s64(v) (S64)llabs(v)

#define sqrt_f32(v)   sqrtf(v)
#define mod_f32(a, b) fmodf((a), (b))
#define pow_f32(b, e) powf((b), (e))
#define ceil_f32(v)   ceilf(v)
#define floor_f32(v)  floorf(v)
#define round_f32(v)  roundf(v)
#define abs_f32(v)    fabsf(v)

#define radians_from_turns_f32(v) ((v) * 2 * 3.1415926535897f)
#define turns_from_radians_f32(v) ((v) / 2 * 3.1415926535897f)
#define degrees_from_turns_f32(v) ((v) * 360.f)
#define turns_from_degrees_f32(v) ((v) / 360.f)

#define degrees_from_radians_f32(v) (degrees_from_turns_f32(turns_from_radians_f32(v)))
#define radians_from_degrees_f32(v) (radians_from_turns_f32(turns_from_degrees_f32(v)))

#define sin_f32(v)    sinf( radians_from_turns_f32(v) )
#define cos_f32(v)    cosf( radians_from_turns_f32(v) )
#define tan_f32(v)    tanf( radians_from_turns_f32(v) )

#define sqrt_f64(v)   sqrt(v)
#define mod_f64(a, b) fmod((a), (b))
#define pow_f64(b, e) pow((b), (e))
#define ceil_f64(v)   ceil(v)
#define floor_f64(v)  floor(v)
#define round_f64(v)  round(v)
#define abs_f64(v)    fabs(v)

#define radians_from_turns_f64(v) ((v) * 2 * 3.1415926535897)
#define turns_from_radians_f64(v) ((v) / 2 * 3.1415926535897)
#define degrees_from_turns_f64(v) ((v) * 360.0)
#define turns_from_degrees_f64(v) ((v) / 360.0)

#define degrees_from_radians_f64(v) (degrees_from_turns_f64(turns_from_radians_f64(v)))
#define radians_from_degrees_f64(v) (radians_from_turns_f64(turns_from_degrees_f64(v)))

#define sin_f64(v)    sin(radians_from_turns_f64(v))
#define cos_f64(v)    cos(radians_from_turns_f64(v))
#define tan_f64(v)    tan(radians_from_turns_f64(v))

inline F32 mix_1f32(F32 a, F32 b, F32 t) { F32 c = (a + (b - a) * clamp(0.f, t, 1.f)); return c; }
inline F64 mix_1f64(F64 a, F64 b, F64 t) { F64 c = (a + (b - a) * clamp(0.0, t, 1.0)); return c; }

////////////////////////////////
//~ rjf: Vector Ops

// ==================== 2D Vectors ====================

#define v2f32(x, y) vec_2f32((x), (y))
inline Vec2F32 vec_2f32           (F32     x, F32     y)        { Vec2F32 v = { x, y };                                       return v; }
inline Vec2F32 add_2f32           (Vec2F32 a, Vec2F32 b)        { Vec2F32 c = {a.x + b.x, a.y + b.y};                         return c; }
inline Vec2F32 sub_2f32           (Vec2F32 a, Vec2F32 b)        { Vec2F32 c = {a.x - b.x, a.y - b.y};                         return c; }
inline Vec2F32 mul_2f32           (Vec2F32 a, Vec2F32 b)        { Vec2F32 c = {a.x * b.x, a.y * b.y};                         return c; }
inline Vec2F32 div_2f32           (Vec2F32 a, Vec2F32 b)        { Vec2F32 c = {a.x / b.x, a.y / b.y};                         return c; }
inline Vec2F32 scale_2f32         (Vec2F32 v, F32     s)        { Vec2F32 c = {v.x * s,   v.y * s  };                         return c; }
inline F32     dot_2f32           (Vec2F32 a, Vec2F32 b)        { F32     c = a.x * b.x + a.y * b.y;                          return c; }
inline F32     length_squared_2f32(Vec2F32 v)                   { F32     c = v.x * v.x + v.y * v.y;                          return c; }
inline F32     length_2f32        (Vec2F32 v)                   { F32     c = sqrt_f32(v.x*v.x + v.y*v.y);                    return c; }
inline Vec2F32 normalize_2f32     (Vec2F32 v)                   {         v = scale_2f32(v, 1.f / length_2f32(v));            return v; }
inline Vec2F32 mix_2f32           (Vec2F32 a, Vec2F32 b, F32 t) { Vec2F32 c = {mix_1f32(a.x, b.x, t), mix_1f32(a.y, b.y, t)}; return c; }

#define v2s64(x, y) vec_2s64((x), (y))
inline Vec2S64 vec_2s64           (S64     x, S64     y)        { Vec2S64 v = { x, y };                                       return v; }
inline Vec2S64 add_2s64           (Vec2S64 a, Vec2S64 b)        { Vec2S64 c = {a.x + b.x, a.y + b.y};                         return c; }
inline Vec2S64 sub_2s64           (Vec2S64 a, Vec2S64 b)        { Vec2S64 c = {a.x - b.x, a.y - b.y};                         return c; }
inline Vec2S64 mul_2s64           (Vec2S64 a, Vec2S64 b)        { Vec2S64 c = {a.x * b.x, a.y * b.y};                         return c; }
inline Vec2S64 div_2s64           (Vec2S64 a, Vec2S64 b)        { Vec2S64 c = {a.x / b.x, a.y / b.y};                         return c; }
inline Vec2S64 scale_2s64         (Vec2S64 v, S64     s)        { Vec2S64 c = {v.x * s,   v.y * s  };                         return c; }
inline S64     dot_2s64           (Vec2S64 a, Vec2S64 b)        { S64     c = a.x * b.x + a.y * b.y;                          return c; }
inline S64     length_squared_2s64(Vec2S64 v)                   { S64     c = v.x * v.x + v.y * v.y;                          return c; }
inline S64     length_2s64        (Vec2S64 v)                   { S64     c = (S64)sqrt_f64((F64)(v.x*v.x + v.y*v.y));        return c; }
inline Vec2S64 normalize_2s64     (Vec2S64 v)                   {         v = scale_2s64(v, (S64)(1.f / length_2s64(v)));     return v; }
inline Vec2S64 mix_2s64           (Vec2S64 a, Vec2S64 b, F32 t) { Vec2S64 c = {(S64)mix_1f32((F32)a.x, (F32)b.x, t), (S64)mix_1f32((F32)a.y, (F32)b.y, t)}; return c; }

#define v2s32(x, y) vec_2s32((x), (y))
inline Vec2S32 vec_2s32           (S32 x, S32 y)                { Vec2S32 v = { x, y };                                       return v; }
inline Vec2S32 add_2s32           (Vec2S32 a, Vec2S32 b)        { Vec2S32 c = {a.x + b.x, a.y + b.y};                         return c; }
inline Vec2S32 sub_2s32           (Vec2S32 a, Vec2S32 b)        { Vec2S32 c = {a.x - b.x, a.y - b.y};                         return c; }
inline Vec2S32 mul_2s32           (Vec2S32 a, Vec2S32 b)        { Vec2S32 c = {a.x * b.x, a.y * b.y};                         return c; }
inline Vec2S32 div_2s32           (Vec2S32 a, Vec2S32 b)        { Vec2S32 c = {a.x / b.x, a.y / b.y};                         return c; }
inline Vec2S32 scale_2s32         (Vec2S32 v, S32     s)        { Vec2S32 c = {v.x * s,   v.y * s  };                         return c; }
inline S32     dot_2s32           (Vec2S32 a, Vec2S32 b)        { S32     c = a.x * b.x + a.y * b.y;                          return c; }
inline S32     length_squared_2s32(Vec2S32 v)                   { S32     c = v.x * v.x + v.y * v.y;                          return c; }
inline S32     length_2s32        (Vec2S32 v)                   { S32     c = (S32)sqrt_f32((F32)v.x*(F32)v.x + (F32)v.y*(F32)v.y); return c; }
inline Vec2S32 normalize_2s32     (Vec2S32 v)                   {         v = scale_2s32(v, (S32)(1.f / length_2s32(v)));     return v; }
inline Vec2S32 mix_2s32           (Vec2S32 a, Vec2S32 b, F32 t) { Vec2S32 c = {(S32)mix_1f32((F32)a.x, (F32)b.x, t), (S32)mix_1f32((F32)a.y, (F32)b.y, t)}; return c; }

#define v2s16(x, y) vec_2s16((x), (y))
inline Vec2S16 vec_2s16           (S16 x, S16 y)                { Vec2S16 v = { x, y };                                       return v; }
inline Vec2S16 add_2s16           (Vec2S16 a, Vec2S16 b)        { Vec2S16 c = {(S16)(a.x + b.x), (S16)(a.y + b.y)};           return c; }
inline Vec2S16 sub_2s16           (Vec2S16 a, Vec2S16 b)        { Vec2S16 c = {(S16)(a.x - b.x), (S16)(a.y - b.y)};           return c; }
inline Vec2S16 mul_2s16           (Vec2S16 a, Vec2S16 b)        { Vec2S16 c = {(S16)(a.x * b.x), (S16)(a.y * b.y)};           return c; }
inline Vec2S16 div_2s16           (Vec2S16 a, Vec2S16 b)        { Vec2S16 c = {(S16)(a.x / b.x), (S16)(a.y / b.y)};           return c; }
inline Vec2S16 scale_2s16         (Vec2S16 v, S16     s)        { Vec2S16 c = {(S16)(v.x * s  ), (S16)(v.y * s  )};           return c; }
inline S16     dot_2s16           (Vec2S16 a, Vec2S16 b)        { S16     c = a.x * b.x + a.y * b.y;                          return c; }
inline S16     length_squared_2s16(Vec2S16 v)                   { S16     c = v.x * v.x + v.y * v.y;                          return c; }
inline S16     length_2s16        (Vec2S16 v)                   { S16     c = (S16)sqrt_f32((F32)(v.x*v.x + v.y*v.y));        return c; }
inline Vec2S16 normalize_2s16     (Vec2S16 v)                   {         v = scale_2s16(v, (S16)(1.f / length_2s16(v)));     return v; }
inline Vec2S16 mix_2s16           (Vec2S16 a, Vec2S16 b, F32 t) { Vec2S16 c = {(S16)mix_1f32((F32)a.x, (F32)b.x, t), (S16)mix_1f32((F32)a.y, (F32)b.y, t)}; return c; }

#define vec2(a, b)             _Generic(a, S16: vec_2s16,            S32: vec_2s32,            S64: vec_2s64,            F32: vec_2f32           )((a), (b))
#define add_vec2(a, b)         _Generic(a, S16: add_2s16,            S32: add_2s32,            S64: add_2s64,            F32: add_2f32           )((a), (b))
#define sub_vec2(a, b)         _Generic(a, S16: sub_2s16,            S32: sub_2s32,            S64: sub_2s64,            F32: sub_2f32           )((a), (b))
#define mul_vec2(a, b)         _Generic(a, S16: mul_2s16,            S32: mul_2s32,            S64: mul_2s64,            F32: mul_2f32           )((a), (b))
#define div_vec2(a, b)         _Generic(a, S16: div_2s16,            S32: div_2s32,            S64: div_2s64,            F32: div_2f32           )((a), (b))
#define scale_vec2(v, s)       _Generic(v, S16: scale_2s16,          S32: scale_2s32,          S64: scale_2s64,          F32: scale_2f32         )((v), (s))
#define dot_vec2(a, b)         _Generic(a, S16: dot_2s16,            S32: dot_2s32,            S64: dot_2s64,            F32: dot_2f32           )((a), (b))
#define length_squared_vec2(v) _Generic(v, S16: length_squared_2s16, S32: length_squared_2s32, S64: length_squared_2s64, F32: length_squared_2f32)((v))
#define length_vec2(v)         _Generic(v, S16: length_2s16,         S32: length_2s32,         S64: length_2s64,         F32: length_2f32        )((v))
#define normalize_vec2(v)      _Generic(v, S16: normalize_2s16,      S32: normalize_2s32,      S64: normalize_2s64,      F32: normalize_2f32     )((v))
#define mix_vec2(a, b, t)      _Generic(a, S16: mix_2s16,            S32: mix_2s32,            S64: mix_2s64,            F32: mix_2f32           )((a), (b), (t))

// ==================== 3D Vectors ====================

#define v3f32(x, y, z) vec_3f32((x), (y), (z))
inline Vec3F32 vec_3f32           (F32 x, F32 y, F32 z)         { Vec3F32 v = {x, y, z}; return v; }
inline Vec3F32 add_3f32           (Vec3F32 a, Vec3F32 b)        { Vec3F32 c = {a.x + b.x, a.y + b.y, a.z + b.z};           return c; }
inline Vec3F32 sub_3f32           (Vec3F32 a, Vec3F32 b)        { Vec3F32 c = {a.x - b.x, a.y - b.y, a.z - b.z};           return c; }
inline Vec3F32 mul_3f32           (Vec3F32 a, Vec3F32 b)        { Vec3F32 c = {a.x * b.x, a.y * b.y, a.z * b.z};           return c; }
inline Vec3F32 div_3f32           (Vec3F32 a, Vec3F32 b)        { Vec3F32 c = {a.x / b.x, a.y / b.y, a.z / b.z};           return c; }
inline Vec3F32 scale_3f32         (Vec3F32 v, F32     s)        { Vec3F32 c = {v.x * s,   v.y * s,   v.z * s};             return c; }
inline F32     dot_3f32           (Vec3F32 a, Vec3F32 b)        { F32     c = a.x * b.x + a.y * b.y + a.z * b.z;           return c; }
inline F32     length_squared_3f32(Vec3F32 v)                   { F32     c = v.x * v.x + v.y * v.y + v.z * v.z;           return c; }
inline F32     length_3f32        (Vec3F32 v)                   { F32     c = sqrt_f32(v.x * v.x + v.y * v.y + v.z * v.z); return c; }
inline Vec3F32 normalize_3f32     (Vec3F32 v)                   {         v = scale_3f32(v, 1.f / length_3f32(v));         return v; }
inline Vec3F32 mix_3f32           (Vec3F32 a, Vec3F32 b, F32 t) { Vec3F32 c = {mix_1f32(a.x, b.x, t), mix_1f32(a.y, b.y, t), mix_1f32(a.z, b.z, t)}; return c; }
inline Vec3F32 cross_3f32         (Vec3F32 a, Vec3F32 b)        { Vec3F32 c = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; return c; }

#define v3s32(x, y, z) vec_3s32((x), (y), (z))
inline Vec3S32 vec_3s32           (S32 x, S32 y, S32 z)         { Vec3S32 v = {x, y, z}; return v; }
inline Vec3S32 add_3s32           (Vec3S32 a, Vec3S32 b)        { Vec3S32 c = {a.x + b.x, a.y + b.y, a.z + b.z};                       return c; }
inline Vec3S32 sub_3s32           (Vec3S32 a, Vec3S32 b)        { Vec3S32 c = {a.x - b.x, a.y - b.y, a.z - b.z};                       return c; }
inline Vec3S32 mul_3s32           (Vec3S32 a, Vec3S32 b)        { Vec3S32 c = {a.x * b.x, a.y * b.y, a.z * b.z};                       return c; }
inline Vec3S32 div_3s32           (Vec3S32 a, Vec3S32 b)        { Vec3S32 c = {a.x / b.x, a.y / b.y, a.z / b.z};                       return c; }
inline Vec3S32 scale_3s32         (Vec3S32 v, S32     s)        { Vec3S32 c = {v.x * s,   v.y * s,   v.z * s  };                       return c; }
inline S32     dot_3s32           (Vec3S32 a, Vec3S32 b)        { S32     c = a.x * b.x + a.y * b.y + a.z * b.z;                       return c; }
inline S32     length_squared_3s32(Vec3S32 v)                   { S32     c = v.x * v.x + v.y * v.y + v.z * v.z;                       return c; }
inline S32     length_3s32        (Vec3S32 v)                   { S32     c = (S32)sqrt_f32((F32)(v.x * v.x + v.y * v.y + v.z * v.z)); return c; }
inline Vec3S32 normalize_3s32     (Vec3S32 v)                   {         v = scale_3s32(v, (S32)(1.f / length_3s32(v)));              return v; }
inline Vec3S32 mix_3s32           (Vec3S32 a, Vec3S32 b, F32 t) { Vec3S32 c = {(S32)mix_1f32((F32)a.x, (F32)b.x, t), (S32)mix_1f32((F32)a.y, (F32)b.y, t), (S32)mix_1f32((F32)a.z, (F32)b.z, t)}; return c; }
inline Vec3S32 cross_3s32         (Vec3S32 a, Vec3S32 b)        { Vec3S32 c = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; return c; }

#define vec3(a, b, c)          _Generic(a, S32: vec_3s32,            F32: vec_3f32           )((a), (b), (c))
#define add_vec3(a, b)         _Generic(a, S32: add_3s32,            F32: add_3f32           )((a), (b))
#define sub_vec3(a, b)         _Generic(a, S32: sub_3s32,            F32: sub_3f32           )((a), (b))
#define mul_vec3(a, b)         _Generic(a, S32: mul_3s32,            F32: mul_3f32           )((a), (b))
#define div_vec3(a, b)         _Generic(a, S32: div_3s32,            F32: div_3f32           )((a), (b))
#define scale_vec3(v, s)       _Generic(v, S32: scale_3s32,          F32: scale_3f32         )((v), (s))
#define dot_vec3(a, b)         _Generic(a, S32: dot_3s32,            F32: dot_3f32           )((a), (b))
#define length_squared_vec3(v) _Generic(v, S32: length_squared_3s32, F32: length_squared_3f32)((v))
#define length_vec3(v)         _Generic(v, S32: length_3s32,         F32: length_3f32        )((v))
#define normalize_vec3(v)      _Generic(v, S32: normalize_3s32,      F32: normalize_3f32     )((v))
#define mix_vec3(a, b, t)      _Generic(a, S32: mix_3s32,            F32: mix_3f32           )((a), (b), (t))
#define cross_vec3(a, b)       _Generic(a, S32: cross_3s32,          F32: cross_3f32         )((a), (b))

// ==================== 4D Vectors ====================

#define v4f32(x, y, z, w) vec_4f32((x), (y), (z), (w))
inline Vec4F32 vec_4f32           (F32 x, F32 y, F32 z, F32 w)  { Vec4F32 v = {x, y, z, w}; return v; }
inline Vec4F32 add_4f32           (Vec4F32 a, Vec4F32 b)        { Vec4F32 c = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};            return c; }
inline Vec4F32 sub_4f32           (Vec4F32 a, Vec4F32 b)        { Vec4F32 c = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};            return c; }
inline Vec4F32 mul_4f32           (Vec4F32 a, Vec4F32 b)        { Vec4F32 c = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};            return c; }
inline Vec4F32 div_4f32           (Vec4F32 a, Vec4F32 b)        { Vec4F32 c = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};            return c; }
inline Vec4F32 scale_4f32         (Vec4F32 v, F32     s)        { Vec4F32 c = {v.x * s,   v.y * s,   v.z * s,   v.w * s  };            return c; }
inline F32     dot_4f32           (Vec4F32 a, Vec4F32 b)        { F32     c = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;           return c; }
inline F32     length_squared_4f32(Vec4F32 v)                   { F32     c = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;           return c; }
inline F32     length_4f32        (Vec4F32 v)                   { F32     c = sqrt_f32(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); return c; }
inline Vec4F32 normalize_4f32     (Vec4F32 v)                   {         v = scale_4f32(v, 1.f / length_4f32(v));                     return v; }
inline Vec4F32 mix_4f32           (Vec4F32 a, Vec4F32 b, F32 t) { Vec4F32 c = {mix_1f32(a.x, b.x, t), mix_1f32(a.y, b.y, t), mix_1f32(a.z, b.z, t), mix_1f32(a.w, b.w, t)}; return c; }

#define v4s32(x, y, z, w) vec_4s32((x), (y), (z), (w))
inline Vec4S32 vec_4s32           (S32 x, S32 y, S32 z, S32 w)  { Vec4S32 v = {x, y, z, w}; return v; }
inline Vec4S32 add_4s32           (Vec4S32 a, Vec4S32 b)        { Vec4S32 c = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};                        return c; }
inline Vec4S32 sub_4s32           (Vec4S32 a, Vec4S32 b)        { Vec4S32 c = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};                        return c; }
inline Vec4S32 mul_4s32           (Vec4S32 a, Vec4S32 b)        { Vec4S32 c = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};                        return c; }
inline Vec4S32 div_4s32           (Vec4S32 a, Vec4S32 b)        { Vec4S32 c = {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};                        return c; }
inline Vec4S32 scale_4s32         (Vec4S32 v, S32 s)            { Vec4S32 c = {v.x * s,   v.y * s,   v.z * s,   v.w * s  };                        return c; }
inline S32     dot_4s32           (Vec4S32 a, Vec4S32 b)        { S32     c = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;                       return c; }
inline S32     length_squared_4s32(Vec4S32 v)                   { S32     c = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;                       return c; }
inline S32     length_4s32        (Vec4S32 v)                   { S32     c = (S32)sqrt_f32((F32)(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w)); return c; }
inline Vec4S32 normalize_4s32     (Vec4S32 v)                   {         v = scale_4s32(v, (S32)(1.f / length_4s32(v)));                          return v; }
inline Vec4S32 mix_4s32           (Vec4S32 a, Vec4S32 b, F32 t) { Vec4S32 c = {(S32)mix_1f32((F32)a.x, (F32)b.x, t), (S32)mix_1f32((F32)a.y, (F32)b.y, t), (S32)mix_1f32((F32)a.z, (F32)b.z, t), (S32)mix_1f32((F32)a.w, (F32)b.w, t)}; return c; }

#define vec4(a, b, c, d)       _Generic(a, S32: vec_4s32,            F32: vec_4f32           )((a), (b), (c), (d))
#define add_vec4(a, b)         _Generic(a, S32: add_4s32,            F32: add_4f32           )((a), (b))
#define sub_vec4(a, b)         _Generic(a, S32: sub_4s32,            F32: sub_4f32           )((a), (b))
#define mul_vec4(a, b)         _Generic(a, S32: mul_4s32,            F32: mul_4f32           )((a), (b))
#define div_vec4(a, b)         _Generic(a, S32: div_4s32,            F32: div_4f32           )((a), (b))
#define scale_vec4(v, s)       _Generic(v, S32: scale_4s32,          F32: scale_4f32         )((v), (s))
#define dot_vec4(a, b)         _Generic(a, S32: dot_4s32,            F32: dot_4f32           )((a), (b))
#define length_squared_vec4(v) _Generic(v, S32: length_squared_4s32, F32: length_squared_4f32)((v))
#define length_vec4(v)         _Generic(v, S32: length_4s32,         F32: length_4f32        )((v))
#define normalize_vec4(v)      _Generic(v, S32: normalize_4s32,      F32: normalize_4f32     )((v))
#define mix_vec4(a, b, t)      _Generic(a, S32: mix_4s32,            F32: mix_4f32           )((a), (b), (t))

////////////////////////////////
//~ rjf: Matrix Ops

// ==================== 3x3 Matrix ====================

inline Mat3x3F32 mat_3x3f32           (F32     diag)             { Mat3x3F32 res = {0};             res.v[0][0] = diag;    res.v[1][1] = diag;    res.v[2][2] = diag; return res; }
inline Mat3x3F32 make_translate_3x3f32(Vec2F32 delta)            { Mat3x3F32 mat = mat_3x3f32(1.f); mat.v[2][0] = delta.x; mat.v[2][1] = delta.y;                     return mat; }
inline Mat3x3F32 make_scale_3x3f32    (Vec2F32 scale)            { Mat3x3F32 mat = mat_3x3f32(1.f); mat.v[0][0] = scale.x; mat.v[1][1] = scale.y;                     return mat; }
inline Mat3x3F32 mul_3x3f32           (Mat3x3F32 a, Mat3x3F32 b) { Mat3x3F32 c = {0}; for(int j = 0; j < 3; j += 1) for(int i = 0; i < 3; i += 1) { c.v[i][j] = (a.v[0][j]*b.v[i][0] + a.v[1][j] * b.v[i][1] + a.v[2][j] * b.v[i][2]); } return c; }

inline Mat4x4F32 mat_4x4f32           (F32     diag)  { Mat4x4F32 res = {0};             res.v[0][0] = diag;    res.v[1][1] = diag;    res.v[2][2] = diag;    res.v[3][3] = diag; return res; }
inline Mat4x4F32 make_translate_4x4f32(Vec3F32 delta) { Mat4x4F32 res = mat_4x4f32(1.f); res.v[3][0] = delta.x; res.v[3][1] = delta.y; res.v[3][2] = delta.z;                     return res; }
inline Mat4x4F32 make_scale_4x4f32    (Vec3F32 scale) { Mat4x4F32 res = mat_4x4f32(1.f); res.v[0][0] = scale.x; res.v[1][1] = scale.y; res.v[2][2] = scale.z;                     return res; }

// ==================== 4x4 Matrix ====================

Mat4x4F32 make_perspective_4x4f32 (F32 fov,  F32 aspect_ratio,               F32 near_z, F32 far_z);
Mat4x4F32 make_orthographic_4x4f32(F32 left, F32 right, F32 bottom, F32 top, F32 near_z, F32 far_z);
Mat4x4F32 make_look_at_4x4f32     (Vec3F32 eye,  Vec3F32 center, Vec3F32 up);
Mat4x4F32 make_rotate_4x4f32      (Vec3F32 axis, F32 turns);

Mat4x4F32 mul_4x4f32     (Mat4x4F32 a, Mat4x4F32 b);
Mat4x4F32 scale_4x4f32   (Mat4x4F32 m, F32       scale);
Mat4x4F32 inverse_4x4f32 (Mat4x4F32 m);
Mat4x4F32 derotate_4x4f32(Mat4x4F32 mat);

inline Mat4x4F32 
make_perspective_4x4f32(F32 fov, F32 aspect_ratio, F32 near_z, F32 far_z) {
	F32 tan_theta_over_2   = tan_f32(fov / 2);
	F32 q_tan_theta_over_2 = 1 / tan_theta_over_2;
	F32 q_near_far_z       = 1 / (near_z - far_z);
	Mat4x4F32 
	result         = mat_4x4f32(1.f);
	result.v[0][0] =                q_tan_theta_over_2;
	result.v[1][1] = aspect_ratio * q_tan_theta_over_2;
	result.v[2][3] = 1.f;
	result.v[2][2] = -(      near_z + far_z) * q_near_far_z;
	result.v[3][2] =  (2.f * near_z * far_z) * q_near_far_z;
	result.v[3][3] = 0.f;
	return result;
}

inline Mat4x4F32 
make_orthographic_4x4f32(F32 left, F32 right, F32 bottom, F32 top, F32 near_z, F32 far_z) {
	Mat4x4F32 result = mat_4x4f32(1.f);
	
	result.v[0][0] = 2.f / (right - left);
	result.v[1][1] = 2.f / (top   - bottom);
	result.v[2][2] = 2.f / (far_z - near_z);
	result.v[3][3] = 1.f;
	
	result.v[3][0] = (left   + right) / (left   - right);
	result.v[3][1] = (bottom + top  ) / (bottom - top  );
	result.v[3][2] = (near_z + far_z) / (near_z - far_z);
	
	return result;
}

inline Mat4x4F32 
make_look_at_4x4f32(Vec3F32 eye, Vec3F32 center, Vec3F32 up) {
	Mat4x4F32 result;
	Vec3F32 f = normalize_3f32(sub_3f32(eye, center));
	Vec3F32 s = normalize_3f32(cross_3f32(f, up));
	Vec3F32 u = cross_3f32(s, f);
	result.v[0][0] =  s.x;              result.v[0][1] =  u.x;              result.v[0][2] = -f.x;              result.v[0][3] = 0.0f; 
	result.v[1][0] =  s.y;              result.v[1][1] =  u.y;              result.v[1][2] = -f.y;              result.v[1][3] = 0.0f;
	result.v[2][0] =  s.z;              result.v[2][1] =  u.z;              result.v[2][2] = -f.z;              result.v[2][3] = 0.0f;
	result.v[3][0] = -dot_3f32(s, eye); result.v[3][1] = -dot_3f32(u, eye); result.v[3][2] =  dot_3f32(f, eye); result.v[3][3] = 1.0f;
	return result;
}

inline Mat4x4F32 
make_rotate_4x4f32(Vec3F32 axis, F32 turns) {
		axis       = normalize_3f32(axis);
	F32 sin_theta  = sin_f32(turns);
	F32 cos_theta  = cos_f32(turns);
	F32 cos_value  = 1.f - cos_theta;
	F32 mul_x_sint = axis.x * sin_theta;
	F32 mul_y_sint = axis.y * sin_theta;
	F32 mul_z_sint = axis.z * sin_theta;
	F32 mul_xx_cos = axis.x * axis.x * cos_value;
	F32 mul_yy_cos = axis.y * axis.y * cos_value;
	F32 mul_zz_cos = axis.z * axis.z * cos_value;
	F32 mul_xy_cos = axis.x * axis.y * cos_value;
	F32 mul_xz_cos = axis.x * axis.z * cos_value;
	F32 mul_yz_cos = axis.y * axis.z * cos_value;
	Mat4x4F32 
	result         = mat_4x4f32(1.f);
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

inline Mat4x4F32
mul_4x4f32(Mat4x4F32 a, Mat4x4F32 b) { 
	Mat4x4F32 c = {0}; 
	for(int j = 0; j < 4; j += 1) 
	for(int i = 0; i < 4; i += 1) { 
		c.v[i][j] = (a.v[0][j] * b.v[i][0] + a.v[1][j] * b.v[i][1] + a.v[2][j] * b.v[i][2] + a.v[3][j] * b.v[i][3]);
	} 
	return c; 
} 

inline Mat4x4F32
scale_4x4f32(Mat4x4F32 m, F32 scale) { 
	for(int j = 0; j < 4; j += 1)
	for(int i = 0; i < 4; i += 1) { 
		m.v[i][j] *= scale;
	} 
	return m;
}

inline Mat4x4F32
inverse_4x4f32(Mat4x4F32 m) {
	F32 coef00 = m.v[2][2] * m.v[3][3] - m.v[3][2] * m.v[2][3];
	F32 coef02 = m.v[1][2] * m.v[3][3] - m.v[3][2] * m.v[1][3];
	F32 coef03 = m.v[1][2] * m.v[2][3] - m.v[2][2] * m.v[1][3];
	F32 coef04 = m.v[2][1] * m.v[3][3] - m.v[3][1] * m.v[2][3];
	F32 coef06 = m.v[1][1] * m.v[3][3] - m.v[3][1] * m.v[1][3];
	F32 coef07 = m.v[1][1] * m.v[2][3] - m.v[2][1] * m.v[1][3];
	F32 coef08 = m.v[2][1] * m.v[3][2] - m.v[3][1] * m.v[2][2];
	F32 coef10 = m.v[1][1] * m.v[3][2] - m.v[3][1] * m.v[1][2];
	F32 coef11 = m.v[1][1] * m.v[2][2] - m.v[2][1] * m.v[1][2];
	F32 coef12 = m.v[2][0] * m.v[3][3] - m.v[3][0] * m.v[2][3];
	F32 coef14 = m.v[1][0] * m.v[3][3] - m.v[3][0] * m.v[1][3];
	F32 coef15 = m.v[1][0] * m.v[2][3] - m.v[2][0] * m.v[1][3];
	F32 coef16 = m.v[2][0] * m.v[3][2] - m.v[3][0] * m.v[2][2];
	F32 coef18 = m.v[1][0] * m.v[3][2] - m.v[3][0] * m.v[1][2];
	F32 coef19 = m.v[1][0] * m.v[2][2] - m.v[2][0] * m.v[1][2];
	F32 coef20 = m.v[2][0] * m.v[3][1] - m.v[3][0] * m.v[2][1];
	F32 coef22 = m.v[1][0] * m.v[3][1] - m.v[3][0] * m.v[1][1];
	F32 coef23 = m.v[1][0] * m.v[2][1] - m.v[2][0] * m.v[1][1];
	
	Vec4F32 fac0 = { coef00, coef00, coef02, coef03 };
	Vec4F32 fac1 = { coef04, coef04, coef06, coef07 };
	Vec4F32 fac2 = { coef08, coef08, coef10, coef11 };
	Vec4F32 fac3 = { coef12, coef12, coef14, coef15 };
	Vec4F32 fac4 = { coef16, coef16, coef18, coef19 };
	Vec4F32 fac5 = { coef20, coef20, coef22, coef23 };
	
	Vec4F32 vec0 = { m.v[1][0], m.v[0][0], m.v[0][0], m.v[0][0] };
	Vec4F32 vec1 = { m.v[1][1], m.v[0][1], m.v[0][1], m.v[0][1] };
	Vec4F32 vec2 = { m.v[1][2], m.v[0][2], m.v[0][2], m.v[0][2] };
	Vec4F32 vec3 = { m.v[1][3], m.v[0][3], m.v[0][3], m.v[0][3] };
	
	Vec4F32 inv0 = add_4f32(sub_4f32(mul_4f32(vec1, fac0), mul_4f32(vec2, fac1)), mul_4f32(vec3, fac2));
	Vec4F32 inv1 = add_4f32(sub_4f32(mul_4f32(vec0, fac0), mul_4f32(vec2, fac3)), mul_4f32(vec3, fac4));
	Vec4F32 inv2 = add_4f32(sub_4f32(mul_4f32(vec0, fac1), mul_4f32(vec1, fac3)), mul_4f32(vec3, fac5));
	Vec4F32 inv3 = add_4f32(sub_4f32(mul_4f32(vec0, fac2), mul_4f32(vec1, fac4)), mul_4f32(vec2, fac5));
	
	Vec4F32 sign_a = { +1, -1, +1, -1 };
	Vec4F32 sign_b = { -1, +1, -1, +1 };
	
	Mat4x4F32 inverse;
	for(U32 i = 0; i < 4; i += 1) {
		inverse.v[0][i] = inv0.v[i] * sign_a.v[i];
		inverse.v[1][i] = inv1.v[i] * sign_b.v[i];
		inverse.v[2][i] = inv2.v[i] * sign_a.v[i];
		inverse.v[3][i] = inv3.v[i] * sign_b.v[i];
	}
	
	Vec4F32 row0 = { inverse.v[0][0], inverse.v[1][0], inverse.v[2][0], inverse.v[3][0] };
	Vec4F32 m0   = { m.v[0][0], m.v[0][1], m.v[0][2], m.v[0][3] };
	Vec4F32 dot0 = mul_4f32(m0, row0);
	F32     dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

	F32    one_over_det = 1 / dot1; 
	return scale_4x4f32(inverse, one_over_det);
}

inline Mat4x4F32
derotate_4x4f32(Mat4x4F32 mat) {
	Vec3F32 scale = {
		length_3f32(v3f32(mat.v[0][0], mat.v[0][1], mat.v[0][2])),
		length_3f32(v3f32(mat.v[1][0], mat.v[1][1], mat.v[1][2])),
		length_3f32(v3f32(mat.v[2][0], mat.v[2][1], mat.v[2][2])),
	};
	mat.v[0][0] = scale.x; mat.v[1][0] = 0.f;     mat.v[2][0] = 0.f; 
	mat.v[0][1] = 0.f;     mat.v[1][1] = scale.y; mat.v[2][1] = 0.f; 
	mat.v[0][2] = 0.f;     mat.v[1][2] = 0.f;     mat.v[2][2] = scale.z;
	return mat;
}

////////////////////////////////
//~ rjf: Range Ops

// ==================== 1D Ranges ====================

#define r1u32(min, max) rng_1u32((min), (max))
inline Rng1U32 rng_1u32      (U32 min, U32 max)     { Rng1U32 r = {min, max}; if(r.min > r.max) { swap(U32, r.min, r.max); } return r; }
inline Rng1U32 shift_1u32    (Rng1U32 r, U32 x)     {         r.min += x; r.max += x;                                        return r; }
inline Rng1U32 pad_1u32      (Rng1U32 r, U32 x)     {         r.min -= x; r.max += x;                                        return r; }
inline U32     center_1u32   (Rng1U32 r)            { U32     c = (r.min + r.max) / 2;                                       return c; }
inline B32     contains_1u32 (Rng1U32 r, U32 x)     { B32     c = (r.min <= x && x < r.max);                                 return c; }
inline U32     dim_1u32      (Rng1U32 r)            { U32     c = r.max - r.min;                                             return c; }
inline Rng1U32 union_1u32    (Rng1U32 a, Rng1U32 b) { Rng1U32 c = {md_min(a.min, b.min), md_min(a.max, b.max)};              return c; }
inline Rng1U32 intersect_1u32(Rng1U32 a, Rng1U32 b) { Rng1U32 c = {md_max(a.min, b.min), md_min(a.max, b.max)};              return c; }
inline U32     clamp_1u32    (Rng1U32 r, U32 v)     {         v = clamp(r.min, v, r.max);                                    return v; }

#define r1s32(min, max) rng_1s32((min), (max))
inline Rng1S32 rng_1s32      (S32 min, S32 max)     { Rng1S32 r = {min, max}; if(r.min > r.max) { swap(S32, r.min, r.max); } return r; }
inline Rng1S32 shift_1s32    (Rng1S32 r, S32 x)     {         r.min += x; r.max += x;                                        return r; }
inline Rng1S32 pad_1s32      (Rng1S32 r, S32 x)     {         r.min -= x; r.max += x;                                        return r; }
inline S32     center_1s32   (Rng1S32 r)            { S32     c = (r.min+r.max) / 2;                                         return c; }
inline B32     contains_1s32 (Rng1S32 r, S32 x)     { B32     c = (r.min <= x && x < r.max);                                 return c; }
inline S32     dim_1s32      (Rng1S32 r)            { S32     c = r.max-r.min;                                               return c; }
inline Rng1S32 union_1s32    (Rng1S32 a, Rng1S32 b) { Rng1S32 c = {md_min(a.min, b.min), md_max(a.max, b.max)};              return c; }
inline Rng1S32 intersect_1s32(Rng1S32 a, Rng1S32 b) { Rng1S32 c = {md_max(a.min, b.min), md_min(a.max, b.max)};              return c; }
inline S32     clamp_1s32    (Rng1S32 r, S32 v)     {         v = clamp(r.min, v, r.max);                                    return v; }

#define r1u64(min, max) rng_1u64((min), (max))
inline Rng1U64 rng_1u64      (U64 min, U64 max)     { Rng1U64 r = {min, max}; if(r.min > r.max) { swap(U64, r.min, r.max); } return r; }
inline Rng1U64 shift_1u64    (Rng1U64 r, U64 x)     {         r.min += x; r.max += x;                                        return r; }
inline Rng1U64 pad_1u64      (Rng1U64 r, U64 x)     {         r.min -= x; r.max += x;                                        return r; }
inline U64     center_1u64   (Rng1U64 r)            { U64     c = (r.min + r.max)/2;                                         return c; }
inline B32     contains_1u64 (Rng1U64 r, U64 x)     { B32     c = (r.min <= x && x < r.max);                                 return c; }
inline U64     dim_1u64      (Rng1U64 r)            { U64     c = r.max-r.min;                                               return c; }
inline Rng1U64 union_1u64    (Rng1U64 a, Rng1U64 b) { Rng1U64 c = {md_min(a.min, b.min), md_max(a.max, b.max)};              return c; }
inline Rng1U64 intersect_1u64(Rng1U64 a, Rng1U64 b) { Rng1U64 c = {md_max(a.min, b.min), md_min(a.max, b.max)};              return c; }
inline U64     clamp_1u64    (Rng1U64 r, U64 v)     {         v = clamp(r.min, v, r.max);                                    return v; }

#define r1s64(min, max) rng_1s64((min), (max))
inline Rng1S64 rng_1s64      (S64 min, S64 max)     { Rng1S64 r = {min, max}; if(r.min > r.max) { swap(S64, r.min, r.max); } return r; }
inline Rng1S64 shift_1s64    (Rng1S64 r, S64 x)     {         r.min += x; r.max += x;                                        return r; }
inline Rng1S64 pad_1s64      (Rng1S64 r, S64 x)     {         r.min -= x; r.max += x;                                        return r; }
inline S64     center_1s64   (Rng1S64 r)            { S64     c = (r.min + r.max) / 2;                                       return c; }
inline B32     contains_1s64 (Rng1S64 r, S64 x)     { B32     c = (r.min <= x && x < r.max);                                 return c; }
inline S64     dim_1s64      (Rng1S64 r)            { S64     c = r.max - r.min;                                             return c; }
inline Rng1S64 union_1s64    (Rng1S64 a, Rng1S64 b) { Rng1S64 c = {md_min(a.min, b.min), md_max(a.max, b.max)};              return c; }
inline Rng1S64 intersect_1s64(Rng1S64 a, Rng1S64 b) { Rng1S64 c = {md_max(a.min, b.min), md_min(a.max, b.max)};              return c; }
inline S64     clamp_1s64    (Rng1S64 r, S64 v)     {         v = clamp(r.min, v, r.max);                                    return v;}

#define r1f32(min, max) rng_1f32((min), (max))
inline Rng1F32 rng_1f32      (F32 min, F32 max)     { Rng1F32 r = {min, max}; if(r.min > r.max) { swap(F32, r.min, r.max); } return r; }
inline Rng1F32 shift_1f32    (Rng1F32 r, F32 x)     {         r.min += x; r.max += x;                                        return r; }
inline Rng1F32 pad_1f32      (Rng1F32 r, F32 x)     {         r.min -= x; r.max += x;                                        return r; }
inline F32     center_1f32   (Rng1F32 r)            { F32     c = (r.min + r.max) / 2;                                       return c; }
inline B32     contains_1f32 (Rng1F32 r, F32 x)     { B32     c = (r.min <= x && x < r.max);                                 return c; }
inline F32     dim_1f32      (Rng1F32 r)            { F32     c = r.max - r.min;                                             return c; }
inline Rng1F32 union_1f32    (Rng1F32 a, Rng1F32 b) { Rng1F32 c = {md_min(a.min, b.min), md_max(a.max, b.max)};              return c; }
inline Rng1F32 intersect_1f32(Rng1F32 a, Rng1F32 b) { Rng1F32 c = {md_max(a.min, b.min), md_min(a.max, b.max)};              return c; }
inline F32     clamp_1f32    (Rng1F32 r, F32 v)     {         v = clamp(r.min, v, r.max);                                    return v; }

// ==================== 2D Ranges ====================

#define r2s16(min, max) rng_2s16((min), (max))
#define r2s16p(x, y, z, w) r2s16(v2s16((x), (y)), v2s16((z), (w)))
inline Rng2S16 rng_2s16      (Vec2S16 min, Vec2S16 max) { Rng2S16 r = {min, max};                                                            return r; }
inline Rng2S16 shift_2s16    (Rng2S16 r, Vec2S16 x)     {         r.min = add_2s16(r.min, x); r.max = add_2s16(r.max, x);                    return r; }
inline Rng2S16 pad_2s16      (Rng2S16 r, S16 x)         { Vec2S16 xv = {x, x}; r.min = sub_2s16(r.min, xv); r.max = add_2s16(r.max, xv);     return r; }
inline Vec2S16 center_2s16   (Rng2S16 r)                { Vec2S16 c = {(S16)((r.min.x + r.max.x) / 2), (S16)((r.min.y + r.max.y) / 2)};      return c; }
inline B32     contains_2s16 (Rng2S16 r, Vec2S16 x)     { B32     c = (r.min.x <= x.x && x.x < r.max.x && r.min.y <= x.y && x.y < r.max.y);  return c; }
inline Vec2S16 dim_2s16      (Rng2S16 r)                { Vec2S16 dim = {(S16)(r.max.x - r.min.x), (S16)(r.max.y - r.min.y)};                return dim; }
inline Rng2S16 union_2s16    (Rng2S16 a, Rng2S16 b)     { Rng2S16 c; c.p0.x = md_min(a.min.x, b.min.x); c.p0.y = md_min(a.min.y, b.min.y); c.p1.x = md_max(a.max.x, b.max.x); c.p1.y = md_max(a.max.y, b.max.y); return c; }
inline Rng2S16 intersect_2s16(Rng2S16 a, Rng2S16 b)     { Rng2S16 c; c.p0.x = md_max(a.min.x, b.min.x); c.p0.y = md_max(a.min.y, b.min.y); c.p1.x = md_min(a.max.x, b.max.x); c.p1.y = md_min(a.max.y, b.max.y); return c; }
inline Vec2S16 clamp_2s16    (Rng2S16 r, Vec2S16 v)     {         v.x = clamp(r.min.x, v.x, r.max.x); v.y = clamp(r.min.y, v.y, r.max.y); return v; }

#define r2s32(min, max) rng_2s32((min), (max))
#define r2s32p(x, y, z, w) r2s32(v2s32((x), (y)), v2s32((z), (w)))
inline Rng2S32 rng_2s32      (Vec2S32 min, Vec2S32 max) { Rng2S32 r = {min, max};                                                           return r; }
inline Rng2S32 shift_2s32    (Rng2S32 r, Vec2S32 x)     {         r.min = add_2s32(r.min, x); r.max = add_2s32(r.max, x);                   return r; }
inline Rng2S32 pad_2s32      (Rng2S32 r, S32 x)         { Vec2S32 xv = {x, x}; r.min = sub_2s32(r.min, xv); r.max = add_2s32(r.max, xv);    return r; }
inline Vec2S32 center_2s32   (Rng2S32 r)                { Vec2S32 c = {(r.min.x + r.max.x) / 2, (r.min.y + r.max.y) / 2};                   return c; }
inline B32     contains_2s32 (Rng2S32 r, Vec2S32 x)     { B32     c = (r.min.x <= x.x && x.x < r.max.x && r.min.y <= x.y && x.y < r.max.y); return c; }
inline Vec2S32 dim_2s32      (Rng2S32 r)                { Vec2S32 dim = {r.max.x - r.min.x, r.max.y - r.min.y};                             return dim; }
inline Rng2S32 union_2s32    (Rng2S32 a, Rng2S32 b)     { Rng2S32 c; c.p0.x = md_min(a.min.x, b.min.x); c.p0.y = md_min(a.min.y, b.min.y); c.p1.x = md_max(a.max.x, b.max.x); c.p1.y = md_max(a.max.y, b.max.y); return c; }
inline Rng2S32 intersect_2s32(Rng2S32 a, Rng2S32 b)     { Rng2S32 c; c.p0.x = md_max(a.min.x, b.min.x); c.p0.y = md_max(a.min.y, b.min.y); c.p1.x = md_min(a.max.x, b.max.x); c.p1.y = md_min(a.max.y, b.max.y); return c; }
inline Vec2S32 clamp_2s32    (Rng2S32 r, Vec2S32 v)     {         v.x = clamp(r.min.x, v.x, r.max.x); v.y = clamp(r.min.y, v.y, r.max.y); return v; }

#define r2s64(min, max) rng_2s64((min), (max))
#define r2s64p(x, y, z, w) r2s64(v2s64((x), (y)), v2s64((z), (w)))
inline Rng2S64 rng_2s64      (Vec2S64 min, Vec2S64 max) { Rng2S64 r = {min, max};                                                           return r; }
inline Rng2S64 shift_2s64    (Rng2S64 r, Vec2S64 x)     {         r.min = add_2s64(r.min, x); r.max = add_2s64(r.max, x);                   return r; }
inline Rng2S64 pad_2s64      (Rng2S64 r, S64 x)         { Vec2S64 xv = {x, x}; r.min = sub_2s64(r.min, xv); r.max = add_2s64(r.max, xv);    return r; }
inline Vec2S64 center_2s64   (Rng2S64 r)                { Vec2S64 c = {(r.min.x + r.max.x) / 2, (r.min.y + r.max.y) / 2};                   return c; }
inline B32     contains_2s64 (Rng2S64 r, Vec2S64 x)     { B32     c = (r.min.x <= x.x && x.x < r.max.x && r.min.y <= x.y && x.y < r.max.y); return c; }
inline Vec2S64 dim_2s64      (Rng2S64 r)                { Vec2S64 dim = {r.max.x - r.min.x, r.max.y - r.min.y};                             return dim; }
inline Rng2S64 union_2s64    (Rng2S64 a, Rng2S64 b)     { Rng2S64 c; c.p0.x = md_min(a.min.x, b.min.x); c.p0.y = md_min(a.min.y, b.min.y); c.p1.x = md_max(a.max.x, b.max.x); c.p1.y = md_max(a.max.y, b.max.y); return c; }
inline Rng2S64 intersect_2s64(Rng2S64 a, Rng2S64 b)     { Rng2S64 c; c.p0.x = md_max(a.min.x, b.min.x); c.p0.y = md_max(a.min.y, b.min.y); c.p1.x = md_min(a.max.x, b.max.x); c.p1.y = md_min(a.max.y, b.max.y); return c; }
inline Vec2S64 clamp_2s64    (Rng2S64 r, Vec2S64 v)     {         v.x = clamp(r.min.x, v.x, r.max.x); v.y = clamp(r.min.y, v.y, r.max.y); return v; }

////////////////////////////////
//~ rjf: Miscellaneous Ops

Vec3F32 hsv_from_rgb  (Vec3F32 rgb);
Vec3F32 rgb_from_hsv  (Vec3F32 hsv);
Vec4F32 hsva_from_rgba(Vec4F32 rgba);
Vec4F32 rgba_from_hsva(Vec4F32 hsva);
Vec4F32 rgba_from_u32 (U32     hex);
U32     u32_from_rgba (Vec4F32 rgba);

inline Vec3F32
hsv_from_rgb(Vec3F32 rgb)
{
	F32 c_max   = md_max(rgb.x, md_max(rgb.y, rgb.z));
	F32 c_min   = md_min(rgb.x, md_min(rgb.y, rgb.z));
	F32 delta   = c_max - c_min;
	F32 q_delta = 1.0f / delta;
	F32 h = ( 
		(delta == 0.f)   ? 0.f                                           :
		(c_max == rgb.x) ? mod_f32((rgb.y - rgb.z) * q_delta + 6.f, 6.f) :
		(c_max == rgb.y) ? (rgb.z - rgb.x)         * q_delta + 2.f       :
		(c_max == rgb.z) ? (rgb.x - rgb.y)         * q_delta + 4.f       :
		                   0.f
	);
	F32 s = (c_max == 0.f) ? 0.f : (delta / c_max);
	F32 v = c_max;
	Vec3F32 hsv = {h * (1.0f / 6.f), s, v};
	return hsv;
}

inline Vec3F32
rgb_from_hsv(Vec3F32 hsv) {
	F32 h = mod_f32(hsv.x * 360.f, 360.f);
	F32 s = hsv.y;
	F32 v = hsv.z;
	F32 c = v * s;
	F32 x = c * (1.f - abs_f32(mod_f32(h / 60.f, 2.f) - 1.f));
	F32 m = v - c;
	F32 r = 0;
	F32 g = 0;
	F32 b = 0;
	if      ((h >= 0.f   && h < 60.f ) || (h >= 360.f && h < 420.f)) { r = c; g = x; b = 0; }
	else if ( h >= 60.f  && h < 120.f)                               { r = x; g = c; b = 0; }
	else if ( h >= 120.f && h < 180.f)                               { r = 0; g = c; b = x; }
	else if ( h >= 180.f && h < 240.f )                              { r = 0; g = x; b = c; }
	else if ( h >= 240.f && h < 300.f )                              { r = x; g = 0; b = c; }
	else if ((h >= 300.f && h <= 360.f) || (h >= -60.f && h <= 0.f)) { r = c; g = 0; b = x; }
	Vec3F32 rgb = {r + m, g + m, b + m};
	return(rgb);
}

inline Vec4F32
hsva_from_rgba(Vec4F32 rgba) {
	Vec3F32 rgb  = v3f32(rgba.x, rgba.y, rgba.z);
	Vec3F32 hsv  = hsv_from_rgb(rgb);
	Vec4F32 hsva = v4f32(hsv.x, hsv.y, hsv.z, rgba.w);
	return hsva;
}

inline Vec4F32
rgba_from_hsva(Vec4F32 hsva)
{
	Vec3F32 hsv  = v3f32(hsva.x, hsva.y, hsva.z);
	Vec3F32 rgb  = rgb_from_hsv(hsv);
	Vec4F32 rgba = v4f32(rgb.x, rgb.y, rgb.z, hsva.w);
	return rgba;
}

inline Vec4F32
rgba_from_u32(U32 hex)
{
	Vec4F32 result = v4f32(
		((hex&0xff000000) >> 24) / 255.f,
		((hex&0x00ff0000) >> 16) / 255.f,
		((hex&0x0000ff00) >>  8) / 255.f,
		((hex&0x000000ff) >>  0) / 255.f);
	return result;
}

inline U32
u32_from_rgba(Vec4F32 rgba)
{
	U32 result = 0;
	result |= ((U32)((U8)(rgba.x * 255.f))) << 24;
	result |= ((U32)((U8)(rgba.y * 255.f))) << 16;
	result |= ((U32)((U8)(rgba.z * 255.f))) <<  8;
	result |= ((U32)((U8)(rgba.w * 255.f))) <<  0;
	return result;
}

#define rgba_from_u32_lit_comp(h)       \
{                                       \
	(((h) & 0xff000000) >> 24) / 255.f, \
	(((h) & 0x00ff0000) >> 16) / 255.f, \
	(((h) & 0x0000ff00) >> 8 ) / 255.f, \
	(((h) & 0x000000ff) >> 0 ) / 255.f  \
}

////////////////////////////////
//~ rjf: List Type Functions

void         rng1s64_list_push__arena           (Arena*        arena, Rng1S64List* list, Rng1S64 rng);
void         rng1s64_list_push__ainfo           (AllocatorInfo ainfo, Rng1S64List* list, Rng1S64 rng);
Rng1S64Array rng1s64_array_from_list_push__arena(Arena*        arena, Rng1S64List* list);
Rng1S64Array rng1s64_array_from_list_push__ainfo(AllocatorInfo ainfo, Rng1S64List* list);

#define rng1s64_list_push(allocator, list, rng)       _Generic(allocator, Arena*: rng1s64_list_push__arena,            AllocatorInfo: rng1s64_list_push__ainfo,            default: assert_generic_sel_fail) generic_call(allocator, list, rng)
#define rng1s64_array_from_list_push(allocator, list) _Generic(allocator, Arena*: rng1s64_array_from_list_push__arena, AllocatorInfo: rng1s64_array_from_list_push__ainfo, default: assert_generic_sel_fail) generic_call(allocator, list)

force_inline void         rng1s64_list_push__arena           (Arena* arena, Rng1S64List* list, Rng1S64 rng) {        rng1s64_list_push__ainfo           (arena_allocator(arena), list, rng); }
force_inline Rng1S64Array rng1s64_array_from_list_push__arena(Arena* arena, Rng1S64List* list)              { return rng1s64_array_from_list_push__ainfo(arena_allocator(arena), list); }

inline void
rng1s64_list_alloc(AllocatorInfo ainfo, Rng1S64List* list, Rng1S64 rng) {
	Rng1S64Node* n = alloc_array(ainfo, Rng1S64Node, 1);
	memory_copy_struct(&n->v, &rng);
	sll_queue_push(list->first, list->last, n);
	list->count += 1;
}

inline Rng1S64Array
rng1s64_array_from_list_alloc(AllocatorInfo ainfo, Rng1S64List* list) {
	Rng1S64Array 
	arr       = {0};
	arr.count = list->count;
	arr.v     = alloc_array_no_zero(ainfo, Rng1S64, arr.count);
	U64 idx   = 0;
	for (Rng1S64Node* n = list->first; n != 0; n = n->next) {
		arr.v[idx] = n->v;
		idx       += 1;
	}
	return arr;
}
