#if !defined( SCROLLING_DIALOG_H )
#define SCROLLING_DIALOG_H

#include "common.h"
#include "vector.h"

#define DIALOG_MAX_LINE_CHARS          32
#define DIALOG_MAX_LINES               7
#define DIALOG_MAX_MESSAGE_CHARS       224
#define DIALOG_SCROLL_CHAR_SECONDS     0.015f

typedef struct Screen_t Screen_t;
typedef struct Player_t Player_t;

typedef struct ScrollingDialog_t
{
   Screen_t* screen;
   Player_t* player;

   DialogMessageId_t messageId;
   Vector2u16_t position;  // in pixels
   Vector2u16_t size;      // in characters
   uint32_t lineWidth;     // in characters

   char lines[DIALOG_MAX_LINES][DIALOG_MAX_LINE_CHARS];
   uint32_t lineCount;
   uint32_t charCount;
   uint32_t section;
   uint32_t sectionCount;

   Bool_t isScrolling;
   float scrollSeconds;
   float scrollTotalSeconds;

   Bool_t showCarat;
   float blinkSeconds;
}
ScrollingDialog_t;

#if defined( __cplusplus )
extern "C" {
#endif

void ScrollingDialog_Init( ScrollingDialog_t* dialog, Screen_t* screen, Player_t* player );
void ScrollingDialog_Load( ScrollingDialog_t* dialog, ScrollingDialogType_t type, DialogMessageId_t messageId );
void ScrollingDialog_Draw( ScrollingDialog_t* dialog );
void ScrollingDialog_Next( ScrollingDialog_t* dialog );
void ScrollingDialog_Tic( ScrollingDialog_t* dialog );
Bool_t ScrollingDialog_IsDone( ScrollingDialog_t* dialog );

#if defined( __cplusplus )
}
#endif

#endif // SCROLLING_DIALOG_H
