#if !defined( VECTOR_H )
#define VECTOR_H

#include "common.h"

typedef struct Vector2u16_t
{
   u16 x;
   u16 y;
}
Vector2u16_t;

typedef struct Vector2i32_t
{
   i32 x;
   i32 y;
}
Vector2i32_t;

typedef struct Vector2u32_t
{
   u32 x;
   u32 y;
}
Vector2u32_t;

typedef struct Vector2r32_t
{
   r32 x;
   r32 y;
}
Vector2r32_t;

typedef struct Vector4i32_t
{
   i32 x;
   i32 y;
   i32 w;
   i32 h;
}
Vector4i32_t;

typedef struct Vector4u32_t
{
   u32 x;
   u32 y;
   u32 w;
   u32 h;
}
Vector4u32_t;

#endif // VECTOR_H
