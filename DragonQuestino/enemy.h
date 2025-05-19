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

#define ENEMY_GREENDRAGON_INDEX        35
#define ENEMY_AXEKNIGHT_INDEX          33
#define ENEMY_GOLEM_INDEX              27

typedef struct Enemy_t
{
   char name[16];
   char indefiniteArticle[3];
   BattleStats_t stats;
   uint16_t experience;
   uint16_t gold;

   uint8_t tileTextures[ENEMY_TILE_TEXTURE_COUNT][ENEMY_TILE_TEXTURE_SIZE_BYTES];
   int8_t tileTextureIndexes[ENEMY_TILE_COUNT];
}
Enemy_t;

#if defined( __cplusplus )
extern "C" {
#endif

// game_data.c
void Enemy_Load( Enemy_t* enemy, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // ENEMY_H
