#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "vector.h"
#include "sprite.h"

typedef struct Player_t
{
   Vector2f_t position;
   Vector2f_t velocity;
   float maxVelocity;
   Vector2u32_t hitBoxSize;

   Sprite_t sprite;
   Vector2i32_t spriteOffset;

   uint32_t tileIndex;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_Init( Player_t* player );

#if defined( __cplusplus )
}
#endif

#endif // PLAYER_H
