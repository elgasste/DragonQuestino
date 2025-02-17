#include "game.h"

internal void Menu_DrawCarat( Menu_t* menu, Screen_t* screen );
internal void Menu_LoadOverworld( Menu_t* menu );
internal void Menu_LoadOverworldItem( Game_t* game );

void Menu_Load( Game_t* game, MenuId_t id )
{
   switch ( id )
   {
      case MenuId_Overworld: Menu_LoadOverworld( &( game->menu ) ); break;
      case MenuId_OverworldItem: Menu_LoadOverworldItem( game ); break;
   }

   game->menu.hasDrawn = False;
   Menu_ResetCarat( &( game->menu ), &( game->screen ) );
}

void Menu_Draw( Menu_t* menu, Screen_t* screen )
{
   uint16_t i;
   uint32_t x, y;
   uint32_t startX = menu->position.x + ( ( menu->borderPadding.x + 1 ) * ( TEXT_TILE_SIZE ) );
   uint32_t startY = menu->position.y + ( ( menu->borderPadding.y + 1 ) * ( TEXT_TILE_SIZE ) );

   if ( !menu->hasDrawn )
   {
      Screen_DrawTextWindowWithTitle( screen, menu->position.x, menu->position.y, menu->borderSize.x, menu->borderSize.y, menu->title, COLOR_WHITE );

      for ( i = 0; i < menu->itemCount; i++ )
      {
         x = startX + ( ( i / menu->itemsPerColumn ) * ( menu->columnWidth * TEXT_TILE_SIZE ) );
         y = startY + ( ( i % menu->itemsPerColumn ) * ( TEXT_TILE_SIZE * ( menu->itemPadding + 1 ) ) );

         Screen_DrawText( screen, menu->items[i].text, x, y, COLOR_WHITE );

         if ( menu->items[i].twoLineText )
         {
            Screen_DrawText( screen, menu->items[i].text + MENU_LINE_LENGTH, x, y + TEXT_TILE_SIZE, COLOR_WHITE );
         }
      }

      menu->hasDrawn = True;
   }

   Menu_DrawCarat( menu, screen );
}

void Menu_ResetCarat( Menu_t* menu, Screen_t* screen )
{
   menu->showCarat = True;
   menu->blinkSeconds = 0.0f;
   Menu_DrawCarat( menu, screen );
}

void Menu_MoveSelection( Menu_t* menu, Direction_t direction, Screen_t* screen )
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
   Menu_ResetCarat( menu, screen );
}

void Menu_Tic( Menu_t* menu )
{
   menu->blinkSeconds += CLOCK_FRAME_SECONDS;

   while ( menu->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
   {
      menu->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      TOGGLE_BOOL( menu->showCarat );
   }
}

internal void Menu_DrawCarat( Menu_t* menu, Screen_t* screen )
{
   uint32_t i;
   uint32_t x, y;
   uint32_t startX = menu->position.x + ( ( menu->borderPadding.x + 1 ) * ( TEXT_TILE_SIZE ) );
   uint32_t startY = menu->position.y + ( ( menu->borderPadding.y + 1 ) * ( TEXT_TILE_SIZE ) );

   for ( i = 0; i < menu->itemCount; i++ )
   {
      x = startX + ( ( i / menu->itemsPerColumn ) * ( menu->columnWidth * TEXT_TILE_SIZE ) );
      y = startY + ( ( i % menu->itemsPerColumn ) * ( TEXT_TILE_SIZE * ( menu->itemPadding + 1 ) ) );

      if ( i == menu->selectedIndex && menu->showCarat )
      {
         Screen_DrawChar( screen, '>', x - ( TEXT_TILE_SIZE * menu->caratOffset ), y, COLOR_WHITE );
      }
      else
      {
         Screen_DrawChar( screen, ' ', x - ( TEXT_TILE_SIZE * menu->caratOffset ), y, COLOR_WHITE );
      }
   }
}

internal void Menu_LoadOverworld( Menu_t* menu )
{
   uint32_t i;

   sprintf( menu->title, STRING_OVERWORLD_MENU_TITLE );
   sprintf( menu->items[0].text, STRING_OVERWORLD_MENU_TALK );
   sprintf( menu->items[1].text, STRING_OVERWORLD_MENU_STATUS );
   sprintf( menu->items[2].text, STRING_OVERWORLD_MENU_SEARCH );
   sprintf( menu->items[3].text, STRING_OVERWORLD_MENU_SPELL );
   sprintf( menu->items[4].text, STRING_OVERWORLD_MENU_ITEM );
   sprintf( menu->items[5].text, STRING_OVERWORLD_MENU_DOOR );
   menu->items[0].command = MenuCommand_OverworldMain_Talk;
   menu->items[1].command = MenuCommand_OverworldMain_Status;
   menu->items[2].command = MenuCommand_OverworldMain_Search;
   menu->items[3].command = MenuCommand_OverworldMain_Spell;
   menu->items[4].command = MenuCommand_OverworldMain_Item;
   menu->items[5].command = MenuCommand_OverworldMain_Door;
   menu->itemCount = 6;
   for ( i = 0; i < menu->itemCount; i++ ) menu->items[i].twoLineText = False;
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
   menu->caratOffset = 1;
}

internal void Menu_LoadOverworldItem( Game_t* game )
{
   uint32_t curItem = 0;
   Menu_t* menu = &( game->menu );
   uint32_t items = game->player.items;

   sprintf( menu->title, STRING_OVERWORLD_MENU_ITEM );

   menu->itemCount = PLAYER_GET_MAPUSEABLEITEMCOUNT( items );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 144;
   menu->position.y = 48;
   menu->borderSize.x = 12;
   menu->borderSize.y = (uint16_t)( ( menu->itemCount * 2 ) + 3 );
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 1;
   menu->columnWidth = 10;
   menu->itemPadding = 1;
   menu->caratOffset = 1;

   if ( PLAYER_GET_HERBCOUNT( items ) )
   {
      menu->items[curItem].twoLineText = False;
      sprintf( menu->items[curItem].text, STRING_OVERWORLD_ITEMMENU_HERB, PLAYER_GET_HERBCOUNT(items));
      menu->items[curItem].command = MenuCommand_OverworldItem_Herb;
      curItem++;
   }
   if ( PLAYER_GET_WINGCOUNT( items ) )
   {
      menu->items[curItem].twoLineText = False;
      sprintf( menu->items[curItem].text, STRING_OVERWORLD_ITEMMENU_WING, PLAYER_GET_WINGCOUNT(items));
      menu->items[curItem].command = MenuCommand_OverworldItem_Herb;
      curItem++;
   }
   if ( PLAYER_GET_FAIRYWATERCOUNT( items ) )
   {
      menu->items[curItem].twoLineText = True;
      sprintf( menu->items[curItem].text, STRING_OVERWORLD_ITEMMENU_FAIRYWATER_1, PLAYER_GET_FAIRYWATERCOUNT(items));
      sprintf( menu->items[curItem].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_FAIRYWATER_2);
      menu->items[curItem].command = MenuCommand_OverworldItem_FairyWater;
      curItem++;
   }
   if ( PLAYER_HAS_SILVERHARP( items ) )
   {
      menu->items[curItem].twoLineText = True;
      sprintf( menu->items[curItem].text, STRING_OVERWORLD_ITEMMENU_SILVERHARP_1 );
      sprintf( menu->items[curItem].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_SILVERHARP_2 );
      menu->items[curItem].command = MenuCommand_OverworldItem_SilverHarp;
      curItem++;
   }
   if ( PLAYER_HAS_FAIRYFLUTE( items ) )
   {
      menu->items[curItem].twoLineText = True;
      sprintf( menu->items[curItem].text, STRING_OVERWORLD_ITEMMENU_FAIRYFLUTE_1 );
      sprintf( menu->items[curItem].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_FAIRYFLUTE_2 );
      menu->items[curItem].command = MenuCommand_OverworldItem_FairyFlute;
      curItem++;
   }
   if ( PLAYER_HAS_GWAELYNSLOVE( items ) )
   {
      menu->items[curItem].twoLineText = True;
      sprintf( menu->items[curItem].text, STRING_OVERWORLD_ITEMMENU_GWAELYNSLOVE_1 );
      sprintf( menu->items[curItem].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_GWAELYNSLOVE_2 );
      menu->items[curItem].command = MenuCommand_OverworldItem_GwaelynsLove;
      curItem++;
   }
   if ( PLAYER_HAS_RAINBOWDROP( items ) )
   {
      menu->items[curItem].twoLineText = True;
      sprintf( menu->items[curItem].text, STRING_OVERWORLD_ITEMMENU_RAINBOWDROP_1 );
      sprintf( menu->items[curItem].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_RAINBOWDROP_2 );
      menu->items[curItem].command = MenuCommand_OverworldItem_RainbowDrop;
      curItem++;
   }
}
