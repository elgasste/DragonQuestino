#include "binary_picker.h"
#include "screen.h"
#include "clock.h"

internal void BinaryPicker_DrawCarat( BinaryPicker_t* picker );

void BinaryPicker_Init( BinaryPicker_t* picker, Screen_t* screen )
{
   picker->screen = screen;
   picker->position.x = 160;
   picker->position.y = 88;
}

void BinaryPicker_Load( BinaryPicker_t* picker,
                        const char* option1, const char* option2,
                        void ( *callback1 )( void* ), void ( *callback2 )( void * ),
                        void* callbackData1, void* callbackData2 )
{
   picker->option1Callback = callback1;
   picker->option2Callback = callback2;
   picker->option1CallbackData = callbackData1;
   picker->option2CallbackData = callbackData2;

   uint32_t strlen1 = (uint32_t)( strlen( option1 ) );
   uint32_t strlen2 = (uint32_t)( strlen( option2 ) );

   strcpy( picker->option1, option1 );
   strcpy( picker->option2, option2 );

   picker->size.x = ( strlen1 > strlen2 ) ? ( strlen1 + 5 ) : ( strlen2 + 5 );
   picker->size.y = 5;
   picker->selectedIndex = 0;
   BinaryPicker_ResetCarat( picker );
}

void BinaryPicker_Draw( BinaryPicker_t* picker )
{
   uint32_t startX = picker->position.x + ( TEXT_TILE_SIZE * 3 );
   uint32_t startY = picker->position.y + TEXT_TILE_SIZE;

   Screen_DrawTextWindow( picker->screen, picker->position.x, picker->position.y, picker->size.x, picker->size.y );
   Screen_DrawText( picker->screen, picker->option1, startX, startY );
   Screen_DrawText( picker->screen, picker->option2, startX, startY + ( TEXT_TILE_SIZE * 2 ) );
   BinaryPicker_DrawCarat( picker );
}

void BinaryPicker_ResetCarat( BinaryPicker_t* picker )
{
   picker->showCarat = True;
   picker->blinkSeconds = 0.0f;
   BinaryPicker_DrawCarat( picker );
}

void BinaryPicker_MoveSelection( BinaryPicker_t* picker, Direction_t direction )
{
   if ( direction == Direction_Up || direction == Direction_Down )
   {
      picker->selectedIndex = ( picker->selectedIndex == 0 ) ? 1 : 0;
   }

   BinaryPicker_ResetCarat( picker );
}

void BinaryPicker_Tic( BinaryPicker_t* picker )
{
   picker->blinkSeconds += CLOCK_FRAME_SECONDS;

   while ( picker->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
   {
      picker->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      TOGGLE_BOOL( picker->showCarat );
   }
}

void BinaryPicker_Select( BinaryPicker_t* picker )
{
   if ( picker->selectedIndex == 0 )
   {
      BinaryPicker_Select1( picker );
   }
   else
   {
      BinaryPicker_Select2( picker );
   }
}

void BinaryPicker_Select1( BinaryPicker_t* picker )
{
   picker->option1Callback( picker->option1CallbackData );
}

void BinaryPicker_Select2( BinaryPicker_t* picker )
{
   picker->option2Callback( picker->option2CallbackData );
}

internal void BinaryPicker_DrawCarat( BinaryPicker_t* picker )
{
   if ( picker->selectedIndex == 0 )
   {
      Screen_DrawChar( picker->screen, picker->showCarat ? '>' : ' ', picker->position.x + TEXT_TILE_SIZE, picker->position.y + TEXT_TILE_SIZE);
      Screen_DrawChar( picker->screen, ' ', picker->position.x + TEXT_TILE_SIZE, picker->position.y + ( TEXT_TILE_SIZE * 3 ) );
   }
   else
   {
      Screen_DrawChar( picker->screen, ' ', picker->position.x + TEXT_TILE_SIZE, picker->position.y + TEXT_TILE_SIZE );
      Screen_DrawChar( picker->screen, picker->showCarat ? '>' : ' ', picker->position.x + TEXT_TILE_SIZE, picker->position.y + ( TEXT_TILE_SIZE * 3 ));
   }
}
