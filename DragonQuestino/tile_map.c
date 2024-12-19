#include "tile_map.h"

float TileMap_GetWalkSpeedForTile( uint16_t tile )
{
   uint16_t walkSpeed = GET_TILEWALKSPEED( tile );

   switch ( walkSpeed )
   {
      case 0: return TILE_WALKSPEED_NORMAL;
      case 1: return TILE_WALKSPEED_SLOW;
      case 2: return TILE_WALKSPEED_VERYSLOW;
      case 3: return TILE_WALKSPEED_CRAWL;
   }

   return TILE_WALKSPEED_NORMAL;
}
