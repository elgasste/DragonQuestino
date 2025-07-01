#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "screen.h"
#include "game_flags.h"
#include "tile_map.h"
#include "clock.h"
#include "input.h"
#include "player.h"
#include "menu.h"
#include "dialog.h"
#include "animation_chain.h"
#include "battle.h"

#define OVERWORLD_INACTIVE_STATUS_SECONDS          1.0f

#define COLLISION_THETA                            0.001f

#define ENCOUNTERRATE_LOW                          24
#define ENCOUNTERRATE_MEDIUM                       16
#define ENCOUNTERRATE_HIGH                         8

typedef struct Game_t
{
   TilePortal_t* targetPortal;

   Screen_t screen;
   GameFlags_t gameFlags;
   TileMap_t tileMap;
   Clock_t clock;
   Input_t input;
   MainState_t mainState;
   SubState_t subState;
   Player_t player;
   Menu_t menus[MenuId_Count];
   Menu_t* activeMenu;
   Dialog_t dialog;
   TilePortal_t zoomPortals[TILEMAP_TOWN_COUNT];
   AnimationChain_t animationChain;
   Battle_t battle;

   float overworldInactivitySeconds;
   Bool_t doAnimation;

   uint8_t pendingPayload8u;
   Spell_t pendingSpell;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

// game.c
void Game_Init( Game_t* game, uint16_t* screenBuffer );
void Game_Tic( Game_t* game );
void Game_ChangeToOverworldState( Game_t* game );
void Game_ChangeToBattleState( Game_t* game );
void Game_ChangeSubState( Game_t* game, SubState_t newState );
void Game_EnterTargetPortal( Game_t* game );
void Game_AnimatePortalEntrance( Game_t* game, TilePortal_t* portal );
void Game_OpenMenu( Game_t* game, MenuId_t id );
void Game_OpenDialog( Game_t* game );
void Game_RestoredHitPointsCallback( Game_t* game );
void Game_CursedCallback( Game_t* game );
void Game_ResetBattleMenu( Game_t* game );

// game_actions.c
void Game_Talk( Game_t* game );
void Game_Search( Game_t* game );
void Game_OpenDoor( Game_t* game );

// game_input.c
void Game_HandleInput( Game_t* game );

// game_physics.c
void Game_TicPhysics( Game_t* game );
void Game_PlayerSteppedOnTile( Game_t* game );

// game_render.c
void Game_Draw( Game_t* game );
void Game_DrawOverworld( Game_t* game );
void Game_DrawQuickStatus( Game_t* game );
void Game_DrawOverworldDeepStatus( Game_t* game );
void Game_DrawOverworldItemMenu( Game_t* game );
void Game_DrawEnemy( Game_t* game );
void Game_WipeEnemy( Game_t* game );
void Game_SetTextColor( Game_t* game );
void Game_DrawTileMap( Game_t* game );

// game_spells.c
void Game_CastHeal( Game_t* game );
void Game_CastSizz( Game_t* game );
void Game_CastSleep( Game_t* game );
void Game_CastGlow( Game_t* game );
void Game_CastFizzle( Game_t* game );
void Game_CastEvac( Game_t* game );
void Game_CastZoom( Game_t* game, uint32_t townId );
void Game_CastRepel( Game_t* game );
void Game_CastMidheal( Game_t* game );
void Game_CastSizzle( Game_t* game );
void Game_SpellSleepSuccessCallback( Game_t* game );

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

// game_npcs.c
void Game_RunNpcDialog( Game_t* game, uint32_t npcId );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
