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

   pinMode( PIN_A_BUTTON, INPUT_PULLUP );
   pinMode( PIN_B_BUTTON, INPUT_PULLUP );

   g_gigaShield.begin();
   Game_Init( &g_game, g_gigaShield.getBuffer() );
   g_gigaShield.setScreen( &( g_game.screen ) );
}

void loop()
{
   Clock_StartFrame( &( g_game.clock ) );
   Game_Tic( &g_game );
   Clock_EndFrame( &( g_game.clock ) );
}

void Screen_RenderBuffer( Screen_t* screen )
{
   g_gigaShield.drawScreen();
}
