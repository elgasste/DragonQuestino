#if !defined( BATTLE_STATS_H )
#define BATTLE_STATS_H

#include "common.h"

typedef struct BattleStats_t
{
   uint8_t hitPoints;
   uint8_t maxHitPoints;
   uint8_t magicPoints;
   uint8_t maxMagicPoints;
   uint8_t strength;
   uint8_t agility;
   uint8_t sleepResist;
   uint8_t stopSpellResist;
   uint8_t hurtResist;
   uint8_t dodge;
   Bool_t isAsleep;
   Bool_t isFizzled;
}
BattleStats_t;

#endif // BATTLE_STATS_H
