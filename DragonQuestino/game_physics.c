#include "game.h"
#include "random.h"

internal void Game_UpdatePlayerTileIndex( Game_t* game );
internal void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex );

void Game_TicPhysics( Game_t* game )
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

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetPermadoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->tileMap.permadoorFlags ) )
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

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetPermadoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->tileMap.permadoorFlags ) )
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

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetPermadoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->tileMap.permadoorFlags ) )
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

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetPermadoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->tileMap.permadoorFlags ) )
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

internal void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex )
{
   TilePortal_t* portal;

   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), tileIndex );
   game->player.tileIndex = tileIndex;
   portal = TileMap_GetPortalForTileIndex( &( game->tileMap ), tileIndex );

   if ( game->tileMap.isDark && game->tileMap.lightDiameter > 1 )
   {
      game->tileMap.lightTileCount++;

      if ( game->tileMap.lightTileCount > LIGHT_MAX_TILES )
      {
         game->tileMap.lightTileCount = 0;
         TileMap_ReduceLightDiameter( &( game->tileMap ) );
      }
   }

   if ( portal )
   {
      game->swapPortal = portal;
      Game_ChangeState( game, GameState_TileMapTransition );
      Random_Seed();
   }
}
