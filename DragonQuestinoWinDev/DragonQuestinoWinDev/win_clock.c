#include "clock.h"
#include "win_common.h"

internal uint32_t Clock_GetMicros();

void Clock_Init( Clock_t* clock )
{
   clock->absoluteStartMicro = Clock_GetMicros();
   clock->absoluteEndMicro = Clock_GetMicros();
   clock->frameStartMicro = 0;
   clock->lastFrameMicro = 0;
   clock->frameCount = 0;
   clock->lagFrameCount = 0;
}

void Clock_StartFrame( Clock_t* clock )
{
   local_persist Bool_t hasStarted = False;

   clock->frameStartMicro = Clock_GetMicros();

   if ( !hasStarted )
   {
      clock->absoluteStartMicro = clock->frameStartMicro;
      hasStarted = True;
   }
}

void Clock_EndFrame( Clock_t* clock )
{
   uint32_t frameEndMicro = Clock_GetMicros();
   uint32_t elapsedMicro;

   clock->frameCount++;
   clock->absoluteEndMicro = frameEndMicro;

   if ( frameEndMicro < clock->frameStartMicro )
   {
      // this means the timer has overflowed (happens after ~70 mins on Arduino)
      elapsedMicro = frameEndMicro + ( ULONG_MAX - clock->frameStartMicro );
   }
   else
   {
      elapsedMicro = frameEndMicro - clock->frameStartMicro;
   }

   clock->lastFrameMicro = elapsedMicro;

   if ( elapsedMicro <= CLOCK_FRAME_MICROSECONDS )
   {
      DELAY_MS( ( CLOCK_FRAME_MICROSECONDS - elapsedMicro ) / 1000 );
   }
   else
   {
      clock->lagFrameCount++;
   }
}

internal uint32_t Clock_GetMicros()
{
   LARGE_INTEGER ticks;
   QueryPerformanceCounter( &ticks );
   return (uint32_t)( ( (double)( ticks.QuadPart ) / (double)( g_globals.performanceFrequency.QuadPart ) ) * (uint64_t)1000000 );
}
