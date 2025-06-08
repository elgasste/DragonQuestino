#include "game.h"
#include "random.h"

internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag );
internal void Game_FoundHiddenStairsCallback( Game_t* game );

void Game_Talk( Game_t* game )
{
   Dialog2_Reset( &( game->dialog2 ) );;
   Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_NOBODY_THERE );
   Game_OpenDialog2( game );
}

void Game_Search( Game_t* game )
{
   uint32_t treasureFlag;
   char msg[64];

   Dialog2_Reset( &( game->dialog2 ) );;

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID && game->player.tileIndex == TILEMAP_TOKEN_INDEX && !ITEM_HAS_TOKEN( game->player.items ) )
   {
      Player_CollectItem( &( game->player ), Item_Token );
      Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_SEARCH );
      sprintf( msg, STRING_DIALOG_SEARCH_FOUND, STRING_FOUNDITEM_TOKEN );
      Dialog2_PushSection( &( game->dialog2 ), msg );
   }
   else if ( game->tileMap.id == TILEMAP_KOL_ID && game->player.tileIndex == TILEMAP_FAIRYFLUTE_INDEX && !ITEM_HAS_FAIRYFLUTE( game->player.items ) )
   {
      Player_CollectItem( &( game->player ), Item_FairyFlute );
      Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_SEARCH );
      sprintf( msg, STRING_DIALOG_SEARCH_FOUND, STRING_FOUNDITEM_FAIRYFLUTE );
      Dialog2_PushSection( &( game->dialog2 ), msg );
   }
   else if ( game->tileMap.id == TILEMAP_CHARLOCK_ID && game->player.tileIndex == TILEMAP_HIDDENSTAIRS_INDEX && !game->gameFlags.foundHiddenStairs )
   {
      game->gameFlags.foundHiddenStairs = True;
      TileMap_LoadHiddenStairs( &( game->tileMap ) );
      Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_SEARCH );
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), STRING_FOUND_HIDDENSTAIRS, Game_FoundHiddenStairsCallback, game );
   }
   else
   {
      treasureFlag = TileMap_GetTreasureFlag( game->tileMap.id, game->player.tileIndex );

      if ( treasureFlag && ( game->gameFlags.treasures & treasureFlag ) )
      {
         Game_CollectTreasure( game, treasureFlag );
      }
      else
      {
         Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_SEARCH );
         Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_SEARCH_NOT_FOUND );
      }
   }

   Game_OpenDialog2( game );
}

void Game_OpenDoor( Game_t* game )
{
   uint32_t doorTileIndex = TileMap_GetFacingTileIndex( &( game->tileMap ), game->player.tileIndex, game->player.sprite.direction );
   uint32_t doorFlag = TileMap_GetDoorFlag( game->tileMap.id, doorTileIndex );

   if ( doorFlag && ( game->gameFlags.doors & doorFlag ) )
   {
      if ( !ITEM_GET_KEYCOUNT( game->player.items ) )
      {
         Dialog2_Reset( &( game->dialog2 ) );;
         Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_NO_KEYS );
         Game_OpenDialog2( game );
      }
      else
      {
         ITEM_SET_KEYCOUNT( game->player.items, ITEM_GET_KEYCOUNT( game->player.items ) - 1 );
         game->gameFlags.doors ^= doorFlag;
         Game_ChangeSubState( game, SubState_None );
      }
   }
   else
   {
      Dialog2_Reset( &( game->dialog2 ) );;
      Dialog2_PushSection( &( game->dialog2 ), STRING_DIALOG_NO_DOOR );
      Game_OpenDialog2( game );
   }
}

internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag )
{
   uint16_t gold = 0;
   Bool_t collected = False;
   char msg[64];

   switch ( treasureFlag )
   {
      case 0x1:         // Tantegel throne room, upper-right chest
      case 0x4000000:   // Charlock B7, center-right chest
         collected = Player_CollectItem( &( game->player ), Item_Key );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_KEY );
         break;
      case 0x4:         // Tantegel throne room, lower-right chest
      case 0x400:       // Rocky Mountain Cave B1
      case 0x20000:     // Garinham, bottom-left chest
      case 0x40000:     // Garin's Grave B1, left chest
      case 0x1000000:   // Charlock B7, top chest
      case 0x20000000:  // Charlock B7, bottom-right chest
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_HERB );
         break;
      case 0x200:       // Rimuldar Inn
      case 0x8000000:   // Charlock B7, bottom-left chest
         collected = Player_CollectItem( &( game->player ), Item_Wing );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_WING );
         break;
      case 0x1000:      // Rocky Mountain Cave B2, upper-left area, right chest
      case 0x10000:     // Garinham, top-right chest
         collected = Player_CollectItem( &( game->player ), Item_Torch );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_TORCH );
         break;
      case 0x200000:    // Garin's Grave B3, upper-left chest
      case 0x10000000:  // Charlock B7, bottom-center chest
         collected = Player_CollectItem( &( game->player ), Item_CursedBelt );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_CURSEDBELT );
         break;
      case 0x800:       // Rocky Mountain Cave B2, upper-left area, left chest
         collected = Player_CollectItem( &( game->player ), Item_DragonScale );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_DRAGONSCALE );
         break;
      case 0x400000:    // Garin's Grave B3, right chest
         collected = Player_CollectItem( &( game->player ), Item_SilverHarp );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_SILVERHARP );
         break;
      case 0x80:        // Tantegel basement
         collected = Player_CollectItem( &( game->player ), Item_StoneOfSunlight );
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_STONEOFSUNLIGHT );
         break;
      case 0x2:         // Tantegel throne room, lower-left chest
         gold = 120; break;
      case 0x8:         // Tantegel ground floor, left room, upper-left chest
      case 0x10:        // Tantegel ground floor, left room, middle chest
      case 0x20:        // Tantegel ground floor, left room, bottom-left chest
      case 0x40:        // Tantegel ground floor, left room, bottom-right chest
      case 0x100000:    // Garin's Grave B1, right chest
         gold = Random_u16( 6, 13 ); break;
      case 0x4000:      // Rocky Mountain Cave B2, center-right chest
      case 0x8000:      // Garinham, top-left chest
         gold = Random_u16( 10, 17 ); break;
      case 0x80000:     // Garin's Grave B1, center chest
         gold = Random_u16( 5, 20 ); break;
      case 0x800000:    // Charlock B2
         // TODO: should be Erdrick's Sword
         gold = 1000; break;
      case 0x2000000:   // Charlock B7, center-left chest
         gold = Random_u16( 500, 755 ); break;
      case 0x2000:      // Rocky Mountain Cave B2, center-left chest
         if ( Random_Percent() <= 5 )
         {
            game->gameFlags.treasures ^= treasureFlag;
            sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_DEATHNECKLACE );
            Dialog2_PushSection( &( game->dialog2 ), msg );
            Dialog2_PushSectionWithCallback( &( game->dialog2 ), STRING_CURSED, Game_CursedCallback, game );
            return;
         }
         else
         {
            gold = Random_u16( 100, 131 ); break;
         }
         break;
      case 0x100:       // Erdrick's Cave, the tablet. this is not an item that can be collected.
         game->gameFlags.treasures ^= treasureFlag;
         sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_TABLET );
         Dialog2_PushSection( &( game->dialog2 ), msg );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_TABLET_1 );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_TABLET_2 );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_TABLET_3 );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_TABLET_4 );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_TABLET_5 );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_TABLET_6 );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_TABLET_7 );
         return;
   }

   if ( gold > 0 )
   {
      collected = ( Player_CollectGold( &( game->player ), gold ) > 0 ) ? True : False;
      sprintf( msg, STRING_CHEST_GOLDFOUND, gold );

      if ( collected > 0 )
      {
         game->gameFlags.treasures ^= treasureFlag;
         Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_DrawQuickStatus, game );
      }
      else
      {
         Dialog2_PushSection( &( game->dialog2 ), msg );
         Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_GOLDNOSPACE );
      }

      return;
   }

   Dialog2_PushSection( &( game->dialog2 ), msg );

   if ( collected )
   {
      game->gameFlags.treasures ^= treasureFlag;
   }
   else
   {
      Dialog2_PushSection( &( game->dialog2 ), STRING_CHEST_ITEMNOSPACE );
   }
}

internal void Game_FoundHiddenStairsCallback( Game_t* game )
{
   game->screen.needsRedraw = True;
}
