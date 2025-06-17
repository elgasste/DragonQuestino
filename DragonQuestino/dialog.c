#include "game.h"
#include "tables.h"

internal void Dialog_LoadActiveSectionLines( Dialog_t* dialog );
internal void Dialog_ResetScroll( Dialog_t* dialog );
internal void Dialog_FinishSection( Dialog_t* dialog );

void Dialog_Init( Dialog_t* dialog, Screen_t* screen, MainState_t* mainState )
{
   dialog->screen = screen;
   dialog->mainState = mainState;
}

void Dialog_Reset( Dialog_t* dialog )
{
   dialog->sectionCount = 0;
   dialog->activeSection = 0;
   dialog->pendingCallback = 0;

   switch ( *( dialog->mainState ) )
   {
      case MainState_Overworld:
         dialog->position.x = 32;
         dialog->position.y = 144;
         dialog->size.x = 24;
         dialog->size.y = 8;
         dialog->lineWidth = 22;
         break;
      case MainState_Battle:
         dialog->position.x = 24;
         dialog->position.y = 168;
         dialog->size.x = 26;
         dialog->size.y = 6;
         dialog->lineWidth = 24;
         break;
   }
}

void Dialog_Start( Dialog_t* dialog )
{
   Dialog_LoadActiveSectionLines( dialog );
   Dialog_ResetScroll( dialog );
}

void Dialog_PushSection( Dialog_t* dialog, const char* text )
{
   Dialog_PushSectionWithCallback( dialog, text, 0, 0 );
}

void Dialog_PushSectionWithCallback( Dialog_t* dialog, const char* text, void ( *callback )( void* ), void* callbackData )
{
   strcpy( dialog->sectionTexts[dialog->sectionCount], text );
   dialog->sectionCallbacks[dialog->sectionCount] = callback;
   dialog->sectionCallbackData[dialog->sectionCount] = callbackData;
   dialog->sectionCount++;
}

void Dialog_NextSection( Dialog_t* dialog )
{
   dialog->activeSection++;
   Dialog_LoadActiveSectionLines( dialog );
   Dialog_ResetScroll( dialog );
}

Bool_t Dialog_StepAhead( Dialog_t* dialog )
{
   if ( dialog->isScrolling )
   {
      Dialog_FinishSection( dialog );
      return False;
   }

   Dialog_NextSection( dialog );
   return True;
}

Bool_t Dialog_IsDone( Dialog_t* dialog )
{
   return ( !dialog->isScrolling && ( dialog->activeSection >= ( dialog->sectionCount - 1 ) ) ) ? True : False;
}

void Dialog_Tic( Dialog_t* dialog )
{
   if ( dialog->isScrolling )
   {
      dialog->scrollSeconds += CLOCK_FRAME_SECONDS;

      if ( dialog->scrollSeconds > dialog->scrollTotalSeconds )
      {
         Dialog_FinishSection( dialog );
      }
   }
   else if ( !Dialog_IsDone( dialog ) )
   {
      dialog->blinkSeconds += CLOCK_FRAME_SECONDS;

      while ( dialog->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
      {
         TOGGLE_BOOL( dialog->showCarat );
         dialog->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      }
   }
}

void Dialog_Draw( Dialog_t* dialog )
{
   uint32_t i, x, y, stopCharIndex, len;
   char substr[DIALOG_LINE_TEXT_SIZE];
   Screen_t* screen = dialog->screen;

   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y );

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
            Screen_DrawText( screen, dialog->lines[i], x, y );
            stopCharIndex -= len;
         }
         else
         {
            strcpy( substr, dialog->lines[i] );
            substr[stopCharIndex] = '\0';
            Screen_DrawText( screen, substr, x, y );
            break;
         }
      }
   }
   else
   {
      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         Screen_DrawText( screen, dialog->lines[i], x, y );
      }

      if ( dialog->showCarat && !Dialog_IsDone( dialog ) )
      {
         Screen_DrawChar( screen, DOWNWARD_CARAT, x + ( ( dialog->lineWidth / 2 ) * TEXT_TILE_SIZE ), y );
      }
   }
}

internal void Dialog_LoadActiveSectionLines( Dialog_t* dialog )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen;
   Bool_t endOfLine, endOfText;
   char curChar;
   char* sectionText = dialog->sectionTexts[dialog->activeSection];

   dialog->lineCount = 0;
   dialog->charCount = 0;

   strLen = (uint16_t)strlen( sectionText );

   for ( textIndex = 0, lineIndex = 0, lastSpaceIndex = 0, currentLine = 0; textIndex < strLen; textIndex++ )
   {
      curChar = sectionText[textIndex];
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
            else if ( sectionText[textIndex + 1] == ' ' )
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

internal void Dialog_ResetScroll( Dialog_t* dialog )
{
   dialog->isScrolling = True;
   dialog->scrollSeconds = 0.0f;
   dialog->scrollTotalSeconds = dialog->charCount * DIALOG_SCROLL_CHAR_SECONDS;
   dialog->showCarat = True;
   dialog->blinkSeconds = 0.0f;
}

internal void Dialog_FinishSection( Dialog_t* dialog )
{
   dialog->isScrolling = False;

   if ( dialog->sectionCallbacks[dialog->activeSection] )
   {
      dialog->pendingCallback = dialog->sectionCallbacks[dialog->activeSection];
      dialog->pendingCallbackData = dialog->sectionCallbackData[dialog->activeSection];
   }
}
