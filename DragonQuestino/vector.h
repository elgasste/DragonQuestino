#if !defined( VECTOR_H )
#define VECTOR_H

#include "common.h"

typedef struct Vector2i32_t
{
   int32_t x;
   int32_t y;
}
Vector2i32_t;

typedef struct Vector2u32_t
{
   uint32_t x;
   uint32_t y;
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

#endif // VECTOR_H
