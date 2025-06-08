#include "animation.h"
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

internal void Animation_Stop( Animation_t* animation );
internal void Animation_Tic_Overworld_Pause( Animation_t* animation );
internal void Animation_Tic_TileMap_FadeOut( Animation_t* animation );
internal void Animation_Tic_TileMap_FadePause( Animation_t* animation );
internal void Animation_Tic_TileMap_FadeIn( Animation_t* animation );
internal void Animation_Tic_TileMap_WhiteOut( Animation_t* animation );
internal void Animation_Tic_TileMap_WhitePause( Animation_t* animation );
internal void Animation_Tic_TileMap_WhiteIn( Animation_t* animation );
internal void Animation_Tic_CastSpell( Animation_t* animation );
internal void Animation_Tic_RainbowBridge_Trippy( Animation_t* animation );
internal void Animation_Tic_RainbowBridge_WhiteOut( Animation_t* animation );
internal void Animation_Tic_RainbowBridge_FadeIn( Animation_t* animation );
internal void Animation_Tic_RainbowBridge_Pause( Animation_t* animation );
internal void Animation_Tic_Battle_Checkerboard( Animation_t* animation );
internal void Animation_Tic_Battle_EnemyFadeIn( Animation_t* animation );
internal void Animation_Tic_Battle_EnemyFadeOut( Animation_t* animation );
internal void Animation_Tic_Battle_EnemyFadeInPause( Animation_t* animation );
internal void Animation_Tic_Battle_EnemyDamage( Animation_t* animation );
internal void Animation_Tic_Battle_EnemyDodge( Animation_t* animation );
internal void Animation_Tic_Battle_VictoryPause( Animation_t* animation );

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

void Animation_Init( Animation_t* animation, Game_t* game )
{
   animation->game = game;
}

void Animation_Start( Animation_t* animation, AnimationId_t id )
{
   animation->id = id;
   
   switch ( id )
   {
      case AnimationId_Pause:
         animation->totalDuration = ANIMATION_OVERWORLD_PAUSE_DURATION;
         break;
      case AnimationId_FadeOut:
         Screen_BackupPalette( &( animation->game->screen ) );
         animation->totalDuration = ANIMATION_TILEMAP_FADE_DURATION;
         break;
      case AnimationId_FadeIn:
         animation->totalDuration = ANIMATION_TILEMAP_FADE_DURATION;
         break;
      case AnimationId_WhiteOut:
         Screen_BackupPalette( &( animation->game->screen ) );
         animation->totalDuration = ANIMATION_TILEMAP_WHITE_DURATION;
         break;
      case AnimationId_WhitePause:
         animation->totalDuration = ANIMATION_TILEMAP_WHITEPAUSE_DURATION;
         break;
      case AnimationId_WhiteIn:
         animation->totalDuration = ANIMATION_TILEMAP_WHITE_DURATION;
         break;
      case AnimationId_CastSpell:
         Dialog2_Draw( &( animation->game->dialog2 ) );
         animation->game->screen.wipeColor = COLOR_WHITE;
         animation->game->screen.needsWipe = True;
         animation->totalDuration = ANIMATION_CASTSPELL_TOTALDURATION;
         break;
      case AnimationId_RainbowBridge_Trippy:
         Screen_BackupPalette( &( animation->game->screen ) );
         animation->totalDuration = ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION;
         break;
      case AnimationId_RainbowBridge_WhiteOut:
         animation->totalDuration = ANIMATION_RAINBOWBRIDGE_WHITEOUT_DURATION;
         break;
      case AnimationId_RainbowBridge_FadeIn:
         animation->totalDuration = ANIMATION_RAINBOWBRIDGE_FADEIN_DURATION;
         break;
      case AnimationId_RainbowBridge_Pause:
         animation->totalDuration = ANIMATION_RAINBOWBRIDGE_PAUSE_DURATION;
         break;
      case AnimationId_Battle_EnemyFadeIn:
         Screen_BackupPalette( &( animation->game->screen ) );
         Screen_ClearPalette( &( animation->game->screen ), COLOR_BLACK );
         animation->totalDuration = ANIMATION_BATTLE_ENEMYFADEIN_DURATION;
         break;
      case AnimationId_Battle_EnemyFadeOut:
         Screen_BackupPalette( &( animation->game->screen ) );
         animation->totalDuration = ANIMATION_BATTLE_ENEMYFADEOUT_DURATION;
         break;
      case AnimationId_Battle_EnemyFadeInPause:
         Dialog2_Draw( &( animation->game->dialog2 ) );
         animation->totalDuration = ANIMATION_BATTLE_ENEMYFADEINPAUSE_DURATION;
         break;
      case AnimationId_Battle_EnemyDamage:
         animation->game->dialog.showCarat = False;
         Dialog2_Draw( &( animation->game->dialog2 ) );
         animation->totalDuration = ANIMATION_BATTLE_ENEMYDAMAGE_DURATION;
         animation->flag = False;
         break;
      case AnimationId_Battle_EnemyDodge:
         animation->game->dialog.showCarat = False;
         Dialog2_Draw( &( animation->game->dialog2 ) );
         animation->totalDuration = ANIMATION_BATTLE_ENEMYDODGE_DURATION;
         break;
      case AnimationId_Battle_VictoryPause:
         Dialog2_Draw( &( animation->game->dialog2 ) );
         animation->totalDuration = ANIMATION_BATTLE_VICTORYPAUSE_DURATION;
         break;
   }

   animation->totalElapsedSeconds = 0.0f;
   animation->frameElapsedSeconds = 0.0f;
}

internal void Animation_Stop( Animation_t* animation )
{
   char enemyName[32];

   switch ( animation->id )
   {
      case AnimationId_Pause:
         Game_ChangeToOverworldState( animation->game );
         break;
      case AnimationId_FadeIn:
      case AnimationId_WhiteIn:
      case AnimationId_RainbowBridge_FadeIn:
         Screen_RestorePalette( &( animation->game->screen ) );
         break;
      case AnimationId_RainbowBridge_Pause:
         Game_ChangeToOverworldState( animation->game );
         break;
      case AnimationId_Battle_Checkerboard:
         Game_WipeEnemy( animation->game );
         Animation_Start( animation, AnimationId_Battle_EnemyFadeIn );
         break;
      case AnimationId_Battle_EnemyFadeIn:
         Screen_RestorePalette( &( animation->game->screen ) );
         animation->game->screen.needsRedraw = True;
         sprintf( enemyName, "%s %s", animation->game->battle.enemy.indefiniteArticle, animation->game->battle.enemy.name );
         Dialog_SetInsertionText( &( animation->game->dialog ), enemyName );
         Game_OpenDialog( animation->game, DialogId_Battle_EnemyApproaches );
         break;
      case AnimationId_Battle_EnemyFadeOut:
         Screen_RestorePalette( &( animation->game->screen ) );
         Game_WipeEnemy( animation->game );
         if ( animation->game->battle.enemy.stats.hitPoints == 0 )
         {
            Battle_Victory( &( animation->game->battle ) );
         }
         break;
      case AnimationId_Battle_EnemyFadeInPause:
         Game_OpenMenu( animation->game, MenuId_Battle );
         Game_DrawQuickStatus( animation->game );
         break;
      case AnimationId_Battle_EnemyDamage:
         Game_DrawEnemy( animation-> game );
         Dialog2_NextSection( &( animation->game->dialog2 ) );
         break;
      case AnimationId_Battle_EnemyDodge:
         Dialog2_NextSection( &( animation->game->dialog2 ) );
         break;
      case AnimationId_Battle_VictoryPause:
         Animation_Start( animation, AnimationId_Battle_EnemyFadeOut );
         break;
      case AnimationId_CastSpell:
         switch ( animation->game->dialog.id )
         {
            case DialogId_Battle_Spell_Sizz: Game_ApplySizz( animation->game ); break;
            case DialogId_Battle_Spell_Sizzle: Game_ApplySizzle( animation->game ); break;
         }
   }
}

internal void Animation_Tic_Overworld_Pause( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > ANIMATION_OVERWORLD_PAUSE_DURATION )
   {
      Animation_Stop( animation );

      if ( animation->game->dialog.id == DialogId_Spell_CastEvac )
      {
         animation->game->targetPortal = &( animation->game->tileMap.evacPortal );
         Animation_Start( animation, AnimationId_FadeOut );
      }
      else if ( animation->game->dialog.id == DialogId_Spell_CastZoom )
      {
         Animation_Start( animation, AnimationId_WhiteOut );
      }
      else
      {
         Game_ChangeToOverworldState( animation->game );
      }

      animation->game->dialog.id = DialogId_Count;
   }
}

internal void Animation_Tic_TileMap_FadeOut( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > ANIMATION_TILEMAP_FADE_DURATION )
   {
      Game_EnterTargetPortal( animation->game );
      Animation_Stop( animation );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = animation->game->screen.backupPalette[i] >> 11;
         rangeG = ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5;
         rangeB = animation->game->screen.backupPalette[i] & 0x1F;
         p = 1.0f - ( animation->totalElapsedSeconds / animation->totalDuration );
         animation->game->screen.palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      }
   }
}

internal void Animation_Tic_TileMap_FadePause( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Start( animation, AnimationId_FadeIn );
   }
}

internal void Animation_Tic_TileMap_FadeIn( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds <= ANIMATION_TILEMAP_FADE_DURATION )
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = animation->game->screen.backupPalette[i] >> 11;
         rangeG = ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5;
         rangeB = animation->game->screen.backupPalette[i] & 0x1F;
         p = animation->totalElapsedSeconds / animation->totalDuration;
         animation->game->screen.palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      }
   }
   else
   {
      Animation_Stop( animation );
   }
}

internal void Animation_Tic_TileMap_WhiteOut( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > ANIMATION_TILEMAP_WHITE_DURATION )
   {
      Game_EnterTargetPortal( animation->game );
      Animation_Stop( animation );
      Animation_Start( animation, AnimationId_WhitePause );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = 0x1F - ( animation->game->screen.backupPalette[i] >> 11 );
         rangeG = 0x3F - ( ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = 0x1F - ( animation->game->screen.backupPalette[i] & 0x1F );
         p = animation->totalElapsedSeconds / animation->totalDuration;
         animation->game->screen.palette[i] = ( ( ( animation->game->screen.backupPalette[i] >> 11 ) + ( uint16_t )( rangeR * p ) ) << 11 ) |
            ( ( ( ( animation->game->screen.backupPalette[i] >> 5 ) & 0x3F ) + ( uint16_t )( rangeG * p ) ) << 5 ) |
            ( ( ( ( animation->game->screen.backupPalette[i] ) & 0x1F ) + ( uint16_t )( rangeB * p ) ) );
      }
   }
}

internal void Animation_Tic_TileMap_WhitePause( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Start( animation, AnimationId_WhiteIn );
   }
}

internal void Animation_Tic_TileMap_WhiteIn( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds <= ANIMATION_TILEMAP_FADE_DURATION )
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = 0x1F - ( animation->game->screen.backupPalette[i] >> 11 );
         rangeG = 0x3F - ( ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = 0x1F - ( animation->game->screen.backupPalette[i] & 0x1F );
         p = 1.0f - ( animation->totalElapsedSeconds / animation->totalDuration );
         animation->game->screen.palette[i] = ( ( ( animation->game->screen.backupPalette[i] >> 11 ) + ( uint16_t )( rangeR * p ) ) << 11 ) |
            ( ( ( ( animation->game->screen.backupPalette[i] >> 5 ) & 0x3F ) + ( uint16_t )( rangeG * p ) ) << 5 ) |
            ( ( ( ( animation->game->screen.backupPalette[i] ) & 0x1F ) + ( uint16_t )( rangeB * p ) ) );
      }
   }
   else
   {
      Animation_Stop( animation );
   }
}

internal void Animation_Tic_CastSpell( Animation_t* animation )
{
   local_persist Bool_t wipeScreen = True;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;
   animation->frameElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
      animation->game->screen.needsWipe = False;
      wipeScreen = True;
   }
   else if ( animation->frameElapsedSeconds > ANIMATION_CASTSPELL_FRAMEDURATION )
   {
      while ( animation->frameElapsedSeconds > ANIMATION_CASTSPELL_FRAMEDURATION )
      {
         animation->frameElapsedSeconds -= ANIMATION_CASTSPELL_FRAMEDURATION;
         TOGGLE_BOOL( wipeScreen );
         animation->game->screen.needsWipe = wipeScreen;
      }
   }
}

internal void Animation_Tic_RainbowBridge_Trippy( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG, increment;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         animation->game->screen.palette[i] = COLOR_WHITE;
      }

      Screen_WipeColor( &( animation->game->screen ), COLOR_WHITE );
      ITEM_TOGGLE_HASRAINBOWDROP( animation->game->player.items );
      animation->game->gameFlags.usedRainbowDrop = True;
      TILE_SET_TEXTUREINDEX( animation->game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX], 13 );
      TILE_TOGGLE_PASSABLE( animation->game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX] );
      Animation_Start( animation, AnimationId_RainbowBridge_WhiteOut );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = UINT8_MAX - ( animation->game->screen.backupPalette[i] >> 11 );
         rangeG = UINT8_MAX - ( ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = UINT8_MAX - ( animation->game->screen.backupPalette[i] & 0x1F );
         p = animation->totalElapsedSeconds / animation->totalDuration;
         increment = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
         animation->game->screen.palette[i] = animation->game->screen.backupPalette[i] + increment;
      }
   }
}

internal void Animation_Tic_RainbowBridge_WhiteOut( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Start( animation, AnimationId_RainbowBridge_FadeIn );
   }
}

internal void Animation_Tic_RainbowBridge_FadeIn( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG, increment;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Start( animation, AnimationId_RainbowBridge_Pause );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = 0x1F - ( animation->game->screen.backupPalette[i] >> 11 );
         rangeG = 0x3F - ( ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = 0x1F - ( animation->game->screen.backupPalette[i] & 0x1F );
         p = 1.0f - ( animation->totalElapsedSeconds / animation->totalDuration );
         increment = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
         animation->game->screen.palette[i] = animation->game->screen.backupPalette[i] + increment;
      }
   }
}

internal void Animation_Tic_RainbowBridge_Pause( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
   }
}

internal void Animation_Tic_Battle_Checkerboard( Animation_t* animation )
{
   animation->frameElapsedSeconds += CLOCK_FRAME_SECONDS;
   int16_t xOffset = animation->game->tileMap.isDark ? -24 : 0;
   int16_t yOffset = animation->game->tileMap.isDark ? 4 : 0;

   while ( animation->frameElapsedSeconds > ANIMATION_BATTLE_CHECKERSQUARE_DURATION )
   {
      uint32_t squareIndex = (uint32_t)( animation->totalElapsedSeconds / ANIMATION_BATTLE_CHECKERSQUARE_DURATION );

      Screen_DrawRectColor( &( animation->game->screen ),
                            (uint16_t)( (int16_t)( g_battleCheckerboardPos[squareIndex].x ) + xOffset ),
                            (uint16_t)( (int16_t)( g_battleCheckerboardPos[squareIndex].y ) + yOffset ),
                            TILE_SIZE, TILE_SIZE, COLOR_BLACK );

      if ( squareIndex == 48 )
      {
         Animation_Stop( animation );
         break;
      }
      else
      {
         animation->totalElapsedSeconds += ANIMATION_BATTLE_CHECKERSQUARE_DURATION;
         animation->frameElapsedSeconds -= ANIMATION_BATTLE_CHECKERSQUARE_DURATION;
      }
   }
}

internal void Animation_Tic_Battle_EnemyFadeIn( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = animation->game->screen.backupPalette[i] >> 11;
         rangeG = ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5;
         rangeB = animation->game->screen.backupPalette[i] & 0x1F;
         p = animation->totalElapsedSeconds / animation->totalDuration;
         animation->game->screen.palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      }
   }
}

internal void Animation_Tic_Battle_EnemyFadeOut( Animation_t* animation )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = animation->game->screen.backupPalette[i] >> 11;
         rangeG = ( animation->game->screen.backupPalette[i] & 0x7E0 ) >> 5;
         rangeB = animation->game->screen.backupPalette[i] & 0x1F;
         p = 1.0f - animation->totalElapsedSeconds / animation->totalDuration;
         animation->game->screen.palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      }
   }
}

internal void Animation_Tic_Battle_EnemyFadeInPause( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
   }
}

internal void Animation_Tic_Battle_EnemyDamage( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
   }
   else
   {
      animation->frameElapsedSeconds += CLOCK_FRAME_SECONDS;

      while ( animation->frameElapsedSeconds > ANIMATION_BATTLE_ENEMYDAMAGE_FRAMEDURATION )
      {
         animation->frameElapsedSeconds -= ANIMATION_BATTLE_ENEMYDAMAGE_FRAMEDURATION;

         if ( animation->flag )
         {
            Game_DrawEnemy( animation->game );
         }
         else
         {
            Game_WipeEnemy( animation->game );
         }

         TOGGLE_BOOL( animation->flag );
      }
   }
}

internal void Animation_Tic_Battle_EnemyDodge( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
   }
}

internal void Animation_Tic_Battle_VictoryPause( Animation_t* animation )
{
   animation->totalElapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->totalElapsedSeconds > animation->totalDuration )
   {
      Animation_Stop( animation );
   }
}

void AnimationChain_Init( AnimationChain_t* chain, Screen_t* screen )
{
   chain->screen = screen;
}

void AnimationChain_Reset( AnimationChain_t* chain )
{
   chain->animationCount = 0;
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
   AnimationChain_StartAnimation( chain );
}

void AnimationChain_Tic( AnimationChain_t* chain )
{
   switch ( chain->animationIds[chain->activeAnimation] )
   {
      case AnimationId_Pause: AnimationChain_Tic_Pause( chain ); break;
      case AnimationId_WhiteOut: AnimationChain_Tic_WhiteOut( chain ); break;
      case AnimationId_WhiteIn: AnimationChain_Tic_WhiteIn( chain ); break;
      case AnimationId_FadeOut: AnimationChain_Tic_FadeOut( chain ); break;
      case AnimationId_FadeIn: AnimationChain_Tic_FadeIn( chain ); break;
      case AnimationId_RainbowBridge_Trippy: AnimationChain_Tic_RainbowBridge_Trippy( chain ); break;
      case AnimationId_RainbowBridge_WhiteOut: AnimationChain_Tic_RainbowBridge_WhiteOut( chain ); break;
      case AnimationId_RainbowBridge_FadeIn: AnimationChain_Tic_RainbowBridge_FadeIn( chain ); break;
   }
}

AnimationId_t AnimationChain_GetActiveAnimationId( AnimationChain_t* chain )
{
   return chain->animationIds[chain->activeAnimation];
}

internal void AnimationChain_StartAnimation( AnimationChain_t* chain )
{
   switch ( chain->animationIds[chain->activeAnimation] )
   {
      case AnimationId_Pause: chain->totalDuration = ANIMATIONCHAIN_PAUSE_DURATION; break;
      case AnimationId_WhiteOut:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATIONCHAIN_WHITE_DURATION;
         break;
      case AnimationId_WhiteIn: chain->totalDuration = ANIMATIONCHAIN_WHITE_DURATION; break;
      case AnimationId_FadeOut:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATIONCHAIN_FADE_DURATION;
         break;
      case AnimationId_FadeIn: chain->totalDuration = ANIMATIONCHAIN_FADE_DURATION; break;
      case AnimationId_RainbowBridge_Trippy:
         Screen_BackupPalette( chain->screen );
         chain->totalDuration = ANIMATIONCHAIN_RAINBOWBRIDGE_TRIPPY_DURATION;
         break;
      case AnimationId_RainbowBridge_WhiteOut:
      case AnimationId_RainbowBridge_FadeIn:
         chain->totalDuration = ANIMATIONCHAIN_RAINBOWBRIDGE_FADE_DURATION;
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
      case AnimationId_WhiteIn:
      case AnimationId_RainbowBridge_FadeIn:
         Screen_RestorePalette( chain->screen );
         break;
   }

   if ( chain->callbacks[chain->activeAnimation] )
   {
      chain->callbacks[chain->activeAnimation]( chain->callbackDatas[chain->activeAnimation] );
   }

   chain->activeAnimation++;

   if ( chain->activeAnimation == chain->animationCount )
   {
      chain->isRunning = False;
   }
   else
   {
      AnimationChain_StartAnimation( chain );
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
