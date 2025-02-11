#include "player.h"
#include "tile_map.h"

void Player_Init( Player_t* player )
{
   player->tileIndex = TILE_COUNT_X * TILE_COUNT_Y; // off the map
   player->sprite.position.x = (float)( TILE_SIZE * 55 );
   player->sprite.position.y = (float)( TILE_SIZE * 49 );
   player->velocity.x = 0.0f;
   player->velocity.y = 0.0f;
   player->maxVelocity = TILE_WALKSPEED_NORMAL;
   player->hitBoxSize.x = TILE_SIZE - 4;
   player->hitBoxSize.y = TILE_SIZE - 4;
   player->spriteOffset.x = -2;
   player->spriteOffset.y = -4;
   player->sprite.direction = Direction_Down;

   player->stats.hitPoints = 12;
   player->stats.maxHitPoints = 12;
   player->stats.magicPoints = 0;
   player->stats.maxMagicPoints = 0;
   player->stats.attackPower = 2;
   player->stats.defensePower = 2;
   player->stats.agility = 2;
   player->stats.luck = 2;

   player->experience = 0;
   player->gold = 0;
}

uint8_t Player_GetLevel( Player_t* player )
{
   if ( player->experience < 7 ) { return 1; }
   else if ( player->experience < 23 ) { return 2; }
   else if ( player->experience < 47 ) { return 3; }
   else if ( player->experience < 110 ) { return 4; }
   else if ( player->experience < 220 ) { return 5; }
   else if ( player->experience < 450 ) { return 6; }
   else if ( player->experience < 800 ) { return 7; }
   else if ( player->experience < 1300 ) { return 8; }
   else if ( player->experience < 2000 ) { return 9; }
   else if ( player->experience < 2900 ) { return 10; }
   else if ( player->experience < 4000 ) { return 11; }
   else if ( player->experience < 5500 ) { return 12; }
   else if ( player->experience < 7500 ) { return 13; }
   else if ( player->experience < 10000 ) { return 14; }
   else if ( player->experience < 13000 ) { return 15; }
   else if ( player->experience < 16000 ) { return 16; }
   else if ( player->experience < 19000 ) { return 17; }
   else if ( player->experience < 22000 ) { return 18; }
   else if ( player->experience < 26000 ) { return 19; }
   else if ( player->experience < 30000 ) { return 20; }
   else if ( player->experience < 34000 ) { return 21; }
   else if ( player->experience < 38000 ) { return 22; }
   else if ( player->experience < 42000 ) { return 23; }
   else if ( player->experience < 46000 ) { return 24; }
   else if ( player->experience < 50000 ) { return 25; }
   else if ( player->experience < 54000 ) { return 26; }
   else if ( player->experience < 58000 ) { return 27; }
   else if ( player->experience < 62000 ) { return 28; }
   else if ( player->experience < 65535 ) { return 29; }
   else { return 30; }
}
