#include "player.h"
#include "screen.h"
#include "tile_map.h"
#include "math.h"
#include "tables.h"

void Player_Init( Player_t* player, TileMap_t* tileMap )
{
   player->tileMap = tileMap;

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
   strcpy( player->name, "Ed209" );
   player->isCursed = False;
   player->hasHolyProtection = False;
   player->holyProtectionSteps = 0;
   player->townsVisited = 0;

   player->experience = 0;
   player->gold = 0;
   player->items = 0;
   player->spells = 0;
   player->level = Player_GetLevelFromExperience( player );
   Player_UpdateSpellsToLevel( player, player->level );

   player->stats.strength = g_strengthTable[player->level];
   player->stats.agility = g_agilityTable[player->level];
   player->stats.hitPoints = g_hitPointsTable[player->level];
   player->stats.maxHitPoints = g_hitPointsTable[player->level];
   player->stats.magicPoints = g_magicPointsTable[player->level];
   player->stats.maxMagicPoints = g_magicPointsTable[player->level];
   player->stats.sleepResist = 0;
   player->stats.stopSpellResist = 0;
   player->stats.hurtResist = 0;
   player->stats.dodge = 1;

   // MUFFINS: for testing
   SPELL_SET_HASHEAL( player->spells );
   SPELL_SET_HASMIDHEAL( player->spells );
   SPELL_SET_HASZOOM( player->spells );
   player->stats.maxHitPoints = 100;
   player->stats.hitPoints = 2;
   player->stats.maxMagicPoints = 100;
   player->stats.magicPoints = 00;
}

uint8_t Player_GetLevelFromExperience( Player_t* player )
{
   uint8_t i;

   for ( i = 0; i < STAT_TABLE_SIZE - 1; i++ )
   {
      if ( player->experience < g_experienceTable[i + 1] )
      {
         return i;
      }
   }

   return i;
}

uint16_t Player_GetExperienceRemaining( Player_t* player )
{
   return ( player->level == ( STAT_TABLE_SIZE - 1 ) ) ? 0 : ( g_experienceTable[player->level + 1] - player->experience );
}

uint16_t Player_CollectGold( Player_t* player, uint16_t gold )
{
   return Math_CollectAmount16u( &( player->gold ), gold );
}

uint16_t Player_CollectExperience( Player_t* player, uint16_t experience )
{
   return Math_CollectAmount16u( &( player->experience ), experience );
}

uint8_t Player_RestoreHitPoints( Player_t* player, uint8_t hitPoints )
{
   uint8_t restoredHitPoints = hitPoints;

   if ( ( player->stats.maxHitPoints - player->stats.hitPoints ) < hitPoints )
   {
      restoredHitPoints = player->stats.maxHitPoints - player->stats.hitPoints;
   }

   player->stats.hitPoints += restoredHitPoints;
   return restoredHitPoints;
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

   if ( cursed )
   {
      TileMap_SetTargetGlowDiameter( player->tileMap, 1 );
   }
}

void Player_SetHolyProtection( Player_t* player, Bool_t hasHolyProtection )
{
   player->hasHolyProtection = hasHolyProtection;

   if ( hasHolyProtection )
   {
      player->holyProtectionSteps = 0;
   }
}

void Player_UpdateSpellsToLevel( Player_t* player, uint8_t level )
{
   uint32_t i;

   for ( i = 0; i < SPELL_TABLE_SIZE; i++ )
   {
      if ( level >= g_spellsLevelTable[i] )
      {
         player->spells |= ( 0x1 << i );
      }
   }
}
