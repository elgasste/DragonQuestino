#include "game.h"

Game_t g_game;

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

   Game_Init( &g_game );
}

void loop()
{
   Clock_StartFrame( &( g_game.clock ) );
   Game_Tic( &g_game );
   Clock_EndFrame( &( g_game.clock ) );
}
