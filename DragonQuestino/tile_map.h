#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"

#define TILE_SIZE                      16
#define TILE_BYTES_X                   8
#define TILE_TEXTURE_BYTES             128   // 4 bpp, so (16 / 2) * 16
#define TILE_TEXTURE_COUNT             64
#define TILE_COUNT                     18900    // 140x135 (overworld size)

#define GET_TILETEXTUREINDEX( t )      ( ( t ) & 0x3F )

typedef struct Screen_t Screen_t;

typedef struct TileTexture_t
{
   uint8_t memory[TILE_TEXTURE_BYTES];
}
TileTexture_t;

typedef struct TileMap_t
{
   // lowest 6 bits are the texture index (max 64 textures)
   // highest 10 bits are for other things (portals? encounter rates? damage?)
   uint16_t tiles[TILE_COUNT];
   uint8_t tilesX;
   uint8_t tilesY;

   TileTexture_t textures[TILE_TEXTURE_COUNT];
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap );

// data.c
void TileMap_Load( TileMap_t* tileMap, Screen_t* screen, uint8_t index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
