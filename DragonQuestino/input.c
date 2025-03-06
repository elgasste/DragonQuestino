#include "input.h"
#include "math.h"

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
   input->analogRestingState.x = analogRead( PIN_ANALOG_X );
   input->analogRestingState.y = analogRead( PIN_ANALOG_Y );
#endif
}

void Input_Read( Input_t* input )
{
#if defined( VISUAL_STUDIO_DEV )
   input->dpadIntensity[Direction_Left] = input->buttonStates[Button_Left].down ? 1.0f : 0.0f;
   input->dpadIntensity[Direction_Up] = input->buttonStates[Button_Up].down ? 1.0f : 0.0f;
   input->dpadIntensity[Direction_Right] = input->buttonStates[Button_Right].down ? 1.0f : 0.0f;
   input->dpadIntensity[Direction_Down] = input->buttonStates[Button_Down].down ? 1.0f : 0.0f;
#else
   Bool_t leftIsDown, upIsDown, rightIsDown, downIsDown;
   int32_t xValue = analogRead( PIN_ANALOG_X );
   int32_t yValue = analogRead( PIN_ANALOG_Y );

   // our analog stick is rotated, so X and Y values are reversed
   leftIsDown = ( xValue >= ( input->analogRestingState.x + INPUT_ANALOG_THRESHOLD ) ) ? True : False;
   upIsDown = ( yValue <= ( input->analogRestingState.y - INPUT_ANALOG_THRESHOLD ) ) ? True: False;
   rightIsDown = ( xValue <= ( input->analogRestingState.x - INPUT_ANALOG_THRESHOLD ) ) ? True : False;
   downIsDown = ( yValue >= ( input->analogRestingState.y + INPUT_ANALOG_THRESHOLD ) ) ? True : False;

   Input_UpdateButtonState( &( input->buttonStates[Button_Left] ), leftIsDown );
   Input_UpdateButtonState( &( input->buttonStates[Button_Up] ), upIsDown );
   Input_UpdateButtonState( &( input->buttonStates[Button_Right] ), rightIsDown );
   Input_UpdateButtonState( &( input->buttonStates[Button_Down] ), downIsDown );

   // for reference:
   //
   // - X is between 0 and 512: right
   // - X is between 512 and 1024: left
   // - Y is between 0 and 512: up
   // - Y is between 512 and 1024: down

   // TODO: fix this
   // input->dpadIntensity[Button_Left] = leftIsDown ? (float)( MATH_MIN( xValue, ANALOG_CUTOFF_HIGH ) - ANALOG_THRESHOLD_HIGH ) / ANALOG_ADJUSTED_RANGE : 0.0f;
   // input->dpadIntensity[Button_Up] = upIsDown ? (float)( MATH_MAX( yValue, ANALOG_CUTOFF_LOW ) - ANALOG_CUTOFF_LOW ) / ANALOG_ADJUSTED_RANGE : 0.0f;
   // input->dpadIntensity[Button_Right] = rightIsDown ? (float)( MATH_MAX( xValue, ANALOG_CUTOFF_LOW ) - ANALOG_CUTOFF_LOW ) / ANALOG_ADJUSTED_RANGE : 0.0f;
   // input->dpadIntensity[Button_Down] = downIsDown ? (float)( MATH_MIN( yValue, ANALOG_CUTOFF_HIGH ) - ANALOG_THRESHOLD_HIGH ) / ANALOG_ADJUSTED_RANGE : 0.0f;
   input->dpadIntensity[Button_Left] = leftIsDown ? 1.0f : 0.0f;
   input->dpadIntensity[Button_Up] = upIsDown ? 1.0f : 0.0f;
   input->dpadIntensity[Button_Right] = rightIsDown ? 1.0f : 0.0f;
   input->dpadIntensity[Button_Down] = downIsDown ? 1.0f : 0.0f;

   Input_UpdateButtonState( &( input->buttonStates[Button_A] ), digitalRead( PIN_A_BUTTON ) == LOW );
   Input_UpdateButtonState( &( input->buttonStates[Button_B] ), digitalRead( PIN_B_BUTTON ) == LOW );
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
