#include "menu.h"
#include "screen.h"
#include "player.h"
#include "clock.h"
#include "tile_map.h"

internal void Menu_DrawCarat( Menu_t* menu );
internal void Menu_LoadOverworld( Menu_t* menu );
internal void Menu_LoadOverworldSpell( Menu_t* menu );
internal void Menu_LoadOverworldItem( Menu_t* menu );
internal void Menu_LoadZoom( Menu_t* menu );

void Menu_Init( Menu_t* menu, Screen_t* screen, Player_t* player )
{
   menu->screen = screen;
   menu->player = player;
}

void Menu_Load( Menu_t* menu, MenuId_t id )
{
   switch ( id )
   {
      case MenuId_Overworld: Menu_LoadOverworld( menu ); break;
      case MenuId_OverworldSpell: Menu_LoadOverworldSpell( menu ); break;
      case MenuId_OverworldItem: Menu_LoadOverworldItem( menu ); break;
      case MenuId_Zoom: Menu_LoadZoom( menu ); break;
   }

   menu->hasDrawn = False;
   Menu_ResetCarat( menu );
}

void Menu_Draw( Menu_t* menu )
{
   uint16_t i;
   uint32_t x, y;
   uint32_t startX = menu->position.x + ( ( menu->borderPadding.x + 1 ) * ( TEXT_TILE_SIZE ) );
   uint32_t startY = menu->position.y + ( ( menu->borderPadding.y + 1 ) * ( TEXT_TILE_SIZE ) );

   if ( !menu->hasDrawn )
   {
      Screen_DrawTextWindowWithTitle( menu->screen, menu->position.x, menu->position.y, menu->borderSize.x, menu->borderSize.y, menu->title );

      for ( i = 0; i < menu->itemCount; i++ )
      {
         x = startX + ( ( i / menu->itemsPerColumn ) * ( menu->columnWidth * TEXT_TILE_SIZE ) );
         y = startY + ( ( i % menu->itemsPerColumn ) * ( TEXT_TILE_SIZE * ( menu->itemPadding + 1 ) ) );

         Screen_DrawText( menu->screen, menu->items[i].text, x, y );

         if ( menu->items[i].twoLineText )
         {
            Screen_DrawText( menu->screen, menu->items[i].text + MENU_LINE_LENGTH, x, y + TEXT_TILE_SIZE );
         }
      }

      menu->hasDrawn = True;
   }

   Menu_DrawCarat( menu );
}

void Menu_ResetCarat( Menu_t* menu )
{
   menu->showCarat = True;
   menu->blinkSeconds = 0.0f;
   Menu_DrawCarat( menu );
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
            newIndex = menu->itemsPerColumn - ( menu->itemCount - menu->selectedIndex );
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
   Menu_ResetCarat( menu );
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

internal void Menu_DrawCarat( Menu_t* menu )
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
         Screen_DrawChar( menu->screen, '>', x - ( TEXT_TILE_SIZE * menu->caratOffset ), y );
      }
      else
      {
         Screen_DrawChar( menu->screen, ' ', x - ( TEXT_TILE_SIZE * menu->caratOffset ), y );
      }
   }
}

internal void Menu_LoadOverworld( Menu_t* menu )
{
   uint32_t i;

   strcpy( menu->title, STRING_OVERWORLD_MENU_TITLE );
   strcpy( menu->items[0].text, STRING_OVERWORLD_MENU_TALK );
   strcpy( menu->items[1].text, STRING_OVERWORLD_MENU_STATUS );
   strcpy( menu->items[2].text, STRING_OVERWORLD_MENU_SEARCH );
   strcpy( menu->items[3].text, STRING_OVERWORLD_MENU_SPELL );
   strcpy( menu->items[4].text, STRING_OVERWORLD_MENU_ITEM );
   strcpy( menu->items[5].text, STRING_OVERWORLD_MENU_DOOR );
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

internal void Menu_LoadOverworldSpell( Menu_t* menu )
{
   uint32_t i;
   uint32_t spells = menu->player->spells;

   strcpy( menu->title, STRING_OVERWORLD_MENU_SPELL );

   menu->itemCount = SPELL_GET_MAPUSEABLECOUNT( spells, menu->player->tileMap->isDungeon, menu->player->tileMap->isDark );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 152;
   menu->position.y = 32;
   menu->borderSize.x = 12;
   menu->borderSize.y = (uint16_t)( ( menu->itemCount * 2 ) + 3 );
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 1;
   menu->columnWidth = 10;
   menu->itemPadding = 1;
   menu->caratOffset = 1;

   for ( i = 0; i < menu->itemCount; i++ )
   {
      menu->items[i].twoLineText = False;
   }

   i = 0;

   if ( SPELL_HAS_HEAL( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELLMENU_HEAL );
      menu->items[i].command = MenuCommand_Spell_Heal;
      i++;
   }
   if ( menu->player->tileMap->isDark && SPELL_HAS_GLOW( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELLMENU_GLOW );
      menu->items[i].command = MenuCommand_Spell_Glow;
      i++;
   }
   if ( menu->player->tileMap->isDungeon && SPELL_HAS_EVAC( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELLMENU_EVAC );
      menu->items[i].command = MenuCommand_Spell_Evac;
      i++;
   }
   if ( SPELL_HAS_ZOOM( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELLMENU_ZOOM );
      menu->items[i].command = MenuCommand_Spell_Zoom;
      i++;
   }
   if ( SPELL_HAS_REPEL( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELLMENU_REPEL );
      menu->items[i].command = MenuCommand_Spell_Repel;
      i++;
   }
   if ( SPELL_HAS_MIDHEAL( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELLMENU_MIDHEAL );
      menu->items[i].command = MenuCommand_Spell_Midheal;
   }
}

internal void Menu_LoadOverworldItem( Menu_t* menu )
{
   uint32_t i = 0;
   uint32_t items = menu->player->items;

   strcpy( menu->title, STRING_OVERWORLD_MENU_ITEM );

   menu->itemCount = ITEM_GET_MAPUSEABLECOUNT( items );
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

   if ( ITEM_GET_HERBCOUNT( items ) )
   {
      menu->items[i].twoLineText = False;
      sprintf( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_HERB, ITEM_GET_HERBCOUNT( items ) );
      menu->items[i].command = MenuCommand_Item_Herb;
      i++;
   }
   if ( ITEM_GET_TORCHCOUNT( items ) )
   {
      menu->items[i].twoLineText = False;
      sprintf( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_TORCH, ITEM_GET_TORCHCOUNT( items ) );
      menu->items[i].command = MenuCommand_Item_Torch;
      i++;
   }
   if ( ITEM_GET_WINGCOUNT( items ) )
   {
      menu->items[i].twoLineText = False;
      sprintf( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_WING, ITEM_GET_WINGCOUNT( items ) );
      menu->items[i].command = MenuCommand_Item_Wing;
      i++;
   }
   if ( ITEM_GET_FAIRYWATERCOUNT( items ) )
   {
      menu->items[i].twoLineText = True;
      sprintf( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_FAIRYWATER_1, ITEM_GET_FAIRYWATERCOUNT( items ) );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_FAIRYWATER_2 );
      menu->items[i].command = MenuCommand_Item_FairyWater;
      i++;
   }
   if ( ITEM_HAS_FAIRYFLUTE( items ) )
   {
      menu->items[i].twoLineText = True;
      strcpy( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_FAIRYFLUTE_1 );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_FAIRYFLUTE_2 );
      menu->items[i].command = MenuCommand_Item_FairyFlute;
      i++;
   }
   if ( ITEM_HAS_SILVERHARP( items ) )
   {
      menu->items[i].twoLineText = True;
      strcpy( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_SILVERHARP_1 );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_SILVERHARP_2 );
      menu->items[i].command = MenuCommand_Item_SilverHarp;
      i++;
   }
   if ( ITEM_HAS_GWAELYNSLOVE( items ) )
   {
      menu->items[i].twoLineText = True;
      strcpy( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_GWAELINSLOVE_1 );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_GWAELINSLOVE_2 );
      menu->items[i].command = MenuCommand_Item_GwaelynsLove;
      i++;
   }
   if ( ITEM_HAS_RAINBOWDROP( items ) )
   {
      menu->items[i].twoLineText = True;
      strcpy( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_RAINBOWDROP_1 );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_RAINBOWDROP_2 );
      menu->items[i].command = MenuCommand_Item_RainbowDrop;
      i++;
   }
   if ( ITEM_HAS_CURSEDBELT( items ) )
   {
      menu->items[i].twoLineText = True;
      strcpy( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_CURSEDBELT_1 );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_CURSEDBELT_2 );
      menu->items[i].command = MenuCommand_Item_CursedBelt;
   }
}

internal void Menu_LoadZoom( Menu_t* menu )
{
   uint32_t i = 0;
   uint8_t tv = menu->player->townsVisited;

   strcpy( menu->title, STRING_SPELLMENU_ZOOM );

   menu->itemCount = HAS_VISITED_COUNT( tv );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 136;
   menu->position.y = 64;
   menu->borderSize.x = 13;
   menu->borderSize.y = (uint16_t)( ( menu->itemCount * 2 ) + 3 );
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 1;
   menu->columnWidth = 10;
   menu->itemPadding = 1;
   menu->caratOffset = 1;

   for ( i = 0; i < menu->itemCount; i++ )
   {
      menu->items[i].twoLineText = False;
   }

   i = 0;

   if ( HAS_VISITED_TANTEGEL( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_TANTEGEL );
      menu->items[i].command = MenuCommand_Zoom_Tantegel;
      i++;
   }
   if ( HAS_VISITED_BRECCONARY( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_BRECCONARY );
      menu->items[i].command = MenuCommand_Zoom_Brecconary;
      i++;
   }
   if ( HAS_VISITED_GARINHAM( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_GARINHAM );
      menu->items[i].command = MenuCommand_Zoom_Garinham;
      i++;
   }
   if ( HAS_VISITED_KOL( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_KOL );
      menu->items[i].command = MenuCommand_Zoom_Kol;
      i++;
   }
   if ( HAS_VISITED_CANTLIN( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_CANTLIN );
      menu->items[i].command = MenuCommand_Zoom_Cantlin;
      i++;
   }
   if ( HAS_VISITED_RIMULDAR( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_RIMULDAR );
      menu->items[i].command = MenuCommand_Zoom_Rimuldar;
   }
}
