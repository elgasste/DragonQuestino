#include "menu.h"
#include "screen.h"
#include "clock.h"

internal void Menu_LoadOverworld( Menu_t* menu );

void Menu_Load( Menu_t* menu, MenuId_t id )
{
   switch ( id )
   {
      case MenuId_Overworld:
         Menu_LoadOverworld( menu );
         break;
   }

   Menu_ResetCursor( menu );
}

void Menu_Draw( Menu_t* menu, Screen_t* screen )
{
   uint16_t i;
   uint32_t x, y, startX, startY;
   char line[32];
   memset( line, 0, sizeof( char ) * 32 );

   Screen_DrawTextWindowWithTitle( screen, menu->position.x, menu->position.y, menu->borderSize.x, menu->borderSize.y, menu->title, COLOR_WHITE );

   startX = menu->position.x + ( ( menu->borderPadding.x + 1 ) * ( TEXT_TILE_SIZE ) );
   startY = menu->position.y + ( ( menu->borderPadding.y + 1 ) * ( TEXT_TILE_SIZE ) );

   // menu items
   for ( i = 0; i < menu->itemCount; i++ )
   {
      x = startX + ( ( i / menu->itemsPerColumn ) * ( menu->columnWidth * TEXT_TILE_SIZE ) );
      y = startY + ( ( i % menu->itemsPerColumn ) * ( TEXT_TILE_SIZE * ( menu->itemPadding + 1 ) ) );
      Screen_DrawText( screen, menu->items[i].text, x, y, COLOR_WHITE );

      if ( i == menu->selectedIndex && menu->showCursor )
      {
         Screen_DrawChar( screen, '>', x - ( TEXT_TILE_SIZE * menu->cursorOffset ), y, COLOR_WHITE );
      }
   }
}

void Menu_ResetCursor( Menu_t* menu )
{
   menu->showCursor = True;
   menu->blinkSeconds = 0.0f;
}

void Menu_MoveSelection( Menu_t* menu, Direction_t direction )
{
   int32_t newIndex = 0;
   uint32_t column;

   switch ( direction )
   {
      case Direction_Left:
         newIndex = (int32_t)menu->selectedIndex - menu->itemsPerColumn;
         if ( newIndex < 0 )
         {
            newIndex = menu->itemCount + newIndex;
         }
         break;
      case Direction_Right:
         newIndex = (int32_t)menu->selectedIndex + menu->itemsPerColumn;
         if ( newIndex >= (int32_t)( menu->itemCount ) )
         {
            newIndex = menu->itemCount - newIndex;
         }
         break;
      case Direction_Up:
         column = menu->selectedIndex / menu->itemsPerColumn;
         newIndex = (int32_t)( menu->selectedIndex ) - 1;
         if ( newIndex < 0 )
         {
            newIndex = menu->itemsPerColumn - 1;
         }
         else
         {
            if ( ( newIndex / menu->itemsPerColumn ) != column )
            {
               newIndex += menu->itemsPerColumn;
            }
         }
         break;
      case Direction_Down:
         column = menu->selectedIndex / menu->itemsPerColumn;
         newIndex = (int32_t)( menu->selectedIndex ) + 1;
         if ( newIndex >= (int32_t)( menu->itemCount ) )
         {
            newIndex = menu->itemCount - menu->itemsPerColumn;
         }
         else
         {
            if ( ( newIndex / menu->itemsPerColumn ) != column )
            {
               newIndex -= menu->itemsPerColumn;
            }
         }
         break;
   }

   menu->selectedIndex = (uint32_t)newIndex;
   Menu_ResetCursor( menu );
}

void Menu_Tic( Menu_t* menu )
{
   menu->blinkSeconds += CLOCK_FRAME_SECONDS;

   while ( menu->blinkSeconds > MENU_BLINK_RATE_SECONDS )
   {
      menu->blinkSeconds -= MENU_BLINK_RATE_SECONDS;
      TOGGLE_BOOL( menu->showCursor );
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
   menu->items[0].command = MenuCommand_Overworld_Talk;
   menu->items[1].command = MenuCommand_Overworld_Status;
   menu->items[2].command = MenuCommand_Overworld_Search;
   menu->items[3].command = MenuCommand_Overworld_Spell;
   menu->items[4].command = MenuCommand_Overworld_Item;
   menu->items[5].command = MenuCommand_Overworld_Door;
   menu->itemCount = 6;
   menu->itemsPerColumn = 3;
   menu->selectedIndex = 0;

   menu->position.x = 96;
   menu->position.y = 16;
   menu->borderSize.x = 16;
   menu->borderSize.y = 8;
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 1;
   menu->columnWidth = 8;
   menu->itemPadding = 1;
   menu->cursorOffset = 1;
}
