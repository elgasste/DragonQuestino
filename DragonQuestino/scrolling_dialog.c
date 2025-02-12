#include "scrolling_dialog.h"
#include "screen.h"
#include "clock.h"

internal void ScrollingDialog_ResetScroll( ScrollingDialog_t* dialog );
internal void ScrollingDialog_LoadMessage( ScrollingDialog_t* dialog );
internal void ScrollingDialog_LoadOverworldType( ScrollingDialog_t* dialog );
internal void ScrollingDialog_LoadMessageSectionCount( ScrollingDialog_t* dialog );
internal void ScrollingDialog_GetMessageText( ScrollingDialog_t* dialog, char* text );

void ScrollingDialog_Init( ScrollingDialog_t* dialog, const char* playerName )
{
   strcpy( dialog->playerName, playerName );
}

void ScrollingDialog_Load( ScrollingDialog_t* dialog, ScrollingDialogType_t type, DialogMessageId_t messageId )
{
   dialog->messageId = messageId;
   dialog->section = 0;
   ScrollingDialog_LoadMessageSectionCount( dialog );

   switch ( type )
   {
      case ScrollingDialogType_Overworld:
         ScrollingDialog_LoadOverworldType( dialog );
         break;
   }
}

void ScrollingDialog_Draw( ScrollingDialog_t* dialog, Screen_t* screen )
{
   uint32_t i, x, y, stopCharIndex, len;
   char substr[DIALOG_MAX_LINE_CHARS];

   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y, COLOR_WHITE );

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
            Screen_DrawText( screen, dialog->lines[i], x, y, COLOR_WHITE );
            stopCharIndex -= len;
         }
         else
         {
            strcpy( substr, dialog->lines[i] );
            substr[stopCharIndex] = '\0';
            Screen_DrawText( screen, substr, x, y, COLOR_WHITE );
            break;
         }
      }
   }
   else
   {
      // TODO: if we've made it to this point and there are more sections to go,
      // show the blinking carat (need to add that to the text table I guess)
      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         Screen_DrawText( screen, dialog->lines[i], x, y, COLOR_WHITE );
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
}

internal void ScrollingDialog_LoadMessage( ScrollingDialog_t* dialog )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen;
   Bool_t endOfLine, endOfText;
   char curChar, message[DIALOG_MAX_MESSAGE_CHARS];

   dialog->lineCount = 0;
   dialog->charCount = 0;

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

internal void ScrollingDialog_LoadOverworldType( ScrollingDialog_t* dialog )
{
   dialog->position.x = 32;
   dialog->position.y = 128;
   dialog->size.x = 24;
   dialog->size.y = 10;
   dialog->lineWidth = 22;

   ScrollingDialog_LoadMessage( dialog );
   ScrollingDialog_ResetScroll( dialog );
}

internal void ScrollingDialog_LoadMessageSectionCount( ScrollingDialog_t* dialog )
{
   switch ( dialog->messageId )
   {
      case DialogMessageId_Talk_NobodyThere: dialog->sectionCount = 1; break;
      case DialogMessageId_Search_NothingFound: dialog->sectionCount = 2; break;
      case DialogMessageId_Spell_None: dialog->sectionCount = 1; break;
      case DialogMessageId_Item_None: dialog->sectionCount = 1; break;
      case DialogMessageId_Door_None: dialog->sectionCount = 1; break;
   }
}

internal void ScrollingDialog_GetMessageText( ScrollingDialog_t* dialog, char* text )
{
   switch ( dialog->messageId )
   {
      case DialogMessageId_Talk_NobodyThere: strcpy( text, STRING_OVERWORLD_DIALOG_NOBODY_THERE ); return;
      case DialogMessageId_Search_NothingFound:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SEARCH, dialog->playerName ); return;
            case 1: strcpy( text, STRING_OVERWORLD_DIALOG_SEARCH_NOT_FOUND ); return;
         }
      case DialogMessageId_Spell_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_SPELLS ); return;
      case DialogMessageId_Item_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_ITEMS ); return;
      case DialogMessageId_Door_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_DOOR ); return;
   }
}
