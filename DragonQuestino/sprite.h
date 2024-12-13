#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"

#define SPRITE_TEXTURE_SIZE      16
#define SPRITE_TEXTURE_BYTES     256
#define SPRITE_TEXTURES          8  // two for each direction

typedef struct SpriteTexture_t
{
   uint8_t memory[SPRITE_TEXTURE_BYTES];
}
SpriteTexture_t;

typedef struct Sprite_t
{
   SpriteTexture_t textures[8];
   Direction_t direction;
}
Sprite_t;

#if defined( __cplusplus )
extern "C" {
#endif

// TODO: implement in game_data.c
void Sprite_Load( Sprite_t* sprite, uint8_t index );

#if defined( __cplusplus )
}
#endif

#endif // SPRITE_H
