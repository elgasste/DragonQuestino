#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"
#include "enemy.h"

typedef struct Player_t Player_t;
typedef struct TileMap_t TileMap_t;

typedef struct Battle_t
{
   Player_t* player;
   TileMap_t* tileMap;

   Enemy_t enemy;
}
Battle_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Battle_Init( Battle_t* battle, Player_t* player, TileMap_t* tileMap );
void Battle_Generate( Battle_t* battle );

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
