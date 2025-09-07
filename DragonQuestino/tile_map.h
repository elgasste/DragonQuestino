#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "sprite.h"
#include "screen.h"

#define TILE_SIZE                               16
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
#define TILE_WALKSPEED_NPC                      20.0f

#define TILE_ENEMY_POOL_ENEMY_INDEX_COUNT       5

#define TILEMAP_MAX_VIEWPORT_WIDTH              SCREEN_WIDTH
#define TILEMAP_MAX_VIEWPORT_HEIGHT             SCREEN_HEIGHT
#define TILEMAP_VIEWPORT_TILES_X                16
#define TILEMAP_VIEWPORT_TILES_Y                14

#define TILEMAP_MAX_PORTALS                     256
#define TILEMAP_MAX_STATICSPRITES               16
#define TILEMAP_MAX_NPCS                        24

#define TILE_GET_TEXTUREINDEX( t )              ( ( t ) & 0x1F )
#define TILE_GET_PASSABLE( t )                  ( ( ( t ) & 0x20 ) >> 5 )
#define TILE_GET_WALKSPEED( t )                 ( ( ( t ) & 0xC0 ) >> 6 )
#define TILE_GET_ENCOUNTERRATE( t )             ( ( ( t ) & 0x300 ) >> 8 )
#define TILE_GET_DAMAGERATE( t )                ( ( ( t ) & 0xC00 ) >> 10 )
#define TILE_GET_ENEMYPOOLINDEX( t )            ( ( ( t ) & 0xF000 ) >> 12 )

#define TILE_SET_TEXTUREINDEX( t, i )           ( t ) = ( ( t ) & 0xFFE0 ) | i
#define TILE_TOGGLE_PASSABLE( t )               ( t ) ^= 0x20

#define TORCH_DIAMETER                          3
#define GLOW_SPELL_DIAMETER                     7
#define GLOW_THREERADIUS_TILES                  80
#define GLOW_TWORADIUS_TILES                    60
#define GLOW_ONERADIUS_TILES                    60
#define GLOW_TRANSITION_FRAME_SECONDS           0.1f

#define TILEMAP_TITLESCREEN_ID                  38
#define TILEMAP_OVERWORLD_ID                    0
#define TILEMAP_TANTEGEL_THRONEROOM_ID          1
#define TILEMAP_TANTEGEL_BASEMENT_ID            2
#define TILEMAP_TANTEGEL_ID                     3
#define TILEMAP_TANTEGEL_VICTORY_ID             39
#define TILEMAP_BRECCONARY_ID                   4
#define TILEMAP_GARINHAM_ID                     7
#define TILEMAP_HAUKSNESS_ID                    9
#define TILEMAP_KOL_ID                          12
#define TILEMAP_CANTLIN_ID                      10
#define TILEMAP_RIMULDAR_ID                     13
#define TILEMAP_KOL_ID                          12
#define TILEMAP_SWAMPCAVE_ID                    20
#define TILEMAP_CHARLOCK_ID                     25
#define TILEMAP_CHARLOCK_DRAGONLORD_ID          33
#define TILEMAP_NORTHERNSHRINE_ID               34
#define TILEMAP_THEENDSCREEN_ID                 40
#define TILEMAP_TANTEGEL_INDEX                  7053
#define TILEMAP_TOKEN_INDEX                     16893
#define TILEMAP_FAIRYFLUTE_INDEX                192
#define TILEMAP_ERDRICKSARMOR_INDEX             305
#define TILEMAP_RAINBOWBRIDGE_INDEX             7914
#define TILEMAP_DRAGONLORDTHRONE_INDEX          99
#define TILEMAP_HIDDENSTAIRS_INDEX              55
#define TILEMAP_HIDDENSTAIRS_DESTINATION_ID     26
#define TILEMAP_HIDDENSTAIRS_DESTINATION_INDEX  398
#define TILEMAP_HIDDENSTAIRS_DESTINATION_DIR    Direction_Right

#define TILEMAP_ZONEZERO_STARTROW               37
#define TILEMAP_ZONEZERO_ENDROW                 51
#define TILEMAP_ZONEZERO_STARTCOL               40
#define TILEMAP_ZONEZERO_ENDCOL                 69

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

#define TILEMAP_GREENDRAGON_MAPID               20
#define TILEMAP_GREENDRAGON_TILEINDEX           687
#define TILEMAP_AXEKNIGHT_MAPID                 9
#define TILEMAP_AXEKNIGHT_TILEINDEX             305
#define TILEMAP_GOLEM_MAPID                     0
#define TILEMAP_GOLEM_TILEINDEX                 15063

#define TILEMAP_SWAMP_DAMAGE                    2
#define TILEMAP_BARRIER_DAMAGE                  15

#define TILEMAP_NPC_MINPAUSESECONDS             2
#define TILEMAP_NPC_MAXPAUSESECONDS             4
#define TILEMAP_NPC_MINWANDERSECONDS            1
#define TILEMAP_NPC_MAXWANDERSECONDS            3

#define TILEMAP_MAX_BOOTHS                      16
#define TILEMAP_MAX_SHOPITEMS                   8

#define TILEMAP_ENDING_TRIGGERINDEX_1           299
#define TILEMAP_ENDING_TRIGGERINDEX_2           300

typedef struct GameFlags_t GameFlags_t;
typedef struct Player_t Player_t;

typedef struct TilePortal_t
{
   uint32_t sourceTileIndex;
   uint32_t destinationTileMapIndex;
   uint32_t destinationTileIndex;
   Direction_t arrivalDirection;
}
TilePortal_t;

typedef struct EnemyIndexPool_t
{
   uint8_t enemyIndexes[TILE_ENEMY_POOL_ENEMY_INDEX_COUNT];
}
EnemyIndexPool_t;

typedef struct NonPlayerCharacter_t
{
   uint32_t id;
   uint32_t tileIndex;
   ActiveSprite_t sprite;

   Bool_t wanders;
   Bool_t isWandering;
   Vector4u32_t wanderBounds;
   float totalDuration;
   float duration;
}
NonPlayerCharacter_t;

typedef struct Booth_t
{
   uint32_t id;
   uint32_t tileIndex;
}
Booth_t;

typedef struct ShopItem_t
{
   uint32_t id;
   AccessoryType_t type;
   uint16_t price;
}
ShopItem_t;

typedef struct TileMap_t
{
   Screen_t* screen;
   GameFlags_t* gameFlags;
   Player_t* player;

   uint32_t id;

   Bool_t hasEncounters;
   Bool_t blocksMagic;
   Bool_t isDungeon;
   Bool_t isDark;
   Bool_t torchIsLit;
   uint32_t glowDiameter;
   uint32_t targetGlowDiameter;
   uint32_t glowTileCount;
   float glowTransitionSeconds;

   // bits 0-4: texture index (max 32 textures)
   // bit 5: is-passable flag
   // bits 6-7: walk speed (0 = normal, 3 = crawl)
   // bits 8-9: encounter rate
   // bits 10-11: damage rate
   // bits 12-15: enemy pool index
   uint16_t tiles[TILE_COUNT];
   uint32_t tilesX;
   uint32_t tilesY;
   Vector4i32_t viewport;
   Vector2u16_t viewportScreenPos;

   TileTexture_t textures[TILE_TEXTURE_COUNT];

   TilePortal_t portals[TILEMAP_MAX_PORTALS];
   uint32_t portalCount;
   TilePortal_t evacPortal;

   EnemyIndexPool_t overworldEnemyIndexPools[TILE_OVERWORLD_ENEMY_INDEX_POOLS];
   EnemyIndexPool_t dungeonEnemyIndexPools[TILE_DUNGEON_ENEMY_INDEX_POOLS];

   StaticSprite_t staticSprites[TILEMAP_MAX_STATICSPRITES];
   uint32_t staticSpriteCount;

   NonPlayerCharacter_t npcs[TILEMAP_MAX_NPCS];
   uint32_t npcCount;

   StaticSprite_t chestSprite;
   StaticSprite_t doorSprite;

   Booth_t booths[TILEMAP_MAX_BOOTHS];
   uint32_t boothCount;
   uint16_t boothPrice;

   ShopItem_t shopItems[TILEMAP_MAX_SHOPITEMS];
   uint32_t shopItemCount;
   ShopType_t shopType;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap, Screen_t* screen, GameFlags_t* gameFlags, Player_t* player );
void TileMap_CheckThroneRoomPrincess( TileMap_t* tileMap );
void TileMap_ResetNpcs( TileMap_t* tileMap );
void TileMap_Tic( TileMap_t* tileMap );
void TileMap_ResetViewport( TileMap_t* tileMap );
void TileMap_ChangeViewportSize( TileMap_t* tileMap, uint16_t w, uint16_t h );
void TileMap_UpdateViewport( TileMap_t* tileMap );
void TileMap_SetTargetGlowDiameter( TileMap_t* tileMap, uint32_t targetDiameter );
void TileMap_ReduceTargetGlowDiameter( TileMap_t* tileMap );
float TileMap_GetWalkSpeedForTileIndex( TileMap_t* tileMap, uint32_t tileIndex );
TilePortal_t* TileMap_GetPortalForTileIndex( TileMap_t* tileMap, uint32_t index );
uint32_t TileMap_GetFacingTileIndex( TileMap_t* tileMap, uint32_t sourceTileIndex, Direction_t direction );
void TileMap_Draw( TileMap_t* tileMap );
void TileMap_StopNpc( NonPlayerCharacter_t* npc );
void TileMap_TicNpcWander( NonPlayerCharacter_t* npc );
Bool_t TileMap_HasBoothAtIndex( TileMap_t* tileMap, uint32_t tileIndex, uint32_t* boothId );

// game_data.c
void TileMap_LoadTextures( TileMap_t* tileMap, TileTextureType_t type );
void TileMap_LoadEnemyIndexPools( TileMap_t* tileMap );
void TileMap_Load( TileMap_t* tileMap, uint32_t id );
void TileMap_LoadHiddenStairs( TileMap_t* tileMap );
uint32_t TileMap_GetTreasureFlag( uint32_t tileMapId, uint32_t tileIndex );
uint32_t TileMap_GetDoorFlag( uint32_t tileMapId, uint32_t tileIndex );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
