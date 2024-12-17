#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"

#define SPRITE_TEXTURE_SIZE      16
#define SPRITE_TEXTURE_BYTES     256
#define SPRITE_TEXTURES          8  // two for each direction
#define SPRITE_FRAMES            2
#define SPRITE_FRAME_SECONDS     0.3f

typedef struct SpriteTexture_t
{
   uint8_t memory[SPRITE_TEXTURE_BYTES];
}
SpriteTexture_t;

typedef struct Sprite_t
{
   SpriteTexture_t textures[SPRITE_TEXTURES];
   Direction_t direction;
   uint32_t currentFrame;
   float elapsedFrameSeconds;
}
Sprite_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Sprite_SetDirection( Sprite_t* sprite, Direction_t direction );
void Sprite_Tic( Sprite_t* sprite );

// game_data.c
void Sprite_LoadPlayer( Sprite_t* sprite );
void Sprite_LoadGeneric( Sprite_t* sprite, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // SPRITE_H
