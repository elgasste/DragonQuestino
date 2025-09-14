#if !defined( INPUT_H )
#define INPUT_H

#include "common.h"
#include "vector.h"

#define INPUT_A_FLAG             0x01
#define INPUT_B_FLAG             0x02
#define INPUT_SELECT_FLAG        0x04
#define INPUT_START_FLAG         0x08
#define INPUT_UP_FLAG            0x10
#define INPUT_DOWN_FLAG          0x20
#define INPUT_LEFT_FLAG          0x40
#define INPUT_RIGHT_FLAG         0x80

#define INPUT_NES_DATA_PIN       4
#define INPUT_NES_CLOCK_PIN      2
#define INPUT_NES_LATCH_PIN      3

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
void Input_ButtonPressed( Input_t* input, u32 button );
void Input_ButtonReleased( Input_t* input, u32 button );
#else
u8 Input_ReadNesController();
#endif

#if defined( __cplusplus )
}
#endif

#endif // INPUT_H
