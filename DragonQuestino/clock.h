#if !defined( CLOCK_H )
#define CLOCK_H

#include "common.h"

#define CLOCK_FPS                 60
#define CLOCK_FRAME_MICROSECONDS  16666
#define CLOCK_FRAME_SECONDS       0.01666666f

typedef struct Clock_t
{
   uint32_t frameStartMicro;

#if defined( VISUAL_STUDIO_DEV )
   uint32_t absoluteStartMicro;
   uint32_t absoluteEndMicro;
   uint32_t lastFrameMicro;
   uint32_t frameCount;
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
