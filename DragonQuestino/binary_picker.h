#if !defined( BINARY_PICKER_H )
#define BINARY_PICKER_H

#include "common.h"
#include "vector.h"

#define BINARYPICKER_OPTION_SIZE    16

typedef struct Screen_t Screen_t;

typedef struct BinaryPicker_t
{
   Screen_t* screen;

   u32 selectedIndex;
   Vector2u32_t position;        // position on the screen, in pixels
   Vector2u32_t size;            // in characters
   Bool_t showCarat;
   float blinkSeconds;
   char option1[BINARYPICKER_OPTION_SIZE];
   char option2[BINARYPICKER_OPTION_SIZE];
   void ( *option1Callback )( void* );
   void ( *option2Callback )( void* );
   void* option1CallbackData;
   void* option2CallbackData;
}
BinaryPicker_t;

#if defined( __cplusplus )
extern "C" {
#endif

void BinaryPicker_Init( BinaryPicker_t* picker, Screen_t* screen );
void BinaryPicker_Load( BinaryPicker_t* picker,
                        const char* option1, const char* option2,
                        void ( *callback1 )( void* ), void ( *callback2 )( void * ),
                        void* callbackData1, void* callbackData2 ) ;
void BinaryPicker_Draw( BinaryPicker_t* picker );
void BinaryPicker_ResetCarat( BinaryPicker_t* picker );
void BinaryPicker_MoveSelection( BinaryPicker_t* picker, Direction_t direction );
void BinaryPicker_Tic( BinaryPicker_t* picker );
void BinaryPicker_Select( BinaryPicker_t* picker );
void BinaryPicker_Select1( BinaryPicker_t* picker );
void BinaryPicker_Select2( BinaryPicker_t* picker );

#if defined( __cplusplus )
}
#endif

#endif // BINARY_PICKER_H 
