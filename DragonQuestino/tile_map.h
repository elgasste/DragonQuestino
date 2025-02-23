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

#define TILEMAP_MAX_VIEWPORT_WIDTH     SCREEN_WIDTH
#define TILEMAP_MAX_VIEWPORT_HEIGHT    SCREEN_HEIGHT
#define TILEMAP_VIEWPORT_TILES_X       16
#define TILEMAP_VIEWPORT_TILES_Y       14

#define TILEMAP_MAX_PORTALS            256
#define TILEMAP_MAX_ACTIVESPRITES      16
#define TILEMAP_MAX_STATICSPRITES      16

#define GET_TILETEXTUREINDEX( t )      ( ( t ) & 0x1F )
#define GET_TILEPASSABLE( t )          ( ( ( t ) & 0x20 ) >> 5 )
#define GET_TILEWALKSPEED( t )         ( ( ( t ) & 0xC0 ) >> 6 )
#define GET_TILEENCOUNTERABLE( t )     ( ( ( t ) & 0x100 ) >> 8 )
#define GET_TILEENCOUNTERRATE( t )     ( ( ( t ) & 0x300 ) >> 9 )
#define GET_TILEDAMAGERATE( t )        ( ( ( t ) & 0x1800 ) >> 11 )

#define TORCH_DIAMETER                 3
#define RADIANT_DIAMETER               7
#define LIGHT_MAX_TILES                200

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
   Screen_t* screen;
   uint32_t id;

   Bool_t isDark;
   uint32_t lightDiameter;
   uint32_t targetLightDiameter;
   uint32_t lightTileCount;

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
   Vector4i32_t viewport;
   Vector2u16_t viewportScreenPos;

   TileTexture_t textures[TILE_TEXTURE_COUNT];

   TilePortal_t portals[TILEMAP_MAX_PORTALS];
   uint32_t portalCount;

   ActiveSprite_t activeSprites[TILEMAP_MAX_ACTIVESPRITES];
   uint32_t activeSpriteCount;

   StaticSprite_t staticSprites[TILEMAP_MAX_STATICSPRITES];
   uint32_t staticSpriteCount;

   StaticSprite_t chestSprite;
   uint32_t treasureFlags;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap, Screen_t* screen );
void TileMap_ResetViewport( TileMap_t* tileMap );
void TileMap_UpdateViewport( TileMap_t* tileMap, int32_t anchorX, int32_t anchorY, uint32_t anchorW, uint32_t anchorH );
void TileMap_ChangeViewportSize( TileMap_t* tileMap, uint16_t w, uint16_t h );
void TileMap_SetTargetLightDiameter( TileMap_t* tileMap, uint32_t targetDiameter );
void TileMap_ReduceLightDiameter( TileMap_t* tileMap );
void TileMap_IncreaseLightDiameter( TileMap_t* tileMap );
float TileMap_GetWalkSpeedForTileIndex( TileMap_t* tileMap, uint32_t tileIndex );
TilePortal_t* TileMap_GetPortalForTileIndex( TileMap_t* tileMap, uint32_t index );
void TileMap_Draw( TileMap_t* tileMap );

// game_data.c
void TileMap_LoadTextures( TileMap_t* tileMap );
void TileMap_Load( TileMap_t* tileMap, uint32_t index );
uint32_t TileMap_GetTreasureFlag( uint32_t tileMapIndex, uint32_t tileIndex );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
