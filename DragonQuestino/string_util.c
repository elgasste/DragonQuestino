#include "string_util.h"

void StringUtil_ToUpper( char* str )
{
   size_t i;

   for ( i = 0; i < strlen( str ); i++ )
   {
      if ( ( str[i] >= 97 ) && ( str[i] <= 122 ) )
      {
         str[i] -= 32;
      }
   }
}
