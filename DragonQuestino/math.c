#include "math.h"

Bool_t Math_RectsIntersect32i( int32_t x1, int32_t y1, int32_t w1, int32_t h1, int32_t x2, int32_t y2, int32_t w2, int32_t h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

uint16_t Math_CollectAmount16u( uint16_t* dest, uint16_t src )
{
   if ( src < ( UINT16_MAX - *dest ) )
   {
      *dest += src;
      return src;
   }
   else
   {
      *dest = UINT16_MAX;
      return UINT16_MAX - src;
   }
}
