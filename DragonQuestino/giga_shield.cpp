
#include "giga_shield.h"
#include "Adafruit_SPITFT.h"
#include "dsi.h"
#include "SDRAM.h"

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
   _buffer = (uint16_t*)ea_malloc( SCREEN_PIXELS * sizeof( uint16_t ) );
   memset( (void*)_buffer, 0, SCREEN_PIXELS * sizeof( uint16_t ) );
   
   uint16_t* b = (uint16_t*)( dsi_getActiveFrameBuffer() );
   for ( int i = 0; i < GIGA_SHIELD_PIXELS; i++ )
   {
      b[i] = 0x4208; // dark grey-ish
   }

   _refreshThread = new rtos::Thread( osPriorityHigh );
   _refreshThread->start( mbed::callback( this, &GigaShield::refreshThreadWorker ) );
}

void GigaShield::refreshThreadWorker()
{
   while ( 1 )
   {
      rtos::ThisThread::flags_wait_any( 0x1 );
      
      uint16_t* shieldBuffer = (uint16_t*)( dsi_getActiveFrameBuffer() );
      shieldBuffer += ( GIGA_SHIELD_WIDTH * ( ( GIGA_SHIELD_HEIGHT - SCREEN_HEIGHT ) / 2 ) ) + ( ( GIGA_SHIELD_WIDTH - SCREEN_WIDTH ) / 2 );

      dsi_lcdDrawImage( (void*)_buffer, (void*)shieldBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, DMA2D_INPUT_RGB565 );
   }
}

void GigaShield::drawScreen()
{
   _refreshThread->flags_set( 0x1 );
}
