#include "game.h"
#include "tables.h"
#include "random.h"

internal void Password_InjectPlayerName( Player_t* player, uint32_t* encodedBits );
internal void Password_InjectChecksum( uint32_t* encodedBits );
internal void Password_ExtractPlayerName( Player_t* player, uint32_t* encodedBits );
internal char Password_GetCharFromBits( uint32_t bits );
internal uint32_t Password_GetBitsFromChar( char c );
internal Bool_t Password_ValidateChars( const char* password );
internal Bool_t Password_ValidateChecksum( uint32_t* encodedBits );

void Game_GetPassword( Game_t* game, char* password )
{
   Player_t* player = &( game->player );

   uint32_t encodedBits[6] = {
      ( game->gameFlags.doors << 16 ) |                           // doors not opened (16 bits)
      ( player->experience ),                                     // experience (16 bits)

      game->gameFlags.treasures,                                  // treasures remaining (32 bits)

      ( player->items << 7 ) |                                    // items (25 bits)
      ( (uint32_t)( player->townsVisited ) << 1 ) |               // towns visited (6 bits)
      ( player->isCursed ? 0x1 : 0x0 ),                           // is cursed (1 bit)

      ( (uint32_t)( player->gold ) << 16 ) |                      // gold (16 bits)
      ( ( player->weapon.id & 0x7 ) << 13 ) |                     // weapon (3 bits)
      ( ( player->armor.id & 0x7 ) << 10 ) |                      // armor (3 bits)
      ( ( player->shield.id & 0x3 ) << 8 ) |                      // shield (2 bits)
      ( game->gameFlags.specialEnemies & 0x7 << 5 ) |             // special enemies not defeated (3 bits)
      ( game->gameFlags.rescuedPrincess ? 0x1 << 4 : 0x0 ) |      // rescued princess (1 bit)
      ( game->gameFlags.foundHiddenStairs ? 0x1 << 3 : 0x0 ) |    // found hidden stairs (1 bit)
      ( game->gameFlags.usedRainbowDrop ? 0x1 << 2 : 0x0 ) |      // used rainbow drop (1 bit)
      ( game->gameFlags.gotStaffOfRain ? 0x1 << 1 : 0x0 ) |       // got the staff of rain (1 bit)
      ( game->gameFlags.gotRainbowDrop ? 0x1 : 0x0 )              // got the rainbow drop (1 bit)
   };

   // the player's encoded name goes on the end, followed by its length
   Password_InjectPlayerName( player, encodedBits );

   // now we inject the checksum before encoding to a string
   Password_InjectChecksum( encodedBits );

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

Bool_t Game_LoadFromPassword( Game_t* game, const char* password )
{
   uint32_t encodedBits[6];
   Player_t* player = &( game->player );

   if ( !Password_ValidateChars( password ) )
   {
      return False;
   }

   encodedBits[0] = ( Password_GetBitsFromChar( password[0] ) << 26 ) |
                    ( Password_GetBitsFromChar( password[1] ) << 20 ) |
                    ( Password_GetBitsFromChar( password[2] ) << 14 ) |
                    ( Password_GetBitsFromChar( password[3] ) << 8 ) |
                    ( Password_GetBitsFromChar( password[4] ) << 2 ) |
                    ( Password_GetBitsFromChar( password[5] ) >> 4 );
   encodedBits[1] = ( Password_GetBitsFromChar( password[5] ) << 28 ) |
                    ( Password_GetBitsFromChar( password[6] ) << 22 ) |
                    ( Password_GetBitsFromChar( password[7] ) << 16 ) |
                    ( Password_GetBitsFromChar( password[8] ) << 10 ) |
                    ( Password_GetBitsFromChar( password[9] ) << 4 ) |
                    ( Password_GetBitsFromChar( password[10] ) >> 2 );
   encodedBits[2] = ( Password_GetBitsFromChar( password[10] ) << 30 ) |
                    ( Password_GetBitsFromChar( password[11] ) << 24 ) |
                    ( Password_GetBitsFromChar( password[12] ) << 18 ) |
                    ( Password_GetBitsFromChar( password[13] ) << 12 ) |
                    ( Password_GetBitsFromChar( password[14] ) << 6 ) |
                    ( Password_GetBitsFromChar( password[15] ) );
   encodedBits[3] = ( Password_GetBitsFromChar( password[16] ) << 26 ) |
                    ( Password_GetBitsFromChar( password[17] ) << 20 ) |
                    ( Password_GetBitsFromChar( password[18] ) << 14 ) |
                    ( Password_GetBitsFromChar( password[19] ) << 8 ) |
                    ( Password_GetBitsFromChar( password[20] ) << 2 ) |
                    ( Password_GetBitsFromChar( password[21] ) >> 4 );
   encodedBits[4] = ( Password_GetBitsFromChar( password[21] ) << 28 ) |
                    ( Password_GetBitsFromChar( password[22] ) << 22 ) |
                    ( Password_GetBitsFromChar( password[23] ) << 16 ) |
                    ( Password_GetBitsFromChar( password[24] ) << 10 ) |
                    ( Password_GetBitsFromChar( password[25] ) << 4 ) |
                    ( Password_GetBitsFromChar( password[26] ) >> 2 );
   encodedBits[5] = ( Password_GetBitsFromChar( password[26] ) << 30 ) |
                    ( Password_GetBitsFromChar( password[27] ) << 24 ) |
                    ( Password_GetBitsFromChar( password[28] ) << 18 ) |
                    ( Password_GetBitsFromChar( password[29] ) << 12 );

   if ( !Password_ValidateChecksum( encodedBits ) )
   {
      return False;
   }

   Password_ExtractPlayerName( player, encodedBits );

   game->gameFlags.doors = 0xFFFF | ( encodedBits[0] >> 16 );
   game->gameFlags.treasures = encodedBits[1];
   game->gameFlags.specialEnemies = (uint8_t)( ( encodedBits[3] >> 5 ) & 0x7 );
   game->gameFlags.rescuedPrincess = (Bool_t)( ( encodedBits[3] >> 4 ) & 0x1 );
   game->gameFlags.foundHiddenStairs = (Bool_t)( ( encodedBits[3] >> 3 ) & 0x1 );
   game->gameFlags.usedRainbowDrop = (Bool_t)( ( encodedBits[3] >> 2 ) & 0x1 );
   game->gameFlags.gotStaffOfRain = (Bool_t)( ( encodedBits[3] >> 1 ) & 0x1 );
   game->gameFlags.gotRainbowDrop = (Bool_t)( ( encodedBits[3] ) & 0x1 );
   game->gameFlags.joinedDragonlord = False;

   player->experience = (uint16_t)( encodedBits[0] & 0xFFFF );
   player->gold = (uint16_t)( encodedBits[3] >> 16 );
   player->items = ( encodedBits[2] >> 7 ) & 0x1FFFFFF;
   player->townsVisited = (uint8_t)( ( encodedBits[2] >> 1 ) & 0x3F );
   player->isCursed = (Bool_t)( encodedBits[2] & 0x1 );

   player->level = Player_GetLevelFromExperience( player );
   Player_UpdateSpellsToLevel( player, player->level );

   player->stats.strength = g_strengthTable[player->level];
   player->stats.agility = g_agilityTable[player->level];
   player->stats.hitPoints = g_hitPointsTable[player->level];
   player->stats.maxHitPoints = g_hitPointsTable[player->level];
   player->stats.magicPoints = g_magicPointsTable[player->level];
   player->stats.maxMagicPoints = g_magicPointsTable[player->level];

   Player_LoadWeapon( player, ( encodedBits[3] >> 13 ) & 0x7 );
   Player_LoadArmor( player, ( encodedBits[3] >> 10 ) & 0x7 );
   Player_LoadShield( player, ( encodedBits[3] >> 8 ) & 0x3 );

   return True;
}

internal void Password_InjectPlayerName( Player_t* player, uint32_t* encodedBits )
{
   uint32_t i, encodedChars[8];
   size_t length = strlen( player->name );

   if ( length <= 0 )
   {
      length = 1;
      player->name[0] = ' ';
   }

   for ( i = 0; i < 8; i++ )
   {
      // pad unused chars with spaces
      encodedChars[i] = ( i < length ) ? Password_GetBitsFromChar( player->name[i] ) : 0x3F;
   }

   // start in slot 4, write out all 8 characters in 6-bit increments, followed by the length
   encodedBits[4] = ( encodedChars[0] << 26 ) |
                    ( encodedChars[1] << 20 ) |
                    ( encodedChars[2] << 14 ) |
                    ( encodedChars[3] << 8 ) |
                    ( encodedChars[4] << 2 ) |
                    ( encodedChars[5] >> 4 );
   encodedBits[5] = ( ( encodedChars[5] & 0xF ) << 28 ) |
                    ( encodedChars[6] << 22 ) |
                    ( encodedChars[7] << 16 ) |
                    ( ( ( length - 1 ) & 0x7 ) << 13 );
}

internal void Password_InjectChecksum( uint32_t* encodedBits )
{
   uint32_t tableIndex = Random_u32( 0, 31 );
   uint32_t tableValue = g_passwordChecksumTable[tableIndex];

   // the highest 7 bits of the permanent door flags
   encodedBits[0] &= ~( 0x7F << 25 );
   encodedBits[0] |= tableIndex << 27;
   encodedBits[0] |= ( tableValue >> 3 ) << 25;

   // the highest two bits of the treasure flags
   encodedBits[1] &= ~( 0x3 << 30 );
   encodedBits[1] |= ( ( tableValue >> 1 ) & 0x3 ) << 30;

   // the last bit of the password
   encodedBits[5] &= ~( 0x1 << 12 );
   encodedBits[5] |= ( tableValue & 0x1 ) << 12;
}

internal void Password_ExtractPlayerName( Player_t* player, uint32_t* encodedBits )
{
   uint32_t length, i;
   char* name = player->name;

   name[0] = 0;
   length = ( ( encodedBits[5] >> 13 ) & 0x7 ) + 1;

   if ( length > 0 )
   {
      name[0] = Password_GetCharFromBits( ( encodedBits[4] >> 26 ) & 0x3F );
      name[1] = 0;
   }
   if ( length > 1 )
   {
      name[1] = Password_GetCharFromBits( ( encodedBits[4] >> 20 ) & 0x3F );
      name[2] = 0;
   }
   if ( length > 2 )
   {
      name[2] = Password_GetCharFromBits( ( encodedBits[4] >> 14 ) & 0x3F );
      name[3] = 0;
   }
   if ( length > 3 )
   {
      name[3] = Password_GetCharFromBits( ( encodedBits[4] >> 8 ) & 0x3F );
      name[4] = 0;
   }
   if ( length > 4 )
   {
      name[4] = Password_GetCharFromBits( ( encodedBits[4] >> 2 ) & 0x3F );
      name[5] = 0;
   }
   if ( length > 5 )
   {
      name[5] = Password_GetCharFromBits( ( ( encodedBits[4] & 0x3 ) << 4 ) | ( ( encodedBits[5] >> 28 ) & 0xF ) );
      name[6] = 0;
   }
   if ( length > 6 )
   {
      name[6] = Password_GetCharFromBits( ( encodedBits[5] >> 22 ) & 0x3F );
      name[7] = 0;
   }
   if ( length > 7 )
   {
      name[7] = Password_GetCharFromBits( ( encodedBits[5] >> 16 ) & 0x3F );
      name[8] = 0;
   }

   // spaces are stored as dots in the encoded password, translate them back here
   for ( i = 0; i < length; i++ )
   {
      if ( name[i] == '.' )
      {
         name[i] = ' ';
      }
   }
}

internal char Password_GetCharFromBits( uint32_t bits )
{
   return ( bits < 26 ) ? (char)( bits + 65 ) :    // A-Z
      ( bits < 52 ) ? (char)( bits - 26 + 97 ) :   // a-z
      ( bits < 62 ) ? (char)( bits - 52 + 48 ) :   // 0-9
      ( bits == 62 ) ? '-' :                       // dash
      '.';                                         // dot
}

internal uint32_t Password_GetBitsFromChar( char c )
{
   return ( c >= 65 && c <= 90 ) ? (uint32_t)( c - 65 ) :        // A-Z
          ( c >= 97 && c <= 122 ) ? (uint32_t)( c - 97 + 26 ) :  // a-z
          ( c >= 48 && c <= 57 ) ? (uint32_t)( c - 48 + 52 ) :   // 0-9
          ( c == '-' ) ? 62 :                                    // dash
          63;                                                    // dot
}

internal Bool_t Password_ValidateChars( const char* password )
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

internal Bool_t Password_ValidateChecksum( uint32_t* encodedBits )
{
   uint32_t tableIndex = encodedBits[0] >> 27;
   uint32_t tableValue = ( ( encodedBits[5] >> 12 ) & 0x1 ) |
                         ( ( encodedBits[1] >> 29 ) & 0x6 ) |
                         ( ( encodedBits[0] >> 22 ) & 0x18 );

   return ( tableValue == g_passwordChecksumTable[tableIndex] ) ? True : False;
}
