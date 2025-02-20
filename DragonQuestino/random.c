#include "random.h"

uint32_t Random_Percent()
{
   return (uint32_t)( rand() % 101 );
}

uint16_t Random_U16( uint16_t min, uint16_t max )
{
   // min and max inclusive
   return ( (uint16_t)rand() % ( ( max + 1 ) - min ) ) + min;
}

uint32_t Random_U32( uint32_t min, uint32_t max )
{
   // min and max inclusive
   return ( (uint32_t)rand() % ( ( max + 1 ) - min ) ) + min;
}
