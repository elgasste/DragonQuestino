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

#define VK_NOCLIP                   49    // 1
#define VK_FASTWALK                 50    // 2
#define VK_NODARK                   51    // 3
#define VK_ALLITEMS                 52    // 4
#define VK_MAXSTATS                 53    // 5
#define VK_TOGGLECURSED             54    // 6
#define VK_TOGGLEENCOUNTERS         55    // 7
#define VK_TOGGLETILEDAMAGE         56    // 8
#define VK_TOGGLESHOWHITBOXES       57    // 9

typedef struct GlobalObjects_t
{
   HWND hWndMain;
   HFONT hFont;
   LARGE_INTEGER performanceFrequency;
   u32 buttonMap[Button_Count];
   Game_t game;
   Bool_t shutdown;
   BITMAPINFO bmpInfo;   
   WinPixelBuffer_t screenBuffer;
}
GlobalObjects_t;

GlobalObjects_t g_globals;

#endif // WIN_COMMON_H
