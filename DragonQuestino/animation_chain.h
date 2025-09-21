#if !defined( ANIMATION_H )
#define ANIMATION_H

#include "common.h"

#define ANIMATIONCHAIN_MAX_LINKS                         32

#define ANIMATION_PAUSE_DURATION                         0.2f
#define ANIMATION_WHITE_DURATION                         0.5f
#define ANIMATION_FADE_DURATION                          0.3f
#define ANIMATION_MIDFADE_DURATION                       2.0f
#define ANIMATION_SLOWFADE_DURATION                      6.0f
#define ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION          6.0f
#define ANIMATION_RAINBOWBRIDGE_FADE_DURATION            1.5f
#define ANIMATION_CASTSPELL_TOTALDURATION                0.5f
#define ANIMATION_CASTSPELL_FRAMEDURATION                0.05f
#define ANIMATION_BATTLE_CHECKERSQUARE_DURATION          0.011f
#define ANIMATION_BATTLE_ENEMYFADE_DURATION              0.25f
#define ANIMATION_BATTLE_ENEMYFADE_SLOWDURATION          2.0f
#define ANIMATION_BATTLE_ENEMYDAMAGE_DURATION            0.6f
#define ANIMATION_BATTLE_ENEMYDAMAGE_FRAMEDURATION       0.08f
#define ANIMATION_BATTLE_PLAYERDAMAGE_DURATION           0.4f
#define ANIMATION_BATTLE_PLAYERDAMAGE_FRAMEDURATION      0.025f
#define ANIMATION_BATTLE_PLAYERDEATH_TOTALDURATION       0.6f
#define ANIMATION_BATTLE_PLAYERDEATH_FRAMEDURATION       0.08f

typedef struct Screen_t Screen_t;
typedef struct TileMap_t TileMap_t;
typedef struct Game_t Game_t;

typedef struct AnimationChain_t
{
   Screen_t* screen;
   TileMap_t* tileMap;
   Game_t* game;

   AnimationId_t animationIds[ANIMATIONCHAIN_MAX_LINKS];
   void ( *callbacks[ANIMATIONCHAIN_MAX_LINKS] )( void* );
   void* callbackDatas[ANIMATIONCHAIN_MAX_LINKS];
   void ( *pendingCallback )( void* );
   void* pendingCallbackData;
   u32 animationCount;
   u32 activeAnimation;

   Bool_t isRunning;
   Bool_t startNext;
   r32 totalElapsedSeconds;
   r32 frameElapsedSeconds;
   r32 totalDuration;
   r32 frameDuration;
   Bool_t flag;
}
AnimationChain_t;

#if defined( __cplusplus )
extern "C" {
#endif

void AnimationChain_Init( AnimationChain_t* chain, Screen_t* screen, TileMap_t* tileMap, Game_t* game );
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
