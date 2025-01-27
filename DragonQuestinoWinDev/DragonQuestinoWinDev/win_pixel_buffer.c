#include "win_pixel_buffer.h"
#include "win_common.h"

void WinPixelBuffer_Init( WinPixelBuffer_t* buffer, uint32_t w, uint32_t h )
{
   buffer->w = w;
   buffer->h = h;
   buffer->memory16 = (uint16_t*)calloc( w * h, sizeof( uint16_t ) );
   buffer->memory32 = (uint32_t*)calloc( w * h, sizeof( uint32_t ) );
   buffer->playAreaMemoryPos32 = buffer->memory32 +
      (
         ( ( ( SCREEN_HEIGHT - PLAY_AREA_HEIGHT ) / 2 ) * SCREEN_WIDTH ) +
         ( ( SCREEN_WIDTH - PLAY_AREA_WIDTH ) / 2 )
      );
}

void WinPixelBuffer_CleanUp( WinPixelBuffer_t* buffer )
{
   free( buffer->memory16 );
   free( buffer->memory32 );
   buffer->memory16 = 0;
   buffer->memory32 = 0;
   buffer->w = 0;
   buffer->h = 0;
}
