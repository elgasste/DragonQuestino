#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "sprite.h"
#include "battle_stats.h"

#define PLAYER_MAXKEYS                         6
#define PLAYER_MAXHERBS                        7
#define PLAYER_MAXWINGS                        4
#define PLAYER_MAXFAIRYWATERS                  4

#define PLAYER_GET_KEYCOUNT( x )               ( ( x ) & 0x7 )
#define PLAYER_GET_HERBCOUNT( x )              ( ( ( x ) >> 3 ) & 0x7 )
#define PLAYER_GET_WINGCOUNT( x )              ( ( ( x ) >> 6 ) & 0x7 )
#define PLAYER_GET_FAIRYWATERCOUNT( x )        ( ( ( x ) >> 9 ) & 0x7 )
#define PLAYER_GET_HASTABLET( x )              ( ( x ) >> 12 & 0x1 )
#define PLAYER_GET_HASSTONEOFSUNLIGHT( x )     ( ( x ) >> 13 & 0x1 )
#define PLAYER_GET_HASSTAFFOFRAIN( x )         ( ( x ) >> 14 & 0x1 )
#define PLAYER_GET_HASRAINBOWDROP( x )         ( ( x ) >> 15 & 0x1 )
#define PLAYER_GET_HASSILVERHARP( x )          ( ( x ) >> 16 & 0x1 )
#define PLAYER_GET_HASFAIRYFLUTE( x )          ( ( x ) >> 17 & 0x1 )
#define PLAYER_GET_HASGWAELYNSLOVE( x )        ( ( x ) >> 18 & 0x1 )
#define PLAYER_GET_HASTOKEN( x )               ( ( x ) >> 19 & 0x1 )
#define PLAYER_GET_HASSPHEREOFLIGHT( x )       ( ( x ) >> 20 & 0x1 )

#define PLAYER_GET_MAPUSEABLEITEMCOUNT( x )    ( 0 + \
                                               ( GET_ITEM_KEYCOUNT( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HERBCOUNT( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_WINGCOUNT( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_FAIRYWATERCOUNT( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HASRAINBOWDROP( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HASSILVERHARP( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HASFAIRYFLUTE( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HASGWAELYNSLOVE( x ) ? 1 : 0 ) )

#define PLAYER_GET_MAPNONUSEABLEITEMCOUNT( x ) ( 0 + \
                                               ( GET_ITEM_HASSTONEOFSUNLIGHT( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HASSTAFFOFRAIN( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HASTOKEN( x ) ? 1 : 0 ) + \
                                               ( GET_ITEM_HASSPHEREOFLIGHT( x ) ? 1 : 0 ) )

#define PLAYER_SET_KEYCOUNT( x, c )            ( x ) = ( ( ( x ) & 0xFFFFFFF8 ) | ( ( c ) & 0x7 ) )
#define PLAYER_SET_HERBCOUNT( x, c )           ( x ) = ( ( ( x ) & 0xFFFFFFC7 ) | ( (uint32_t)( c ) & 0x7 ) << 3 )
#define PLAYER_SET_WINGCOUNT( x, c )           ( x ) = ( ( ( x ) & 0xFFFFFE3F ) | ( (uint32_t)( c ) & 0x7 ) << 6 )
#define PLAYER_SET_FAIRYWATERCOUNT( x, c )     ( x ) = ( ( ( x ) & 0xFFFFF1FF ) | ( (uint32_t)( c ) & 0x7 ) << 9 )
#define PLAYER_SET_HASTABLET( x, b )           x |= ( ( uint32_t )( b ) << 12 )
#define PLAYER_SET_HASSTONEOFSUNLIGHT( x, b )  x |= ( ( uint32_t )( b ) << 13 )
#define PLAYER_SET_HASSTAFFOFRAIN( x, b )      x |= ( ( uint32_t )( b ) << 14 )
#define PLAYER_SET_HASRAINBOWDROP( x, b )      x |= ( ( uint32_t )( b ) << 15 )
#define PLAYER_SET_HASSILVERHARP( x, b )       x |= ( ( uint32_t )( b ) << 16 )
#define PLAYER_SET_HASFAIRYFLUTE( x, b )       x |= ( ( uint32_t )( b ) << 17 )
#define PLAYER_SET_HASGWAELYNSLOVE( x, b )     x |= ( ( uint32_t )( b ) << 18 )
#define PLAYER_SET_HASTOKEN( x, b )            x |= ( ( uint32_t )( b ) << 19 )
#define PLAYER_SET_HASSPHEREOFLIGHT( x, b )    x |= ( ( uint32_t )( b ) << 20 )

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

   // lowest 3 bits: keys
   // next 3 bits: herbs
   // next 3 bits: wings
   // next 3 bits: fairy waters
   // bit 12: fairy flute
   // bit 13: silver harp
   // bit 14: gwaelin's love
   // bit 15: stone of sunlight
   // bit 16: staff of rain
   // bit 17: erdrick's token
   // bit 18: rainbow drop
   // bit 19: ball of light
   // bits 20-31: reserved
   uint32_t items;
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
