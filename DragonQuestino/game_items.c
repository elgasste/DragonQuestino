#include "game.h"
#include "random.h"
#include "math.h"

internal void Game_UseWingCallback( Game_t* game );
internal void Game_UseRainbowDropCallback( Game_t* game );
internal void Game_RainbowDropTrippyCallback( Game_t* game );
internal void Game_UseSilverHarpCallback( Game_t* game );
internal void Game_UseFairyFluteCallback( Game_t* game );
internal void Game_UseFairyFluteMessageCallback( Game_t* game );
internal void Game_UseTorchCallback( Game_t* game );
internal void Game_UseTorchResultCallback( Game_t* game );

void Game_UseHerb( Game_t* game )
{
   uint8_t restoredHitPoints;
   char msg[64];

   Dialog_Reset( &( game->dialog ) );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Dialog_PushSectionWithCallback( &( game->dialog ),
                                      ( game->mainState == MainState_Overworld ) ? STRING_FULLYHEALED : STRING_BATTLE_FULLYHEALED,
                                      ( game->mainState == MainState_Overworld ) ? 0 : Game_ResetBattleMenu,
                                      ( game->mainState == MainState_Overworld ) ? 0 : game );
   }
   else
   {
      ITEM_SET_HERBCOUNT( game->player.items, ITEM_GET_HERBCOUNT( game->player.items ) - 1 );
      restoredHitPoints = Math_Min8u( Random_u8( ITEM_HERB_MINEFFECT, ITEM_HERB_MAXEFFECT ), game->player.stats.maxHitPoints - game->player.stats.hitPoints );
      game->pendingPayload8u = restoredHitPoints;
      Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_HERB );
      sprintf( msg, STRING_DIALOG_HEAL_RESULT, restoredHitPoints, ( restoredHitPoints == 1 ) ? STRING_POINT : STRING_POINTS );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_RestoredHitPointsCallback, game );
   }

   Game_OpenDialog( game );
}

void Game_UseWing( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );

   if ( game->tileMap.isDungeon )
   {
      Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_WING_CANTUSE );
   }
   else
   {
      ITEM_SET_WINGCOUNT( game->player.items, ITEM_GET_WINGCOUNT( game->player.items ) - 1 );
      game->targetPortal = &( game->zoomPortals[TILEMAP_TANTEGEL_TOWN_ID] );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMUSE_WING, Game_UseWingCallback, game );
   }

   Game_OpenDialog( game );
}

void Game_UseFairyWater( Game_t* game )
{
   ITEM_SET_FAIRYWATERCOUNT( game->player.items, ITEM_GET_FAIRYWATERCOUNT( game->player.items ) - 1 );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_FAIRYWATER );

   if ( game->player.isCursed )
   {
      Dialog_PushSection( &( game->dialog ), STRING_HOLYPROTECTION_CURSED );
   }
   else
   {
      Player_SetHolyProtection( &( game->player ), True );
      Dialog_PushSection( &( game->dialog ), STRING_HOLYPROTECTION_ON );
   }

   Game_OpenDialog( game );
}

void Game_UseTorch( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );

   if ( game->tileMap.isDark )
   {
      if ( game->tileMap.torchIsLit )
      {
         Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_TORCH_ALREADYUSED );
      }
      else
      {
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMUSE_TORCH, Game_UseTorchCallback, game );
      }
   }
   else
   {
      Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_TORCH_CANTUSE );
   }

   Game_OpenDialog( game );
}

void Game_UseSilverHarp( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_SILVERHARP_1 );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMUSE_SILVERHARP_2, Game_UseSilverHarpCallback, game );
   Game_OpenDialog( game );
}

void Game_UseFairyFlute( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_FAIRYFLUTE_1 );

   if ( game->mainState == MainState_Battle )
   {
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMUSE_FAIRYFLUTE_2, Game_UseFairyFluteCallback, game );
   }
   else
   {
      Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_FAIRYFLUTE_2 );
   }

   Game_OpenDialog( game );
}

void Game_UseGwaelynsLove( Game_t* game )
{
   uint32_t px, py, tx, ty, exp;
   char msg[128], loc[64];

   Dialog_Reset( &( game->dialog ) );

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID )
   {
      sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_1, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );

      exp = Player_GetExperienceRemaining( &( game->player ) );

      if ( exp > 0 )
      {
         sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_2, exp );
         Dialog_PushSection( &( game->dialog ), msg );
      }

      px = game->player.tileIndex % game->tileMap.tilesX;
      py = game->player.tileIndex / game->tileMap.tilesX;
      tx = TILEMAP_TANTEGEL_INDEX % game->tileMap.tilesX;
      ty = TILEMAP_TANTEGEL_INDEX / game->tileMap.tilesX;

      if ( px == tx && py == ty )
      {
         strcpy( loc, STRING_ITEMUSE_GWAELINSLOVE_3_HOME );
      }
      else if ( px == tx || py == ty )
      {
         sprintf( loc, STRING_ITEMUSE_GWAELINSLOVE_3_SINGLE,
                  ( py == ty ) ? ( ( px > tx ) ? ( px - tx ) : ( tx - px ) ) : ( ( py > ty ) ? ( py - ty ) : ( ty - py ) ),
                  ( py == ty ) ? ( ( px > tx ) ? STRING_WEST : STRING_EAST ) : ( ( py > ty ) ? STRING_NORTH : STRING_SOUTH ) );
      }
      else
      {
         sprintf( loc, STRING_ITEMUSE_GWAELINSLOVE_3_DOUBLE,
                  ( py > ty ) ? ( py - ty ) : ( ty - py ), ( py > ty ) ? STRING_NORTH : STRING_SOUTH,
                  ( px > tx ) ? ( px - tx ) : ( tx - px ), ( px > tx ) ? STRING_WEST : STRING_EAST );
      }

      sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_3, loc );
      Dialog_PushSection( &( game->dialog ), msg );
      sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_4, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
   }
   else
   {
      Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_GWAELINSLOVE_CANTUSE );
   }

   Game_OpenDialog( game );
}

void Game_UseRainbowDrop( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID &&
        game->player.tileIndex == ( TILEMAP_RAINBOWBRIDGE_INDEX + 1 ) &&
        game->player.sprite.direction == Direction_Left )
   {
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMUSE_RAINBOWDROP, Game_UseRainbowDropCallback, game );
   }
   else
   {
      Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_RAINBOWDROP_CANTUSE );
   }

   Game_OpenDialog( game );
}

void Game_UseCursedBelt( Game_t* game )
{
   ITEM_TOGGLE_HASCURSEDBELT( game->player.items );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_ITEMUSE_CURSEDBELT );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_CURSED, Game_CursedCallback, game );
   Game_OpenDialog( game );
}

internal void Game_UseWingCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ChangeToOverworldState, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_WhiteOut, Game_EnterTargetPortal, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ChangeToOverworldState, game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_WhiteIn );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_UseRainbowDropCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ChangeToOverworldState, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_RainbowBridge_Trippy, Game_RainbowDropTrippyCallback, game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_RainbowBridge_WhiteOut );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_RainbowBridge_FadeIn );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_RainbowDropTrippyCallback( Game_t* game )
{
   uint32_t i;

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      game->screen.palette[i] = COLOR_WHITE;
   }

   Screen_WipeColor( &( game->screen ), COLOR_WHITE );
   ITEM_TOGGLE_HASRAINBOWDROP( game->player.items );
   game->gameFlags.usedRainbowDrop = True;
   TILE_SET_TEXTUREINDEX( game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX], 13 );
   TILE_TOGGLE_PASSABLE( game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX] );
}

internal void Game_UseSilverHarpCallback( Game_t* game )
{
   if ( game->tileMap.hasEncounters )
   {
      Battle_Generate( &( game->battle ) );
      AnimationChain_Reset( &( game->animationChain ) );
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ChangeToBattleState, game );
      AnimationChain_Start( &( game->animationChain ) );
   }
}

internal void Game_UseFairyFluteCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_UseFairyFluteMessageCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_UseFairyFluteMessageCallback( Game_t* game )
{
   if ( game->battle.specialEnemy == SpecialEnemy_Golem && !game->battle.enemy.stats.isAsleep )
   {
      Game_SpellSleepSuccessCallback( game );
   }
   else
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_BUTNOTHINGHAPPENS, Battle_SwitchTurn, &( game->battle ) );
      Game_OpenDialog( game );
   }
}

internal void Game_UseTorchCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_UseTorchResultCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_UseTorchResultCallback( Game_t* game )
{
   if ( game->tileMap.glowDiameter <= TORCH_DIAMETER )
   {
      TileMap_SetTargetGlowDiameter( &( game->tileMap ), TORCH_DIAMETER );
      game->tileMap.torchIsLit = True;
   }

   ITEM_SET_TORCHCOUNT( game->player.items, ITEM_GET_TORCHCOUNT( game->player.items ) - 1 );
   game->subState = SubState_None;
}
