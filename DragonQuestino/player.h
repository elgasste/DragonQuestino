#if !defined( ITEM_H )
#define ITEM_H

#include "common.h"
#include "sprite.h"
#include "battle_stats.h"

typedef struct TileMap_t TileMap_t;

#define PLAYER_LOWHEALTH_PERCENTAGE             0.26f

#define SPELL_HAS_HEAL( x )                     ( ( x ) & 0x1 )
#define SPELL_HAS_SIZZ( x )                     ( ( x ) & 0x2 )
#define SPELL_HAS_SLEEP( x )                    ( ( x ) & 0x4 )
#define SPELL_HAS_GLOW( x )                     ( ( x ) & 0x8 )
#define SPELL_HAS_FIZZLE( x )                   ( ( x ) & 0x10 )
#define SPELL_HAS_EVAC( x )                     ( ( x ) & 0x20 )
#define SPELL_HAS_ZOOM( x )                     ( ( x ) & 0x40 )
#define SPELL_HAS_REPEL( x )                    ( ( x ) & 0x80 )
#define SPELL_HAS_MIDHEAL( x )                  ( ( x ) & 0x100 )
#define SPELL_HAS_SIZZLE( x )                   ( ( x ) & 0x200 )

#define SPELL_SET_HASHEAL( x )                  ( x ) |= 0x1
#define SPELL_SET_HASSIZZ( x )                  ( x ) |= 0x2
#define SPELL_SET_HASSLEEP( x )                 ( x ) |= 0x4
#define SPELL_SET_HASGLOW( x )                  ( x ) |= 0x8
#define SPELL_SET_HASFIZZLE( x )                ( x ) |= 0x10
#define SPELL_SET_HASEVAC( x )                  ( x ) |= 0x20
#define SPELL_SET_HASZOOM( x )                  ( x ) |= 0x40
#define SPELL_SET_HASREPEL( x )                 ( x ) |= 0x80
#define SPELL_SET_HASMIDHEAL( x )               ( x ) |= 0x100
#define SPELL_SET_HASSIZZLE( x )                ( x ) |= 0x200

#define SPELL_HEAL_MP                           4
#define SPELL_SIZZ_MP                           2
#define SPELL_SLEEP_MP                          2
#define SPELL_GLOW_MP                           3
#define SPELL_FIZZLE_MP                         2
#define SPELL_EVAC_MP                           6
#define SPELL_ZOOM_MP                           8
#define SPELL_REPEL_MP                          2
#define SPELL_MIDHEAL_MP                        10
#define SPELL_SIZZLE_MP                         5

#define SPELL_HEAL_MINEFFECT                    10
#define SPELL_HEAL_MAXEFFECT                    17
#define SPELL_MIDHEAL_MINEFFECT                 85
#define SPELL_MIDHEAL_MAXEFFECT                 100
#define SPELL_SIZZ_MINEFFECT                    3
#define SPELL_SIZZ_MAXEFFECT                    16
#define SPELL_SIZZLE_MINEFFECT                  30
#define SPELL_SIZZLE_MAXEFFECT                  45

#define SPELL_GET_MAPUSEABLECOUNT( x, d1, d2 )  ( 0 + \
                                                ( SPELL_HAS_HEAL( x ) ? 1 : 0 ) + \
                                                ( ( ( d2 ) && SPELL_HAS_GLOW( x ) ) ? 1 : 0 ) + \
                                                ( ( ( d1 ) && SPELL_HAS_EVAC( x ) ) ? 1 : 0 ) + \
                                                ( ( ( !( d1 ) ) && SPELL_HAS_ZOOM( x ) ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_REPEL( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_MIDHEAL( x ) ? 1 : 0 ) )

#define SPELL_GET_BATTLEUSEABLECOUNT( x )       ( 0 + \
                                                ( SPELL_HAS_HEAL( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_SIZZ( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_SLEEP( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_FIZZLE( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_MIDHEAL( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_SIZZLE( x ) ? 1 : 0 ) )

#define ITEM_HERB_MINEFFECT                     23
#define ITEM_HERB_MAXEFFECT                     30

#define ITEM_MAXKEYS                            7
#define ITEM_MAXHERBS                           7
#define ITEM_MAXWINGS                           5
#define ITEM_MAXFAIRYWATERS                     5
#define ITEM_MAXTORCHES                         7

#define ITEM_GET_KEYCOUNT( x )                  ( ( x ) & 0x7 )
#define ITEM_GET_HERBCOUNT( x )                 ( ( ( x ) >> 3 ) & 0x7 )
#define ITEM_GET_WINGCOUNT( x )                 ( ( ( x ) >> 6 ) & 0x7 )
#define ITEM_GET_FAIRYWATERCOUNT( x )           ( ( ( x ) >> 9 ) & 0x7 )
#define ITEM_GET_TORCHCOUNT( x )                ( ( ( x ) >> 12 ) & 0x7 )
#define ITEM_HAS_FAIRYFLUTE( x )                ( ( ( x ) >> 15 ) & 0x1 )
#define ITEM_HAS_SILVERHARP( x )                ( ( ( x ) >> 16 ) & 0x1 )
#define ITEM_HAS_GWAELYNSLOVE( x )              ( ( ( x ) >> 17 ) & 0x1 )
#define ITEM_HAS_STONEOFSUNLIGHT( x )           ( ( ( x ) >> 18 ) & 0x1 )
#define ITEM_HAS_STAFFOFRAIN( x )               ( ( ( x ) >> 19 ) & 0x1 )
#define ITEM_HAS_TOKEN( x )                     ( ( ( x ) >> 20 ) & 0x1 )
#define ITEM_HAS_RAINBOWDROP( x )               ( ( ( x ) >> 21 ) & 0x1 )
#define ITEM_HAS_SPHEREOFLIGHT( x )             ( ( ( x ) >> 22 ) & 0x1 )
#define ITEM_HAS_DRAGONSCALE( x )               ( ( ( x ) >> 23 ) & 0x1 )
#define ITEM_HAS_CURSEDBELT( x )                ( ( ( x ) >> 24 ) & 0x1 )

#define ITEM_GET_MAPUSEABLECOUNT( x )           ( 0 + \
                                                ( ITEM_GET_HERBCOUNT( x ) ? 1 : 0 ) + \
                                                ( ITEM_GET_WINGCOUNT( x ) ? 1 : 0 ) + \
                                                ( ITEM_GET_FAIRYWATERCOUNT( x ) ? 1 : 0 ) + \
                                                ( ITEM_GET_TORCHCOUNT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_FAIRYFLUTE( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_SILVERHARP( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_GWAELYNSLOVE( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_RAINBOWDROP( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_CURSEDBELT( x ) ? 1 : 0 ) )

#define ITEM_GET_SELLABLECOUNT( x )           ( 0 + \
                                              ( ITEM_GET_KEYCOUNT( x ) ? 1 : 0 ) + \
                                              ( ITEM_GET_HERBCOUNT( x ) ? 1 : 0 ) + \
                                              ( ITEM_GET_WINGCOUNT( x ) ? 1 : 0 ) + \
                                              ( ITEM_GET_FAIRYWATERCOUNT( x ) ? 1 : 0 ) + \
                                              ( ITEM_GET_TORCHCOUNT( x ) ? 1 : 0 ) + \
                                              ( ITEM_HAS_DRAGONSCALE( x ) ? 1 : 0 ) )

#define ITEM_GET_BATTLEUSEABLECOUNT( x )        ( 0 + \
                                                ( ITEM_GET_HERBCOUNT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_FAIRYFLUTE( x ) ? 1 : 0 ) )

#define ITEM_GET_MAPNONUSEABLECOUNT( x )        ( 0 + \
                                                ( ITEM_GET_KEYCOUNT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_STONEOFSUNLIGHT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_STAFFOFRAIN( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_TOKEN( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_SPHEREOFLIGHT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_DRAGONSCALE( x ) ? 1 : 0 ) )

#define ITEM_HAS_SELLABLE( x )                  ( ITEM_GET_KEYCOUNT( x ) || \
                                                  ITEM_GET_HERBCOUNT( x ) || \
                                                  ITEM_GET_WINGCOUNT( x ) || \
                                                  ITEM_GET_FAIRYWATERCOUNT( x ) || \
                                                  ITEM_GET_TORCHCOUNT( x ) || \
                                                  ITEM_HAS_DRAGONSCALE( x ) )

#define ITEM_SET_KEYCOUNT( x, c )               ( x ) = ( ( ( x ) & 0xFFFFFFF8 ) | ( ( c ) & 0x7 ) )
#define ITEM_SET_HERBCOUNT( x, c )              ( x ) = ( ( ( x ) & 0xFFFFFFC7 ) | ( (u32)( c ) & 0x7 ) << 3 )
#define ITEM_SET_WINGCOUNT( x, c )              ( x ) = ( ( ( x ) & 0xFFFFFE3F ) | ( (u32)( c ) & 0x7 ) << 6 )
#define ITEM_SET_FAIRYWATERCOUNT( x, c )        ( x ) = ( ( ( x ) & 0xFFFFF1FF ) | ( (u32)( c ) & 0x7 ) << 9 )
#define ITEM_SET_TORCHCOUNT( x, c )             ( x ) = ( ( ( x ) & 0xFFFF8FFF ) | ( (u32)( c ) & 0x7 ) << 12 )
#define ITEM_TOGGLE_HASFAIRYFLUTE( x )          ( x ) ^= ( 0x1 << 15 )
#define ITEM_TOGGLE_HASSILVERHARP( x )          ( x ) ^= ( 0x1 << 16 )
#define ITEM_TOGGLE_HASGWAELYNSLOVE( x )        ( x ) ^= ( 0x1 << 17 )
#define ITEM_TOGGLE_HASSTONEOFSUNLIGHT( x )     ( x ) ^= ( 0x1 << 18 )
#define ITEM_TOGGLE_HASSTAFFOFRAIN( x )         ( x ) ^= ( 0x1 << 19 )
#define ITEM_TOGGLE_HASTOKEN( x )               ( x ) ^= ( 0x1 << 20 )
#define ITEM_TOGGLE_HASRAINBOWDROP( x )         ( x ) ^= ( 0x1 << 21 )
#define ITEM_TOGGLE_HASSPHEREOFLIGHT( x )       ( x ) ^= ( 0x1 << 22 )
#define ITEM_TOGGLE_HASDRAGONSCALE( x )         ( x ) ^= ( 0x1 << 23 )
#define ITEM_TOGGLE_HASCURSEDBELT( x )          ( x ) ^= ( 0x1 << 24 )

#define HAS_VISITED_TANTEGEL( x )               ( x ) & 0x1
#define HAS_VISITED_BRECCONARY( x )             ( x ) & 0x2
#define HAS_VISITED_GARINHAM( x )               ( x ) & 0x4
#define HAS_VISITED_KOL( x )                    ( x ) & 0x8
#define HAS_VISITED_RIMULDAR( x )               ( x ) & 0x20
#define HAS_VISITED_CANTLIN( x )                ( x ) & 0x10

#define HAS_VISITED_COUNT( x )                  ( 0 + \
                                                ( HAS_VISITED_TANTEGEL( x ) ? 1 : 0 ) + \
                                                ( HAS_VISITED_BRECCONARY( x ) ? 1 : 0 ) + \
                                                ( HAS_VISITED_GARINHAM( x ) ? 1 : 0 ) + \
                                                ( HAS_VISITED_KOL( x ) ? 1 : 0 ) + \
                                                ( HAS_VISITED_RIMULDAR( x ) ? 1 : 0 ) + \
                                                ( HAS_VISITED_CANTLIN( x ) ? 1 : 0 ) )

#define SET_VISITED_TANTEGEL( x )               ( x ) |= 0x1
#define SET_VISITED_BRECCONARY( x )             ( x ) |= 0x2
#define SET_VISITED_GARINHAM( x )               ( x ) |= 0x4
#define SET_VISITED_KOL( x )                    ( x ) |= 0x8
#define SET_VISITED_RIMULDAR( x )               ( x ) |= 0x20
#define SET_VISITED_CANTLIN( x )                ( x ) |= 0x10

#define HOLY_PROTECTION_MAX_STEPS               127

#define WEAPON_NONE_ID                          0
#define WEAPON_BAMBOOPOLE_ID                    1
#define WEAPON_CLUB_ID                          2
#define WEAPON_COPPERSWORD_ID                   3
#define WEAPON_HANDAXE_ID                       4
#define WEAPON_BROADSWORD_ID                    5
#define WEAPON_FLAMESWORD_ID                    6
#define WEAPON_ERDRICKSSWORD_ID                 7

#define ARMOR_NONE_ID                           0
#define ARMOR_CLOTHES_ID                        1
#define ARMOR_LEATHERARMOR_ID                   2
#define ARMOR_CHAINMAIL_ID                      3
#define ARMOR_HALFPLATE_ID                      4
#define ARMOR_FULLPLATE_ID                      5
#define ARMOR_MAGICARMOR_ID                     6
#define ARMOR_ERDRICKSARMOR_ID                  7

#define SHIELD_NONE_ID                          0
#define SHIELD_SMALLSHIELD_ID                   1
#define SHIELD_LARGESHIELD_ID                   2
#define SHIELD_SILVERSHIELD_ID                  3

#define ITEM_KEY_ID                             0
#define ITEM_HERB_ID                            1
#define ITEM_WING_ID                            2
#define ITEM_FAIRYWATER_ID                      3
#define ITEM_TORCH_ID                           4
#define ITEM_DRAGONSCALE_ID                     5

typedef struct Accessory_t
{
   u32 id;
   char name1[12];
   char name2[12];
   u8 effect;
}
Accessory_t;

typedef struct Player_t
{
   TileMap_t* tileMap;

   ActiveSprite_t sprite;
   Vector2r32_t velocity;
   r32 maxVelocity;
   u32 tileIndex;
   u32 canonicalTileIndex;
   Bool_t isCursed;
   Bool_t hasHolyProtection;
   u32 holyProtectionSteps;
   char name[9];
   BattleStats_t stats;
   Accessory_t weapon;
   Accessory_t armor;
   Accessory_t shield;
   u8 level;
   u16 experience;
   u16 gold;

   // 0: short term strength and agility
   // 1: short term agility and MP
   // 2: short term strength and HP
   // 3: short term HP and MP
   u32 statGrowthType;

   // bits 0-2: keys
   // bits 3-5: herbs
   // bits 6-8: wings
   // bits 9-11: fairy waters
   // bits 12-14: torches
   // bit 15: fairy flute
   // bit 16: silver harp
   // bit 17: gwaelin's love
   // bit 18: stone of sunlight
   // bit 19: staff of rain
   // bit 20: erdrick's token
   // bit 21: rainbow drop
   // bit 22: sphere of light
   // bit 23: dragon scale
   // bit 24: cursed belt
   u32 items;

   // bit 0: heal
   // bit 1: sizz
   // bit 2: sleep
   // bit 3: glow
   // bit 4: fizzle
   // bit 5: evac
   // bit 6: zoom
   // bit 7: repel
   // bit 8: midheal
   // bit 9: sizzle
   u16 spells;

   // bit 0: tantegel
   // bit 1: brecconary
   // bit 2: garinham
   // bit 3: kol
   // bit 4: cantlin
   // bit 5: rimuldar
   u8 townsVisited;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_Init( Player_t* player, TileMap_t* tileMap );
void Player_SetName( Player_t* player, const char* name );
u8 Player_GetLevelFromExperience( u16 experience );
u8 Player_GetStrengthFromLevel( Player_t* player, u8 level );
u8 Player_GetAgilityFromLevel( Player_t* player, u8 level );
u8 Player_GetMaxHitPointsFromLevel( Player_t* player, u8 level );
u8 Player_GetMaxMagicPointsFromLevel( Player_t* player, u8 level );
u16 Player_GetExperienceRemaining( Player_t* player );
u8 Player_RestoreHitPoints( Player_t* player, u8 hitPoints );
Bool_t Player_CollectItem( Player_t* player, Item_t item );
void Player_SetCursed( Player_t* player, Bool_t cursed );
void Player_SetHolyProtection( Player_t* player, Bool_t hasHolyProtection );
void Player_UpdateSpellsToLevel( Player_t* player, u8 level );
void Player_LoadWeapon( Player_t* player, u32 weaponId );
void Player_LoadArmor( Player_t* player, u32 armorId );
void Player_LoadShield( Player_t* player, u32 shieldId );
void Player_SetCanonicalTileIndex( Player_t* player );
void Player_CenterOnTile( Player_t* player );
void Player_GetAccessoryName( Player_t* player, AccessoryType_t type, char* name );
u16 Player_GetAccessoryResellValue( Player_t* player, AccessoryType_t type );
void Player_GetItemResellName( u32 itemId, char* name );
u16 Player_GetItemResellValue( u32 itemId );
u8 Player_GetAttackPower( Player_t* player );
u8 Player_GetDefensePower( Player_t* player );

#if defined( __cplusplus )
}
#endif

#endif // ITEM_H
