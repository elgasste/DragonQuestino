#if !defined( MATH_H )
#define MATH_H

#include "common.h"

#define MATH_MINU32( l, r ) ( ( l ) < ( r ) ? ( l ) : ( r ) )
#define MATH_MAXU32( l, r ) ( ( l ) > ( r ) ? ( l ) : ( r ) )

#if defined( __cplusplus )
extern "C" {
#endif

Bool_t Math_RectsIntersect32i( int32_t x1, int32_t y1, int32_t w1, int32_t h1, int32_t x2, int32_t y2, int32_t w2, int32_t h2 );

#if defined( __cplusplus )
}
#endif

#endif // MATH_H
