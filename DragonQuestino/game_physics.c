#include "game.h"
#include "random.h"

internal void Game_UpdatePlayerTileIndex( Game_t* game );
internal void Game_RollEncounter( Game_t* game, uint32_t tileIndex );
internal SpecialEnemy_t Game_GetSpecialEnemyFromPlayerLocation( Game_t* game );

void Game_TicPhysics( Game_t* game )
{
   Vector2f_t newPos;
   uint32_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile, tileIndex;
   Player_t* player = &( game->player );

   if ( player->velocity.x == 0.0f && player->velocity.y == 0.0f )
   {
      return;
   }

   newPos.x = player->sprite.position.x + ( player->velocity.x * CLOCK_FRAME_SECONDS );
   newPos.y = player->sprite.position.y + ( player->velocity.y * CLOCK_FRAME_SECONDS );

   if ( newPos.x < 0 )
   {
      newPos.x = COLLISION_THETA;
   }
   else if ( ( newPos.x + player->hitBoxSize.x ) >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      newPos.x = ( game->tileMap.tilesX * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_THETA;
   }

   if ( newPos.y < 0 )
   {
      newPos.y = COLLISION_THETA;
   }
   else if ( ( newPos.y + player->hitBoxSize.y ) >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      newPos.y = ( game->tileMap.tilesY * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_THETA;
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.noClip == False ) {
#endif

      // clip to unpassable horizontal tiles
      if ( newPos.x != player->sprite.position.x )
      {
         tileRowStartIndex = (uint32_t)( player->sprite.position.y / TILE_SIZE );
         tileRowEndIndex = (uint32_t)( ( player->sprite.position.y + player->hitBoxSize.y ) / TILE_SIZE );

         if ( newPos.x < player->sprite.position.x )
         {
            // moving left, check leftward tiles
            col = (uint32_t)( newPos.x / TILE_SIZE );

            for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetDoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->gameFlags.doors ) )
               {
                  newPos.x = (float)( ( ( col + 1 ) * TILE_SIZE ) );
                  break;
               }
            }
         }
         else
         {
            // moving right, check rightward tiles
            col = (uint32_t )( ( newPos.x + player->hitBoxSize.x ) / TILE_SIZE );

            for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetDoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->gameFlags.doors ) )
               {
                  newPos.x = ( col * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_THETA;
                  break;
               }
            }
         }
      }

      // clip to unpassable vertical tiles
      if ( newPos.y != player->sprite.position.y )
      {
         tileColStartIndex = ( uint32_t )( player->sprite.position.x / TILE_SIZE );
         tileColEndIndex = (uint32_t)( ( player->sprite.position.x + player->hitBoxSize.x ) / TILE_SIZE );

         if ( newPos.y < player->sprite.position.y )
         {
            // moving up, check upward tiles
            row = (uint32_t)( newPos.y / TILE_SIZE );

            for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetDoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->gameFlags.doors ) )
               {
                  newPos.y = (float)( ( ( row + 1 ) * TILE_SIZE ) );
                  break;
               }
            }
         }
         else
         {
            // moving down, check downward tiles
            row = (uint32_t)( ( newPos.y + player->hitBoxSize.y ) / TILE_SIZE );

            for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetDoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->gameFlags.doors ) )
               {
                  newPos.y = ( row * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_THETA;
                  break;
               }
            }
         }
      }

#if defined( VISUAL_STUDIO_DEV )
   }
#endif

   player->sprite.position = newPos;
   player->velocity.x = 0;
   player->velocity.y = 0;

   Game_UpdatePlayerTileIndex( game );
}

void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex )
{
   TilePortal_t* portal;

   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), tileIndex );
   game->player.tileIndex = tileIndex;
   portal = TileMap_GetPortalForTileIndex( &( game->tileMap ), tileIndex );

   if ( portal )
   {
      AnimationChain_Reset( &( game->animationChain ) );
      Game_AnimatePortalEntrance( game, portal );
      AnimationChain_Start( &( game->animationChain ) );
      return;
   }

   if ( game->tileMap.isDark && game->tileMap.glowDiameter > 1 )
   {
      game->tileMap.glowTileCount++;

      if ( ( game->tileMap.glowDiameter == 7 && game->tileMap.glowTileCount > GLOW_THREERADIUS_TILES ) ||
           ( game->tileMap.glowDiameter == 5 && game->tileMap.glowTileCount > GLOW_TWORADIUS_TILES ) ||
           ( game->tileMap.glowDiameter == 3 && game->tileMap.glowTileCount > GLOW_ONERADIUS_TILES ) )
      {
         game->tileMap.glowTileCount = 0;
         TileMap_ReduceTargetGlowDiameter( &( game->tileMap ) );
      }
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.noEncounters )
   {
      return;
   }
#endif

   game->battle.specialEnemy = Game_GetSpecialEnemyFromPlayerLocation( game );

   if ( game->battle.specialEnemy != SpecialEnemy_None )
   {
      Battle_Generate( &( game->battle ) );
      Game_ChangeToBattleState( game );
   }
   else if ( game->tileMap.hasEncounters )
   {
      if ( !( game->tileMap.isDungeon ) && game->player.hasHolyProtection )
      {
         game->player.holyProtectionSteps++;
      }

      Game_RollEncounter( game, tileIndex );
   }
}

internal void Game_UpdatePlayerTileIndex( Game_t* game )
{
   uint32_t centerX = (uint32_t)( game->player.sprite.position.x + ( game->player.hitBoxSize.x / 2 ) );
   uint32_t centerY = (uint32_t)( game->player.sprite.position.y + ( game->player.hitBoxSize.y / 2 ) );
   uint32_t newTileIndex = ( ( centerY / TILE_SIZE ) * game->tileMap.tilesX ) + ( centerX / TILE_SIZE );

   if ( newTileIndex != game->player.tileIndex )
   {
      game->player.tileIndex = newTileIndex;
      Game_PlayerSteppedOnTile( game, newTileIndex );
   }
}

internal void Game_RollEncounter( Game_t* game, uint32_t tileIndex )
{
   uint32_t encounterRate = TILE_GET_ENCOUNTERRATE( game->tileMap.tiles[tileIndex] );
   Bool_t spawnEncounter = False;

   switch( encounterRate )
   {
      case 0: spawnEncounter = Random_Percent() <= ENCOUNTERRATE_LOW; break;
      case 1: spawnEncounter = Random_Percent() <= ENCOUNTERRATE_MEDIUM; break;
      case 2: spawnEncounter = Random_Percent() <= ENCOUNTERRATE_HIGH; break;
      case 3: spawnEncounter = Random_Percent() <= ENCOUNTERRATE_EXTREME; break;
      default: return;
   }

   if ( spawnEncounter )
   {
      Battle_Generate( &( game->battle ) );

      if ( game->tileMap.isDungeon || !( game->player.hasHolyProtection ) || game->battle.enemy.stats.strength > ( game->player.stats.agility / 2 ) )
      {
         Game_ChangeToBattleState( game );
      }
   }
   else
   {
      if ( game->player.hasHolyProtection && game->player.holyProtectionSteps >= HOLY_PROTECTION_MAX_STEPS )
      {
         game->player.hasHolyProtection = False;
         Dialog_Reset( &( game->dialog ) );
         Dialog_PushSection( &( game->dialog ), STRING_HOLYPROTECTION_OFF );
         Game_OpenDialog( game );
      }
   }
}

internal SpecialEnemy_t Game_GetSpecialEnemyFromPlayerLocation( Game_t* game )
{
   uint32_t tileMapId = game->tileMap.id;
   uint32_t tileIndex = game->player.tileIndex;

   if ( tileMapId == TILEMAP_GREENDRAGON_MAPID && tileIndex == TILEMAP_GREENDRAGON_TILEINDEX )
   {
      if ( game->gameFlags.specialEnemies & ( 0x1 << SpecialEnemy_GreenDragon ) )
      {
         return SpecialEnemy_GreenDragon;
      }
   }
   else if ( tileMapId == TILEMAP_AXEKNIGHT_MAPID && tileIndex == TILEMAP_AXEKNIGHT_TILEINDEX )
   {
      if ( game->gameFlags.specialEnemies & ( 0x1 << SpecialEnemy_AxeKnight ) )
      {
         return SpecialEnemy_AxeKnight;
      }
   }
   else if ( tileMapId == TILEMAP_GOLEM_MAPID && tileIndex == TILEMAP_GOLEM_TILEINDEX )
   {
      if ( game->gameFlags.specialEnemies & ( 0x1 << SpecialEnemy_Golem ) )
      {
         return SpecialEnemy_Golem;
      }
   }

   return SpecialEnemy_None;
}
