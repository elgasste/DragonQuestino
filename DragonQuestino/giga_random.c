#include "random.h"

void Random_Seed()
{
   srand( micros() & (uint16_t)0xFFFF );
}
