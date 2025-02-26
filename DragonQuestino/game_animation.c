#include "game.h"

internal void Game_TicAnimation_Overworld_Wash( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_Trippy( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_Whiteout( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_FadeIn( Game_t* game );
internal void Game_TicAnimation_RainbowBridge_Pause( Game_t* game );

void Game_StartAnimation( Game_t* game, Animation_t animation )
{
   game->isAnimating = True;
   game->animation = animation;
   game->animationSeconds = 0.0f;

   switch ( animation )
   {
      case Animation_Overworld_Wash: game->animationDuration = ANIMATION_OVERWORLD_WASH_DURATION; break;
      case Animation_RainbowBridge_Trippy: game->animationDuration = ANIMATION_RAINBOWBRIDGE_TRIPPY_DURATION; break;
      case Animation_RainbowBridge_Whiteout: game->animationDuration = ANIMATION_RAINBOWBRIDGE_WHITEOUT_DURATION; break;
      case Animation_RainbowBridge_FadeIn: game->animationDuration = ANIMATION_RAINBOWBRIDGE_FADEIN_DURATION; break;
      case Animation_RainbowBridge_Pause: game->animationDuration = ANIMATION_RAINBOWBRIDGE_PAUSE_DURATION; break;
   }
}

void Game_StopAnimation( Game_t* game )
{
   game->isAnimating = False;

   switch ( game->animation )
   {
      case Animation_Overworld_Wash:
      case Animation_RainbowBridge_Pause:
         Game_ChangeState( game, GameState_Overworld );
         break;
   }
}

void Game_TicAnimation( Game_t* game )
{
   switch ( game->animation )
   {
      case Animation_Overworld_Wash: Game_TicAnimation_Overworld_Wash( game ); break;
      case Animation_RainbowBridge_Trippy: Game_TicAnimation_RainbowBridge_Trippy( game ); break;
      case Animation_RainbowBridge_Whiteout: Game_TicAnimation_RainbowBridge_Whiteout( game ); break;
      case Animation_RainbowBridge_FadeIn: Game_TicAnimation_RainbowBridge_FadeIn( game ); break;
      case Animation_RainbowBridge_Pause: Game_TicAnimation_RainbowBridge_Pause( game ); break;
   }
}

internal void Game_TicAnimation_Overworld_Wash( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > ANIMATION_OVERWORLD_WASH_DURATION )
   {
      Game_StopAnimation( game );
      Game_ChangeState( game, GameState_Overworld );
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
      game->tileMap.usedRainbowDrop = True;
      TILE_SET_TEXTUREINDEX( game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX], 13 );
      TILE_SET_PASSABLE( game->tileMap.tiles[TILEMAP_RAINBOWBRIDGE_INDEX], True );
      Game_StartAnimation( game, Animation_RainbowBridge_Whiteout );
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

internal void Game_TicAnimation_RainbowBridge_Whiteout( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > game->animationDuration )
   {
      Game_StartAnimation( game, Animation_RainbowBridge_FadeIn );
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
      Screen_RestorePalette( &( game->screen ) );
      Game_StartAnimation( game, Animation_RainbowBridge_Pause );
   }
   else
   {
      for ( i = 0; i < PALETTE_COLORS; i++ )
      {
         rangeR = 0x1F - ( game->screen.backupPalette[i] >> 11 );
         rangeG = 0x3F - ( ( game->screen.backupPalette[i] & 0x7E0 ) >> 5 );
         rangeB = 0x1F - ( game->screen.backupPalette[i] & 0x1F );
         p = 1.0f -  ( game->animationSeconds / game->animationDuration );
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
