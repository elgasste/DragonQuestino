#include "enemy.h"

r32 Enemy_GetFleeFactor( Enemy_t* enemy )
{
   r32 result = 0.0f;

   switch ( enemy->groupId )
   {
      case 0: result = 0.25f; break;
      case 1: result = 0.375f; break;
      case 2: result = 0.5f; break;
      case 3: result = 1.0f; break;
   }

   return result;
}
