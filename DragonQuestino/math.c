#include "math.h"

#define MIN_FORMULA( l, r ) return l < r ? l : r
#define MAX_FORMULA( l, r ) return l > r ? l : r

u8 Math_Min8u( u8 l, u8 r )
{
   MIN_FORMULA( l, r );
}

u16 Math_Min16u( u16 l, u16 r )
{
   MIN_FORMULA( l, r );
}

u32 Math_Min32u( u32 l, u32 r )
{
   MIN_FORMULA( l, r );
}

int32_t Math_Min32i( int32_t l, int32_t r )
{
   MIN_FORMULA( l, r );
}

u8 Math_Max8u( u8 l, u8 r )
{
   MAX_FORMULA( l, r );
}

u16 Math_Max16u( u16 l, u16 r )
{
   MAX_FORMULA( l, r );
}

u32 Math_Max32u( u32 l, u32 r )
{
   MAX_FORMULA( l, r );
}

int32_t Math_Max32i( int32_t l, int32_t r )
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

u16 Math_AmountToCollect16u( u16 existing, u16 bounty )
{
   if ( bounty < ( UINT16_MAX - existing ) )
   {
      return bounty;
   }
   else
   {
      return UINT16_MAX - existing;
   }
}
