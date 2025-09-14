#if !defined( ALPHA_PICKER_H )
#define ALPHA_PICKER_H

#include "common.h"
#include "vector.h"

#define ALPHA_PICKER_TITLE_LENGTH   24
#define ALPHA_PICKER_ROWS           6
#define ALPHA_PICKER_COLS           11

typedef struct Screen_t Screen_t;

typedef struct AlphaPicker_t
{
   Screen_t* screen;

   char title[ALPHA_PICKER_TITLE_LENGTH];
   char chars[65];
   Bool_t hasDot;
   u32 selectedIndex;
   Vector2u32_t position;        // position on the screen, in pixels
   Bool_t showCarat;
   float blinkSeconds;
}
AlphaPicker_t;

#if defined( __cplusplus )
extern "C" {
#endif

void AlphaPicker_Init( AlphaPicker_t* picker, Screen_t* screen );
void AlphaPicker_Reset( AlphaPicker_t* picker, const char* title, Bool_t hasDot );
void AlphaPicker_Draw( AlphaPicker_t* picker );
void AlphaPicker_ResetCarat( AlphaPicker_t* picker );
void AlphaPicker_MoveSelection( AlphaPicker_t* picker, Direction_t direction );
void AlphaPicker_Tic( AlphaPicker_t* picker );
char AlphaPicker_GetSelectedChar( AlphaPicker_t* picker );

#if defined( __cplusplus )
}
#endif

#endif // ALPHA_PICKER_H
