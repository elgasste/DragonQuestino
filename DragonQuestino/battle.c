#include "game.h"
#include "random.h"
#include "tables.h"
#include "math.h"

internal u32 Battle_GenerateEnemyIndex( Battle_t* battle );
internal u8 Battle_GetAttackDamage( Battle_t* battle );
internal Bool_t Battle_GetFleeResult( Battle_t* battle );
internal void Battle_FleeSucceededCallback( Battle_t* battle );
internal void Battle_FleeSucceededMessageCallback( Battle_t* battle );
internal void Battle_FleeSucceededSpecialEnemyCheck( Battle_t* battle );
internal void Battle_FleeFailedCallback( Battle_t* battle );
internal void Battle_FleeFailedMessageCallback( Battle_t* battle );
internal void Battle_AttackCallback( Battle_t* battle );
internal void Battle_AttackDodgedCallback( Battle_t* battle );
internal void Battle_AttackIneffectiveCallback( Battle_t* battle );
internal void Battle_EnemyDefeatedCallback( Battle_t* battle );
internal void Battle_EnemyDefeatedMessageCallback( Battle_t* battle );
internal void Battle_EnemyDefeatedSpoilsCallback( Battle_t* battle );
internal void Battle_NewLevelCallback( Battle_t* battle );
internal void Battle_GainedPointsCallback( Battle_t* battle );
internal void Battle_SwitchTurnCallback( Battle_t* battle );
internal void Battle_EnemyInitiativeCallback( Battle_t* battle );
internal void Battle_EnemyInitiativeMessageCallback( Battle_t* battle );
internal void Battle_EnemyTurn( Battle_t* battle );
internal void Battle_EnemyWokeUpCallback( Battle_t* battle );
internal void Battle_EnemyInitiateBehavior( Battle_t* battle );
internal void Battle_EnemyActionOrAttack( Battle_t* battle, void ( *action )( Battle_t* ), u32 chance );
internal Bool_t Battle_EnemyTryHeal( Battle_t* battle, Bool_t midHeal, u32 chance );
internal Bool_t Battle_EnemyTryFizzle( Battle_t* battle, u32 chance );
internal Bool_t Battle_EnemyTrySleep( Battle_t* battle, u32 chance );
internal void Battle_EnemyAttack( Battle_t* battle );
internal void Battle_PushPlayerHurtAnimation( Battle_t* battle );
internal void Battle_EnemyAttackCallback( Battle_t* battle );
internal u8 Battle_GetEnemyAttackDamage( Battle_t* battle );
internal void Battle_EnemyAttackDodgedCallback( Battle_t* battle );
internal void Battle_EnemyAttackSucceededCallback( Battle_t* battle );
internal void Battle_PlayerDefeatedCallback( Battle_t* battle );
internal void Battle_PlayerDefeatedCallbackMessage( Battle_t* battle );
internal void Battle_PlayerDefeatedDialogCallback( Battle_t* battle );
internal void Battle_PlayerDefeatedPostDialogCallback( Battle_t* battle );
internal void Battle_EnemyBreatheFire( Battle_t* battle );
internal void Battle_EnemyBreatheFireCallback( Battle_t* battle );
internal void Battle_EnemyBreatheStrongFire( Battle_t* battle );
internal void Battle_EnemyBreatheStrongFireCallback( Battle_t* battle );
internal void Battle_EnemyCastSpell( Battle_t* battle, const char* spellName, void ( *callback )( Battle_t* ) );
internal void Battle_EnemySpellFizzledCallback( Battle_t* battle );
internal void Battle_EnemySpellFizzledMessageCallback( Battle_t* battle );
internal void Battle_EnemyCastSizz( Battle_t* battle );
internal void Battle_EnemyCastSizzCallback( Battle_t* battle );
internal void Battle_EnemyCastSizzle( Battle_t* battle );
internal void Battle_EnemyCastSizzleCallback( Battle_t* battle );
internal void Battle_EnemyAnimateSpellWithCallback( Battle_t* battle, void ( *callback )( Battle_t* battle ) );
internal void Battle_EnemyCastHeal( Battle_t* battle );
internal void Battle_EnemyCastHealCallback( Battle_t* battle );
internal void Battle_EnemyHealMessageCallback( Battle_t* battle );
internal void Battle_EnemyCastMidheal( Battle_t* battle );
internal void Battle_EnemyCastMidhealCallback( Battle_t* battle );
internal void Battle_EnemyCastFizzle( Battle_t* battle );
internal void Battle_EnemyCastFizzleCallback( Battle_t* battle );
internal void Battle_EnemyCastFizzleMessageCallback( Battle_t* battle );
internal void Battle_EnemyCastSleep( Battle_t* battle );
internal void Battle_EnemyCastSleepCallback( Battle_t* battle );
internal void Battle_EnemyCastSleepMessageCallback( Battle_t* battle );
internal void Battle_EnemyCastSleepAnimation( Battle_t* battle );
internal void Battle_EnemyFlee( Battle_t* battle );
internal void Battle_EnemyFleeCallback( Battle_t* battle );
internal void Battle_EnemyFledCallback( Battle_t* battle );
internal void Battle_MultiPauseBeforeAnimation( Battle_t* battle, u32 numPauses,
                                                AnimationId_t finalAnimationId, void ( *callback )( Battle_t* ) );
internal void Battle_DefeatedWizardDragonlordPauseCallback( Battle_t* battle );
internal void Battle_DefeatedWizardDragonlordCallback( Battle_t* battle );
internal void Battle_StartFinalDragonlordBattleCallback( Battle_t* battle );
internal void Battle_FadeFinalDragonlordBattleInCallback( Battle_t* battle );
internal void Battle_DefeatedFinalDragonlordCallback( Battle_t* battle );
internal void Battle_DefeatedFinalDragonlordPauseCallback( Battle_t* battle );
internal void Battle_CollectSphereCallback( Battle_t* battle );
internal void Battle_CollectSpherePauseCallback( Battle_t* battle );
internal void Battle_CollectSphereLightupCallback( Battle_t* battle );
internal void Battle_CollectSphereHealCallback( Battle_t* battle );

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
   u32 enemyIndex;
   u32 playerFactor, enemyFactor;
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );

   battle->isOver = False;
   battle->game->player.stats.isAsleep = False;
   battle->game->player.stats.isFizzled = False;

   switch ( battle->specialEnemy )
   {
      case SpecialEnemy_GreenDragon: enemyIndex = ENEMY_GREENDRAGON_ID; break;
      case SpecialEnemy_AxeKnight: enemyIndex = ENEMY_AXEKNIGHT_ID; break;
      case SpecialEnemy_Golem: enemyIndex = ENEMY_GOLEM_ID; break;
      case SpecialEnemy_DragonlordWizard: enemyIndex = ENEMY_DRAGONLORDWIZARD_ID; break;
      case SpecialEnemy_DragonlordDragon: enemyIndex = ENEMY_DRAGONLORDDRAGON_ID; break;
      default: enemyIndex = Battle_GenerateEnemyIndex( battle ); break;
   }

   Enemy_Load( enemy, enemyIndex );

   enemy->stats.maxHitPoints = Random_u8( enemy->minHitPoints, enemy->maxHitPoints );
   enemy->stats.hitPoints = enemy->stats.maxHitPoints;
   enemy->gold = Random_u8( enemy->minGold, enemy->maxGold );
   enemy->stats.isAsleep = False;
   enemy->stats.isFizzled = False;

   playerFactor = (u32)( player->stats.agility ) * Random_u32( 0, 255 );
   enemyFactor = (u32)( (u32)( enemy->stats.agility ) * Random_u32( 0, 255 ) * 0.25f );
   battle->turn = ( playerFactor < enemyFactor ) ? BattleTurn_Enemy : BattleTurn_Player;
}

void Battle_Attack( Battle_t* battle )
{
   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_ATTACKATTEMPT, Battle_AttackCallback, battle );
   Game_OpenDialog( battle->game );
}

void Battle_Flee( Battle_t* battle )
{
   Bool_t fled = Battle_GetFleeResult( battle );

   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_FLEEATTEMPT, fled ? Battle_FleeSucceededCallback : Battle_FleeFailedCallback, battle );
   Game_OpenDialog( battle->game );
}

void Battle_AttackSucceededCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   battle->enemy.stats.hitPoints -= Math_Min8u( battle->pendingPayload8u, battle->enemy.stats.hitPoints );
   sprintf( msg,
            battle->excellentMove ? STRING_BATTLE_ATTACKEXCELLENTMOVE : STRING_BATTLE_ATTACKSUCCEEDED,
            battle->enemy.name, battle->pendingPayload8u, ( battle->pendingPayload8u == 1 ) ? STRING_POINT : STRING_POINTS );

   if ( battle->enemy.stats.hitPoints == 0 )
   {
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyDefeatedCallback, battle );
   }
   else
   {
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_SwitchTurn, battle );
   }

   Game_OpenDialog( battle->game );
}

void Battle_SwitchTurn( Battle_t* battle )
{
   battle->turn = ( battle->turn == BattleTurn_Player ) ? BattleTurn_Enemy : BattleTurn_Player;

   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_SwitchTurnCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

void Battle_EnemyInitiative( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_EnemyInitiativeCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

Bool_t Battle_RollEnemyFlee( Battle_t* battle )
{
   if ( battle->specialEnemy != SpecialEnemy_None )
   {
      return False;
   }

   return ( ( battle->game->player.stats.strength >= ( battle->enemy.stats.strength * 2 ) ) && ( Random_u8( 1, 4 ) == 1 ) ) ? True : False;
}

void Battle_EnemyInitiativeFlee( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 2, AnimationId_Pause, Battle_EnemyFlee );
}

internal u32 Battle_GenerateEnemyIndex( Battle_t* battle )
{
   TileMap_t* tileMap = &( battle->game->tileMap );
   Player_t* player = &( battle->game->player );
   u32 enemyPoolIndex = TILE_GET_ENEMYPOOLINDEX( tileMap->tiles[player->canonicalTileIndex] );
   u32 i, enemyIndex;

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

internal u8 Battle_GetAttackDamage( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );
   u8 power, defense, damage, minDamage = 0, maxDamage = 0;

   if ( !enemy->stats.isAsleep && enemy->stats.dodge > 0 && Random_u8( 1, 64 ) <= enemy->stats.dodge )
   {
      return 0;
   }

   power = player->stats.strength + player->weapon.effect;

   if ( power < player->stats.strength ) // overflow
   {
      power = UINT8_MAX;
   }

   if ( ( battle->specialEnemy != SpecialEnemy_DragonlordWizard ) &&
        ( battle->specialEnemy != SpecialEnemy_DragonlordDragon ) &&
        ( Random_u8( 1, 32 ) == 1 ) )
   {
      battle->excellentMove = True;
      minDamage = power / 2;
      maxDamage = power;
   }
   else
   {
      battle->excellentMove = False;
      defense = enemy->stats.agility / 2;

      if ( defense < power )
      {
         minDamage = ( power - defense ) / 4;
         maxDamage = ( power - defense ) / 2;
      }
   }

   damage = Random_u8( minDamage, maxDamage );

   if ( damage == 0 )
   {
      if ( enemy->stats.isAsleep )
      {
         damage = 1;
      }
      else
      {
         damage = Random_u8( 0, 1 );
      }
   }

   return damage;
}

internal Bool_t Battle_GetFleeResult( Battle_t* battle )
{
   u32 playerFactor, enemyFactor;
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );

   if ( battle->enemy.stats.isAsleep )
   {
      return True;
   }

   playerFactor = (u32)( player->stats.agility ) * Random_u32( 0, 255 );
   enemyFactor = (u32)( (u32)( enemy->stats.agility ) * Random_u32( 0, 255 ) * Enemy_GetFleeFactor( enemy ) );

   return ( playerFactor < enemyFactor ) ? False : True;
}

internal void Battle_FleeSucceededCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 2, AnimationId_Battle_EnemyFadeOut, Battle_FleeSucceededMessageCallback );
}

internal void Battle_FleeSucceededMessageCallback( Battle_t* battle )
{
   char msg[64];

   battle->isOver = True;
   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_FLEEATTEMPTSUCCEEDED, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_FleeSucceededSpecialEnemyCheck, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_FleeSucceededSpecialEnemyCheck( Battle_t* battle )
{
   if ( ( battle->specialEnemy != SpecialEnemy_None ) &&
        ( battle->specialEnemy != SpecialEnemy_DragonlordWizard ) &&
        ( battle->specialEnemy != SpecialEnemy_DragonlordDragon ) )
   {
      switch ( battle->specialEnemy )
      {
         case SpecialEnemy_GreenDragon:
            battle->game->player.tileIndex = TILEMAP_GREENDRAGON_TILEINDEX - battle->game->tileMap.tilesX;
            break;
         case SpecialEnemy_AxeKnight:
            battle->game->player.tileIndex = TILEMAP_AXEKNIGHT_TILEINDEX - 1;
            break;
         case SpecialEnemy_Golem:
            battle->game->player.tileIndex = TILEMAP_GOLEM_TILEINDEX - battle->game->tileMap.tilesX;
            break;
      }

      Player_SetCanonicalTileIndex( &( battle->game->player ) );
      Player_CenterOnTile( &( battle->game->player ) );
   }
}

internal void Battle_FleeFailedCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 1, AnimationId_Pause, Battle_FleeFailedMessageCallback );
}

internal void Battle_FleeFailedMessageCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_FLEEATTEMPTFAILED, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_SwitchTurn, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_AttackCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );

   if ( battle->enemy.id == ENEMY_DRAGONLORDDRAGON_ID && battle->game->player.weapon.id != WEAPON_ERDRICKSSWORD_ID )
   {
      AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_AttackIneffectiveCallback, battle );
   }
   else
   {
      battle->pendingPayload8u = Battle_GetAttackDamage( battle );

      if ( battle->pendingPayload8u == 0 )
      {
         AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_AttackDodgedCallback, battle );
      }
      else
      {
         AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Battle_EnemyDamage, Battle_AttackSucceededCallback, battle );
      }
   }

   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_AttackDodgedCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ATTACKDODGED, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_SwitchTurn, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_AttackIneffectiveCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ATTACKINEFFECTIVE, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_SwitchTurn, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyDefeatedCallback( Battle_t* battle )
{
   AnimationId_t id = ( battle->specialEnemy == SpecialEnemy_DragonlordDragon ) ? AnimationId_Battle_EnemySlowFadeOut : AnimationId_Battle_EnemyFadeOut;
   Battle_MultiPauseBeforeAnimation( battle, 2, id, Battle_EnemyDefeatedMessageCallback );
}

internal void Battle_EnemyDefeatedMessageCallback( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   Enemy_t* enemy = &( battle->enemy );
   Dialog_t* dialog = &( battle->game->dialog );
   u16 i, learnedSpell = 0;
   char msg[96];
      
   Dialog_Reset( dialog );
   sprintf( msg, STRING_BATTLE_VICTORY, battle->enemy.name );
   battle->isOver = True;

   if ( battle->enemy.id == ENEMY_DRAGONLORDWIZARD_ID )
   {
      Dialog_PushSectionWithCallback( dialog, msg, Battle_DefeatedWizardDragonlordPauseCallback, battle );
   }
   else if ( battle->enemy.id == ENEMY_DRAGONLORDDRAGON_ID )
   {
      Dialog_PushSectionWithCallback( dialog, msg, Battle_DefeatedFinalDragonlordCallback, battle );
   }
   else
   {
      Dialog_PushSection( dialog, msg );
      battle->experienceGained = Math_AmountToCollect16u( player->experience, enemy->experience );
      battle->goldGained = Math_AmountToCollect16u( player->gold, enemy->gold );
      battle->newLevel = Player_GetLevelFromExperience( player->experience + battle->experienceGained );
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

         Dialog_PushSectionWithCallback( dialog, msg, Battle_EnemyDefeatedSpoilsCallback, battle );
      }

      if ( battle->newLevel > player->level )
      {
         Dialog_PushSectionWithCallback( dialog, STRING_BATTLE_LEVELUP, Battle_NewLevelCallback, battle );

         battle->strengthGained = Player_GetStrengthFromLevel( player, battle->newLevel ) - player->stats.strength;
         battle->agilityGained = Player_GetAgilityFromLevel( player, battle->newLevel ) - player->stats.agility;
         battle->hitPointsGained = Player_GetMaxHitPointsFromLevel( player, battle->newLevel ) - player->stats.maxHitPoints;
         battle->magicPointsGained = Player_GetMaxMagicPointsFromLevel( player, battle->newLevel ) - player->stats.maxMagicPoints;
         Player_UpdateSpellsToLevel( &( battle->game->player ), battle->newLevel );

         player->stats.strength += battle->strengthGained;
         player->stats.agility += battle->agilityGained;

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

            Dialog_PushSectionWithCallback( dialog, msg, Battle_GainedPointsCallback, battle );
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

      if ( battle->specialEnemy != SpecialEnemy_None )
      {
         battle->game->gameFlags.specialEnemies ^= ( 0x1 << battle->specialEnemy );
      }
   }

   Game_OpenDialog( battle->game );
}

internal void Battle_NewLevelCallback( Battle_t* battle )
{
   battle->game->player.level = battle->newLevel;
   Game_DrawQuickStatus( battle->game );
}

internal void Battle_EnemyDefeatedSpoilsCallback( Battle_t* battle )
{
   battle->game->player.experience += battle->experienceGained;
   battle->game->player.gold += battle->goldGained;
}

internal void Battle_GainedPointsCallback( Battle_t* battle )
{
   battle->game->player.stats.maxHitPoints += battle->hitPointsGained;
   battle->game->player.stats.maxMagicPoints += battle->magicPointsGained;

   Game_DrawQuickStatus( battle->game );
}

internal void Battle_SwitchTurnCallback( Battle_t* battle )
{
   if ( battle->turn == BattleTurn_Player )
   {
      if ( battle->game->player.stats.isAsleep )
      {
         Dialog_Reset( &( battle->game->dialog ) );

         if ( Random_u8( 1, 2 ) == 1 )
         {
            battle->game->player.stats.isAsleep = False;
            Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_PLAYERWOKEUP, Game_ResetBattleMenu, battle->game );
         }
         else
         {
            Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_PLAYERSTILLASLEEP, Battle_SwitchTurn, battle );
         }

         Game_OpenDialog( battle->game );
      }
      else
      {
         Dialog_Reset( &( battle->game->dialog ) );
         Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_COMMAND, Game_ResetBattleMenu, battle->game );
         Game_OpenDialog( battle->game );
      }
   }
   else
   {
      Battle_EnemyTurn( battle );
   }
}

internal void Battle_EnemyInitiativeCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMYINITIATIVE, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyInitiativeMessageCallback, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyInitiativeMessageCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 2, AnimationId_Pause, Battle_EnemyTurn );
}

internal void Battle_EnemyTurn( Battle_t* battle )
{
   char msg[64];

   if ( battle->enemy.stats.isAsleep )
   {
      if ( Random_u8( 1, 3 ) == 1 )
      {
         battle->enemy.stats.isAsleep = False;
         Dialog_Reset( &( battle->game->dialog ) );
         sprintf( msg, STRING_BATTLE_ENEMYWOKEUP, battle->enemy.name );
         Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyWokeUpCallback, battle );
         Game_OpenDialog( battle->game );
      }
      else
      {
         battle->turn = BattleTurn_Player;
         Dialog_Reset( &( battle->game->dialog ) );
         sprintf( msg, STRING_BATTLE_ENEMYSTILLASLEEP, battle->enemy.name );
         Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Game_ResetBattleMenu, battle->game );
         Game_OpenDialog( battle->game );
      }
   }
   else
   {
      if ( Battle_RollEnemyFlee( battle ) )
      {
         Battle_EnemyFlee( battle );
      }
      else
      {
         Battle_EnemyInitiateBehavior( battle );
      }
   }
}

internal void Battle_EnemyWokeUpCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 1, AnimationId_Pause, Battle_EnemyInitiateBehavior );
}

internal void Battle_EnemyInitiateBehavior( Battle_t* battle )
{
   switch ( battle->enemy.id )
   {
      case ENEMY_MAGICIAN_ID:
      case ENEMY_MAGIDRAKEE_ID: Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizz, 50 ); break;
      case ENEMY_POLTERGEIST_ID:
      case ENEMY_METALSLIME_ID: Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizz, 75 ); break;
      case ENEMY_DRAKEEMA_ID: if ( !Battle_EnemyTryHeal( battle, False, 25 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizz, 50 ); break;
      case ENEMY_WARLOCK_ID: if ( !Battle_EnemyTrySleep( battle, 25 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizz, 50 ); break;
      case ENEMY_WRAITH_ID: if ( !Battle_EnemyTryHeal( battle, False, 25 ) ) Battle_EnemyAttack( battle ); break;
      case ENEMY_SPECTER_ID: if ( !Battle_EnemyTrySleep( battle, 25 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizz, 75 ); break;
      case ENEMY_WOLFLORD_ID:
      case ENEMY_DROLLMAGI_ID:
      case ENEMY_KNIGHT_ID: if ( !Battle_EnemyTryFizzle( battle, 50 ) ) Battle_EnemyAttack( battle ); break;
      case ENEMY_DRUINLORD_ID: if ( !Battle_EnemyTryHeal( battle, False, 75 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizz, 25 ); break;
      case ENEMY_WRAITHKNIGHT_ID: if ( !Battle_EnemyTryHeal( battle, False, 75 ) ) Battle_EnemyAttack( battle ); break;
      case ENEMY_MAGIWYVERN_ID: if ( !Battle_EnemyTrySleep( battle, 50 ) ) Battle_EnemyAttack( battle ); break;
      case ENEMY_GREENDRAGON_ID:
      case ENEMY_BLUEDRAGON_ID: Battle_EnemyActionOrAttack( battle, Battle_EnemyBreatheFire, 25 ); break;
      case ENEMY_STARWYVERN_ID: if ( !Battle_EnemyTryHeal( battle, True, 75 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyBreatheFire, 25 ); break;
      case ENEMY_WIZARD_ID: Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizzle, 50 ); break;
      case ENEMY_AXEKNIGHT_ID: if ( !Battle_EnemyTrySleep( battle, 25 ) ) Battle_EnemyAttack( battle ); break;
      case ENEMY_ARMOREDKNIGHT_ID: if ( !Battle_EnemyTryHeal( battle, True, 75 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizzle, 25 ); break;
      case ENEMY_REDDRAGON_ID: if ( !Battle_EnemyTrySleep( battle, 25 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyBreatheFire, 25 ); break;
      case ENEMY_DRAGONLORDWIZARD_ID: if ( !Battle_EnemyTryFizzle( battle, 25 ) ) Battle_EnemyActionOrAttack( battle, Battle_EnemyCastSizzle, 75 ); break;
      case ENEMY_DRAGONLORDDRAGON_ID: Battle_EnemyActionOrAttack( battle, Battle_EnemyBreatheStrongFire, 50 ); break;
      default: Battle_EnemyAttack( battle );
   }
}

internal void Battle_EnemyActionOrAttack( Battle_t* battle, void ( *action )( Battle_t* ), u32 chance )
{
   if ( Random_Percent() <= chance )
   {
      action( battle );
   }
   else
   {
      Battle_EnemyAttack( battle );
   }
}

internal Bool_t Battle_EnemyTryHeal( Battle_t* battle, Bool_t midHeal, u32 chance )
{
   if ( battle->enemy.stats.hitPoints < ( battle->enemy.stats.maxHitPoints / 4 ) && Random_Percent() <= chance )
   {
      if ( midHeal )
      {
         Battle_EnemyCastMidheal( battle );
      }
      else
      {
         Battle_EnemyCastHeal( battle );
      }

      return True;
   }

   return False;
}

internal Bool_t Battle_EnemyTryFizzle( Battle_t* battle, u32 chance )
{
   if ( !battle->game->player.stats.isFizzled && Random_Percent() <= chance )
   {
      Battle_EnemyCastFizzle( battle );
      return True;
   }

   return False;
}

internal Bool_t Battle_EnemyTrySleep( Battle_t* battle, u32 chance )
{
   if ( !battle->game->player.stats.isAsleep && Random_Percent() <= chance )
   {
      Battle_EnemyCastSleep( battle );
      return True;
   }

   return False;
}

internal void Battle_EnemyAttack( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMY_ATTACKATTEMPT, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyAttackCallback, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_PushPlayerHurtAnimation( Battle_t* battle )
{
   AnimationChain_PushAnimation( &( battle->game->animationChain ),
                                 battle->game->player.stats.hitPoints > battle->pendingPayload8u
                                    ? AnimationId_Battle_PlayerDamage
                                    : AnimationId_Battle_PlayerDeath );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_EnemyAttackSucceededCallback, battle );
}

internal void Battle_EnemyAttackCallback( Battle_t* battle )
{
   battle->pendingPayload8u = Battle_GetEnemyAttackDamage( battle );
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );

   if ( battle->pendingPayload8u == 0 )
   {
      AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_EnemyAttackDodgedCallback, battle );
   }
   else
   {
      Battle_PushPlayerHurtAnimation( battle );
   }

   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyAttackDodgedCallback( Battle_t* battle )
{
   battle->turn = BattleTurn_Player;
   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_ENEMY_ATTACKDODGED, Game_ResetBattleMenu, battle->game );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyAttackSucceededCallback( Battle_t* battle )
{
   Player_t* player = &( battle->game->player );
   char msg[64];

   player->stats.hitPoints -= Math_Min8u( battle->pendingPayload8u, player->stats.hitPoints );
   Dialog_Reset( &( battle->game->dialog ) );

   if ( player->stats.hitPoints == 0 )
   {
      sprintf( msg, STRING_BATTLE_ENEMY_ATTACKDEATH, battle->pendingPayload8u, ( battle->pendingPayload8u == 1 ) ? STRING_POINT : STRING_POINTS );
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_PlayerDefeatedCallback, battle );
   }
   else if ( player->stats.isAsleep )
   {
      sprintf( msg, STRING_BATTLE_ENEMY_ATTACKSUCCEEDEDASLEEP, battle->pendingPayload8u, ( battle->pendingPayload8u == 1 ) ? STRING_POINT : STRING_POINTS );
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_SwitchTurn, battle );
   }
   else
   {
      battle->turn = BattleTurn_Player;
      sprintf( msg, STRING_BATTLE_ENEMY_ATTACKSUCCEEDED, battle->pendingPayload8u, ( battle->pendingPayload8u == 1 ) ? STRING_POINT : STRING_POINTS );
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Game_ResetBattleMenu, battle->game );
   }

   Game_OpenDialog( battle->game );
}

internal void Battle_PlayerDefeatedCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 4, AnimationId_Pause, Battle_PlayerDefeatedCallbackMessage );
}

internal void Battle_PlayerDefeatedCallbackMessage( Battle_t* battle )
{
   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_DIALOG_DEAD, Battle_PlayerDefeatedDialogCallback, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_PlayerDefeatedDialogCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 12, AnimationId_Pause, Battle_PlayerDefeatedPostDialogCallback );
}

internal void Battle_PlayerDefeatedPostDialogCallback( Battle_t* battle )
{
   Game_HandleDeath( battle->game );
}

internal u8 Battle_GetEnemyAttackDamage( Battle_t* battle )
{
   u8 defense, damage, minDamage = 0, maxDamage = 0;
   Enemy_t* enemy = &( battle->enemy );
   Player_t* player = &( battle->game->player );

   defense = ( player->stats.agility / 2 ) + player->armor.effect + player->shield.effect;

   if ( ITEM_HAS_DRAGONSCALE( player->items ) )
   {
      defense += 2;
   }

   if ( defense < ( player->stats.agility / 2 ) ) // overflow
   {
      defense = UINT8_MAX;
   }

   if ( enemy->stats.strength > defense )
   {
      minDamage = ( enemy->stats.strength - defense ) / 4;
      maxDamage = ( enemy->stats.strength - defense ) / 2;
   }
   else
   {
      maxDamage = ( enemy->stats.strength + 4 ) / 6;
   }

   damage = Random_u8( minDamage, maxDamage );

   if ( damage == 0 )
   {
      if ( player->stats.isAsleep )
      {
         damage = 1;
      }
      else
      {
         damage = Random_u8( 0, 1 );
      }
   }

   return damage;
}

internal void Battle_EnemyBreatheFire( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMY_BREATHEFIRE, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyBreatheFireCallback, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyBreatheFireCallback( Battle_t* battle )
{
   battle->pendingPayload8u = Random_u8( FIRE_BREATH_MIN_DAMAGE, FIRE_BREATH_MAX_DAMAGE );

   if ( battle->game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
   {
      battle->pendingPayload8u /= 3;
      battle->pendingPayload8u *= 2;
   }

   AnimationChain_Reset( &( battle->game->animationChain ) );
   Battle_PushPlayerHurtAnimation( battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyBreatheStrongFire( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMY_BREATHEFIRE, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyBreatheStrongFireCallback, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyBreatheStrongFireCallback( Battle_t* battle )
{
   battle->pendingPayload8u = Random_u8( STRONG_FIRE_BREATH_MIN_DAMAGE, STRONG_FIRE_BREATH_MAX_DAMAGE );

   if ( battle->game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
   {
      battle->pendingPayload8u /= 3;
      battle->pendingPayload8u *= 2;
   }

   AnimationChain_Reset( &( battle->game->animationChain ) );
   Battle_PushPlayerHurtAnimation( battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyCastSpell( Battle_t* battle, const char* spellName, void ( *callback )( Battle_t* ) )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMY_SPELLCAST, battle->enemy.name, spellName );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, ( battle->enemy.stats.isFizzled ) ? Battle_EnemySpellFizzledCallback : callback, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemySpellFizzledCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_CastSpell );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_EnemySpellFizzledMessageCallback, battle );
   Battle_EnemyAnimateSpellWithCallback( battle, Battle_EnemySpellFizzledMessageCallback );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemySpellFizzledMessageCallback( Battle_t* battle )
{
   battle->turn = BattleTurn_Player;

   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_SPELLFIZZLEDCOMMAND, Game_ResetBattleMenu, battle->game );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyCastSizz( Battle_t* battle )
{
   Battle_EnemyCastSpell( battle, STRING_SPELL_SIZZ, Battle_EnemyCastSizzCallback );
}

internal void Battle_EnemyCastSizzCallback( Battle_t* battle )
{
   u8 minDamage = ( battle->game->player.armor.id == ARMOR_MAGICARMOR_ID || battle->game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
                          ? ENEMY_SIZZ_REDUCEDMIN_DAMAGE : ENEMY_SIZZ_MIN_DAMAGE;
   u8 maxDamage = ( battle->game->player.armor.id == ARMOR_MAGICARMOR_ID || battle->game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
                          ? ENEMY_SIZZ_REDUCEDMAX_DAMAGE : ENEMY_SIZZ_MAX_DAMAGE;
   battle->pendingPayload8u = Random_u8( minDamage, maxDamage );

   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_CastSpell );
   Battle_PushPlayerHurtAnimation( battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyCastSizzle( Battle_t* battle )
{
   Battle_EnemyCastSpell( battle, STRING_SPELL_SIZZLE, Battle_EnemyCastSizzleCallback );
}

internal void Battle_EnemyCastSizzleCallback( Battle_t* battle )
{
   u8 minDamage = ( battle->game->player.armor.id == ARMOR_MAGICARMOR_ID || battle->game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
                          ? ENEMY_SIZZLE_REDUCEDMIN_DAMAGE : ENEMY_SIZZLE_MIN_DAMAGE;
   u8 maxDamage = ( battle->game->player.armor.id == ARMOR_MAGICARMOR_ID || battle->game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
                          ? ENEMY_SIZZLE_REDUCEDMAX_DAMAGE : ENEMY_SIZZLE_MAX_DAMAGE;
   battle->pendingPayload8u = Random_u8( minDamage, maxDamage );

   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_CastSpell );
   Battle_PushPlayerHurtAnimation( battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyAnimateSpellWithCallback( Battle_t* battle, void ( *callback )( Battle_t* battle ) )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_CastSpell );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, callback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyCastHeal( Battle_t* battle )
{
   Battle_EnemyCastSpell( battle, STRING_SPELL_HEAL, Battle_EnemyCastHealCallback );
}

internal void Battle_EnemyCastHealCallback( Battle_t* battle )
{
   u8 payload = Math_Min8u( Random_u8( ENEMY_HEAL_MIN_RECOVERY, ENEMY_HEAL_MAX_RECOVERY ), battle->enemy.stats.maxHitPoints - battle->enemy.stats.hitPoints );
   battle->enemy.stats.hitPoints += payload;
   Battle_EnemyAnimateSpellWithCallback( battle, Battle_EnemyHealMessageCallback );
}

internal void Battle_EnemyHealMessageCallback( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );

   if ( battle->game->player.stats.isAsleep )
   {
      sprintf( msg, STRING_BATTLE_ENEMY_RECOVEREDASLEEP, battle->enemy.name );
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_SwitchTurn, battle );
   }
   else
   {
      battle->turn = BattleTurn_Player;
      sprintf( msg, STRING_BATTLE_ENEMY_RECOVERED, battle->enemy.name );
      Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Game_ResetBattleMenu, battle->game );
   }

   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyCastMidheal( Battle_t* battle )
{
   Battle_EnemyCastSpell( battle, STRING_SPELL_MIDHEAL, Battle_EnemyCastMidhealCallback );
}

internal void Battle_EnemyCastMidhealCallback( Battle_t* battle )
{
   u8 payload = Math_Min8u( Random_u8( ENEMY_MIDHEAL_MIN_RECOVERY, ENEMY_MIDHEAL_MAX_RECOVERY ), battle->enemy.stats.maxHitPoints - battle->enemy.stats.hitPoints );
   battle->enemy.stats.hitPoints += payload;
   Battle_EnemyAnimateSpellWithCallback( battle, Battle_EnemyHealMessageCallback );
}

internal void Battle_EnemyCastFizzle( Battle_t* battle )
{
   Battle_EnemyCastSpell( battle, STRING_SPELL_FIZZLE, Battle_EnemyCastFizzleCallback );
}

internal void Battle_EnemyCastFizzleCallback( Battle_t* battle )
{
   if ( battle->game->player.armor.id != ARMOR_ERDRICKSARMOR_ID )
   {
      battle->game->player.stats.isFizzled = ( Random_u8( 1, 2 ) == 1 ) ? True : False;
   }

   Battle_EnemyAnimateSpellWithCallback( battle, Battle_EnemyCastFizzleMessageCallback );
}

internal void Battle_EnemyCastFizzleMessageCallback( Battle_t* battle )
{
   Dialog_Reset( &( battle->game->dialog ) );

   if ( battle->game->player.stats.isAsleep )
   {
      Dialog_PushSectionWithCallback( &( battle->game->dialog ),
                                      battle->game->player.stats.isFizzled ? STRING_BATTLE_PLAYER_FIZZLEDASLEEP : STRING_BATTLE_PLAYER_SPELL_NOEFFECTASLEEP,
                                      Battle_SwitchTurn, battle );
   }
   else
   {
      battle->turn = BattleTurn_Player;
      Dialog_PushSectionWithCallback( &( battle->game->dialog ),
                                      battle->game->player.stats.isFizzled ? STRING_BATTLE_PLAYER_FIZZLED : STRING_BATTLE_PLAYER_SPELL_NOEFFECT,
                                      Game_ResetBattleMenu, battle->game );
   }

   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyCastSleep( Battle_t* battle )
{
   Battle_EnemyCastSpell( battle, STRING_SPELL_SLEEP, Battle_EnemyCastSleepCallback );
}

internal void Battle_EnemyCastSleepCallback( Battle_t* battle )
{
   battle->game->player.stats.isAsleep = True;
   Battle_EnemyAnimateSpellWithCallback( battle, Battle_EnemyCastSleepMessageCallback );
}

internal void Battle_EnemyCastSleepMessageCallback( Battle_t* battle )
{
   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_BATTLE_PLAYER_ASLEEP, Battle_EnemyCastSleepAnimation, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyCastSleepAnimation( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 1, AnimationId_Pause, Battle_EnemyTurn );
}

internal void Battle_EnemyFlee( Battle_t* battle )
{
   char msg[64];

   Dialog_Reset( &( battle->game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMY_FLEE, battle->enemy.name );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), msg, Battle_EnemyFleeCallback, battle );
   Game_OpenDialog( battle->game );
}

internal void Battle_EnemyFleeCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Battle_EnemyFadeOut, Battle_EnemyFledCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_EnemyFledCallback( Battle_t* battle )
{
   battle->isOver = True;
}

internal void Battle_MultiPauseBeforeAnimation( Battle_t* battle, u32 numPauses,
                                                AnimationId_t finalAnimationId, void ( *callback )( Battle_t* ) )
{
   u32 i;

   AnimationChain_Reset( &( battle->game->animationChain ) );

   for ( i = 0; i < numPauses; i++ )
   {
      AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), finalAnimationId, callback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_DefeatedWizardDragonlordPauseCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 7, AnimationId_Pause, Battle_DefeatedWizardDragonlordCallback );
}

internal void Battle_DefeatedWizardDragonlordCallback( Battle_t* battle )
{
   Game_ChangeToOverworldState( battle->game );
   Battle_MultiPauseBeforeAnimation( battle, 5, AnimationId_Pause, Battle_StartFinalDragonlordBattleCallback );
}

internal void Battle_StartFinalDragonlordBattleCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_MidFadeOut );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_FadeFinalDragonlordBattleInCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_FadeFinalDragonlordBattleInCallback( Battle_t* battle )
{
   Screen_RestorePalette( &( battle->game->screen ) );
   Screen_WipeColor( &( battle->game->screen ), COLOR_BLACK );

   battle->specialEnemy = SpecialEnemy_DragonlordDragon;
   Battle_Generate( battle );
   Game_ChangeToBattleState( battle->game );
}

internal void Battle_DefeatedFinalDragonlordCallback( Battle_t* battle )
{
   Battle_MultiPauseBeforeAnimation( battle, 15, AnimationId_Pause, Battle_DefeatedFinalDragonlordPauseCallback );
}

internal void Battle_DefeatedFinalDragonlordPauseCallback( Battle_t* battle )
{
   battle->game->gameFlags.defeatedDragonlord = True;
   battle->game->tileMap.npcCount = 0;

   Game_ChangeToOverworldState( battle->game );
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_MidFadeIn );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( battle->game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_Pause, Battle_CollectSpherePauseCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_CollectSphereCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   Battle_MultiPauseBeforeAnimation( battle, 4, AnimationId_Pause, Battle_CollectSpherePauseCallback );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_CollectSpherePauseCallback( Battle_t* battle )
{
   if ( !ITEM_HAS_SPHEREOFLIGHT( battle->game->player.items ) )
   {
      ITEM_TOGGLE_HASSPHEREOFLIGHT( battle->game->player.items );
   }

   if ( ITEM_HAS_CURSEDBELT( battle->game->player.items ) )
   {
      ITEM_TOGGLE_HASCURSEDBELT( battle->game->player.items );
   }

   battle->game->gameFlags.treasures ^= 0x10000000;

   Dialog_Reset( &( battle->game->dialog ) );
   Dialog_PushSection( &( battle->game->dialog ), STRING_DIALOG_FOUNDSPHEREOFLIGHT_1 );
   Dialog_PushSectionWithCallback( &( battle->game->dialog ), STRING_DIALOG_FOUNDSPHEREOFLIGHT_2, Battle_CollectSphereLightupCallback, battle );
   Dialog_PushSection( &( battle->game->dialog ), STRING_DIALOG_FOUNDSPHEREOFLIGHT_3 );
   Game_OpenDialog( battle->game );
}

internal void Battle_CollectSphereLightupCallback( Battle_t* battle )
{
   AnimationChain_Reset( &( battle->game->animationChain ) );
   AnimationChain_PushAnimationWithCallback( &( battle->game->animationChain ), AnimationId_CastSpell, Battle_CollectSphereHealCallback, battle );
   AnimationChain_Start( &( battle->game->animationChain ) );
}

internal void Battle_CollectSphereHealCallback( Battle_t* battle )
{
   battle->game->player.isCursed = False;
   battle->game->player.stats.hitPoints = battle->game->player.stats.maxHitPoints;
   battle->game->player.stats.magicPoints = battle->game->player.stats.maxMagicPoints;
}
