#include "screen.h"

void Screen_RenderBuffer( Screen_t* screen )
{
   uint8_t x, y, paletteIndex, *bufferPos = screen->buffer;
   uint16_t color;

   // TODO: It needs to be this way in Windows, but on Arduino it can probably be consolidated,
   // as long as SetAddrWindow is still a thing. I guess we'll see.
   for ( y = 0; y < SCREEN_BUFFER_HEIGHT; y++ )
   {
      for ( x = 0; x < SCREEN_BUFFER_BYTES_X; x++ )
      {
         color = screen->palette[( *bufferPos & 0xF0 ) >> 4];
         // TODO: render it at the correct position (remember x needs to be scaled)
         color = screen->palette[( *bufferPos & 0xF )];
         // TODO: render it at the correct position (remember x needs to be scaled)

         bufferPos++;
      }
   }
}
