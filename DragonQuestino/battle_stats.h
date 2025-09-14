#if !defined( BATTLE_STATS_H )
#define BATTLE_STATS_H

#include "common.h"

typedef struct BattleStats_t
{
   u8 hitPoints;
   u8 maxHitPoints;
   u8 magicPoints;
   u8 maxMagicPoints;
   u8 strength;
   u8 agility;
   u8 sleepResist;
   u8 stopSpellResist;
   u8 hurtResist;
   u8 dodge;
   Bool_t isAsleep;
   Bool_t isFizzled;
}
BattleStats_t;

#endif // BATTLE_STATS_H
