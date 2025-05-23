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
}
Battle_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Battle_Init( Battle_t* battle, Game_t* game );
void Battle_Generate( Battle_t* battle );
void Battle_AttemptFlee( Battle_t* battle );

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
