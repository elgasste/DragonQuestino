#include "game.h"

void Game_RunNpcDialog( Game_t* game, uint32_t npcId )
{
   char msg[128];
   char password[32];
   char passwordMsg[34];

   Dialog_Reset( &( game->dialog ) );

   switch ( npcId )
   {
      case 0: // King Lorik
         sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_1, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_3 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_4 );
         Game_GetPassword( game, password );
         sprintf( passwordMsg, STRING_NPC_TANTEGEL_THRONEROOM_KING_5, password );
         Dialog_PushSection( &( game->dialog ), passwordMsg );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_6 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_7 );
         break;
      case 1: // Gwaelin
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_PRINCESS_1 );
         sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_PRINCESS_2, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         break;
      case 2: // Tantegel throne room left soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_LEFTSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_LEFTSOLDIER_2 );
         break;
      case 3: // Tantegel throne room right soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_RIGHTSOLDIER );
         break;
      case 4: // Tantegel throne room moving solder
         sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_MOVINGSOLDIER, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         break;
      default:
         Dialog_PushSection( &( game->dialog ), "No response. How rude." );
         break;
   }

   Game_OpenDialog( game );
}
