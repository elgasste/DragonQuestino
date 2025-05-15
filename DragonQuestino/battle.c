#include "battle.h"
#include "player.h"
#include "tile_map.h"
#include "random.h"

void Battle_Init( Battle_t* battle, Player_t* player, TileMap_t* tileMap )
{
   battle->player = player;
   battle->tileMap = tileMap;
   battle->enemyIndex = 0;
}

void Battle_Generate( Battle_t* battle )
{
   Player_t* player = battle->player;
   TileMap_t* tileMap = battle->tileMap;
   uint32_t enemyPoolIndex = TILE_GET_ENEMYPOOLINDEX( tileMap->tiles[player->tileIndex] );
   uint32_t i;

   if ( tileMap->isDungeon )
   {
      i = Random_u32( 0, tileMap->dungeonEnemyIndexPools[enemyPoolIndex].enemyCount );
      battle->enemyIndex = tileMap->dungeonEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }
   else
   {
      i = Random_u32( 0, tileMap->overworldEnemyIndexPools[enemyPoolIndex].enemyCount );
      battle->enemyIndex = tileMap->overworldEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }
}
