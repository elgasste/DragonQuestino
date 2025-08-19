#include "game.h"
#include "random.h"
#include "math.h"

internal void Game_ClipSprites( ActiveSprite_t* mainSprite, ActiveSprite_t* clipSprite, Vector2f_t prevPos, Vector2f_t* newPos );
internal uint32_t Game_GetSpriteTileIndex( Game_t* game, ActiveSprite_t* sprite );
internal void Game_UpdatePlayerTileIndex( Game_t* game );
internal void Game_RollEncounter( Game_t* game );
internal SpecialEnemy_t Game_GetSpecialEnemyFromPlayerLocation( Game_t* game );
internal void Game_ApplyTileDamage( Game_t* game );
internal void Game_TileDamageDeathCallback( Game_t* game );
internal void Game_TileDamageDeathMessageCallback( Game_t* game );
internal void Game_MoveNpcs( Game_t* game );

void Game_TicPhysics( Game_t* game )
{
   uint32_t i;
   Vector2f_t prevPos, newPos;
   uint32_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile, tileIndex;
   Player_t* player = &( game->player );

   if ( player->velocity.x == 0.0f && player->velocity.y == 0.0f )
   {
      ActiveSprite_StopFlickering( &( game->player.sprite ) );
      Game_MoveNpcs( game );
      return;
   }

   prevPos = player->sprite.position;
   newPos.x = player->sprite.position.x + ( player->velocity.x * CLOCK_FRAME_SECONDS );
   newPos.y = player->sprite.position.y + ( player->velocity.y * CLOCK_FRAME_SECONDS );

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.noClip == False ) {
#endif

      for ( i = 0; i < game->tileMap.npcCount; i++ )
      {
         Game_ClipSprites( &( player->sprite ), &( game->tileMap.npcs[i].sprite ), prevPos, &newPos );
      }

#if defined( VISUAL_STUDIO_DEV )
   }
#endif

   if ( newPos.x < 0 )
   {
      newPos.x = COLLISION_THETA;
   }
   else if ( ( newPos.x + player->sprite.hitBoxSize.x ) >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      newPos.x = ( game->tileMap.tilesX * TILE_SIZE ) - player->sprite.hitBoxSize.x - COLLISION_THETA;
   }

   if ( newPos.y < 0 )
   {
      newPos.y = COLLISION_THETA;
   }
   else if ( ( newPos.y + player->sprite.hitBoxSize.y ) >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      newPos.y = ( game->tileMap.tilesY * TILE_SIZE ) - player->sprite.hitBoxSize.y - COLLISION_THETA;
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.noClip == False ) {
#endif

      // clip to unpassable horizontal tiles
      if ( newPos.x != player->sprite.position.x )
      {
         tileRowStartIndex = (uint32_t)( player->sprite.position.y / TILE_SIZE );
         tileRowEndIndex = (uint32_t)( ( player->sprite.position.y + player->sprite.hitBoxSize.y ) / TILE_SIZE );

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
            col = (uint32_t )( ( newPos.x + player->sprite.hitBoxSize.x ) / TILE_SIZE );

            for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetDoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->gameFlags.doors ) )
               {
                  newPos.x = ( col * TILE_SIZE ) - player->sprite.hitBoxSize.x - COLLISION_THETA;
                  break;
               }
            }
         }
      }

      // clip to unpassable vertical tiles
      if ( newPos.y != player->sprite.position.y )
      {
         tileColStartIndex = ( uint32_t )( player->sprite.position.x / TILE_SIZE );
         tileColEndIndex = (uint32_t)( ( player->sprite.position.x + player->sprite.hitBoxSize.x ) / TILE_SIZE );

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
            row = (uint32_t)( ( newPos.y + player->sprite.hitBoxSize.y ) / TILE_SIZE );

            for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !TILE_GET_PASSABLE( tile ) ||
                    ( TileMap_GetDoorFlag( game->tileMap.id, col + ( row * game->tileMap.tilesX ) ) & game->gameFlags.doors ) )
               {
                  newPos.y = ( row * TILE_SIZE ) - player->sprite.hitBoxSize.y - COLLISION_THETA;
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
   Game_MoveNpcs( game );

#if defined( VISUAL_STUDIO_DEV )
   if ( !g_debugFlags.noTileDamage ) {
#endif

      if ( !game->gameFlags.defeatedDragonlord && ( game->player.armor.id != ARMOR_ERDRICKSARMOR_ID ) &&
           ( TILE_GET_DAMAGERATE( game->tileMap.tiles[player->canonicalTileIndex] ) > 0 ) )
      {
         if ( ( prevPos.x != newPos.x ) || ( prevPos.y != newPos.y ) )
         {
            ActiveSprite_Flicker( &( player->sprite ) );
         }
      }
      else
      {
         ActiveSprite_StopFlickering( &( player->sprite ) );
      }

#if defined( VISUAL_STUDIO_DEV )
   }
   else
   {
      ActiveSprite_StopFlickering( &( game->player.sprite ) );
   }
#endif
}

void Game_PlayerSteppedOnTile( Game_t* game )
{
   TilePortal_t* portal;
   char msg[64];

#if defined VISUAL_STUDIO_DEV
   if ( !g_debugFlags.noTileDamage )
   {
#endif

      Game_ApplyTileDamage( game );

#if defined VISUAL_STUDIO_DEV
   }
#endif

   if ( game->player.stats.hitPoints == 0 )
   {
      // must have stepped on a damage tile and died
      return;
   }
   
   if ( game->player.armor.id == ARMOR_MAGICARMOR_ID || game->player.armor.id == ARMOR_ERDRICKSARMOR_ID )
   {
      Player_RestoreHitPoints( &( game->player ), 1 );
   }

   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), game->player.tileIndex );
   portal = TileMap_GetPortalForTileIndex( &( game->tileMap ), game->player.tileIndex );

   if ( portal )
   {
      if ( ( game->tileMap.id == TILEMAP_OVERWORLD_ID ) && game->gameFlags.carryingPrincess &&
           ( portal->destinationTileMapIndex != TILEMAP_TANTEGEL_ID ) && ( portal->destinationTileMapIndex != TILEMAP_SWAMPCAVE_ID ) )
      {
         Dialog_Reset( &( game->dialog ) );
         sprintf( msg, STRING_NPC_OVERWORLD_PRINCESS, game->player.name );
         Dialog_PushSection( &( game->dialog ), msg );
         Game_OpenDialog( game );
         return;
      }
      else
      {
         AnimationChain_Reset( &( game->animationChain ) );
         Game_AnimatePortalEntrance( game, portal );
         AnimationChain_Start( &( game->animationChain ) );
         return;
      }
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

   if ( !game->gameFlags.defeatedDragonlord )
   {
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

         Game_RollEncounter( game );
      }
   }
}

#define PHYSICS_CLIPSPRITE_TOPLEFT() if ( newPos->x < prevPos.x ) if ( newPos->y < prevPos.y ) if ( ( clipHitBoxR - newPos->x ) > ( clipHitBoxB - newPos->y ) ) newPos->y = clipHitBoxB + COLLISION_THETA; else newPos->x = clipHitBoxR + COLLISION_THETA; else newPos->x = clipHitBoxR + COLLISION_THETA; else newPos->y = clipHitBoxB + COLLISION_THETA
#define PHYSICS_CLIPSPRITE_TOPRIGHT() if ( newPos->x > prevPos.x ) if ( newPos->y < prevPos.y ) if ( ( mainHitBoxR - clipHitBoxX ) > ( clipHitBoxB - newPos->y ) ) newPos->y = clipHitBoxB + COLLISION_THETA; else newPos->x = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else newPos->x = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else newPos->y = clipHitBoxY + clipHitBoxH + COLLISION_THETA
#define PHYSICS_CLIPSPRITE_BOTTOMLEFT() if ( newPos->x < prevPos.x ) if ( newPos->y > prevPos.y ) if ( ( clipHitBoxR - newPos->x ) > ( mainHitBoxB - clipHitBoxY ) ) newPos->y = clipHitBoxY - mainHitBoxH - COLLISION_THETA; else newPos->x = clipHitBoxR + COLLISION_THETA; else newPos->x = clipHitBoxR + COLLISION_THETA; else newPos->y = clipHitBoxY - mainHitBoxH - COLLISION_THETA
#define PHYSICS_CLIPSPRITE_BOTTOMRIGHT() if ( newPos->x > prevPos.x ) if ( newPos->y > prevPos.y ) if ( ( mainHitBoxR - clipHitBoxX ) > ( mainHitBoxB - clipHitBoxY ) ) newPos->y = clipHitBoxY - mainHitBoxH - COLLISION_THETA; else newPos->x = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else newPos->x = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else newPos->y = clipHitBoxY - mainHitBoxH - COLLISION_THETA

internal void Game_ClipSprites( ActiveSprite_t* mainSprite, ActiveSprite_t* clipSprite, Vector2f_t prevPos, Vector2f_t* newPos )
{
   float clipHitBoxX = clipSprite->position.x;
   float clipHitBoxY = clipSprite->position.y;
   float clipHitBoxW = (float)( clipSprite->hitBoxSize.x );
   float clipHitBoxH = (float)( clipSprite->hitBoxSize.y );
   float clipHitBoxR = clipHitBoxX + clipHitBoxW;
   float clipHitBoxB = clipHitBoxY + clipHitBoxH;
   float mainHitBoxW = (float)( mainSprite->hitBoxSize.x );
   float mainHitBoxH = (float)( mainSprite->hitBoxSize.y );
   float mainHitBoxR = newPos->x + mainHitBoxW;
   float mainHitBoxB = newPos->y + mainHitBoxH;

   if ( newPos->x < clipHitBoxR && newPos->x > clipHitBoxX && newPos->y < clipHitBoxB && mainHitBoxB > clipHitBoxY )
   {
      // left side is colliding
      if ( Math_PointInRectF( newPos->x, newPos->y, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_TOPLEFT();
      }
      else if ( Math_PointInRectF( newPos->x, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_BOTTOMLEFT();
      }
      else
      {
         newPos->x = clipHitBoxR + COLLISION_THETA;
      }
   }
   else if ( newPos->y < clipHitBoxB && newPos->y > clipHitBoxY && newPos->x < clipHitBoxR && mainHitBoxR > clipHitBoxX )
   {
      // top side is colliding
      if ( Math_PointInRectF( newPos->x, newPos->y, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_TOPLEFT();
      }
      else if ( Math_PointInRectF( mainHitBoxR, newPos->y, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_TOPRIGHT();
      }
      else
      {
         newPos->y = clipHitBoxB + COLLISION_THETA;
      }
   }
   else if ( mainHitBoxR > clipHitBoxX && mainHitBoxR < clipHitBoxR && newPos->y < clipHitBoxB && mainHitBoxB > clipHitBoxY )
   {
      // right side is colliding
      if ( Math_PointInRectF( mainHitBoxR, newPos->y, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_TOPRIGHT();
      }
      else if ( Math_PointInRectF( mainHitBoxR, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_BOTTOMRIGHT();
      }
      else
      {
         newPos->x = clipHitBoxX - mainHitBoxW - COLLISION_THETA;
      }
   }
   else if ( mainHitBoxB > clipHitBoxY && mainHitBoxB < clipHitBoxB && newPos->x < clipHitBoxR && mainHitBoxR > clipHitBoxX )
   {
      // bottom side is colliding
      if ( Math_PointInRectF( newPos->x, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_BOTTOMLEFT();
      }
      else if ( Math_PointInRectF( mainHitBoxR, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPSPRITE_BOTTOMRIGHT();
      }
      else
      {
         newPos->y = clipHitBoxY - mainHitBoxH - COLLISION_THETA;
      }
   }
}

internal uint32_t Game_GetSpriteTileIndex( Game_t* game, ActiveSprite_t* sprite )
{
   uint32_t centerX = (uint32_t)( sprite->position.x + ( sprite->hitBoxSize.x / 2 ) );
   uint32_t centerY = (uint32_t)( sprite->position.y + ( sprite->hitBoxSize.y / 2 ) );
   return ( ( centerY / TILE_SIZE ) * game->tileMap.tilesX ) + ( centerX / TILE_SIZE );
}

internal void Game_UpdatePlayerTileIndex( Game_t* game )
{
   uint32_t newTileIndex = Game_GetSpriteTileIndex( game, &( game->player.sprite ) );

   if ( newTileIndex != game->player.tileIndex )
   {
      game->player.tileIndex = newTileIndex;
      Player_SetCanonicalTileIndex( &( game->player ) );
      Game_PlayerSteppedOnTile( game );
   }
}

internal void Game_RollEncounter( Game_t* game )
{
   uint32_t encounterRate = TILE_GET_ENCOUNTERRATE( game->tileMap.tiles[game->player.canonicalTileIndex] );
   Bool_t spawnEncounter = False, zoneZero = False;
   uint32_t row, col;

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID )
   {
      row = game->player.tileIndex / game->tileMap.tilesX;
      col = game->player.tileIndex % game->tileMap.tilesX;

      if ( row >= TILEMAP_ZONEZERO_STARTROW && row <= TILEMAP_ZONEZERO_ENDROW &&
           col >= TILEMAP_ZONEZERO_STARTCOL && col <= TILEMAP_ZONEZERO_ENDCOL )
      {
         zoneZero = True;
      }
   }

   switch( encounterRate )
   {
      case 0: spawnEncounter = ( Random_u8( 1, zoneZero ? ENCOUNTERRATE_LOW * 2 : ENCOUNTERRATE_LOW ) == 1 ) ? True : False; break;
      case 1: spawnEncounter = ( Random_u8( 1, zoneZero ? ENCOUNTERRATE_MEDIUM * 2 : ENCOUNTERRATE_MEDIUM ) == 1 ) ? True : False; break;
      case 2: spawnEncounter = ( Random_u8( 1, zoneZero ? ENCOUNTERRATE_HIGH * 2 : ENCOUNTERRATE_HIGH ) == 1 ) ? True : False; break;
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

internal void Game_ApplyTileDamage( Game_t* game )
{
   uint8_t damage;
   uint16_t damageRate;
   uint32_t i;

   if ( game->gameFlags.defeatedDragonlord || ( game->player.armor.id == ARMOR_ERDRICKSARMOR_ID ) )
   {
      return;
   }

   damageRate = TILE_GET_DAMAGERATE( game->tileMap.tiles[game->player.canonicalTileIndex] );

   if ( damageRate == 0 )
   {
      return;
   }

   damage = ( damageRate == 1 ) ? TILEMAP_SWAMP_DAMAGE : TILEMAP_BARRIER_DAMAGE;
   game->player.stats.hitPoints -= Math_Min8u( damage, game->player.stats.hitPoints );

   if ( game->player.stats.hitPoints == 0 )
   {
      // TODO: need to show some kind of animation here, and make sure the player gets drawn

      AnimationChain_Reset( &( game->animationChain ) );

      for ( i = 0; i < 7; i++ )
      {
         AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
      }

      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_TileDamageDeathCallback, game );
      AnimationChain_Start( &( game->animationChain ) );
   }
}

internal void Game_TileDamageDeathCallback( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_DIALOG_DEAD, Game_TileDamageDeathMessageCallback, game );
   Game_OpenDialog( game );
}

internal void Game_TileDamageDeathMessageCallback( Game_t* game )
{
   uint32_t i;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 11; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_HandleDeath, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_MoveNpcs( Game_t* game )
{
   uint32_t i;
   NonPlayerCharacter_t* npc;
   Vector2f_t newPos;
   float leftBound, topBound, rightBound, bottomBound;

   for ( i = 0; i < game->tileMap.npcCount; i++ )
   {
      npc = &( game->tileMap.npcs[i] );

      TileMap_TicNpcWander( npc );
      newPos.x = npc->sprite.position.x;
      newPos.y = npc->sprite.position.y;

      if ( npc->isWandering )
      {
         switch ( npc->sprite.direction )
         {
            case Direction_Left: newPos.x -= ( TILE_WALKSPEED_NPC * CLOCK_FRAME_SECONDS ); break;
            case Direction_Up: newPos.y -= ( TILE_WALKSPEED_NPC * CLOCK_FRAME_SECONDS ); break;
            case Direction_Right: newPos.x += ( TILE_WALKSPEED_NPC * CLOCK_FRAME_SECONDS ); break;
            case Direction_Down: newPos.y += ( TILE_WALKSPEED_NPC * CLOCK_FRAME_SECONDS ); break;
         }

         Game_ClipSprites( &( npc->sprite ), &( game->player.sprite ), npc->sprite.position, &newPos );

         npc->sprite.position.x = newPos.x;
         npc->sprite.position.y = newPos.y;
         leftBound = (float)( npc->wanderBounds.x * TILE_SIZE );
         topBound = (float)( npc->wanderBounds.y * TILE_SIZE );
         rightBound = (float)( ( npc->wanderBounds.x + npc->wanderBounds.w ) * TILE_SIZE );
         bottomBound = (float)( ( npc->wanderBounds.y + npc->wanderBounds.h ) * TILE_SIZE );

         if ( newPos.x < leftBound )
         {
            npc->sprite.position.x = leftBound;
            TileMap_StopNpc( npc );
         }
         else if ( ( newPos.x + SPRITE_TEXTURE_SIZE ) > rightBound )
         {
            npc->sprite.position.x = rightBound - SPRITE_TEXTURE_SIZE;
            TileMap_StopNpc( npc );
         }

         if ( newPos.y < topBound )
         {
            npc->sprite.position.y = topBound;
            TileMap_StopNpc( npc );
         }
         else if ( ( newPos.y + SPRITE_TEXTURE_SIZE ) > bottomBound )
         {
            npc->sprite.position.y = bottomBound - SPRITE_TEXTURE_SIZE;
            TileMap_StopNpc( npc );
         }

         npc->tileIndex = Game_GetSpriteTileIndex( game, &( npc->sprite ) );
      }
   }
}
