#include "tile_map.h"

float TileMap_GetWalkSpeedForTileIndex( TileMap_t* tileMap, uint32_t tileIndex )
{
   uint16_t tile = tileMap->tiles[( ( tileIndex / tileMap->tilesX ) * TILE_COUNT_X ) + ( tileIndex % tileMap->tilesX )];
   uint16_t walkSpeed = GET_TILEWALKSPEED( tile );

#if defined( VISUAL_STUDIO_DEV )

   if ( g_debugFlags.fastWalk )
   {
      return TILE_WALKSPEED_FAST;
   }

#endif

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
