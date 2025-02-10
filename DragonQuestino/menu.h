#if !defined( MENU_H )
#define MENU_H

#include "common.h"
#include "vector.h"

#define MENU_MAX_ITEMS              16
#define MENU_TEXT_LENGTH            32

#define MENU_BLINK_RATE_SECONDS     0.25f

typedef struct Screen_t Screen_t;

typedef struct MenuItem_t
{
   char text[MENU_TEXT_LENGTH];
   MenuCommand_t command;
}
MenuItem_t;

typedef struct Menu_t
{
   char title[MENU_TEXT_LENGTH];
   MenuItem_t items[MENU_MAX_ITEMS];
   uint32_t itemCount;
   uint32_t itemsPerColumn;
   uint32_t selectedIndex;

   Vector2u32_t position;        // position on the screen, in pixels
   Vector2u16_t borderSize;      // overall border size, in characters
   Vector2u16_t borderPadding;   // left and top padding of menu item list, in characters
   uint32_t columnWidth;         // in characters
   uint32_t itemPadding;         // individual menu item top padding, in characters
   uint32_t cursorOffset;        // how far to the left to draw the cursor, in characters

   Bool_t showCursor;
   float blinkSeconds;
}
Menu_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Menu_Load( Menu_t* menu, MenuId_t id );
void Menu_Draw( Menu_t* menu, Screen_t* screen );
void Menu_ResetCursor( Menu_t* menu );
void Menu_MoveSelection( Menu_t* menu, Direction_t direction );
void Menu_Tic( Menu_t* menu );

#if defined( __cplusplus )
}
#endif

#endif // MENU_H
