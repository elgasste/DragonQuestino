#if !defined( ANIMATION_H )
#define ANIMATION_H

#include "common.h"

#define ANIMATION_OVERWORLD_PAUSE_DURATION            0.2f
#define ANIMATION_TILEMAP_FADE_DURATION               0.3f
#define ANIMATION_TILEMAP_FADEPAUSE_DURATION          0.2f
#define ANIMATION_TILEMAP_WHITE_DURATION              0.5f
#define ANIMATION_TILEMAP_WHITEPAUSE_DURATION         1.0f
#define ANIMATION_CASTSPELL_TOTALDURATION             0.2f
#define ANIMATION_CASTSPELL_FRAMEDURATION             0.005f
#define ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION       6.0f
#define ANIMATION_RAINBOWBRIDGE_WHITEOUT_DURATION     1.5f
#define ANIMATION_RAINBOWBRIDGE_FADEIN_DURATION       1.5f
#define ANIMATION_RAINBOWBRIDGE_PAUSE_DURATION        1.5f
#define ANIMATION_BATTLE_CHECKERSQUARE_DURATION       0.011f
#define ANIMATION_BATTLE_ENEMYFADEIN_DURATION         0.25f
#define ANIMATION_BATTLE_ENEMYFADEOUT_DURATION        0.25f
#define ANIMATION_BATTLE_ENEMYFADEINPAUSE_DURATION    0.1f
#define ANIMATION_BATTLE_ENEMYDAMAGE_DURATION         0.6f
#define ANIMATION_BATTLE_ENEMYDAMAGE_FRAMEDURATION    0.1f
#define ANIMATION_BATTLE_ENEMYDODGE_DURATION          0.5f
#define ANIMATION_BATTLE_VICTORYPAUSE_DURATION        0.6f

#define ANIMATIONCHAIN_MAX_LINKS                         16

#define ANIMATIONCHAIN_PAUSE_DURATION                    0.3f
#define ANIMATIONCHAIN_WHITE_DURATION                    0.5f
#define ANIMATIONCHAIN_FADE_DURATION                     0.3f
#define ANIMATIONCHAIN_RAINBOWBRIDGE_TRIPPY_DURATION     6.0f
#define ANIMATIONCHAIN_RAINBOWBRIDGE_FADE_DURATION       1.5f

typedef struct Game_t Game_t;

typedef struct Animation_t
{
   Game_t* game;

   AnimationId_t id;
   float totalElapsedSeconds;
   float frameElapsedSeconds;
   float totalDuration;
   Bool_t flag;
}
Animation_t;

typedef struct Screen_t Screen_t;

typedef struct AnimationChain_t
{
   Screen_t* screen;

   AnimationId_t animationIds[ANIMATIONCHAIN_MAX_LINKS];
   void ( *callbacks[ANIMATIONCHAIN_MAX_LINKS] )( void* );
   void* callbackDatas[ANIMATIONCHAIN_MAX_LINKS];
   uint32_t animationCount;
   uint32_t activeAnimation;

   Bool_t isRunning;
   float totalElapsedSeconds;
   float frameElapsedSeconds;
   float totalDuration;
}
AnimationChain_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Animation_Init( Animation_t* animation, Game_t* game );
void Animation_Start( Animation_t* animation, AnimationId_t id );

void AnimationChain_Init( AnimationChain_t* chain, Screen_t* screen );
void AnimationChain_Reset( AnimationChain_t* chain );
void AnimationChain_PushAnimation( AnimationChain_t* chain, AnimationId_t id );
void AnimationChain_PushAnimationWithCallback( AnimationChain_t* chain, AnimationId_t id, void ( *callback )( void* ), void* callbackData );
void AnimationChain_Start( AnimationChain_t* chain );
void AnimationChain_Tic( AnimationChain_t* chain );
AnimationId_t AnimationChain_GetActiveAnimationId( AnimationChain_t* chain );

#if defined( __cplusplus )
}
#endif

#endif // ANIMATION_H
