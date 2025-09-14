#include "screen.h"
#include "win_common.h"

internal u32 Convert565To32( u16 color );
internal void Screen_Wipe( Screen_t* screen );

void Screen_RenderBuffer( Screen_t* screen )
{
   u32 i;
   u16* bufferPos16 = screen->buffer;
   u32* bufferPos32 = g_globals.screenBuffer.memory32;

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

internal u32 Convert565To32( u16 color )
{
   if ( color == 0 )
   {
      return 0;
   }

   u16 r5 = ( ( color & 0xF800 ) >> 11 );
   u16 g6 = ( ( color & 0x07E0 ) >> 5 );
   u16 b5 = ( ( color & 0x001F ) );

   r32 pR = ( r5 == 0 ) ? 0.0f : (r32)r5 / 0x1F;
   r32 pG = ( g6 == 0 ) ? 0.0f : (r32)g6 / 0x3F;
   r32 pB = ( b5 == 0 ) ? 0.0f : (r32)b5 / 0x1F;

   return (u32)0xFF000000 | ( (u32)( 0xFF * pR ) << 16 ) | ( (u32)( 0xFF * pG ) << 8 ) | (u32)( 0xFF * pB );
}

internal void Screen_Wipe( Screen_t* screen )
{
   u32 color = Convert565To32( screen->wipeColor );

   for ( int i = 0; i < SCREEN_PIXELS; i++ )
   {
      g_globals.screenBuffer.memory32[i] = color;
   }
}
