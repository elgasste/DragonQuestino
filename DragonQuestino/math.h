#if !defined( MATH_H )
#define MATH_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

uint8_t Math_Min8u( uint8_t l, uint8_t r);
uint16_t Math_Min16u( uint16_t l, uint16_t r);
uint32_t Math_Min32u( uint32_t l, uint32_t r);
int32_t Math_Min32i( int32_t l, int32_t r );
uint8_t Math_Max8u( uint8_t l, uint8_t r);
uint16_t Math_Max16u( uint16_t l, uint16_t r);
uint32_t Math_Max32u( uint32_t l, uint32_t r);
int32_t Math_Max32i( int32_t l, int32_t r);
Bool_t Math_RectsIntersect32i( int32_t x1, int32_t y1, int32_t w1, int32_t h1, int32_t x2, int32_t y2, int32_t w2, int32_t h2 );
Bool_t Math_RectsIntersectF( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 );
Bool_t Math_PointInRectF( float px, float py, float rx, float ry, float rw, float rh );
uint8_t Math_CollectAmount8u( uint8_t* dest, uint8_t src );
uint16_t Math_CollectAmount16u( uint16_t* dest, uint16_t src );

#if defined( __cplusplus )
}
#endif

#endif // MATH_H
