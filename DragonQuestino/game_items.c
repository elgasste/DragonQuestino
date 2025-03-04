#include "game.h"

void Game_UseHerb( Game_t* game )
{
   if ( game->player.stats.hitPoints == UINT8_MAX )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_FullyHealed );
   }
   else
   {
      ITEM_SET_HERBCOUNT( game->player.items, ITEM_GET_HERBCOUNT( game->player.items ) - 1 );
      Game_ApplyHealing( game, ITEM_HERB_MINEFFECT, ITEM_HERB_MAXEFFECT, DialogMessageId_Use_Herb1, DialogMessageId_Use_Herb2 );
   }
}

void Game_UseWing( Game_t* game )
{
   ITEM_SET_WINGCOUNT( game->player.items, ITEM_GET_WINGCOUNT( game->player.items ) - 1 );
   Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_Wing );
}

void Game_UseFairyWater( Game_t* game )
{
   ITEM_SET_FAIRYWATERCOUNT( game->player.items, ITEM_GET_FAIRYWATERCOUNT( game->player.items ) - 1 );
   Player_SetHolyProtection( &( game->player ), True );
   Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_FairyWater );
}

void Game_UseTorch( Game_t* game )
{
   if ( game->tileMap.isDark )
   {
      if ( game->tileMap.glowDiameter <= TORCH_DIAMETER )
      {
         TileMap_SetTargetGlowDiameter( &( game->tileMap ), TORCH_DIAMETER );
         game->tileMap.glowTileCount = 0;
         game->glowExpandSeconds = GLOW_EXPAND_FRAME_SECONDS; // push one frame immediately
      }

      ITEM_SET_TORCHCOUNT( game->player.items, ITEM_GET_TORCHCOUNT( game->player.items ) - 1 );
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_Torch );
   }
   else
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_TorchCantUse );
   }
}

void Game_UseSilverHarp( Game_t* game )
{
   Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_SilverHarp );
}

void Game_UseFairyFlute( Game_t* game )
{
   Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_FairyFlute );
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

      ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), msg );
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_GwaelynsLove );
   }
   else
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_GwaelynsLoveCantUse );
   }
}

void Game_UseRainbowDrop( Game_t* game )
{
   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID &&
        game->player.tileIndex == ( TILEMAP_RAINBOWBRIDGE_INDEX + 1 ) &&
        game->player.sprite.direction == Direction_Left )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_RainbowDrop );
   }
   else
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_RainbowDropCantUse );
   }
}

void Game_UseCursedBelt( Game_t* game )
{
   ITEM_TOGGLE_HASCURSEDBELT( game->player.items );
   Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Use_CursedBelt );
}
