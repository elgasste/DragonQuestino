#include "game.h"
#include "tables.h"

internal void Game_VisitInn( Game_t* game, uint32_t boothId );
internal void Game_VisitInnChoiceCallback( Game_t* game );
internal void Game_VisitInnStayCallback( Game_t* game );
internal void Game_VisitInnLeaveCallback( Game_t* game );
internal void Game_VisitInnStayAnimationCallback( Game_t* game );
internal void Game_VisitInnStayPostAnimationCallback( Game_t* game );
internal void Game_VisitInnStayMorningCallback( Game_t* game );
internal void Game_VisitWeaponShop( Game_t* game );
internal void Game_WeaponShopLeaveOrStayCallback( Game_t* game );
internal void Game_WeaponShopViewItemsCallback( Game_t* game );
internal void Game_WeaponShopViewItemsMessageCallback( Game_t* game );
internal void Game_WeaponShopLeaveCallback( Game_t* game );
internal void Game_WeaponShopPurchaseOrNotCallback( Game_t* game );
internal void Game_WeaponShopPurchaseCallback( Game_t* game );
internal void Game_WeaponShopNoPurchaseCallback( Game_t* game );
internal void Game_LoadWeaponShop( Game_t* game, uint32_t boothId );

void Game_ActivateBooth( Game_t* game, uint32_t boothId )
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
}

void Game_SelectShopItem( Game_t* game )
{
   char msg[64];
   ShopItem_t* item = &( game->tileMap.shopItems[game->shopPicker.selectedIndex] );
   ShopPickerItem_t* pickerItem = &( game->shopPicker.items[game->shopPicker.selectedIndex] );

   Dialog_Reset( &( game->dialog ) );

   if ( item->price > game->player.gold )
   {
      Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_TOOEXPENSIVE );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_WeaponShopViewItemsMessageCallback, game );
   }
   else if ( game->tileMap.shopType == ShopType_Weapon )
   {
      if ( item->type == AccessoryType_Weapon && game->player.weapon.id == WEAPON_ERDRICKSSWORD_ID )
      {
         Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_ERDRICKSSWORD );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_WeaponShopViewItemsMessageCallback, game );
      }
      else if ( item->type == AccessoryType_Armor && game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
      {
         Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_ERDRICKSARMOR );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_WeaponShopViewItemsMessageCallback, game );
      }
      else if ( ( item->type == AccessoryType_Weapon && game->player.weapon.id == item->id ) ||
                ( item->type == AccessoryType_Armor && game->player.armor.id == item->id ) ||
                ( item->type == AccessoryType_Shield && game->player.shield.id == item->id ) )
      {
         Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_ALREADYHAVE );
         Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_WeaponShopViewItemsMessageCallback, game );
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

         Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_WeaponShopPurchaseOrNotCallback, game );
      }
   }

   Game_OpenDialog( game );
}

internal void Game_VisitInn( Game_t* game, uint32_t boothId )
{
   char msg[128];

   game->tileMap.innPrice = g_innPriceTable[boothId];
   sprintf( msg, STRING_INN_WELCOME, game->tileMap.innPrice );

   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_VisitInnChoiceCallback, game );
   Game_OpenDialog( game );
}

internal void Game_VisitInnChoiceCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_VisitInnStayCallback, Game_VisitInnLeaveCallback,
                      game, game );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_VisitInnStayCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );

   if ( game->player.gold >= game->tileMap.innPrice )
   {
      game->player.gold -= game->tileMap.innPrice;
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
   uint32_t i;

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
   uint32_t i;

   Game_ChangeSubState( game, SubState_None );

   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeOut );

   for ( i = 0; i < 6; i++ )
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
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_WELCOME, Game_WeaponShopLeaveOrStayCallback, game );
   Game_OpenDialog( game );
}

internal void Game_WeaponShopLeaveOrStayCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_WeaponShopViewItemsCallback, Game_WeaponShopLeaveCallback,
                      game, game );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_WeaponShopViewItemsCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_VIEWITEMS, Game_WeaponShopViewItemsMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_WeaponShopViewItemsMessageCallback( Game_t* game )
{
   ShopPicker_Reset( &( game->shopPicker ) );
   Game_ChangeSubState( game, SubState_ShopMenu );
}

internal void Game_WeaponShopLeaveCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_LEAVE );
   Game_OpenDialog( game );
}

internal void Game_WeaponShopPurchaseOrNotCallback( Game_t* game )
{
   BinaryPicker_Load( &( game->binaryPicker ),
                      STRING_YES, STRING_NO,
                      Game_WeaponShopPurchaseCallback, Game_WeaponShopNoPurchaseCallback,
                      game, game );
   Game_ChangeSubState( game, SubState_BinaryChoice );
}

internal void Game_WeaponShopPurchaseCallback( Game_t* game )
{
   ShopItem_t* item = &( game->tileMap.shopItems[game->shopPicker.selectedIndex] );

   switch ( item->type )
   {
      case AccessoryType_Weapon: Player_LoadWeapon( &( game->player ), item->id ); break;
      case AccessoryType_Armor: Player_LoadArmor( &( game->player ), item->id ); break;
      case AccessoryType_Shield: Player_LoadShield( &( game->player ), item->id ); break;
   }

   game->player.gold -= item->price;

   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_THANKYOU, Game_WeaponShopViewItemsMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_WeaponShopNoPurchaseCallback( Game_t* game )
{
   Game_ChangeSubState( game, SubState_Dialog );
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_WEAPONSHOP_ANYTHINGELSE, Game_WeaponShopViewItemsMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_LoadWeaponShop( Game_t* game, uint32_t boothId )
{
   TileMap_t* tileMap = &( game->tileMap );

   tileMap->shopType = ShopType_Weapon;

   switch ( boothId )
   {
      case 5: // Brecconary weapon shop
         tileMap->shopItemCount = 6;
         tileMap->shopItems[0].id = WEAPON_BAMBOOPOLE_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 10;
         tileMap->shopItems[1].id = WEAPON_CLUB_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[1].price = 60;
         tileMap->shopItems[2].id = WEAPON_COPPERSWORD_ID;
         tileMap->shopItems[2].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[2].price = 180;
         tileMap->shopItems[3].id = ARMOR_CLOTHES_ID;
         tileMap->shopItems[3].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[3].price = 20;
         tileMap->shopItems[4].id = ARMOR_LEATHERARMOR_ID;
         tileMap->shopItems[4].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[4].price = 70;
         tileMap->shopItems[5].id = SHIELD_SMALLSHIELD_ID;
         tileMap->shopItems[5].type = (uint32_t)AccessoryType_Shield;
         tileMap->shopItems[5].price = 90;
         break;
      case 6: // Garinham weapon shop
         tileMap->shopItemCount = 7;
         tileMap->shopItems[0].id = WEAPON_CLUB_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 60;
         tileMap->shopItems[1].id = WEAPON_COPPERSWORD_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[1].price = 180;
         tileMap->shopItems[2].id = WEAPON_HANDAXE_ID;
         tileMap->shopItems[2].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[2].price = 560;
         tileMap->shopItems[3].id = ARMOR_LEATHERARMOR_ID;
         tileMap->shopItems[3].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[3].price = 70;
         tileMap->shopItems[4].id = ARMOR_CHAINMAIL_ID;
         tileMap->shopItems[4].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[4].price = 300;
         tileMap->shopItems[5].id = ARMOR_HALFPLATE_ID;
         tileMap->shopItems[5].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[5].price = 1000;
         tileMap->shopItems[6].id = SHIELD_LARGESHIELD_ID;
         tileMap->shopItems[6].type = (uint32_t)AccessoryType_Shield;
         tileMap->shopItems[6].price = 800;
         break;
      case 7: // Kol weapon shop
         tileMap->shopItemCount = 5;
         tileMap->shopItems[0].id = WEAPON_COPPERSWORD_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 180;
         tileMap->shopItems[1].id = WEAPON_HANDAXE_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[1].price = 560;
         tileMap->shopItems[2].id = ARMOR_HALFPLATE_ID;
         tileMap->shopItems[2].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[2].price = 1000;
         tileMap->shopItems[3].id = ARMOR_FULLPLATE_ID;
         tileMap->shopItems[3].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[3].price = 3000;
         tileMap->shopItems[4].id = SHIELD_SMALLSHIELD_ID;
         tileMap->shopItems[4].type = (uint32_t)AccessoryType_Shield;
         tileMap->shopItems[4].price = 90;
         break;
      case 8: // Cantlin upper-right weapon shop
         tileMap->shopItemCount = 6;
         tileMap->shopItems[0].id = WEAPON_BAMBOOPOLE_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 10;
         tileMap->shopItems[1].id = WEAPON_CLUB_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[1].price = 60;
         tileMap->shopItems[2].id = WEAPON_COPPERSWORD_ID;
         tileMap->shopItems[2].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[2].price = 180;
         tileMap->shopItems[3].id = ARMOR_LEATHERARMOR_ID;
         tileMap->shopItems[3].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[3].price = 70;
         tileMap->shopItems[4].id = ARMOR_CHAINMAIL_ID;
         tileMap->shopItems[4].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[4].price = 300;
         tileMap->shopItems[5].id = SHIELD_LARGESHIELD_ID;
         tileMap->shopItems[5].type = (uint32_t)AccessoryType_Shield;
         tileMap->shopItems[5].price = 800;
         break;
      case 9: // Cantlin middle-right weapon shop
         tileMap->shopItemCount = 2;
         tileMap->shopItems[0].id = WEAPON_FLAMESWORD_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 9800;
         tileMap->shopItems[1].id = SHIELD_SILVERSHIELD_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Shield;
         tileMap->shopItems[1].price = 14800;
         break;
      case 10: // Cantlin lower-right weapon shop
         tileMap->shopItemCount = 4;
         tileMap->shopItems[0].id = WEAPON_HANDAXE_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 560;
         tileMap->shopItems[1].id = WEAPON_BROADSWORD_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[1].price = 1500;
         tileMap->shopItems[2].id = ARMOR_FULLPLATE_ID;
         tileMap->shopItems[2].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[2].price = 3000;
         tileMap->shopItems[3].id = ARMOR_MAGICARMOR_ID;
         tileMap->shopItems[3].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[3].price = 7700;
         break;
      case 11: // Rimuldar weapon shop
         tileMap->shopItemCount = 6;
         tileMap->shopItems[0].id = WEAPON_COPPERSWORD_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 180;
         tileMap->shopItems[1].id = WEAPON_HANDAXE_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[1].price = 560;
         tileMap->shopItems[2].id = WEAPON_BROADSWORD_ID;
         tileMap->shopItems[2].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[2].price = 1500;
         tileMap->shopItems[3].id = ARMOR_HALFPLATE_ID;
         tileMap->shopItems[3].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[3].price = 1000;
         tileMap->shopItems[4].id = ARMOR_FULLPLATE_ID;
         tileMap->shopItems[4].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[4].price = 3000;
         tileMap->shopItems[5].id = ARMOR_MAGICARMOR_ID;
         tileMap->shopItems[5].type = (uint32_t)AccessoryType_Armor;
         tileMap->shopItems[5].price = 7700;
         break;
   }
}
