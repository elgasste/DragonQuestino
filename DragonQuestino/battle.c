#include "battle.h"
#include "player.h"
#include "tile_map.h"
#include "random.h"

internal uint32_t Battle_GenerateEnemyIndex( Battle_t* battle );

void Battle_Init( Battle_t* battle, Player_t* player, TileMap_t* tileMap )
{
   battle->player = player;
   battle->tileMap = tileMap;

   // enemies have infinite magic
   battle->enemy.stats.magicPoints = 0;
   battle->enemy.stats.maxMagicPoints = 0;

   battle->specialEnemy = SpecialEnemy_None;
}

void Battle_Generate( Battle_t* battle )
{
   uint32_t enemyIndex;
   Enemy_t* enemy = &( battle->enemy );

   switch ( battle->specialEnemy )
   {
      case SpecialEnemy_GreenDragon: enemyIndex = ENEMY_GREENDRAGON_INDEX; break;
      case SpecialEnemy_AxeKnight: enemyIndex = ENEMY_AXEKNIGHT_INDEX; break;
      case SpecialEnemy_Golem: enemyIndex = ENEMY_GOLEM_INDEX; break;
      default: enemyIndex = Battle_GenerateEnemyIndex( battle ); break;
   }

   Enemy_Load( enemy, enemyIndex );

   enemy->stats.maxHitPoints = Random_u8( enemy->minHitPoints, enemy->maxHitPoints );
   enemy->stats.hitPoints = enemy->stats.maxHitPoints;
   enemy->gold = Random_u8( enemy->minGold, enemy->maxGold );
}

internal uint32_t Battle_GenerateEnemyIndex( Battle_t* battle )
{
   TileMap_t* tileMap = battle->tileMap;
   Player_t* player = battle->player;
   uint32_t adjustedTileIndex = player->tileIndex + ( ( player->tileIndex / tileMap->tilesX ) * ( TILE_COUNT_X - tileMap->tilesX ) );
   uint32_t enemyPoolIndex = TILE_GET_ENEMYPOOLINDEX( tileMap->tiles[adjustedTileIndex] );
   uint32_t i, enemyIndex;

   if ( tileMap->isDungeon )
   {
      i = Random_u32( 0, tileMap->dungeonEnemyIndexPools[enemyPoolIndex].enemyCount - 1 );
      enemyIndex = tileMap->dungeonEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }
   else
   {
      i = Random_u32( 0, tileMap->overworldEnemyIndexPools[enemyPoolIndex].enemyCount - 1 );
      enemyIndex = tileMap->overworldEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }

   return enemyIndex;
}
