#include "player.h"
#include "screen.h"
#include "tile_map.h"
#include "math.h"
#include "tables.h"

internal void Player_UpdateTextColor( Player_t* player, uint8_t previousHitPoints );

void Player_Init( Player_t* player, Screen_t* screen, TileMap_t* tileMap )
{
   player->screen = screen;
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

   Player_UpdateTextColor( player, UINT8_MAX );
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

void Player_RestoreHitPoints( Player_t* player, uint8_t hitPoints )
{
   uint8_t previousHitPoints = player->stats.hitPoints;

   Math_CollectAmount8u( &( player->stats.hitPoints ), hitPoints );
   Player_UpdateTextColor( player, previousHitPoints );
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
      player->screen->textColor = COLOR_GROSSYELLOW;
      TileMap_SetTargetGlowDiameter( player->tileMap, 1 );
   }
   else
   {
      player->screen->textColor = COLOR_WHITE;
      Player_UpdateTextColor( player, INT8_MAX );
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

internal void Player_UpdateTextColor( Player_t* player, uint8_t previousHitPoints )
{
   float percentage, previousPercentage;

   if ( player->isCursed )
   {
      return;
   }

   percentage = (float)( player->stats.hitPoints ) / player->stats.maxHitPoints;
   previousPercentage = (float)( previousHitPoints ) / player->stats.maxHitPoints;

   if ( percentage < PLAYER_LOWHEALTH_PERCENTAGE && previousPercentage >= PLAYER_LOWHEALTH_PERCENTAGE )
   {
      player->screen->textColor = COLOR_INJUREDRED;
      player->screen->needsRedraw = True;
   }
   else if ( percentage >= PLAYER_LOWHEALTH_PERCENTAGE && previousPercentage < PLAYER_LOWHEALTH_PERCENTAGE )
   {
      player->screen->textColor = COLOR_WHITE;
      player->screen->needsRedraw = True;
   }
}
