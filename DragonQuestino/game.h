#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "screen.h"
#include "tile_map.h"
#include "clock.h"
#include "input.h"
#include "player.h"
#include "menu.h"
#include "scrolling_dialog.h"

#define OVERWORLD_INACTIVE_STATUS_SECONDS          1.0f
#define GLOW_EXPAND_FRAME_SECONDS                  0.1f

#define ANIMATION_OVERWORLD_PAUSE_DURATION         0.3f
#define ANIMATION_TILEMAP_FADE_DURATION            0.3f
#define ANIMATION_TILEMAP_FADEPAUSE_DURATION       0.2f
#define ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION    6.0f
#define ANIMATION_RAINBOWBRIDGE_WHITEOUT_DURATION  1.5f
#define ANIMATION_RAINBOWBRIDGE_FADEIN_DURATION    1.5f
#define ANIMATION_RAINBOWBRIDGE_PAUSE_DURATION     1.5f

#define COLLISION_THETA                            0.001f

typedef struct Game_t
{
   TilePortal_t* targetPortal;

   Screen_t screen;
   TileMap_t tileMap;
   Clock_t clock;
   Input_t input;
   GameState_t state;
   Player_t player;
   Menu_t menu;
   ScrollingDialog_t scrollingDialog;

   float overworldInactivitySeconds;
   float glowExpandSeconds;

   Bool_t isAnimating;
   Animation_t animation;
   float animationSeconds;
   float animationDuration;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

// game.c
void Game_Init( Game_t* game, uint16_t* screenBuffer );
void Game_Tic( Game_t* game );
void Game_ChangeState( Game_t* game, GameState_t newState );
void Game_EnterTargetPortal( Game_t* game );
void Game_OpenMenu( Game_t* game, MenuId_t id );
void Game_OpenScrollingDialog( Game_t* game, ScrollingDialogType_t type, DialogMessageId_t messageId );
void Game_Search( Game_t* game );
void Game_OpenDoor( Game_t* game );

// game_input.c
void Game_HandleInput( Game_t* game );

// game_animation.c
void Game_StartAnimation( Game_t* game, Animation_t animation );
void Game_StopAnimation( Game_t* game );
void Game_TicAnimation( Game_t* game );

// game_physics.c
void Game_TicPhysics( Game_t* game );
void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex );

// game_render.c
void Game_Draw( Game_t* game );
void Game_DrawOverworldQuickStatus( Game_t* game );
void Game_DrawOverworldDeepStatus( Game_t* game );
void Game_DrawNonUseableItems( Game_t* game, Bool_t hasUseableItems );

// game_spells.c
void Game_CastHeal( Game_t* game );
void Game_CastGlow( Game_t* game );
void Game_CastEvac( Game_t* game );
void Game_CastZoom( Game_t* game );
void Game_CastRepel( Game_t* game );
void Game_CastMidheal( Game_t* game );

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
