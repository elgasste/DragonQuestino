#include "screen.h"

void Screen_Init( Screen_t* screen, uint16_t* buffer )
{
   screen->buffer = buffer;
   Screen_LoadPalette( screen );
   Screen_LoadTextBitFields( screen );
}

Bool_t Screen_GetPaletteIndexForColor( Screen_t* screen, uint16_t color, uint32_t* paletteIndex )
{
   uint32_t i;

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

void Screen_Wipe( Screen_t* screen, uint32_t paletteIndex )
{
   uint32_t i;
   uint16_t* bufferPos = screen->buffer;

   for ( i = 0; i < SCREEN_PIXELS; i++ )
   {
      *bufferPos = screen->palette[paletteIndex];
      bufferPos++;
   }
}
