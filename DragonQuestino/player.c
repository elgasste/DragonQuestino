#include "player.h"
#include "tile_map.h"
#include "screen.h"
#include "math.h"

void Player_Init( Player_t* player, Screen_t* screen )
{
   player->screen = screen;

   player->tileIndex = 148; // sort of in front of King Lorik
   player->sprite.position.x = (float)( TILE_SIZE * 8 );
   player->sprite.position.y = (float)( TILE_SIZE * 7 );
   player->velocity.x = 0.0f;
   player->velocity.y = 0.0f;
   player->maxVelocity = TILE_WALKSPEED_NORMAL;
   player->hitBoxSize.x = TILE_SIZE - 4;
   player->hitBoxSize.y = TILE_SIZE - 4;
   player->spriteOffset.x = -2;
   player->spriteOffset.y = -4;
   player->sprite.direction = Direction_Down;

   strcpy( player->name, "TestMan1" );
   player->isCursed = False;

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
   player->spells = 0;

   // for testing
   player->spells = 0x3FF;
   //SPELL_SET_HASSIZZ( player->spells );x
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

uint16_t Player_GetExperienceRemaining( Player_t* player )
{
   uint8_t level = Player_GetLevel( player );

   switch ( level )
   {
      case 1: return 7 - player->experience;
      case 2: return 23 - player->experience;
      case 3: return 47 - player->experience;
      case 4: return 110 - player->experience;
      case 5: return 220 - player->experience;
      case 6: return 450 - player->experience;
      case 7: return 800 - player->experience;
      case 8: return 1300 - player->experience;
      case 9: return 2000 - player->experience;
      case 10: return 2900 - player->experience;
      case 11: return 4000 - player->experience;
      case 12: return 5500 - player->experience;
      case 13: return 7500 - player->experience;
      case 14: return 10000 - player->experience;
      case 15: return 13000 - player->experience;
      case 16: return 16000 - player->experience;
      case 17: return 19000 - player->experience;
      case 18: return 22000 - player->experience;
      case 19: return 26000 - player->experience;
      case 20: return 30000 - player->experience;
      case 21: return 34000 - player->experience;
      case 22: return 38000 - player->experience;
      case 23: return 42000 - player->experience;
      case 24: return 46000 - player->experience;
      case 25: return 50000 - player->experience;
      case 26: return 54000 - player->experience;
      case 27: return 58000 - player->experience;
      case 28: return 62000 - player->experience;
      case 29: return UINT16_MAX - player->experience;
      default: return 0;
   }
}

uint16_t Player_CollectGold( Player_t* player, uint16_t gold )
{
   return Math_CollectAmount16u( &( player->gold ), gold );
}

uint16_t Player_CollectExperience( Player_t* player, uint16_t experience )
{
   return Math_CollectAmount16u( &( player->experience ), experience );
}

Bool_t Player_CollectItem( Player_t* player, Item_t item )
{
   Bool_t collected = False;
   int8_t count;

   switch ( item )
   {
      case Item_Key:
         count = ITEM_GET_KEYCOUNT( player->items );
         if ( count < ITEM_MAXKEYS )
         {
            count++;
            ITEM_SET_KEYCOUNT( player->items, count );
            collected = True;
         }
         break;
      case Item_Herb:
         count = ITEM_GET_HERBCOUNT( player->items );
         if ( count < ITEM_MAXHERBS )
         {
            count++;
            ITEM_SET_HERBCOUNT( player->items, count );
            collected = True;
         }
         break;
      case Item_Wing:
         count = ITEM_GET_WINGCOUNT( player->items );
         if ( count < ITEM_MAXWINGS )
         {
            count++;
            ITEM_SET_WINGCOUNT( player->items, count );
            collected = True;
         }
         break;
      case Item_FairyWater:
         count = ITEM_GET_FAIRYWATERCOUNT( player->items );
         if ( count < ITEM_MAXFAIRYWATERS )
         {
            count++;
            ITEM_SET_FAIRYWATERCOUNT( player->items, count );
            collected = True;
         }
         break;
      case Item_Torch:
         count = ITEM_GET_TORCHCOUNT( player->items );
         if ( count < ITEM_MAXTORCHES )
         {
            count++;
            ITEM_SET_TORCHCOUNT( player->items, count );
            collected = True;
         }
         break;
      default:
         if ( item < Item_Count )
         {
            // single items start at 5 (Item_FairyFlute), and shifting starts at 15, hence "item + 10"
            uint32_t shiftedFlag = ( 0x1 << ( ( uint32_t )( item ) + 10 ) );
            if ( !( player->items & shiftedFlag ) )
            {
               player->items |= shiftedFlag;
               collected = True;
            }
         }
         break;
   }

   return collected;
}

void Player_SetCursed( Player_t* player, Bool_t cursed )
{
   player->isCursed = cursed;
   player->screen->textColor = cursed ? COLOR_GROSSYELLOW : COLOR_WHITE;
}
