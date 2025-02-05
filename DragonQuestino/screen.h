#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define SCREEN_WIDTH             256
#define SCREEN_HEIGHT            224
#define SCREEN_PIXELS            57344

#define PALETTE_COLORS           256

#define TEXT_TILE_COUNT          83
#define TEXT_TILE_SIZE           8

#define TRANSPARENT_COLOR_INDEX  0x0A

#define MENU_BORDER_CHAR_TOPLEFT       0
#define MENU_BORDER_CHAR_TOPRIGHT      1
#define MENU_BORDER_CHAR_BOTTOMLEFT    2
#define MENU_BORDER_CHAR_BOTTOMRIGHT   3
#define MENU_BORDER_CHAR_LEFT          4
#define MENU_BORDER_CHAR_TOP           5
#define MENU_BORDER_CHAR_RIGHT         6
#define MENU_BORDER_CHAR_BOTTOM        7

typedef struct Screen_t
{
   uint16_t* buffer;
   uint16_t palette[PALETTE_COLORS];
   uint8_t textBitFields[TEXT_TILE_COUNT][TEXT_TILE_SIZE];
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen, uint16_t* buffer );
Bool_t Screen_GetPaletteIndexForColor( Screen_t* screen, uint16_t color, uint32_t* paletteIndex );
void Screen_Wipe( Screen_t* screen, uint32_t paletteIndex );
void Screen_DrawChar( Screen_t* screen, char c, uint32_t x, uint32_t y, uint16_t color );
void Screen_DrawText( Screen_t* screen, const char* text, uint32_t x, uint32_t y, uint16_t color );
void Screen_DrawWrappedText( Screen_t* screen, const char* text, uint32_t x, uint32_t y, uint32_t lineChars, uint16_t color );

// platform-specific
void Screen_RenderBuffer( Screen_t* screen );

// game_data.c
void Screen_LoadPalette( Screen_t* screen );
void Screen_LoadTextBitFields( Screen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
