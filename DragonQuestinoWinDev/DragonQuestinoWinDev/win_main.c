#include <stdio.h>

#include "win_common.h"
#include "game.h"
#include "tables.h"

internal void FatalError( const char* message );
internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam );
internal void InitButtonMap();
internal void HandleKeyboardInput( u32 keyCode, LPARAM flags );
internal void RenderScreen();
internal void DrawDiagnostics( HDC* dcMem );
internal void ToggleFastWalk();
internal void ToggleNoDark();
internal void ToggleNoEncounters();
internal void ToggleTileDamage();
internal void ToggleShowHitBoxes();
internal void GetAllItems();
internal void MaxOutStats();
internal void ScaleScreen( r32 scale );
internal void LoadSavedPassword();

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
   TIMECAPS timeCaps;
   UINT timerResolution;
   WNDCLASSA mainWindowClass = { 0 };
   DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
   RECT expectedWindowRect = { 0 };
   MSG msg;
   char windowTitle[128];

   UNUSED_PARAM( hPrevInstance );
   UNUSED_PARAM( lpCmdLine );
   UNUSED_PARAM( nCmdShow );

   if ( !QueryPerformanceFrequency( &( g_winGlobals.performanceFrequency ) ) )
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

   g_winGlobals.clientPaddingRight = ( expectedWindowRect.right - expectedWindowRect.left ) - SCREEN_WIDTH;
   g_winGlobals.clientPaddingTop = ( expectedWindowRect.bottom - expectedWindowRect.top ) - SCREEN_HEIGHT;

#if defined( _DEBUG )
   sprintf( windowTitle, "Dragon Questino Windows Development Tool" );
#else
   sprintf( windowTitle, "Dragon Questino" );
#endif

   g_winGlobals.graphicsScale = GRAPHICS_SCALE_DEFAULT;

   g_winGlobals.hWndMain = CreateWindowExA( 0,
                                            mainWindowClass.lpszClassName,
                                            windowTitle,
                                            windowStyle,
                                            CW_USEDEFAULT,
                                            CW_USEDEFAULT,
                                            (int)( SCREEN_WIDTH * g_winGlobals.graphicsScale ) + g_winGlobals.clientPaddingRight,
                                            (int)( SCREEN_HEIGHT * g_winGlobals.graphicsScale ) + g_winGlobals.clientPaddingTop,
                                            0,
                                            0,
                                            hInstance,
                                            0 );

   if ( !g_winGlobals.hWndMain )
   {
      FatalError( "failed to create main window." );
   }

   SetCursor( LoadCursor( 0, IDC_ARROW ) );

   g_winGlobals.hFont = CreateFontA( 16, 0, 0, 0,
                                     FW_BOLD, FALSE, FALSE, FALSE,
                                     ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
                                     "Consolas" );

   WinPixelBuffer_Init( &( g_winGlobals.screenBuffer ), SCREEN_WIDTH, SCREEN_HEIGHT );
   g_winGlobals.bmpInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
   g_winGlobals.bmpInfo.bmiHeader.biWidth = g_winGlobals.screenBuffer.w;
   g_winGlobals.bmpInfo.bmiHeader.biHeight = -(LONG)( g_winGlobals.screenBuffer.h );
   g_winGlobals.bmpInfo.bmiHeader.biPlanes = 1;
   g_winGlobals.bmpInfo.bmiHeader.biBitCount = 32;
   g_winGlobals.bmpInfo.bmiHeader.biCompression = BI_RGB;

   InitButtonMap();
   LoadSavedPassword();

   Game_Init( &( g_winGlobals.game ), g_winGlobals.screenBuffer.memory16 );
   g_winGlobals.shutdown = False;
   g_debugFlags.showDiagnostics = False;
   g_debugFlags.noClip = False;
   g_debugFlags.fastWalk = False;
   g_debugFlags.noDark = False;
   g_debugFlags.noEncounters = False;
   g_debugFlags.noTileDamage = False;
   g_debugFlags.showHitBoxes = False;

   while ( 1 )
   {
      Clock_StartFrame( &( g_winGlobals.game.clock ) );
      Input_ResetState( &( g_winGlobals.game.input ) );
      
      while ( PeekMessageA( &msg, g_winGlobals.hWndMain, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessageA( &msg );
      }

      Game_Tic( &( g_winGlobals.game ) );

      InvalidateRect( g_winGlobals.hWndMain, 0, FALSE );
      Clock_EndFrame( &( g_winGlobals.game.clock ) );

      if ( g_winGlobals.shutdown )
      {
         break;
      }
   }

   WinPixelBuffer_CleanUp( &( g_winGlobals.screenBuffer ) );
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
         g_winGlobals.shutdown = True;
         break;
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
         HandleKeyboardInput( (u32)wParam, lParam );
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
   g_winGlobals.buttonMap[Button_Left] = VK_LEFT;
   g_winGlobals.buttonMap[Button_Up] = VK_UP;
   g_winGlobals.buttonMap[Button_Right] = VK_RIGHT;
   g_winGlobals.buttonMap[Button_Down] = VK_DOWN;
   g_winGlobals.buttonMap[Button_A] = 0x58; // X
   g_winGlobals.buttonMap[Button_B] = 0x5A; // Z
   g_winGlobals.buttonMap[Button_Start] = VK_RETURN;
}

internal void HandleKeyboardInput( u32 keyCode, LPARAM flags )
{
   Bool_t keyWasDown = ( flags & ( (LONG_PTR)1 << 30 ) ) != 0 ? True : False;
   Bool_t keyIsDown = ( flags & ( (LONG_PTR)1 << 31 ) ) == 0 ? True : False;
   u32 i;

   // ignore repeat presses
   if ( keyWasDown != keyIsDown )
   {
      if ( keyIsDown )
      {
         // ensure alt+F4 still closes the window
         if ( keyCode == VK_F4 && ( flags & ( (LONG_PTR)1 << 29 ) ) )
         {
            g_winGlobals.shutdown = True;
            return;
         }

         for ( i = 0; i < (u32)Button_Count; i++ )
         {
            if ( g_winGlobals.buttonMap[i] == keyCode )
            {
               Input_ButtonPressed( &( g_winGlobals.game.input ), i );
               break;
            }
         }

         // Windows-specific diagnostic/debug keys
#if defined( _DEBUG )
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
               Player_SetCursed( &( g_winGlobals.game.player ), g_winGlobals.game.player.isCursed ? False : True );
               break;
            case VK_TOGGLEENCOUNTERS:
               ToggleNoEncounters();
               break;
            case VK_TOGGLETILEDAMAGE:
               ToggleTileDamage();
               break;
            case VK_TOGGLESHOWHITBOXES:
               ToggleShowHitBoxes();
               break;
         }
#else
         switch ( keyCode )
         {
            case VK_GRAPHICS_SCALE_1: ScaleScreen( 1.0f ); break;
            case VK_GRAPHICS_SCALE_2: ScaleScreen( 2.0f ); break;
            case VK_GRAPHICS_SCALE_3: ScaleScreen( 3.0f ); break;
            case VK_GRAPHICS_SCALE_4: ScaleScreen( 4.0f ); break;
         }
#endif
      }
      else
      {
         for ( i = 0; i < (u32)Button_Count; i++ )
         {
            if ( g_winGlobals.buttonMap[i] == keyCode )
            {
               Input_ButtonReleased( &( g_winGlobals.game.input ), i );
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
   int winWidth = (int)( SCREEN_WIDTH * g_winGlobals.graphicsScale );
   int winHeight = (int)( SCREEN_HEIGHT * g_winGlobals.graphicsScale );

   dc = BeginPaint( g_winGlobals.hWndMain, &ps );

   // create an off-screen DC for double-buffering
   dcMem = CreateCompatibleDC( dc );
   bmMem = CreateCompatibleBitmap( dc, winWidth, winHeight );
   hOld = SelectObject( dcMem, bmMem );

   // actually draw everything
   StretchDIBits(
      dcMem,
      0, 0, (int)( SCREEN_WIDTH * g_winGlobals.graphicsScale ), (int)( SCREEN_HEIGHT * g_winGlobals.graphicsScale ), // dest
      0, 0, g_winGlobals.screenBuffer.w, g_winGlobals.screenBuffer.h, // src
      g_winGlobals.screenBuffer.memory32,
      &( g_winGlobals.bmpInfo ),
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
   EndPaint( g_winGlobals.hWndMain, &ps );
}

internal void DrawDiagnostics( HDC* dcMem )
{
   u32 gameSeconds, realSeconds;
   RECT r = { 10, 10, 0, 0 };
   char str[STRING_SIZE_DEFAULT];
   HFONT oldFont = (HFONT)SelectObject( *dcMem, g_winGlobals.hFont );

   SetTextColor( *dcMem, 0x00FFFFFF );
   SetBkMode( *dcMem, TRANSPARENT );

   sprintf_s( str, STRING_SIZE_DEFAULT, "   Frame Rate: %u", CLOCK_FPS );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "Last Frame MS: %u", g_winGlobals.game.clock.lastFrameMicro / 1000 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, " Total Frames: %u", g_winGlobals.game.clock.frameCount );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   gameSeconds = g_winGlobals.game.clock.frameCount / CLOCK_FPS;
   sprintf_s( str, STRING_SIZE_DEFAULT, " In-Game Time: %u:%02u:%02u", gameSeconds / 3600, gameSeconds / 60, gameSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   realSeconds = ( g_winGlobals.game.clock.absoluteEndMicro - g_winGlobals.game.clock.absoluteStartMicro ) / 1000000;
   sprintf_s( str, STRING_SIZE_DEFAULT, "    Real Time: %u:%02u:%02u", realSeconds / 3600, realSeconds / 60, realSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, " Map Viewport: %i, %i", g_winGlobals.game.tileMap.viewport.x, g_winGlobals.game.tileMap.viewport.y );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "   Player Pos: %u, %u", (u32)( g_winGlobals.game.player.sprite.position.x ), (u32)( g_winGlobals.game.player.sprite.position.y ) );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "   Tile Index: %u", g_winGlobals.game.player.tileIndex );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[Button_Up].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "--" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[Button_Left].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "   --" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[Button_Right].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "      B" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[Button_B].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "        A" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[Button_A].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[Button_Down].down ? 0x00FFFFFF : 0x00333333 );
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
   sprintf_s( str, STRING_SIZE_DEFAULT, "7: No encounters" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, g_debugFlags.noTileDamage ? 0x00FFFFFF : 0x00333333 );
   sprintf_s( str, STRING_SIZE_DEFAULT, "8: No tile damage" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, g_debugFlags.showHitBoxes ? 0x00FFFFFF : 0x00333333 );
   sprintf_s( str, STRING_SIZE_DEFAULT, "9: Show hit boxes" );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   SelectObject( *dcMem, oldFont );
}

internal void ToggleFastWalk()
{
   TOGGLE_BOOL( g_debugFlags.fastWalk );

   if ( g_debugFlags.fastWalk )
   {
      g_winGlobals.game.player.maxVelocity = TILE_WALKSPEED_FAST;
   }
   else
   {
      g_winGlobals.game.player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( g_winGlobals.game.tileMap ), g_winGlobals.game.player.tileIndex );
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

internal void ToggleTileDamage()
{
   TOGGLE_BOOL( g_debugFlags.noTileDamage );
}

internal void ToggleShowHitBoxes()
{
   TOGGLE_BOOL( g_debugFlags.showHitBoxes );
}

internal void GetAllItems()
{
   ITEM_SET_KEYCOUNT( g_winGlobals.game.player.items, ITEM_MAXKEYS );
   ITEM_SET_HERBCOUNT( g_winGlobals.game.player.items, ITEM_MAXHERBS );
   ITEM_SET_WINGCOUNT( g_winGlobals.game.player.items, ITEM_MAXWINGS );
   ITEM_SET_FAIRYWATERCOUNT( g_winGlobals.game.player.items, ITEM_MAXFAIRYWATERS );
   ITEM_SET_TORCHCOUNT( g_winGlobals.game.player.items, ITEM_MAXTORCHES );

   if ( !ITEM_HAS_FAIRYFLUTE( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASFAIRYFLUTE( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_SILVERHARP( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSILVERHARP( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_GWAELYNSLOVE( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASGWAELYNSLOVE( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_STONEOFSUNLIGHT( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSTONEOFSUNLIGHT( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_STAFFOFRAIN( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSTAFFOFRAIN( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_TOKEN( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASTOKEN( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_RAINBOWDROP( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASRAINBOWDROP( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_SPHEREOFLIGHT( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASSPHEREOFLIGHT( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_DRAGONSCALE( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASDRAGONSCALE( g_winGlobals.game.player.items );
   }
   if ( !ITEM_HAS_CURSEDBELT( g_winGlobals.game.player.items ) )
   {
      ITEM_TOGGLE_HASCURSEDBELT( g_winGlobals.game.player.items );
   }
}

internal void MaxOutStats()
{
   u32 i;

   g_winGlobals.game.player.level = STAT_TABLE_SIZE - 1;
   g_winGlobals.game.player.experience = UINT16_MAX;
   g_winGlobals.game.player.gold = UINT16_MAX;

   g_winGlobals.game.player.stats.maxHitPoints = UINT8_MAX;
   Player_RestoreHitPoints( &( g_winGlobals.game.player ), UINT8_MAX );
   g_winGlobals.game.player.stats.magicPoints = UINT8_MAX;
   g_winGlobals.game.player.stats.maxMagicPoints = UINT8_MAX;
   g_winGlobals.game.player.stats.strength = UINT8_MAX;
   g_winGlobals.game.player.stats.agility = UINT8_MAX;
   g_winGlobals.game.player.stats.sleepResist = 15;
   g_winGlobals.game.player.stats.stopSpellResist = 15;
   g_winGlobals.game.player.stats.hurtResist = 15;
   g_winGlobals.game.player.stats.dodge = 2;

   g_winGlobals.game.player.spells = 0x3FF;
   g_winGlobals.game.player.townsVisited = 0x3F;

   for ( i = 0; i < MenuId_Count; i++ )
   {
      Menu_Reset( &( g_winGlobals.game.menus[i] ) );
   }
}

internal void ScaleScreen( r32 scale )
{
   g_winGlobals.graphicsScale = scale;

   SetWindowPos( g_winGlobals.hWndMain, 
                 NULL, // No change in Z-order
                 0, 0, // No change in position
                 (int)( SCREEN_WIDTH * g_winGlobals.graphicsScale ) + g_winGlobals.clientPaddingRight, 
                 (int)( SCREEN_HEIGHT * g_winGlobals.graphicsScale ) + g_winGlobals.clientPaddingTop,
                 SWP_NOMOVE | SWP_NOZORDER | SWP_ASYNCWINDOWPOS); 
}

internal void LoadSavedPassword()
{
   FILE* file;

   g_winGlobals.savedPassword[0] = '\0';
   file = fopen( WIN_SAVE_FILE_NAME, "r" );

   if ( file )
   {
      if ( fgets( g_winGlobals.savedPassword, PASSWORD_LENGTH + 1, file ) )
      {
         g_winGlobals.savedPassword[PASSWORD_LENGTH] = '\0';

         if ( !Game_PasswordIsValid( g_winGlobals.savedPassword ) )
         {
            g_winGlobals.savedPassword[0] = '\0';
         }
      }
      else
      {
         g_winGlobals.savedPassword[0] = '\0';
      }

      fclose( file );
   }
}
