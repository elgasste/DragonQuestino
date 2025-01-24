#include "input.h"

internal void Input_UpdateButtonState( ButtonState_t* buttonState, Bool_t down );

void Input_Init( Input_t* input )
{
   uint32_t i;

   for ( i = 0; i < Button_Count; i++ )
   {
      input->buttonStates[i].pressed = False;
      input->buttonStates[i].released = False;
      input->buttonStates[i].down = False;
   }

#if !defined( VISUAL_STUDIO_DEV )
   // TODO: this is how it was handled on the Mega 2560, figure it out for the Giga R1
   // pinMode( PIN_A_BUTTON, INPUT_PULLUP );
   // pinMode( PIN_B_BUTTON, INPUT_PULLUP );
#endif
}

void Input_Read( Input_t* input )
{
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( input );
#else
   int32_t xValue = analogRead( PIN_ANALOG_X );
   int32_t yValue = analogRead( PIN_ANALOG_Y );

   // our analog stick is rotated, so our values are reversed
   Input_UpdateButtonState( &( input->buttonStates[Button_Left] ), xValue > ANALOG_THRESHOLD_HIGH );
   Input_UpdateButtonState( &( input->buttonStates[Button_Up] ), yValue < ANALOG_THRESHOLD_LOW );
   Input_UpdateButtonState( &( input->buttonStates[Button_Right] ), xValue < ANALOG_THRESHOLD_LOW );
   Input_UpdateButtonState( &( input->buttonStates[Button_Down] ), yValue > ANALOG_THRESHOLD_HIGH );

   // TODO: implement these
   // Input_UpdateButtonState( &( input->buttonStates[BUTTON_A] ), digitalRead( PIN_A_BUTTON ) == LOW );
   // Input_UpdateButtonState( &( input->buttonStates[BUTTON_B] ), digitalRead( PIN_B_BUTTON ) == LOW );
#endif
}

Bool_t Input_AnyButtonPressed( Input_t* input )
{
   uint32_t i;

   for ( i = 0; i < Button_Count; i++ )
   {
      if ( input->buttonStates[i].pressed )
      {
         return True;
      }
   }

   return False;
}

internal void Input_UpdateButtonState( ButtonState_t* buttonState, Bool_t down )
{
   if ( down )
   {
      buttonState->released = False;
      buttonState->pressed = buttonState->down ? False : True;
   }
   else
   {
      buttonState->pressed = False;
      buttonState->released = buttonState->down ? True : False;
   }

   buttonState->down = down;
}
