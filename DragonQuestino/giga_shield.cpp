
#include "giga_shield.h"
#include "Adafruit_SPITFT.h"
#include "dsi.h"
#include "SDRAM.h"
#include "giga_shield_background_data.h"

#define PLAY_AREA_OFFSET_X    115
#define PLAY_AREA_OFFSET_Y    211

GigaShield::GigaShield() : Adafruit_GFX( GIGA_SHIELD_WIDTH, GIGA_SHIELD_HEIGHT )
{
}

GigaShield::~GigaShield()
{
   if ( _mainBuffer )
   {
      free( _mainBuffer );
   }

   if ( _wipeBuffer )
   {
      free( _wipeBuffer );
   }
}

void GigaShield::begin()
{
   _display = new Arduino_H7_Video( GIGA_SHIELD_WIDTH, GIGA_SHIELD_HEIGHT, GigaDisplayShield );
   _display->begin();
   _mainBuffer = (uint16_t*)ea_malloc( SCREEN_PIXELS * sizeof( uint16_t ) );
   memset( (void*)_mainBuffer, 0, SCREEN_PIXELS * sizeof( uint16_t ) );
   _wipeBuffer = (uint16_t*)ea_malloc( SCREEN_PIXELS * sizeof( uint16_t ) );
   memset( (void*)_wipeBuffer, 0, SCREEN_PIXELS * sizeof( uint16_t ) );
   
   uint32_t* b = (uint32_t*)( dsi_getActiveFrameBuffer() );
   Giga_LoadShieldBackground( b );

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

      if ( _isWiping )
      {
         dsi_lcdDrawImage( (void*)_wipeBuffer, (void*)shieldBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, DMA2D_INPUT_RGB565 );
      }
      else
      {
         dsi_lcdDrawImage( (void*)_mainBuffer, (void*)shieldBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, DMA2D_INPUT_RGB565 );
      }
   }
}

void GigaShield::drawScreen()
{
   _isWiping = false;
   _refreshThread->flags_set( 0x1 );
}

void GigaShield::wipeScreen( uint16_t color )
{
   if ( !_isWiping && _wipeBuffer[0] != color )
   {
      memset( _wipeBuffer, color, sizeof( uint16_t ) * SCREEN_PIXELS );
   }

   _isWiping = true;
   _refreshThread->flags_set( 0x1 );
}
