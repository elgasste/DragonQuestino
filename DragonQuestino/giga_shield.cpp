
#include "giga_shield.h"
#include "Adafruit_SPITFT.h"
#include "dsi.h"
#include "SDRAM.h"
#include "screen.h"

GigaShield::GigaShield() : Adafruit_GFX( SCREEN_BUFFER_WIDTH, SCREEN_BUFFER_HEIGHT )
{
}

GigaShield::~GigaShield()
{
   if ( _buffer )
   {
      free( _buffer );
   }
}

void GigaShield::begin( Screen_t* screen )
{
   _screen = screen;
   _display = new Arduino_H7_Video( SCREEN_BUFFER_WIDTH, SCREEN_BUFFER_HEIGHT, GigaDisplayShield );
   _display->begin();
   _buffer = (uint16_t*)ea_malloc( SCREEN_BUFFER_WIDTH * SCREEN_BUFFER_HEIGHT * 2 );

   // TODO: add some kind of cool border, like a TV screen or something
   memset( (void*)_buffer, 0, SCREEN_BUFFER_PIXELS * 2 );

   _refreshThread = new rtos::Thread( osPriorityHigh );
   _refreshThread->start( mbed::callback( this, &GigaShield::refreshThreadWorker ) );
}

void GigaShield::refreshThreadWorker()
{
   while ( 1 )
   {
      rtos::ThisThread::flags_wait_any( 0x1 );

      int shieldXPadding = ( SCREEN_BUFFER_WIDTH - VIEWPORT_WIDTH ) / 2;
      int shieldYPadding = ( SCREEN_BUFFER_HEIGHT - VIEWPORT_HEIGHT ) / 2;
      uint8_t* screenBufferPos = _screen->buffer;
      uint16_t* shieldBufferPos = _buffer + ( ( shieldYPadding * SCREEN_BUFFER_WIDTH ) + shieldXPadding );

      for ( int i = 0, col = 0; i < VIEWPORT_PIXELS; i++ )
      {
         *shieldBufferPos = _screen->palette[*screenBufferPos];
         screenBufferPos++;
         shieldBufferPos++;
         col++;

         if ( col == VIEWPORT_WIDTH )
         {
            shieldBufferPos += ( SCREEN_BUFFER_WIDTH - VIEWPORT_WIDTH );
            col = 0;
         }
      }

      dsi_lcdDrawImage( (void *)_buffer, (void *)( dsi_getActiveFrameBuffer() ), SCREEN_BUFFER_WIDTH, SCREEN_BUFFER_HEIGHT, DMA2D_INPUT_RGB565 );
   }
}

void GigaShield::drawScreen()
{
   _refreshThread->flags_set( 0x1 );
}
