#include "game.h"
#include "physics.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleInput( Game_t* game );
internal void Game_UpdateTileMapViewport( Game_t* game );
internal void Game_EnterTilePortal( Game_t* game, TilePortal_t* portal );
internal void Game_DrawTileMap( Game_t* game );
internal void Game_DrawSprites( Game_t* game );
internal void Game_DrawTextureSection( Game_t* game, uint8_t* memory, uint32_t stride,
                                       uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th,
                                       uint32_t sx, uint32_t sy, Bool_t transparency );

void Game_Init( Game_t* game )
{
   Screen_Init( &( game->screen ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 0 );
   Sprite_LoadPlayer( &( game->player.sprite ) );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ) );

   game->tileMapViewport.x = 0;
   game->tileMapViewport.y = 0;
   game->tileMapViewport.w = SCREEN_BUFFER_WIDTH;
   game->tileMapViewport.h = SCREEN_BUFFER_HEIGHT;

   game->player.position.x = (float)( TILE_SIZE * 12 );
   game->player.position.y = (float)( TILE_SIZE * 8 );
   game->player.velocity.x = 0.0f;
   game->player.velocity.y = 0.0f;
   game->player.maxVelocity = TILE_WALKSPEED_NORMAL;
   game->player.hitBoxSize.x = TILE_SIZE - 4;
   game->player.hitBoxSize.y = TILE_SIZE - 4;
   game->player.spriteOffset.x = -2;
   game->player.spriteOffset.y = -4;
   game->player.sprite.direction = Direction_Down;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_HandleInput( game );
   Physics_Tic( game );
   Sprite_Tic( &( game->player.sprite ) );
   Game_UpdateTileMapViewport( game );
   Game_DrawTileMap( game );
   Game_DrawSprites( game );
   Screen_RenderBuffer( &( game->screen ) );
}

void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex )
{
   TilePortal_t* portal;

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.fastWalk == False )
   {
#endif

   game->player.maxVelocity = TileMap_GetWalkSpeedForTile( game->tileMap.tiles[tileIndex] );

#if defined( VISUAL_STUDIO_DEV )
   }
#endif

   game->player.tileIndex = tileIndex;
   portal = TileMap_GetPortalForTileIndex( &( game->tileMap ), tileIndex );

   if ( portal )
   {
      Game_EnterTilePortal( game, portal );
   }
}

internal void Game_HandleInput( Game_t* game )
{
   Player_t* player = &( game->player );
   Sprite_t* playerSprite = &( game->player.sprite );
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

   viewport->x = (int32_t)( game->player.position.x ) - ( viewport->w / 2 ) + ( game->player.hitBoxSize.x / 2 );
   viewport->y = (int32_t)( game->player.position.y ) - ( viewport->h / 2 ) + ( game->player.hitBoxSize.y / 2 );

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

internal void Game_EnterTilePortal( Game_t* game, TilePortal_t* portal )
{
   // TODO: some kind of animation between maps, because this is too abrupt

   TileMap_Load( &( game->tileMap ), portal->destinationTileMapIndex );

   game->player.position.x = (float)( ( int32_t )( TILE_SIZE * ( portal->destinationTileIndex % game->tileMap.tilesX ) ) - game->player.spriteOffset.x );
   game->player.position.y = (float)( ( int32_t )( TILE_SIZE * ( portal->destinationTileIndex / game->tileMap.tilesY ) ) - game->player.spriteOffset.y );

   Sprite_SetDirection( &( game->player.sprite ), portal->arrivalDirection );
}

internal void Game_DrawTileMap( Game_t* game )
{
   uint32_t firstTileX, firstTileY, lastTileX, lastTileY, tileX, tileY, textureIndex, tileOffsetX, tileOffsetY, tileWidth, tileHeight, screenX, screenY;

   firstTileX = game->tileMapViewport.x / TILE_SIZE;
   firstTileY = game->tileMapViewport.y / TILE_SIZE;
   lastTileX = ( game->tileMapViewport.x + SCREEN_BUFFER_WIDTH ) / TILE_SIZE;
   lastTileY = ( game->tileMapViewport.y + SCREEN_BUFFER_HEIGHT ) / TILE_SIZE;
   tileOffsetX = game->tileMapViewport.x % TILE_SIZE;
   tileOffsetY = game->tileMapViewport.y % TILE_SIZE;

   for ( tileY = firstTileY, screenY = 0; tileY <= lastTileY; tileY++ )
   {
      tileHeight = ( tileY == firstTileY ) ? TILE_SIZE - tileOffsetY : ( tileY == lastTileY ) ? ( game->tileMapViewport.y + SCREEN_BUFFER_HEIGHT ) % TILE_SIZE : TILE_SIZE;

      for ( tileX = firstTileX, screenX = 0; tileX <= lastTileX; tileX++ )
      {
         textureIndex = GET_TILETEXTUREINDEX( game->tileMap.tiles[( tileY * TILE_COUNT_X ) + tileX] );
         tileWidth = ( tileX == firstTileX ) ? TILE_SIZE - tileOffsetX : ( tileX == lastTileX ) ? ( game->tileMapViewport.x + SCREEN_BUFFER_WIDTH ) % TILE_SIZE : TILE_SIZE;

         Game_DrawTextureSection( game, game->tileMap.textures[textureIndex].memory, TILE_SIZE,
                                  tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                  tileWidth, tileHeight,
                                  screenX, screenY, False );

         screenX += tileWidth;
      }

      screenY += tileHeight;
   }
}

internal void Game_DrawSprites( Game_t* game )
{
   Sprite_t* sprite = &( game->player.sprite );
   int32_t wx = (int32_t)( game->player.position.x ) + game->player.spriteOffset.x;
   int32_t wy = (int32_t)( game->player.position.y ) + game->player.spriteOffset.y;
   int32_t sx = wx - game->tileMapViewport.x;
   int32_t sy = wy - game->tileMapViewport.y;
   uint32_t textureIndex = ( (uint32_t)( sprite->direction ) * SPRITE_FRAMES ) + sprite->currentFrame;
   uint32_t tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
   uint32_t ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
   uint32_t tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > SCREEN_BUFFER_WIDTH ) ? ( SCREEN_BUFFER_WIDTH - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
   uint32_t th = ( ( sy + SPRITE_TEXTURE_SIZE ) > SCREEN_BUFFER_HEIGHT ) ? ( SCREEN_BUFFER_HEIGHT - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
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
   uint8_t* screenBufferPos = game->screen.buffer + ( sy * SCREEN_BUFFER_WIDTH ) + sx;

   for ( y = 0; y < th; y++ )
   {
      if ( transparency )
      {
         for ( x = 0; x < tw; x++ )
         {
            if ( *textureBufferPos != TRANSPARENT_COLOR_INDEX )
            {
               *screenBufferPos = *textureBufferPos;
            }

            textureBufferPos++;
            screenBufferPos++;
         }

         textureBufferPos += tx + ( stride - ( tx + tw ) );
         screenBufferPos += ( SCREEN_BUFFER_WIDTH - tw );
      }
      else
      {
         memcpy( screenBufferPos, textureBufferPos, tw );
         textureBufferPos += tw + tx + ( stride - ( tx + tw ) );
         screenBufferPos += SCREEN_BUFFER_WIDTH;
      }
   }
}
