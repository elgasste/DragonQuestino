#include "game.h"
#include "random.h"
#include "tables.h"

internal uint32_t Battle_GenerateEnemyIndex( Battle_t* battle );
internal uint8_t Battle_GetAttackDamage( Battle_t* battle );
internal Bool_t Battle_GetFleeResult( Battle_t* battle );
internal void Battle_FleeSucceededCallback( Battle_t* battle );
internal void Battle_FleeSucceededMessageCallback( Battle_t* battle );
internal void Battle_FleeFailedCallback( Battle_t* battle );
internal void Battle_FleeFailedMessageCallback( Battle_t* battle );

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

   battle->isOver = False;
   battle->game->player.stats.isFizzled = False;

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
   enemy->stats.isFizzled = False;
}

// MUFFINS: let's do this next
void Battle_AttemptAttack( Battle_t* battle )
{
   uint8_t damage = Battle_GetAttackDamage( battle );
   char msg[64];

   battle->game->screen.needsRedraw = True;

   if ( damage > 0 )
   {
      battle->enemy.stats.hitPoints -= damage;
      sprintf( msg,
               battle->excellentMove ? STRING_BATTLE_ATTACKEXCELLENTMOVE : STRING_BATTLE_ATTACKATTEMPTSUCCEEDED,
               battle->enemy.name, damage, ( damage == 1 ) ? STRING_POINT : STRING_POINTS );
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

   battle->game->screen.needsRedraw = True;
   Dialog2_Reset( &( battle->game->dialog2 ) );
   Dialog2_PushSectionWithCallback( &( battle->game->dialog2 ), STRING_BATTLE_FLEEATTEMPT, fleed ? Battle_FleeSucceededCallback : Battle_FleeFailedCallback, battle );
   Game_OpenDialog2( battle->game );
}

void Battle_Victory( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );
   DialogId_t dialogId;
   uint8_t newLevel;

   battle->experienceGained = Player_CollectExperience( player, enemy->experience );
   battle->goldGained = Player_CollectGold( player, enemy->gold );
   newLevel = Player_GetLevelFromExperience( player );
   battle->previousSpells = player->spells;

   if ( newLevel > player->level )
   {
      battle->strengthGained =  g_strengthTable[newLevel] - player->stats.strength;
      battle->agilityGained = g_agilityTable[newLevel] - player->stats.agility;
      battle->hitPointsGained = g_hitPointsTable[newLevel] - player->stats.maxHitPoints;
      battle->magicPointsGained = g_magicPointsTable[newLevel] - player->stats.maxMagicPoints;
      player->stats.strength += battle->strengthGained;
      player->stats.agility += battle->agilityGained;
      Player_UpdateSpellsToLevel( player, newLevel );
      dialogId = DialogId_Battle_VictoryWithLevelUp;
   }
   else
   {
      dialogId = battle->experienceGained == 0 && battle->goldGained == 0 ? DialogId_Battle_Victory : DialogId_Battle_VictoryWithSpoils;
   }

   Dialog_SetInsertionText( &( battle->game->dialog ), enemy->name );
   Game_OpenDialog( battle->game, dialogId );
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

internal uint8_t Battle_GetAttackDamage( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );
   uint8_t defense, damage, minDamage = 0, maxDamage = 0;

   if ( enemy->stats.dodge > 0 && Random_u8( 1, 64 ) <= enemy->stats.dodge )
   {
      return 0;
   }

   if ( battle->specialEnemy != SpecialEnemy_Dragonlord && Random_u8( 1, 32 ) == 1 )
   {
      battle->excellentMove = True;
      minDamage = player->stats.strength / 2;
      maxDamage = player->stats.strength;
   }
   else
   {
      battle->excellentMove = False;
      defense = enemy->stats.agility / 2;

      if ( defense < player->stats.strength )
      {
         minDamage = ( player->stats.strength - defense ) / 4;
         maxDamage = ( player->stats.strength - defense ) / 2;
      }
   }

   damage = Random_u8( minDamage, maxDamage );

   if ( damage == 0 )
   {
      damage = Random_u8( 0, 1 );
   }

   return ( damage > enemy->stats.hitPoints ) ? enemy->stats.hitPoints : damage;
}

internal Bool_t Battle_GetFleeResult( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );
   float enemyFleeFactor;

   if ( battle->specialEnemy != SpecialEnemy_None )
   {
      return False;
   }

   enemyFleeFactor = Enemy_GetFleeFactor( enemy );
   uint16_t playerFactor = (uint16_t)( player->stats.agility ) * Random_u8( 0, UINT8_MAX );
   uint16_t enemyFactor = (uint16_t)( enemy->stats.agility ) * Random_u8( 0, UINT8_MAX );
   enemyFactor = (uint16_t)( enemyFactor * enemyFleeFactor );

   return ( playerFactor < enemyFactor ) ? False : True;
}

internal void Battle_FleeSucceededCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Battle_EnemyFadeOut, Battle_FleeSucceededMessageCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_FleeSucceededMessageCallback( Battle_t* battle )
{
   char msg[64];

   battle->isOver = True;
   Dialog2_Reset( &( battle->game->dialog2 ) );
   sprintf( msg, STRING_BATTLE_FLEEATTEMPTSUCCEEDED, battle->enemy.name );
   Dialog2_PushSection( &( battle->game->dialog2 ), msg );
   Game_OpenDialog2( battle->game );
}

internal void Battle_FleeFailedCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_FleeFailedMessageCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_FleeFailedMessageCallback( Battle_t* battle )
{
   char msg[64];

   Dialog2_Reset( &( battle->game->dialog2 ) );
   sprintf( msg, STRING_BATTLE_FLEEATTEMPTFAILED, battle->enemy.name );
   Dialog2_PushSectionWithCallback( &( battle->game->dialog2 ), msg, Game_ResetBattleMenu, battle->game );
   Game_OpenDialog2( battle->game );
}
