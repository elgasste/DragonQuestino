#include "tile_map.h"

void TileMap_Init( TileMap_t* tileMap )
{
   uint32_t i, j;

   tileMap->tilesX = 0;
   tileMap->tilesY = 0;

   for ( i = 0; i < TILE_COUNT; i++ )
   {
      tileMap->tiles[i] = 0;
   }

   for ( i = 0; i < TILE_TEXTURE_COUNT; i++ )
   {
      for ( j = 0; j < TILE_TEXTURE_BYTES; j++ )
      {
         tileMap->textures[i].memory[j] = 0;
      }
   }

   tileMap->spriteCount = 0;
}

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
