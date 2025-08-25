#include "player.h"
#include "screen.h"
#include "tile_map.h"
#include "math.h"
#include "tables.h"

void Player_Init( Player_t* player, TileMap_t* tileMap )
{
   player->tileMap = tileMap;
   player->maxVelocity = TILE_WALKSPEED_NORMAL;
   player->sprite.hitBoxSize.x = TILE_SIZE - 4;
   player->sprite.hitBoxSize.y = TILE_SIZE - 4;
   player->sprite.offset.x = -2;
   player->sprite.offset.y = -4;
   Sprite_LoadActive( &( player->sprite ), ACTIVE_SPRITE_PLAYER_ID );
}

uint8_t Player_GetLevelFromExperience( uint16_t experience )
{
   uint8_t i;

   for ( i = 0; i < STAT_TABLE_SIZE - 1; i++ )
   {
      if ( experience < g_experienceTable[i + 1] )
      {
         return i;
      }
   }

   return i;
}

uint8_t Player_GetStrengthFromLevel( Player_t* player, uint8_t level )
{
   // TODO
   UNUSED_PARAM( player );

   return g_strengthTable[level];
}

uint8_t Player_GetAgilityFromLevel( Player_t* player, uint8_t level )
{
   // TODO
   UNUSED_PARAM( player );

   return g_agilityTable[level];
}

uint8_t Player_GetMaxHitPointsFromLevel( Player_t* player, uint8_t level )
{
   // TODO
   UNUSED_PARAM( player );

   return g_hitPointsTable[level];
}

uint8_t Player_GetMaxMagicPointsFromLevel( Player_t* player, uint8_t level )
{
   // TODO
   UNUSED_PARAM( player );

   return g_magicPointsTable[level];
}

uint16_t Player_GetExperienceRemaining( Player_t* player )
{
   return ( player->level == ( STAT_TABLE_SIZE - 1 ) ) ? 0 : ( g_experienceTable[player->level + 1] - player->experience );
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

void Player_LoadWeapon( Player_t* player, uint32_t weaponId )
{
   player->weapon.id = weaponId;
   player->weapon.effect = g_weaponEffectTable[weaponId];

   switch ( weaponId )
   {
      case WEAPON_NONE_ID: strcpy( player->weapon.name1, STRING_ACCESSORY_NONE ); player->weapon.name2[0] = 0; break;
      case WEAPON_BAMBOOPOLE_ID: strcpy( player->weapon.name1, STRING_WEAPON_BAMBOOPOLE1 ); strcpy( player->weapon.name2, STRING_WEAPON_BAMBOOPOLE2 ); break;
      case WEAPON_CLUB_ID: strcpy( player->weapon.name1, STRING_WEAPON_CLUB ); player->weapon.name2[0] = 0; break;
      case WEAPON_COPPERSWORD_ID: strcpy( player->weapon.name1, STRING_WEAPON_COPPERSWORD1 ); strcpy( player->weapon.name2, STRING_WEAPON_COPPERSWORD2 ); break;
      case WEAPON_HANDAXE_ID: strcpy( player->weapon.name1, STRING_WEAPON_HANDAXE ); player->weapon.name2[0] = 0; break;
      case WEAPON_BROADSWORD_ID: strcpy( player->weapon.name1, STRING_WEAPON_BROADSWORD1 ); strcpy( player->weapon.name2, STRING_WEAPON_BROADSWORD2 ); break;
      case WEAPON_FLAMESWORD_ID: strcpy( player->weapon.name1, STRING_WEAPON_FLAMESWORD1 ); strcpy( player->weapon.name2, STRING_WEAPON_FLAMESWORD2 ); break;
      case WEAPON_ERDRICKSSWORD_ID: strcpy( player->weapon.name1, STRING_WEAPON_ERDRICKSSWORD1 ); strcpy( player->weapon.name2, STRING_WEAPON_ERDRICKSSWORD2 ); break;
   }
}

void Player_LoadArmor( Player_t* player, uint32_t armorId )
{
   player->armor.id = armorId;
   player->armor.effect = g_armorEffectTable[armorId];

   switch ( armorId )
   {
      case ARMOR_NONE_ID: strcpy( player->armor.name1, STRING_ACCESSORY_NONE ); player->armor.name2[0] = 0; break;
      case ARMOR_CLOTHES_ID: strcpy( player->armor.name1, STRING_ARMOR_CLOTHES ); player->armor.name2[0] = 0; break;
      case ARMOR_LEATHERARMOR_ID: strcpy( player->armor.name1, STRING_ARMOR_LEATHERARMOR1 ); strcpy( player->armor.name2, STRING_ARMOR_LEATHERARMOR2 ); break;
      case ARMOR_CHAINMAIL_ID: strcpy( player->armor.name1, STRING_ARMOR_CHAINMAIL1 ); strcpy( player->armor.name2, STRING_ARMOR_CHAINMAIL2 ); break;
      case ARMOR_HALFPLATE_ID: strcpy( player->armor.name1, STRING_ARMOR_HALFPLATE1 ); strcpy( player->armor.name2, STRING_ARMOR_HALFPLATE2 ); break;
      case ARMOR_FULLPLATE_ID: strcpy( player->armor.name1, STRING_ARMOR_FULLPLATE1 ); strcpy( player->armor.name2, STRING_ARMOR_FULLPLATE2 ); break;
      case ARMOR_MAGICARMOR_ID: strcpy( player->armor.name1, STRING_ARMOR_MAGICARMOR1 ); strcpy( player->armor.name2, STRING_ARMOR_MAGICARMOR2 ); break;
      case ARMOR_ERDRICKSARMOR_ID: strcpy( player->armor.name1, STRING_ARMOR_ERDRICKSARMOR1 ); strcpy( player->armor.name2, STRING_ARMOR_ERDRICKSARMOR2 ); break;
   }
}

void Player_LoadShield( Player_t* player, uint32_t shieldId )
{
   player->shield.id = shieldId;
   player->shield.effect = g_shieldEffectTable[shieldId];

   switch ( shieldId )
   {
      case SHIELD_NONE_ID: strcpy( player->shield.name1, STRING_ACCESSORY_NONE ); player->shield.name2[0] = 0; break;
      case SHIELD_SMALLSHIELD_ID: strcpy( player->shield.name1, STRING_SHIELD_SMALL1 ); strcpy( player->shield.name2, STRING_SHIELD_SMALL2 ); break;
      case SHIELD_LARGESHIELD_ID: strcpy( player->shield.name1, STRING_SHIELD_LARGE1 ); strcpy( player->shield.name2, STRING_SHIELD_LARGE2 ); break;
      case SHIELD_SILVERSHIELD_ID: strcpy( player->shield.name1, STRING_SHIELD_SILVER1 ); strcpy( player->shield.name2, STRING_SHIELD_SILVER2 ); break;
   }
}

void Player_SetCanonicalTileIndex( Player_t* player )
{
   player->canonicalTileIndex = player->tileIndex + ( ( player->tileIndex / player->tileMap->tilesX ) * ( TILE_COUNT_X - player->tileMap->tilesX ) );
}

void Player_CenterOnTile( Player_t* player )
{
   uint32_t tileX = ( player->tileIndex % player->tileMap->tilesX ) * TILE_SIZE;
   uint32_t tileY = ( player->tileIndex / player->tileMap->tilesX ) * TILE_SIZE;

   player->sprite.position.x = (float)( tileX + ( ( TILE_SIZE / 2 ) - ( player->sprite.hitBoxSize.x / 2 ) ) );
   player->sprite.position.y = (float)( tileY + ( ( TILE_SIZE / 2 ) - ( player->sprite.hitBoxSize.y / 2 ) ) );
}
