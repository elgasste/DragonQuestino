#include "physics.h"
#include "game.h"

#define COLLISION_THETA  0.001f

void Physics_Tic( Game_t* game )
{
   Vector2f_t newPos;
   uint32_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile, tileIndex;
   Player_t* player = &( game->player );

   if ( player->velocity.x == 0.0f && player->velocity.y == 0.0f )
   {
      return;
   }

   newPos.x = player->position.x + ( player->velocity.x * CLOCK_FRAME_SECONDS );
   newPos.y = player->position.y + ( player->velocity.y * CLOCK_FRAME_SECONDS );

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
   if ( newPos.x != player->position.x )
   {
      tileRowStartIndex = (uint32_t)( player->position.y / TILE_SIZE );
      tileRowEndIndex = (uint32_t)( ( player->position.y + player->hitBoxSize.y ) / TILE_SIZE );

      if ( newPos.x < player->position.x )
      {
         // moving left, check leftward tiles
         col = (uint32_t)( newPos.x / TILE_SIZE );

         for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
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
            tileIndex = col + ( row * game->tileMap.tilesX );
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
   if ( newPos.y != player->position.y )
   {
      tileColStartIndex = ( uint32_t )( player->position.x / TILE_SIZE );
      tileColEndIndex = (uint32_t)( ( player->position.x + player->hitBoxSize.x ) / TILE_SIZE );

      if ( newPos.y < player->position.y )
      {
         // moving up, check upward tiles
         row = (uint32_t)( newPos.y / TILE_SIZE );

         for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
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
            tileIndex = col + ( row * game->tileMap.tilesX );
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

   player->position = newPos;
   player->velocity.x = 0;
   player->velocity.y = 0;
}
