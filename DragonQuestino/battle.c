#include "battle.h"
#include "player.h"
#include "tile_map.h"
#include "random.h"

void Battle_Init( Battle_t* battle, Player_t* player, TileMap_t* tileMap )
{
   battle->player = player;
   battle->tileMap = tileMap;

   // enemies have infinite magic
   battle->enemy.stats.magicPoints = 0;
   battle->enemy.stats.maxMagicPoints = 0;
}

void Battle_Generate( Battle_t* battle )
{
   Player_t* player = battle->player;
   TileMap_t* tileMap = battle->tileMap;
   uint32_t enemyPoolIndex = TILE_GET_ENEMYPOOLINDEX( tileMap->tiles[player->tileIndex] );
   uint32_t i, enemyIndex;

   if ( tileMap->isDungeon )
   {
      i = Random_u32( 0, tileMap->dungeonEnemyIndexPools[enemyPoolIndex].enemyCount );
      enemyIndex = tileMap->dungeonEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }
   else
   {
      i = Random_u32( 0, tileMap->overworldEnemyIndexPools[enemyPoolIndex].enemyCount );
      enemyIndex = tileMap->overworldEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }

   // TODO: this doesn't load the "strength" stat, but really I'm not sure what that actually does
   Enemy_Load( &( battle->enemy ), enemyIndex );

   // TODO: possibly adjust this later, 75% seems a little much
   battle->enemy.stats.hitPoints = Random_u8( (uint8_t)( battle->enemy.stats.maxHitPoints * 0.75f ), battle->enemy.stats.maxHitPoints );
}
