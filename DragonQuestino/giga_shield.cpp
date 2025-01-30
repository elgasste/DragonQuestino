
#include "giga_shield.h"
#include "Adafruit_SPITFT.h"
#include "dsi.h"
#include "SDRAM.h"

#define PLAY_AREA_OFFSET_X    112
#define PLAY_AREA_OFFSET_Y    203

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
   
   uint32_t* b = (uint32_t*)( dsi_getActiveFrameBuffer() );
   Screen_LoadShieldBackground( b );

   _refreshThread = new rtos::Thread( osPriorityHigh );
   _refreshThread->start( mbed::callback( this, &GigaShield::refreshThreadWorker ) );
}

void GigaShield::refreshThreadWorker()
{
   while ( 1 )
   {
      rtos::ThisThread::flags_wait_any( 0x1 );
      
      uint16_t* shieldBuffer = (uint16_t*)( dsi_getActiveFrameBuffer() );
      shieldBuffer += ( ( GIGA_SHIELD_WIDTH * PLAY_AREA_OFFSET_Y ) + PLAY_AREA_OFFSET_X );

      dsi_lcdDrawImage( (void*)_buffer, (void*)shieldBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, DMA2D_INPUT_RGB565 );
   }
}

void GigaShield::drawScreen()
{
   _refreshThread->flags_set( 0x1 );
}
