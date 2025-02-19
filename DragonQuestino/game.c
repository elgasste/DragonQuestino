#include "game.h"

internal void Game_TicOverworld( Game_t* game );
internal void Game_TicOverworldWashing( Game_t* game );
internal void Game_TicTileMapTransition( Game_t* game );
internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag );

void Game_Init( Game_t* game, uint16_t* screenBuffer )
{
   Screen_Init( &( game->screen ), screenBuffer );
   TileMap_Init( &( game->tileMap ), &( game->screen ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 1 );
   Sprite_LoadPlayer( &( game->player.sprite ) );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ) );
   Menu_Init( &( game->menu ), &( game->screen ), &( game->player ) );
   ScrollingDialog_Init( &( game->scrollingDialog ), &( game->screen ), &( game->player ) );

   game->state = GameState_Overworld;
   game->swapPortal = 0;
   game->overworldInactivitySeconds = 0.0f;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_HandleInput( game );

   switch ( game->state )
   {
      case GameState_Overworld:
         Game_TicOverworld( game );
         break;
      case GameState_Overworld_Washing:
         Game_TicOverworldWashing( game );
         break;
      case GameState_Overworld_ScrollingDialog:
         ScrollingDialog_Tic( &( game->scrollingDialog ) );
         break;
      case GameState_Overworld_MainMenu:
      case GameState_Overworld_ItemMenu:
         Menu_Tic( &( game->menu ) );
         break;
      case GameState_TileMapTransition:
         Game_TicTileMapTransition( game );
         break;
   }

   Game_Draw( game );
   Screen_RenderBuffer( &( game->screen ) );
}

void Game_ChangeState( Game_t* game, GameState_t newState )
{
   game->state = newState;

   switch ( newState )
   {
      case GameState_Overworld:
         game->overworldInactivitySeconds = 0.0f;
         break;
      case GameState_Overworld_Washing:
         game->overworldWashSeconds = 0.0f;
         break;
   }
}

internal void Game_TicOverworld( Game_t* game )
{
   Game_TicPhysics( game );
   Sprite_Tic( &( game->player.sprite ) );
   TileMap_UpdateViewport( &( game->tileMap ),
                           (int32_t)( game->player.sprite.position.x ), (int32_t)( game->player.sprite.position.y ),
                           game->player.hitBoxSize.x, game->player.hitBoxSize.y );
}

internal void Game_TicOverworldWashing( Game_t* game )
{
   game->overworldWashSeconds += CLOCK_FRAME_SECONDS;

   if ( game->overworldWashSeconds > OVERWORLD_WASH_TOTAL_SECONDS )
   {
      Game_ChangeState( game, GameState_Overworld );
   }
}

internal void Game_TicTileMapTransition( Game_t* game )
{
   uint32_t destinationTileIndex;
   Direction_t arrivalDirection;

   if ( game->swapPortal )
   {
      destinationTileIndex = game->swapPortal->destinationTileIndex;
      arrivalDirection = game->swapPortal->arrivalDirection;

      TileMap_Load( &( game->tileMap ), game->swapPortal->destinationTileMapIndex );

      game->player.sprite.position.x = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex % game->tileMap.tilesX ) ) - game->player.spriteOffset.x ) + COLLISION_THETA;
      // the player sprite gets caught on unpassable tiles unless we use COLLISION_THETA here, but for some reason the x-axis has no problems
      game->player.sprite.position.y = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex / game->tileMap.tilesX ) ) - game->player.spriteOffset.y ) - COLLISION_THETA;
      game->player.tileIndex = destinationTileIndex;
      game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), destinationTileIndex );
      game->tileMapSwapSecondsElapsed = 0.0f;
      game->swapPortal = 0;

      Sprite_SetDirection( &( game->player.sprite ), arrivalDirection );
      TileMap_UpdateViewport( &( game->tileMap ),
                              (int32_t)( game->player.sprite.position.x ), (int32_t)( game->player.sprite.position.y ),
                              game->player.hitBoxSize.x, game->player.hitBoxSize.y );
   }
   else
   {
      game->tileMapSwapSecondsElapsed += CLOCK_FRAME_SECONDS;

      if ( game->tileMapSwapSecondsElapsed > TILEMAP_SWAP_SECONDS )
      {
         Game_ChangeState( game, GameState_Overworld );
      }
   }
}

void Game_Search( Game_t* game )
{
   uint32_t treasureFlag = TileMap_GetTreasureFlag( game->tileMap.id, game->player.tileIndex );

   if ( treasureFlag && ( game->tileMap.treasureFlags & treasureFlag ) )
   {
      Game_CollectTreasure( game, treasureFlag );
   }
   else
   {
      Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
      ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Search_NothingFound );
   }
}

internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag )
{
   uint16_t gold = 0;
   Bool_t isGold = True, collected = False;
   char msg[4];

   switch ( treasureFlag )
   {
      case 0x1:      // Tantegel throne room, upper-right chest
         isGold = False;
         collected = Player_CollectItem( &( game->player ), Item_Key );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_ITEMCOLLECT_KEY );
         break;
      case 0x2:      // Tantegel throne room, lower-left chest
         isGold = True;
         gold = 120;
         break;
      case 0x4:      // Tantegel throne room, lower-right chest
         isGold = False;
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_ITEMCOLLECT_HERB );
         break;
      case 0x8:      // Tantegel ground floor, left room, upper-left chest
         gold = 13;
         break;
      case 0x10:     // Tantegel ground floor, left room, middle chest
         gold = 11;
         break;
      case 0x20:     // Tantegel ground floor, left room, bottom-left chest
         gold = 7;
         break;
      case 0x40:     // Tantegel ground floor, left room, bottom-right chest
         gold = 13;
         break;
   }

   if ( gold > 0 )
   {
      collected = ( Player_CollectGold( &( game->player ), gold ) > 0 ) ? True : False;
      sprintf( msg, "%u", gold );
      ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), msg );
   }

   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );

   if ( collected )
   {
      game->tileMap.treasureFlags ^= treasureFlag;
      ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, isGold ? DialogMessageId_Chest_GoldCollected : DialogMessageId_Chest_ItemCollected );
   }
   else
   {
      ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, isGold ? DialogMessageId_Chest_GoldNoSpace : DialogMessageId_Chest_ItemNoSpace );
   }
}
