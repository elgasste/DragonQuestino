#if !defined( WIN_PIXEL_BUFFER_H )
#define WIN_PIXEL_BUFFER_H

#include "common.h"

typedef struct WinPixelBuffer_t
{
   uint32_t w;
   uint32_t h;
   uint32_t* memory;
}
WinPixelBuffer_t;

void WinPixelBuffer_Init( WinPixelBuffer_t* buffer, uint32_t w, uint32_t h );
void WinPixelBuffer_CleanUp( WinPixelBuffer_t* buffer );

#endif // WIN_PIXEL_BUFFER_H
