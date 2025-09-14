#if !defined( RANDOM_H )
#define RANDOM_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

u32 Random_Percent();
u8 Random_u8( u8 min, u8 max );
u16 Random_u16( u16 min, u16 max );
u32 Random_u32( u32 min, u32 max );
r32 Random_NormalizedPercent();

// platform-specific
void Random_Seed();

#if defined( __cplusplus )
}
#endif

#endif // RANDOM_H
