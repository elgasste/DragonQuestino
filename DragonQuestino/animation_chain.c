#include "animation_chain.h"
#include "game.h"
#include "vector.h"
#include "math.h"

#define ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( c ) \
   c->totalElapsedSeconds += CLOCK_FRAME_SECONDS; \
   if ( c->totalElapsedSeconds > c->totalDuration ) \
   { \
      AnimationChain_AnimationFinished( c ); \
      return; \
   } \

internal void AnimationChain_StartAnimation( AnimationChain_t* chain );
internal void AnimationChain_AnimationFinished( AnimationChain_t* chain );
internal void AnimationChain_Tic_Pause( AnimationChain_t* chain );
internal void AnimationChain_Tic_WhiteOut( AnimationChain_t* chain );
internal void AnimationChain_Tic_WhiteIn( AnimationChain_t* chain );
internal void AnimationChain_Tic_FadeOut( AnimationChain_t* chain );
internal void AnimationChain_Tic_FadeIn( AnimationChain_t* chain );
internal void AnimationChain_Tic_RainbowBridge_Trippy( AnimationChain_t* chain );
internal void AnimationChain_Tic_RainbowBridge_WhiteOut( AnimationChain_t* chain );
internal void AnimationChain_Tic_RainbowBridge_FadeIn( AnimationChain_t* chain );
internal void AnimationChain_Tic_Flash( AnimationChain_t* chain );
internal void AnimationChain_Tic_Battle_Checkerboard( AnimationChain_t* chain );
internal void AnimationChain_Tic_Battle_EnemyFadeIn( AnimationChain_t* chain );
internal void AnimationChain_Tic_Battle_EnemyFadeOut( AnimationChain_t* chain );
internal void AnimationChain_Tic_Battle_EnemyDamage( AnimationChain_t* chain );
internal void AnimationChain_Tic_Battle_PlayerDamage( AnimationChain_t* chain );

internal Vector2u16_t g_battleCheckerboardPos[49] =
{
   { 144, 100 }, { 160, 100 }, { 160, 116 }, { 144, 116 }, { 128, 116 }, { 128, 100 }, { 128, 84  },
   { 144, 84  }, { 160, 84  }, { 176, 84  }, { 176, 100 }, { 176, 116 }, { 176, 132 }, { 160, 132 },
   { 144, 132 }, { 128, 132 }, { 112, 132 }, { 112, 116 }, { 112, 100 }, { 112, 84  }, { 112, 68  },
   { 128, 68  }, { 144, 68  }, { 160, 68  }, { 176, 68  }, { 192, 68  }, { 192, 84  }, { 192, 100 },
   { 192, 116 }, { 192, 132 }, { 192, 148 }, { 176, 148 }, { 160, 148 }, { 144, 148 }, { 128, 148 },
   { 112, 148 }, { 96,  148 }, { 96,  132 }, { 96,  116 }, { 96,  100 }, { 96,  84  }, { 96,  68  },
   { 96,  52  }, { 112, 52  }, { 128, 52  }, { 144, 52  }, { 160, 52  }, { 176, 52  }, { 192, 52  }
};
internal uint32_t g_squaresDrawn;

void AnimationChain_Init( AnimationChain_t* chain, Screen_t* screen, TileMap_t* tileMap, Game_t* game )
{
   chain->screen = screen;
   chain->tileMap = tileMap;
   chain->game = game;
}

void AnimationChain_Reset( AnimationChain_t* chain )
{
   chain->animationCount = 0;
   chain->pendingCallback = 0;
}

void AnimationChain_PushAnimation( AnimationChain_t* chain, AnimationId_t id )
{
   AnimationChain_PushAnimationWithCallback( chain, id, 0, 0 );
}

void AnimationChain_PushAnimationWithCallback( AnimationChain_t* chain, AnimationId_t id, void ( *callback )( void* ), void* callbackData )
{
   chain->animationIds[chain->animationCount] = id;
   chain->callbacks[chain->animationCount] = callback;
   chain->callbackDatas[chain->animationCount] = callbackData;
   chain->animationCount++;
}

void AnimationChain_Start( AnimationChain_t* chain )
{
   chain->activeAnimation = 0;
   chain->isRunning = True;
   chain->startNext = True;
   AnimationChain_StartAnimation( chain );
}

void AnimationChain_Tic( AnimationChain_t* chain )
{
   if ( chain->startNext )
   {
      AnimationChain_StartAnimation( chain );
   }
   else
   {
      switch ( chain->animationIds[chain->activeAnimation] )
      {
         case AnimationId_Pause:
         case AnimationId_ActivePause:
            AnimationChain_Tic_Pause( chain );
            break;
         case AnimationId_WhiteOut: AnimationChain_Tic_WhiteOut( chain ); break;
         case AnimationId_WhiteIn: AnimationChain_Tic_WhiteIn( chain ); break;
         case AnimationId_FadeOut:
         case AnimationId_MidFadeOut:
         case AnimationId_SlowFadeOut:
            AnimationChain_Tic_FadeOut( chain );
            break;
         case AnimationId_FadeIn:
         case AnimationId_MidFadeIn:
            AnimationChain_Tic_FadeIn( chain );
            break;
         case AnimationId_RainbowBridge_Trippy: AnimationChain_Tic_RainbowBridge_Trippy( chain ); break;
         case AnimationId_RainbowBridge_WhiteOut: AnimationChain_Tic_RainbowBridge_WhiteOut( chain ); break;
         case AnimationId_RainbowBridge_FadeIn: AnimationChain_Tic_RainbowBridge_FadeIn( chain ); break;
         case AnimationId_CastSpell:
         case AnimationId_TileDeath:
         case AnimationId_Battle_PlayerDeath:
            AnimationChain_Tic_Flash( chain );
            break;
         case AnimationId_Battle_Checkerboard: AnimationChain_Tic_Battle_Checkerboard( chain ); break;
         case AnimationId_Battle_EnemyFadeIn:
         case AnimationId_Battle_EnemySlowFadeIn:
            AnimationChain_Tic_Battle_EnemyFadeIn( chain );
            break;
         case AnimationId_Battle_EnemyFadeOut:
         case AnimationId_Battle_EnemySlowFadeOut:
            AnimationChain_Tic_Battle_EnemyFadeOut( chain );
            break;
         case AnimationId_Battle_EnemyDamage: AnimationChain_Tic_Battle_EnemyDamage( chain ); break;
         case AnimationId_Battle_PlayerDamage: AnimationChain_Tic_Battle_PlayerDamage( chain ); break;
      }
   }
}

AnimationId_t AnimationChain_GetActiveAnimationId( AnimationChain_t* chain )
{
   return chain->animationIds[chain->activeAnimation];
}

internal void AnimationChain_StartAnimation( AnimationChain_t* chain )
{
   chain->startNext = False;

   switch ( chain->animationIds[chain->activeAnimation] )
   {
      case AnimationId_Pause:
      case AnimationId_ActivePause:
         chain->totalDuration = ANIMATION_PAUSE_DURATION;
         break;
      case AnimationId_WhiteOut:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATION_WHITE_DURATION;
         break;
      case AnimationId_WhiteIn: chain->totalDuration = ANIMATION_WHITE_DURATION; break;
      case AnimationId_FadeOut:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATION_FADE_DURATION;
         break;
      case AnimationId_MidFadeOut:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATION_MIDFADE_DURATION;
         break;
      case AnimationId_SlowFadeOut:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATION_SLOWFADE_DURATION;
         break;
      case AnimationId_FadeIn: chain->totalDuration = ANIMATION_FADE_DURATION; break;
      case AnimationId_MidFadeIn: chain->totalDuration = ANIMATION_MIDFADE_DURATION; break;
      case AnimationId_RainbowBridge_Trippy:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION;
         break;
      case AnimationId_RainbowBridge_WhiteOut:
      case AnimationId_RainbowBridge_FadeIn:
         chain->totalDuration = ANIMATION_RAINBOWBRIDGE_FADE_DURATION;
         break;
      case AnimationId_Battle_Checkerboard:
         g_squaresDrawn = 0;
         break;
      case AnimationId_CastSpell:
         chain->screen->wipeColor = COLOR_WHITE;
         chain->totalDuration = ANIMATION_CASTSPELL_TOTALDURATION;
         chain->frameDuration = ANIMATION_CASTSPELL_FRAMEDURATION;
         break;
      case AnimationId_Battle_EnemyFadeIn:
      case AnimationId_Battle_EnemyFadeOut:
         chain->totalDuration = ANIMATION_BATTLE_ENEMYFADE_DURATION;
         break;
      case AnimationId_Battle_EnemySlowFadeIn:
      case AnimationId_Battle_EnemySlowFadeOut:
         chain->totalDuration = ANIMATION_BATTLE_ENEMYFADE_SLOWDURATION;
         break;
      case AnimationId_Battle_EnemyDamage:
         chain->totalDuration = ANIMATION_BATTLE_ENEMYDAMAGE_DURATION;
         chain->flag = True;
         break;
      case AnimationId_Battle_PlayerDamage:
         chain->totalDuration = ANIMATION_BATTLE_PLAYERDAMAGE_DURATION;
         chain->flag = True;
         break;
      case AnimationId_TileDeath:
      case AnimationId_Battle_PlayerDeath:
         chain->screen->wipeColor = COLOR_DEEPRED;
         chain->totalDuration = ANIMATION_BATTLE_PLAYERDEATH_TOTALDURATION;
         chain->frameDuration = ANIMATION_BATTLE_PLAYERDEATH_FRAMEDURATION;
         break;
   }

   chain->totalElapsedSeconds = 0.0f;
   chain->frameElapsedSeconds = 0.0f;
}

internal void AnimationChain_AnimationFinished( AnimationChain_t* chain )
{
   switch ( chain->animationIds[chain->activeAnimation] )
   {
      case AnimationId_FadeIn:
      case AnimationId_MidFadeIn:
      case AnimationId_WhiteIn:
      case AnimationId_RainbowBridge_FadeIn:
      case AnimationId_Battle_EnemyFadeIn:
      case AnimationId_Battle_EnemySlowFadeIn:
      case AnimationId_Battle_EnemyFadeOut:
      case AnimationId_Battle_EnemySlowFadeOut:
         Screen_RestorePalette( chain->screen );
         break;
      case AnimationId_Battle_Checkerboard:
         Screen_BackupPalette( chain->screen );
         Screen_ClearPalette( chain->screen, COLOR_BLACK );
         break;
      case AnimationId_Battle_EnemyDamage:
         Game_DrawEnemy( chain->game );
         break;
      case AnimationId_Battle_PlayerDamage:
         Game_DrawQuickStatus( chain->game );
         Dialog_Draw( &( chain->game->dialog ) );
         Game_WipeEnemy( chain->game );
         Game_DrawEnemy( chain->game );
         break;
   }

   if ( chain->callbacks[chain->activeAnimation] )
   {
      chain->pendingCallback = chain->callbacks[chain->activeAnimation];
      chain->pendingCallbackData = chain->callbackDatas[chain->activeAnimation];
   }

   chain->activeAnimation++;

   if ( chain->activeAnimation == chain->animationCount )
   {
      chain->isRunning = False;
   }
   else
   {
      chain->startNext = True;
   }
}

internal void AnimationChain_Tic_Pause( AnimationChain_t* chain )
{
   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )
}

internal void AnimationChain_Tic_WhiteOut( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;
   Screen_t* screen = chain->screen;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = 0x1F - ( screen->backupPalette[i] >> 11 );
      rangeG = 0x3F - ( ( screen->backupPalette[i] & 0x7E0 ) >> 5 );
      rangeB = 0x1F - ( screen->backupPalette[i] & 0x1F );
      p = chain->totalElapsedSeconds / chain->totalDuration;
      screen->palette[i] = ( ( ( screen->backupPalette[i] >> 11 ) + ( uint16_t )( rangeR * p ) ) << 11 ) |
         ( ( ( ( screen->backupPalette[i] >> 5 ) & 0x3F ) + ( uint16_t )( rangeG * p ) ) << 5 ) |
         ( ( ( ( screen->backupPalette[i] ) & 0x1F ) + ( uint16_t )( rangeB * p ) ) );
   }
}

internal void AnimationChain_Tic_WhiteIn( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;
   Screen_t* screen = chain->screen;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = 0x1F - ( screen->backupPalette[i] >> 11 );
      rangeG = 0x3F - ( ( screen->backupPalette[i] & 0x7E0 ) >> 5 );
      rangeB = 0x1F - ( screen->backupPalette[i] & 0x1F );
      p = 1.0f - ( chain->totalElapsedSeconds / chain->totalDuration );
      screen->palette[i] = ( ( ( screen->backupPalette[i] >> 11 ) + ( uint16_t )( rangeR * p ) ) << 11 ) |
         ( ( ( ( screen->backupPalette[i] >> 5 ) & 0x3F ) + ( uint16_t )( rangeG * p ) ) << 5 ) |
         ( ( ( ( screen->backupPalette[i] ) & 0x1F ) + ( uint16_t )( rangeB * p ) ) );
   }
}

internal void AnimationChain_Tic_FadeOut( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;
   Screen_t* screen = chain->screen;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )
   
   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = screen->backupPalette[i] >> 11;
      rangeG = ( screen->backupPalette[i] & 0x7E0 ) >> 5;
      rangeB = screen->backupPalette[i] & 0x1F;
      p =  1.0f - ( chain->totalElapsedSeconds / chain->totalDuration ), 0.0f;
      screen->palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
   }
}

internal void AnimationChain_Tic_FadeIn( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;
   Screen_t* screen = chain->screen;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = screen->backupPalette[i] >> 11;
      rangeG = ( screen->backupPalette[i] & 0x7E0 ) >> 5;
      rangeB = screen->backupPalette[i] & 0x1F;
      p = chain->totalElapsedSeconds / chain->totalDuration;
      screen->palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
   }
}

internal void AnimationChain_Tic_RainbowBridge_Trippy( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG, increment;
   float p;
   Screen_t* screen = chain->screen;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = UINT8_MAX - ( screen->backupPalette[i] >> 11 );
      rangeG = UINT8_MAX - ( ( screen->backupPalette[i] & 0x7E0 ) >> 5 );
      rangeB = UINT8_MAX - ( screen->backupPalette[i] & 0x1F );
      p = chain->totalElapsedSeconds / chain->totalDuration;
      increment = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      screen->palette[i] = screen->backupPalette[i] + increment;
   }
}

internal void AnimationChain_Tic_RainbowBridge_WhiteOut( AnimationChain_t* chain )
{
   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )
}

internal void AnimationChain_Tic_RainbowBridge_FadeIn( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG, increment;
   float p;
   Screen_t* screen = chain->screen;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = 0x1F - ( screen->backupPalette[i] >> 11 );
      rangeG = 0x3F - ( ( screen->backupPalette[i] & 0x7E0 ) >> 5 );
      rangeB = 0x1F - ( screen->backupPalette[i] & 0x1F );
      p = 1.0f - ( chain->totalElapsedSeconds / chain->totalDuration );
      increment = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      screen->palette[i] = screen->backupPalette[i] + increment;
   }
}

internal void AnimationChain_Tic_Flash( AnimationChain_t* chain )
{
   local_persist Bool_t wipeScreen = True;

   chain->totalElapsedSeconds += CLOCK_FRAME_SECONDS;
   chain->frameElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( chain->totalElapsedSeconds > chain->totalDuration )
   {
      AnimationChain_AnimationFinished( chain );
      chain->screen->needsWipe = False;
      wipeScreen = True;
   }
   else if ( chain->frameElapsedSeconds > chain->frameDuration )
   {
      while ( chain->frameElapsedSeconds > chain->frameDuration )
      {
         chain->frameElapsedSeconds -= chain->frameDuration;
         TOGGLE_BOOL( wipeScreen );
         chain->screen->needsWipe = wipeScreen;
      }
   }
}

internal void AnimationChain_Tic_Battle_Checkerboard( AnimationChain_t* chain )
{
   chain->frameElapsedSeconds += CLOCK_FRAME_SECONDS;
   int16_t xOffset = chain->tileMap->isDark ? -24 : 0;
   int16_t yOffset = chain->tileMap->isDark ? 4 : 0;
   uint32_t squaresToDraw;

   while ( chain->frameElapsedSeconds > ANIMATION_BATTLE_CHECKERSQUARE_DURATION )
   {
      squaresToDraw = (uint32_t)( chain->totalElapsedSeconds / ANIMATION_BATTLE_CHECKERSQUARE_DURATION ) + 1;

      while ( g_squaresDrawn <= squaresToDraw )
      {
         Screen_DrawRectColor( chain->screen,
                               (uint16_t)( (int16_t)( g_battleCheckerboardPos[g_squaresDrawn].x ) + xOffset ),
                               (uint16_t)( (int16_t)( g_battleCheckerboardPos[g_squaresDrawn].y ) + yOffset ),
                               TILE_SIZE, TILE_SIZE, COLOR_BLACK );
         g_squaresDrawn++;

         if ( g_squaresDrawn > 48 )
         {
            break;
         }
      }

      if ( g_squaresDrawn > 48 )
      {
         AnimationChain_AnimationFinished( chain );
         break;
      }
      else
      {
         chain->totalElapsedSeconds += ANIMATION_BATTLE_CHECKERSQUARE_DURATION;
         chain->frameElapsedSeconds -= ANIMATION_BATTLE_CHECKERSQUARE_DURATION;
      }
   }
}

internal void AnimationChain_Tic_Battle_EnemyFadeIn( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;
   Screen_t* screen = chain->screen;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = screen->backupPalette[i] >> 11;
      rangeG = ( screen->backupPalette[i] & 0x7E0 ) >> 5;
      rangeB = screen->backupPalette[i] & 0x1F;
      p = chain->totalElapsedSeconds / chain->totalDuration;
      screen->palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
   }
}

internal void AnimationChain_Tic_Battle_EnemyFadeOut( AnimationChain_t* chain )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      rangeR = chain->screen->backupPalette[i] >> 11;
      rangeG = ( chain->screen->backupPalette[i] & 0x7E0 ) >> 5;
      rangeB = chain->screen->backupPalette[i] & 0x1F;
      p = 1.0f - chain->totalElapsedSeconds / chain->totalDuration;
      chain->screen->palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
   }
}

internal void AnimationChain_Tic_Battle_EnemyDamage( AnimationChain_t* chain )
{
   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   chain->frameElapsedSeconds += CLOCK_FRAME_SECONDS;

   while ( chain->frameElapsedSeconds > ANIMATION_BATTLE_ENEMYDAMAGE_FRAMEDURATION )
   {
      chain->frameElapsedSeconds -= ANIMATION_BATTLE_ENEMYDAMAGE_FRAMEDURATION;

      if ( chain->flag )
      {
         Game_DrawEnemy( chain->game );
      }
      else
      {
         Game_WipeEnemy( chain->game );
      }

      TOGGLE_BOOL( chain->flag );
   }
}

internal void AnimationChain_Tic_Battle_PlayerDamage( AnimationChain_t* chain )
{
   ANIMATIONCHAIN_CHECK_ANIMATIONFINISHED( chain )

   chain->frameElapsedSeconds += CLOCK_FRAME_SECONDS;

   while ( chain->frameElapsedSeconds > ANIMATION_BATTLE_PLAYERDAMAGE_FRAMEDURATION )
   {
      chain->frameElapsedSeconds -= ANIMATION_BATTLE_PLAYERDAMAGE_FRAMEDURATION;

      if ( chain->flag )
      {
         Game_DrawQuickStatus( chain->game );
         Dialog_Draw( &( chain->game->dialog ) );
      }
      else
      {
         if ( chain->game->battle.enemy.id == ENEMY_DRAGONLORDDRAGON_ID )
         {
            Screen_WipeColor( &( chain->game->screen ), COLOR_BLACK );
         }
         else
         {
            Game_DrawTileMap( chain->game );
            Game_WipeEnemy( chain->game );
         }

         Game_DrawEnemy( chain->game );
      }

      TOGGLE_BOOL( chain->flag );
   }
}
