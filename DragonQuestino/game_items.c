#include "game.h"

void Game_UseHerb( Game_t* game )
{
   PLAYER_SET_HERBCOUNT( game->player.items, PLAYER_GET_HERBCOUNT( game->player.items ) - 1 );
   Game_DrawOverworldQuickStatus( game );
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_Herb );
}

void Game_UseWing( Game_t* game )
{
   PLAYER_SET_WINGCOUNT( game->player.items, PLAYER_GET_WINGCOUNT( game->player.items ) - 1 );
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_Wing );
}

void Game_UseFairyWater( Game_t* game )
{
   PLAYER_SET_FAIRYWATERCOUNT( game->player.items, PLAYER_GET_FAIRYWATERCOUNT( game->player.items ) - 1 );
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_FairyWater );
}

void Game_UseTorch( Game_t* game )
{
   if ( game->tileMap.isDark && game->tileMap.lightDiameter <= TORCH_DIAMETER )
   {
      TileMap_SetTargetLightDiameter( &( game->tileMap ), TORCH_DIAMETER );
      game->tileMap.lightTileCount = 0;
      game->lightingSecondsElapsed = OVERWORLD_LIGHTING_FRAME_SECONDS; // push one frame immediately
   }

   PLAYER_SET_TORCHCOUNT( game->player.items, PLAYER_GET_TORCHCOUNT( game->player.items ) - 1 );
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_Torch );
}

void Game_UseSilverHarp( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_SilverHarp );
}

void Game_UseFairyFlute( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_FairyFlute );
}

void Game_UseGwaelynsLove( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_GwaelynsLove );
}

void Game_UseRainbowDrop( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_RainbowDrop );
}

void Game_UseCursedBelt( Game_t* game )
{
   PLAYER_TOGGLE_HASCURSEDBELT( game->player.items );
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_CursedBelt );
}
