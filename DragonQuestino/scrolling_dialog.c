#include "scrolling_dialog.h"
#include "screen.h"

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
   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y, COLOR_WHITE );

   // TODO: draw the appropriate section of text
}

void ScrollingDialog_Tic( ScrollingDialog_t* dialog )
{
   // TODO: tic the scrolling
   UNUSED_PARAM( dialog );
}

internal void ScrollingDialog_LoadOverworld( ScrollingDialog_t* dialog )
{
   dialog->position.x = 32;
   dialog->position.y = 128;
   dialog->size.x = 24;
   dialog->size.y = 10;

   // TODO: load the text
}
