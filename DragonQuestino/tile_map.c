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

TilePortal_t* TileMap_GetPortalForTileIndex( TileMap_t* tileMap, uint32_t index )
{
   uint32_t i;

   for ( i = 0; i < tileMap->portalCount; i++ )
   {
      if ( tileMap->portals[i].sourceTileIndex == index )
      {
         return &( tileMap->portals[i] );
      }
   }

   return 0;
}
