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
   int32_t x;
   int32_t y;
}
Vector2i32_t;

typedef struct Vector2u32_t
{
   u32 x;
   u32 y;
}
Vector2u32_t;

typedef struct Vector2f_t
{
   float x;
   float y;
}
Vector2f_t;

typedef struct Vector4i32_t
{
   int32_t x;
   int32_t y;
   int32_t w;
   int32_t h;
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
