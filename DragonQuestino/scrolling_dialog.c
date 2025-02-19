#include "scrolling_dialog.h"
#include "screen.h"
#include "player.h"
#include "clock.h"

internal void ScrollingDialog_ResetScroll( ScrollingDialog_t* dialog );
internal void ScrollingDialog_LoadMessage( ScrollingDialog_t* dialog );
internal void ScrollingDialog_LoadType( ScrollingDialog_t* dialog, ScrollingDialogType_t type );
internal void ScrollingDialog_LoadOverworldType( ScrollingDialog_t* dialog );
internal uint32_t ScrollingDialog_GetMessageSectionCount( DialogMessageId_t messageId );
internal void ScrollingDialog_GetMessageText( ScrollingDialog_t* dialog, char* text );

void ScrollingDialog_Init( ScrollingDialog_t* dialog, Screen_t* screen, Player_t* player )
{
   dialog->screen = screen;
   dialog->player = player;
}

void ScrollingDialog_Load( ScrollingDialog_t* dialog, ScrollingDialogType_t type, DialogMessageId_t messageId )
{
   dialog->messageId = messageId;
   dialog->section = 0;

   ScrollingDialog_LoadType( dialog, type );
   ScrollingDialog_LoadMessage( dialog );
   ScrollingDialog_ResetScroll( dialog );
}

void ScrollingDialog_SetInsertionText( ScrollingDialog_t* dialog, const char* text )
{
   strcpy( dialog->insertionText, text );
}

void ScrollingDialog_Draw( ScrollingDialog_t* dialog )
{
   uint32_t i, x, y, stopCharIndex, len;
   char substr[DIALOG_MAX_LINE_CHARS];

   Screen_DrawTextWindow( dialog->screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y, COLOR_WHITE );

   x = dialog->position.x + TEXT_TILE_SIZE;
   y = dialog->position.y + TEXT_TILE_SIZE;

   if ( dialog->isScrolling )
   {
      stopCharIndex = (uint32_t)( dialog->scrollSeconds / DIALOG_SCROLL_CHAR_SECONDS );

      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         len = (uint32_t)( strlen( dialog->lines[i] ) );

         if ( len < stopCharIndex )
         {
            Screen_DrawText( dialog->screen, dialog->lines[i], x, y, COLOR_WHITE );
            stopCharIndex -= len;
         }
         else
         {
            strcpy( substr, dialog->lines[i] );
            substr[stopCharIndex] = '\0';
            Screen_DrawText( dialog->screen, substr, x, y, COLOR_WHITE );
            break;
         }
      }
   }
   else
   {
      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         Screen_DrawText( dialog->screen, dialog->lines[i], x, y, COLOR_WHITE );
      }

      if ( dialog->showCarat && !ScrollingDialog_IsDone( dialog ) )
      {
         Screen_DrawChar( dialog->screen, DOWNWARD_CARAT, x + ( ( dialog->lineWidth / 2 ) * TEXT_TILE_SIZE ), y, COLOR_WHITE );
      }
   }
}

void ScrollingDialog_Next( ScrollingDialog_t* dialog )
{
   if ( dialog->isScrolling )
   {
      dialog->isScrolling = False;
   }
   else if ( dialog->section < ( dialog->sectionCount - 1 ) )
   {
      dialog->section++;
      ScrollingDialog_LoadMessage( dialog );
      ScrollingDialog_ResetScroll( dialog );
   }
}

void ScrollingDialog_Tic( ScrollingDialog_t* dialog )
{
   if ( dialog->isScrolling )
   {
      dialog->scrollSeconds += CLOCK_FRAME_SECONDS;

      if ( dialog->scrollSeconds > dialog->scrollTotalSeconds )
      {
         dialog->isScrolling = False;
      }
   }
   else if ( !ScrollingDialog_IsDone( dialog ) )
   {
      dialog->blinkSeconds += CLOCK_FRAME_SECONDS;

      while ( dialog->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
      {
         TOGGLE_BOOL( dialog->showCarat );
         dialog->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      }
   }
}

Bool_t ScrollingDialog_IsDone( ScrollingDialog_t* dialog )
{
   return ( !dialog->isScrolling && ( dialog->section >= ( dialog->sectionCount - 1 ) ) ) ? True : False;
}

internal void ScrollingDialog_ResetScroll( ScrollingDialog_t* dialog )
{
   dialog->isScrolling = True;
   dialog->scrollSeconds = 0.0f;
   dialog->scrollTotalSeconds = dialog->charCount * DIALOG_SCROLL_CHAR_SECONDS;
   dialog->showCarat = True;
   dialog->blinkSeconds = 0.0f;
}

internal void ScrollingDialog_LoadMessage( ScrollingDialog_t* dialog )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen;
   Bool_t endOfLine, endOfText;
   char curChar, message[DIALOG_MAX_MESSAGE_CHARS];

   dialog->lineCount = 0;
   dialog->charCount = 0;
   dialog->sectionCount = ScrollingDialog_GetMessageSectionCount( dialog->messageId );

   ScrollingDialog_GetMessageText( dialog, message );
   strLen = (uint16_t)strlen( message );

   for ( textIndex = 0, lineIndex = 0, lastSpaceIndex = 0, currentLine = 0; textIndex < strLen; textIndex++ )
   {
      curChar = message[textIndex];
      endOfLine = ( lineIndex == ( dialog->lineWidth - 1 ) );
      endOfText = ( textIndex == ( strLen - 1 ) ) ? True : False;

      if ( endOfLine || endOfText )
      {
         if ( ( lastSpaceIndex > 0 ) && !endOfText )
         {
            if ( curChar == ' ' )
            {
               dialog->lines[currentLine][lineIndex] = '\0';
            }
            else if ( message[textIndex + 1] == ' ' )
            {
               dialog->lines[currentLine][lineIndex] = curChar;
               dialog->lines[currentLine][lineIndex + 1] = '\0';
            }
            else
            {
               dialog->lines[currentLine][lastSpaceIndex - 1] = '\0';
               textIndex -= ( ( lineIndex - lastSpaceIndex ) + 1 );
            }
         }
         else if ( curChar == ' ' )
         {
            dialog->lines[currentLine][lineIndex] = '\0';
         }
         else
         {
            dialog->lines[currentLine][lineIndex] = curChar;
            dialog->lines[currentLine][lineIndex + 1] = '\0';
         }

         lineIndex = 0;
         lastSpaceIndex = 0;
         dialog->lineCount++;
         dialog->charCount += (uint32_t)( strlen( dialog->lines[currentLine] ) );
         currentLine++;
      }
      else if ( curChar != ' ' )
      {
         dialog->lines[currentLine][lineIndex++] = curChar;
      }
      else if ( lineIndex > 0 && lastSpaceIndex != lineIndex )
      {
         dialog->lines[currentLine][lineIndex++] = curChar;
         lastSpaceIndex = lineIndex;
      }
   }
}

internal void ScrollingDialog_LoadType( ScrollingDialog_t* dialog, ScrollingDialogType_t type )
{
   switch ( type )
   {
      case ScrollingDialogType_Overworld:
         ScrollingDialog_LoadOverworldType( dialog );
         break;
   }
}

internal void ScrollingDialog_LoadOverworldType( ScrollingDialog_t* dialog )
{
   dialog->position.x = 32;
   dialog->position.y = 144;
   dialog->size.x = 24;
   dialog->size.y = 8;
   dialog->lineWidth = 22;
}

internal uint32_t ScrollingDialog_GetMessageSectionCount( DialogMessageId_t messageId )
{
   switch ( messageId )
   {
      case DialogMessageId_Talk_NobodyThere: return 1;
      case DialogMessageId_Search_NothingFound: return 2;
      case DialogMessageId_Spell_None: return 1;
      case DialogMessageId_Item_None: return 1;
      case DialogMessageId_Door_None: return 1;
      case DialogMessageId_Use_Herb: return 2;
      case DialogMessageId_Use_Wing: return 2;
      case DialogMessageId_Use_FairyWater: return 2;
      case DialogMessageId_Use_SilverHarp: return 2;
      case DialogMessageId_Use_FairyFlute: return 2;
      case DialogMessageId_Use_GwaelynsLove: return 2;
      case DialogMessageId_Use_RainbowDrop: return 1;
      case DialogMessageId_Chest_ItemCollected: return 1;
      case DialogMessageId_Chest_ItemNoSpace: return 2;
      case DialogMessageId_Chest_GoldCollected: return 1;
      case DialogMessageId_Chest_GoldNoSpace: return 2;
      case DialogMessageId_Chest_Tablet: return 8;
   }

   return 0;
}

internal void ScrollingDialog_GetMessageText( ScrollingDialog_t* dialog, char* text )
{
   Player_t* player = dialog->player;

   switch ( dialog->messageId )
   {
      case DialogMessageId_Talk_NobodyThere: strcpy( text, STRING_OVERWORLD_DIALOG_NOBODY_THERE ); return;
      case DialogMessageId_Search_NothingFound:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SEARCH, player->name ); return;
            case 1: strcpy( text, STRING_OVERWORLD_DIALOG_SEARCH_NOT_FOUND ); return;
         }
      case DialogMessageId_Spell_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_SPELLS ); return;
      case DialogMessageId_Item_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_ITEMS ); return;
      case DialogMessageId_Door_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_DOOR ); return;
      case DialogMessageId_Use_Herb:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_ITEMUSE_HERB, player->name ); return;
            case 1: strcpy( text, STRING_BUTNOTHINGHAPPENS ); return;
         }
      case DialogMessageId_Use_Wing:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_ITEMUSE_WING, player->name ); return;
            case 1: strcpy( text, STRING_BUTNOTHINGHAPPENS ); return;
         }
      case DialogMessageId_Use_FairyWater:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_ITEMUSE_FAIRYWATER, player->name ); return;
            case 1: strcpy( text, STRING_BUTNOTHINGHAPPENS ); return;
         }
      case DialogMessageId_Use_SilverHarp:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_ITEMUSE_SILVERHARP_1, player->name ); return;
            case 1: strcpy( text, STRING_ITEMUSE_SILVERHARP_2 ); return;
         }
      case DialogMessageId_Use_FairyFlute:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_ITEMUSE_FAIRYFLUTE_1, player->name ); return;
            case 1: strcpy( text, STRING_ITEMUSE_FAIRYFLUTE_2 ); return;
         }
      case DialogMessageId_Use_GwaelynsLove:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_ITEMUSE_GWAELYNSLOVE_1, player->name ); return;
            case 1: strcpy( text, STRING_ITEMUSE_GWAELYNSLOVE_2 ); return;
         }
      case DialogMessageId_Use_RainbowDrop:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_RAINBOWDROP_CANTUSE ); return;
         }
      case DialogMessageId_Chest_ItemCollected: sprintf( text, STRING_CHEST_ITEMFOUND, dialog->insertionText ); return;
      case DialogMessageId_Chest_ItemNoSpace:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_CHEST_ITEMFOUND, dialog->insertionText ); return;
            case 1: sprintf( text, STRING_CHEST_ITEMNOSPACE, dialog->player->name ); return;
         }
      case DialogMessageId_Chest_GoldCollected: sprintf( text, STRING_CHEST_GOLDFOUND, dialog->insertionText ); return;
      case DialogMessageId_Chest_GoldNoSpace:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_CHEST_GOLDFOUND, dialog->insertionText ); return;
            case 1: sprintf( text, STRING_CHEST_GOLDNOSPACE, dialog->player->name ); return;
         }
      case DialogMessageId_Chest_Tablet:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_CHEST_ITEMFOUND, STRING_ITEMCOLLECT_TABLET ); return;
            case 1: strcpy( text, STRING_CHEST_TABLET_1 ); return;
            case 2: strcpy( text, STRING_CHEST_TABLET_2 ); return;
            case 3: strcpy( text, STRING_CHEST_TABLET_3 ); return;
            case 4: strcpy( text, STRING_CHEST_TABLET_4 ); return;
            case 5: strcpy( text, STRING_CHEST_TABLET_5 ); return;
            case 6: strcpy( text, STRING_CHEST_TABLET_6 ); return;
            case 7: strcpy( text, STRING_CHEST_TABLET_7 ); return;
         }
   }
}
