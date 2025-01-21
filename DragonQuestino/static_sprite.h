#if !defined( STATIC_SPRITE_H )
#define STATIC_SPRITE_H

#include "sprite_texture.h"
#include "vector.h"

typedef struct StaticSprite_t
{
   SpriteTexture_t texture;
   Vector2f_t position;
}
StaticSprite_t;

#endif // STATIC_SPRITE_H
