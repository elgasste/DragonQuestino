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

#define TILEMAP_VIEWPORT_WIDTH         256
#define TILEMAP_VIEWPORT_HEIGHT        240
#define TILEMAP_VIEWPORT_PIXELS        61440

#define TILEMAP_MAX_PORTALS            256
#define TILEMAP_MAX_ACTIVESPRITES      16
#define TILEMAP_MAX_STATICSPRITES      16

#define GET_TILETEXTUREINDEX( t )      ( ( t ) & 0x1F )
#define GET_TILEPASSABLE( t )          ( ( ( t ) & 0x20 ) >> 5 )
#define GET_TILEWALKSPEED( t )         ( ( ( t ) & 0xC0 ) >> 6 )
#define GET_TILEENCOUNTERABLE( t )     ( ( ( t ) & 0x100 ) >> 8 )
#define GET_TILEENCOUNTERRATE( t )     ( ( ( t ) & 0x300 ) >> 9 )
#define GET_TILEDAMAGERATE( t )        ( ( ( t ) & 0x1800 ) >> 11 )

typedef struct Screen_t Screen_t;

typedef struct TileTexture_t
{
   uint8_t memory[TILE_TEXTURE_BYTES];
}
TileTexture_t;

typedef struct TilePortal_t
{
   uint32_t sourceTileIndex;
   uint32_t destinationTileMapIndex;
   uint32_t destinationTileIndex;
   Direction_t arrivalDirection;
}
TilePortal_t;

typedef struct TileMap_t
{
   // bits 1-5: texture index (max 32 textures)
   // bit 6: is-passable flag
   // bits 7-8: walk speed (0 = normal, 3 = crawl)
   // bit 9: is-encounterable flag
   // bits 10-11: encounter rate
   // bits 12-13: damage rate
   // bits 14-16: reserved
   uint16_t tiles[TILE_COUNT];
   uint32_t tilesX;
   uint32_t tilesY;

   TileTexture_t textures[TILE_TEXTURE_COUNT];

   TilePortal_t portals[TILEMAP_MAX_PORTALS];
   uint32_t portalCount;

   ActiveSprite_t activeSprites[TILEMAP_MAX_ACTIVESPRITES];
   uint32_t activeSpriteCount;

   StaticSprite_t staticSprites[TILEMAP_MAX_STATICSPRITES];
   uint32_t staticSpriteCount;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

float TileMap_GetWalkSpeedForTile( uint16_t tile );
TilePortal_t* TileMap_GetPortalForTileIndex( TileMap_t* tileMap, uint32_t index );

// game_data.c
void TileMap_LoadTextures( TileMap_t* tileMap );
void TileMap_Load( TileMap_t* tileMap, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
