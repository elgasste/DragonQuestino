#if !defined( GIGA_SHIELD_BACKGROUND_DATA_H )
#define GIGA_SHIELD_BACKGROUND_DATA_H

#include "common.h"

#define GIGA_SHIELD_WIDTH        480
#define GIGA_SHIELD_HEIGHT       800
#define GIGA_SHIELD_PIXELS       384000

#if defined( __cplusplus )
extern "C" {
#endif

void Giga_LoadShieldBackground( uint32_t* buffer );

#if defined( __cplusplus )
}
#endif

#endif // GIGA_SHIELD_BACKGROUND_DATA_H
