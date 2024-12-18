#include "player.h"
#include "tile_map.h"

void Player_Init( Player_t* player )
{
   player->tileIndex = TILE_COUNT_X * TILE_COUNT_Y; // off the map
}
