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
