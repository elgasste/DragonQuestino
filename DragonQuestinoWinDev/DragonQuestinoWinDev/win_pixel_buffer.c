#include "win_pixel_buffer.h"
#include "win_common.h"

void WinPixelBuffer_Init( WinPixelBuffer_t* buffer, u32 w, u32 h )
{
   buffer->w = w;
   buffer->h = h;
   buffer->memory16 = (u16*)calloc( w * h, sizeof( u16 ) );
   buffer->memory32 = (u32*)calloc( w * h, sizeof( u32 ) );
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
