#if !defined( SHOP_PICKER_H )
#define SHOP_PICKER_H

#include "common.h"
#include "vector.h"
#include "tile_map.h"

#define SHOP_ITEM_TEXTLINE_LENGTH   10

typedef struct Screen_t Screen_t;

typedef struct ShopPickerItemText_t
{
   char line1[SHOP_ITEM_TEXTLINE_LENGTH];
   char line2[SHOP_ITEM_TEXTLINE_LENGTH];
   Bool_t hasTwoLines;
}
ShopPickerItemText_t;

typedef struct ShopPickerItem_t
{
   ShopPickerItemText_t itemText;
   char priceText[SHOP_ITEM_TEXTLINE_LENGTH];
}
ShopPickerItem_t;

typedef struct ShopPicker_t
{
   Screen_t* screen;
   TileMap_t* tileMap;

   ShopPickerItem_t items[TILEMAP_MAX_SHOPITEMS];
   u32 itemCount;

   u32 selectedIndex;
   Vector2u32_t position;        // position on the screen, in pixels
   Bool_t showCarat;
   float blinkSeconds;
}
ShopPicker_t;

#if defined( __cplusplus )
extern "C" {
#endif

   void ShopPicker_Init( ShopPicker_t* picker, Screen_t* screen, TileMap_t* tileMap );
   void ShopPicker_Reset( ShopPicker_t* picker );
   void ShopPicker_Draw( ShopPicker_t* picker );
   void ShopPicker_ResetCarat( ShopPicker_t* picker );
   void ShopPicker_MoveSelection( ShopPicker_t* picker, Direction_t direction );
   void ShopPicker_Tic( ShopPicker_t* picker );

#if defined( __cplusplus )
}
#endif

#endif // SHOP_PICKER_H
