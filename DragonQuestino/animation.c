#include "animation.h"
#include "game.h"
#include "vector.h"

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

internal Vector2u16_t g_battleCheckerboardPos[49] =
{
   { 144, 96  }, { 160, 96  }, { 160, 112 }, { 144, 112 }, { 128, 112 }, { 128, 96  }, { 128, 80  },
   { 144, 80  }, { 160, 80  }, { 176, 80  }, { 176, 96  }, { 176, 112 }, { 176, 128 }, { 160, 128 },
   { 144, 128 }, { 128, 128 }, { 112, 128 }, { 112, 112 }, { 112, 96  }, { 112, 80  }, { 112, 64  },
   { 128, 64  }, { 144, 64  }, { 160, 64  }, { 176, 64  }, { 192, 64  }, { 192, 80  }, { 192, 96  },
   { 192, 112 }, { 192, 128 }, { 192, 144 }, { 176, 144 }, { 160, 144 }, { 144, 144 }, { 128, 144 },
   { 112, 144 }, { 96,  144 }, { 96,  128 }, { 96,  112 }, { 96,  96  }, { 96,  80  }, { 96,  64  },
   { 96,  48  }, { 112, 48  }, { 128, 48  }, { 144, 48  }, { 160, 48  }, { 176, 48  }, { 192, 48  }
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
      case AnimationId_Overworld_Pause:
         animation->totalDuration = ANIMATION_OVERWORLD_PAUSE_DURATION;
         break;
      case AnimationId_TileMap_FadeOut:
         Screen_BackupPalette( &( animation->game->screen ) );
         animation->totalDuration = ANIMATION_TILEMAP_FADE_DURATION;
         break;
      case AnimationId_TileMap_FadePause:
         animation->totalDuration = ANIMATION_TILEMAP_FADEPAUSE_DURATION;
         break;
      case AnimationId_TileMap_FadeIn:
         animation->totalDuration = ANIMATION_TILEMAP_FADE_DURATION;
         break;
      case AnimationId_TileMap_WhiteOut:
         Screen_BackupPalette( &( animation->game->screen ) );
         animation->totalDuration = ANIMATION_TILEMAP_WHITE_DURATION;
         break;
      case AnimationId_TileMap_WhitePause:
         animation->totalDuration = ANIMATION_TILEMAP_WHITEPAUSE_DURATION;
         break;
      case AnimationId_TileMap_WhiteIn:
         animation->totalDuration = ANIMATION_TILEMAP_WHITE_DURATION;
         break;
      case AnimationId_CastSpell:
         Dialog_Draw( &( animation->game->dialog ) );
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
   }

   animation->totalElapsedSeconds = 0.0f;
   animation->frameElapsedSeconds = 0.0f;
   animation->isRunning = True;
}

void Animation_Tic( Animation_t* animation )
{
   switch ( animation->id )
   {
      case AnimationId_Overworld_Pause: Animation_Tic_Overworld_Pause( animation ); break;
      case AnimationId_TileMap_FadeOut: Animation_Tic_TileMap_FadeOut( animation ); break;
      case AnimationId_TileMap_FadePause: Animation_Tic_TileMap_FadePause( animation ); break;
      case AnimationId_TileMap_FadeIn: Animation_Tic_TileMap_FadeIn( animation ); break;
      case AnimationId_TileMap_WhiteOut: Animation_Tic_TileMap_WhiteOut( animation ); break;
      case AnimationId_TileMap_WhitePause: Animation_Tic_TileMap_WhitePause( animation ); break;
      case AnimationId_TileMap_WhiteIn: Animation_Tic_TileMap_WhiteIn( animation ); break;
      case AnimationId_CastSpell: Animation_Tic_CastSpell( animation ); break;
      case AnimationId_RainbowBridge_Trippy: Animation_Tic_RainbowBridge_Trippy( animation ); break;
      case AnimationId_RainbowBridge_WhiteOut: Animation_Tic_RainbowBridge_WhiteOut( animation ); break;
      case AnimationId_RainbowBridge_FadeIn: Animation_Tic_RainbowBridge_FadeIn( animation ); break;
      case AnimationId_RainbowBridge_Pause: Animation_Tic_RainbowBridge_Pause( animation ); break;
      case AnimationId_Battle_Checkerboard: Animation_Tic_Battle_Checkerboard( animation ); break;
      case AnimationId_Battle_EnemyFadeIn: Animation_Tic_Battle_EnemyFadeIn( animation ); break;
   }
}

internal void Animation_Stop( Animation_t* animation )
{
   int16_t xOffset, yOffset;

   animation->isRunning = False;

   switch ( animation->id )
   {
      case AnimationId_Overworld_Pause:
         Game_ChangeMainState( animation->game, MainState_Overworld );
         break;
      case AnimationId_TileMap_FadeIn:
      case AnimationId_TileMap_WhiteIn:
      case AnimationId_RainbowBridge_FadeIn:
         Screen_RestorePalette( &( animation->game->screen ) );
         break;
      case AnimationId_RainbowBridge_Pause:
         Game_ChangeMainState( animation->game, MainState_Overworld );
         break;
      case AnimationId_Battle_Checkerboard:
         xOffset = animation->game->tileMap.isDark ? -24 : 0;
         yOffset = animation->game->tileMap.isDark ? 8 : 0;
         Screen_DrawRectColor( &( animation->game->screen ), 96 + xOffset, 48 + yOffset, 96, 96, COLOR_BLACK );
         Animation_Start( animation, AnimationId_Battle_EnemyFadeIn );
         break;
      case AnimationId_Battle_EnemyFadeIn:
         Screen_RestorePalette( &( animation->game->screen ) );
         animation->game->screen.needsRedraw = True;
         break;
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
         Animation_Start( animation, AnimationId_TileMap_FadeOut );
      }
      else if ( animation->game->dialog.id == DialogId_Use_Wing ||
                animation->game->dialog.id == DialogId_Spell_CastZoom )
      {
         Animation_Start( animation, AnimationId_TileMap_WhiteOut );
      }
      else if ( animation->game->dialog.id == DialogId_Search_FoundHiddenStairs &&
                animation->game->tileMap.id == TILEMAP_CHARLOCK_ID &&
                animation->game->player.tileIndex == TILEMAP_HIDDENSTAIRS_INDEX )
      {
         Game_PlayerSteppedOnTile( animation->game, TILEMAP_HIDDENSTAIRS_INDEX );
      }
      else
      {
         Game_ChangeMainState( animation->game, MainState_Overworld );
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
      Animation_Start( animation, AnimationId_TileMap_FadePause );
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
      Animation_Start( animation, AnimationId_TileMap_FadeIn );
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
      Animation_Start( animation, AnimationId_TileMap_WhitePause );
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
      Animation_Start( animation, AnimationId_TileMap_WhiteIn );
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
   int16_t yOffset = animation->game->tileMap.isDark ? 8 : 0;

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
