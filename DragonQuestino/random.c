#include "random.h"

uint32_t Random_Percent()
{
   return (uint32_t)( rand() % 101 );
}

uint8_t Random_u8( uint8_t min, uint8_t max )
{
   // min and max inclusive
   return ( (uint8_t)rand() % ( ( max + 1 ) - min ) ) + min;
}

uint16_t Random_u16( uint16_t min, uint16_t max )
{
   // min and max inclusive
   return ( (uint16_t)rand() % ( ( max + 1 ) - min ) ) + min;
}

uint32_t Random_u32( uint32_t min, uint32_t max )
{
   // min and max inclusive
   return ( (uint32_t)rand() % ( ( max + 1 ) - min ) ) + min;
}

float Random_NormalizedPercent()
{
   uint32_t percent = Random_Percent();
   return (float)percent * 0.01f;
}
