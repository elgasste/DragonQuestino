#include "game.h"

internal void Game_TicAnimation_Overworld_Pause( Game_t* game );
internal void Game_TicAnimation_TileMap_FadeOut( Game_t* game );
internal void Game_TicAnimation_TileMap_FadePause( Game_t* game );
internal void Game_TicAnimation_TileMap_FadeIn( Game_t* game );
internal void Game_TicAnimation_TileMap_WhiteOut( Game_t* game );
internal void Game_TicAnimation_TileMap_WhitePause( Game_t* game );
internal void Game_TicAnimation_TileMap_WhiteIn( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_Trippy( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_WhiteOut( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_FadeIn( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_Pause( Game_t* game );

void Game_StartAnimation( Game_t* game, Animation_t animation )
{
   game->isAnimating = True;
   game->animation = animation;
   game->animationSeconds = 0.0f;

   switch ( animation )
   {
      case AnimationId_Overworld_Pause:
         game->animationDuration = ANIMATION_OVERWORLD_PAUSE_DURATION;
         break;
      case AnimationId_TileMap_FadeOut:
         Screen_BackupPalette( &( game->screen ) );
         game->animationDuration = ANIMATION_TILEMAP_FADE_DURATION;
         break;
      case AnimationId_TileMap_FadePause:
         game->animationDuration = ANIMATION_TILEMAP_FADEPAUSE_DURATION;
         break;
      case AnimationId_TileMap_FadeIn:
         game->animationDuration = ANIMATION_TILEMAP_FADE_DURATION;
         break;
      case AnimationId_TileMap_WhiteOut:
         Screen_BackupPalette( &( game->screen ) );
         game->animationDuration = ANIMATION_TILEMAP_WHITE_DURATION;
         break;
      case AnimationId_TileMap_WhitePause:
         game->animationDuration = ANIMATION_TILEMAP_WHITEPAUSE_DURATION;
         break;
      case AnimationId_TileMap_WhiteIn:
         game->animationDuration = ANIMATION_TILEMAP_WHITE_DURATION;
         break;
      case AnimationId_RainbowBridge_Trippy:
         Screen_BackupPalette( &( game->screen ) );
         game->animationDuration = ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION;
         break;
      case AnimationId_RainbowBridge_WhiteOut:
         game->animationDuration = ANIMATION_RAINBOWBRIDGE_WHITEOUT_DURATION;
         break;
      case AnimationId_RainbowBridge_FadeIn:
         game->animationDuration = ANIMATION_RAINBOWBRIDGE_FADEIN_DURATION;
         break;
      case AnimationId_RainbowBridge_Pause:
         game->animationDuration = ANIMATION_RAINBOWBRIDGE_PAUSE_DURATION;
         break;
   }
}

void Game_StopAnimation( Game_t* game )
{
   game->isAnimating = False;

   switch ( game->animation )
   {
      case AnimationId_Overworld_Pause:
         Game_ChangeMainState( game, MainState_Overworld );
         break;
      case AnimationId_TileMap_FadeIn:
      case AnimationId_TileMap_WhiteIn:
      case AnimationId_RainbowBridge_FadeIn:
         Screen_RestorePalette( &( game->screen ) );
         break;
      case AnimationId_RainbowBridge_Pause:
         Game_ChangeMainState( game, MainState_Overworld );
         break;
   }
}

void Game_TicAnimation( Game_t* game )
{
   switch ( game->animation )
   {
      case AnimationId_Overworld_Pause: Game_TicAnimation_Overworld_Pause( game ); break;
      case AnimationId_TileMap_FadeOut: Game_TicAnimation_TileMap_FadeOut( game ); break;
      case AnimationId_TileMap_FadePause: Game_TicAnimation_TileMap_FadePause( game ); break;
      case AnimationId_TileMap_FadeIn: Game_TicAnimation_TileMap_FadeIn( game ); break;
      case AnimationId_TileMap_WhiteOut: Game_TicAnimation_TileMap_WhiteOut( game ); break;
      case AnimationId_TileMap_WhitePause: Game_TicAnimation_TileMap_WhitePause( game ); break;
      case AnimationId_TileMap_WhiteIn: Game_TicAnimation_TileMap_WhiteIn( game ); break;
      case AnimationId_RainbowBridge_Trippy: Game_TicAnimation_RainbowBridge_Trippy( game ); break;
      case AnimationId_RainbowBridge_WhiteOut: Game_TicAnimation_RainbowBridge_WhiteOut( game ); break;
      case AnimationId_RainbowBridge_FadeIn: Game_TicAnimation_RainbowBridge_FadeIn( game ); break;
      case AnimationId_RainbowBridge_Pause: Game_TicAnimation_RainbowBridge_Pause( game ); break;
   }
}

internal void Game_TicAnimation_Overworld_Pause( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > ANIMATION_OVERWORLD_PAUSE_DURATION )
   {
      Game_StopAnimation( game );

      if ( game->dialog.id == DialogId_Spell_CastEvac )
      {
         game->targetPortal = &( game->tileMap.evacPortal );
         Game_StartAnimation( game, AnimationId_TileMap_FadeOut );
      }
      else if ( game->dialog.id == DialogId_Use_Wing ||
                game->dialog.id == DialogId_Spell_CastZoom )
      {
         Game_StartAnimation( game, AnimationId_TileMap_WhiteOut );
      }
      else if ( game->dialog.id == DialogId_Search_FoundHiddenStairs &&
                game->tileMap.id == TILEMAP_CHARLOCK_ID &&
                game->player.tileIndex == TILEMAP_HIDDENSTAIRS_INDEX )
      {
         Game_PlayerSteppedOnTile( game, TILEMAP_HIDDENSTAIRS_INDEX );
      }
      else
      {
         Game_ChangeMainState( game, MainState_Overworld );
      }

      game->dialog.id = DialogId_Count;
   }
}

internal void Game_TicAnimation_TileMap_FadeOut( Game_t* game )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > ANIMATION_TILEMAP_FADE_DURATION )
   {
      Game_EnterTargetPortal( game );
      Game_StopAnimation( game );
      Game_StartAnimation( game, AnimationId_TileMap_FadePause );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = game->screen.backupPalette[i] >> 11;
         rangeG = ( game->screen.backupPalette[i] & 0x7E0 ) >> 5;
         rangeB = game->screen.backupPalette[i] & 0x1F;
         p = 1.0f - ( game->animationSeconds / game->animationDuration );
         game->screen.palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      }
   }
}

internal void Game_TicAnimation_TileMap_FadePause( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > game->animationDuration )
   {
      Game_StartAnimation( game, AnimationId_TileMap_FadeIn );
   }
}

internal void Game_TicAnimation_TileMap_FadeIn( Game_t* game )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds <= ANIMATION_TILEMAP_FADE_DURATION )
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = game->screen.backupPalette[i] >> 11;
         rangeG = ( game->screen.backupPalette[i] & 0x7E0 ) >> 5;
         rangeB = game->screen.backupPalette[i] & 0x1F;
         p = game->animationSeconds / game->animationDuration;
         game->screen.palette[i] = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
      }
   }
   else
   {
      Game_StopAnimation( game );
   }
}

internal void Game_TicAnimation_TileMap_WhiteOut( Game_t* game )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > ANIMATION_TILEMAP_WHITE_DURATION )
   {
      Game_EnterTargetPortal( game );
      Game_StopAnimation( game );
      Game_StartAnimation( game, AnimationId_TileMap_WhitePause );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = 0x1F - ( game->screen.backupPalette[i] >> 11 );
         rangeG = 0x3F - ( ( game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = 0x1F - ( game->screen.backupPalette[i] & 0x1F );
         p = game->animationSeconds / game->animationDuration;
         game->screen.palette[i] = ( ( ( game->screen.backupPalette[i] >> 11 ) + ( uint16_t )( rangeR * p ) ) << 11 ) |
                                   ( ( ( ( game->screen.backupPalette[i] >> 5 ) & 0x3F ) + ( uint16_t )( rangeG * p ) ) << 5 ) |
                                   ( ( ( ( game->screen.backupPalette[i] ) & 0x1F ) + ( uint16_t )( rangeB * p ) ) );
      }
   }
}

internal void Game_TicAnimation_TileMap_WhitePause( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > game->animationDuration )
   {
      Game_StartAnimation( game, AnimationId_TileMap_WhiteIn );
   }
}

internal void Game_TicAnimation_TileMap_WhiteIn( Game_t* game )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG;
   float p;

   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds <= ANIMATION_TILEMAP_FADE_DURATION )
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = 0x1F - ( game->screen.backupPalette[i] >> 11 );
         rangeG = 0x3F - ( ( game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = 0x1F - ( game->screen.backupPalette[i] & 0x1F );
         p = 1.0f - ( game->animationSeconds / game->animationDuration );
         game->screen.palette[i] = ( ( ( game->screen.backupPalette[i] >> 11 ) + ( uint16_t )( rangeR * p ) ) << 11 ) |
                                   ( ( ( ( game->screen.backupPalette[i] >> 5 ) & 0x3F ) + ( uint16_t )( rangeG * p ) ) << 5 ) |
                                   ( ( ( ( game->screen.backupPalette[i] ) & 0x1F ) + ( uint16_t )( rangeB * p ) ) );
      }
   }
   else
   {
      Game_StopAnimation( game );
   }
}

internal void Game_TicAnimation_RainbowBridge_Trippy( Game_t* game )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG, increment;
   float p;

   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > game->animationDuration )
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         game->screen.palette[i] = COLOR_WHITE;
      }

      Screen_WipeColor( &( game->screen ), COLOR_WHITE );
      ITEM_TOGGLE_HASRAINBOWDROP( game->player.items );
      game->gameFlags.usedRainbowDrop = True;
      TILE_SET_TEXTUREINDEX( game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX], 13 );
      TILE_TOGGLE_PASSABLE( game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX] );
      Game_StartAnimation( game, AnimationId_RainbowBridge_WhiteOut );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = UINT8_MAX - ( game->screen.backupPalette[i] >> 11 );
         rangeG = UINT8_MAX - ( ( game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = UINT8_MAX - ( game->screen.backupPalette[i] & 0x1F );
         p = game->animationSeconds / game->animationDuration;
         increment = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
         game->screen.palette[i] = game->screen.backupPalette[i] + increment;
      }
   }
}

internal void Game_TicAnimation_RainbowBridge_WhiteOut( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > game->animationDuration )
   {
      Game_StartAnimation( game, AnimationId_RainbowBridge_FadeIn );
   }
}

internal void Game_TicAnimation_RainbowBridge_FadeIn( Game_t* game )
{
   uint32_t i;
   uint16_t rangeR, rangeB, rangeG, increment;
   float p;

   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > game->animationDuration )
   {
      Game_StartAnimation( game, AnimationId_RainbowBridge_Pause );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = 0x1F - ( game->screen.backupPalette[i] >> 11 );
         rangeG = 0x3F - ( ( game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = 0x1F - ( game->screen.backupPalette[i] & 0x1F );
         p = 1.0f - ( game->animationSeconds / game->animationDuration );
         increment = ( (uint16_t)( rangeR * p ) << 11 ) | ( (uint16_t)( rangeG * p ) << 5 ) | (uint16_t)( rangeB * p );
         game->screen.palette[i] = game->screen.backupPalette[i] + increment;
      }
   }
}

internal void Game_TicAnimation_RainbowBridge_Pause( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > game->animationDuration )
   {
      Game_StopAnimation( game );
   }
}
