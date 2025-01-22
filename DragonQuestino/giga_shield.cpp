
#include "giga_shield.h"
#include "Adafruit_SPITFT.h"
#include "dsi.h"
#include "SDRAM.h"
#include "screen.h"

GigaShield::GigaShield() : Adafruit_GFX( GIGA_SHIELD_WIDTH, GIGA_SHIELD_HEIGHT )
{
}

GigaShield::~GigaShield()
{
   if ( _buffer )
   {
      free( _buffer );
   }
}

void GigaShield::begin()
{
   _display = new Arduino_H7_Video( GIGA_SHIELD_WIDTH, GIGA_SHIELD_HEIGHT, GigaDisplayShield );
   _display->begin();
   _buffer = (uint16_t*)ea_malloc( GIGA_SHIELD_WIDTH * GIGA_SHIELD_HEIGHT * 2 );

   // TODO: add some kind of cool border, like a TV screen or something
   memset( (void*)_buffer, 0, GIGA_SHIELD_WIDTH * GIGA_SHIELD_HEIGHT * 2 );
}

void GigaShield::drawScreen( Screen_t* screen )
{
   int shieldXPadding = ( GIGA_SHIELD_WIDTH - SCREEN_BUFFER_HEIGHT ) / 2;
   int shieldYPadding = ( GIGA_SHIELD_HEIGHT - SCREEN_BUFFER_WIDTH ) / 2;
   uint8_t* screenBufferPos = screen->buffer;
   // this should be the upper-left corner of the buffer if the shield is rotated
   uint16_t* shieldBufferPos = _buffer + ( ( shieldYPadding * GIGA_SHIELD_WIDTH ) + shieldXPadding + SCREEN_BUFFER_HEIGHT );

   for ( int i = 0, col = 0; i < SCREEN_BUFFER_PIXELS; i++ )
   {
      *shieldBufferPos = screen->palette[*screenBufferPos];
      screenBufferPos++;
      shieldBufferPos += GIGA_SHIELD_WIDTH;
      col++;

      if ( col == SCREEN_BUFFER_WIDTH )
      {
         shieldBufferPos -= ( ( SCREEN_BUFFER_WIDTH * GIGA_SHIELD_WIDTH ) + 1 );
         col = 0;
      }
   }

   dsi_lcdDrawImage( (void*)_buffer, (void*)( dsi_getActiveFrameBuffer() ), GIGA_SHIELD_WIDTH, GIGA_SHIELD_HEIGHT, DMA2D_INPUT_RGB565 );
}
