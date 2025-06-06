#include "game.h"
#include "random.h"

void Game_UseHerb( Game_t* game )
{
   uint8_t restoredHitPoints;
   char msg[64];

   Dialog2_Reset( &( game->dialog2 ), game->mainState );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_FULLYHEALED, 0, 0 );
   }
   else
   {
      ITEM_SET_HERBCOUNT( game->player.items, ITEM_GET_HERBCOUNT( game->player.items ) - 1 );
      restoredHitPoints = Random_u8( ITEM_HERB_MINEFFECT, ITEM_HERB_MAXEFFECT );
      game->pendingPayload8u = restoredHitPoints;
      Dialog2_PushSection( &( game->dialog2 ), STRING_ITEMUSE_HERB, 0, 0 );
      sprintf( msg, STRING_DIALOG_HEAL_RESULT, restoredHitPoints, ( restoredHitPoints == 1 ) ? STRING_POINT : STRING_POINTS );
      Dialog2_PushSection( &( game->dialog2 ), msg, Game_RestoredHitPointsCallback, game );
   }

   Dialog2_Start( &( game->dialog2 ) );
   Game_ChangeSubState( game, SubState_Dialog );
}

void Game_UseWing( Game_t* game )
{
   if ( game->tileMap.isDungeon )
   {
      Game_OpenDialog( game, DialogId_Use_WingCantUse );
   }
   else
   {
      ITEM_SET_WINGCOUNT( game->player.items, ITEM_GET_WINGCOUNT( game->player.items ) - 1 );
      game->targetPortal = &( game->zoomPortals[TILEMAP_TANTEGEL_TOWN_ID] );
      Game_OpenDialog( game, DialogId_Use_Wing );
   }
}

void Game_UseFairyWater( Game_t* game )
{
   ITEM_SET_FAIRYWATERCOUNT( game->player.items, ITEM_GET_FAIRYWATERCOUNT( game->player.items ) - 1 );

   if ( game->player.isCursed )
   {
      Game_OpenDialog( game, DialogId_Use_FairyWaterCursed );
   }
   else
   {
      Player_SetHolyProtection( &( game->player ), True );
      Game_OpenDialog( game, DialogId_Use_FairyWater );
   }
}

void Game_UseTorch( Game_t* game )
{
   if ( game->tileMap.isDark )
   {
      if ( game->tileMap.torchIsLit )
      {
         Game_OpenDialog( game, DialogId_Use_TorchAlreadyUsed );
      }
      else
      {
         if ( game->tileMap.glowDiameter <= TORCH_DIAMETER )
         {
            TileMap_SetTargetGlowDiameter( &( game->tileMap ), TORCH_DIAMETER );
            game->tileMap.torchIsLit = True;
            TileMap_StartGlowTransition( &( game->tileMap ) );
         }

         ITEM_SET_TORCHCOUNT( game->player.items, ITEM_GET_TORCHCOUNT( game->player.items ) - 1 );
         Game_OpenDialog( game, DialogId_Use_Torch );
      }
   }
   else
   {
      Game_OpenDialog( game, DialogId_Use_TorchCantUse );
   }
}

void Game_UseSilverHarp( Game_t* game )
{
   Game_OpenDialog( game, DialogId_Use_SilverHarp );
}

void Game_UseFairyFlute( Game_t* game )
{
   Game_OpenDialog( game, DialogId_Use_FairyFlute );
}

void Game_UseGwaelynsLove( Game_t* game )
{
   uint32_t px, py, tx, ty;
   char msg[64];

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID )
   {
      px = game->player.tileIndex % game->tileMap.tilesX;
      py = game->player.tileIndex / game->tileMap.tilesX;
      tx = TILEMAP_TANTEGEL_INDEX % game->tileMap.tilesX;
      ty = TILEMAP_TANTEGEL_INDEX / game->tileMap.tilesX;

      if ( px == tx && py == ty )
      {
         sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_3_HOME );
      }
      else if ( px == tx || py == ty )
      {
         sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_3_SINGLE,
                  ( py == ty ) ? ( ( px > tx ) ? ( px - tx ) : ( tx - px ) ) : ( ( py > ty ) ? ( py - ty ) : ( ty - py ) ),
                  ( py == ty ) ? ( ( px > tx ) ? STRING_WEST : STRING_EAST ) : ( ( py > ty ) ? STRING_NORTH : STRING_SOUTH ) );
      }
      else
      {
         sprintf( msg, STRING_ITEMUSE_GWAELINSLOVE_3_DOUBLE,
                  ( py > ty ) ? ( py - ty ) : ( ty - py ), ( py > ty ) ? STRING_NORTH : STRING_SOUTH,
                  ( px > tx ) ? ( px - tx ) : ( tx - px ), ( px > tx ) ? STRING_WEST : STRING_EAST );
      }

      Dialog_SetInsertionText( &( game->dialog ), msg );
      Game_OpenDialog( game, DialogId_Use_GwaelynsLove );
   }
   else
   {
      Game_OpenDialog( game, DialogId_Use_GwaelynsLoveCantUse );
   }
}

void Game_UseRainbowDrop( Game_t* game )
{
   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID &&
        game->player.tileIndex == ( TILEMAP_RAINBOWBRIDGE_INDEX + 1 ) &&
        game->player.sprite.direction == Direction_Left )
   {
      Game_OpenDialog( game, DialogId_Use_RainbowDrop );
   }
   else
   {
      Game_OpenDialog( game, DialogId_Use_RainbowDropCantUse );
   }
}

void Game_UseCursedBelt( Game_t* game )
{
   ITEM_TOGGLE_HASCURSEDBELT( game->player.items );
   Game_OpenDialog( game, DialogId_Use_CursedBelt );
}
