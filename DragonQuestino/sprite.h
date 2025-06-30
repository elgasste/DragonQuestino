#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"
#include "vector.h"

#define SPRITE_TEXTURE_SIZE            16
#define SPRITE_TEXTURE_BYTES           256

#define ACTIVE_SPRITE_TEXTURES         8  // two for each direction
#define ACTIVE_SPRITE_FRAMES           2
#define ACTIVE_SPRITE_FRAME_SECONDS    0.3f
#define ACTIVE_SPRITE_FLICKER_SECONDS  0.03f

typedef struct SpriteTexture_t
{
   uint8_t memory[SPRITE_TEXTURE_BYTES];
}
SpriteTexture_t;

typedef struct StaticSprite_t
{
   SpriteTexture_t texture;
   Vector2u32_t position;
}
StaticSprite_t;

typedef struct ActiveSprite_t
{
   SpriteTexture_t textures[ACTIVE_SPRITE_TEXTURES];
   Vector2f_t position;
   Direction_t direction;
   uint32_t currentFrame;
   float elapsedFrameSeconds;

   Bool_t isFlickering;
   Bool_t flickerOff;
   float elapsedFlickerSeconds;
}
ActiveSprite_t;

#if defined( __cplusplus )
extern "C" {
#endif

void ActiveSprite_Reset( ActiveSprite_t* sprite );
void ActiveSprite_SetDirection( ActiveSprite_t* sprite, Direction_t direction );
void ActiveSprite_Tic( ActiveSprite_t* sprite );
void ActiveSprite_Flicker( ActiveSprite_t* sprite );
void ActiveSprite_StopFlickering( ActiveSprite_t* sprite );

// game_data.c
void Sprite_LoadStatic( StaticSprite_t* sprite, uint32_t index );
void Sprite_LoadActive( ActiveSprite_t* sprite, uint32_t index );

#if defined( __cplusplus )
}
#endif

#endif // SPRITE_H
