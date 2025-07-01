#include "game.h"

void Game_RunNpcDialog( Game_t* game, uint32_t npcId )
{
   Dialog_Reset( &( game->dialog ) );

   switch ( npcId )
   {
      case 0: // King Lorik
         Dialog_PushSection( &( game->dialog ), "I'm the king, yo." );
         Dialog_PushSection( &( game->dialog ), "..." );
         Dialog_PushSection( &( game->dialog ), "Could you go away please?" );
         break;
      case 1: // Gwaelin
         Dialog_PushSection( &( game->dialog ), "My dad's kind of a dick." );
         break;
      case 2: // Tantegel throne room left guard
         Dialog_PushSection( &( game->dialog ), "I know I should say something useful, but I'm not ready yet." );
         Dialog_PushSection( &( game->dialog ), "Come back later when you've got the final dialogue." );
         break;
      case 3: // Tantegel throne room right guard
         Dialog_PushSection( &( game->dialog ), "I took this job to stab civilians, now I'm stuck on guard duty? How??" );
         break;
   }

   Game_OpenDialog( game );
}
