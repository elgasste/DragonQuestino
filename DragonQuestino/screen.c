#include "screen.h"

void Screen_Init( Screen_t* screen )
{
   uint32_t i;

   for ( i = 0; i < SCREEN_BUFFER_PIXELS; i++ )
   {
      screen->buffer[i] = 0;
   }

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
   uint32_t i;

   for ( i = 0; i < SCREEN_BUFFER_PIXELS; i++ )
   {
      screen->buffer[i] = paletteIndex;
   }
}
