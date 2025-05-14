#include <stdio.h>

#include "win_common.h"
#include "game.h"

internal void FatalError( const char* message );
internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam );
internal void InitButtonMap();
internal void HandleKeyboardInput( uint32_t keyCode, LPARAM flags );
internal void RenderScreen();
internal void DrawDiagnostics( HDC* dcMem );
internal void ToggleFastWalk();
internal void ToggleNoDark();
internal void ToggleNoEncounters();
internal void GetAllItems();
internal void MaxOutStats();

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

   expectedWindowRect.right = SCREEN_WIDTH;
   expectedWindowRect.bottom = SCREEN_HEIGHT;

   if ( !AdjustWindowRect( &expectedWindowRect, windowStyle, 0 ) )
   {
      FatalError( "failed to adjust window rect." );
   }

   clientPaddingRight = ( expectedWindowRect.right - expectedWindowRect.left ) - SCREEN_WIDTH;
   clientPaddingTop = ( expectedWindowRect.bottom - expectedWindowRect.top ) - SCREEN_HEIGHT;

   g_globals.hWndMain = CreateWindowExA( 0,
                                         mainWindowClass.lpszClassName,
                                         "Dragon Questino Windows Development Tool",
                                         windowStyle,
                                         CW_USEDEFAULT,
                                         CW_USEDEFAULT,
                                         (int)( SCREEN_WIDTH * GRAPHICS_SCALE ) + clientPaddingRight,
                                         (int)( SCREEN_HEIGHT * GRAPHICS_SCALE ) + clientPaddingTop,
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

   WinPixelBuffer_Init( &( g_globals.screenBuffer ), SCREEN_WIDTH, SCREEN_HEIGHT );
   g_globals.bmpInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
   g_globals.bmpInfo.bmiHeader.biWidth = g_globals.screenBuffer.w;
   g_globals.bmpInfo.bmiHeader.biHeight = -(LONG)( g_globals.screenBuffer.h );
   g_globals.bmpInfo.bmiHeader.biPlanes = 1;
   g_globals.bmpInfo.bmiHeader.biBitCount = 32;
   g_globals.bmpInfo.bmiHeader.biCompression = BI_RGB;

   InitButtonMap();

   Game_Init( &( g_globals.game ), g_globals.screenBuffer.memory16 );
   g_globals.shutdown = False;
   g_debugFlags.showDiagnostics = False;
   g_debugFlags.noClip = False;
   g_debugFlags.fastWalk = False;
   g_debugFlags.noDark = False;
   g_debugFlags.noEncounters = False;

   while ( 1 )
   {
      Clock_StartFrame( &( g_globals.game.clock ) );
      Input_ResetState( &( g_globals.game.input ) );
      
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
   uint32_t i;

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

         for ( i = 0; i < (uint32_t)Button_Count; i++ )
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
               TOGGLE_BOOL( g_debugFlags.showDiagnostics );
               break;
            case VK_NOCLIP:
               TOGGLE_BOOL( g_debugFlags.noClip );
               break;
            case VK_FASTWALK:
               ToggleFastWalk();
               break;
            case VK_NODARK:
               ToggleNoDark();
               break;
            case VK_ALLITEMS:
               GetAllItems();
               break;
            case VK_MAXSTATS:
               MaxOutStats();
               break;
            case VK_TOGGLECURSED:
               Player_SetCursed( &( g_globals.game.player ), g_globals.game.player.isCursed ? False : True );
               g_globals.game.screen.needsRedraw = True;
               break;
            case VK_TOGGLEENCOUNTERS:
               ToggleNoEncounters();
               break;
         }
      }
      else
      {
         for ( i = 0; i < (uint32_t)Button_Count; i++ )
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
   int winWidth = (int)( SCREEN_WIDTH * GRAPHICS_SCALE );
   int winHeight = (int)( SCREEN_HEIGHT * GRAPHICS_SCALE );

   dc = BeginPaint( g_globals.hWndMain, &ps );

   // create an off-screen DC for double-buffering
   dcMem = CreateCompatibleDC( dc );
   bmMem = CreateCompatibleBitmap( dc, winWidth, winHeight );
   hOld = SelectObject( dcMem, bmMem );

   // actually draw everything
   StretchDIBits(
      dcMem,
      0, 0, (int)( SCREEN_WIDTH * GRAPHICS_SCALE ), (int)( SCREEN_HEIGHT * GRAPHICS_SCALE ), // dest
      0, 0, g_globals.screenBuffer.w, g_globals.screenBuffer.h, // src
      g_globals.screenBuffer.memory32,
      &( g_globals.bmpInfo ),
      DIB_RGB_COLORS, SRCCOPY
   );

   if ( g_debugFlags.showDiagnostics )
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

   sprintf_s( str, STRING_SIZE_DEFAULT, "   Frame Rate: %u", CLOCK_FPS );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "Last Frame MS: %u", g_globals.game.clock.lastFrameMicro / 1000 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, " Total Frames: %u", g_globals.game.clock.frameCount );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   gameSeconds = g_globals.game.clock.frameCount / CLOCK_FPS;
   sprintf_s( str, STRING_SIZE_DEFAULT, " In-Game Time: %u:%02u:%02u", gameSeconds / 3600, gameSeconds / 60, gameSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   realSeconds = ( g_globals.game.clock.absoluteEndMicro - g_globals.game.clock.absoluteStartMicro ) / 1000000;
   sprintf_s( str, STRING_SIZE_DEFAULT, "    Real Time: %u:%02u:%02u", realSeconds / 3600, realSeconds / 60, realSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, " Map Viewport: %i, %i", g_globals.game.tileMap.viewport.x, g_globals.game.tileMap.viewport.y );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "   Player Pos: %u, %u", (uint32_t)( g_globals.game.player.sprite.position.x ), (uint32_t)( g_globals.game.player.sprite.position.y ) );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
   SetTextColor( *dcMem, g_globals.game.input.buttonStates[Button_Up].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "--" );
   SetTextColor( *dcMem, g_globals.game.input.buttonStates[Button_Left].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "   --" );
   SetTextColor( *dcMem, g_globals.game.input.buttonStates[Button_Right].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "      B" );
   SetTextColor( *dcMem, g_globals.game.input.buttonStates[Button_B].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "        A" );
   SetTextColor( *dcMem, g_globals.game.input.buttonStates[Button_A].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
   SetTextColor( *dcMem, g_globals.game.input.buttonStates[Button_Down].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, g_debugFlags.noClip ? 0x00FFFFFF : 0x00333333 );
   sprintf_s( str, STRING_SIZE_DEFAULT, "1: No clip" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, g_debugFlags.fastWalk ? 0x00FFFFFF : 0x00333333 );
   sprintf_s( str, STRING_SIZE_DEFAULT, "2: Fast walk" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, g_debugFlags.noDark ? 0x00FFFFFF : 0x00333333 );
   sprintf_s( str, STRING_SIZE_DEFAULT, "3: No dark" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, 0x00FFFFFF );
   sprintf_s( str, STRING_SIZE_DEFAULT, "4: Get all items" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, 0x00FFFFFF );
   sprintf_s( str, STRING_SIZE_DEFAULT, "5: Max out stats" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "6: Toggle cursed" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, g_debugFlags.noEncounters ? 0x00FFFFFF : 0x00333333 );
   sprintf_s( str, STRING_SIZE_DEFAULT, "7: No Encounters" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   SelectObject( *dcMem, oldFont );
}

internal void ToggleFastWalk()
{
   TOGGLE_BOOL( g_debugFlags.fastWalk );

   if ( g_debugFlags.fastWalk )
   {
      g_globals.game.player.maxVelocity = TILE_WALKSPEED_FAST;
   }
   else
   {
      g_globals.game.player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( g_globals.game.tileMap ), g_globals.game.player.tileIndex );
   }
}

internal void ToggleNoDark()
{
   TOGGLE_BOOL( g_debugFlags.noDark );
}

internal void ToggleNoEncounters()
{
   TOGGLE_BOOL( g_debugFlags.noEncounters );
}

internal void GetAllItems()
{
   ITEM_SET_KEYCOUNT( g_globals.game.player.items, ITEM_MAXKEYS );
   ITEM_SET_HERBCOUNT( g_globals.game.player.items, ITEM_MAXHERBS );
   ITEM_SET_WINGCOUNT( g_globals.game.player.items, ITEM_MAXWINGS );
   ITEM_SET_FAIRYWATERCOUNT( g_globals.game.player.items, ITEM_MAXFAIRYWATERS );
   ITEM_SET_TORCHCOUNT( g_globals.game.player.items, ITEM_MAXTORCHES );

   if ( !ITEM_HAS_FAIRYFLUTE( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASFAIRYFLUTE( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_SILVERHARP( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSILVERHARP( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_GWAELYNSLOVE( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASGWAELYNSLOVE( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_STONEOFSUNLIGHT( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSTONEOFSUNLIGHT( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_STAFFOFRAIN( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSTAFFOFRAIN( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_TOKEN( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASTOKEN( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_RAINBOWDROP( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASRAINBOWDROP( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_SPHEREOFLIGHT( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSPHEREOFLIGHT( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_DRAGONSCALE( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASDRAGONSCALE( g_globals.game.player.items );
   }
   if ( !ITEM_HAS_CURSEDBELT( g_globals.game.player.items ) )
   {
      ITEM_TOGGLE_HASCURSEDBELT( g_globals.game.player.items );
   }

   g_globals.game.screen.needsRedraw = True;
}

internal void MaxOutStats()
{
   uint32_t i;

   g_globals.game.player.experience = UINT16_MAX;
   g_globals.game.player.gold = UINT16_MAX;

   Player_RestoreHitPoints( &( g_globals.game.player ), UINT8_MAX );
   g_globals.game.player.stats.maxHitPoints = UINT8_MAX;
   g_globals.game.player.stats.magicPoints = UINT8_MAX;
   g_globals.game.player.stats.maxMagicPoints = UINT8_MAX;
   g_globals.game.player.stats.attackPower = UINT8_MAX;
   g_globals.game.player.stats.defensePower = UINT8_MAX;
   g_globals.game.player.stats.strength = UINT8_MAX;
   g_globals.game.player.stats.agility = UINT8_MAX;

   g_globals.game.player.spells = 0x3FF;
   g_globals.game.player.townsVisited = 0x3F;

   for ( i = 0; i < MenuId_Count; i++ )
   {
      Menu_Reset( &( g_globals.game.menus[i] ) );
   }

   g_globals.game.screen.needsRedraw = True;
}
