#include "game.h"
#include "random.h"

internal uint32_t Battle_GenerateEnemyIndex( Battle_t* battle );
internal uint8_t Battle_GetAttackResult( Battle_t* battle );
internal Bool_t Battle_GetFleeResult( Battle_t* battle );

void Battle_Init( Battle_t* battle, Game_t* game )
{
   battle->game = game;

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

void Battle_AttemptAttack( Battle_t* battle )
{
   uint8_t payload = Battle_GetAttackResult( battle );
   char msg[64];

   if ( payload > 0 )
   {
      sprintf( msg, STRING_BATTLE_ATTACKATTEMPTSUCCEEDED, battle->enemy.name, payload );
      Dialog_SetInsertionText( &( battle->game->dialog ), msg );
      Game_OpenDialog( battle->game, DialogId_Battle_AttackAttemptSucceeded );
   }
   else
   {
      Dialog_SetInsertionText( &( battle->game->dialog ), battle->enemy.name );
      Game_OpenDialog( battle->game, DialogId_Battle_AttackAttemptFailed );
   }
}

void Battle_AttemptFlee( Battle_t* battle )
{
   Bool_t fleed = Battle_GetFleeResult( battle );

   Dialog_SetInsertionText( &( battle->game->dialog ), battle->enemy.name );

   if ( fleed )
   {  
      Game_OpenDialog( battle->game, DialogId_Battle_FleeAttemptSucceeded );
   }
   else
   {
      Game_OpenDialog( battle->game, DialogId_Battle_FleeAttemptFailed );
   }
}

internal uint32_t Battle_GenerateEnemyIndex( Battle_t* battle )
{
   TileMap_t* tileMap = &( battle->game->tileMap );
   Player_t* player = &( battle->game->player );
   uint32_t adjustedTileIndex = player->tileIndex + ( ( player->tileIndex / tileMap->tilesX ) * ( TILE_COUNT_X - tileMap->tilesX ) );
   uint32_t enemyPoolIndex = TILE_GET_ENEMYPOOLINDEX( tileMap->tiles[adjustedTileIndex] );
   uint32_t i, enemyIndex;

   if ( tileMap->isDungeon )
   {
      i = Random_u32( 0, TILE_ENEMY_POOL_ENEMY_INDEX_COUNT - 1 );
      enemyIndex = tileMap->dungeonEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }
   else
   {
      i = Random_u32( 0, TILE_ENEMY_POOL_ENEMY_INDEX_COUNT - 1 );
      enemyIndex = tileMap->overworldEnemyIndexPools[enemyPoolIndex].enemyIndexes[i];
   }

   return enemyIndex;
}

internal uint8_t Battle_GetAttackResult( Battle_t* battle )
{
   // TODO: use the algorithm in the tech guide
   UNUSED_PARAM( battle );
   return 0;
}

internal Bool_t Battle_GetFleeResult( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );
   float enemyFleeFactor = Enemy_GetFleeFactor( enemy );

   if ( battle->specialEnemy != SpecialEnemy_None )
   {
      return False;
   }

   uint16_t playerFactor = (uint16_t)( player->stats.agility ) * Random_u8( 0, UINT8_MAX );
   uint16_t enemyFactor = (uint16_t)( enemy->stats.agility ) * Random_u8( 0, UINT8_MAX );
   enemyFactor = (uint16_t)( enemyFactor * enemyFleeFactor );

   return ( playerFactor < enemyFactor ) ? False : True;
}
