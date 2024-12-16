#include "physics.h"
#include "game.h"

#define COLLISION_THETA  0.001f

void Physics_Tic( Game_t* game )
{
   Player_t* player = &( game->player );

   player->position.x += ( player->velocity.x * CLOCK_FRAME_SECONDS );
   player->position.y += ( player->velocity.y * CLOCK_FRAME_SECONDS );

   if ( player->position.x < 0 )
   {
      player->position.x = COLLISION_THETA;
   }
   else if ( ( player->position.x + player->hitBoxSize.x ) >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      player->position.x = ( game->tileMap.tilesX * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_THETA;
   }

   if ( player->position.y < 0 )
   {
      player->position.y = COLLISION_THETA;
   }
   else if ( ( player->position.y + player->hitBoxSize.y ) >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      player->position.y = ( game->tileMap.tilesY * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_THETA;
   }

   player->velocity.x = 0;
   player->velocity.y = 0;
}
