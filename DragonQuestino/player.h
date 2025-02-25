#if !defined( ITEM_H )
#define ITEM_H

#include "common.h"
#include "sprite.h"
#include "battle_stats.h"

typedef struct Screen_t Screen_t;

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

#define SPELL_GET_MAPUSEABLECOUNT( x )          ( 0 + \
                                                ( SPELL_HAS_HEAL( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_GLOW( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_EVAC( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_ZOOM( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_REPEL( x ) ? 1 : 0 ) + \
                                                ( SPELL_HAS_MIDHEAL( x ) ? 1 : 0 ) )

#define ITEM_MAXKEYS                            6
#define ITEM_MAXHERBS                           7
#define ITEM_MAXWINGS                           4
#define ITEM_MAXFAIRYWATERS                     4
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

#define ITEM_GET_MAPNONUSEABLECOUNT( x )        ( 0 + \
                                                ( ITEM_GET_KEYCOUNT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_STONEOFSUNLIGHT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_STAFFOFRAIN( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_TOKEN( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_SPHEREOFLIGHT( x ) ? 1 : 0 ) + \
                                                ( ITEM_HAS_DRAGONSCALE( x ) ? 1 : 0 ) )

#define ITEM_SET_KEYCOUNT( x, c )               ( x ) = ( ( ( x ) & 0xFFFFFFF8 ) | ( ( c ) & 0x7 ) )
#define ITEM_SET_HERBCOUNT( x, c )              ( x ) = ( ( ( x ) & 0xFFFFFFC7 ) | ( (uint32_t)( c ) & 0x7 ) << 3 )
#define ITEM_SET_WINGCOUNT( x, c )              ( x ) = ( ( ( x ) & 0xFFFFFE3F ) | ( (uint32_t)( c ) & 0x7 ) << 6 )
#define ITEM_SET_FAIRYWATERCOUNT( x, c )        ( x ) = ( ( ( x ) & 0xFFFFF1FF ) | ( (uint32_t)( c ) & 0x7 ) << 9 )
#define ITEM_SET_TORCHCOUNT( x, c )             ( x ) = ( ( ( x ) & 0xFFFF8FFF ) | ( (uint32_t)( c ) & 0x7 ) << 12 )
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

typedef struct Player_t
{
   Screen_t* screen;

   ActiveSprite_t sprite;
   Vector2i32_t spriteOffset;
   Vector2f_t velocity;
   float maxVelocity;
   Vector2u32_t hitBoxSize;

   uint32_t tileIndex;
   Bool_t isCursed;

   char name[9];
   BattleStats_t stats;
   uint16_t experience;
   uint16_t gold;

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
   // bits 25-31: reserved
   uint32_t items;

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
   // bits 10-15: reserved
   uint16_t spells;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_Init( Player_t* player, Screen_t* screen );
uint8_t Player_GetLevel( Player_t* player );
uint16_t Player_GetExperienceRemaining( Player_t* player );
uint16_t Player_CollectGold( Player_t* player, uint16_t gold );
uint16_t Player_CollectExperience( Player_t* player, uint16_t experience );
Bool_t Player_CollectItem( Player_t* player, Item_t item );
void Player_SetCursed( Player_t* player, Bool_t cursed );

#if defined( __cplusplus )
}
#endif

#endif // ITEM_H
