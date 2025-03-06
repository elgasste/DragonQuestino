#include "input.h"
#include "math.h"

internal void Input_UpdateButtonState( ButtonState_t* buttonState, Bool_t down );

void Input_Init( Input_t* input )
{
   uint32_t i;
   int32_t totalX, totalY;

   for ( i = 0; i < Button_Count; i++ )
   {
      input->buttonStates[i].pressed = False;
      input->buttonStates[i].released = False;
      input->buttonStates[i].down = False;
   }

#if !defined( VISUAL_STUDIO_DEV )
   analogReadResolution( INPUT_ANALOG_BITS );

   for ( i = 0, totalX = 0, totalY = 0; i < INPUT_ANALOG_POLL_COUNT; i++ )
   {
      totalX += analogRead( PIN_ANALOG_X );
      totalY += analogRead( PIN_ANALOG_Y );
      DELAY_MS( 1 );
   }

   input->analogRestingState.x = totalX / INPUT_ANALOG_POLL_COUNT;
   input->analogRestingState.y = totalY / INPUT_ANALOG_POLL_COUNT;
   input->analogLowRange.x = input->analogRestingState.x - INPUT_ANALOG_THRESHOLD - INPUT_ANALOG_CUTOFF_LOW;
   input->analogLowRange.y = input->analogRestingState.y - INPUT_ANALOG_THRESHOLD - INPUT_ANALOG_CUTOFF_LOW;
   input->analogHighRange.x = INPUT_ANALOG_CUTOFF_HIGH - ( input->analogRestingState.x + INPUT_ANALOG_THRESHOLD );
   input->analogHighRange.y = INPUT_ANALOG_CUTOFF_HIGH - ( input->analogRestingState.y + INPUT_ANALOG_THRESHOLD );
#endif
}

void Input_Read( Input_t* input )
{
#if defined( VISUAL_STUDIO_DEV )
   input->analogIntensity[Direction_Left] = input->buttonStates[Button_Left].down ? 1.0f : 0.0f;
   input->analogIntensity[Direction_Up] = input->buttonStates[Button_Up].down ? 1.0f : 0.0f;
   input->analogIntensity[Direction_Right] = input->buttonStates[Button_Right].down ? 1.0f : 0.0f;
   input->analogIntensity[Direction_Down] = input->buttonStates[Button_Down].down ? 1.0f : 0.0f;
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
   // - X is between restingState and max: left
   // - X is between 0 and restingState: right
   // - Y is between 0 and restingState: up
   // - Y is between restingState and max: down

   input->analogIntensity[Button_Left] = leftIsDown ? (float)( MATH_MIN( xValue, INPUT_ANALOG_CUTOFF_HIGH ) - INPUT_ANALOG_THRESHOLD - input->analogRestingState.x ) / input->analogHighRange.x : 0.0f;
   input->analogIntensity[Button_Right] = rightIsDown ? 1.0f - ( (float)( MATH_MAX( xValue, INPUT_ANALOG_CUTOFF_LOW ) - INPUT_ANALOG_CUTOFF_LOW ) / input->analogLowRange.x ) : 0.0f;
   input->analogIntensity[Button_Up] = upIsDown ? 1.0f - ( (float)( MATH_MAX( yValue, INPUT_ANALOG_CUTOFF_LOW ) - INPUT_ANALOG_CUTOFF_LOW ) / input->analogLowRange.y ) : 0.0f;
   input->analogIntensity[Button_Down] = downIsDown ? (float)( MATH_MIN( yValue, INPUT_ANALOG_CUTOFF_HIGH ) - INPUT_ANALOG_THRESHOLD - input->analogRestingState.y ) / input->analogHighRange.y : 0.0f;

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
