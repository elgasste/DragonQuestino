#if !defined( COMMON_H )
#define COMMON_H

// ARDUINO DEBUG FLAGS
// un-comment to get serial logging
#define DEBUG_SERIAL

#if !defined( VISUAL_STUDIO_DEV )

#include <Arduino.h>

#if defined( DEBUG_SERIAL )
#define SERIAL_BAUD 115200
#endif

#endif // !VISUAL_STUDIO_DEV

#include <stdint.h>
#include <memory.h>
#include <limits.h>

#define internal static
#define global static
#define local_persist static

#define Bool_t int32_t
#define True 1
#define False 0

#define UNUSED_PARAM( x )              (void)x
#define TOGGLE_BOOL( b )               b = b ? False : True;

#if defined( VISUAL_STUDIO_DEV )
#define DELAY_MS( x )                  Sleep( x )
#pragma warning( disable: 4996 )  // this avoids having to use sprintf_s
#else
#define DELAY_MS( x )                  delay( x )
#endif

#if defined( __cplusplus )
extern "C" {
#endif

void Program_Log( const char* msg );

#if defined( __cplusplus )
}
#endif

#endif // COMMON_H