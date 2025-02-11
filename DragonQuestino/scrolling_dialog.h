#if !defined( SCROLLING_DIALOG_H )
#define SCROLLING_DIALOG_H

#include "common.h"
#include "vector.h"

#define DIALOG_MAX_LINE_CHARS          32
#define DIALOG_MAX_LINES               8
#define DIALOG_SCROLL_CHAR_SECONDS     0.015f

typedef struct Screen_t Screen_t;

typedef struct ScrollingDialog_t
{
   Vector2u16_t position;  // in pixels
   Vector2u16_t size;      // in characters
   uint32_t lineWidth;     // in characters

   char lines[DIALOG_MAX_LINES][DIALOG_MAX_LINE_CHARS];
   uint32_t lineCount;
   uint32_t charCount;

   Bool_t isScrolling;
   float scrollSeconds;
   float scrollTotalSeconds;
}
ScrollingDialog_t;

#if defined( __cplusplus )
extern "C" {
#endif

void ScrollingDialog_Load( ScrollingDialog_t* dialog, ScrollingDialogId_t id );
void ScrollingDialog_Draw( ScrollingDialog_t* dialog, Screen_t* screen );
void ScrollingDialog_SkipScroll( ScrollingDialog_t* dialog );
void ScrollingDialog_Tic( ScrollingDialog_t* dialog );

#if defined( __cplusplus )
}
#endif

#endif // SCROLLING_DIALOG_H
