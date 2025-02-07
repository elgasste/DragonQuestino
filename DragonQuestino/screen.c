#include "screen.h"

internal int8_t Screen_GetCharIndexFromChar( const char c );

void Screen_Init( Screen_t* screen, uint16_t* buffer )
{
   screen->buffer = buffer;
   Screen_LoadPalette( screen );
   Screen_LoadTextBitFields( screen );
}

Bool_t Screen_GetPaletteIndexForColor( Screen_t* screen, uint16_t color, uint32_t* paletteIndex )
{
   uint32_t i;

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

void Screen_Wipe( Screen_t* screen, uint32_t paletteIndex )
{
   uint32_t i;
   uint16_t* bufferPos = screen->buffer;

   for ( i = 0; i < SCREEN_PIXELS; i++ )
   {
      *bufferPos = screen->palette[paletteIndex];
      bufferPos++;
   }
}

void Screen_DrawChar( Screen_t* screen, char c, uint32_t x, uint32_t y, uint16_t color )
{
   int32_t i;
   uint32_t j, row;
   uint8_t* bitField;
   int8_t charIndex = Screen_GetCharIndexFromChar( c );
   uint16_t* bufferPos = screen->buffer + ( y * SCREEN_WIDTH ) + x;

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
            *bufferPos = ( bitField[row] & ( 0x01 << i ) ) ? color : 0;
            bufferPos++;
         }

         bufferPos += ( SCREEN_WIDTH - TEXT_TILE_SIZE );
      }
   }
}

void Screen_DrawText( Screen_t* screen, const char* text, uint32_t x, uint32_t y, uint16_t color )
{
   uint16_t ch, j;
   int8_t charIndex, i;
   uint8_t row;
   uint8_t* bitField;
   uint16_t* bufferPos;
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
               *bufferPos = ( bitField[row] & ( 0x01 << i ) ) ? color : 0;
               bufferPos++;
            }
            bufferPos += ( SCREEN_WIDTH - TEXT_TILE_SIZE );
         }
      }
      x += 8;
   }
}

void Screen_DrawWrappedText( Screen_t* screen, const char* text, uint32_t x, uint32_t y, uint32_t lineChars, uint16_t color )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen = (uint16_t)strlen( text );
   Bool_t endOfLine, endOfText;
   char line[32];
   char curChar;

   for ( textIndex = 0, lineIndex = 0, lastSpaceIndex = 0, currentLine = 0; textIndex < strLen; textIndex++ )
   {
      curChar = text[textIndex];
      endOfLine = ( lineIndex == ( lineChars - 1 ) );
      endOfText = ( textIndex == ( strLen - 1 ) ) ? True : False;

      if ( endOfLine || endOfText )
      {
         if ( ( lastSpaceIndex > 0 ) && !endOfText )
         {
            if ( curChar == ' ' )
            {
               line[lineIndex] = '\0';
            }
            else if ( text[textIndex + 1] == ' ' )
            {
               line[lineIndex] = curChar;
               line[lineIndex + 1] = '\0';
            }
            else
            {
               line[lastSpaceIndex - 1] = '\0';
               textIndex -= ( ( lineIndex - lastSpaceIndex ) + 1 );
            }
         }
         else if ( curChar == ' ' )
         {
            line[lineIndex] = '\0';
         }
         else
         {
            line[lineIndex] = curChar;
            line[lineIndex + 1] = '\0';
         }

         Screen_DrawText( screen, line, x, y + ( currentLine * TEXT_TILE_SIZE ), color );
         lineIndex = 0;
         lastSpaceIndex = 0;
         currentLine++;
      }
      else if ( curChar != ' ' )
      {
         line[lineIndex++] = curChar;
      }
      else if ( lineIndex > 0 && lastSpaceIndex != lineIndex )
      {
         line[lineIndex++] = curChar;
         lastSpaceIndex = lineIndex;
      }
   }
}

internal int8_t Screen_GetCharIndexFromChar( const char c )
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

         // menu borders
         case MENU_BORDER_CHAR_TOPLEFT: return 75;
         case MENU_BORDER_CHAR_TOPRIGHT: return 76;
         case MENU_BORDER_CHAR_BOTTOMLEFT: return 77;
         case MENU_BORDER_CHAR_BOTTOMRIGHT: return 78;
         case MENU_BORDER_CHAR_LEFT: return 79;
         case MENU_BORDER_CHAR_TOP: return 80;
         case MENU_BORDER_CHAR_RIGHT: return 81;
         case MENU_BORDER_CHAR_BOTTOM: return 82;

         default: return -1;
      }
   }
}

void Screen_DrawMemorySection( Screen_t* screen, uint8_t* memory, uint32_t stride,
                               uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th,
                               uint32_t sx, uint32_t sy, Bool_t transparency )
{
   uint32_t x, y;
   uint8_t* textureBufferPos = memory + ( ty * stride ) + tx;
   uint16_t* screenBufferPos = screen->buffer + ( sy * SCREEN_WIDTH ) + sx;

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
