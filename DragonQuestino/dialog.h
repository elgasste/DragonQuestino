#if !defined( DIALOG_H )
#define DIALOG_H

#include "common.h"
#include "vector.h"

#define DIALOG_MAX_SECTIONS           16
#define DIALOG_SECTION_TEXT_SIZE      224
#define DIALOG_LINE_TEXT_SIZE         32
#define DIALOG_MAX_LINES              7
#define DIALOG_SCROLL_CHAR_SECONDS    0.015f

typedef struct Screen_t Screen_t;

typedef struct Dialog_t
{
   Screen_t* screen;
   MainState_t* mainState;

   char sectionTexts[DIALOG_MAX_SECTIONS][DIALOG_SECTION_TEXT_SIZE];
   void ( *sectionCallbacks[DIALOG_MAX_SECTIONS] )( void* );
   void* sectionCallbackData[DIALOG_MAX_SECTIONS];
   void ( *pendingCallback )( void* );
   void* pendingCallbackData;
   uint32_t sectionCount;
   uint32_t activeSection;

   Vector2u16_t position;  // in pixels
   Vector2u16_t size;      // in characters
   uint32_t lineWidth;     // in characters

   char lines[DIALOG_MAX_LINES][DIALOG_LINE_TEXT_SIZE];
   uint32_t lineCount;
   uint32_t charCount;

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

void Dialog_Init( Dialog_t* dialog, Screen_t* screen, MainState_t* mainState );
void Dialog_Reset( Dialog_t* dialog );
void Dialog_Start( Dialog_t* dialog );
void Dialog_PushSection( Dialog_t* dialog, const char* text );
void Dialog_PushSectionWithCallback( Dialog_t* dialog, const char* text, void ( *callback )( void* ), void* callbackData );
void Dialog_NextSection( Dialog_t* dialog );
Bool_t Dialog_StepAhead( Dialog_t* dialog );
Bool_t Dialog_IsDone( Dialog_t* dialog );
void Dialog_Tic( Dialog_t* dialog );
void Dialog_Draw( Dialog_t* dialog );

#if defined( __cplusplus )
}
#endif

#endif // DIALOG_H
