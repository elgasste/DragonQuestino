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
}

void Input_Read( Input_t* input )
{
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( input );
#else
   uint8_t nesInput = Input_ReadNesController();

   Input_UpdateButtonState( &( input->buttonStates[ Button_A ] ), ( nesInput & INPUT_A_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ Button_B ] ), ( nesInput & INPUT_B_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ Button_Left ] ), ( nesInput & INPUT_LEFT_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ Button_Up ] ), ( nesInput & INPUT_UP_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ Button_Right ] ), ( nesInput & INPUT_RIGHT_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ Button_Down ] ), ( nesInput & INPUT_DOWN_FLAG ) ? True : False );
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
