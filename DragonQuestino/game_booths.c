#include "game.h"

internal void Game_VisitWeaponShop( Game_t* game );
internal void Game_WeaponShopLeaveOrStayCallback( Game_t* game );
internal void Game_WeaponShopViewItemsCallback( Game_t* game );
internal void Game_WeaponShopViewItemsMessageCallback( Game_t* game );
internal void Game_WeaponShopLeaveCallback( Game_t* game );
internal void Game_LoadWeaponShop( Game_t* game, uint32_t boothId );

void Game_ActivateBooth( Game_t* game, uint32_t boothId )
{
   if ( boothId <= 6 )
   {
      Game_LoadWeaponShop( game, boothId );
      Game_VisitWeaponShop( game );
   }
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

internal void Game_LoadWeaponShop( Game_t* game, uint32_t boothId )
{
   TileMap_t* tileMap = &( game->tileMap );

   switch ( boothId )
   {
      case 0: // Brecconary weapon shop
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
      case 1: // Garinham weapon shop
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
      case 2: // Kol weapon shop
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
      case 3: // Cantlin upper-right weapon shop
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
      case 4: // Cantlin middle-right weapon shop
         tileMap->shopItemCount = 2;
         tileMap->shopItems[0].id = WEAPON_FLAMESWORD_ID;
         tileMap->shopItems[0].type = (uint32_t)AccessoryType_Weapon;
         tileMap->shopItems[0].price = 9800;
         tileMap->shopItems[1].id = SHIELD_SILVERSHIELD_ID;
         tileMap->shopItems[1].type = (uint32_t)AccessoryType_Shield;
         tileMap->shopItems[1].price = 14800;
         break;
      case 5: // Cantlin lower-right weapon shop
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
      case 6: // Rimuldar weapon shop
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
