#include "game.h"

void Game_UseHerb( Game_t* game )
{
   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Game_OpenDialog( game, DialogId_FullyHealed );
   }
   else
   {
      ITEM_SET_HERBCOUNT( game->player.items, ITEM_GET_HERBCOUNT( game->player.items ) - 1 );
      Game_ApplyHealing( game, ITEM_HERB_MINEFFECT, ITEM_HERB_MAXEFFECT, DialogId_Use_Herb1, DialogId_Use_Herb2 );
   }
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
