#if !defined( DIALOG_H )
#define DIALOG_H

#include "common.h"
#include "vector.h"

#define DIALOG_MAX_LINE_CHARS          32
#define DIALOG_MAX_INSERTION_CHARS     64
#define DIALOG_MAX_LINES               7
#define DIALOG_MAX_MESSAGE_CHARS       224
#define DIALOG_SCROLL_CHAR_SECONDS     0.015f

#define DIALOG2_MAX_SECTIONS           16
#define DIALOG2_SECTION_TEXT_SIZE      224
#define DIALOG2_LINE_TEXT_SIZE         32
#define DIALOG2_MAX_INSERTION_CHARS    64
#define DIALOG2_MAX_LINES              7
#define DIALOG2_SCROLL_CHAR_SECONDS    0.015f

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

typedef struct Screen_t Screen_t;

typedef struct Dialog2_t
{
   Screen_t* screen;
   MainState_t* mainState;

   char sectionTexts[DIALOG2_MAX_SECTIONS][DIALOG2_SECTION_TEXT_SIZE];
   void ( *sectionCallbacks[DIALOG2_MAX_SECTIONS] )( void* );
   void* sectionCallbackData[DIALOG2_MAX_SECTIONS];
   uint32_t sectionCount;
   uint32_t activeSection;

   Vector2u16_t position;  // in pixels
   Vector2u16_t size;      // in characters
   uint32_t lineWidth;     // in characters

   char lines[DIALOG2_MAX_LINES][DIALOG2_LINE_TEXT_SIZE];
   uint32_t lineCount;
   uint32_t charCount;

   Bool_t isScrolling;
   float scrollSeconds;
   float scrollTotalSeconds;

   Bool_t showCarat;
   float blinkSeconds;
}
Dialog2_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Dialog_SetInsertionText( Dialog_t* dialog, const char* text );

void Dialog2_Init( Dialog2_t* dialog, Screen_t* screen, MainState_t* mainState );
void Dialog2_Reset( Dialog2_t* dialog );
void Dialog2_Start( Dialog2_t* dialog );
void Dialog2_PushSection( Dialog2_t* dialog, const char* text );
void Dialog2_PushSectionWithCallback( Dialog2_t* dialog, const char* text, void ( *callback )( void* ), void* callbackData );
void Dialog2_NextSection( Dialog2_t* dialog );
Bool_t Dialog2_StepAhead( Dialog2_t* dialog );
Bool_t Dialog2_IsDone( Dialog2_t* dialog );
void Dialog2_Tic( Dialog2_t* dialog );
void Dialog2_Draw( Dialog2_t* dialog );

#if defined( __cplusplus )
}
#endif

#endif // DIALOG_H
