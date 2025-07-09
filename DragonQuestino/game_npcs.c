#include "game.h"

void Game_RunNpcDialog( Game_t* game, uint32_t npcId )
{
   char msg[32];
   char password[32];
   char passwordMsg[34];

   Dialog_Reset( &( game->dialog ) );

   switch ( npcId )
   {
      case 0: // King Lorik
         sprintf( msg, "\"Hello %s!\"", game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         Dialog_PushSection( &( game->dialog ), "..." );
         Dialog_PushSection( &( game->dialog ), "King Lorik suddenly convulses grotesquely..." );
         Dialog_PushSection( &( game->dialog ), "\"Your progress... remember...\"" );
         Game_GetPassword( game, password );
         sprintf( passwordMsg, "\"%s\"", password );
         Dialog_PushSection( &( game->dialog ), passwordMsg );
         Dialog_PushSection( &( game->dialog ), "..." );
         Dialog_PushSection( &( game->dialog ), "He snaps out of it and can't remember a thing. What did he mean??" );
         break;
      case 1: // Gwaelin
         Dialog_PushSection( &( game->dialog ), "\"My dad is getting older, lately he's been prone to rambling.\"" );
         Dialog_PushSection( &( game->dialog ), "\"Sometimes he just spouts gibberish, we don't know what to make of it.\"" );
         break;
      case 2: // Tantegel throne room left guard
         Dialog_PushSection( &( game->dialog ), "\"I know I should say something useful, but I'm not ready yet.\"" );
         Dialog_PushSection( &( game->dialog ), "\"Come back later when you've got the final dialogue.\"" );
         break;
      case 3: // Tantegel throne room right guard
         Dialog_PushSection( &( game->dialog ), "\"I took this job to stab civilians, now I'm stuck on guard duty? How??\"" );
         break;
      default:
         Dialog_PushSection( &( game->dialog ), "No response. How rude." );
         break;
   }

   Game_OpenDialog( game );
}
