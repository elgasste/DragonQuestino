#if !defined( RANDOM_H )
#define RANDOM_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

uint32_t Random_Percent();
uint8_t Random_u8( uint8_t min, uint8_t max );
uint16_t Random_u16( uint16_t min, uint16_t max );
uint32_t Random_u32( uint32_t min, uint32_t max );
float Random_NormalizedPercent();

// platform-specific
void Random_Seed();

#if defined( __cplusplus )
}
#endif

#endif // RANDOM_H
