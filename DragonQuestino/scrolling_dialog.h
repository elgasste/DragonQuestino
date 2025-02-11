#if !defined( SCROLLING_DIALOG_H )
#define SCROLLING_DIALOG_H

#include "common.h"
#include "vector.h"

#define DIALOG_MAX_LINE_CHARS    32
#define DIALOG_MAX_LINES         8

typedef struct Screen_t Screen_t;

typedef struct ScrollingDialog_t
{
   Vector2u16_t position;  // in pixels
   Vector2u16_t size;      // in characters
   uint32_t lineWidth;     // in characters

   char lines[DIALOG_MAX_LINE_CHARS][DIALOG_MAX_LINES];
   uint32_t lineCount;
}
ScrollingDialog_t;

#if defined( __cplusplus )
extern "C" {
#endif

void ScrollingDialog_Load( ScrollingDialog_t* dialog, ScrollingDialogId_t id );
void ScrollingDialog_Draw( ScrollingDialog_t* dialog, Screen_t* screen );
void ScrollingDialog_Tic( ScrollingDialog_t* dialog );

#if defined( __cplusplus )
}
#endif

#endif // SCROLLING_DIALOG_H
