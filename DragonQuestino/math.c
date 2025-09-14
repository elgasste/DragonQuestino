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

i32 Math_Min32i( i32 l, i32 r )
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

i32 Math_Max32i( i32 l, i32 r )
{
   MAX_FORMULA( l, r );
}

Bool_t Math_RectsIntersect32i( i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

Bool_t Math_RectsIntersectF( r32 x1, r32 y1, r32 w1, r32 h1, r32 x2, r32 y2, r32 w2, r32 h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

Bool_t Math_PointInRectF( r32 px, r32 py, r32 rx, r32 ry, r32 rw, r32 rh )
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
