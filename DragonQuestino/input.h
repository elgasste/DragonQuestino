#if !defined( INPUT_H )
#define INPUT_H

#include "common.h"

typedef enum
{
   Button_Left = 0,
   Button_Up,
   Button_Right,
   Button_Down,
   Button_A,
   Button_B,

   Button_Count
}
Button_t;

typedef struct ButtonState_t
{
   Bool_t pressed;
   Bool_t released;
   Bool_t down;
}
ButtonState_t;

typedef struct Input_t
{
   ButtonState_t buttonStates[Button_Count];
}
Input_t;

#if defined( __cplusplus )
extern "C" {
#endif

   void Input_Init( Input_t* input );
   void Input_Read( Input_t* input );
   Bool_t Input_AnyButtonPressed( Input_t* input );

#if defined( VISUAL_STUDIO_DEV )
   void Input_ResetState( Input_t* input );
   void Input_ButtonPressed( Input_t* input, uint8_t button );
   void Input_ButtonReleased( Input_t* input, uint8_t button );
#else
   void Input_SetPinModes( Input_t* input );
#endif

#if defined( __cplusplus )
}
#endif

#endif // INPUT_H
