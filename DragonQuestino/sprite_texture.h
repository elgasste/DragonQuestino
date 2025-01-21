#if !defined( SPRITE_TEXTURE_H )
#define SPRITE_TEXTURE_H

#include "common.h"

#define SPRITE_TEXTURE_SIZE      16
#define SPRITE_TEXTURE_BYTES     256

typedef struct SpriteTexture_t
{
   uint8_t memory[SPRITE_TEXTURE_BYTES];
}
SpriteTexture_t;

#endif // SPRITE_TEXTURE_H
