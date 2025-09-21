#include "screen.h"

internal i8 Screen_GetCharIndexFromChar( const char c );
internal uint16_t Screen_BlendColor( u16 foreground, u16 background, u8 alpha );

void Screen_Init( Screen_t* screen, u16* buffer )
{
   screen->buffer = buffer;
   Screen_LoadPalette( screen );
   Screen_LoadTextBitFields( screen );
   screen->textColor = COLOR_WHITE;
   screen->wipeColor = COLOR_BLACK;
   screen->needsWipe = False;
}

void Screen_BackupPalette( Screen_t* screen )
{
   u32 i;

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      screen->backupPalette[i] = screen->palette[i];
   }
}

void Screen_RestorePalette( Screen_t* screen )
{
   u32 i;

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      screen->palette[i] = screen->backupPalette[i];
   }
}

void Screen_ClearPalette( Screen_t* screen, u16 color )
{
   u32 i;

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      screen->palette[i] = color;
   }
}

void Screen_BackupPaletteAndWipeColor( Screen_t* screen, u16 color )
{
   Screen_BackupPalette( screen );
   Screen_ClearPalette( screen, color );
}

Bool_t Screen_GetPaletteIndexForColor( Screen_t* screen, u16 color, u32* paletteIndex )
{
   u32 i;

   for ( i = 0; i < PALETTE_COLORS; i++ )
   {
      if ( screen->palette[i] == color )
      {
         *paletteIndex = i;
         return True;
      }
   }

   return False;
}

void Screen_WipeFromPalette( Screen_t* screen, u32 paletteIndex )
{
   u32 i;
   u16* bufferPos = screen->buffer;

   for ( i = 0; i < SCREEN_PIXELS; i++ )
   {
      *bufferPos = screen->palette[paletteIndex];
      bufferPos++;
   }
}

void Screen_WipeColor( Screen_t* screen, u16 color )
{
   u32 i;
   u16* bufferPos = screen->buffer;

   for ( i = 0; i < SCREEN_PIXELS; i++ )
   {
      *bufferPos = color;
      bufferPos++;
   }
}

void Screen_DrawRectFromPalette( Screen_t* screen, u32 x, u32 y, u32 w, u32 h, u32 paletteIndex )
{
   u32 i, j;
   u16* bufferPos = screen->buffer + ( y * SCREEN_WIDTH ) + x;

   for ( i = 0; i < h; i++ )
   {
      for ( j = 0; j < w; j++ )
      {
         *bufferPos = screen->palette[paletteIndex];
         bufferPos++;
      }

      bufferPos += SCREEN_WIDTH - w;
   }
}

void Screen_DrawRectColor( Screen_t* screen, u32 x, u32 y, u32 w, u32 h, u16 color )
{
   u32 i, j;
   u16* bufferPos = screen->buffer + ( y * SCREEN_WIDTH ) + x;

   for ( i = 0; i < h; i++ )
   {
      for ( j = 0; j < w; j++ )
      {
         *bufferPos = color;
         bufferPos++;
      }

      bufferPos += SCREEN_WIDTH - w;
   }
}

void Screen_DrawChar( Screen_t* screen, char c, u32 x, u32 y )
{
   i32 i;
   u32 j, row;
   u8* bitField;
   i8 charIndex = Screen_GetCharIndexFromChar( c );
   u16* bufferPos = screen->buffer + ( y * SCREEN_WIDTH ) + x;

   if ( charIndex < 0 )
   {
      for ( i = 0, j = 0; i < TEXT_TILE_SIZE * TEXT_TILE_SIZE; i++ )
      {
         *bufferPos = 0;
         bufferPos++;
         j++;

         if ( j == TEXT_TILE_SIZE )
         {
            bufferPos += ( SCREEN_WIDTH - TEXT_TILE_SIZE );
            j = 0;
         }
      }
   }
   else
   {
#pragma warning( disable: 4047 )
      bitField = &( screen->textBitFields[charIndex] );
#pragma warning( default: 4047 )

      for ( row = 0; row < TEXT_TILE_SIZE; row++ )
      {
         for ( i = ( TEXT_TILE_SIZE - 1 ); i >= 0; i-- )
         {
            *bufferPos = ( bitField[row] & ( 0x01 << i ) ) ? screen->textColor : 0;
            bufferPos++;
         }

         bufferPos += ( SCREEN_WIDTH - TEXT_TILE_SIZE );
      }
   }
}

void Screen_DrawText( Screen_t* screen, const char* text, u32 x, u32 y )
{
   u16 ch, j;
   i8 charIndex, i;
   u8 row;
   u8* bitField;
   u16* bufferPos;

   for ( ch = 0; ch < strlen( text ); ch++ )
   {
      bufferPos = screen->buffer + ( y * SCREEN_WIDTH ) + x;
      charIndex = Screen_GetCharIndexFromChar( text[ch] );

      if ( charIndex < 0 )
      {
         for ( i = 0, j = 0; i < TEXT_TILE_SIZE * TEXT_TILE_SIZE; i++ )
         {
            *bufferPos = 0;
            bufferPos++;
            j++;

            if ( j == TEXT_TILE_SIZE )
            {
               bufferPos += ( SCREEN_WIDTH - TEXT_TILE_SIZE );
               j = 0;
            }
         }
      }
      else
      {
#pragma warning( disable: 4047 )
         bitField = &( screen->textBitFields[charIndex] );
#pragma warning( default: 4047 )

         for ( row = 0; row < TEXT_TILE_SIZE; row++ )
         {
            for ( i = ( TEXT_TILE_SIZE - 1 ); i >= 0; i-- )
            {
               *bufferPos = ( bitField[row] & ( 0x01 << i ) ) ? screen->textColor : 0;
               bufferPos++;
            }

            bufferPos += ( SCREEN_WIDTH - TEXT_TILE_SIZE );
         }
      }

      x += 8;
   }
}

void Screen_DrawCenteredText( Screen_t* screen, const char* text, u32 y )
{
   size_t textLength = strlen( text );
   u32 textWidth = (u32)textLength * TEXT_TILE_SIZE;
   u32 x;

   if ( textWidth > SCREEN_WIDTH )
   {
      return;
   }

   x = ( SCREEN_WIDTH - textWidth ) / 2;
   Screen_DrawText( screen, text, x, y );
}

void Screen_DrawMemorySection( Screen_t* screen, u8* memory, u32 stride,
                               u32 tx, u32 ty, u32 tw, u32 th,
                               u32 sx, u32 sy, Bool_t transparency )
{
   u32 x, y;
   u8* textureBufferPos = memory + ( ty * stride ) + tx;
   u16* screenBufferPos = screen->buffer + ( sy * SCREEN_WIDTH ) + sx;

   if ( transparency )
   {
      for ( y = 0; y < th; y++ )
      {
         for ( x = 0; x < tw; x++ )
         {
            if ( *textureBufferPos != TRANSPARENT_COLOR_INDEX )
            {
               *screenBufferPos = screen->palette[*textureBufferPos];
            }

            textureBufferPos++;
            screenBufferPos++;
         }

         textureBufferPos += tx + ( stride - ( tx + tw ) );
         screenBufferPos += ( SCREEN_WIDTH - tw );
      }
   }
   else
   {
      for ( y = 0; y < th; y++ )
      {
         for ( x = 0; x < tw; x++ )
         {
            *screenBufferPos = screen->palette[*textureBufferPos];
            textureBufferPos++;
            screenBufferPos++;
         }

         textureBufferPos += tx + ( stride - ( tx + tw ) );
         screenBufferPos += ( SCREEN_WIDTH - tw );
      }
   }
}

void Screen_DrawMemorySectionBlended( Screen_t* screen, u8* memory, u32 stride,
                                      u32 tx, u32 ty,
                                      u32 tw, u32 th,
                                      u32 sx, u32 sy,
                                      u8 alpha )
{
   u32 x, y;
   u8* textureBufferPos = memory + ( ty * stride ) + tx;
   u16* screenBufferPos = screen->buffer + ( sy * SCREEN_WIDTH ) + sx;

   for ( y = 0; y < th; y++ )
   {
      for ( x = 0; x < tw; x++ )
      {
         if ( *textureBufferPos != TRANSPARENT_COLOR_INDEX )
         {
            *screenBufferPos = Screen_BlendColor( screen->palette[*textureBufferPos], *screenBufferPos, alpha );
         }

         textureBufferPos++;
         screenBufferPos++;
      }

      textureBufferPos += tx + ( stride - ( tx + tw ) );
      screenBufferPos += ( SCREEN_WIDTH - tw );
   }
}

void Screen_DrawTextWindow( Screen_t* screen, u32 x, u32 y, u32 w, u32 h )
{
   u16 i;
   char line[32];
   memset( line, 0, sizeof( char ) * 32 );

   // top border
   line[0] = MENU_BORDER_CHAR_TOPLEFT;
   for ( i = 1; i < w - 1; i++ ) line[i] = MENU_BORDER_CHAR_TOP;
   line[w - 1] = MENU_BORDER_CHAR_TOPRIGHT;
   Screen_DrawText( screen, line, x, y );

   // side borders
   for ( i = 1; i < h - 1; i++ )
   {
      Screen_DrawChar( screen, MENU_BORDER_CHAR_LEFT, x, y + ( i * TEXT_TILE_SIZE ) );
      Screen_DrawChar( screen, MENU_BORDER_CHAR_RIGHT, x + ( ( w - 1 ) * TEXT_TILE_SIZE ), y + ( i * TEXT_TILE_SIZE ) );
   }

   // bottom border
   line[0] = MENU_BORDER_CHAR_BOTTOMLEFT;
   for ( i = 1; i < w - 1; i++ ) line[i] = MENU_BORDER_CHAR_BOTTOM;
   line[w - 1] = MENU_BORDER_CHAR_BOTTOMRIGHT;
   Screen_DrawText( screen, line, x, y + ( ( h - 1 ) * TEXT_TILE_SIZE ) );

   // inner section
   Screen_DrawRectColor( screen, x + TEXT_TILE_SIZE, y + TEXT_TILE_SIZE, ( w - 2 ) * TEXT_TILE_SIZE, ( h - 2 ) * TEXT_TILE_SIZE, COLOR_BLACK );
}

void Screen_DrawTextWindowWithTitle( Screen_t* screen, u32 x, u32 y, u32 w, u32 h, const char* title )
{
   Screen_DrawTextWindow( screen, x, y, w, h );
   Screen_DrawText( screen, title, x + ( ( ( w - (u32)( strlen( title ) ) ) / 2 ) * TEXT_TILE_SIZE ), y );
}

internal i8 Screen_GetCharIndexFromChar( const char c )
{
   if ( c >= 97 && c <= 122 )
   {
      // a - z (lower case letters start at 0 in our table)
      return c - 97;
   }
   else if ( c >= 65 && c <= 90 )
   {
      // A - Z (upper case letters start at 26 in our table)
      return c - 39;
   }
   else if ( c >= 48 && c <= 57 )
   {
      // 0 - 9 (numbers start at 52 in our table)
      return c + 4;
   }
   else
   {
      // special characters start at 62 in our table
      switch ( c )
      {
         case 44: return 62;     // comma
         case 33: return 63;     // exclamation point
         case 39: return 64;     // single quote
         case 38: return 65;     // ampersand
         case 46: return 66;     // period
         case 34: return 67;     // double quotes
         case 63: return 68;     // question mark
         case 45: return 69;     // dash
         case 62: return 70;     // greater-than
         case 58: return 71;     // colon
         case 47: return 72;     // forward slash
         case 40: return 73;     // left parenthesis
         case 41: return 74;     // right parenthesis
         case 42: return 84;     // asterisk

         case MENU_BORDER_CHAR_TOPLEFT: return 75;
         case MENU_BORDER_CHAR_TOPRIGHT: return 76;
         case MENU_BORDER_CHAR_BOTTOMLEFT: return 77;
         case MENU_BORDER_CHAR_BOTTOMRIGHT: return 78;
         case MENU_BORDER_CHAR_LEFT: return 79;
         case MENU_BORDER_CHAR_TOP: return 80;
         case MENU_BORDER_CHAR_RIGHT: return 81;
         case MENU_BORDER_CHAR_BOTTOM: return 82;

         case DOWNWARD_CARAT: return 83;

         default: return -1;
      }
   }
}

internal uint16_t Screen_BlendColor( u16 foreground, u16 background, u8 alpha )
{
   u8 foregroundR = COLOR_GET_R5( foreground );
   u8 foregroundG = COLOR_GET_G6( foreground );
   u8 foregroundB = COLOR_GET_B5( foreground );

   u8 backgroundR = COLOR_GET_R5( background );
   u8 backgroundG = COLOR_GET_G6( background );
   u8 backgroundB = COLOR_GET_B5( background );

   u8 blendedR = ( ( foregroundR * alpha ) + ( backgroundR * ( 255 - alpha ) ) ) / 255;
   u8 blendedG = ( ( foregroundG * alpha ) + ( backgroundG * ( 255 - alpha ) ) ) / 255;
   u8 blendedB = ( ( foregroundB * alpha ) + ( backgroundB * ( 255 - alpha ) ) ) / 255;

   return COLOR_MAKE_RGB565( blendedR, blendedG, blendedB );
}
