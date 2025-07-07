#include "game.h"

internal void Password_InjectPlayerName( Player_t* player, uint32_t* encodedBits );
internal char Password_GetCharFromBits( uint32_t bits );
internal Bool_t Password_Validate( const char* password );

void Game_GetPassword( Game_t* game, char* password )
{
   Player_t* player = &( game->player );

   uint32_t encodedBits[6] = {

      game->gameFlags.treasures,                                  // treasures remaining (32 bits)

      ( game->gameFlags.doors << 16 ) |                           // doors not opened (16 bits)
      ( player->experience ),                                     // experience (16 bits)

      ( player->items << 7 ) |                                    // items (25 bits)
      ( (uint32_t)( player->townsVisited ) << 1 ) |               // towns visited (6 bits)
      ( player->isCursed ? 0x1 : 0x0 ),                           // is cursed (1 bit)

      ( (uint32_t)( player->gold ) << 16 ) |                      // gold (16 bits)
      ( ( player->weapon.id & 0x7 ) << 13 ) |                     // weapon (3 bits)
      ( ( player->armor.id & 0x7 ) << 10 ) |                      // armor (3 bits)
      ( ( player->shield.id & 0x3 ) << 8 ) |                      // shield (2 bits)
      ( game->gameFlags.specialEnemies & 0x7 << 5 ) |             // special enemies defeated (3 bits)
      ( game->gameFlags.rescuedPrincess ? 0x1 << 4 : 0x0 ) |      // rescued princess (1 bit)
      ( game->gameFlags.foundHiddenStairs ? 0x1 << 3 : 0x0 ) |    // found hidden stairs (1 bit)
      ( game->gameFlags.usedRainbowDrop ? 0x1 << 2 : 0x0 )        // used rainbow drop (1 bit)
   };

   // the player's encoded name goes on the end, followed by its length, the last 15 bits are unused
   Password_InjectPlayerName( player, encodedBits );

   // since the last 15 bits are unused, we only need 30 characters, not 32
   password[0] = Password_GetCharFromBits( encodedBits[0] >> 26 );
   password[1] = Password_GetCharFromBits( ( encodedBits[0] >> 20 ) & 0x3F );
   password[2] = Password_GetCharFromBits( ( encodedBits[0] >> 14 ) & 0x3F );
   password[3] = Password_GetCharFromBits( ( encodedBits[0] >> 8 ) & 0x3F );
   password[4] = Password_GetCharFromBits( ( encodedBits[0] >> 2 ) & 0x3F );
   password[5] = Password_GetCharFromBits( ( ( encodedBits[0] & 0x3 ) << 4 ) | ( encodedBits[1] >> 28 ) );
   password[6] = Password_GetCharFromBits( ( encodedBits[1] >> 22 ) & 0x3F );
   password[7] = Password_GetCharFromBits( ( encodedBits[1] >> 16 ) & 0x3F );
   password[8] = Password_GetCharFromBits( ( encodedBits[1] >> 10 ) & 0x3F );
   password[9] = Password_GetCharFromBits( ( encodedBits[1] >> 4 ) & 0x3F );
   password[10] = Password_GetCharFromBits( ( ( encodedBits[1] & 0xF ) << 2 ) | ( encodedBits[2] >> 30 ) );
   password[11] = Password_GetCharFromBits( ( encodedBits[2] >> 24 ) & 0x3F );
   password[12] = Password_GetCharFromBits( ( encodedBits[2] >> 18 ) & 0x3F );
   password[13] = Password_GetCharFromBits( ( encodedBits[2] >> 12 ) & 0x3F );
   password[14] = Password_GetCharFromBits( ( encodedBits[2] >> 6 ) & 0x3F );
   password[15] = Password_GetCharFromBits( encodedBits[2] & 0x3F );
   password[16] = Password_GetCharFromBits( encodedBits[3] >> 26 );
   password[17] = Password_GetCharFromBits( ( encodedBits[3] >> 20 ) & 0x3F );
   password[18] = Password_GetCharFromBits( ( encodedBits[3] >> 14 ) & 0x3F );
   password[19] = Password_GetCharFromBits( ( encodedBits[3] >> 8 ) & 0x3F );
   password[20] = Password_GetCharFromBits( ( encodedBits[3] >> 2 ) & 0x3F );
   password[21] = Password_GetCharFromBits( ( ( encodedBits[3] & 0x3 ) << 4 ) | ( encodedBits[4] >> 28 ) );
   password[22] = Password_GetCharFromBits( ( encodedBits[4] >> 22 ) & 0x3F );
   password[23] = Password_GetCharFromBits( ( encodedBits[4] >> 16 ) & 0x3F );
   password[24] = Password_GetCharFromBits( ( encodedBits[4] >> 10 ) & 0x3F );
   password[25] = Password_GetCharFromBits( ( encodedBits[4] >> 4 ) & 0x3F );
   password[26] = Password_GetCharFromBits( ( ( encodedBits[4] & 0xF ) << 2 ) | ( encodedBits[5] >> 30 ) );
   password[27] = Password_GetCharFromBits( ( encodedBits[5] >> 24 ) & 0x3F );
   password[28] = Password_GetCharFromBits( ( encodedBits[5] >> 18 ) & 0x3F );
   password[29] = Password_GetCharFromBits( ( encodedBits[5] >> 12 ) & 0x3F );
   password[30] = 0;
}

void Game_LoadFromPassword( Game_t* game, const char* password )
{
   UNUSED_PARAM( game );

   if ( !Password_Validate( password ) )
   {
      return;
   }
}

internal void Password_InjectPlayerName( Player_t* player, uint32_t* encodedBits )
{
   uint32_t i, encodedChars[8];
   char c;
   size_t length = strlen( player->name );

   if ( length <= 0 )
   {
      length = 1;
      player->name[0] = ' ';
   }

   for ( i = 0; i < 8; i++ )
   {
      if ( i < length )
      {
         c = player->name[i];

         encodedChars[i] = ( c >= 65 && c <= 90 ) ? (uint32_t)( c - 65 ) :        // A-Z
                           ( c >= 97 && c <= 122 ) ? (uint32_t)( c - 97 + 26 ) :  // a-z
                           ( c >= 48 && c <= 57 ) ? (uint32_t)( c - 48 + 52 ) :   // 0-9
                           ( c == 46 ) ? 62 :                                     // dash
                           63;                                                    // space
      }
      else
      {
         // pad unused chars with spaces
         encodedChars[i] = 0x3F;
      }
   }

   // store the name starting with the last two bits of slot 3, and stop after the
   // first 14 bits of slot 5, followed by the length of the name
   encodedBits[3] |= ( encodedChars[0] >> 4 );
   encodedBits[4] = ( ( encodedChars[0] & 0xF ) << 28 ) |
                    ( encodedChars[1] << 22 ) |
                    ( encodedChars[2] << 16 ) |
                    ( encodedChars[3] << 10 ) |
                    ( encodedChars[4] << 4 ) |
                    ( encodedChars[5] >> 2 );
   encodedBits[5] = ( ( encodedChars[5] & 0x3 ) << 30 ) |
                    ( encodedChars[6] << 24 ) |
                    ( encodedChars[7] << 18 ) |
                    ( ( length & 0x7 ) << 15 );
}

internal char Password_GetCharFromBits( uint32_t bits )
{
   if ( bits < 26 )
   {
      return (char)( bits + 65 );    // A-Z
   }
   else if ( bits < 52 )
   {
      return (char)( bits - 26 + 97 );    // a-z
   }
   else if ( bits < 62 )
   {
      return (char)( bits - 52 + 48 );    // 0-9
   }
   else
   {
      return ( bits == 62 ) ? '-' : '.';
   }
}

internal Bool_t Password_Validate( const char* password )
{
   uint32_t i;
   char c;
   size_t length = strlen( password );

   if ( length != PASSWORD_LENGTH )
   {
      return False;
   }

   for ( i = 0; i < strlen( password ); i++ )
   {
      c = password[i];

      // A-Z, a-z, 0-9, dot, and dash are allowed
      if ( !( ( c >= 65 && c <= 90 ) || ( c >= 97 && c <= 122 ) || ( c >= 48 && c <= 57 ) || c == 45 || c == 46 ) )
      {
         return False;
      }
   }

   return True;
}
