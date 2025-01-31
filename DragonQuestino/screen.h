#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define SCREEN_WIDTH             256
#define SCREEN_HEIGHT            224
#define SCREEN_PIXELS            57344

#define PALETTE_COLORS           256

#define TRANSPARENT_COLOR_INDEX  0x0A

typedef struct Screen_t
{
   uint16_t* buffer;
   uint16_t palette[PALETTE_COLORS];
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen, uint16_t* buffer );
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
