#include "game.h"
#include "random.h"
#include "math.h"

internal void Game_BattleIntroMessageCallback( Game_t* game );

void Game_Init( Game_t* game, uint16_t* screenBuffer )
{
   uint32_t i;

   Random_Seed();
   Screen_Init( &( game->screen ), screenBuffer );
   TileMap_Init( &( game->tileMap ), &( game->screen ), &( game->gameFlags ), &( game->player ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 1 );
   AnimationChain_Init( &( game->animationChain ), &( game->screen ), &( game->tileMap ), game );
   Sprite_LoadActive( &( game->player.sprite ), ACTIVE_SPRITE_PLAYER_ID );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ), &( game->tileMap ) );
   Battle_Init( &( game->battle ), game );
   Game_SetTextColor( game );

   game->activeMenu = 0;

   for ( i = 0; i < MenuId_Count; i++ )
   {
      Menu_Init( &( game->menus[(MenuId_t)i] ), (MenuId_t)( i ), &( game->screen ), &( game->player ), &( game->tileMap ) );
   }

   Dialog_Init( &( game->dialog ), &( game->screen ), &( game->mainState ) );

   for ( i = 0; i < TILEMAP_TOWN_COUNT; i++ )
   {
      game->zoomPortals[i].destinationTileMapIndex = TILEMAP_OVERWORLD_ID;
      game->zoomPortals[i].arrivalDirection = Direction_Down;
   }

   game->zoomPortals[TILEMAP_TANTEGEL_TOWN_ID].destinationTileIndex = TILEMAP_TANTEGEL_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_BRECCONARY_TOWN_ID].destinationTileIndex = TILEMAP_BRECCONARY_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_GARINHAM_TOWN_ID].destinationTileIndex = TILEMAP_GARINHAM_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_KOL_TOWN_ID].destinationTileIndex = TILEMAP_KOL_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_CANTLIN_TOWN_ID].destinationTileIndex = TILEMAP_CANTLIN_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_RIMULDAR_TOWN_ID].destinationTileIndex = TILEMAP_RIMULDAR_ZOOM_INDEX;

   game->gameFlags.treasures = 0xFFFFFFFF;
   game->gameFlags.doors = 0xFFFFFFFF;
   game->gameFlags.specialEnemies = 0xFF;
   game->gameFlags.usedRainbowDrop = False;
   game->gameFlags.foundHiddenStairs = False;
   game->mainState = MainState_Overworld;
   game->subState = SubState_None;
   game->targetPortal = 0;
   game->overworldInactivitySeconds = 0.0f;
   game->doAnimation = False;
}

void Game_Tic( Game_t* game )
{
   Bool_t runningCallback;

   game->doAnimation = game->animationChain.isRunning;
   Input_Read( &( game->input ) );

   runningCallback = ( game->animationChain.pendingCallback != 0 ) || ( game->dialog.pendingCallback != 0 );

   if ( game->animationChain.pendingCallback )
   {
      game->animationChain.pendingCallback( game->animationChain.pendingCallbackData );
      game->animationChain.pendingCallback = 0;
   }
   else if ( game->dialog.pendingCallback )
   {
      game->dialog.pendingCallback( game->dialog.pendingCallbackData );
      game->dialog.pendingCallback = 0;
   }

   if ( game->animationChain.isRunning )
   {
      AnimationChain_Tic( &( game->animationChain ) );
   }
   else if ( !runningCallback )
   {
      Game_HandleInput( game );

      if ( game->mainState == MainState_Overworld && game->subState == SubState_None )
      {
         Game_TicPhysics( game );
         ActiveSprite_Tic( &( game->player.sprite ) );
         TileMap_Tic( &(game->tileMap ) );
      }
      else
      {
         switch ( game->subState )
         {
            case SubState_Menu:
               Menu_Tic( game->activeMenu );
               break;
            case SubState_Dialog:
               Dialog_Tic( &( game->dialog ) );
               break;
         }
      }
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( game->tileMap.isDark )
   {
      if ( g_debugFlags.noDark )
      {
         TileMap_ChangeViewportSize( &( game->tileMap ), SCREEN_WIDTH, SCREEN_HEIGHT );
      }
      else
      {
         TileMap_ChangeViewportSize( &( game->tileMap ),
                                     TILE_SIZE * (uint16_t)( game->tileMap.glowDiameter ),
                                     TILE_SIZE * (uint16_t)( game->tileMap.glowDiameter ) );
      }
   }
#endif

   Game_Draw( game );
   Screen_RenderBuffer( &( game->screen ) );
}

void Game_ChangeToOverworldState( Game_t* game )
{
   game->mainState = MainState_Overworld;
   game->subState = SubState_None;
   game->overworldInactivitySeconds = 0.0f;
   game->screen.needsRedraw = True;
}

void Game_ChangeToBattleState( Game_t* game )
{
   game->mainState = MainState_Battle;
   game->subState = SubState_None;
   TileMap_UpdateViewport( &( game->tileMap ) );
   Game_DrawTileMap( game );
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Battle_Checkerboard );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Battle_EnemyFadeIn, Game_DrawEnemy, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_BattleIntroMessageCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

void Game_ChangeSubState( Game_t* game, SubState_t newState )
{
   game->subState = newState;
}

void Game_AnimatePortalEntrance( Game_t* game, TilePortal_t* portal )
{
   game->targetPortal = portal;
   Game_ChangeToOverworldState( game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeOut );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_EnterTargetPortal, game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeIn );
}

void Game_EnterTargetPortal( Game_t* game )
{
   uint32_t destinationTileIndex = game->targetPortal->destinationTileIndex;
   Direction_t arrivalDirection = game->targetPortal->arrivalDirection;

   switch ( game->targetPortal->destinationTileMapIndex )
   {
      case TILEMAP_TANTEGEL_ID: SET_VISITED_TANTEGEL( game->player.townsVisited ); break;
      case TILEMAP_BRECCONARY_ID: SET_VISITED_BRECCONARY( game->player.townsVisited ); break;
      case TILEMAP_GARINHAM_ID: SET_VISITED_GARINHAM( game->player.townsVisited ); break;
      case TILEMAP_KOL_ID: SET_VISITED_KOL( game->player.townsVisited ); break;
      case TILEMAP_CANTLIN_ID: SET_VISITED_CANTLIN( game->player.townsVisited ); break;
      case TILEMAP_RIMULDAR_ID: SET_VISITED_RIMULDAR( game->player.townsVisited ); break;
   }

   TileMap_Load( &( game->tileMap ), game->targetPortal->destinationTileMapIndex );

   game->player.sprite.position.x = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex % game->tileMap.tilesX ) ) - game->player.sprite.offset.x );
   game->player.sprite.position.y = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex / game->tileMap.tilesX ) ) - game->player.sprite.offset.y );
   game->player.tileIndex = destinationTileIndex;
   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), destinationTileIndex );
   game->targetPortal = 0;

   ActiveSprite_SetDirection( &( game->player.sprite ), arrivalDirection );

   if ( game->tileMap.isDark )
   {
      TileMap_ChangeViewportSize( &( game->tileMap ),
                                  (uint16_t)( game->tileMap.glowDiameter * TILE_SIZE ),
                                  (uint16_t)( game->tileMap.glowDiameter * TILE_SIZE ) );
   }
   else
   {
      TileMap_ResetViewport( &( game->tileMap ) );
   }
}

void Game_OpenMenu( Game_t* game, MenuId_t id )
{
   game->activeMenu = &( game->menus[(int32_t)id] );
   Menu_Reset( game->activeMenu );
   Game_ChangeSubState( game, SubState_Menu );
}

void Game_OpenDialog( Game_t* game )
{
   Dialog_Start( &( game->dialog ) );
   Game_ChangeSubState( game, SubState_Dialog );
}

void Game_RestoredHitPointsCallback( Game_t* game )
{
   Player_RestoreHitPoints( &( game->player ), game->pendingPayload8u );
   game->screen.needsRedraw = True;

   if ( game->mainState == MainState_Battle )
   {
      Battle_SwitchTurn( &( game->battle ) );
   }
}

void Game_CursedCallback( Game_t* game )
{
   Player_SetCursed( &( game->player ), True );
   game->screen.needsRedraw = True;
}

void Game_ResetBattleMenu( Game_t* game )
{
   if ( game->mainState == MainState_Battle )
   {
      game->screen.needsRedraw = True;
      game->activeMenu = &( game->menus[MenuId_Battle] );
      Menu_Reset( game->activeMenu );
      Game_ChangeSubState( game, SubState_Menu );
   }
}

internal void Game_BattleIntroMessageCallback( Game_t* game )
{
   char enemyName[24];
   char msg[64];

   Dialog_Reset( &( game->dialog ) );
   sprintf( enemyName, "%s %s", game->battle.enemy.indefiniteArticle, game->battle.enemy.name );

   if ( Battle_RollEnemyFlee( &( game->battle ) ) )
   {
      sprintf( msg, STRING_BATTLE_ENEMYAPPROACHES, enemyName );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Battle_EnemyInitiativeFlee, &( game->battle ) );
   }
   if ( game->battle.turn == BattleTurn_Player )
   {
      sprintf( msg, STRING_BATTLE_ENEMYAPPROACHESINITIATIVE, enemyName );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
   }
   else
   {
      sprintf( msg, STRING_BATTLE_ENEMYAPPROACHES, enemyName );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Battle_EnemyInitiative, &( game->battle ) );
   }
   
   Game_OpenDialog( game );
}
