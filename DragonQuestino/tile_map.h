#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "sprite.h"

#define TILE_SIZE                      16
#define TILE_TEXTURE_BYTES             256   // 8 bpp
#define TILE_TEXTURE_COUNT             32
#define TILE_COUNT_X                   140   // overworld size
#define TILE_COUNT_Y                   135
#define TILE_COUNT                     18900

// pixels per second
#define TILE_WALKSPEED_FAST            192.0f
#define TILE_WALKSPEED_NORMAL          72.0f
#define TILE_WALKSPEED_SLOW            60.0f
#define TILE_WALKSPEED_VERYSLOW        48.0f
#define TILE_WALKSPEED_CRAWL           36.0f

#define TILEMAP_MAX_SPRITES            12

#define GET_TILETEXTUREINDEX( t )      ( ( t ) & 0x1F )
#define GET_TILEPASSABLE( t )          ( ( ( t ) & 0x20 ) >> 5 )
#define GET_TILEWALKSPEED( t )         ( ( ( t ) & 0xC0 ) >> 6 )

typedef struct Screen_t Screen_t;

typedef struct TileTexture_t
{
   uint8_t memory[TILE_TEXTURE_BYTES];
}
TileTexture_t;

typedef struct TileMap_t
{
   // lowest 5 bits: texture index (max 32 textures)
   // bit 6: "is passable" flag
   // bits 7 and 8: "slow-down" factor (0 = walk normal, 3 = walk very slow)
   // highest 3 bits are for other things (portals? encounter rates? damage?)
   uint16_t tiles[TILE_COUNT];
   uint32_t tilesX;
   uint32_t tilesY;

   TileTexture_t textures[TILE_TEXTURE_COUNT];

   Sprite_t sprites[TILEMAP_MAX_SPRITES];
   uint32_t spriteCount;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap );
float TileMap_GetWalkSpeedForTile( uint16_t tile );

// game_data.c
void TileMap_LoadTextures( TileMap_t* tileMap );
void TileMap_Load( TileMap_t* tileMap, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
