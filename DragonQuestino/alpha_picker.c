#include "alpha_picker.h"
#include "screen.h"
#include "clock.h"

internal void AlphaPicker_DrawCarat( AlphaPicker_t* picker );

void AlphaPicker_Init( AlphaPicker_t* picker, Screen_t* screen )
{
   uint32_t i;

   picker->screen = screen;
   picker->hasDot = False;

   // A-Z
   for ( i = 0; i < 26; i++ )
   {
      picker->chars[i] = (char)( i + 65 );
   }

   // a-z
   for ( i = 26; i < 52; i++ )
   {
      picker->chars[i] = (char)( i - 26 + 97 );
   }

   // 0-9
   for ( i = 52; i < 62; i++ )
   {
      picker->chars[i] = (char)( i - 52 + 48 );
   }

   picker->chars[62] = '-';
   picker->chars[63] = ' ';
   picker->chars[64] = 0;
}

void AlphaPicker_Reset( AlphaPicker_t* picker, const char* title, Bool_t hasDot )
{
   picker->selectedIndex = 0;
   picker->hasDot = hasDot;
   picker->chars[63] = hasDot ? '.' : '_';
   strcpy( picker->title, title );
   AlphaPicker_ResetCarat( picker );
}

void AlphaPicker_Draw( AlphaPicker_t* picker )
{
   uint32_t i, j, x, y, index;
   uint32_t startX = picker->position.x + ( TEXT_TILE_SIZE * 2 );
   uint32_t startY = picker->position.y + ( TEXT_TILE_SIZE * 2 );
   char text[2];

   Screen_DrawTextWindowWithTitle( picker->screen, picker->position.x, picker->position.y, 25, 14, picker->title );

   text[1] = 0;

   for ( i = 0; i < ALPHA_PICKER_ROWS; i++ )
   {
      for ( j = 0; j < ALPHA_PICKER_COLS; j++ )
      {
         index = ( i * ALPHA_PICKER_COLS ) + j;
         x = startX + ( j * TEXT_TILE_SIZE * 2 );
         y = startY + ( i * TEXT_TILE_SIZE * 2 );

         if ( index == 64 )
         {
            Screen_DrawText( picker->screen, STRING_ALPHAPICKER_NAME_END, x, y );
            break;
         }
         else
         {
            text[0] = picker->chars[index];
            Screen_DrawText( picker->screen, text, x, y );
         }
      }
   }

   AlphaPicker_DrawCarat( picker );
}

void AlphaPicker_ResetCarat( AlphaPicker_t* picker )
{
   picker->showCarat = True;
   picker->blinkSeconds = 0.0f;
   AlphaPicker_DrawCarat( picker );
}

void AlphaPicker_MoveSelection( AlphaPicker_t* picker, Direction_t direction )
{
   uint32_t row = picker->selectedIndex / ALPHA_PICKER_COLS;
   uint32_t col = picker->selectedIndex % ALPHA_PICKER_COLS;

   if ( direction == Direction_Left )
   {
      picker->selectedIndex = ( col == 0 ) ? ( picker->selectedIndex + ALPHA_PICKER_COLS - 1 ) : ( picker->selectedIndex - 1 );
   }
   else if ( direction == Direction_Right )
   {
      if ( picker->selectedIndex == 64 )
      {
         picker->selectedIndex -= ( ALPHA_PICKER_COLS - 2 );
      }
      else
      {
         picker->selectedIndex = ( col == ( ALPHA_PICKER_COLS - 1 ) ) ? ( picker->selectedIndex - ALPHA_PICKER_COLS + 1 ) : ( picker->selectedIndex + 1 );
      }
   }
   else if ( direction == Direction_Up )
   {
      picker->selectedIndex = ( row == 0 ) ? ( picker->selectedIndex + ( ( ALPHA_PICKER_ROWS - 1 ) * ALPHA_PICKER_COLS ) ) : ( picker->selectedIndex - ALPHA_PICKER_COLS );

      if ( picker->selectedIndex > 64 )
      {
         picker->selectedIndex -= ALPHA_PICKER_COLS;
      }
   }
   else if ( direction == Direction_Down )
   {
      picker->selectedIndex = ( row == ( ALPHA_PICKER_ROWS - 1 ) ) ? picker->selectedIndex - ( ( ALPHA_PICKER_ROWS - 1 ) * ALPHA_PICKER_COLS ) : ( picker->selectedIndex + ALPHA_PICKER_COLS );

      if ( picker->selectedIndex > 64 )
      {
         picker->selectedIndex -= ( ( ALPHA_PICKER_ROWS - 1 ) * ALPHA_PICKER_COLS );
      }
   }

   AlphaPicker_ResetCarat( picker );
}

void AlphaPicker_Tic( AlphaPicker_t* picker )
{
   picker->blinkSeconds += CLOCK_FRAME_SECONDS;

   while ( picker->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
   {
      picker->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      TOGGLE_BOOL( picker->showCarat );
   }
}

internal void AlphaPicker_DrawCarat( AlphaPicker_t* picker )
{
   UNUSED_PARAM( picker );

   uint32_t i, j, x, y, index;
   uint32_t startX = picker->position.x + TEXT_TILE_SIZE;
   uint32_t startY = picker->position.y + ( TEXT_TILE_SIZE * 2 );

   for ( i = 0; i < ALPHA_PICKER_ROWS; i++ )
   {
      for ( j = 0; j < ALPHA_PICKER_COLS; j++ )
      {
         index = ( i * ALPHA_PICKER_COLS ) + j;
         x = startX + ( j * TEXT_TILE_SIZE * 2 );
         y = startY + ( i * TEXT_TILE_SIZE * 2 );

         if ( ( index == picker->selectedIndex ) && picker->showCarat )
         {
            Screen_DrawChar( picker->screen, '>', x, y );
         }
         else
         {
            Screen_DrawChar( picker->screen, ' ', x, y );
         }

         if ( index == 64 )
         {
            break;
         }
      }
   }
}
