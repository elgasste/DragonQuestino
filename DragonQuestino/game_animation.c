#include "game.h"

internal void Game_TicAnimation_OverworldWash( Game_t* game );

void Game_StartAnimation( Game_t* game, Animation_t animation )
{
   game->isAnimating = True;
   game->animationSeconds = 0.0f;

   switch ( animation )
   {
      case Animation_Overworld_Wash: game->animationDuration = ANIMATION_OVERWORLD_WASH_DURATION; break;
   }
}

void Game_StopAnimation( Game_t* game )
{
   game->isAnimating = False;
}

void Game_TicAnimation( Game_t* game )
{
   switch ( game->animation )
   {
      case Animation_Overworld_Wash:
         Game_TicAnimation_OverworldWash( game );
         break;
   }
}

internal void Game_TicAnimation_OverworldWash( Game_t* game )
{
   game->animationSeconds += CLOCK_FRAME_SECONDS;

   if ( game->animationSeconds > ANIMATION_OVERWORLD_WASH_DURATION )
   {
      Game_StopAnimation( game );
      Game_ChangeState( game, GameState_Overworld );
   }
}
