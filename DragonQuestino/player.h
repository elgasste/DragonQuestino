#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "sprite.h"
#include "battle_stats.h"

#define PLAYER_MAXKEYS                         6
#define PLAYER_MAXHERBS                        7
#define PLAYER_MAXWINGS                        4
#define PLAYER_MAXFAIRYWATERS                  4
#define PLAYER_MAXTORCHES                      7

#define PLAYER_GET_KEYCOUNT( x )               ( ( x ) & 0x7 )
#define PLAYER_GET_HERBCOUNT( x )              ( ( ( x ) >> 3 ) & 0x7 )
#define PLAYER_GET_WINGCOUNT( x )              ( ( ( x ) >> 6 ) & 0x7 )
#define PLAYER_GET_FAIRYWATERCOUNT( x )        ( ( ( x ) >> 9 ) & 0x7 )
#define PLAYER_GET_TORCHCOUNT( x )             ( ( ( x ) >> 12 ) & 0x7 )
#define PLAYER_HAS_FAIRYFLUTE( x )             ( ( ( x ) >> 15 ) & 0x1 )
#define PLAYER_HAS_SILVERHARP( x )             ( ( ( x ) >> 16 ) & 0x1 )
#define PLAYER_HAS_GWAELYNSLOVE( x )           ( ( ( x ) >> 17 ) & 0x1 )
#define PLAYER_HAS_STONEOFSUNLIGHT( x )        ( ( ( x ) >> 18 ) & 0x1 )
#define PLAYER_HAS_STAFFOFRAIN( x )            ( ( ( x ) >> 19 ) & 0x1 )
#define PLAYER_HAS_TOKEN( x )                  ( ( ( x ) >> 20 ) & 0x1 )
#define PLAYER_HAS_RAINBOWDROP( x )            ( ( ( x ) >> 21 ) & 0x1 )
#define PLAYER_HAS_SPHEREOFLIGHT( x )          ( ( ( x ) >> 22 ) & 0x1 )
#define PLAYER_HAS_DRAGONSCALE( x )            ( ( ( x ) >> 23 ) & 0x1 )
#define PLAYER_HAS_CURSEDBELT( x )             ( ( ( x ) >> 24 ) & 0x1 )

#define PLAYER_GET_MAPUSEABLEITEMCOUNT( x )    ( 0 + \
                                               ( PLAYER_GET_HERBCOUNT( x ) ? 1 : 0 ) + \
                                               ( PLAYER_GET_WINGCOUNT( x ) ? 1 : 0 ) + \
                                               ( PLAYER_GET_FAIRYWATERCOUNT( x ) ? 1 : 0 ) + \
                                               ( PLAYER_GET_TORCHCOUNT( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_FAIRYFLUTE( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_SILVERHARP( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_GWAELYNSLOVE( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_RAINBOWDROP( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_CURSEDBELT( x ) ? 1 : 0 ) )

#define PLAYER_GET_MAPNONUSEABLEITEMCOUNT( x ) ( 0 + \
                                               ( PLAYER_GET_KEYCOUNT( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_STONEOFSUNLIGHT( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_STAFFOFRAIN( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_TOKEN( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_SPHEREOFLIGHT( x ) ? 1 : 0 ) + \
                                               ( PLAYER_HAS_DRAGONSCALE( x ) ? 1 : 0 ) )

#define PLAYER_SET_KEYCOUNT( x, c )            ( x ) = ( ( ( x ) & 0xFFFFFFF8 ) | ( ( c ) & 0x7 ) )
#define PLAYER_SET_HERBCOUNT( x, c )           ( x ) = ( ( ( x ) & 0xFFFFFFC7 ) | ( (uint32_t)( c ) & 0x7 ) << 3 )
#define PLAYER_SET_WINGCOUNT( x, c )           ( x ) = ( ( ( x ) & 0xFFFFFE3F ) | ( (uint32_t)( c ) & 0x7 ) << 6 )
#define PLAYER_SET_FAIRYWATERCOUNT( x, c )     ( x ) = ( ( ( x ) & 0xFFFFF1FF ) | ( (uint32_t)( c ) & 0x7 ) << 9 )
#define PLAYER_SET_TORCHCOUNT( x, c )          ( x ) = ( ( ( x ) & 0xFFFF8FFF ) | ( (uint32_t)( c ) & 0x7 ) << 12 )
#define PLAYER_TOGGLE_HASFAIRYFLUTE( x )       ( x ) ^= ( 0x1 << 15 )
#define PLAYER_TOGGLE_HASSILVERHARP( x )       ( x ) ^= ( 0x1 << 16 )
#define PLAYER_TOGGLE_HASGWAELYNSLOVE( x )     ( x ) ^= ( 0x1 << 17 )
#define PLAYER_TOGGLE_HASSTONEOFSUNLIGHT( x )  ( x ) ^= ( 0x1 << 18 )
#define PLAYER_TOGGLE_HASSTAFFOFRAIN( x )      ( x ) ^= ( 0x1 << 19 )
#define PLAYER_TOGGLE_HASTOKEN( x )            ( x ) ^= ( 0x1 << 20 )
#define PLAYER_TOGGLE_HASRAINBOWDROP( x )      ( x ) ^= ( 0x1 << 21 )
#define PLAYER_TOGGLE_HASSPHEREOFLIGHT( x )    ( x ) ^= ( 0x1 << 22 )
#define PLAYER_TOGGLE_HASDRAGONSCALE( x )      ( x ) ^= ( 0x1 << 23 )
#define PLAYER_TOGGLE_HASCURSEDBELT( x )       ( x ) ^= ( 0x1 << 24 )

typedef struct Player_t
{
   ActiveSprite_t sprite;
   Vector2i32_t spriteOffset;
   Vector2f_t velocity;
   float maxVelocity;
   Vector2u32_t hitBoxSize;

   uint32_t tileIndex;

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

   Bool_t isCursed;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_Init( Player_t* player );
uint8_t Player_GetLevel( Player_t* player );
uint16_t Player_CollectGold( Player_t* player, uint16_t gold );
uint16_t Player_CollectExperience( Player_t* player, uint16_t experience );
Bool_t Player_CollectItem( Player_t* player, Item_t item );

#if defined( __cplusplus )
}
#endif

#endif // PLAYER_H
