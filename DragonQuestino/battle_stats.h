#if !defined( BATTLE_STATS_H )
#define BATTLE_STATS_H

#include "common.h"

#define SPELL_HEAL_MP      4
#define SPELL_SIZZ_MP      2
#define SPELL_SLEEP_MP     2
#define SPELL_GLOW_MP      3
#define SPELL_FIZZLE_MP    2
#define SPELL_EVAC_MP      6
#define SPELL_ZOOM_MP      8
#define SPELL_REPEL_MP     2
#define SPELL_MIDHEAL_MP   10
#define SPELL_SIZZLE_MP    5

typedef struct BattleStats_t
{
   uint8_t hitPoints;
   uint8_t maxHitPoints;
   uint8_t magicPoints;
   uint8_t maxMagicPoints;
   uint8_t attackPower;
   uint8_t defensePower;
   uint8_t strength;
   uint8_t agility;
}
BattleStats_t;

#endif // BATTLE_STATS_H
