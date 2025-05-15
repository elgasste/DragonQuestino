#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "battle_stats.h"

typedef struct Enemy_t
{
   char name[16];
   char indefiniteArticle[3];
   BattleStats_t stats;
   uint16_t experience;
   uint16_t gold;
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
