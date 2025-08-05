#include "giga_shield.h"
#include "game.h"

Game_t g_game;
GigaShield g_gigaShield;

void Program_Log( const char* msg )
{
#if defined( DEBUG_SERIAL )
   Serial.println( msg );
#endif
}

void setup()
{
#if defined( DEBUG_SERIAL )
   Serial.begin( SERIAL_BAUD );
#endif

   g_gigaShield.begin();
   Game_Init( &g_game, g_gigaShield.getBuffer() );
   g_gigaShield.setScreen( &( g_game.screen ) );

   // for NES controller input
   pinMode( INPUT_NES_DATA_PIN, INPUT );
  
   pinMode( INPUT_NES_CLOCK_PIN, OUTPUT );
   pinMode( INPUT_NES_LATCH_PIN, OUTPUT );
  
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );
   digitalWrite( INPUT_NES_LATCH_PIN, LOW );
}

void loop()
{
   Clock_StartFrame( &( g_game.clock ) );
   Game_Tic( &g_game );
   Clock_EndFrame( &( g_game.clock ) );
}

void Screen_RenderBuffer( Screen_t* screen )
{
   if ( screen->needsWipe )
   {
      g_gigaShield.wipeScreen( screen->wipeColor );
   }
   else
   {
      g_gigaShield.drawScreen();
   }
}

uint8_t Input_ReadNesController()
{
   uint32_t i;
   uint8_t result = 0;

   digitalWrite( INPUT_NES_LATCH_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_LATCH_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_A_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_B_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_SELECT_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_START_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_UP_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_DOWN_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_LEFT_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );
   
   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_RIGHT_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   return result;
}
