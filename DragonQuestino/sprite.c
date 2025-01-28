#include "sprite.h"
#include "clock.h"

void Sprite_SetDirection( ActiveSprite_t* sprite, Direction_t direction )
{
   if ( direction != sprite->direction )
   {
      sprite->direction = direction;
      sprite->currentFrame = 0;
      sprite->elapsedFrameSeconds = 0.0f;
   }
}

void Sprite_Tic( ActiveSprite_t* sprite )
{
   sprite->elapsedFrameSeconds += CLOCK_FRAME_SECONDS;

   while ( sprite->elapsedFrameSeconds > ACTIVE_SPRITE_FRAME_SECONDS )
   {
      sprite->elapsedFrameSeconds -= ACTIVE_SPRITE_FRAME_SECONDS;
      sprite->currentFrame++;

      if ( sprite->currentFrame >= ACTIVE_SPRITE_FRAMES )
      {
         sprite->currentFrame = 0;
      }
   }
}
