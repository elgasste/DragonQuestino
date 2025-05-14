#if !defined( ANIMATION_H )
#define ANIMATION_H

#include "common.h"

#define ANIMATION_OVERWORLD_PAUSE_DURATION         0.3f
#define ANIMATION_TILEMAP_FADE_DURATION            0.3f
#define ANIMATION_TILEMAP_FADEPAUSE_DURATION       0.2f
#define ANIMATION_TILEMAP_WHITE_DURATION           0.5f
#define ANIMATION_TILEMAP_WHITEPAUSE_DURATION      1.0f
#define ANIMATION_CASTSPELL_TOTALDURATION          0.2f
#define ANIMATION_CASTSPELL_FRAMEDURATION          0.005f
#define ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION    6.0f
#define ANIMATION_RAINBOWBRIDGE_WHITEOUT_DURATION  1.5f
#define ANIMATION_RAINBOWBRIDGE_FADEIN_DURATION    1.5f
#define ANIMATION_RAINBOWBRIDGE_PAUSE_DURATION     1.5f
#define ANIMATION_BATTLE_CHECKERSQUARE_DURATION    0.011f

typedef struct Game_t Game_t;

typedef struct Animation_t
{
   Game_t* game;

   Bool_t isRunning;
   AnimationId_t id;
   float totalElapsedSeconds;
   float frameElapsedSeconds;
   float totalDuration;
}
Animation_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Animation_Init( Animation_t* animation, Game_t* game );
void Animation_Start( Animation_t* animation, AnimationId_t id );
void Animation_Tic( Animation_t* animation );

#if defined( __cplusplus )
}
#endif

#endif // ANIMATION_H
