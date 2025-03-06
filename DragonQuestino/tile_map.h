#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "sprite.h"

#define TILE_SIZE                               16
#define TILE_TEXTURE_BYTES                      256   // 8 bpp
#define TILE_TEXTURE_COUNT                      32
#define TILE_COUNT_X                            140   // overworld size
#define TILE_COUNT_Y                            135
#define TILE_COUNT                              18900

// pixels per second
#define TILE_WALKSPEED_FAST                     192.0f
#define TILE_WALKSPEED_NORMAL                   64.0f
#define TILE_WALKSPEED_SLOW                     56.0f
#define TILE_WALKSPEED_VERYSLOW                 44.0f
#define TILE_WALKSPEED_CRAWL                    36.0f

#define TILEMAP_MAX_VIEWPORT_WIDTH              SCREEN_WIDTH
#define TILEMAP_MAX_VIEWPORT_HEIGHT             SCREEN_HEIGHT
#define TILEMAP_VIEWPORT_TILES_X                16
#define TILEMAP_VIEWPORT_TILES_Y                14

#define TILEMAP_MAX_PORTALS                     256
#define TILEMAP_MAX_ACTIVESPRITES               16
#define TILEMAP_MAX_STATICSPRITES               16

#define TILE_GET_TEXTUREINDEX( t )              ( ( t ) & 0x1F )
#define TILE_GET_PASSABLE( t )                  ( ( ( t ) & 0x20 ) >> 5 )
#define TILE_GET_WALKSPEED( t )                 ( ( ( t ) & 0xC0 ) >> 6 )
#define TILE_GET_ENCOUNTERABLE( t )             ( ( ( t ) & 0x100 ) >> 8 )
#define TILE_GET_ENCOUNTERRATE( t )             ( ( ( t ) & 0x300 ) >> 9 )
#define TILE_GET_DAMAGERATE( t )                ( ( ( t ) & 0x1800 ) >> 11 )

#define TILE_SET_TEXTUREINDEX( t, i )           ( t ) = ( ( t ) & 0xFFE0 ) | i
#define TILE_TOGGLE_PASSABLE( t )               ( t ) ^= 0x20

#define TORCH_DIAMETER                          3
#define GLOW_SPELL_DIAMETER                     7
#define GLOW_MAX_TILES                          200
#define GLOW_TRANSITION_FRAME_SECONDS           0.1f

#define TILEMAP_OVERWORLD_ID                    0
#define TILEMAP_TANTEGEL_ID                     3
#define TILEMAP_BRECCONARY_ID                   4
#define TILEMAP_GARINHAM_ID                     7
#define TILEMAP_KOL_ID                          12
#define TILEMAP_CANTLIN_ID                      10
#define TILEMAP_RIMULDAR_ID                     13
#define TILEMAP_KOL_ID                          12
#define TILEMAP_CHARLOCK_ID                     25
#define TILEMAP_TANTEGEL_INDEX                  7053
#define TILEMAP_TOKEN_INDEX                     16893
#define TILEMAP_FAIRYFLUTE_INDEX                192
#define TILEMAP_RAINBOWBRIDGE_INDEX             7914
#define TILEMAP_HIDDENSTAIRS_INDEX              55
#define TILEMAP_HIDDENSTAIRS_DESTINATION_ID     26
#define TILEMAP_HIDDENSTAIRS_DESTINATION_INDEX  398
#define TILEMAP_HIDDENSTAIRS_DESTINATION_DIR    Direction_Right

#define TILEMAP_TOWN_COUNT                      6

#define TILEMAP_TANTEGEL_TOWN_ID                0
#define TILEMAP_BRECCONARY_TOWN_ID              1
#define TILEMAP_GARINHAM_TOWN_ID                2
#define TILEMAP_KOL_TOWN_ID                     3
#define TILEMAP_CANTLIN_TOWN_ID                 4
#define TILEMAP_RIMULDAR_TOWN_ID                5

#define TILEMAP_TANTEGEL_ZOOM_INDEX             7193
#define TILEMAP_BRECCONARY_ZOOM_INDEX           6918
#define TILEMAP_GARINHAM_ZOOM_INDEX             1412
#define TILEMAP_KOL_ZOOM_INDEX                  2634
#define TILEMAP_CANTLIN_ZOOM_INDEX              15483
#define TILEMAP_RIMULDAR_ZOOM_INDEX             11312

typedef struct Screen_t Screen_t;
typedef struct GameFlags_t GameFlags_t;
typedef struct Player_t Player_t;

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
   GameFlags_t* gameFlags;
   Player_t* player;

   uint32_t id;

   Bool_t isDungeon;
   Bool_t isDark;
   uint32_t glowDiameter;
   uint32_t targetGlowDiameter;
   uint32_t glowTileCount;
   float glowTransitionSeconds;

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
   TilePortal_t evacPortal;

   ActiveSprite_t activeSprites[TILEMAP_MAX_ACTIVESPRITES];
   uint32_t activeSpriteCount;

   StaticSprite_t staticSprites[TILEMAP_MAX_STATICSPRITES];
   uint32_t staticSpriteCount;

   StaticSprite_t chestSprite;
   StaticSprite_t doorSprite;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap, Screen_t* screen, GameFlags_t* gameFlags, Player_t* player );
void TileMap_Tic( TileMap_t* tileMap );
void TileMap_ResetViewport( TileMap_t* tileMap );
void TileMap_ChangeViewportSize( TileMap_t* tileMap, uint16_t w, uint16_t h );
void TileMap_SetTargetGlowDiameter( TileMap_t* tileMap, uint32_t targetDiameter );
void TileMap_StartGlowTransition( TileMap_t* tileMap );
void TileMap_ReduceTargetGlowDiameter( TileMap_t* tileMap );
float TileMap_GetWalkSpeedForTileIndex( TileMap_t* tileMap, uint32_t tileIndex );
TilePortal_t* TileMap_GetPortalForTileIndex( TileMap_t* tileMap, uint32_t index );
uint32_t TileMap_GetFacingTileIndex( TileMap_t* tileMap, uint32_t sourceTileIndex, Direction_t direction );
void TileMap_Draw( TileMap_t* tileMap );

// game_data.c
void TileMap_LoadTextures( TileMap_t* tileMap );
void TileMap_Load( TileMap_t* tileMap, uint32_t id );
void TileMap_LoadHiddenStairs( TileMap_t* tileMap );
uint32_t TileMap_GetTreasureFlag( uint32_t tileMapId, uint32_t tileIndex );
uint32_t TileMap_GetDoorFlag( uint32_t tileMapId, uint32_t tileIndex );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
