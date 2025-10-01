#include "menu.h"
#include "screen.h"
#include "player.h"
#include "clock.h"
#include "tile_map.h"

internal void Menu_DrawCarat( Menu_t* menu );
internal void Menu_InitStartup( Menu_t* menu );
internal void Menu_InitOverworld( Menu_t* menu );
internal void Menu_InitBattle( Menu_t* menu );
internal void Menu_Update( Menu_t* menu );
internal void Menu_UpdateOverworldSpell( Menu_t* menu );
internal void Menu_UpdateOverworldItem( Menu_t* menu );
internal void Menu_UpdateZoom( Menu_t* menu );
internal void Menu_UpdateSellItem( Menu_t* menu );
internal void Menu_UpdateBattleSpell( Menu_t* menu );
internal void Menu_UpdateBattleItem( Menu_t* menu );

void Menu_Init( Menu_t* menu, MenuId_t id, Screen_t* screen, Player_t* player, TileMap_t* tileMap )
{
   menu->screen = screen;
   menu->player = player;
   menu->tileMap = tileMap;
   menu->id = id;

   switch ( id )
   {
      case MenuId_Startup:
         Menu_InitStartup( menu );
         break;
      case MenuId_Overworld:
         Menu_InitOverworld( menu );
         break;
      case MenuId_Battle:
         Menu_InitBattle( menu );
         break;
      default:
         Menu_Update( menu );
         break;
   }
}

void Menu_Reset( Menu_t* menu )
{
   menu->selectedIndex = 0;
   Menu_Update( menu );
   Menu_ResetCarat( menu );
}

void Menu_Draw( Menu_t* menu )
{
   u16 i;
   u32 x, y;
   u32 startX = menu->position.x + ( ( menu->borderPadding.x + 1 ) * ( TEXT_TILE_SIZE ) );
   u32 startY = menu->position.y + ( ( menu->borderPadding.y + 1 ) * ( TEXT_TILE_SIZE ) );

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
   i32 newIndex = 0;
   u32 column;

   switch ( direction )
   {
      case Direction_Left:
         newIndex = (i32)menu->selectedIndex - menu->itemsPerColumn;
         if ( newIndex < 0 )
         {
            newIndex = menu->itemCount + newIndex;
         }
         break;
      case Direction_Right:
         newIndex = (i32)menu->selectedIndex + menu->itemsPerColumn;
         if ( newIndex >= (i32)( menu->itemCount ) )
         {
            newIndex = menu->itemsPerColumn - ( menu->itemCount - menu->selectedIndex );
         }
         break;
      case Direction_Up:
         column = menu->selectedIndex / menu->itemsPerColumn;
         newIndex = (i32)( menu->selectedIndex ) - 1;
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
         newIndex = (i32)( menu->selectedIndex ) + 1;
         if ( newIndex >= (i32)( menu->itemCount ) )
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

   menu->selectedIndex = (u32)newIndex;
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

internal void Menu_Update( Menu_t* menu )
{
   switch ( menu->id )
   {
      case MenuId_OverworldSpell: Menu_UpdateOverworldSpell( menu ); break;
      case MenuId_OverworldItem: Menu_UpdateOverworldItem( menu ); break;
      case MenuId_Zoom: Menu_UpdateZoom( menu ); break;
      case MenuId_SellItem: Menu_UpdateSellItem( menu ); break;
      case MenuId_BattleSpell: Menu_UpdateBattleSpell( menu ); break;
      case MenuId_BattleItem: Menu_UpdateBattleItem( menu ); break;
   }
}

internal void Menu_DrawCarat( Menu_t* menu )
{
   u32 i;
   u32 x, y;
   u32 startX = menu->position.x + ( ( menu->borderPadding.x + 1 ) * ( TEXT_TILE_SIZE ) );
   u32 startY = menu->position.y + ( ( menu->borderPadding.y + 1 ) * ( TEXT_TILE_SIZE ) );

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

internal void Menu_InitStartup( Menu_t* menu )
{
   strcpy( menu->items[0].text, STRING_STARTUP_MENU_START );
   strcpy( menu->items[1].text, STRING_STARTUP_MENU_PASSWORD );
   menu->items[0].command = MenuCommand_Startup_NewGame;
   menu->items[1].command = MenuCommand_Startup_EnterPassword;
   menu->itemCount = 2;
   menu->itemsPerColumn = 2;
   menu->selectedIndex = 0;

   menu->position.x = 48;
   menu->position.y = 152;
   menu->borderSize.x = 20;
   menu->borderSize.y = 7;
   menu->borderPadding.x = 3;
   menu->borderPadding.y = 1;
   menu->columnWidth = 8;
   menu->itemPadding = 1;
   menu->caratOffset = 2;
}

internal void Menu_InitOverworld( Menu_t* menu )
{
   u32 i;

   strcpy( menu->title, STRING_OVERWORLD_MENU_TITLE );
   strcpy( menu->items[0].text, STRING_OVERWORLD_MENU_TALK );
   strcpy( menu->items[1].text, STRING_OVERWORLD_MENU_STATUS );
   strcpy( menu->items[2].text, STRING_OVERWORLD_MENU_SEARCH );
   strcpy( menu->items[3].text, STRING_OVERWORLD_MENU_SPELL );
   strcpy( menu->items[4].text, STRING_OVERWORLD_MENU_ITEM );
   strcpy( menu->items[5].text, STRING_OVERWORLD_MENU_DOOR );
   menu->items[0].command = MenuCommand_Overworld_Talk;
   menu->items[1].command = MenuCommand_Overworld_Status;
   menu->items[2].command = MenuCommand_Overworld_Search;
   menu->items[3].command = MenuCommand_Overworld_Spell;
   menu->items[4].command = MenuCommand_Overworld_Item;
   menu->items[5].command = MenuCommand_Overworld_Door;
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

internal void Menu_InitBattle( Menu_t* menu )
{
   u32 i;

   strcpy( menu->title, "" );
   strcpy( menu->items[0].text, STRING_BATTLE_MENU_ATTACK );
   strcpy( menu->items[1].text, STRING_BATTLE_MENU_FLEE );
   strcpy( menu->items[2].text, STRING_BATTLE_MENU_SPELL );
   strcpy( menu->items[3].text, STRING_BATTLE_MENU_ITEM );
   menu->items[0].command = MenuCommand_Battle_Attack;
   menu->items[1].command = MenuCommand_Battle_Flee;
   menu->items[2].command = MenuCommand_Battle_Spell;
   menu->items[3].command = MenuCommand_Battle_Item;
   menu->itemCount = 4;
   for ( i = 0; i < menu->itemCount; i++ ) menu->items[i].twoLineText = False;
   menu->itemsPerColumn = 2;
   menu->selectedIndex = 0;

   menu->position.x = 88;
   menu->position.y = 8;
   menu->borderSize.x = 16;
   menu->borderSize.y = 5;
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 0;
   menu->columnWidth = 8;
   menu->itemPadding = 1;
   menu->caratOffset = 1;
}

internal void Menu_UpdateOverworldSpell( Menu_t* menu )
{
   u32 i;
   u32 spells = menu->player->spells;

   strcpy( menu->title, STRING_OVERWORLD_MENU_SPELL );

   menu->itemCount = SPELL_GET_MAPUSEABLECOUNT( spells, menu->tileMap->isDungeon, menu->tileMap->isDark );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 152;
   menu->position.y = 32;
   menu->borderSize.x = 12;
   menu->borderSize.y = (u16)( ( menu->itemCount * 2 ) + 3 );
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
      sprintf( menu->items[i].text, STRING_SPELL_HEAL );
      menu->items[i].command = MenuCommand_Spell_Heal;
      i++;
   }
   if ( menu->tileMap->isDark && SPELL_HAS_GLOW( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_GLOW );
      menu->items[i].command = MenuCommand_Spell_Glow;
      i++;
   }
   if ( menu->tileMap->isDungeon && SPELL_HAS_EVAC( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_EVAC );
      menu->items[i].command = MenuCommand_Spell_Evac;
      i++;
   }
   if ( !( menu->tileMap->isDungeon ) && SPELL_HAS_ZOOM(spells) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_ZOOM );
      menu->items[i].command = MenuCommand_Spell_Zoom;
      i++;
   }
   if ( SPELL_HAS_REPEL( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_REPEL );
      menu->items[i].command = MenuCommand_Spell_Repel;
      i++;
   }
   if ( SPELL_HAS_MIDHEAL( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_MIDHEAL );
      menu->items[i].command = MenuCommand_Spell_Midheal;
   }
}

internal void Menu_UpdateOverworldItem( Menu_t* menu )
{
   u32 i = 0;
   u32 items = menu->player->items;

   strcpy( menu->title, STRING_OVERWORLD_MENU_ITEM );

   menu->itemCount = ITEM_GET_MAPUSEABLECOUNT( items );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 144;
   menu->position.y = 48;
   menu->borderSize.x = 12;
   menu->borderSize.y = (u16)( ( menu->itemCount * 2 ) + 3 );
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

internal void Menu_UpdateZoom( Menu_t* menu )
{
   u32 i = 0;
   u8 tv = menu->player->townsVisited;

   strcpy( menu->title, STRING_SPELL_ZOOM );

   menu->itemCount = HAS_VISITED_COUNT( tv );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 136;
   menu->position.y = 64;
   menu->borderSize.x = 13;
   menu->borderSize.y = (u16)( ( menu->itemCount * 2 ) + 3 );
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
   if ( HAS_VISITED_RIMULDAR( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_RIMULDAR );
      menu->items[i].command = MenuCommand_Zoom_Rimuldar;
      i++;
   }
   if ( HAS_VISITED_CANTLIN( tv ) )
   {
      strcpy( menu->items[i].text, STRING_TOWN_CANTLIN );
      menu->items[i].command = MenuCommand_Zoom_Cantlin;
   }
}

internal void Menu_UpdateSellItem( Menu_t* menu )
{
   u32 i = 0;
   u32 items = menu->player->items;

   menu->title[0] = 0;
   menu->itemCount = ITEM_GET_SELLABLECOUNT( items );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 128;
   menu->position.y = 16;
   menu->borderSize.x = 12;
   menu->borderSize.y = (u16)( ( menu->itemCount * 2 ) + 3 );
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 1;
   menu->columnWidth = 10;
   menu->itemPadding = 1;
   menu->caratOffset = 1;

   if ( ITEM_GET_KEYCOUNT( items ) )
   {
      menu->items[i].twoLineText = False;
      sprintf( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_KEY, ITEM_GET_KEYCOUNT( items ) );
      menu->items[i].command = MenuCommand_Item_Herb;
      i++;
   }
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
   if ( ITEM_HAS_DRAGONSCALE( items ) )
   {
      menu->items[i].twoLineText = True;
      strcpy( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_DRAGONSCALE_1 );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_DRAGONSCALE_2 );
      menu->items[i].command = MenuCommand_Item_DragonScale;
      i++;
   }
}

internal void Menu_UpdateBattleSpell( Menu_t* menu )
{
   u32 i;
   u32 spells = menu->player->spells;

   strcpy( menu->title, STRING_BATTLE_MENU_SPELL );

   menu->itemCount = SPELL_GET_BATTLEUSEABLECOUNT( spells );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 144;
   menu->position.y = 16;
   menu->borderSize.x = 12;
   menu->borderSize.y = (u16)( ( menu->itemCount * 2 ) + 3 );
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
      sprintf( menu->items[i].text, STRING_SPELL_HEAL );
      menu->items[i].command = MenuCommand_Spell_Heal;
      i++;
   }
   if ( SPELL_HAS_SIZZ( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_SIZZ );
      menu->items[i].command = MenuCommand_Spell_Sizz;
      i++;
   }
   if ( SPELL_HAS_SLEEP( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_SLEEP );
      menu->items[i].command = MenuCommand_Spell_Sleep;
      i++;
   }
   if ( SPELL_HAS_FIZZLE(spells) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_FIZZLE );
      menu->items[i].command = MenuCommand_Spell_Fizzle;
      i++;
   }
   if ( SPELL_HAS_MIDHEAL( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_MIDHEAL );
      menu->items[i].command = MenuCommand_Spell_Midheal;
      i++;
   }
   if ( SPELL_HAS_SIZZLE( spells ) )
   {
      sprintf( menu->items[i].text, STRING_SPELL_SIZZLE );
      menu->items[i].command = MenuCommand_Spell_Sizzle;
   }
}

internal void Menu_UpdateBattleItem( Menu_t* menu )
{
   u32 i;
   u32 items = menu->player->items;

   strcpy( menu->title, STRING_BATTLE_MENU_ITEM );

   menu->itemCount = ITEM_GET_BATTLEUSEABLECOUNT( items );
   menu->itemsPerColumn = menu->itemCount;
   menu->selectedIndex = 0;
   menu->position.x = 136;
   menu->position.y = 32;
   menu->borderSize.x = 12;
   menu->borderSize.y = (u16)( ( menu->itemCount * 2 ) + 3 );
   menu->borderPadding.x = 1;
   menu->borderPadding.y = 1;
   menu->columnWidth = 10;
   menu->itemPadding = 1;
   menu->caratOffset = 1;

   i = 0;

   if ( ITEM_GET_HERBCOUNT( items ) )
   {
      menu->items[i].twoLineText = False;
      sprintf( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_HERB, ITEM_GET_HERBCOUNT( items ) );
      menu->items[i].command = MenuCommand_Item_Herb;
      i++;
   }
   if ( ITEM_HAS_FAIRYFLUTE( items ) )
   {
      menu->items[i].twoLineText = True;
      strcpy( menu->items[i].text, STRING_OVERWORLD_ITEMMENU_FAIRYFLUTE_1 );
      strcpy( menu->items[i].text + MENU_LINE_LENGTH, STRING_OVERWORLD_ITEMMENU_FAIRYFLUTE_2 );
      menu->items[i].command = MenuCommand_Item_FairyFlute;
   }
}
