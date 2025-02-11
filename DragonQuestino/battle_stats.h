#if !defined( BATTLE_STATS_H )
#define BATTLE_STATS_H

#include "common.h"

typedef struct BattleStats_t
{
   uint8_t hitPoints;
   uint8_t maxHitPoints;
   uint8_t magicPoints;
   uint8_t maxMagicPoints;
   uint8_t attackPower;
   uint8_t defensePower;
   uint8_t agility;
   uint8_t luck;
}
BattleStats_t;

#endif // BATTLE_STATS_H
