
#include "giga_shield.h"
#include "Adafruit_SPITFT.h"
#include "dsi.h"
#include "SDRAM.h"
#include "screen.h"

GigaShield::GigaShield() : Adafruit_GFX( SCREEN_WIDTH, SCREEN_HEIGHT )
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
   _display = new Arduino_H7_Video( SCREEN_WIDTH, SCREEN_HEIGHT, GigaDisplayShield );
   _display->begin();
   _buffer = (uint16_t*)ea_malloc( SCREEN_WIDTH * SCREEN_HEIGHT * 2 );

   // TODO: add some kind of cool border around the play area, like a big CRT TV or something
   memset( (void*)_buffer, 0, SCREEN_PIXELS * 2 );

   _refreshThread = new rtos::Thread( osPriorityHigh );
   _refreshThread->start( mbed::callback( this, &GigaShield::refreshThreadWorker ) );
}

void GigaShield::refreshThreadWorker()
{
   while ( 1 )
   {
      rtos::ThisThread::flags_wait_any( 0x1 );
      dsi_lcdDrawImage( (void *)_buffer, (void *)( dsi_getActiveFrameBuffer() ), SCREEN_WIDTH, SCREEN_HEIGHT, DMA2D_INPUT_RGB565 );
   }
}

void GigaShield::drawScreen()
{
   _refreshThread->flags_set( 0x1 );
}
