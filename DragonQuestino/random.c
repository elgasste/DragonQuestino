#include "random.h"

u32 Random_Percent()
{
   return (u32)( rand() % 101 );
}

u8 Random_u8( u8 min, u8 max )
{
   // min and max inclusive
   return ( (u8)rand() % ( ( max + 1 ) - min ) ) + min;
}

u16 Random_u16( u16 min, u16 max )
{
   // min and max inclusive
   return ( (u16)rand() % ( ( max + 1 ) - min ) ) + min;
}

u32 Random_u32( u32 min, u32 max )
{
   // min and max inclusive
   return ( (u32)rand() % ( ( max + 1 ) - min ) ) + min;
}

r32 Random_NormalizedPercent()
{
   u32 percent = Random_Percent();
   return (r32)percent * 0.01f;
}
