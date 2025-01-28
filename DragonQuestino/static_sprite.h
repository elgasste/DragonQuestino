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

#if defined( __cplusplus )
extern "C" {
#endif

// game_data.c
void StaticSprite_Load( StaticSprite_t* sprite, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // STATIC_SPRITE_H
