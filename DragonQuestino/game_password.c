#include "game.h"

internal Bool_t Game_ValidatePassword( const char* password );

void Game_LoadFromPassword( Game_t* game, const char* password )
{
   UNUSED_PARAM( game );

   if ( !Game_ValidatePassword( password ) )
   {
      return;
   }
}

internal Bool_t Game_ValidatePassword( const char* password )
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
