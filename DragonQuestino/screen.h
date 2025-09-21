#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define SCREEN_WIDTH                   256
#define SCREEN_HEIGHT                  224
#define SCREEN_PIXELS                  57344

#define COLOR_BLACK                    0x0000
#define COLOR_WHITE                    0xFFFF
#define COLOR_RED                      0xF800
#define COLOR_GREEN                    0x07E0
#define COLOR_BLUE                     0x001F
#define COLOR_GROSSYELLOW              0x7BA1
#define COLOR_INJUREDRED               0xDA8A
#define COLOR_DARKRED                  0xB0E4
#define COLOR_DEEPRED                  0x7800
#define COLOR_ENDINGYELLOW             0xFD00
#define COLOR_ENDINGPURPLE             0xEB1F

#define TEXT_TILE_COUNT                85
#define TEXT_TILE_SIZE                 8

#define TILE_TEXTURE_BYTES             256   // 8 bpp

#define TRANSPARENT_COLOR_INDEX        0x0B

#define MENU_BORDER_CHAR_TOPLEFT       1
#define MENU_BORDER_CHAR_TOPRIGHT      2
#define MENU_BORDER_CHAR_BOTTOMLEFT    3
#define MENU_BORDER_CHAR_BOTTOMRIGHT   4
#define MENU_BORDER_CHAR_LEFT          5
#define MENU_BORDER_CHAR_TOP           6
#define MENU_BORDER_CHAR_RIGHT         7
#define MENU_BORDER_CHAR_BOTTOM        8
#define DOWNWARD_CARAT                 9

#define CARAT_BLINK_RATE_SECONDS       0.3f

#define BATTLE_BACKGROUND_TILE_COUNT   36

#define COLOR_GET_R5( c16 ) ( ( ( c16 ) >> 11 ) & 0x1F )
#define COLOR_GET_G6( c16 ) ( ( ( c16 ) >> 5 ) & 0x3F )
#define COLOR_GET_B5( c16 ) ( ( c16 ) & 0x1F )
#define COLOR_MAKE_RGB565( r, g, b ) ( ( ( r ) & 0x1F ) << 11 ) | ( ( ( g ) & 0x3F ) << 5 ) | ( ( b ) & 0x1F )

typedef struct TileTexture_t
{
   u8 memory[TILE_TEXTURE_BYTES];
}
TileTexture_t;

typedef struct Screen_t
{
   u16* buffer;
   u16 palette[PALETTE_COLORS];
   u16 backupPalette[PALETTE_COLORS];
   u8 textBitFields[TEXT_TILE_COUNT][TEXT_TILE_SIZE];
   u16 textColor;
   u16 wipeColor;
   Bool_t needsWipe;

   TileTexture_t battleBackgroundTileTextures[BATTLE_BACKGROUND_TILE_COUNT];
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen, u16* buffer );
void Screen_BackupPalette( Screen_t* screen );
void Screen_RestorePalette( Screen_t* screen );
void Screen_ClearPalette( Screen_t* screen, u16 color );
void Screen_BackupPaletteAndWipeColor( Screen_t* screen, u16 color );
Bool_t Screen_GetPaletteIndexForColor( Screen_t* screen, u16 color, u32* paletteIndex );
void Screen_WipeFromPalette( Screen_t* screen, u32 paletteIndex );
void Screen_WipeColor( Screen_t* screen, u16 color );
void Screen_DrawRectFromPalette( Screen_t* screen, u32 x, u32 y, u32 w, u32 h, u32 paletteIndex );
void Screen_DrawRectColor( Screen_t* screen, u32 x, u32 y, u32 w, u32 h, u16 color );
void Screen_DrawChar( Screen_t* screen, char c, u32 x, u32 y );
void Screen_DrawText( Screen_t* screen, const char* text, u32 x, u32 y );
void Screen_DrawCenteredText( Screen_t* screen, const char* text, u32 y );
void Screen_DrawMemorySection( Screen_t* screen, u8* memory, u32 stride,
                               u32 tx, u32 ty,
                               u32 tw, u32 th,
                               u32 sx, u32 sy,
                               Bool_t transparency );
void Screen_DrawMemorySectionBlended( Screen_t* screen, u8* memory, u32 stride,
                                      u32 tx, u32 ty,
                                      u32 tw, u32 th,
                                      u32 sx, u32 sy,
                                      u8 alpha );
void Screen_DrawTextWindow( Screen_t* screen, u32 x, u32 y, u32 w, u32 h );
void Screen_DrawTextWindowWithTitle( Screen_t* screen, u32 x, u32 y, u32 w, u32 h, const char* title );

// platform-specific
void Screen_RenderBuffer( Screen_t* screen );

// game_data.c
void Screen_LoadPalette( Screen_t* screen );
void Screen_LoadTextBitFields( Screen_t* screen );
void Screen_LoadBattleBackgroundTileTextures( Screen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
