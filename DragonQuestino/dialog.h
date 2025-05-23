#if !defined( DIALOG_H )
#define DIALOG_H

#include "common.h"
#include "vector.h"

#define DIALOG_MAX_LINE_CHARS          32
#define DIALOG_MAX_INSERTION_CHARS     64
#define DIALOG_MAX_LINES               7
#define DIALOG_MAX_MESSAGE_CHARS       224
#define DIALOG_SCROLL_CHAR_SECONDS     0.015f

typedef struct Game_t Game_t;

typedef struct Dialog_t
{
   Game_t* game;

   DialogId_t id;
   Vector2u16_t position;  // in pixels
   Vector2u16_t size;      // in characters
   uint32_t lineWidth;     // in characters

   char lines[DIALOG_MAX_LINES][DIALOG_MAX_LINE_CHARS];
   char insertionText[DIALOG_MAX_INSERTION_CHARS];
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
Dialog_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Dialog_Init( Dialog_t* dialog, Game_t* game );
void Dialog_Load( Dialog_t* dialog, DialogId_t id );
void Dialog_SetInsertionText( Dialog_t* dialog, const char* text );
void Dialog_Draw( Dialog_t* dialog );
Bool_t Dialog_StepAhead( Dialog_t* dialog );
void Dialog_NextSection( Dialog_t* dialog );
void Dialog_Tic( Dialog_t* dialog );
Bool_t Dialog_IsDone( Dialog_t* dialog );

#if defined( __cplusplus )
}
#endif

#endif // DIALOG_H
