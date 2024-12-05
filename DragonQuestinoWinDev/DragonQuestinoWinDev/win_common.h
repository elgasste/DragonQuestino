#if !defined( WIN_COMMON_H )
#define WIN_COMMON_H

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "win_pixel_buffer.h"
#include "game.h"

#define STRING_SIZE_DEFAULT         1024
#define GRAPHICS_SCALE              3.0f

typedef struct DebugFlags_t
{
   Bool_t showDiagnostics;
}
DebugFlags_t;

typedef struct GlobalObjects_t
{
   HWND hWndMain;
   HFONT hFont;
   LARGE_INTEGER performanceFrequency;
   Game_t game;
   Bool_t shutdown;
   BITMAPINFO bmpInfo;   
   WinPixelBuffer_t screenBuffer;
   DebugFlags_t debugFlags;
}
GlobalObjects_t;

GlobalObjects_t g_globals;

#endif // WIN_COMMON_H
