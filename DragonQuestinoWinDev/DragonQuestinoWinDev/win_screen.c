#include "screen.h"
#include "win_common.h"

internal uint32_t Convert565To32( uint16_t color );
internal void Screen_Wipe( Screen_t* screen );

void Screen_RenderBuffer( Screen_t* screen )
{
   uint32_t i;
   uint16_t* bufferPos16 = screen->buffer;
   uint32_t* bufferPos32 = g_globals.screenBuffer.memory32;

   if ( screen->needsWipe )
   {
      Screen_Wipe( screen );
   }
   else
   {
      for ( i = 0; i < SCREEN_PIXELS; i++ )
      {
         *bufferPos32 = Convert565To32( *bufferPos16 );
         bufferPos16++;
         bufferPos32++;
      }
   }
}

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

internal void Screen_Wipe( Screen_t* screen )
{
   uint32_t color = Convert565To32( screen->wipeColor );

   for ( int i = 0; i < SCREEN_PIXELS; i++ )
   {
      g_globals.screenBuffer.memory32[i] = color;
   }
}
