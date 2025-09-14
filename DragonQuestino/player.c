#include "player.h"
#include "screen.h"
#include "tile_map.h"
#include "math.h"
#include "tables.h"

internal u32 Player_GetNameSum( Player_t* player );

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

void Player_SetName( Player_t* player, const char* name )
{
   strcpy( player->name, name );
   player->statGrowthType = Player_GetNameSum( player ) % 4;
}

u8 Player_GetLevelFromExperience( u16 experience )
{
   u8 i;

   for ( i = 0; i < STAT_TABLE_SIZE - 1; i++ )
   {
      if ( experience < g_experienceTable[i + 1] )
      {
         return i;
      }
   }

   return i;
}

internal u8 Player_GetStatFromLevel( Player_t* player, u8 level, u8* table, Bool_t shortTerm )
{
   u8 stat = table[level];

   if ( shortTerm )
   {
      stat = ( ( Player_GetNameSum( player ) / 4 ) % 4 ) + ( ( stat * 9 ) / 10 );
   }

   return stat;
}

u8 Player_GetStrengthFromLevel( Player_t* player, u8 level )
{
   return Player_GetStatFromLevel( player, level, g_strengthTable, ( player->statGrowthType == 0 || player->statGrowthType == 2 ) ? True : False );
}

u8 Player_GetAgilityFromLevel( Player_t* player, u8 level )
{
   return Player_GetStatFromLevel( player, level, g_agilityTable, ( player->statGrowthType == 0 || player->statGrowthType == 1 ) ? True : False );
}

u8 Player_GetMaxHitPointsFromLevel( Player_t* player, u8 level )
{
   return Player_GetStatFromLevel( player, level, g_hitPointsTable, ( player->statGrowthType == 2 || player->statGrowthType == 3 ) ? True : False );
}

u8 Player_GetMaxMagicPointsFromLevel( Player_t* player, u8 level )
{
   return ( level < 2 )
      ? 0
      : Player_GetStatFromLevel( player, level, g_magicPointsTable, ( player->statGrowthType == 1 || player->statGrowthType == 3 ) ? True : False );
}

u16 Player_GetExperienceRemaining( Player_t* player )
{
   return ( player->level == ( STAT_TABLE_SIZE - 1 ) ) ? 0 : ( g_experienceTable[player->level + 1] - player->experience );
}

u8 Player_RestoreHitPoints( Player_t* player, u8 hitPoints )
{
   u8 restoredHitPoints = hitPoints;

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
   i8 count;

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
            u32 shiftedFlag = ( 0x1 << ( ( u32 )( item ) + 10 ) );
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

void Player_UpdateSpellsToLevel( Player_t* player, u8 level )
{
   u32 i;

   for ( i = 0; i < SPELL_TABLE_SIZE; i++ )
   {
      if ( level >= g_spellsLevelTable[i] )
      {
         player->spells |= ( 0x1 << i );
      }
   }
}

void Player_LoadWeapon( Player_t* player, u32 weaponId )
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

void Player_LoadArmor( Player_t* player, u32 armorId )
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

void Player_LoadShield( Player_t* player, u32 shieldId )
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
   u32 tileX = ( player->tileIndex % player->tileMap->tilesX ) * TILE_SIZE;
   u32 tileY = ( player->tileIndex / player->tileMap->tilesX ) * TILE_SIZE;

   player->sprite.position.x = (float)( tileX + ( ( TILE_SIZE / 2 ) - ( player->sprite.hitBoxSize.x / 2 ) ) );
   player->sprite.position.y = (float)( tileY + ( ( TILE_SIZE / 2 ) - ( player->sprite.hitBoxSize.y / 2 ) ) );
}

internal u32 Player_GetNameSum( Player_t* player )
{
   u32 i, nameSum = 0;

   for ( i = 0; i < strlen( player->name ); i++ )
   {
      nameSum += (u32)( player->name[i] ) % 16;
   }

   return nameSum;
}
