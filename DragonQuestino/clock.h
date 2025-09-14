#if !defined( CLOCK_H )
#define CLOCK_H

#include "common.h"

#define CLOCK_FPS                 60
#define CLOCK_FRAME_MICROSECONDS  16666
#define CLOCK_FRAME_SECONDS       0.01666666f

typedef struct Clock_t
{
   u32 frameStartMicro;
   u32 lagFrameCount;

#if defined( VISUAL_STUDIO_DEV )
   u32 absoluteStartMicro;
   u32 absoluteEndMicro;
   u32 lastFrameMicro;
   u32 frameCount;
#endif
}
Clock_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Clock_Init( Clock_t* clock );
void Clock_StartFrame( Clock_t* clock );
void Clock_EndFrame( Clock_t* clock );

#if defined( __cplusplus )
}
#endif

#endif // CLOCK_H
