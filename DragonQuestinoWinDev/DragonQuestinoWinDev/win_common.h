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

#define WIN_SAVE_FILE_NAME          "win_save.dat"

#if defined ( _DEBUG )
#define GRAPHICS_SCALE_DEFAULT      3.0f
#else
#define GRAPHICS_SCALE_DEFAULT      2.0f
#endif 

#define VK_NOCLIP                   49    // 1
#define VK_FASTWALK                 50    // 2
#define VK_NODARK                   51    // 3
#define VK_ALLITEMS                 52    // 4
#define VK_MAXSTATS                 53    // 5
#define VK_TOGGLECURSED             54    // 6
#define VK_TOGGLEENCOUNTERS         55    // 7
#define VK_TOGGLETILEDAMAGE         56    // 8
#define VK_TOGGLESHOWHITBOXES       57    // 9

#define VK_GRAPHICS_SCALE_1         49    // 1
#define VK_GRAPHICS_SCALE_2         50    // 2
#define VK_GRAPHICS_SCALE_3         51    // 3
#define VK_GRAPHICS_SCALE_4         52    // 4

typedef struct WinGlobalObjects_t
{
   HWND hWndMain;
   LONG clientPaddingRight;
   LONG clientPaddingTop;
   HFONT hFont;
   LARGE_INTEGER performanceFrequency;
   u32 buttonMap[Button_Count];
   Game_t game;
   Bool_t shutdown;
   BITMAPINFO bmpInfo;   
   WinPixelBuffer_t screenBuffer;
   r32 graphicsScale;
   char savedPassword[PASSWORD_LENGTH + 1];
}
WinGlobalObjects_t;

WinGlobalObjects_t g_winGlobals;

#endif // WIN_COMMON_H
