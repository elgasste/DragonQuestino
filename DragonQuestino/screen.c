#include "screen.h"

void Screen_Init( Screen_t* screen )
{
   uint32_t i;

   for ( i = 0; i < SCREEN_BUFFER_BYTES; i++ )
   {
      screen->buffer[i] = 0;
   }

   Screen_LoadPalette( screen );
}
