#include "screen.h"
#include "win_common.h"

internal uint32_t Convert565To32( uint16_t color )
{
   if ( color == 0 )
   {
      return 0;
   }

   uint16_t r5 = ( ( color & 0xF800 ) >> 11 );
   uint16_t g6 = ( ( color & 0x07E0 ) >> 5 );
   uint16_t b5 = ( ( color & 0x001F ) );

   float pR = ( r5 == 0 ) ? 0.0f : (float)r5 / 0x1F;
   float pG = ( g6 == 0 ) ? 0.0f : (float)g6 / 0x3F;
   float pB = ( b5 == 0 ) ? 0.0f : (float)b5 / 0x1F;

   return (uint32_t)0xFF000000 | ( (uint32_t)( 0xFF * pR ) << 16 ) | ( (uint32_t)( 0xFF * pG ) << 8 ) | (uint32_t)( 0xFF * pB );
}

void Screen_RenderBuffer( Screen_t* screen )
{
   uint8_t *bufferPos = screen->buffer;
   uint16_t x, y, color16;
   uint32_t color32;
   uint32_t* winBufferPos = g_globals.screenBuffer.memory;

   for ( y = 0; y < SCREEN_BUFFER_HEIGHT; y++ )
   {
      for ( x = 0; x < SCREEN_BUFFER_WIDTH; x++ )
      {
         color16 = screen->palette[*bufferPos];
         color32 = Convert565To32( color16 );
         *winBufferPos = color32;
         winBufferPos++;
         bufferPos++;
      }
   }
}
