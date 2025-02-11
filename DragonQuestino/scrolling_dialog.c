#include "scrolling_dialog.h"
#include "screen.h"

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
   uint32_t i, x, y;

   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y, COLOR_WHITE );

   x = dialog->position.x + ( TEXT_TILE_SIZE * 2 );
   y = dialog->position.y + TEXT_TILE_SIZE;

   // TODO: actually scroll the text
   for ( i = 0; i < dialog->lineCount; i++ )
   {
      Screen_DrawText( screen, dialog->lines[i], x, y, COLOR_WHITE );
      y += TEXT_TILE_SIZE;
   }
}

void ScrollingDialog_Tic( ScrollingDialog_t* dialog )
{
   // TODO: tic the scrolling
   UNUSED_PARAM( dialog );
}

internal void ScrollingDialog_LoadText( ScrollingDialog_t* dialog, const char* text )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen = (uint16_t)strlen( text );
   Bool_t endOfLine, endOfText;
   char curChar;

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
}
