#include "tile_map.h"

void TileMap_Init( TileMap_t* tileMap )
{
   tileMap->viewport.x = 0;
   tileMap->viewport.y = 0;
   tileMap->viewport.w = TILEMAP_VIEWPORT_WIDTH;
   tileMap->viewport.h = TILEMAP_VIEWPORT_HEIGHT;
}

void TileMap_UpdateViewport( TileMap_t* tileMap, int32_t anchorX, int32_t anchorY, uint32_t anchorW, uint32_t anchorH )
{
   Vector4i32_t* viewport = &( tileMap->viewport );

   viewport->x = anchorX - ( viewport->w / 2 ) + ( anchorW / 2 );
   viewport->y = anchorY - ( viewport->h / 2 ) + ( anchorH / 2 );

   if ( viewport->x < 0 )
   {
      viewport->x = 0;
   }
   else if ( ( viewport->x + viewport->w ) > (int32_t)( tileMap->tilesX * TILE_SIZE ) )
   {
      viewport->x = ( tileMap->tilesX * TILE_SIZE ) - viewport->w;
   }

   if ( viewport->y < 0 )
   {
      viewport->y = 0;
   }
   else if ( ( viewport->y + viewport->h ) > (int32_t)( tileMap->tilesY * TILE_SIZE ) )
   {
      viewport->y = ( tileMap->tilesY * TILE_SIZE ) - viewport->h;
   }
}

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

void TileMap_Draw( TileMap_t* tileMap, Screen_t* screen )
{
   uint32_t firstTileX, firstTileY, lastTileX, lastTileY, tileX, tileY, textureIndex, tileOffsetX, tileOffsetY, tileWidth, tileHeight, screenX, screenY;
   Vector4i32_t* viewport = &( tileMap->viewport );

   firstTileX = viewport->x / TILE_SIZE;
   firstTileY = viewport->y / TILE_SIZE;
   lastTileX = ( viewport->x + TILEMAP_VIEWPORT_WIDTH ) / TILE_SIZE;
   lastTileY = ( viewport->y + TILEMAP_VIEWPORT_HEIGHT ) / TILE_SIZE;
   tileOffsetX = viewport->x % TILE_SIZE;
   tileOffsetY = viewport->y % TILE_SIZE;

   for ( tileY = firstTileY, screenY = 0; tileY <= lastTileY; tileY++ )
   {
      tileHeight = ( tileY == firstTileY ) ? TILE_SIZE - tileOffsetY : ( tileY == lastTileY ) ? ( viewport->y + TILEMAP_VIEWPORT_HEIGHT ) % TILE_SIZE : TILE_SIZE;

      for ( tileX = firstTileX, screenX = 0; tileX <= lastTileX; tileX++ )
      {
         textureIndex = GET_TILETEXTUREINDEX( tileMap->tiles[( tileY * TILE_COUNT_X ) + tileX] );
         tileWidth = ( tileX == firstTileX ) ? TILE_SIZE - tileOffsetX : ( tileX == lastTileX ) ? ( viewport->x + TILEMAP_VIEWPORT_WIDTH ) % TILE_SIZE : TILE_SIZE;

         Screen_DrawMemorySection( screen, tileMap->textures[textureIndex].memory, TILE_SIZE,
                                   tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                   tileWidth, tileHeight,
                                   screenX, screenY, False );

         screenX += tileWidth;
      }

      screenY += tileHeight;
   }
}
