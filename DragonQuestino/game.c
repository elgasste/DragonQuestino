#include "game.h"
#include "physics.h"
#include "math.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_TicOverworld( Game_t* game );
internal void Game_TicTileMapTransition( Game_t* game );
internal void Game_HandleInput( Game_t* game );
internal void Game_UpdateTileMapViewport( Game_t* game );
internal void Game_DrawTileMap( Game_t* game );
internal void Game_DrawStaticSprites( Game_t* game );
internal void Game_DrawPlayer( Game_t* game );
internal void Game_DrawTextureSection( Game_t* game, uint8_t* memory, uint32_t stride,
                                       uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th,
                                       uint32_t sx, uint32_t sy, Bool_t transparency );

void Game_Init( Game_t* game, uint16_t* screenBuffer )
{
   Screen_Init( &( game->screen ), screenBuffer );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 0 );
   Sprite_LoadPlayer( &( game->player.sprite ) );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ) );

   game->tileMapViewport.x = 0;
   game->tileMapViewport.y = 0;
   game->tileMapViewport.w = TILEMAP_VIEWPORT_WIDTH;
   game->tileMapViewport.h = TILEMAP_VIEWPORT_HEIGHT;

   game->player.sprite.position.x = (float)( TILE_SIZE * 55 );
   game->player.sprite.position.y = (float)( TILE_SIZE * 49 );
   game->player.velocity.x = 0.0f;
   game->player.velocity.y = 0.0f;
   game->player.maxVelocity = TILE_WALKSPEED_NORMAL;
   game->player.hitBoxSize.x = TILE_SIZE - 4;
   game->player.hitBoxSize.y = TILE_SIZE - 4;
   game->player.spriteOffset.x = -2;
   game->player.spriteOffset.y = -4;
   game->player.sprite.direction = Direction_Down;

   game->state = GameState_Overworld;
   game->isSwappingTileMap = False;
}

void Game_Tic( Game_t* game )
{
   switch ( game->state )
   {
      case GameState_Overworld:
         Game_TicOverworld( game );
         break;
      case GameState_TileMapTransition:
         Game_TicTileMapTransition( game );
         break;
   }

   Screen_RenderBuffer( &( game->screen ) );
}

internal void Game_TicOverworld( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_HandleInput( game );
   Physics_Tic( game );

   // TODO: there's probably a better way to do this. by the time we get here,
   // the state may have changed (if the player stepped on a portal, for instance).
   // I don't like having to check for that in here though, so maybe refactor?
   if ( game->state == GameState_Overworld )
   {
      Sprite_Tic( &( game->player.sprite ) );
      Game_UpdateTileMapViewport( game );
      Game_DrawTileMap( game );
      Game_DrawStaticSprites( game );
      Game_DrawPlayer( game );
   }
}

internal void Game_TicTileMapTransition( Game_t* game )
{
   uint32_t destinationTileIndex;
   Direction_t arrivalDirection;

   if ( !game->isSwappingTileMap )
   {
      game->isSwappingTileMap = True;
      game->tileMapSwapSecondsElapsed = 0.0f;

      destinationTileIndex = game->swapPortal->destinationTileIndex;
      arrivalDirection = game->swapPortal->arrivalDirection;

      TileMap_Load( &( game->tileMap ), game->swapPortal->destinationTileMapIndex );

      game->player.sprite.position.x = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex % game->tileMap.tilesX ) ) - game->player.spriteOffset.x ) + COLLISION_THETA;
      // the player sprite gets caught on unpassable tiles unless we use COLLISION_THETA here, but for some reason the x-axis has no problems
      game->player.sprite.position.y = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex / game->tileMap.tilesX ) ) - game->player.spriteOffset.y ) - COLLISION_THETA;
      game->player.tileIndex = destinationTileIndex;
      game->player.maxVelocity = TileMap_GetWalkSpeedForTile( game->tileMap.tiles[destinationTileIndex] );

      Sprite_SetDirection( &( game->player.sprite ), arrivalDirection );
   }
   else
   {
      game->tileMapSwapSecondsElapsed += CLOCK_FRAME_SECONDS;

      if ( game->tileMapSwapSecondsElapsed > TILEMAP_SWAP_SECONDS )
      {
         game->isSwappingTileMap = False;
         game->state = GameState_Overworld;
      }
   }
}

void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex )
{
   TilePortal_t* portal;
   uint8_t wipePaletteIndex;

   game->player.maxVelocity = TileMap_GetWalkSpeedForTile( game->tileMap.tiles[tileIndex] );
   game->player.tileIndex = tileIndex;
   portal = TileMap_GetPortalForTileIndex( &( game->tileMap ), tileIndex );

   if ( portal )
   {
      game->swapPortal = portal;

      if ( Screen_GetPaletteIndexForColor( &( game->screen ), 0, &wipePaletteIndex ) )
      {
         Screen_Wipe( &( game->screen ), wipePaletteIndex );
      }

      game->state = GameState_TileMapTransition;
   }
}

internal void Game_HandleInput( Game_t* game )
{
   Player_t* player = &( game->player );
   ActiveSprite_t* playerSprite = &( game->player.sprite );
   Bool_t leftIsDown = game->input.buttonStates[Button_Left].down;
   Bool_t upIsDown = game->input.buttonStates[Button_Up].down;
   Bool_t rightIsDown = game->input.buttonStates[Button_Right].down;
   Bool_t downIsDown = game->input.buttonStates[Button_Down].down;

   if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
   {
      if ( leftIsDown && !rightIsDown )
      {
         player->velocity.x = -( player->maxVelocity );

         if ( !( upIsDown && playerSprite->direction == Direction_Up ) &&
              !( downIsDown && playerSprite->direction == Direction_Down ) )
         {
            Sprite_SetDirection( playerSprite, Direction_Left );
         }

         if ( upIsDown || downIsDown )
         {
            player->velocity.x *= DIAGONAL_SCALAR;
         }
      }
      else if ( rightIsDown && !leftIsDown )
      {
         player->velocity.x = player->maxVelocity;

         if ( !( upIsDown && playerSprite->direction == Direction_Up ) &&
              !( downIsDown && playerSprite->direction == Direction_Down ) )
         {
            Sprite_SetDirection( playerSprite, Direction_Right );
         }

         if ( upIsDown || downIsDown )
         {
            player->velocity.x *= DIAGONAL_SCALAR;
         }
      }

      if ( upIsDown && !downIsDown )
      {
         player->velocity.y = -( player->maxVelocity );

         if ( !( leftIsDown && playerSprite->direction == Direction_Left ) &&
              !( rightIsDown && playerSprite->direction == Direction_Right ) )
         {
            Sprite_SetDirection( playerSprite, Direction_Up );
         }

         if ( leftIsDown || rightIsDown )
         {
            player->velocity.y *= DIAGONAL_SCALAR;
         }
      }
      else if ( downIsDown && !upIsDown )
      {
         player->velocity.y = player->maxVelocity;

         if ( !( leftIsDown && playerSprite->direction == Direction_Left ) &&
              !( rightIsDown && playerSprite->direction == Direction_Right ) )
         {
            Sprite_SetDirection( playerSprite, Direction_Down );
         }

         if ( leftIsDown || rightIsDown )
         {
            player->velocity.y *= DIAGONAL_SCALAR;
         }
      }
   }
}

internal void Game_UpdateTileMapViewport( Game_t* game )
{
   Vector4i32_t* viewport = &( game->tileMapViewport );

   viewport->x = (int32_t)( game->player.sprite.position.x ) - ( viewport->w / 2 ) + ( game->player.hitBoxSize.x / 2 );
   viewport->y = (int32_t)( game->player.sprite.position.y ) - ( viewport->h / 2 ) + ( game->player.hitBoxSize.y / 2 );

   if ( viewport->x < 0 )
   {
      viewport->x = 0;
   }
   else if ( ( viewport->x + viewport->w ) > (int32_t)( game->tileMap.tilesX * TILE_SIZE ) )
   {
      viewport->x = ( game->tileMap.tilesX * TILE_SIZE ) - viewport->w;
   }

   if ( viewport->y < 0 )
   {
      viewport->y = 0;
   }
   else if ( ( viewport->y + viewport->h ) > (int32_t)( game->tileMap.tilesY * TILE_SIZE ) )
   {
      viewport->y = ( game->tileMap.tilesY * TILE_SIZE ) - viewport->h;
   }
}

internal void Game_DrawTileMap( Game_t* game )
{
   uint32_t firstTileX, firstTileY, lastTileX, lastTileY, tileX, tileY, textureIndex, tileOffsetX, tileOffsetY, tileWidth, tileHeight, screenX, screenY;

   firstTileX = game->tileMapViewport.x / TILE_SIZE;
   firstTileY = game->tileMapViewport.y / TILE_SIZE;
   lastTileX = ( game->tileMapViewport.x + TILEMAP_VIEWPORT_WIDTH ) / TILE_SIZE;
   lastTileY = ( game->tileMapViewport.y + TILEMAP_VIEWPORT_HEIGHT ) / TILE_SIZE;
   tileOffsetX = game->tileMapViewport.x % TILE_SIZE;
   tileOffsetY = game->tileMapViewport.y % TILE_SIZE;

   for ( tileY = firstTileY, screenY = 0; tileY <= lastTileY; tileY++ )
   {
      tileHeight = ( tileY == firstTileY ) ? TILE_SIZE - tileOffsetY : ( tileY == lastTileY ) ? ( game->tileMapViewport.y + TILEMAP_VIEWPORT_HEIGHT ) % TILE_SIZE : TILE_SIZE;

      for ( tileX = firstTileX, screenX = 0; tileX <= lastTileX; tileX++ )
      {
         textureIndex = GET_TILETEXTUREINDEX( game->tileMap.tiles[( tileY * TILE_COUNT_X ) + tileX] );
         tileWidth = ( tileX == firstTileX ) ? TILE_SIZE - tileOffsetX : ( tileX == lastTileX ) ? ( game->tileMapViewport.x + TILEMAP_VIEWPORT_WIDTH ) % TILE_SIZE : TILE_SIZE;

         Game_DrawTextureSection( game, game->tileMap.textures[textureIndex].memory, TILE_SIZE,
                                  tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                  tileWidth, tileHeight,
                                  screenX, screenY, False );

         screenX += tileWidth;
      }

      screenY += tileHeight;
   }
}

internal void Game_DrawStaticSprites( Game_t* game )
{
   uint32_t i, tx, ty, tw, th, sxu, syu;
   int32_t sx, sy;
   StaticSprite_t* sprite;

   for ( i = 0; i < game->tileMap.staticSpriteCount; i++ )
   {
      sprite = &( game->tileMap.staticSprites[i] );
      sx = sprite->position.x - game->tileMapViewport.x;
      sy = sprite->position.y - game->tileMapViewport.y;

      if ( Math_RectsIntersect32i( sprite->position.x, sprite->position.y, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE,
                                   game->tileMapViewport.x, game->tileMapViewport.y, game->tileMapViewport.w, game->tileMapViewport.h ) )
      {
         tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
         ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
         tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_WIDTH ) ? ( TILEMAP_VIEWPORT_WIDTH - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
         th = ( ( sy + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_HEIGHT ) ? ( TILEMAP_VIEWPORT_HEIGHT - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
         sxu = ( sx < 0 ) ? 0 : sx;
         syu = ( sy < 0 ) ? 0 : sy;

         Game_DrawTextureSection( game, sprite->texture.memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th, sxu, syu, True );
      }
   }
}

internal void Game_DrawPlayer( Game_t* game )
{
   ActiveSprite_t* sprite = &( game->player.sprite );
   int32_t wx = (int32_t)( sprite->position.x ) + game->player.spriteOffset.x;
   int32_t wy = (int32_t)( sprite->position.y ) + game->player.spriteOffset.y;
   int32_t sx = wx - game->tileMapViewport.x;
   int32_t sy = wy - game->tileMapViewport.y;
   uint32_t textureIndex = ( (uint32_t)( sprite->direction ) * ACTIVE_SPRITE_FRAMES ) + sprite->currentFrame;
   uint32_t tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
   uint32_t ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
   uint32_t tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_WIDTH ) ? ( TILEMAP_VIEWPORT_WIDTH - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
   uint32_t th = ( ( sy + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_HEIGHT ) ? ( TILEMAP_VIEWPORT_HEIGHT - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
   uint32_t sxu = ( sx < 0 ) ? 0 : sx;
   uint32_t syu = ( sy < 0 ) ? 0 : sy;

   Game_DrawTextureSection( game, sprite->textures[textureIndex].memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th, sxu, syu, True );
}

internal void Game_DrawTextureSection( Game_t* game, uint8_t* memory, uint32_t stride,
                                       uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th,
                                       uint32_t sx, uint32_t sy, Bool_t transparency )
{
   uint32_t x, y;
   uint8_t* textureBufferPos = memory + ( ty * stride ) + tx;
   uint16_t* screenBufferPos = game->screen.buffer + ( sy * SCREEN_WIDTH ) + sx;

   if ( transparency )
   {
      for ( y = 0; y < th; y++ )
      {
         for ( x = 0; x < tw; x++ )
         {
            if ( *textureBufferPos != TRANSPARENT_COLOR_INDEX )
            {
               *screenBufferPos = game->screen.palette[*textureBufferPos];
            }

            textureBufferPos++;
            screenBufferPos++;
         }

         textureBufferPos += tx + ( stride - ( tx + tw ) );
         screenBufferPos += ( SCREEN_WIDTH - tw );
      }
   }
   else
   {
      for ( y = 0; y < th; y++ )
      {
         for ( x = 0; x < tw; x++ )
         {
            *screenBufferPos = game->screen.palette[*textureBufferPos];
            textureBufferPos++;
            screenBufferPos++;
         }

         textureBufferPos += tx + ( stride - ( tx + tw ) );
         screenBufferPos += ( SCREEN_WIDTH - tw );
      }
   }
}
