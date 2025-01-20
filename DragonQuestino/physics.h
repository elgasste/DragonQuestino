#if !defined( PHYSICS_H )
#define PHYSICS_H

#define COLLISION_THETA  0.001f

#include "common.h"

typedef struct Game_t Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Physics_Tic( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // PHYSICS_H
