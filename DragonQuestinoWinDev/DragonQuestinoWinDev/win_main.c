#include <stdio.h>

#include "win_common.h"
#include "game.h"

internal void FatalError( const char* message );
internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam );
internal void InitButtonMap();
internal void HandleKeyboardInput( uint32_t keyCode, LPARAM flags );
internal void RenderScreen();
internal void DrawDiagnostics( HDC* dcMem );

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
   TIMECAPS timeCaps;
   UINT timerResolution;
   WNDCLASSA mainWindowClass = { 0 };
   DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
   RECT expectedWindowRect = { 0 };
   LONG clientPaddingRight, clientPaddingTop;
   MSG msg;

   UNUSED_PARAM( hPrevInstance );
   UNUSED_PARAM( lpCmdLine );
   UNUSED_PARAM( nCmdShow );

   if ( !QueryPerformanceFrequency( &( g_globals.performanceFrequency ) ) )
   {
      FatalError( "failed to query performance frequency." );
   }

   if ( timeGetDevCaps( &timeCaps, sizeof( TIMECAPS ) ) != TIMERR_NOERROR )
   {
      FatalError( "failed to set timer resolution." );
   }

   timerResolution = min( max( timeCaps.wPeriodMin, 1 ), timeCaps.wPeriodMax );
   timeBeginPeriod( timerResolution );

   mainWindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
   mainWindowClass.lpfnWndProc = MainWindowProc;
   mainWindowClass.hInstance = hInstance;
   mainWindowClass.lpszClassName = "mainWindowClass";

   if ( !RegisterClassA( &mainWindowClass ) )
   {
      FatalError( "failed to register window class." );
   }

   expectedWindowRect.right = SCREEN_BUFFER_WIDTH;
   expectedWindowRect.bottom = SCREEN_BUFFER_HEIGHT;

   if ( !AdjustWindowRect( &expectedWindowRect, windowStyle, 0 ) )
   {
      FatalError( "failed to adjust window rect." );
   }

   clientPaddingRight = ( expectedWindowRect.right - expectedWindowRect.left ) - SCREEN_BUFFER_WIDTH;
   clientPaddingTop = ( expectedWindowRect.bottom - expectedWindowRect.top ) - SCREEN_BUFFER_HEIGHT;

   g_globals.hWndMain = CreateWindowExA( 0,
                                         mainWindowClass.lpszClassName,
                                         "Final Questino Windows Development Tool",
                                         windowStyle,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         (int)( SCREEN_BUFFER_WIDTH * GRAPHICS_SCALE ) + clientPaddingRight,
                                         (int)( SCREEN_BUFFER_HEIGHT * GRAPHICS_SCALE ) + clientPaddingTop,
                                         0,
                                         0,
                                         hInstance,
                                         0 );

   if ( !g_globals.hWndMain )
   {
      FatalError( "failed to create main window." );
   }

   SetCursor( LoadCursor( 0, IDC_ARROW ) );

   g_globals.hFont = CreateFontA( 16, 0, 0, 0,
                                  FW_BOLD, FALSE, FALSE, FALSE,
                                  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
                                  "Consolas" );

   WinPixelBuffer_Init( &( g_globals.screenBuffer ), SCREEN_BUFFER_WIDTH, SCREEN_BUFFER_HEIGHT );
   g_globals.bmpInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
   g_globals.bmpInfo.bmiHeader.biWidth = g_globals.screenBuffer.w;
   g_globals.bmpInfo.bmiHeader.biHeight = -(LONG)( g_globals.screenBuffer.h );
   g_globals.bmpInfo.bmiHeader.biPlanes = 1;
   g_globals.bmpInfo.bmiHeader.biBitCount = 32;
   g_globals.bmpInfo.bmiHeader.biCompression = BI_RGB;

   InitButtonMap();

   Game_Init( &( g_globals.game ) );
   g_globals.shutdown = False;
   g_globals.debugFlags.showDiagnostics = False;

   while ( 1 )
   {
      Clock_StartFrame( &( g_globals.game.clock ) );
      
      while ( PeekMessageA( &msg, g_globals.hWndMain, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessageA( &msg );
      }

      Game_Tic( &( g_globals.game ) );

      InvalidateRect( g_globals.hWndMain, 0, FALSE );
      Clock_EndFrame( &( g_globals.game.clock ) );

      if ( g_globals.shutdown )
      {
         break;
      }
   }

   WinPixelBuffer_CleanUp( &( g_globals.screenBuffer ) );
   return 0;
}

void Program_Log( const char* msg )
{
   OutputDebugStringA( msg );
}

internal void FatalError( const char* message )
{
   char errorMsg[STRING_SIZE_DEFAULT];
   snprintf( errorMsg, STRING_SIZE_DEFAULT, "Windows error: %s", message );
   MessageBoxA( 0, message, "Error", MB_OK | MB_ICONERROR );
   exit( 1 );
}

internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam )
{
   LRESULT result = 0;

   switch ( uMsg )
   {
      case WM_QUIT:
      case WM_CLOSE:
      case WM_DESTROY:
         g_globals.shutdown = True;
         break;
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
         HandleKeyboardInput( (uint32_t)wParam, lParam );
         break;
      case WM_PAINT:
         RenderScreen();
         break;
      default:
         result = DefWindowProcA( hWnd, uMsg, wParam, lParam );
   }

   return result;
}

internal void InitButtonMap()
{
   g_globals.buttonMap[Button_Left] = VK_LEFT;
   g_globals.buttonMap[Button_Up] = VK_UP;
   g_globals.buttonMap[Button_Right] = VK_RIGHT;
   g_globals.buttonMap[Button_Down] = VK_DOWN;
   g_globals.buttonMap[Button_A] = 0x58; // X
   g_globals.buttonMap[Button_B] = 0x5A; // Z
}

internal void HandleKeyboardInput( uint32_t keyCode, LPARAM flags )
{
   Bool_t keyWasDown = ( flags & ( (LONG_PTR)1 << 30 ) ) != 0 ? True : False;
   Bool_t keyIsDown = ( flags & ( (LONG_PTR)1 << 31 ) ) == 0 ? True : False;
   uint8_t i;

   // ignore repeat presses
   if ( keyWasDown != keyIsDown )
   {
      if ( keyIsDown )
      {
         // ensure alt+F4 still closes the window
         if ( keyCode == VK_F4 && ( flags & ( (LONG_PTR)1 << 29 ) ) )
         {
            g_globals.shutdown = True;
            return;
         }

         for ( i = 0; i < Button_Count; i++ )
         {
            if ( g_globals.buttonMap[i] == keyCode )
            {
               Input_ButtonPressed( &( g_globals.game.input ), i );
               break;
            }
         }

         // Windows-specific diagnostic/debug keys
         switch ( keyCode )
         {
            case VK_F8:
               TOGGLE_BOOL( g_globals.debugFlags.showDiagnostics );
               break;
         }
      }
      else
      {
         for ( i = 0; i < Button_Count; i++ )
         {
            if ( g_globals.buttonMap[i] == keyCode )
            {
               Input_ButtonReleased( &( g_globals.game.input ), i );
               break;
            }
         }
      }
   }
}

// the double-buffering part of this came from Stack Overflow
internal void RenderScreen()
{
   HDC dc, dcMem;
   HBITMAP bmMem;
   HANDLE hOld;
   PAINTSTRUCT ps;
   int winWidth = (int)( SCREEN_BUFFER_WIDTH * GRAPHICS_SCALE );
   int winHeight = (int)( SCREEN_BUFFER_HEIGHT * GRAPHICS_SCALE );

   dc = BeginPaint( g_globals.hWndMain, &ps );

   // create an off-screen DC for double-buffering
   dcMem = CreateCompatibleDC( dc );
   bmMem = CreateCompatibleBitmap( dc, winWidth, winHeight );
   hOld = SelectObject( dcMem, bmMem );

   // actually draw everything
   StretchDIBits(
      dcMem,
      0, 0, (int)( SCREEN_BUFFER_WIDTH * GRAPHICS_SCALE ), (int)( SCREEN_BUFFER_HEIGHT * GRAPHICS_SCALE ), // dest
      0, 0, g_globals.screenBuffer.w, g_globals.screenBuffer.h, // src
      g_globals.screenBuffer.memory,
      &( g_globals.bmpInfo ),
      DIB_RGB_COLORS, SRCCOPY
   );

   if ( g_globals.debugFlags.showDiagnostics )
   {
      DrawDiagnostics( &dcMem );
   }

   // transfer the off-screen DC to the screen
   BitBlt( dc, 0, 0, winWidth, winHeight, dcMem, 0, 0, SRCCOPY );

   SelectObject( dcMem, hOld );
   DeleteObject( bmMem );
   DeleteDC( dcMem );
   EndPaint( g_globals.hWndMain, &ps );
}

internal void DrawDiagnostics( HDC* dcMem )
{
   uint32_t gameSeconds, realSeconds;
   RECT r = { 10, 10, 0, 0 };
   char str[STRING_SIZE_DEFAULT];
   HFONT oldFont = (HFONT)SelectObject( *dcMem, g_globals.hFont );

   SetTextColor( *dcMem, 0x00FFFFFF );
   SetBkMode( *dcMem, TRANSPARENT );

   sprintf_s( str, STRING_SIZE_DEFAULT, "   Frame Rate: %u", GAME_FPS );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "Last Frame MS: %u", g_globals.game.clock.lastFrameMicro / 1000 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, " Total Frames: %u", g_globals.game.clock.frameCount );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   gameSeconds = g_globals.game.clock.frameCount / GAME_FPS;
   sprintf_s( str, STRING_SIZE_DEFAULT, " In-Game Time: %u:%02u:%02u", gameSeconds / 3600, gameSeconds / 60, gameSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   realSeconds = ( g_globals.game.clock.absoluteEndMicro - g_globals.game.clock.absoluteStartMicro ) / 1000000;
   sprintf_s( str, STRING_SIZE_DEFAULT, "    Real Time: %u:%02u:%02u", realSeconds / 3600, realSeconds / 60, realSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "Map Offset X: %i", g_globals.game.tileMapViewport.x );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "Map Offset Y: %i", g_globals.game.tileMapViewport.y );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   if ( g_globals.game.input.buttonStates[Button_Up].down )
   {
      sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
      DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   }
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "         " );
   if ( g_globals.game.input.buttonStates[Button_Left].down ) str[0] = '-';
   if ( g_globals.game.input.buttonStates[Button_Left].down ) str[1] = '-';
   if ( g_globals.game.input.buttonStates[Button_Right].down ) str[3] = '-';
   if ( g_globals.game.input.buttonStates[Button_Right].down ) str[4] = '-';
   if ( g_globals.game.input.buttonStates[Button_B].down ) str[6] = 'B';
   if ( g_globals.game.input.buttonStates[Button_A].down ) str[8] = 'A';
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   if ( g_globals.game.input.buttonStates[Button_Down].down )
   {
      sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
      DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   }

   SelectObject( *dcMem, oldFont );
}
