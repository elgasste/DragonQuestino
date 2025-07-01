#include "game.h"
#include "random.h"

internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag );
internal void Game_FoundHiddenStairsCallback( Game_t* game );
internal void Game_TakeHiddenStairsCallback( Game_t* game );

void Game_Talk( Game_t* game )
{
   uint32_t i, tileIndex;
   Bool_t canTalk = False;

   if ( game->tileMap.npcCount > 0 )
   {
      tileIndex = TileMap_GetFacingTileIndex( &( game->tileMap ), game->player.tileIndex, game->player.sprite.direction );

      for ( i = 0; i < game->tileMap.npcCount; i++ )
      {
         if ( game->tileMap.npcs[i].tileIndex == tileIndex )
         {
            canTalk = True;
            Game_RunNpcDialog( game, game->tileMap.npcs[i].id );
            break;
         }
      }
   }

   if ( !canTalk )
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_NOBODY_THERE );
      Game_OpenDialog( game );
   }
}

void Game_Search( Game_t* game )
{
   uint32_t treasureFlag;
   char msg[64];

   Dialog_Reset( &( game->dialog ) );

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID && game->player.tileIndex == TILEMAP_TOKEN_INDEX && !ITEM_HAS_TOKEN( game->player.items ) )
   {
      Player_CollectItem( &( game->player ), Item_Token );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_SEARCH );
      sprintf( msg, STRING_DIALOG_SEARCH_FOUND, STRING_FOUNDITEM_TOKEN );
      Dialog_PushSection( &( game->dialog ), msg );
   }
   else if ( game->tileMap.id == TILEMAP_KOL_ID && game->player.tileIndex == TILEMAP_FAIRYFLUTE_INDEX && !ITEM_HAS_FAIRYFLUTE( game->player.items ) )
   {
      Player_CollectItem( &( game->player ), Item_FairyFlute );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_SEARCH );
      sprintf( msg, STRING_DIALOG_SEARCH_FOUND, STRING_FOUNDITEM_FAIRYFLUTE );
      Dialog_PushSection( &( game->dialog ), msg );
   }
   else if ( game->tileMap.id == TILEMAP_HAUKSNESS_ID && game->player.tileIndex == TILEMAP_ERDRICKSARMOR_INDEX && game->player.armor.id != ARMOR_ERDRICKSARMOR_ID )
   {
      Player_LoadArmor( &( game->player ), ARMOR_ERDRICKSARMOR_ID );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_SEARCH );
      sprintf( msg, STRING_DIALOG_SEARCH_FOUND, STRING_FOUNDITEM_ERDRICKSARMOR );
      Dialog_PushSection( &( game->dialog ), msg );
   }
   else if ( game->tileMap.id == TILEMAP_CHARLOCK_ID && game->player.tileIndex == TILEMAP_HIDDENSTAIRS_INDEX && !game->gameFlags.foundHiddenStairs )
   {
      game->gameFlags.foundHiddenStairs = True;
      TileMap_LoadHiddenStairs( &( game->tileMap ) );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_SEARCH );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_FOUND_HIDDENSTAIRS, Game_FoundHiddenStairsCallback, game );
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
         Dialog_PushSection( &( game->dialog ), STRING_DIALOG_SEARCH );
         Dialog_PushSection( &( game->dialog ), STRING_DIALOG_SEARCH_NOT_FOUND );
      }
   }

   Game_OpenDialog( game );
}

void Game_OpenDoor( Game_t* game )
{
   uint32_t doorTileIndex = TileMap_GetFacingTileIndex( &( game->tileMap ), game->player.tileIndex, game->player.sprite.direction );
   uint32_t doorFlag = TileMap_GetDoorFlag( game->tileMap.id, doorTileIndex );

   if ( doorFlag && ( game->gameFlags.doors & doorFlag ) )
   {
      if ( !ITEM_GET_KEYCOUNT( game->player.items ) )
      {
         Dialog_Reset( &( game->dialog ) );;
         Dialog_PushSection( &( game->dialog ), STRING_DIALOG_NO_KEYS );
         Game_OpenDialog( game );
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
      Dialog_Reset( &( game->dialog ) );;
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_NO_DOOR );
      Game_OpenDialog( game );
   }
}

internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag )
{
   uint16_t gold = 0;
   Bool_t collected = False;
   char msg[64], itemMsg[24];

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
         collected = True;
         Player_LoadWeapon( &( game->player ), WEAPON_ERDRICKSSWORD_ID );
         sprintf( itemMsg, "%s %s", STRING_WEAPON_ERDRICKSSWORD1, STRING_WEAPON_ERDRICKSSWORD2 );
         sprintf( msg, STRING_CHEST_ITEMFOUND, itemMsg );
         break;
      case 0x2000000:   // Charlock B7, center-left chest
         gold = Random_u16( 500, 755 ); break;
      case 0x2000:      // Rocky Mountain Cave B2, center-left chest
         if ( Random_Percent() <= 5 )
         {
            game->gameFlags.treasures ^= treasureFlag;
            sprintf( msg, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_DEATHNECKLACE );
            Dialog_PushSection( &( game->dialog ), msg );
            Dialog_PushSectionWithCallback( &( game->dialog ), STRING_CURSED, Game_CursedCallback, game );
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
         Dialog_PushSection( &( game->dialog ), msg );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_TABLET_1 );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_TABLET_2 );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_TABLET_3 );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_TABLET_4 );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_TABLET_5 );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_TABLET_6 );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_TABLET_7 );
         return;
   }

   if ( gold > 0 )
   {
      collected = ( Player_CollectGold( &( game->player ), gold ) > 0 ) ? True : False;
      sprintf( msg, STRING_CHEST_GOLDFOUND, gold );

      if ( collected > 0 )
      {
         game->gameFlags.treasures ^= treasureFlag;
         Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_DrawQuickStatus, game );
      }
      else
      {
         Dialog_PushSection( &( game->dialog ), msg );
         Dialog_PushSection( &( game->dialog ), STRING_CHEST_GOLDNOSPACE );
      }

      return;
   }

   Dialog_PushSection( &( game->dialog ), msg );

   if ( collected )
   {
      game->gameFlags.treasures ^= treasureFlag;
   }
   else
   {
      Dialog_PushSection( &( game->dialog ), STRING_CHEST_ITEMNOSPACE );
   }
}

internal void Game_FoundHiddenStairsCallback( Game_t* game )
{
   game->screen.needsRedraw = True;
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_TakeHiddenStairsCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_TakeHiddenStairsCallback( Game_t* game )
{
   Game_PlayerSteppedOnTile( game );
}
