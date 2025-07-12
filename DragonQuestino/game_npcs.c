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
      case 5: // Tantegel ground floor top soldier next to throne room stairs
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSUPPERSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSUPPERSOLDIER_2 );
         break;
      case 6: // Tantegel ground floor bottom soldier next to throne room stairs
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSLOWERSOLDIER );
         break;
      case 7: // Tantegel ground floor bottom right static soldier
         Dialog_PushSection( &( game->dialog ), game->gameFlags.rescuedPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_BOTTOMRIGHTSTATICSOLDIER_2 : STRING_NPC_TANTEGEL_GROUNDFLOOR_BOTTOMRIGHTSTATICSOLDIER_1 );
         break;
      case 8: // Tantegel ground floor entry left soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYLEFTSOLDIER );
         break;
      case 9: // Tantegel ground floor entry right soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYRIGHTSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYRIGHTSOLDIER_2 );
         break;
      case 10: // Tantegel ground floor upper-left static blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERLEFTSTATICBLUEMAN );
         break;
      case 11: // Tantegel ground floor upper-right static blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERRIGHTSTATICBLUEWOMAN_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERRIGHTSTATICBLUEWOMAN_2 );
         break;
      case 12: // Tantegel ground floor static barrier soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STATICBARRIERSOLDIER );
         break;
      case 13: // Tantegel ground floor blue man by the stairs
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSBLUEMAN );
         break;
      case 14: // Tantegel ground floor blue woman in the courtyard
         Dialog_PushSection( &( game->dialog ), game->gameFlags.rescuedPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_COURTYARDBLUEWOMAN_2 : STRING_NPC_TANTEGEL_GROUNDFLOOR_COURTYARDBLUEWOMAN_1 );
         break;
      case 15: // Tantegel ground floor green man on the right in the lower-left room
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANRIGHT_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANRIGHT_2 );
         break;
      case 16: // Tantegel ground floor green man on the left in the lower-left room
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANLEFT );
         break;
      case 17: // Tantegel ground floor treasure room soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_TREASUREROOMSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_TREASUREROOMSOLDIER_2 );
         break;
      case 18: // Tantegel ground floor upper area soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERSOLDIER );
         break;
      case 19: // Tantegel ground floor right area moving soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_RIGHTACTIVESOLDIER );
         break;
      case 20: // Tantegel ground floor moving wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ACTIVEWIZARD );
         break;
      case 21: // Tantegel ground floor soldier on the other side of the barrier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_PASTBARRIERSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_PASTBARRIERSOLDIER_2 );
         break;
      case 22: // Tantegel basement wizard
         Dialog_PushSection( &( game->dialog ), ( game->gameFlags.treasures & 0x80 ) ? STRING_NPC_TANTEGEL_BASEMENT_WIZARD_1 : STRING_NPC_TANTEGEL_BASEMENT_WIZARD_2 );
         break;
      case 23: // Brecconary west entrance blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_WESTENTRANCEBLUEMAN );
         break;
      case 24: // Brecconary weapon shop blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_WEAPONSHOPBLUEWOMAN );
         break;
      case 25: // Brecconary left room blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_LEFTROOMBLUEMAN );
         break;
      case 26: // Brecconary right room wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD );
         break;
      case 27: // Brecconary inn static soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_INNSTATICSOLDIER );
         break;
      case 28: // Brecconary lower-right area static blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_LOWERRIGHTSTATICBLUEMAN );
         break;
      case 29: // Brecconary upper-right soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_UPPERRIGHTSOLDIER );
         break;
      case 30: // Brecconary upper wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_UPPERWIZARD );
         break;
      case 31: // Brecconary upper green man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_UPPERGREENMAN );
         break;
      case 32: // Brecconary center blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_CENTERBLUEMAN );
         break;
      case 33: // Brecconary center red soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_CENTERREDSOLDIER_1 );
         sprintf( msg, STRING_NPC_BRECCONARY_CENTERREDSOLDIER_2, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         break;
      case 34: // Brecconary east of the inn blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_EASTOFINNBLUEWOMAN );
         break;
      case 35: // Brecconary right soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTSOLDIER );
         break;
      case 36: // Brecconary active blue man in the lower right
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_LOWERRIGHTACTIVEBLUEMAN_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_LOWERRIGHTACTIVEBLUEMAN_2 );
         break;
      case 37 : // Brecconary active soldier in the inn:
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_INNACTIVESOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_INNACTIVESOLDIER_2 );
         break;
      case 38: // Garinham outside lower-left wizard
         Dialog_PushSection( &( game->dialog ), game->gameFlags.rescuedPrincess ? STRING_NPC_GARINHAMOUTSIDE_LOWERLEFTWIZARD_2 : STRING_NPC_GARINHAMOUTSIDE_LOWERLEFTWIZARD_1 );
         break;
      case 39: // Garinham outside left wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_LEFTWIZARD );
         break;
      case 40: // Garinham outside soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_SOLDIER );
         break;
      case 41: // Garinham outside blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEWOMAN );
         break;
      case 42: // Garinham outside blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEMAN );
         break;
      case 43: // Garinham inside left guard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMINSIDE_LEFTGUARD );
         break;
      case 44: // Garinham inside right guard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMINSIDE_RIGHTGUARD );
         break;
      case 45: // Garinham inside blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMINSIDE_BLUEMAN );
         break;
      case 46: // Garinham inside green man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMINSIDE_GREENMAN );
         break;
      case 47: // Garinham inside wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMINSIDE_WIZARD_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMINSIDE_WIZARD_2 );
         break;
      case 48: // Garinham inside blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMINSIDE_BLUEWOMAN );
         break;
      default: // should never happen, but it's nice to have a catch-all
         Dialog_PushSection( &( game->dialog ), STRING_NPC_DEFAULT );
         break;
   }

   Game_OpenDialog( game );
}
