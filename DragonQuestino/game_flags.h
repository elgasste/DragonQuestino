#if !defined( GAME_FLAGS_H )
#define GAME_FLAGS_H

#include "common.h"

typedef struct GameFlags_t
{
   // low 16 bits: permanent doors (they stay opened)
   // high 16 bits: temporary doors (they close again when you leave the tile map)
   uint32_t doors;

   uint32_t treasures;
   uint8_t specialEnemies;

   Bool_t gotStaffOfRain;
   Bool_t gotRainbowDrop;
   Bool_t usedRainbowDrop;
   Bool_t foundHiddenStairs;
   Bool_t rescuedPrincess;
   Bool_t joinedDragonlord;
   Bool_t defeatedDragonlord;
}
GameFlags_t;

#endif // GAME_FLAGS_H
