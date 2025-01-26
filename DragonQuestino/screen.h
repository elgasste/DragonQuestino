#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define VIEWPORT_BUFFER_WIDTH      256
#define VIEWPORT_BUFFER_HEIGHT     240
#define VIEWPORT_BUFFER_PIXELS     61440

#define PALETTE_COLORS           256

#define TRANSPARENT_COLOR_INDEX  0x0A

typedef struct Screen_t
{
   uint8_t buffer[VIEWPORT_BUFFER_PIXELS];
   uint16_t palette[PALETTE_COLORS];
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen );
Bool_t Screen_GetPaletteIndexForColor( Screen_t* screen, uint16_t color, uint8_t* paletteIndex );
void Screen_Wipe( Screen_t* screen, uint8_t paletteIndex );

// platform-specific
void Screen_RenderBuffer( Screen_t* screen );

// game_data.c
void Screen_LoadPalette( Screen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
