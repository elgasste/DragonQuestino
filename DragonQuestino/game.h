#if !defined( GAME_H )
#define GAME_H

#define TILEMAP_SWAP_SECONDS 0.4f

#include "common.h"
#include "screen.h"
#include "tile_map.h"
#include "clock.h"
#include "input.h"
#include "player.h"
#include "vector.h"

typedef struct Game_t
{
   Screen_t screen;
   TileMap_t tileMap;
   Clock_t clock;
   Input_t input;
   GameState_t state;
   Player_t player;

   Vector4i32_t tileMapViewport;

   TilePortal_t* swapPortal;
   float tileMapSwapSecondsElapsed;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Game_Init( Game_t* game, uint16_t* screenBuffer );
void Game_Tic( Game_t* game );
void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
