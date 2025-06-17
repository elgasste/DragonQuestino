#include "game.h"
#include "tables.h"

internal void Dialog2_LoadActiveSectionLines( Dialog2_t* dialog );
internal void Dialog2_ResetScroll( Dialog2_t* dialog );
internal void Dialog2_FinishSection( Dialog2_t* dialog );

void Dialog2_Init( Dialog2_t* dialog, Screen_t* screen, MainState_t* mainState )
{
   dialog->screen = screen;
   dialog->mainState = mainState;
}

void Dialog2_Reset( Dialog2_t* dialog )
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

void Dialog2_Start( Dialog2_t* dialog )
{
   Dialog2_LoadActiveSectionLines( dialog );
   Dialog2_ResetScroll( dialog );
}

void Dialog2_PushSection( Dialog2_t* dialog, const char* text )
{
   Dialog2_PushSectionWithCallback( dialog, text, 0, 0 );
}

void Dialog2_PushSectionWithCallback( Dialog2_t* dialog, const char* text, void ( *callback )( void* ), void* callbackData )
{
   strcpy( dialog->sectionTexts[dialog->sectionCount], text );
   dialog->sectionCallbacks[dialog->sectionCount] = callback;
   dialog->sectionCallbackData[dialog->sectionCount] = callbackData;
   dialog->sectionCount++;
}

void Dialog2_NextSection( Dialog2_t* dialog )
{
   dialog->activeSection++;
   Dialog2_LoadActiveSectionLines( dialog );
   Dialog2_ResetScroll( dialog );
}

Bool_t Dialog2_StepAhead( Dialog2_t* dialog )
{
   if ( dialog->isScrolling )
   {
      Dialog2_FinishSection( dialog );
      return False;
   }

   Dialog2_NextSection( dialog );
   return True;
}

Bool_t Dialog2_IsDone( Dialog2_t* dialog )
{
   return ( !dialog->isScrolling && ( dialog->activeSection >= ( dialog->sectionCount - 1 ) ) ) ? True : False;
}

void Dialog2_Tic( Dialog2_t* dialog )
{
   if ( dialog->isScrolling )
   {
      dialog->scrollSeconds += CLOCK_FRAME_SECONDS;

      if ( dialog->scrollSeconds > dialog->scrollTotalSeconds )
      {
         Dialog2_FinishSection( dialog );
      }
   }
   else if ( !Dialog2_IsDone( dialog ) )
   {
      dialog->blinkSeconds += CLOCK_FRAME_SECONDS;

      while ( dialog->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
      {
         TOGGLE_BOOL( dialog->showCarat );
         dialog->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      }
   }
}

void Dialog2_Draw( Dialog2_t* dialog )
{
   uint32_t i, x, y, stopCharIndex, len;
   char substr[DIALOG2_LINE_TEXT_SIZE];
   Screen_t* screen = dialog->screen;

   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y );

   x = dialog->position.x + TEXT_TILE_SIZE;
   y = dialog->position.y + TEXT_TILE_SIZE;

   if ( dialog->isScrolling )
   {
      stopCharIndex = (uint32_t)( dialog->scrollSeconds / DIALOG2_SCROLL_CHAR_SECONDS );

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

      if ( dialog->showCarat && !Dialog2_IsDone( dialog ) )
      {
         Screen_DrawChar( screen, DOWNWARD_CARAT, x + ( ( dialog->lineWidth / 2 ) * TEXT_TILE_SIZE ), y );
      }
   }
}

internal void Dialog2_LoadActiveSectionLines( Dialog2_t* dialog )
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

internal void Dialog2_ResetScroll( Dialog2_t* dialog )
{
   dialog->isScrolling = True;
   dialog->scrollSeconds = 0.0f;
   dialog->scrollTotalSeconds = dialog->charCount * DIALOG2_SCROLL_CHAR_SECONDS;
   dialog->showCarat = True;
   dialog->blinkSeconds = 0.0f;
}

internal void Dialog2_FinishSection( Dialog2_t* dialog )
{
   dialog->isScrolling = False;

   if ( dialog->sectionCallbacks[dialog->activeSection] )
   {
      dialog->pendingCallback = dialog->sectionCallbacks[dialog->activeSection];
      dialog->pendingCallbackData = dialog->sectionCallbackData[dialog->activeSection];
   }
}
