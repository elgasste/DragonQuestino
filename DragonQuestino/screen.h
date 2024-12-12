#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define SCREEN_BUFFER_WIDTH      320
#define SCREEN_BUFFER_HEIGHT     240
#define SCREEN_BUFFER_BYTES      76800    // 8 bpp

#define PALETTE_COLORS           256

typedef struct Screen_t
{
   uint8_t buffer[SCREEN_BUFFER_BYTES];
   uint16_t palette[PALETTE_COLORS];
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen );

// platform-specific
void Screen_RenderBuffer( Screen_t* screen );

// data.c
void Screen_LoadPalette( Screen_t* screen, uint8_t index );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
