#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define COLOR_BLACK              0x0000
#define COLOR_DARKGRAY           0x4228
#define COLOR_BLUE               0x001F
#define COLOR_RED                0xF800
#define COLOR_GREEN              0x07E0
#define COLOR_CYAN               0x07FF
#define COLOR_MAGENTA            0xF81F
#define COLOR_YELLOW             0xFFE0
#define COLOR_WHITE              0xFFFF

#define SCREEN_BUFFER_WIDTH      320
#define SCREEN_BUFFER_HEIGHT     240
#define SCREEN_BUFFER_BYTES      38400    // 4 bpp, so (width/2) * height
#define SCREEN_BUFFER_BYTES_X    160

#define PALETTE_COLORS           16

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
