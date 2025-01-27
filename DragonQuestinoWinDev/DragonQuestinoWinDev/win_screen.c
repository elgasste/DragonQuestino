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
   uint32_t i, j;
   uint16_t* playAreaBufferPos16 = screen->playAreaBufferPos;
   uint32_t* playAreaBufferPos32 = g_globals.screenBuffer.playAreaMemoryPos32;

   for ( i = 0; i < PLAY_AREA_HEIGHT; i++ )
   {
      for ( j = 0; j < PLAY_AREA_WIDTH; j++ )
      {
         *playAreaBufferPos32 = Convert565To32( *playAreaBufferPos16 );
         playAreaBufferPos16++;
         playAreaBufferPos32++;
      }

      playAreaBufferPos16 += ( SCREEN_WIDTH - PLAY_AREA_WIDTH );
      playAreaBufferPos32 += ( SCREEN_WIDTH - PLAY_AREA_WIDTH );
   }
}
