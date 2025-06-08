#include "game.h"
#include "random.h"
#include "math.h"

internal void Game_UseWingCallback( Game_t* game );
internal void Game_UseRainbowDropCallback( Game_t* game );
internal void Game_RainbowDropTrippyCallback( Game_t* game );

void Game_UseHerb( Game_t* game )
{
   uint8_t restoredHitPoints;
   char msg[64];

   game->screen.needsRedraw = True;
   Dialog2_Reset( &( game->dialog2 ) );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_FULLYHEALED );
   }
   else
   {
      ITEM_SET_HERBCOUNT( game->player.items, ITEM_GET_HERBCOUNT( game->player.items ) - 1 );
      restoredHitPoints = MATH_MIN( Random_u8( ITEM_HERB_MINEFFECT, ITEM_HERB_MAXEFFECT ), game->player.stats.maxHitPoints - game->player.stats.hitPoints );
      game->pendingPayload8u = restoredHitPoints;
      Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_HERB );
      sprintf( msg, STRING_DIALOG_HEAL_RESULT, restoredHitPoints, ( restoredHitPoints == 1 ) ? STRING_POINT : STRING_POINTS );
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_RestoredHitPointsCallback, game );
   }

   Game_OpenDialog2( game );
}

void Game_UseWing( Game_t* game )
{
   Dialog2_Reset( &( game->dialog2 ) );

   if ( game->tileMap.isDungeon )
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_WING_CANTUSE );
   }
   else
   {
      game->screen.needsRedraw = True;
      ITEM_SET_WINGCOUNT( game->player.items, ITEM_GET_WINGCOUNT( game->player.items ) - 1 );
      game->targetPortal = &( game->zoomPortals[TILEMAP_TANTEGEL_TOWN_ID] );
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), STRING_ITEMUSE_WING, Game_UseWingCallback, game );
   }

   Game_OpenDialog2( game );
}

void Game_UseFairyWater( Game_t* game )
{
   ITEM_SET_FAIRYWATERCOUNT( game->player.items, ITEM_GET_FAIRYWATERCOUNT( game->player.items ) - 1 );
   Dialog2_Reset( &( game->dialog2 ) );
   Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_FAIRYWATER );

   if ( game->player.isCursed )
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_HOLYPROTECTION_CURSED );
   }
   else
   {
      Player_SetHolyProtection( &( game->player ), True );
      Dialog2_PushSection( &( game->dialog2 ), STRING_HOLYPROTECTION_ON );
   }

   Game_OpenDialog2( game );
}

void Game_UseTorch( Game_t* game )
{
   game->screen.needsRedraw = True;
   Dialog2_Reset( &( game->dialog2 ) );

   if ( game->tileMap.isDark )
   {
      if ( game->tileMap.torchIsLit )
      {
         Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_TORCH_ALREADYUSED );
      }
      else
      {
         if ( game->tileMap.glowDiameter <= TORCH_DIAMETER )
         {
            TileMap_SetTargetGlowDiameter( &( game->tileMap ), TORCH_DIAMETER );
            game->tileMap.torchIsLit = True;
         }

         ITEM_SET_TORCHCOUNT( game->player.items, ITEM_GET_TORCHCOUNT( game->player.items ) - 1 );
         Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_TORCH );
      }
   }
   else
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_TORCH_CANTUSE );
   }

   Game_OpenDialog2( game );
}

void Game_UseSilverHarp( Game_t* game )
{
   game->screen.needsRedraw = True;
   Dialog2_Reset( &( game->dialog2 ) );
   Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_SILVERHARP_1 );
   Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_SILVERHARP_2 );
   Game_OpenDialog2( game );
}

void Game_UseFairyFlute( Game_t* game )
{
   game->screen.needsRedraw = True;
   Dialog2_Reset( &( game->dialog2 ) );
   Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_FAIRYFLUTE_1 );
   Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_FAIRYFLUTE_2 );
   Game_OpenDialog2( game );
}

void Game_UseGwaelynsLove( Game_t* game )
{
   uint32_t px, py, tx, ty, exp;
   char msg[128], loc[64];

   game->screen.needsRedraw = True;
   Dialog2_Reset( &( game->dialog2 ) );

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID )
   {
      sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_1, game->player.name );
      Dialog2_PushSection( &( game->dialog2 ), msg );

      exp = Player_GetExperienceRemaining( &( game->player ) );

      if ( exp > 0 )
      {
         sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_2, exp );
         Dialog2_PushSection( &( game->dialog2 ), msg );
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
      Dialog2_PushSection( &( game->dialog2 ), msg );
      sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_4, game->player.name );
      Dialog2_PushSection( &( game->dialog2 ), msg );
   }
   else
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_GWAELINSLOVE_CANTUSE );
   }

   Game_OpenDialog2( game );
}

void Game_UseRainbowDrop( Game_t* game )
{
   Dialog2_Reset( &( game->dialog2 ) );

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID &&
        game->player.tileIndex == ( TILEMAP_RAINBOWBRIDGE_INDEX + 1 ) &&
        game->player.sprite.direction == Direction_Left )
   {
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), STRING_ITEMUSE_RAINBOWDROP, Game_UseRainbowDropCallback, game );
   }
   else
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_RAINBOWDROP_CANTUSE );
   }

   Game_OpenDialog2( game );
}

void Game_UseCursedBelt( Game_t* game )
{
   game->screen.needsRedraw = True;
   ITEM_TOGGLE_HASCURSEDBELT( game->player.items );
   Dialog2_Reset( &( game->dialog2 ) );
   Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_CURSEDBELT );
   Dialog2_PushSectionWithCallback( &( game->dialog2 ), STRING_CURSED, Game_CursedCallback, game );
   Game_OpenDialog2( game );
}

internal void Game_UseWingCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
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
