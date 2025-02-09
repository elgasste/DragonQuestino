#include "menu.h"

internal void Menu_LoadOverworld( Menu_t* menu );

void Menu_Load( Menu_t* menu, MenuId_t id )
{
   switch ( id )
   {
      case MenuId_Overworld:
         Menu_LoadOverworld( menu );
         break;
   }
}

internal void Menu_LoadOverworld( Menu_t* menu )
{
   sprintf( menu->title, STRING_OVERWORLD_MENU_TITLE );
   sprintf( menu->items[0].text, STRING_OVERWORLD_MENU_TALK );
   sprintf( menu->items[1].text, STRING_OVERWORLD_MENU_STATUS );
   sprintf( menu->items[2].text, STRING_OVERWORLD_MENU_SEARCH );
   sprintf( menu->items[3].text, STRING_OVERWORLD_MENU_SPELL );
   sprintf( menu->items[4].text, STRING_OVERWORLD_MENU_ITEM );
   sprintf( menu->items[5].text, STRING_OVERWORLD_MENU_DOOR );
   menu->itemCount = 6;
   menu->columnCount = 2;
   menu->selectedIndex = 0;

   menu->position.x = 96;
   menu->position.y = 16;
   menu->borderSize.x = 16;
   menu->borderSize.y = 10;
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 1;
   menu->itemPadding = 1;
}
