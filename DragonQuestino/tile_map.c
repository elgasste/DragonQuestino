#include "tile_map.h"
#include "screen.h"
#include "math.h"

#define SPRITE_CHEST_INDEX  0

internal void TileMap_DrawStaticSprites( TileMap_t* tileMap );

void TileMap_Init( TileMap_t* tileMap, Screen_t* screen )
{
   tileMap->screen = screen;
   TileMap_ResetViewport( tileMap );

   Sprite_LoadStatic( &( tileMap->chestSprite ), SPRITE_CHEST_INDEX );
   tileMap->treasureFlags = 0xFFFFFFFF;
   tileMap->isDark = False;
   tileMap->lightDiameter = 1;
}

void TileMap_ResetViewport( TileMap_t* tileMap )
{
   tileMap->viewport.x = 0;
   tileMap->viewport.y = 0;
   tileMap->viewport.w = SCREEN_WIDTH;
   tileMap->viewport.h = SCREEN_HEIGHT;
   tileMap->viewportScreenPos.x = 0;
   tileMap->viewportScreenPos.y = 0;
}

void TileMap_UpdateViewport( TileMap_t* tileMap, int32_t anchorX, int32_t anchorY, uint32_t anchorW, uint32_t anchorH )
{
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

void TileMap_ChangeViewportSize( TileMap_t* tileMap, uint16_t w, uint16_t h )
{
   tileMap->viewport.w = w;
   tileMap->viewport.h = h;
   tileMap->viewportScreenPos.x = ( SCREEN_WIDTH - w ) / 2;
   tileMap->viewportScreenPos.y = ( SCREEN_HEIGHT - h ) / 2;
}

void TileMap_SetLightDiameter( TileMap_t* tileMap, uint32_t diameter )
{
   tileMap->lightDiameter = diameter;
   TileMap_ChangeViewportSize( tileMap, (uint16_t)( diameter * TILE_SIZE ), (uint16_t)diameter * TILE_SIZE );
}

float TileMap_GetWalkSpeedForTileIndex( TileMap_t* tileMap, uint32_t tileIndex )
{
   uint16_t tile = tileMap->tiles[( ( tileIndex / tileMap->tilesX ) * TILE_COUNT_X ) + ( tileIndex % tileMap->tilesX )];
   uint16_t walkSpeed = GET_TILEWALKSPEED( tile );

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

void TileMap_Draw( TileMap_t* tileMap )
{
   uint32_t firstTileX, firstTileY, lastTileX, lastTileY, tileX, tileY, textureIndex, tileOffsetX, tileOffsetY, tileWidth, tileHeight, screenX, screenY, treasureFlag;
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
         textureIndex = GET_TILETEXTUREINDEX( tileMap->tiles[( tileY * TILE_COUNT_X ) + tileX] );
         tileWidth = ( tileX == firstTileX ) ? TILE_SIZE - tileOffsetX : ( tileX == lastTileX ) ? ( viewport->x + viewport->w ) % TILE_SIZE : TILE_SIZE;

         Screen_DrawMemorySection( tileMap->screen, tileMap->textures[textureIndex].memory, TILE_SIZE,
                                   tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                   tileWidth, tileHeight,
                                   screenX + tileMap->viewportScreenPos.x, screenY + tileMap->viewportScreenPos.y, False );

         treasureFlag = TileMap_GetTreasureFlag( tileMap->id, ( tileY * tileMap->tilesX ) + tileX );

         if ( treasureFlag && ( tileMap->treasureFlags & treasureFlag ) )
         {
            Screen_DrawMemorySection( tileMap->screen, tileMap->chestSprite.texture.memory, SPRITE_TEXTURE_SIZE,
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
