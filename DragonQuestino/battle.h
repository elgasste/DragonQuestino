#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"
#include "enemy.h"

#define ENEMY_SIZZ_MIN_DAMAGE                   3
#define ENEMY_SIZZ_REDUCEDMIN_DAMAGE            2
#define ENEMY_SIZZ_MAX_DAMAGE                   10
#define ENEMY_SIZZ_REDUCEDMAX_DAMAGE            6
#define ENEMY_SIZZLE_MIN_DAMAGE                 30
#define ENEMY_SIZZLE_REDUCEDMIN_DAMAGE          20
#define ENEMY_SIZZLE_MAX_DAMAGE                 45
#define ENEMY_SIZZLE_REDUCEDMAX_DAMAGE          30
#define ENEMY_HEAL_MIN_RECOVERY                 20
#define ENEMY_HEAL_MAX_RECOVERY                 27
#define ENEMY_MIDHEAL_MIN_RECOVERY              85
#define ENEMY_MIDHEAL_MAX_RECOVERY              100
#define FIRE_BREATH_MIN_DAMAGE                  16
#define FIRE_BREATH_MAX_DAMAGE                  23
#define STRONG_FIRE_BREATH_MIN_DAMAGE           65
#define STRONG_FIRE_BREATH_MAX_DAMAGE           72

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
void Battle_Attack( Battle_t* battle );
void Battle_Flee( Battle_t* battle );
void Battle_AttackSucceededCallback( Battle_t* battle );
void Battle_SwitchTurn( Battle_t* battle );

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
