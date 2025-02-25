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

#define TILE_GET_TEXTUREINDEX( t )      ( ( t ) & 0x1F )
#define TILE_GET_PASSABLE( t )          ( ( ( t ) & 0x20 ) >> 5 )
#define TILE_GET_WALKSPEED( t )         ( ( ( t ) & 0xC0 ) >> 6 )
#define TILE_GET_ENCOUNTERABLE( t )     ( ( ( t ) & 0x100 ) >> 8 )
#define TILE_GET_ENCOUNTERRATE( t )     ( ( ( t ) & 0x300 ) >> 9 )
#define TILE_GET_DAMAGERATE( t )        ( ( ( t ) & 0x1800 ) >> 11 )

#define TILE_SET_TEXTUREINDEX( t, i )   ( t ) = ( ( t ) & 0xFFE0 ) | i
#define TILE_SET_PASSABLE( t, b )       ( t ) = ( ( t ) & 0xFFDF ) | ( b << 5 )

#define TORCH_DIAMETER                 3
#define RADIANT_DIAMETER               7
#define LIGHT_MAX_TILES                200

#define TILEMAP_OVERWORLD_ID           0
#define TILEMAP_KOL_ID                 12
#define TILEMAP_TANTEGEL_INDEX         7053
#define TILEMAP_TOKEN_INDEX            16893
#define TILEMAP_FAIRYFLUTE_INDEX       192
#define TILEMAP_RAINBOWBRIDGE_INDEX    7914

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

   Bool_t usedRainbowDrop;

   // bits 0-4: texture index (max 32 textures)
   // bit 5: is-passable flag
   // bits 6-7: walk speed (0 = normal, 3 = crawl)
   // bit 8: is-encounterable flag
   // bits 9-10: encounter rate
   // bits 11-12: damage rate
   // bits 13-15: reserved
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

   StaticSprite_t doorSprite;
   // MUFFINS: make it so the high 16 bits are "temporary", and the low 16 bits are "permanent".
   uint32_t doorFlags;
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
uint32_t TileMap_GetFacingTileIndex( TileMap_t* tileMap, uint32_t sourceTileIndex, Direction_t direction );
void TileMap_Draw( TileMap_t* tileMap );

// game_data.c
void TileMap_LoadTextures( TileMap_t* tileMap );
void TileMap_Load( TileMap_t* tileMap, uint32_t id );
uint32_t TileMap_GetTreasureFlag( uint32_t tileMapId, uint32_t tileIndex );
uint32_t TileMap_GetDoorFlag( uint32_t tileMapId, uint32_t tileIndex );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
