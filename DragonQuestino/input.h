#if !defined( INPUT_H )
#define INPUT_H

#include "common.h"

#define PIN_ANALOG_X          A7
#define PIN_ANALOG_Y          A6
#define PIN_A_BUTTON          51
#define PIN_B_BUTTON          53

// the analog stick ranges from 0 to 1024
#define ANALOG_THRESHOLD_LOW  200
#define ANALOG_THRESHOLD_HIGH 824

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
   void Input_ButtonPressed( Input_t* input, uint32_t button );
   void Input_ButtonReleased( Input_t* input, uint32_t button );
#endif

#if defined( __cplusplus )
}
#endif

#endif // INPUT_H
