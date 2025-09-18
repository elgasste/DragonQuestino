#include "shop_picker.h"
#include "screen.h"
#include "clock.h"
#include "player.h"

internal void ShopPicker_DrawCarat( ShopPicker_t* picker );
internal void ShopPicker_LoadItems( ShopPicker_t* picker );
internal void ShopPicker_LoadWeaponItemText( ShopPickerItemText_t* itemText, u32 itemId, AccessoryType_t type );
internal void ShopPicker_LoadItemItemText( ShopPickerItemText_t* itemText, u32 itemId );

void ShopPicker_Init( ShopPicker_t* picker, Screen_t* screen, TileMap_t* tileMap )
{
   picker->screen = screen;
   picker->tileMap = tileMap;

   picker->position.x = TILE_SIZE * 6;
   picker->position.y = TILE_SIZE;
}

void ShopPicker_Reset( ShopPicker_t* picker )
{
   ShopPicker_LoadItems( picker );
   picker->selectedIndex = 0;
   ShopPicker_ResetCarat( picker );
}

void ShopPicker_Draw( ShopPicker_t* picker )
{
   u32 i, dx;
   u32 x = picker->position.x + ( TEXT_TILE_SIZE * 2 );
   u32 y = picker->position.y + TEXT_TILE_SIZE;

   Screen_DrawTextWindow( picker->screen, picker->position.x, picker->position.y, 16, 2 + ( picker->itemCount * 2 ) );

   for ( i = 0; i < picker->itemCount; i++ )
   {
      Screen_DrawText( picker->screen, picker->items[i].itemText.line1, x, y );

      if ( picker->items[i].itemText.hasTwoLines )
      {
         Screen_DrawText( picker->screen, picker->items[i].itemText.line2, x + TEXT_TILE_SIZE, y + TEXT_TILE_SIZE );
      }

      dx = ( strlen( picker->items[i].priceText ) < 5 ) ? 9 : 8;
      Screen_DrawText( picker->screen, picker->items[i].priceText, x + ( TEXT_TILE_SIZE * dx ), y );
      y += ( TEXT_TILE_SIZE * 2 );
   }

   ShopPicker_DrawCarat( picker );
}

void ShopPicker_ResetCarat( ShopPicker_t* picker )
{
   picker->showCarat = True;
   picker->blinkSeconds = 0.0f;
   ShopPicker_DrawCarat( picker );
}

void ShopPicker_MoveSelection( ShopPicker_t* picker, Direction_t direction )
{
   if ( direction == Direction_Up )
   {
      picker->selectedIndex = ( picker->selectedIndex == 0 ) ? ( picker->itemCount - 1 ) : ( picker->selectedIndex - 1 );
   }
   else if ( direction == Direction_Down )
   {
      picker->selectedIndex = ( picker->selectedIndex == ( picker->itemCount - 1 ) ) ? 0 : ( picker->selectedIndex + 1 );
   }

   ShopPicker_ResetCarat( picker );
}

void ShopPicker_Tic( ShopPicker_t* picker )
{
   picker->blinkSeconds += CLOCK_FRAME_SECONDS;

   while ( picker->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
   {
      picker->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      TOGGLE_BOOL( picker->showCarat );
   }
}

internal void ShopPicker_DrawCarat( ShopPicker_t* picker )
{
   u32 i;
   u32 x = picker->position.x + TEXT_TILE_SIZE;
   u32 y = picker->position.y + TEXT_TILE_SIZE;

   for ( i = 0; i < picker->itemCount; i++ )
   {
      if ( ( i == picker->selectedIndex ) && picker->showCarat )
      {
         Screen_DrawChar( picker->screen, '>', x, y );
      }
      else
      {
         Screen_DrawChar( picker->screen, ' ', x, y );
      }

      y += ( TEXT_TILE_SIZE * 2 );
   }
}

internal void ShopPicker_LoadItems( ShopPicker_t* picker )
{
   u32 i;
   u16 price;

   picker->itemCount = picker->tileMap->shopItemCount;

   for ( i = 0; i < picker->itemCount; i++ )
   {
      if ( picker->tileMap->shopType == ShopType_Weapon )
      {
         ShopPicker_LoadWeaponItemText( &( picker->items[i].itemText ), picker->tileMap->shopItems[i].id, picker->tileMap->shopItems[i].type );
      }
      else
      {
         ShopPicker_LoadItemItemText( &( picker->items[i].itemText ), picker->tileMap->shopItems[i].id );
      }

      price = picker->tileMap->shopItems[i].price;
      sprintf( picker->items[i].priceText, price < 10 ? "   %u" : price < 100 ? "  %u" : price < 1000 ? " %u" : "%u", picker->tileMap->shopItems[i].price );
   }
}

internal void ShopPicker_LoadWeaponItemText( ShopPickerItemText_t* itemText, u32 itemId, AccessoryType_t type )
{
   switch ( type )
   {
      case AccessoryType_Weapon:
         switch ( itemId )
         {
            case WEAPON_BAMBOOPOLE_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_WEAPON_BAMBOOPOLE1 );
               strcpy( itemText->line2, STRING_WEAPON_BAMBOOPOLE2 );
               break;
            case WEAPON_CLUB_ID:
               itemText->hasTwoLines = False;
               strcpy( itemText->line1, STRING_WEAPON_CLUB );
               break;
            case WEAPON_COPPERSWORD_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_WEAPON_COPPERSWORD1 );
               strcpy( itemText->line2, STRING_WEAPON_COPPERSWORD2 );
               break;
            case WEAPON_HANDAXE_ID:
               itemText->hasTwoLines = False;
               strcpy( itemText->line1, STRING_WEAPON_HANDAXE );
               break;
            case WEAPON_BROADSWORD_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_WEAPON_BROADSWORD1 );
               strcpy( itemText->line2, STRING_WEAPON_BROADSWORD2 );
               break;
            case WEAPON_FLAMESWORD_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_WEAPON_FLAMESWORD1 );
               strcpy( itemText->line2, STRING_WEAPON_FLAMESWORD2 );
               break;
            default:
               itemText->line1[0] = 0;
               itemText->hasTwoLines = False;
               break;
         }
         break;
      case AccessoryType_Armor:
         switch ( itemId )
         {
            case ARMOR_CLOTHES_ID:
               itemText->hasTwoLines = False;
               strcpy( itemText->line1, STRING_ARMOR_CLOTHES );break;
            case ARMOR_LEATHERARMOR_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_ARMOR_LEATHERARMOR1 );
               strcpy( itemText->line2, STRING_ARMOR_LEATHERARMOR2 );
               break;
            case ARMOR_CHAINMAIL_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_ARMOR_CHAINMAIL1 );
               strcpy( itemText->line2, STRING_ARMOR_CHAINMAIL2 );
               break;
            case ARMOR_HALFPLATE_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_ARMOR_HALFPLATE1 );
               strcpy( itemText->line2, STRING_ARMOR_HALFPLATE2 );
               break;
            case ARMOR_FULLPLATE_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_ARMOR_FULLPLATE1 );
               strcpy( itemText->line2, STRING_ARMOR_FULLPLATE2 );
               break;
            case ARMOR_MAGICARMOR_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_ARMOR_MAGICARMOR1 );
               strcpy( itemText->line2, STRING_ARMOR_MAGICARMOR2 );break;
            default:
               itemText->line1[0] = 0;
               itemText->hasTwoLines = False;
               break;
         }
         break;
      case AccessoryType_Shield:
         switch ( itemId )
         {
            case SHIELD_SMALLSHIELD_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_SHIELD_SMALL1 );
               strcpy( itemText->line2, STRING_SHIELD_SMALL2 );
               break;
            case SHIELD_LARGESHIELD_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_SHIELD_LARGE1 );
               strcpy( itemText->line2, STRING_SHIELD_LARGE2 );
               break;
            case SHIELD_SILVERSHIELD_ID:
               itemText->hasTwoLines = True;
               strcpy( itemText->line1, STRING_SHIELD_SILVER1 );
               strcpy( itemText->line2, STRING_SHIELD_SILVER2 );
               break;
            default:
               itemText->line1[0] = 0;
               itemText->hasTwoLines = False;
               break;
         }
         break;
      default:
         itemText->line1[0] = 0;
         itemText->hasTwoLines = False;
         break;
   }
}

internal void ShopPicker_LoadItemItemText( ShopPickerItemText_t* itemText, u32 itemId )
{
   switch ( itemId )
   {
      case ITEM_KEY_ID:
         itemText->hasTwoLines = False;
         strcpy( itemText->line1, STRING_ITEM_KEY );
         break;
      case ITEM_HERB_ID:
         itemText->hasTwoLines = False;
         strcpy( itemText->line1, STRING_ITEM_HERB );
         break;
      case ITEM_WING_ID:
         itemText->hasTwoLines = False;
         strcpy( itemText->line1, STRING_ITEM_WING );
         break;
      case ITEM_FAIRYWATER_ID:
         itemText->hasTwoLines = True;
         strcpy( itemText->line1, STRING_ITEM_FAIRYWATER1 );
         strcpy( itemText->line2, STRING_ITEM_FAIRYWATER2 );
         break;
      case ITEM_TORCH_ID:
         itemText->hasTwoLines = False;
         strcpy( itemText->line1, STRING_ITEM_TORCH );
         break;
      case ITEM_DRAGONSCALE_ID:
         itemText->hasTwoLines = True;
         strcpy( itemText->line1, STRING_ITEM_DRAGONSCALE1 );
         strcpy( itemText->line2, STRING_ITEM_DRAGONSCALE2 );
         break;
   }
}
