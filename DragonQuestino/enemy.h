#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "battle_stats.h"

#define ENEMY_TILE_TEXTURE_COUNT       78
#define ENEMY_TILE_TEXTURE_SIZE_BYTES  64
#define ENEMY_TILE_SIZE                8
#define ENEMY_TILE_COUNT               120
#define ENEMY_TILES_X                  10
#define ENEMY_TILES_Y                  12

#define ENEMY_GREENDRAGON_INDEX        30
#define ENEMY_AXEKNIGHT_INDEX          33
#define ENEMY_GOLEM_INDEX              24

typedef struct Enemy_t
{
   uint32_t groupId;
   char name[16];
   char indefiniteArticle[3];
   uint8_t minHitPoints;
   uint8_t maxHitPoints;
   uint8_t experience;
   uint8_t minGold;
   uint8_t maxGold;
   uint8_t gold;
   BattleStats_t stats;

   uint8_t tileTextures[ENEMY_TILE_TEXTURE_COUNT][ENEMY_TILE_TEXTURE_SIZE_BYTES];
   int8_t tileTextureIndexes[ENEMY_TILE_COUNT];
}
Enemy_t;

#if defined( __cplusplus )
extern "C" {
#endif

float Enemy_GetFleeFactor( Enemy_t* enemy );

// game_data.c
void Enemy_Load( Enemy_t* enemy, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // ENEMY_H
