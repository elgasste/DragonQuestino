#include "game.h"

internal void Game_TicOverworld( Game_t* game );
internal void Game_TicOverworldWashing( Game_t* game );
internal void Game_TicPhysics( Game_t* game );
internal void Game_TicTileMapTransition( Game_t* game );
internal void Game_UpdatePlayerTileIndex( Game_t* game );

void Game_Init( Game_t* game, uint16_t* screenBuffer )
{
   Screen_Init( &( game->screen ), screenBuffer );
   TileMap_Init( &( game->tileMap ), &( game->screen ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 0 );
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

void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex )
{
   TilePortal_t* portal;

   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), tileIndex );
   game->player.tileIndex = tileIndex;
   portal = TileMap_GetPortalForTileIndex( &( game->tileMap ), tileIndex );

   if ( portal )
   {
      game->swapPortal = portal;
      Game_ChangeState( game, GameState_TileMapTransition );
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

internal void Game_TicPhysics( Game_t* game )
{
   Vector2f_t newPos;
   uint32_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile, tileIndex;
   Player_t* player = &( game->player );

   if ( player->velocity.x == 0.0f && player->velocity.y == 0.0f )
   {
      return;
   }

   newPos.x = player->sprite.position.x + ( player->velocity.x * CLOCK_FRAME_SECONDS );
   newPos.y = player->sprite.position.y + ( player->velocity.y * CLOCK_FRAME_SECONDS );

   if ( newPos.x < 0 )
   {
      newPos.x = COLLISION_THETA;
   }
   else if ( ( newPos.x + player->hitBoxSize.x ) >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      newPos.x = ( game->tileMap.tilesX * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_THETA;
   }

   if ( newPos.y < 0 )
   {
      newPos.y = COLLISION_THETA;
   }
   else if ( ( newPos.y + player->hitBoxSize.y ) >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      newPos.y = ( game->tileMap.tilesY * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_THETA;
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.noClip == False ) {
#endif

      // clip to unpassable horizontal tiles
      if ( newPos.x != player->sprite.position.x )
      {
         tileRowStartIndex = (uint32_t)( player->sprite.position.y / TILE_SIZE );
         tileRowEndIndex = (uint32_t)( ( player->sprite.position.y + player->hitBoxSize.y ) / TILE_SIZE );

         if ( newPos.x < player->sprite.position.x )
         {
            // moving left, check leftward tiles
            col = (uint32_t)( newPos.x / TILE_SIZE );

            for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.x = (float)( ( ( col + 1 ) * TILE_SIZE ) );
                  break;
               }
            }
         }
         else
         {
            // moving right, check rightward tiles
            col = (uint32_t )( ( newPos.x + player->hitBoxSize.x ) / TILE_SIZE );

            for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.x = ( col * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_THETA;
                  break;
               }
            }
         }
      }

      // clip to unpassable vertical tiles
      if ( newPos.y != player->sprite.position.y )
      {
         tileColStartIndex = ( uint32_t )( player->sprite.position.x / TILE_SIZE );
         tileColEndIndex = (uint32_t)( ( player->sprite.position.x + player->hitBoxSize.x ) / TILE_SIZE );

         if ( newPos.y < player->sprite.position.y )
         {
            // moving up, check upward tiles
            row = (uint32_t)( newPos.y / TILE_SIZE );

            for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.y = (float)( ( ( row + 1 ) * TILE_SIZE ) );
                  break;
               }
            }
         }
         else
         {
            // moving down, check downward tiles
            row = (uint32_t)( ( newPos.y + player->hitBoxSize.y ) / TILE_SIZE );

            for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.y = ( row * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_THETA;
                  break;
               }
            }
         }
      }

#if defined( VISUAL_STUDIO_DEV )
   }
#endif

   player->sprite.position = newPos;
   player->velocity.x = 0;
   player->velocity.y = 0;

   Game_UpdatePlayerTileIndex( game );
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

internal void Game_UpdatePlayerTileIndex( Game_t* game )
{
   uint32_t centerX = (uint32_t)( game->player.sprite.position.x + ( game->player.hitBoxSize.x / 2 ) );
   uint32_t centerY = (uint32_t)( game->player.sprite.position.y + ( game->player.hitBoxSize.y / 2 ) );
   uint32_t newTileIndex = ( ( centerY / TILE_SIZE ) * game->tileMap.tilesX ) + ( centerX / TILE_SIZE );

   if ( newTileIndex != game->player.tileIndex )
   {
      game->player.tileIndex = newTileIndex;
      Game_PlayerSteppedOnTile( game, newTileIndex );
   }
}
