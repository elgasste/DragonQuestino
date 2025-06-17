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
internal void Battle_AttemptAttackCallback( Battle_t* battle );
internal void Battle_AttackDodgedCallback( Battle_t* battle );
internal void Battle_EnemyDefeatedCallback( Battle_t* battle );
internal void Battle_EnemyDefeatedMessageCallback( Battle_t* battle );

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

void Battle_AttemptAttack( Battle_t* battle )
{
   battle->game->screen.needsRedraw = True;
   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_ATTACKATTEMPT, Battle_AttemptAttackCallback, battle );
   Game_OpenDialog( battle->game );
}

void Battle_AttemptFlee( Battle_t* battle )
{
   Bool_t fleed = Battle_GetFleeResult( battle );

   battle->game->screen.needsRedraw = True;
   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_FLEEATTEMPT, fleed ? Battle_FleeSucceededCallback : Battle_FleeFailedCallback, battle );
   Game_OpenDialog( battle->game );
}

void Battle_AttackSucceededCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   battle->enemy.stats.hitPoints -= battle->pendingPayload8u;
   sprintf( msg,
            battle->excellentMove ? STRING_BATTLE_ATTACKEXCELLENTMOVE : STRING_BATTLE_ATTACKSUCCEEDED,
            battle->enemy.name, battle->pendingPayload8u, ( battle->pendingPayload8u == 1 ) ? STRING_POINT : STRING_POINTS );

   if ( battle->enemy.stats.hitPoints == 0 )
   {
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyDefeatedCallback, battle );
   }
   else
   {
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Game_ResetBattleMenu, battle->game );
   }

   Game_OpenDialog( battle->game );
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
   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_FLEEATTEMPTSUCCEEDED, battle->enemy.name );
   Dialog_PushSection( &( battle->game->dialog ), msg );
   Game_OpenDialog( battle->game );
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

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_FLEEATTEMPTFAILED, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Game_ResetBattleMenu, battle->game );
   Game_OpenDialog( battle->game );
}

internal void Battle_AttemptAttackCallback( Battle_t* battle )
{
   battle->pendingPayload8u = Battle_GetAttackDamage( battle );
   AnimationChain_Reset( &( battle->game->animationChain ) );

   if ( battle->pendingPayload8u == 0 )
   {
      AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
      AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_AttackDodgedCallback, battle );
   }
   else
   {
      AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
      AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Battle_EnemyDamage, Battle_AttackSucceededCallback, battle );
   }

   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_AttackDodgedCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ATTACKDODGED, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Game_ResetBattleMenu, battle->game );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyDefeatedCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Battle_EnemyFadeOut, Battle_EnemyDefeatedMessageCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyDefeatedMessageCallback( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );
   Dialog_t* dialog = &( battle->game->dialog );
   uint8_t newLevel;
   uint16_t i, learnedSpell = 0;
   char msg[64];

   Dialog_Reset( dialog );
   sprintf( msg, STRING_BATTLE_VICTORY, battle->enemy.name );
   Dialog_PushSection( dialog, msg );

   battle->isOver = True;
   battle->experienceGained = Player_CollectExperience( player, enemy->experience );
   battle->goldGained = Player_CollectGold( player, enemy->gold );
   newLevel = Player_GetLevelFromExperience( player );
   battle->previousSpells = player->spells;

   if ( battle->experienceGained > 0 || battle->goldGained > 0 )
   {
      if ( battle->experienceGained > 0 && battle->goldGained == 0 )
      {
         sprintf( msg, STRING_BATTLE_EXPERIENCEONLY, battle->experienceGained, ( battle->experienceGained == 1 ) ? STRING_POINT : STRING_POINTS );
      }
      else if ( battle->experienceGained == 0 && battle->goldGained > 0 )
      {
         sprintf( msg, STRING_BATTLE_GOLDONLY, battle->goldGained );
      }
      else
      {
         sprintf( msg, STRING_BATTLE_EXPERIENCEANDGOLD,
                  battle->experienceGained, ( battle->experienceGained == 1 ) ? STRING_POINT : STRING_POINTS,
                  battle->goldGained );
      }

      Dialog_PushSection( dialog, msg );
   }

   if ( newLevel > player->level )
   {
      battle->strengthGained =  g_strengthTable[newLevel] - player->stats.strength;
      battle->agilityGained = g_agilityTable[newLevel] - player->stats.agility;
      battle->hitPointsGained = g_hitPointsTable[newLevel] - player->stats.maxHitPoints;
      battle->magicPointsGained = g_magicPointsTable[newLevel] - player->stats.maxMagicPoints;
      player->stats.strength += battle->strengthGained;
      player->stats.agility += battle->agilityGained;
      Player_UpdateSpellsToLevel( player, newLevel );
      Dialog_PushSection( dialog, STRING_BATTLE_LEVELUP );

      if ( battle->strengthGained > 0 || battle->agilityGained > 0 )
      {
         if ( battle->strengthGained > 0 && battle->agilityGained == 0 )
         {
            sprintf( msg, STRING_BATTLE_STRENGTHGAIN, battle->strengthGained );
         }
         else if ( battle->strengthGained == 0 && battle->agilityGained > 0 )
         {
            sprintf( msg, STRING_BATTLE_AGILITYGAIN, battle->agilityGained );
         }
         else
         {
            sprintf( msg, STRING_BATTLE_STRENGTHANDAGILITYGAIN, battle->strengthGained, battle->agilityGained );
         }

         Dialog_PushSection( dialog, msg );
      }

      if ( battle->hitPointsGained > 0 || battle->magicPointsGained > 0 )
      {
         if ( battle->hitPointsGained > 0 && battle->magicPointsGained == 0 )
         {
            sprintf( msg, STRING_BATTLE_HITPOINTSGAIN, battle->hitPointsGained );
         }
         else if ( battle->hitPointsGained == 0 && battle->magicPointsGained > 0 )
         {
            sprintf( msg, STRING_BATTLE_MAGICPOINTSGAIN, battle->magicPointsGained );
         }
         else
         {
            sprintf( msg, STRING_BATTLE_HITPOINTSANDMAGICPOINTSGAIN, battle->hitPointsGained, battle->magicPointsGained );
         }

         Dialog_PushSection( dialog, msg );
      }

      if ( player->spells > battle->previousSpells )
      {
         for ( i = 0; i < SPELL_TABLE_SIZE; i++ )
         {
            if ( ( battle->previousSpells & ( 0x1 << i ) ) != ( player->spells & ( 0x1 << i ) ) )
            {
               learnedSpell = i;
               break;
            }
         }

         switch ( i )
         {
            case 0: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_HEAL ); break;
            case 1: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_SIZZ ); break;
            case 2: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_SLEEP ); break;
            case 3: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_GLOW ); break;
            case 4: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_FIZZLE ); break;
            case 5: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_EVAC ); break;
            case 6: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_ZOOM ); break;
            case 7: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_REPEL ); break;
            case 8: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_MIDHEAL ); break;
            case 9: sprintf( msg, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_SIZZLE ); break;
         }

         Dialog_PushSection( dialog, msg );
      }
   }

   Game_OpenDialog( battle->game );
}
