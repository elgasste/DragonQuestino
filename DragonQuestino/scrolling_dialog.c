#include "scrolling_dialog.h"
#include "screen.h"
#include "clock.h"

internal void ScrollingDialog_LoadText( ScrollingDialog_t* dialog, const char* text );
internal void ScrollingDialog_LoadOverworld( ScrollingDialog_t* dialog );

void ScrollingDialog_Load( ScrollingDialog_t* dialog, ScrollingDialogId_t id )
{
   switch ( id )
   {
      case ScrollingDialogId_Overworld:
         ScrollingDialog_LoadOverworld( dialog );
         break;
   }
}

void ScrollingDialog_Draw( ScrollingDialog_t* dialog, Screen_t* screen )
{
   uint32_t i, x, y, stopCharIndex, len;
   char substr[DIALOG_MAX_LINE_CHARS];

   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y, COLOR_WHITE );

   x = dialog->position.x + ( TEXT_TILE_SIZE * 2 );
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
      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         Screen_DrawText( screen, dialog->lines[i], x, y, COLOR_WHITE );
      }
   }
}

void ScrollingDialog_SkipScroll( ScrollingDialog_t* dialog )
{
   dialog->isScrolling = False;
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

internal void ScrollingDialog_LoadText( ScrollingDialog_t* dialog, const char* text )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen = (uint16_t)strlen( text );
   Bool_t endOfLine, endOfText;
   char curChar;

   dialog->lineCount = 0;
   dialog->charCount = 0;

   for ( textIndex = 0, lineIndex = 0, lastSpaceIndex = 0, currentLine = 0; textIndex < strLen; textIndex++ )
   {
      curChar = text[textIndex];
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
            else if ( text[textIndex + 1] == ' ' )
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

internal void ScrollingDialog_LoadOverworld( ScrollingDialog_t* dialog )
{
   dialog->position.x = 32;
   dialog->position.y = 128;
   dialog->size.x = 24;
   dialog->size.y = 10;
   dialog->lineWidth = 20;

   ScrollingDialog_LoadText( dialog, STRING_OVERWORLD_DIALOG_NOBODY_THERE );

   dialog->isScrolling = True;
   dialog->scrollSeconds = 0.0f;
   dialog->scrollTotalSeconds = dialog->charCount * DIALOG_SCROLL_CHAR_SECONDS;
}
