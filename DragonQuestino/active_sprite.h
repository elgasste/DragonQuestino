#if !defined( ACTIVE_SPRITE_H )
#define ACTIVE_SPRITE_H

#define ACTIVE_SPRITE_TEXTURES          8  // two for each direction
#define ACTIVE_SPRITE_FRAMES            2
#define ACTIVE_SPRITE_FRAME_SECONDS     0.3f

#include "sprite_texture.h"

typedef struct ActiveSprite_t
{
   SpriteTexture_t textures[ACTIVE_SPRITE_TEXTURES];
   Direction_t direction;
   uint32_t currentFrame;
   float elapsedFrameSeconds;
}
ActiveSprite_t;

#if defined( __cplusplus )
extern "C" {
#endif

void ActiveSprite_SetDirection( ActiveSprite_t* sprite, Direction_t direction );
void ActiveSprite_Tic( ActiveSprite_t* sprite );

// game_data.c
void ActiveSprite_LoadPlayer( ActiveSprite_t* sprite );
void ActiveSprite_LoadGeneric( ActiveSprite_t* sprite, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // ACTIVE_SPRITE_H
