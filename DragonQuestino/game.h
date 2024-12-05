#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "screen.h"
#include "tile_map.h"
#include "clock.h"

typedef struct Game_t
{
   Screen_t screen;
   TileMap_t tileMap;
   Clock_t clock;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Game_Init( Game_t* game );
void Game_Tic( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
