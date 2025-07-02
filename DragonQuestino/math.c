#include "math.h"

#define MIN_FORMULA( l, r ) return l < r ? l : r
#define MAX_FORMULA( l, r ) return l > r ? l : r

uint8_t Math_Min8u( uint8_t l, uint8_t r )
{
   MIN_FORMULA( l, r );
}

uint16_t Math_Min16u( uint16_t l, uint16_t r )
{
   MIN_FORMULA( l, r );
}

uint32_t Math_Min32u( uint32_t l, uint32_t r )
{
   MIN_FORMULA( l, r );
}

int32_t Math_Min32( int32_t l, int32_t r )
{
   MIN_FORMULA( l, r );
}

uint8_t Math_Max8u( uint8_t l, uint8_t r )
{
   MAX_FORMULA( l, r );
}

uint16_t Math_Max16u( uint16_t l, uint16_t r )
{
   MAX_FORMULA( l, r );
}

uint32_t Math_Max32u( uint32_t l, uint32_t r )
{
   MAX_FORMULA( l, r );
}

Bool_t Math_RectsIntersect32i( int32_t x1, int32_t y1, int32_t w1, int32_t h1, int32_t x2, int32_t y2, int32_t w2, int32_t h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

Bool_t Math_RectsIntersectF( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

Bool_t Math_PointInRectF( float px, float py, float rx, float ry, float rw, float rh )
{
   return ( px > rx ) && ( px < ( rx + rw ) ) && ( py > ry ) && py < ( ry + rh ) ? True : False;
}

uint8_t Math_CollectAmount8u( uint8_t* dest, uint8_t src )
{
   uint8_t returnAmount;

   if ( src < ( UINT8_MAX - *dest ) )
   {
      *dest += src;
      return src;
   }
   else
   {
      returnAmount = UINT8_MAX - *dest;
      *dest = UINT8_MAX;
      return returnAmount;
   }
}

uint16_t Math_CollectAmount16u( uint16_t* dest, uint16_t src )
{
   uint16_t returnAmount;

   if ( src < ( UINT16_MAX - *dest ) )
   {
      *dest += src;
      return src;
   }
   else
   {
      returnAmount = UINT16_MAX - *dest;
      *dest = UINT16_MAX;
      return returnAmount;
   }
}
