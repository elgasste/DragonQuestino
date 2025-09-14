#if !defined( MATH_H )
#define MATH_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

u8 Math_Min8u( u8 l, u8 r);
u16 Math_Min16u( u16 l, u16 r);
u32 Math_Min32u( u32 l, u32 r);
int32_t Math_Min32i( int32_t l, int32_t r );
u8 Math_Max8u( u8 l, u8 r);
u16 Math_Max16u( u16 l, u16 r);
u32 Math_Max32u( u32 l, u32 r);
int32_t Math_Max32i( int32_t l, int32_t r);
Bool_t Math_RectsIntersect32i( int32_t x1, int32_t y1, int32_t w1, int32_t h1, int32_t x2, int32_t y2, int32_t w2, int32_t h2 );
Bool_t Math_RectsIntersectF( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 );
Bool_t Math_PointInRectF( float px, float py, float rx, float ry, float rw, float rh );
u16 Math_AmountToCollect16u( u16 existing, u16 bounty );

#if defined( __cplusplus )
}
#endif

#endif // MATH_H
