#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"
#include "enemy.h"

typedef struct Game_t Game_t;

typedef struct Battle_t
{
   Game_t* game;

   Enemy_t enemy;
   SpecialEnemy_t specialEnemy;
   BattleTurn_t turn;

   Bool_t excellentMove;
   Bool_t isOver;

   uint8_t pendingPayload8u;

   uint16_t experienceGained;
   uint16_t goldGained;
   uint8_t strengthGained;
   uint8_t agilityGained;
   uint8_t hitPointsGained;
   uint8_t magicPointsGained;
   uint16_t previousSpells;
   uint8_t newLevel;
}
Battle_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Battle_Init( Battle_t* battle, Game_t* game );
void Battle_Generate( Battle_t* battle );
void Battle_AttemptAttack( Battle_t* battle );
void Battle_AttemptFlee( Battle_t* battle );
void Battle_AttackSucceededCallback( Battle_t* battle );
void Battle_SwitchTurn( Battle_t* battle );

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
