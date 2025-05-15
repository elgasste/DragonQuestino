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

#endif // ENEMY_H
