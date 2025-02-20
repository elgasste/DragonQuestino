#if !defined( RANDOM_H )
#define RANDOM_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

uint32_t Random_Percent();
uint16_t Random_U16( uint16_t min, uint16_t max );
uint32_t Random_U32( uint32_t min, uint32_t max );

// platform-specific
void Random_Seed();

#if defined( __cplusplus )
}
#endif

#endif // RANDOM_H
