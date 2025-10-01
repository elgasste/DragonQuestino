#include "game.h"
#include "tables.h"

internal void Game_VisitInn( Game_t* game, u32 boothId );
internal void Game_VisitInnChoiceCallback( Game_t* game );
internal void Game_VisitInnStayCallback( Game_t* game );
internal void Game_VisitInnLeaveCallback( Game_t* game );
internal void Game_VisitInnStayAnimationCallback( Game_t* game );
internal void Game_VisitInnStayPostAnimationCallback( Game_t* game );
internal void Game_VisitInnStayMorningCallback( Game_t* game );
internal void Game_VisitWeaponShop( Game_t* game );
internal void Game_VisitItemShop( Game_t* game );
internal void Game_VisitKeyShop( Game_t* game, u32 boothId );
internal void Game_VisitFairyWaterShop( Game_t* game );
internal void Game_VisitTantegelWizard( Game_t* game );
internal void Game_KeyShopLeaveOrStayCallback( Game_t* game );
internal void Game_KeyShopPurchaseCallback( Game_t* game );
internal void Game_KeyShopLeaveCallback( Game_t* game );
internal void Game_FairyWaterShopLeaveOrBuyCallback( Game_t* game );
internal void Game_FairyWaterShopPurchaseCallback( Game_t* game );
internal void Game_FairyWaterShopLeaveCallback( Game_t* game );
internal void Game_ShopLeaveOrStayCallback( Game_t* game );
internal void Game_ShopViewItemsCallback( Game_t* game );
internal void Game_ShopViewItemsMessageCallback( Game_t* game );
internal void Game_ShopSellItemCallback( Game_t* game );
internal void Game_ShopCannotSellItemCallback( Game_t* game );
internal void Game_ShopSellItemMessageCallback( Game_t* game );
internal void Game_ShopLeaveCallback( Game_t* game );
internal void Game_ShopPurchaseOrNotCallback( Game_t* game );
internal void Game_ShopNoPurchaseCallback( Game_t* game );
internal void Game_WeaponShopPurchaseCallback( Game_t* game );
internal void Game_WeaponShopResellOrNotCallback( Game_t* game );
internal void Game_WeaponShopPurchaseCompleteCallback( Game_t* game );
internal void Game_ItemShopPurchaseCallback( Game_t* game );
internal void Game_TantegelWizardCallback( Game_t* game );
internal void Game_TantegelWizardMagicCallback( Game_t* game );
internal void Game_LoadWeaponShop( Game_t* game, u32 boothId );
internal void Game_LoadWeaponShopItem( ShopItem_t* item, AccessoryType_t type, u32 accessoryId );
internal void Game_LoadItemShop( Game_t* game, u32 boothId );

void Game_ActivateBooth( Game_t* game, u32 boothId )
{
   if ( boothId <= 4 )
   {
      Game_VisitInn( game, boothId );
   }
   else if ( boothId <= 11 )
   {
      Game_LoadWeaponShop( game, boothId );
      Game_VisitWeaponShop( game );
   }
   else if ( boothId <= 16 )
   {
      Game_LoadItemShop( game, boothId );
      Game_VisitItemShop( game );
   }
   else if ( boothId <= 19 )
   {
      Game_VisitKeyShop( game, boothId );
   }
   else if ( boothId <= 21 )
   {
      Game_VisitFairyWaterShop( game );
   }
   else if ( boothId == 22 )
   {
      Game_VisitTantegelWizard( game );
   }
   else
   {
      Dialog_Reset( &( game->dialog ) );

      switch ( boothId )
      {
         case 23: // Kol NW building
            if ( game->player.weapon.id == WEAPON_ERDRICKSSWORD_ID )
            {
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD7 );
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD8 );
            }
            else
            {
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD1 );
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD2 );
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD3 );
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD4 );
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD5 );
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_KOLWIZARD6 );
            }
            break;
         case 24: // Cantlin E building
            Dialog_PushSection( &( game->dialog ), STRING_BOOTH_CANTLINWIZARD );
            break;
         case 25: // Cantlin W building
            Dialog_PushSection( &( game->dialog ), STRING_BOOTH_CANTLINRADISHES );
            break;
         case 26: // Cantlin SW building
            Dialog_PushSection( &( game->dialog ), STRING_BOOTH_CANTLINWYNN1 );
            Dialog_PushSection( &( game->dialog ), STRING_BOOTH_CANTLINWYNN2 );
            break;
         case 27: // Rimuldar S building
            if ( game->gameFlags.gotRainbowDrop )
            {
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_RIMULDARWIZARD3 );
            }
            else
            {
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_RIMULDARWIZARD1 );
               Dialog_PushSection( &( game->dialog ), STRING_BOOTH_RIMULDARWIZARD2 );
            }
            break;
      }

      Game_OpenDialog( game );
   }
}

void Game_SelectShopItem( Game_t* game )
{
   char msg[64];
   ShopItem_t* item = &( game->tileMap.shopItems[game->shopPicker.selectedIndex] );
   ShopPickerItem_t* pickerItem = &( game->shopPicker.items[game->shopPicker.selectedIndex] );

   Dialog_Reset( &( game->dialog ) );

   if ( item->price > game->player.gold )
   {
      Dialog_PushSection( &( game->dialog ), ( game->tileMap.shopType == ShopType_Weapon ) ? STRING_WEAPONSHOP_TOOEXPENSIVE : STRING_ITEMSHOP_TOOEXPENSIVE );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_ShopViewItemsMessageCallback, game );
   }
   else if ( game->tileMap.shopType == ShopType_Weapon )
   {
      if ( item->type == AccessoryType_Weapon && game->player.weapon.id == WEAPON_ERDRICKSSWORD_ID )
      {
         Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_ERDRICKSSWORD );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_ShopViewItemsMessageCallback, game );
      }
      else if ( item->type == AccessoryType_Armor && game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
      {
         Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_ERDRICKSARMOR );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_ShopViewItemsMessageCallback, game );
      }
      else if ( ( item->type == AccessoryType_Weapon && game->player.weapon.id == item->id ) ||
                ( item->type == AccessoryType_Armor && game->player.armor.id == item->id ) ||
                ( item->type == AccessoryType_Shield && game->player.shield.id == item->id ) )
      {
         Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_ALREADYHAVE );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_ShopViewItemsMessageCallback, game );
      }
      else
      {
         if ( pickerItem->itemText.hasTwoLines )
         {
            sprintf( msg, STRING_WEAPONSHOP_AREYOUSURE2, pickerItem->itemText.line1, pickerItem->itemText.line2 );
         }
         else
         {
            sprintf( msg, STRING_WEAPONSHOP_AREYOUSURE1, pickerItem->itemText.line1 );
         }

         Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ShopPurchaseOrNotCallback, game );
      }
   }
   else
   {
      if ( ( item->id == ITEM_KEY_ID && ITEM_GET_KEYCOUNT( game->player.items ) >= ITEM_MAXKEYS ) ||
           ( item->id == ITEM_HERB_ID && ITEM_GET_HERBCOUNT( game->player.items ) >= ITEM_MAXHERBS ) ||
           ( item->id == ITEM_WING_ID && ITEM_GET_WINGCOUNT( game->player.items ) >= ITEM_MAXWINGS ) ||
           ( item->id == ITEM_FAIRYWATER_ID && ITEM_GET_FAIRYWATERCOUNT( game->player.items ) >= ITEM_MAXFAIRYWATERS ) ||
           ( item->id == ITEM_TORCH_ID && ITEM_GET_TORCHCOUNT( game->player.items ) >= ITEM_MAXTORCHES ) ||
           ( item->id == ITEM_DRAGONSCALE_ID && ITEM_HAS_DRAGONSCALE( game->player.items ) ) )
      {
         Dialog_PushSection( &( game->dialog ), STRING_ITEMSHOP_NOSPACE );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMSHOP_ANYTHINGELSE, Game_ShopViewItemsMessageCallback, game );
      }
      else
      {
         if ( pickerItem->itemText.hasTwoLines )
         {
            sprintf( msg, STRING_ITEMSHOP_AREYOUSURE2, pickerItem->itemText.line1, pickerItem->itemText.line2 );
         }
         else
         {
            sprintf( msg, STRING_ITEMSHOP_AREYOUSURE1, pickerItem->itemText.line1 );
         }

         Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ShopPurchaseOrNotCallback, game );
      }
   }

   Game_OpenDialog( game );
}

internal void Game_VisitInn( Game_t* game, u32 boothId )
{
   char msg[128];

   game->tileMap.boothPrice = g_innPriceTable[boothId];
   sprintf( msg, STRING_INN_WELCOME, game->tileMap.boothPrice );

   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_VisitInnChoiceCallback, game );
   Game_OpenDialog( game );
}

internal void Game_VisitInnChoiceCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_VisitInnStayCallback, Game_VisitInnLeaveCallback, 0,
                      game, game, 0, False );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_VisitInnStayCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );

   if ( game->player.gold >= game->tileMap.boothPrice )
   {
      game->player.gold -= game->tileMap.boothPrice;
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_INN_STAY, Game_VisitInnStayAnimationCallback, game );
   }
   else
   {
      Dialog_PushSection( &( game->dialog ), STRING_INN_TOOEXPENSIVE_1 );
      Dialog_PushSection( &( game->dialog ), STRING_INN_TOOEXPENSIVE_2 );
   }

   Game_OpenDialog( game );
}

internal void Game_VisitInnLeaveCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_INN_LEAVE );
   Game_OpenDialog( game );
}

internal void Game_VisitInnStayAnimationCallback( Game_t* game )
{
   u32 i;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 4; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_VisitInnStayPostAnimationCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_VisitInnStayPostAnimationCallback( Game_t* game )
{
   u32 i;

   Game_ChangeSubState( game, SubState_None );

   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeOut );

   for ( i = 0; i < 8; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_FadeIn, Game_VisitInnStayMorningCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_VisitInnStayMorningCallback( Game_t* game )
{
   game->player.stats.hitPoints = game->player.stats.maxHitPoints;
   game->player.stats.magicPoints = game->player.stats.maxMagicPoints;

   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_INN_MORNING );
   Game_OpenDialog( game );
}

internal void Game_VisitWeaponShop( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_WELCOME, Game_ShopLeaveOrStayCallback, game );
   Game_OpenDialog( game );
}

internal void Game_VisitItemShop( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );

   if ( ITEM_HAS_SELLABLE( game->player.items ) )
   {
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMSHOP_WELCOMEBUYSELL, Game_ShopLeaveOrStayCallback, game );
   }
   else
   {
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMSHOP_WELCOMEVIEWCHOICE, Game_ShopLeaveOrStayCallback, game );
   }

   Game_OpenDialog( game );
}

internal void Game_VisitKeyShop( Game_t* game, u32 boothId )
{
   char msg [128];

   switch ( boothId )
   {
      case 17: game->tileMap.boothPrice = 85; break;
      case 18: game->tileMap.boothPrice = 98; break;
      case 19: game->tileMap.boothPrice = 53; break;
   }

   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_KEYSHOP_WELCOME, game->tileMap.boothPrice );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_KeyShopLeaveOrStayCallback, game );
   Game_OpenDialog( game );
}

internal void Game_VisitFairyWaterShop( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_FAIRYWATERSHOP_PURCHASE, Game_FairyWaterShopLeaveOrBuyCallback, game );
   Game_OpenDialog( game );
}

internal void Game_VisitTantegelWizard( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_BOOTH_TANTEGELWIZARD, Game_TantegelWizardCallback, game );
   Game_OpenDialog( game );
}

internal void Game_KeyShopLeaveOrStayCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_KeyShopPurchaseCallback, Game_KeyShopLeaveCallback, 0,
                      game, game, 0, False );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_KeyShopPurchaseCallback( Game_t* game )
{
   u32 keyCount;

   Dialog_Reset( &( game->dialog ) );

   if ( game->tileMap.boothPrice > game->player.gold )
   {
      Dialog_PushSection( &( game->dialog ), STRING_KEYSHOP_TOOEXPENSIVE );
   }
   else
   {
      keyCount = ITEM_GET_KEYCOUNT( game->player.items );

      if ( keyCount >= ITEM_MAXKEYS )
      {
         Dialog_PushSection( &( game->dialog ), STRING_KEYSHOP_NOSPACE );
      }
      else
      {
         game->player.gold -= game->tileMap.boothPrice;
         ITEM_SET_KEYCOUNT( game->player.items, keyCount + 1 );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_KEYSHOP_THANKYOU, Game_KeyShopLeaveOrStayCallback, game );
      }
   }

   Game_OpenDialog( game );
}

internal void Game_KeyShopLeaveCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_KEYSHOP_LEAVE );
   Game_OpenDialog( game );
}

internal void Game_FairyWaterShopLeaveOrBuyCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_FairyWaterShopPurchaseCallback, Game_FairyWaterShopLeaveCallback, 0,
                      game, game, 0, False );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_FairyWaterShopPurchaseCallback( Game_t* game )
{
   u32 fairyWaterCount;

   Dialog_Reset( &( game->dialog ) );

   if ( game->player.gold < 38 )
   {
      Dialog_PushSection( &( game->dialog ), STRING_FAIRYWATERSHOP_TOOEXPENSIVE );
   }
   else
   {
      fairyWaterCount = ITEM_GET_FAIRYWATERCOUNT( game->player.items );

      if ( fairyWaterCount >= ITEM_MAXFAIRYWATERS )
      {
         Dialog_PushSection( &( game->dialog ), STRING_FAIRYWATERSHOP_NOSPACE );
      }
      else
      {
         game->player.gold -= 38;
         ITEM_SET_FAIRYWATERCOUNT( game->player.items, fairyWaterCount + 1 );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_FAIRYWATERSHOP_THANKYOU, Game_FairyWaterShopLeaveOrBuyCallback, game );
      }
   }

   Game_OpenDialog( game );
}

internal void Game_FairyWaterShopLeaveCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_FAIRYWATERSHOP_LEAVE );
   Game_OpenDialog( game );
}

internal void Game_ShopLeaveOrStayCallback( Game_t* game )
{
   if ( ITEM_HAS_SELLABLE( game->player.items ) )
   {
      BinaryPicker_Load( &( game->binaryPicker ),
                         STRING_BUY, STRING_SELL,
                         Game_ShopViewItemsCallback, Game_ShopSellItemCallback, Game_ShopLeaveCallback,
                         game, game, game, True );
   }
   else
   {
      BinaryPicker_Load( &( game->binaryPicker ),
                         STRING_YES, STRING_NO,
                         Game_ShopViewItemsCallback, Game_ShopLeaveCallback, 0,
                         game, game, 0, False );
   }

   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_ShopViewItemsCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ),
                                   ( game->tileMap.shopType == ShopType_Weapon ) ? STRING_WEAPONSHOP_VIEWITEMS : STRING_ITEMSHOP_VIEWITEMS,
                                   Game_ShopViewItemsMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_ShopViewItemsMessageCallback( Game_t* game )
{
   ShopPicker_Reset( &( game->shopPicker ) );
   Game_ChangeSubState( game, SubState_ShopMenu );
}

internal void Game_ShopSellItemCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );

   if ( game->player.gold == UINT16_MAX )
   {
      Dialog_PushSection( &( game->dialog ), STRING_ITEMSHOP_CANNOTSELLITEM_1 );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMSHOP_CANNOTSELLITEM_2, Game_ShopCannotSellItemCallback, game );
   }
   else
   {
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMSHOP_SELLWHICHITEM, Game_ShopSellItemMessageCallback, game );
   }

   Game_OpenDialog( game );
}

internal void Game_ShopCannotSellItemCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_ShopViewItemsCallback, Game_ShopLeaveCallback, 0,
                      game, game, 0, False );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_ShopSellItemMessageCallback( Game_t* game )
{
   Game_OpenMenu( game, MenuId_SellItem );
}

internal void Game_ShopLeaveCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), ( game->tileMap.shopType == ShopType_Weapon ) ? STRING_WEAPONSHOP_LEAVE : STRING_ITEMSHOP_LEAVE );
   Game_OpenDialog( game );
}

internal void Game_ShopPurchaseOrNotCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      ( game->tileMap.shopType == ShopType_Weapon ) ? Game_WeaponShopPurchaseCallback : Game_ItemShopPurchaseCallback,
                      Game_ShopNoPurchaseCallback, 0,
                      game, game, 0, False );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_ShopNoPurchaseCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ),
                                   ( game->tileMap.shopType == ShopType_Weapon ) ? STRING_WEAPONSHOP_ANYTHINGELSE : STRING_ITEMSHOP_ANYTHINGELSE,
                                   Game_ShopViewItemsMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_WeaponShopPurchaseCallback( Game_t* game )
{
   ShopItem_t* item = &( game->tileMap.shopItems[game->shopPicker.selectedIndex] );
   char tradeName[32];
   char msg[128];

   if ( ( item->type == AccessoryType_Weapon && game->player.weapon.id != WEAPON_NONE_ID ) ||
        ( item->type == AccessoryType_Armor && game->player.armor.id != ARMOR_NONE_ID ) ||
        ( item->type == AccessoryType_Shield && game->player.shield.id != SHIELD_NONE_ID ) )
   {
      Dialog_Reset( &( game->dialog ) );
      Player_GetAccessoryName( &( game->player ), item->type, tradeName );
      sprintf( msg, STRING_WEAPONSHOP_RESELL, tradeName, Player_GetAccessoryResellValue( &( game->player ), item->type ) );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_WeaponShopResellOrNotCallback, game );
      Game_OpenDialog( game );
   }
   else
   {
      Game_WeaponShopPurchaseCompleteCallback( game );
   }
}

internal void Game_WeaponShopResellOrNotCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_WeaponShopPurchaseCompleteCallback, Game_ShopNoPurchaseCallback, 0,
                      game, game, 0, False );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_WeaponShopPurchaseCompleteCallback( Game_t* game )
{
   u16 resellValue;
   ShopItem_t* item = &( game->tileMap.shopItems[game->shopPicker.selectedIndex] );

   game->player.gold -= item->price;

   if ( ( item->type == AccessoryType_Weapon && game->player.weapon.id != WEAPON_NONE_ID ) ||
        ( item->type == AccessoryType_Armor && game->player.armor.id != ARMOR_NONE_ID ) ||
        ( item->type == AccessoryType_Shield && game->player.shield.id != SHIELD_NONE_ID ) )
   {
      resellValue = Player_GetAccessoryResellValue( &( game->player ), item->type );
      game->player.gold += resellValue;

      if ( game->player.gold < resellValue ) // overflow
      {
         game->player.gold = UINT16_MAX;
      }
   }

   switch ( item->type )
   {
      case AccessoryType_Weapon: Player_LoadWeapon( &( game->player ), item->id ); break;
      case AccessoryType_Armor: Player_LoadArmor( &( game->player ), item->id ); break;
      case AccessoryType_Shield: Player_LoadShield( &( game->player ), item->id ); break;
   }

   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_THANKYOU, Game_ShopViewItemsMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_ItemShopPurchaseCallback( Game_t* game )
{
   ShopItem_t* item = &( game->tileMap.shopItems[game->shopPicker.selectedIndex] );

   switch ( item->id )
   {
      case ITEM_KEY_ID: ITEM_SET_KEYCOUNT( game->player.items, ITEM_GET_KEYCOUNT( game->player.items ) + 1 ); break;
      case ITEM_HERB_ID: ITEM_SET_HERBCOUNT( game->player.items, ITEM_GET_HERBCOUNT( game->player.items ) + 1 ); break;
      case ITEM_WING_ID: ITEM_SET_WINGCOUNT( game->player.items, ITEM_GET_WINGCOUNT( game->player.items ) + 1 ); break;
      case ITEM_FAIRYWATER_ID: ITEM_SET_FAIRYWATERCOUNT( game->player.items, ITEM_GET_FAIRYWATERCOUNT( game->player.items ) + 1 ); break;
      case ITEM_TORCH_ID: ITEM_SET_TORCHCOUNT( game->player.items, ITEM_GET_TORCHCOUNT( game->player.items ) + 1 ); break;
      case ITEM_DRAGONSCALE_ID: ITEM_TOGGLE_HASDRAGONSCALE( game->player.items );
   }

   game->player.gold -= item->price;

   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_ITEMSHOP_THANKYOU, Game_ShopViewItemsMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_TantegelWizardCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_CastSpell, Game_TantegelWizardMagicCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_TantegelWizardMagicCallback( Game_t* game )
{
   game->player.stats.magicPoints = game->player.stats.maxMagicPoints;
}

internal void Game_LoadWeaponShop( Game_t* game, u32 boothId )
{
   TileMap_t* tileMap = &( game->tileMap );
   ShopItem_t* item = tileMap->shopItems;

   tileMap->shopType = ShopType_Weapon;

   switch ( boothId )
   {
      case 5: // Brecconary weapon shop
         tileMap->shopItemCount = 6;
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_BAMBOOPOLE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_CLUB_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_COPPERSWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_CLOTHES_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_LEATHERARMOR_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Shield, SHIELD_SMALLSHIELD_ID );
         break;
      case 6: // Garinham weapon shop
         tileMap->shopItemCount = 7;
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_CLUB_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_COPPERSWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_HANDAXE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_LEATHERARMOR_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_CHAINMAIL_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_HALFPLATE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Shield, SHIELD_LARGESHIELD_ID );
         break;
      case 7: // Kol weapon shop
         tileMap->shopItemCount = 5;
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_COPPERSWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_HANDAXE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_HALFPLATE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_FULLPLATE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Shield, SHIELD_SMALLSHIELD_ID );
         break;
      case 8: // Cantlin upper-right weapon shop
         tileMap->shopItemCount = 6;
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_BAMBOOPOLE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_CLUB_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_COPPERSWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_LEATHERARMOR_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_CHAINMAIL_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Shield, SHIELD_LARGESHIELD_ID );
         break;
      case 9: // Cantlin middle-right weapon shop
         tileMap->shopItemCount = 2;
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_FLAMESWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Shield, SHIELD_SILVERSHIELD_ID );
         break;
      case 10: // Cantlin lower-right weapon shop
         tileMap->shopItemCount = 4;
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_HANDAXE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_BROADSWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_FULLPLATE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_MAGICARMOR_ID );
         break;
      case 11: // Rimuldar weapon shop
         tileMap->shopItemCount = 6;
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_COPPERSWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_HANDAXE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Weapon, WEAPON_BROADSWORD_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_HALFPLATE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_FULLPLATE_ID );
         Game_LoadWeaponShopItem( item++, AccessoryType_Armor, ARMOR_MAGICARMOR_ID );
         break;
   }
}

internal void Game_LoadWeaponShopItem( ShopItem_t* item, AccessoryType_t type, u32 accessoryId )
{
   item->id = accessoryId;
   item->type = type;

   switch ( type )
   {
      case AccessoryType_Weapon: item->price = g_weaponCostTable[accessoryId]; break;
      case AccessoryType_Armor: item->price = g_armorCostTable[accessoryId]; break;
      case AccessoryType_Shield: item->price = g_shieldCostTable[accessoryId]; break;
   }
}

internal void Game_LoadItemShop( Game_t* game, u32 boothId )
{
   TileMap_t* tileMap = &( game->tileMap );

   tileMap->shopType = ShopType_Item;

   switch ( boothId )
   {
      case 12: // Brecconary item shop
         tileMap->shopItemCount = 3;
         tileMap->shopItems[0].id = ITEM_HERB_ID;
         tileMap->shopItems[0].price = 24;
         tileMap->shopItems[1].id = ITEM_TORCH_ID;
         tileMap->shopItems[1].price = 8;
         tileMap->shopItems[2].id = ITEM_DRAGONSCALE_ID;
         tileMap->shopItems[2].price = 20;
         break;
      case 13: // Garinham item shop
         tileMap->shopItemCount = 3;
         tileMap->shopItems[0].id = ITEM_HERB_ID;
         tileMap->shopItems[0].price = 24;
         tileMap->shopItems[1].id = ITEM_TORCH_ID;
         tileMap->shopItems[1].price = 8;
         tileMap->shopItems[2].id = ITEM_DRAGONSCALE_ID;
         tileMap->shopItems[2].price = 20;
         break;
      case 14: // Kol item shop
         tileMap->shopItemCount = 4;
         tileMap->shopItems[0].id = ITEM_HERB_ID;
         tileMap->shopItems[0].price = 24;
         tileMap->shopItems[1].id = ITEM_TORCH_ID;
         tileMap->shopItems[1].price = 8;
         tileMap->shopItems[2].id = ITEM_DRAGONSCALE_ID;
         tileMap->shopItems[2].price = 20;
         tileMap->shopItems[3].id = ITEM_WING_ID;
         tileMap->shopItems[3].price = 70;
         break;
      case 15: // Cantlin upper-left item shop
         tileMap->shopItemCount = 2;
         tileMap->shopItems[0].id = ITEM_HERB_ID;
         tileMap->shopItems[0].price = 24;
         tileMap->shopItems[1].id = ITEM_TORCH_ID;
         tileMap->shopItems[1].price = 8;
         break;
      case 16: // Cantlin lower-left item shop
         tileMap->shopItemCount = 2;
         tileMap->shopItems[1].id = ITEM_DRAGONSCALE_ID;
         tileMap->shopItems[1].price = 20;
         tileMap->shopItems[2].id = ITEM_WING_ID;
         tileMap->shopItems[2].price = 70;
         break;
   }
}
