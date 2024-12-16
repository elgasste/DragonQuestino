#include "screen.h"

void Screen_RenderBuffer( Screen_t* screen )
{
   uint32_t x, y, paletteIndex;
   uint16_t color;
   uint8_t* bufferPos = screen->buffer;

   // TODO: It needs to be this way in Windows, but on Arduino it can probably be consolidated,
   // as long as SetAddrWindow is still a thing. I guess we'll see.
   for ( y = 0; y < SCREEN_BUFFER_HEIGHT; y++ )
   {
      for ( x = 0; x < SCREEN_BUFFER_WIDTH; x++ )
      {
         color = screen->palette[*bufferPos];
         // TODO: render it at the correct position
         bufferPos++;
      }
   }
}
