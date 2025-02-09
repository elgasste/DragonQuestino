#if !defined( ENUMS_H )
#define ENUMS_H

#include "common.h"

typedef enum Direction_t
{
   Direction_Left = 0,
   Direction_Up,
   Direction_Right,
   Direction_Down,

   Direction_Count
}
Direction_t;

typedef enum GameState_t
{
   GameState_Overworld = 0,
   GameState_OverworldMenu,
   GameState_TileMapTransition,

   GameState_Count
}
GameState_t;

typedef enum MenuId_t
{
   MenuId_Overworld = 0,

   MenuId_Count
}
MenuId_t;

#endif // ENUMS_H
