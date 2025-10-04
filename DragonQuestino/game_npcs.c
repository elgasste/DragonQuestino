#include "game.h"

internal void Game_StaffOfRainNpcCallback( Game_t* game );
internal void Game_RainbowDropNpcCallback( Game_t* game );
internal void Game_DragonlordChoicePresentationCallback( Game_t* game );
internal void Game_DragonlordJoinCallback( Game_t* game );
internal void Game_DragonlordJoinTransferCallback( Game_t* game );
internal void Game_DragonlordJoinedCallback( Game_t* game );
internal void Game_DragonlordJoinFadeCallback( Game_t* game );
internal void Game_DragonlordJoinPostFadeCallback( Game_t* game );
internal void Game_DragonlordRefuseCallback( Game_t* game );
internal void Game_DragonlordRefuseMessageCallback( Game_t* game );
internal void Game_DragonlordInitiateFightCallback( Game_t* game );
internal void Game_CurseLiftedCallback( Game_t* game );
internal void Game_CurseLiftedSpellCallback( Game_t* game );
internal void Game_RescuePrincessCallback( Game_t* game );
internal void Game_ReturnPrincessCallback( Game_t* game );
internal void Game_ReturnPrincessPreFadeCallback( Game_t* game );
internal void Game_ReturnPrincessFadeOutCallback( Game_t* game );
internal void Game_ReturnPrincessPostFadeCallback( Game_t* game );

void Game_RunNpcDialog( Game_t* game, u32 npcId )
{
   u16 experienceRemaining;
   char msg[128];
   char password[32];
   char passwordMsg[34];

   Dialog_Reset( &( game->dialog ) );

   switch ( npcId )
   {
      case 0: // King Lorik
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_1 );
            sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_2, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
            sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_3, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_4 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_5 );
            sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_6, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_7 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_8 );
            sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_9, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_10 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_11 );
            sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_THANKS_12, game->player.name );
            Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ReturnPrincessCallback, game );
         }
         else if ( game->gameFlags.leftThroneRoom )
         {
            sprintf( msg, game->gameFlags.rescuedPrincess ? STRING_NPC_TANTEGEL_THRONEROOM_KING_1_2 : STRING_NPC_TANTEGEL_THRONEROOM_KING_1_1, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
            experienceRemaining = Player_GetExperienceRemaining( &( game->player ) );
            if ( experienceRemaining > 0 )
            {
               sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_2, experienceRemaining,
                        experienceRemaining == 1 ? STRING_POINT : STRING_POINTS );
               Dialog_PushSection( &( game->dialog ), msg );
            }
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_3 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_4 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_5 );
            Game_GetPassword( game, password );
            sprintf( passwordMsg, STRING_NPC_TANTEGEL_THRONEROOM_KING_6, password );
            strcpy( game->lastPassword, password );
#if defined( VISUAL_STUDIO_DEV )
            Dialog_PushSectionWithCallback( &( game->dialog ), passwordMsg, Game_QuickSave, game );
#else
            Dialog_PushSection( &( game->dialog ), passwordMsg );
#endif
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_7 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_8 );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_2_1 );
         }
         break;
      case 1: // Gwaelin (throne room)
         Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_PRINCESS_1 );
         sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_PRINCESS_2, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         break;
      case 2: // Tantegel throne room left soldier
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_LEFTSOLDIER_PR );
         }
         else if ( game->gameFlags.leftThroneRoom )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_LEFTSOLDIER_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_LEFTSOLDIER_2 );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_LEFTSOLDIER_INTRO_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_LEFTSOLDIER_INTRO_2 );
         }
         break;
      case 3: // Tantegel throne room right soldier
         if ( game->gameFlags.leftThroneRoom )
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_THRONEROOM_RIGHTSOLDIER_PR : STRING_NPC_TANTEGEL_THRONEROOM_RIGHTSOLDIER );
         }
         else
         {
            if ( game->gameFlags.doors & 0x1 )
            {
               Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_RIGHTSOLDIER_INTRO_1_1 );
               Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_RIGHTSOLDIER_INTRO_1_2 );
            }
            else
            {
               Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_RIGHTSOLDIER_INTRO_2 );
            }
         }
         break;
      case 4: // Tantegel throne room moving solder
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_MOVINGSOLDIER_PR );
         }
         else if ( game->gameFlags.rescuedPrincess )
         {
            sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_MOVINGSOLDIER_APR, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_MOVINGSOLDIER_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_MOVINGSOLDIER_2 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_MOVINGSOLDIER_3 );
         }
         break;
      case 5: // Tantegel ground floor top soldier next to throne room stairs
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSUPPERSOLDIER_PR );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSUPPERSOLDIER_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSUPPERSOLDIER_2 );
         }
         break;
      case 6: // Tantegel ground floor bottom soldier next to throne room stairs
         Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSLOWERSOLDIER_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSLOWERSOLDIER );
         break;
      case 7: // Tantegel ground floor bottom right static soldier
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_BOTTOMRIGHTSTATICSOLDIER_PR );
         }
         else if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_BOTTOMRIGHTSTATICSOLDIER_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.rescuedPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_BOTTOMRIGHTSTATICSOLDIER_2 : STRING_NPC_TANTEGEL_GROUNDFLOOR_BOTTOMRIGHTSTATICSOLDIER_1 );
         }
         break;
      case 8: // Tantegel ground floor entry left soldier
         if ( game->gameFlags.defeatedDragonlord )
         {
            sprintf( msg, STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYLEFTSOLDIER_DF, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYLEFTSOLDIER_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYLEFTSOLDIER );
         }
         break;
      case 9: // Tantegel ground floor entry right soldier
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYRIGHTSOLDIER_PR );
         }
         else if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYRIGHTSOLDIER_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYRIGHTSOLDIER_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_ENTRYRIGHTSOLDIER_2 );
         }
         break;
      case 10: // Tantegel ground floor upper-left static blue man
         if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERLEFTSTATICBLUEMAN_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERLEFTSTATICBLUEMAN_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERLEFTSTATICBLUEMAN );
         }
         break;
      case 11: // Tantegel ground floor upper-right static blue woman
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERRIGHTSTATICBLUEWOMAN_PR );
         }
         else if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERRIGHTSTATICBLUEWOMAN_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERRIGHTSTATICBLUEWOMAN_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERRIGHTSTATICBLUEWOMAN_2 );
         }
         break;
      case 12: // Tantegel ground floor static barrier soldier
         Dialog_PushSection( &( game->dialog ), game->gameFlags.defeatedDragonlord ? STRING_NPC_TANTEGEL_GROUNDFLOOR_STATICBARRIERSOLDIER_DF : STRING_NPC_TANTEGEL_GROUNDFLOOR_STATICBARRIERSOLDIER );
         break;
      case 13: // Tantegel ground floor blue man by the stairs
         Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSBLUEMAN_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_STAIRSBLUEMAN );
         break;
      case 14: // Tantegel ground floor blue woman in the courtyard
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_COURTYARDBLUEWOMAN_PR );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.rescuedPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_COURTYARDBLUEWOMAN_2 : STRING_NPC_TANTEGEL_GROUNDFLOOR_COURTYARDBLUEWOMAN_1 );
         }
         break;
      case 15: // Tantegel ground floor green man on the right in the lower-left room
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANRIGHT_PR );
         }
         else if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANRIGHT_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANRIGHT_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANRIGHT_2 );
         }
         break;
      case 16: // Tantegel ground floor green man on the left in the lower-left room
         if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANLEFT_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANLEFT_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_LOWERLEFTGREENMANLEFT );
         }
         break;
      case 17: // Tantegel ground floor treasure room soldier
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_TREASUREROOMSOLDIER_PR );
         }
         else if ( game->gameFlags.defeatedDragonlord )
         {
            sprintf( msg, STRING_NPC_TANTEGEL_GROUNDFLOOR_TREASUREROOMSOLDIER_DF, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_TREASUREROOMSOLDIER_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_TREASUREROOMSOLDIER_2 );
         }
         break;
      case 18: // Tantegel ground floor upper area soldier
         if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERSOLDIER_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERSOLDIER_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_UPPERSOLDIER );
         }
         break;
      case 19: // Tantegel ground floor right area moving soldier
         if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_RIGHTACTIVESOLDIER_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_RIGHTACTIVESOLDIER_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_RIGHTACTIVESOLDIER );
         }
         break;
      case 20: // Tantegel ground floor moving wizard
         if ( game->gameFlags.defeatedDragonlord )
         {
            sprintf( msg, STRING_NPC_TANTEGEL_GROUNDFLOOR_ACTIVEWIZARD_DF, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), game->gameFlags.carryingPrincess ? STRING_NPC_TANTEGEL_GROUNDFLOOR_ACTIVEWIZARD_PR : STRING_NPC_TANTEGEL_GROUNDFLOOR_ACTIVEWIZARD );
         }
         break;
      case 21: // Tantegel ground floor soldier on the other side of the barrier
         if ( game->gameFlags.carryingPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_PASTBARRIERSOLDIER_PR );
         }
         else if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_PASTBARRIERSOLDIER_DF );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_PASTBARRIERSOLDIER_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_GROUNDFLOOR_PASTBARRIERSOLDIER_2 );
         }
         break;
      case 22: // Tantegel basement wizard
         if ( game->gameFlags.defeatedDragonlord )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_BASEMENT_WIZARD_DF_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_BASEMENT_WIZARD_DF_2 );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), ( game->gameFlags.treasures & 0x80 ) ? STRING_NPC_TANTEGEL_BASEMENT_WIZARD_1 : STRING_NPC_TANTEGEL_BASEMENT_WIZARD_2 );
         }
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
         if ( game->player.isCursed )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD2 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD3 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD4 );
            Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD5, Game_CurseLiftedCallback, game );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD6 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD7 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD8 );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_RIGHTROOMWIZARD1 );
         }
         break;
      case 27: // Brecconary inn static soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_INNSTATICSOLDIER );
         break;
      case 28: // Brecconary lower-right area static blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_LOWERRIGHTSTATICBLUEMAN_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_LOWERRIGHTSTATICBLUEMAN_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_LOWERRIGHTSTATICBLUEMAN_3 );
         break;
      case 29: // Brecconary upper-right soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_UPPERRIGHTSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_UPPERRIGHTSOLDIER_2 );
         break;
      case 30: // Brecconary upper wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_UPPERWIZARD );
         break;
      case 31: // Brecconary upper green man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_UPPERGREENMAN );
         break;
      case 32: // Brecconary center blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_CENTERBLUEMAN_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_CENTERBLUEMAN_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_CENTERBLUEMAN_3 );
         break;
      case 33: // Brecconary center red soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_CENTERREDSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_BRECCONARY_CENTERREDSOLDIER_2 );
         sprintf( msg, STRING_NPC_BRECCONARY_CENTERREDSOLDIER_3, game->player.name );
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
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_SOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_SOLDIER_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_SOLDIER_3 );
         break;
      case 41: // Garinham outside blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEWOMAN );
         break;
      case 42: // Garinham outside blue man
         if ( game->gameFlags.rescuedPrincess )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEMAN_2 );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEMAN_1_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEMAN_1_2 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEMAN_1_3 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_GARINHAMOUTSIDE_BLUEMAN_1_4 );
         }
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
      case 49: // Kol right-side static soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_STATICRIGHTSOLDIER );
         break;
      case 50: // Kol static blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_STATICBLUEWOMAN );
         break;
      case 51: // Kol left-side static soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_STATICLEFTSOLDIER );
         break;
      case 52: // Kol wizard at the entrance
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_ENTRANCEWIZARD );
         break;
      case 53: // Kol blue man at the entrance
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_ENTRANCEBLUEMAN_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_ENTRANCEBLUEMAN_2 );
         break;
      case 54: // Kol blue woman in the courtyard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_COURTYARDBLUEWOMAN );
         break;
      case 55: // Kol soldier in the courtyard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_COURTYARDSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_COURTYARDSOLDIER_2 );
         break;
      case 56: // Kol brown soldier in the courtyard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_COURTYARDBROWNSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_COURTYARDBROWNSOLDIER_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_COURTYARDBROWNSOLDIER_3 );
         break;
      case 57: // Kol left-side wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_LEFTWIZARD_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_LEFTWIZARD_2 );
         break;
      case 58: // Kol lower-left green man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_LOWERLEFTGREENMAN_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_LOWERLEFTGREENMAN_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_KOL_LOWERLEFTGREENMAN_3 );
         break;
      case 59: // Cantlin upper-left soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_UPPERLEFTSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_UPPERLEFTSOLDIER_2 );
         break;
      case 60: // Cantlin upper soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_UPPERSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_UPPERSOLDIER_2 );
         break;
      case 61: // Cantlin upper-right soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_UPPERRIGHTSOLDIER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_UPPERRIGHTSOLDIER_2 );
         break;
      case 62: // Cantlin left-upper blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_LEFTUPPERBLUEWOMAN );
         break;
      case 63: // Cantlin left-lower blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_LEFTLOWERBLUEWOMAN );
         break;
      case 64: // Cantlin Nester
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_NESTER_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_NESTER_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_NESTER_3 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_NESTER_4 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_NESTER_5 );
         break;
      case 65: // Cantlin right green man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_RIGHTGREENMAN_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_RIGHTGREENMAN_2 );
         break;
      case 66: // Cantlin barrier wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_BARRIERWIZARD_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_BARRIERWIZARD_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLIN_BARRIERWIZARD_3 );
         break;
      case 67: // Cantlin courtyard wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CANTLINCOURTYARD_WIZARD );
         break;
      case 68: // Rimuldar inn wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_INNWIZARD_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_INNWIZARD_2 );
         break;
      case 69: // Rimuldar upper static blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_UPPERSTATICBLUEWOMAN );
         break;
      case 70: // Rimuldar left static wizard
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_LEFTSTATICWIZARD_1 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_LEFTSTATICWIZARD_2 );
         break;
      case 71: // Rimuldar upper-right edge of town blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_UPPERRIGHTEDGEBLUEMAN );
         break;
      case 72: // Rimuldar lower-left edge of town blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_LOWERLEFTEDGEBLUEWOMAN );
         break;
      case 73: // Rimuldar upper-left static green man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_UPPERLEFTSTATICGREENMAN );
         break;
      case 74: // Rimuldar entrance blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_ENTRANCEBLUEWOMAN );
         break;
      case 75: // Rimuldar center blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_CENTERBLUEMAN );
         break;
      case 76: // Rimuldar inn soldier
         sprintf( msg, STRING_NPC_RIMULDAR_INNSOLDIER_1, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_INNSOLDIER_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_INNSOLDIER_3 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_INNSOLDIER_4 );
         break;
      case 77: // Rimuldar weapon shop soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_WEAPONSHOPSOLDIER );
         break;
      case 78: // Rimuldar upper room blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_UPPERROOMBLUEMAN );
         break;
      case 79: // Rimuldar lower-left soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDAR_LOWERLEFTSOLDIER );
         break;
      case 80: // Rimuldar lower soldier
         Dialog_PushSection( &( game->dialog ), game->gameFlags.rescuedPrincess ? STRING_NPC_RIMULDAR_LOWERSOLDIER_2 : STRING_NPC_RIMULDAR_LOWERSOLDIER_1 );
         break;
      case 81: // Rimuldar center building upper-left blue woman
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDARBUILDING_UPPERLEFTBLUEWOMAN );
         break;
      case 82: // Rimuldar center building lower-left blue man
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDARBUILDING_LOWERLEFTBLUEMAN );
         break;
      case 83: // Rimuldar center building lower-right soldier
         Dialog_PushSection( &( game->dialog ), STRING_NPC_RIMULDARBUILDING_LOWERRIGHTSOLDIER );
         break;
      case 84: // Northern shrine wizard
         if ( game->gameFlags.gotStaffOfRain )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_NORTHERNSHRINE_WIZARD_3 );
         }
         else if ( ITEM_HAS_SILVERHARP( game->player.items ) )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_NORTHERNSHRINE_WIZARD_2_1 );
            Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_NORTHERNSHRINE_WIZARD_2_2, Game_StaffOfRainNpcCallback, game );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_NORTHERNSHRINE_WIZARD_1_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_NORTHERNSHRINE_WIZARD_1_2 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_NORTHERNSHRINE_WIZARD_1_3 );
         }
         break;
      case 85: // Southern shrine wizard
         if ( game->gameFlags.gotRainbowDrop )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_4 );
         }
         else if ( !ITEM_HAS_TOKEN( game->player.items ) )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_1_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_1_2 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_1_3 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_1_4 );
         }
         else if ( ITEM_HAS_STAFFOFRAIN( game->player.items ) && ITEM_HAS_STONEOFSUNLIGHT( game->player.items ) )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_3_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_3_2 );
            Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_3_3, Game_RainbowDropNpcCallback, game );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_2_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SOUTHERNSHRINE_WIZARD_2_2 );
         }
         break;
      case 86: // Dragonlord
         sprintf( msg, STRING_NPC_CHARLOCK_DRAGONLORD_1_1, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_1_2 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_1_3 );
         Dialog_PushSection( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_1_4 );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_1_5, Game_DragonlordChoicePresentationCallback, game );
         break;
      case 87: // Gwaelin (swamp cave)
         if ( game->player.isCursed )
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SWAMPCAVE_PRINCESS_CURSED_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SWAMPCAVE_PRINCESS_CURSED_2 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SWAMPCAVE_PRINCESS_CURSED_3 );
         }
         else
         {
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SWAMPCAVE_PRINCESS_1 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SWAMPCAVE_PRINCESS_2 );
            Dialog_PushSection( &( game->dialog ), STRING_NPC_SWAMPCAVE_PRINCESS_3 );
            sprintf( msg, STRING_NPC_SWAMPCAVE_PRINCESS_4, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
            Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_SWAMPCAVE_PRINCESS_5, Game_RescuePrincessCallback, game );
            sprintf( msg, STRING_NPC_SWAMPCAVE_PRINCESS_6, game->player.name );
            Dialog_PushSection( &( game->dialog ), msg );
         }
         break;
      case 88:
         Dialog_PushSection( &( game->dialog ), STRING_NPC_STATICSOLDIER );
         break;

      default: // should never happen, but it's nice to have a catch-all
         Dialog_PushSection( &( game->dialog ), STRING_NPC_DEFAULT );
         break;
   }

   Game_OpenDialog( game );
}

internal void Game_StaffOfRainNpcCallback( Game_t* game )
{
   game->gameFlags.gotStaffOfRain = True;

   if ( !ITEM_HAS_STAFFOFRAIN( game->player.items ) )
   {
      ITEM_TOGGLE_HASSTAFFOFRAIN( game->player.items );
   }
}

internal void Game_RainbowDropNpcCallback( Game_t* game )
{
   game->gameFlags.gotRainbowDrop = True;

   if ( ITEM_HAS_STAFFOFRAIN( game->player.items ) )
   {
      ITEM_TOGGLE_HASSTAFFOFRAIN( game->player.items );
   }

   if ( ITEM_HAS_STONEOFSUNLIGHT( game->player.items ) )
   {
      ITEM_TOGGLE_HASSTONEOFSUNLIGHT( game->player.items );
   }

   if ( !ITEM_HAS_RAINBOWDROP( game->player.items ) )
   {
      ITEM_TOGGLE_HASRAINBOWDROP( game->player.items );
   }
}

internal void Game_DragonlordChoicePresentationCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_DragonlordJoinCallback, Game_DragonlordRefuseCallback, 0,
                      game, game, 0, False );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_DragonlordJoinCallback( Game_t* game )
{
   char msg[128];

   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_2_1 );
   sprintf( msg, STRING_NPC_CHARLOCK_DRAGONLORD_2_2, game->player.name );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_DragonlordJoinTransferCallback, game );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_2_3 );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_2_4, Game_DragonlordJoinFadeCallback, game );
   Game_OpenDialog( game );
}

internal void Game_DragonlordJoinTransferCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_DragonlordJoinedCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_DragonlordJoinedCallback( Game_t* game )
{
   game->gameFlags.joinedDragonlord = True;
}

internal void Game_DragonlordJoinFadeCallback( Game_t* game )
{
   u32 i;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 18; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ChangeToOverworldState, game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_SlowFadeOut );

   for ( i = 0; i < 10; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_DragonlordJoinPostFadeCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_DragonlordJoinPostFadeCallback( Game_t* game )
{
   Screen_RestorePalette( &( game->screen ) );
   game->gameFlags.joinedDragonlord = False;
   Game_Reset( game );
}

internal void Game_DragonlordRefuseCallback( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_CHARLOCK_DRAGONLORD_3, Game_DragonlordRefuseMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_DragonlordRefuseMessageCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_DragonlordInitiateFightCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_DragonlordInitiateFightCallback( Game_t* game )
{
   game->battle.specialEnemy = SpecialEnemy_DragonlordWizard;
   Battle_Generate( &( game->battle ) );
   game->postRenderCallback = Game_ChangeToBattleState;
   game->postRenderCallbackData = game;
}

internal void Game_CurseLiftedCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_CastSpell, Game_CurseLiftedSpellCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_CurseLiftedSpellCallback( Game_t* game )
{
   Player_SetCursed( &( game->player ), False );
}

internal void Game_RescuePrincessCallback( Game_t* game )
{
   game->gameFlags.rescuedPrincess = True;
   game->gameFlags.carryingPrincess = True;
   game->tileMap.npcCount = 0;
   Sprite_LoadActive( &( game->player.sprite ), ACTIVE_SPRITE_PLAYER_CARRY_ID );
}

internal void Game_ReturnPrincessCallback( Game_t* game )
{
   u32 i;
   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 18; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ReturnPrincessPreFadeCallback, game );

   for ( i = 0; i < 3; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeOut );

   for ( i = 0; i < 5; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ReturnPrincessFadeOutCallback, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_FadeIn, Screen_RestorePalette, &( game->screen ) );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ReturnPrincessPostFadeCallback, game );

   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_ReturnPrincessPreFadeCallback( Game_t* game )
{
   game->mainState = MainState_Overworld;
   game->subState = SubState_None;
}

internal void Game_ReturnPrincessFadeOutCallback( Game_t* game )
{
   if ( !ITEM_HAS_GWAELYNSLOVE( game->player.items ) )
   {
      ITEM_TOGGLE_HASGWAELYNSLOVE( game->player.items );
   }

   game->gameFlags.carryingPrincess = False;
   Sprite_LoadActive( &( game->player.sprite ), ACTIVE_SPRITE_PLAYER_ID );
   TileMap_Load( &( game->tileMap ), TILEMAP_TANTEGEL_THRONEROOM_ID );
   game->player.tileIndex = 131;
   Player_SetCanonicalTileIndex( &( game->player ) );
   Player_CenterOnTile( &( game->player ) );
   ActiveSprite_SetDirection( &( game->player.sprite ), Direction_Up );
   TileMap_UpdateViewport( &( game->tileMap ) );
}

internal void Game_ReturnPrincessPostFadeCallback( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_PRINCESS_3 );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_PRINCESS_4 );
   Game_OpenDialog( game );
}
