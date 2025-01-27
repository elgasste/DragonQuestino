#include "screen.h"

void Screen_Init( Screen_t* screen, uint16_t* screenBuffer )
{
   screen->screenBuffer = screenBuffer;
   screen->playAreaBufferPos = screenBuffer +
      (
         ( ( ( SCREEN_HEIGHT - PLAY_AREA_HEIGHT ) / 2 ) * SCREEN_WIDTH ) +
         ( ( SCREEN_WIDTH - PLAY_AREA_WIDTH ) / 2 )
      );

   Screen_LoadPalette( screen );
}

Bool_t Screen_GetPaletteIndexForColor( Screen_t* screen, uint16_t color, uint8_t* paletteIndex )
{
   uint8_t i;

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      if ( screen->palette[i] == color )
      {
         *paletteIndex = i;
         return True;
      }
   }

   return False;
}

void Screen_Wipe( Screen_t* screen, uint8_t paletteIndex )
{
   uint32_t i, j;
   uint16_t* bufferPos = screen->playAreaBufferPos;

   for ( i = 0; i < PLAY_AREA_HEIGHT; i++ )
   {
      for ( j = 0; j < PLAY_AREA_WIDTH; j++ )
      {
         *bufferPos = screen->palette[paletteIndex];
         bufferPos++;
      }

      bufferPos += ( SCREEN_WIDTH - PLAY_AREA_WIDTH );
   }
}
