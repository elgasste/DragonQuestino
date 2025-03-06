#include "tile_map.h"
#include "screen.h"
#include "game_flags.h"
#include "player.h"
#include "clock.h"
#include "math.h"

#define SPRITE_CHEST_INDEX    0
#define SPRITE_DOOR_INDEX     1

internal void TileMap_UpdateViewport( TileMap_t* tileMap );
internal void TileMap_SetGlowDiameter( TileMap_t* tileMap, uint32_t diameter );
internal void TileMap_ReduceGlowDiameter( TileMap_t* tileMap );
internal void TileMap_IncreaseGlowDiameter( TileMap_t* tileMap );
internal void TileMap_DrawStaticSprites( TileMap_t* tileMap );

void TileMap_Init( TileMap_t* tileMap, Screen_t* screen, GameFlags_t* gameFlags, Player_t* player )
{
   tileMap->screen = screen;
   tileMap->gameFlags = gameFlags;
   tileMap->player = player;
   TileMap_ResetViewport( tileMap );

   Sprite_LoadStatic( &( tileMap->chestSprite ), SPRITE_CHEST_INDEX );
   Sprite_LoadStatic( &( tileMap->doorSprite ), SPRITE_DOOR_INDEX );

   tileMap->isDungeon = False;
   tileMap->isDark = False;
}

void TileMap_Tic( TileMap_t* tileMap )
{
   if ( tileMap->isDark && tileMap->glowDiameter != tileMap->targetGlowDiameter )
   {
      tileMap->glowTransitionSeconds += CLOCK_FRAME_SECONDS;

      while ( tileMap->glowTransitionSeconds > GLOW_TRANSITION_FRAME_SECONDS )
      {
         tileMap->glowTransitionSeconds -= GLOW_TRANSITION_FRAME_SECONDS;

         if ( tileMap->glowDiameter < tileMap->targetGlowDiameter )
         {
            TileMap_IncreaseGlowDiameter( tileMap );
         }
         else
         {
            TileMap_ReduceGlowDiameter( tileMap );
         }
      }
   }

   TileMap_UpdateViewport( tileMap );
}

void TileMap_ResetViewport( TileMap_t* tileMap )
{
   tileMap->viewport.x = 0;
   tileMap->viewport.y = 0;
   tileMap->viewport.w = SCREEN_WIDTH;
   tileMap->viewport.h = SCREEN_HEIGHT;
   tileMap->viewportScreenPos.x = 0;
   tileMap->viewportScreenPos.y = 0;
   tileMap->glowDiameter = 1;
   tileMap->targetGlowDiameter = 1;

   TileMap_UpdateViewport( tileMap );
}

void TileMap_ChangeViewportSize( TileMap_t* tileMap, uint16_t w, uint16_t h )
{
   tileMap->viewport.w = w;
   tileMap->viewport.h = h;
   tileMap->viewportScreenPos.x = ( SCREEN_WIDTH - w ) / 2;
   tileMap->viewportScreenPos.y = ( SCREEN_HEIGHT - h ) / 2;

   TileMap_UpdateViewport( tileMap );
}

void TileMap_SetTargetGlowDiameter( TileMap_t* tileMap, uint32_t targetDiameter )
{
   tileMap->targetGlowDiameter = targetDiameter;
}

void TileMap_StartGlowTransition( TileMap_t* tileMap )
{
   tileMap->glowTransitionSeconds = GLOW_TRANSITION_FRAME_SECONDS; // push one frame immediately
}

void TileMap_ReduceTargetGlowDiameter( TileMap_t* tileMap )
{
   if ( tileMap->targetGlowDiameter > 1 )
   {
      tileMap->targetGlowDiameter -= 2;
   }
}

float TileMap_GetWalkSpeedForTileIndex( TileMap_t* tileMap, uint32_t tileIndex )
{
   uint16_t tile = tileMap->tiles[( ( tileIndex / tileMap->tilesX ) * TILE_COUNT_X ) + ( tileIndex % tileMap->tilesX )];
   uint16_t walkSpeed = TILE_GET_WALKSPEED( tile );

#if defined( VISUAL_STUDIO_DEV )

   if ( g_debugFlags.fastWalk )
   {
      return TILE_WALKSPEED_FAST;
   }

#endif

   switch ( walkSpeed )
   {
      case 0: return TILE_WALKSPEED_NORMAL;
      case 1: return TILE_WALKSPEED_SLOW;
      case 2: return TILE_WALKSPEED_VERYSLOW;
      case 3: return TILE_WALKSPEED_CRAWL;
   }

   return TILE_WALKSPEED_NORMAL;
}

TilePortal_t* TileMap_GetPortalForTileIndex( TileMap_t* tileMap, uint32_t index )
{
   uint32_t i;

   for ( i = 0; i < tileMap->portalCount; i++ )
   {
      if ( tileMap->portals[i].sourceTileIndex == index )
      {
         return &( tileMap->portals[i] );
      }
   }

   return 0;
}

uint32_t TileMap_GetFacingTileIndex( TileMap_t* tileMap, uint32_t sourceTileIndex, Direction_t direction )
{
   switch ( direction )
   {
      case Direction_Left: return sourceTileIndex - 1;
      case Direction_Right: return sourceTileIndex + 1;
      case Direction_Up: return sourceTileIndex - tileMap->tilesX;
      case Direction_Down: return sourceTileIndex + tileMap->tilesX;
   }

   return 0;
}

void TileMap_Draw( TileMap_t* tileMap )
{
   uint32_t firstTileX, firstTileY, lastTileX, lastTileY, tileX, tileY, textureIndex, tileOffsetX, tileOffsetY, tileWidth, tileHeight, screenX, screenY, tileIndex, treasureFlag, doorFlag;
   Vector4i32_t* viewport = &( tileMap->viewport );

   firstTileX = viewport->x / TILE_SIZE;
   firstTileY = viewport->y / TILE_SIZE;
   lastTileX = ( viewport->x + viewport->w ) / TILE_SIZE;
   lastTileY = ( viewport->y + viewport->h ) / TILE_SIZE;
   tileOffsetX = viewport->x % TILE_SIZE;
   tileOffsetY = viewport->y % TILE_SIZE;

   for ( tileY = firstTileY, screenY = 0; tileY <= lastTileY; tileY++ )
   {
      tileHeight = ( tileY == firstTileY ) ? TILE_SIZE - tileOffsetY : ( tileY == lastTileY ) ? ( viewport->y + viewport->h ) % TILE_SIZE : TILE_SIZE;

      for ( tileX = firstTileX, screenX = 0; tileX <= lastTileX; tileX++ )
      {
         textureIndex = TILE_GET_TEXTUREINDEX( tileMap->tiles[( tileY * TILE_COUNT_X ) + tileX] );
         tileWidth = ( tileX == firstTileX ) ? TILE_SIZE - tileOffsetX : ( tileX == lastTileX ) ? ( viewport->x + viewport->w ) % TILE_SIZE : TILE_SIZE;

         Screen_DrawMemorySection( tileMap->screen, tileMap->textures[textureIndex].memory, TILE_SIZE,
                                   tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                   tileWidth, tileHeight,
                                   screenX + tileMap->viewportScreenPos.x, screenY + tileMap->viewportScreenPos.y, False );

         tileIndex = ( tileY * tileMap->tilesX ) + tileX;
         treasureFlag = TileMap_GetTreasureFlag( tileMap->id, tileIndex );
         doorFlag = TileMap_GetDoorFlag( tileMap->id, tileIndex );

         if ( treasureFlag && ( tileMap->gameFlags->treasures & treasureFlag ) )
         {
            Screen_DrawMemorySection( tileMap->screen, tileMap->chestSprite.texture.memory, SPRITE_TEXTURE_SIZE,
                                      tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                      tileWidth, tileHeight,
                                      screenX + tileMap->viewportScreenPos.x, screenY + tileMap->viewportScreenPos.y, True );
         }
         else if ( doorFlag && ( tileMap->gameFlags->doors & doorFlag ) )
         {
            Screen_DrawMemorySection( tileMap->screen, tileMap->doorSprite.texture.memory, SPRITE_TEXTURE_SIZE,
                                      tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                      tileWidth, tileHeight,
                                      screenX + tileMap->viewportScreenPos.x, screenY + tileMap->viewportScreenPos.y, True );
         }

         screenX += tileWidth;
      }

      screenY += tileHeight;
   }

   TileMap_DrawStaticSprites( tileMap );
}

internal void TileMap_UpdateViewport( TileMap_t* tileMap )
{
   int32_t anchorX = (int32_t)( tileMap->player->sprite.position.x );
   int32_t anchorY = (int32_t)( tileMap->player->sprite.position.y );
   uint32_t anchorW = tileMap->player->hitBoxSize.x;
   uint32_t anchorH = tileMap->player->hitBoxSize.y;
   Vector4i32_t* viewport = &( tileMap->viewport );

   viewport->x = anchorX - ( viewport->w / 2 ) + ( anchorW / 2 );
   viewport->y = anchorY - ( viewport->h / 2 ) + ( anchorH / 2 );

   if ( viewport->x < 0 )
   {
      viewport->x = 0;
   }
   else if ( ( viewport->x + viewport->w ) > (int32_t)( tileMap->tilesX * TILE_SIZE ) )
   {
      viewport->x = ( tileMap->tilesX * TILE_SIZE ) - viewport->w;
   }

   if ( viewport->y < 0 )
   {
      viewport->y = 0;
   }
   else if ( ( viewport->y + viewport->h ) > (int32_t)( tileMap->tilesY * TILE_SIZE ) )
   {
      viewport->y = ( tileMap->tilesY * TILE_SIZE ) - viewport->h;
   }
}

internal void TileMap_SetGlowDiameter( TileMap_t* tileMap, uint32_t diameter )
{
   tileMap->glowDiameter = diameter;
   TileMap_ChangeViewportSize( tileMap, (uint16_t)( tileMap->glowDiameter * TILE_SIZE ), (uint16_t)tileMap->glowDiameter * TILE_SIZE );
}

internal void TileMap_ReduceGlowDiameter( TileMap_t* tileMap )
{
   if ( tileMap->glowDiameter > tileMap->targetGlowDiameter )
   {
      TileMap_SetGlowDiameter( tileMap, tileMap->glowDiameter - 2 );
   }
}

internal void TileMap_IncreaseGlowDiameter( TileMap_t* tileMap )
{
   if ( tileMap->glowDiameter < tileMap->targetGlowDiameter )
   {
      TileMap_SetGlowDiameter( tileMap, tileMap->glowDiameter + 2 );
   }
}

internal void TileMap_DrawStaticSprites( TileMap_t* tileMap )
{
   uint32_t i, tx, ty, tw, th, sxu, syu;
   int32_t sx, sy;
   StaticSprite_t* sprite;
   Vector4i32_t* viewport = &( tileMap->viewport );

   for ( i = 0; i < tileMap->staticSpriteCount; i++ )
   {
      sprite = &( tileMap->staticSprites[i] );
      sx = sprite->position.x - viewport->x;
      sy = sprite->position.y - viewport->y;

      if ( Math_RectsIntersect32i( sprite->position.x, sprite->position.y, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE,
                                   viewport->x, viewport->y, viewport->w, viewport->h ) )
      {
         tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
         ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
         tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > viewport->w ) ? ( viewport->w - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
         th = ( ( sy + SPRITE_TEXTURE_SIZE ) > viewport->h ) ? ( viewport->h - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
         sxu = ( sx < 0 ) ? 0 : sx;
         syu = ( sy < 0 ) ? 0 : sy;

         Screen_DrawMemorySection( tileMap->screen, sprite->texture.memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th,
                                   sxu + tileMap->viewportScreenPos.x, syu + tileMap->viewportScreenPos.y, True );
      }
   }
}
