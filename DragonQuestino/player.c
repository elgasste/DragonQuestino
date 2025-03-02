#include "player.h"
#include "tile_map.h"
#include "screen.h"
#include "math.h"
#include "tables.h"

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

   // uncomment for testing
   //player->spells = 0x3FF;
   //SPELL_SET_HASSIZZ( player->spells );
}

uint16_t Player_GetLevel( Player_t* player )
{
   uint16_t i;

   for ( i = 0; i < EXPERIENCE_TABLE_SIZE; i++ )
   {
      if ( player->experience < g_experienceTable[i] )
      {
         return i + 1;
      }
   }

   return EXPERIENCE_TABLE_SIZE + 1;
}

uint16_t Player_GetExperienceRemaining( Player_t* player )
{
   uint16_t level = Player_GetLevel( player );

   return ( level == ( EXPERIENCE_TABLE_SIZE + 1 ) ) ? 0 : ( g_experienceTable[level - 1] - player->experience );
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
