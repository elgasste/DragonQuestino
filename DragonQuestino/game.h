#if !defined( GAME_H )
#define GAME_H

#define TILEMAP_SWAP_SECONDS                 0.4f
#define OVERWORLD_INACTIVE_STATUS_SECONDS    1.0f
#define OVERWORLD_LIGHTING_FRAME_SECONDS     0.1f
#define OVERWORLD_WASH_TOTAL_SECONDS         0.2f
#define COLLISION_THETA                      0.001f

#include "common.h"
#include "screen.h"
#include "tile_map.h"
#include "clock.h"
#include "input.h"
#include "player.h"
#include "menu.h"
#include "scrolling_dialog.h"

typedef struct Game_t
{
   Screen_t screen;
   TileMap_t tileMap;
   Clock_t clock;
   Input_t input;
   GameState_t state;
   Player_t player;
   Menu_t menu;
   ScrollingDialog_t scrollingDialog;

   float overworldInactivitySeconds;
   float overworldWashSeconds;

   TilePortal_t* swapPortal;
   float tileMapSwapSecondsElapsed;

   float lightingSecondsElapsed;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

// game.c
void Game_Init( Game_t* game, uint16_t* screenBuffer );
void Game_Tic( Game_t* game );
void Game_ChangeState( Game_t* game, GameState_t newState );
void Game_Search( Game_t* game );

// game_input.c
void Game_HandleInput( Game_t* game );

// game_physics.c
void Game_TicPhysics( Game_t* game );

// game_render.c
void Game_Draw( Game_t* game );
void Game_DrawOverworldQuickStatus( Game_t* game );
void Game_DrawOverworldDeepStatus( Game_t* game );
void Game_DrawNonUseableItems( Game_t* game );

// game_items.c
void Game_UseHerb( Game_t* game );
void Game_UseWing( Game_t* game );
void Game_UseFairyWater( Game_t* game );
void Game_UseTorch( Game_t* game );
void Game_UseFairyFlute( Game_t* game );
void Game_UseSilverHarp( Game_t* game );
void Game_UseGwaelynsLove( Game_t* game );
void Game_UseRainbowDrop( Game_t* game );
void Game_UseCursedBelt( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
