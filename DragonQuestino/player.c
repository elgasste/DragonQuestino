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

   strcpy( player->name, "TestMan1" );

   player->stats.hitPoints = 12;
   player->stats.maxHitPoints = 12;
   player->stats.magicPoints = 0;
   player->stats.maxMagicPoints = 0;
   player->stats.attackPower = 2;
   player->stats.defensePower = 2;
   player->stats.strength = 2;
   player->stats.agility = 2;

   player->experience = 0;
   player->gold = 0;
   player->items = 0;
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
   else if ( player->experience < UINT16_MAX ) { return 29; }
   else { return 30; }
}

uint16_t Player_CollectGold( Player_t* player, uint16_t gold )
{
   if ( gold < ( UINT16_MAX - player->gold ) )
   {
      player->gold += gold;
      return gold;
   }
   else
   {
      player->gold = UINT16_MAX;
      return UINT16_MAX - player->gold;
   }
}

uint16_t Player_CollectExperience( Player_t* player, uint16_t experience )
{
   if ( experience < ( UINT16_MAX - player->experience ) )
   {
      player->experience += experience;
      return experience;
   }
   else
   {
      player->experience = UINT16_MAX;
      return UINT16_MAX - player->experience;
   }
}

Bool_t Player_CollectItem( Player_t* player, Item_t item )
{
   Bool_t collected = False;
   int8_t count;

   switch ( item )
   {
      case Item_Key:
         count = PLAYER_GET_KEYCOUNT( player->items );
         if ( count < PLAYER_MAXKEYS )
         {
            count++;
            PLAYER_SET_KEYCOUNT( player->items, count );
            collected = True;
         }
         break;
      case Item_Herb:
         count = PLAYER_GET_HERBCOUNT( player->items );
         if ( count < PLAYER_MAXHERBS )
         {
            count++;
            PLAYER_SET_HERBCOUNT( player->items, count );
            collected = True;
         }
         break;
      case Item_Wing:
         count = PLAYER_GET_WINGCOUNT( player->items );
         if ( count < PLAYER_MAXWINGS )
         {
            count++;
            PLAYER_SET_WINGCOUNT( player->items, count );
            collected = True;
         }
         break;
      case Item_FairyWater:
         count = PLAYER_GET_FAIRYWATERCOUNT( player->items );
         if ( count < PLAYER_MAXFAIRYWATERS )
         {
            count++;
            PLAYER_SET_FAIRYWATERCOUNT( player->items, count );
            collected = True;
         }
         break;
      default:
         if ( item < Item_Count )
         {
            // single items start at 4 (Item_Tablet), and shifting starts at 12, hence "item + 8"
            player->items |= ( ( uint32_t )( True ) << ( ( uint32_t )( item ) + 8 ) );
            collected = True;
         }
         break;
   }

   return collected;
}
