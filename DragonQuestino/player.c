#include "player.h"
#include "tile_map.h"

void Player_Init( Player_t* player )
{
   player->tileIndex = TILE_COUNT_X * TILE_COUNT_Y; // off the map
   player->sprite.position.x = (float)( TILE_SIZE * 55 );
   player->sprite.position.y = (float)( TILE_SIZE * 49 );
   player->velocity.x = 0.0f;
   player->velocity.y = 0.0f;
   player->maxVelocity = TILE_WALKSPEED_NORMAL;
   player->hitBoxSize.x = TILE_SIZE - 4;
   player->hitBoxSize.y = TILE_SIZE - 4;
   player->spriteOffset.x = -2;
   player->spriteOffset.y = -4;
   player->sprite.direction = Direction_Down;
}
