#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "battle_stats.h"

#define ENEMY_TILE_TEXTURE_COUNT       120
#define ENEMY_TILE_TEXTURE_SIZE_BYTES  64
#define ENEMY_TILE_SIZE                8
#define ENEMY_TILE_COUNT               120
#define ENEMY_TILES_X                  10
#define ENEMY_TILES_Y                  12

#define ENEMY_SLIME_ID                 0
#define ENEMY_REDSLIME_ID              1
#define ENEMY_DRAKEE_ID                2
#define ENEMY_GHOST_ID                 3
#define ENEMY_MAGICIAN_ID              4
#define ENEMY_MAGIDRAKEE_ID            5
#define ENEMY_SCORPION_ID              6
#define ENEMY_DRUIN_ID                 7
#define ENEMY_POLTERGEIST_ID           8
#define ENEMY_DROLL_ID                 9
#define ENEMY_DRAKEEMA_ID              10
#define ENEMY_SKELETON_ID              11
#define ENEMY_WARLOCK_ID               12
#define ENEMY_METALSCORPION_ID         13
#define ENEMY_WOLF_ID                  14
#define ENEMY_WRAITH_ID                15
#define ENEMY_METALSLIME_ID            16
#define ENEMY_SPECTER_ID               17
#define ENEMY_WOLFLORD_ID              18
#define ENEMY_DRUINLORD_ID             19
#define ENEMY_DROLLMAGI_ID             20
#define ENEMY_WYVERN_ID                21
#define ENEMY_ROGUESCORPION_ID         22
#define ENEMY_WRAITHKNIGHT_ID          23
#define ENEMY_GOLEM_ID                 24
#define ENEMY_GOLDMAN_ID               25
#define ENEMY_KNIGHT_ID                26
#define ENEMY_MAGIWYVERN_ID            27
#define ENEMY_DEMONKNIGHT_ID           28
#define ENEMY_WEREWOLF_ID              29
#define ENEMY_GREENDRAGON_ID           30
#define ENEMY_STARWYVERN_ID            31
#define ENEMY_WIZARD_ID                32
#define ENEMY_AXEKNIGHT_ID             33
#define ENEMY_BLUEDRAGON_ID            34
#define ENEMY_STONEMAN_ID              35
#define ENEMY_ARMOREDKNIGHT_ID         36
#define ENEMY_REDDRAGON_ID             37
#define ENEMY_DRAGONLORDWIZARD_ID      38
#define ENEMY_DRAGONLORDDRAGON_ID      39

typedef struct Enemy_t
{
   uint32_t id;
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
