#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "vector.h"
#include "sprite.h"

#define PLAYER_VELOCITY_NORMAL      72.0f    // pixels per second
#define PLAYER_VELOCITY_SLOW        60.0f
#define PLAYER_VELOCITY_VERYSLOW    48.0f
#define PLAYER_VELOCITY_CRAWL       36.0f

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
