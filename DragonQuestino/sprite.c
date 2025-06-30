#include "sprite.h"
#include "clock.h"

void ActiveSprite_Reset( ActiveSprite_t* sprite )
{
   sprite->currentFrame = 0;
   sprite->elapsedFrameSeconds = 0.0f;
   sprite->isFlickering = False;
}

void ActiveSprite_SetDirection( ActiveSprite_t* sprite, Direction_t direction )
{
   if ( direction != sprite->direction )
   {
      sprite->direction = direction;
      sprite->currentFrame = 0;
      sprite->elapsedFrameSeconds = 0.0f;
   }
}

void ActiveSprite_Tic( ActiveSprite_t* sprite )
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

void ActiveSprite_Flicker( ActiveSprite_t* sprite )
{
   if ( sprite->isFlickering )
   {
      sprite->elapsedFlickerSeconds += CLOCK_FRAME_SECONDS;

      while ( sprite->elapsedFlickerSeconds > ACTIVE_SPRITE_FLICKER_SECONDS )
      {
         sprite->elapsedFlickerSeconds -= ACTIVE_SPRITE_FLICKER_SECONDS;
         TOGGLE_BOOL( sprite->flickerOff );
      }
   }
   else
   {
      sprite->isFlickering = True;
      sprite->flickerOff = True;
      sprite->elapsedFlickerSeconds = 0.0f;
   }
}

void ActiveSprite_StopFlickering( ActiveSprite_t* sprite )
{
   sprite->isFlickering = False;
   sprite->flickerOff = False;
}
