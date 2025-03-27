#if !defined( MENU_H )
#define MENU_H

#include "common.h"
#include "vector.h"

#define MENU_MAX_ITEMS              16
#define MENU_LINE_LENGTH            16
#define MENU_TEXT_SIZE              32 // account for 2 lines

typedef struct Screen_t Screen_t;
typedef struct Player_t Player_t;
typedef struct TileMap_t TileMap_t;

typedef struct MenuItem_t
{
   char text[MENU_TEXT_SIZE];
   Bool_t twoLineText;
   MenuCommand_t command;
}
MenuItem_t;

typedef struct Menu_t
{
   Screen_t* screen;
   Player_t* player;
   TileMap_t* tileMap;

   MenuId_t id;
   char title[MENU_LINE_LENGTH];
   MenuItem_t items[MENU_MAX_ITEMS];
   uint32_t itemCount;
   uint32_t itemsPerColumn;
   uint32_t selectedIndex;

   Vector2u32_t position;        // position on the screen, in pixels
   Vector2u16_t borderSize;      // overall border size, in characters
   Vector2u16_t borderPadding;   // left and top padding of menu item list, in characters
   uint32_t columnWidth;         // in characters
   uint32_t itemPadding;         // individual menu item top padding, in characters
   uint32_t caratOffset;         // how far to the left to draw the carat, in characters

   Bool_t hasDrawn;
   Bool_t showCarat;
   float blinkSeconds;
}
Menu_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Menu_Init( Menu_t* menu, MenuId_t id, Screen_t* screen, Player_t* player, TileMap_t* tileMap );
void Menu_Reset( Menu_t* menu );
void Menu_Draw( Menu_t* menu );
void Menu_ResetCarat( Menu_t* menu );
void Menu_MoveSelection( Menu_t* menu, Direction_t direction );
void Menu_Tic( Menu_t* menu );

#if defined( __cplusplus )
}
#endif

#endif // MENU_H
