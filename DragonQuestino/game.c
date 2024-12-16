#include "game.h"
#include "physics.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_DrawTileMap( Game_t* game );
internal void Game_HandleInput( Game_t* game );
internal void Game_DrawSprite( Game_t* game, Sprite_t* sprite, int32_t x, int32_t y );
internal void Game_DrawTextureSection( Game_t* game, uint8_t* memory, uint32_t stride,
                                       uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th,
                                       uint32_t sx, uint32_t sy, Bool_t transparency );

void Game_Init( Game_t* game )
{
   Screen_Init( &( game->screen ) );
   TileMap_Init( &( game->tileMap ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 0 );
   Sprite_Load( &( game->playerSprite ), 0 );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ) );

   game->tileMapViewport.x = 0;
   game->tileMapViewport.y = 0;
   game->tileMapViewport.w = SCREEN_BUFFER_WIDTH;
   game->tileMapViewport.h = SCREEN_BUFFER_HEIGHT;

   game->player.position.x = 0.0f;
   game->player.position.y = 0.0f;
   game->player.velocity.x = 0.0f;
   game->player.velocity.y = 0.0f;
   game->player.hitBoxSize.x = TILE_SIZE - 4;
   game->player.hitBoxSize.y = TILE_SIZE - 4;
   game->player.spriteOffset.x = -2;
   game->player.spriteOffset.y = -4;
   game->playerSprite.direction = Direction_Down;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_HandleInput( game );
   Game_DrawTileMap( game );
   Physics_Tic( game );
   Sprite_Tic( &( game->playerSprite ) );
   Game_DrawSprite( game, &( game->playerSprite ),
                    (int16_t)( game->player.position.x ) + game->player.spriteOffset.x,
                    (int16_t)( game->player.position.y ) + game->player.spriteOffset.y );
   Screen_RenderBuffer( &( game->screen ) );
}

internal void Game_HandleInput( Game_t* game )
{
   Player_t* player = &( game->player );
   Sprite_t* sprite = &( game->playerSprite );
   Bool_t leftIsDown = game->input.buttonStates[Button_Left].down;
   Bool_t upIsDown = game->input.buttonStates[Button_Up].down;
   Bool_t rightIsDown = game->input.buttonStates[Button_Right].down;
   Bool_t downIsDown = game->input.buttonStates[Button_Down].down;

   if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
   {
      if ( leftIsDown && !rightIsDown )
      {
         player->velocity.x = -PLAYER_MAX_VELOCITY;

         if ( !( upIsDown && sprite->direction == Direction_Up ) &&
              !( downIsDown && sprite->direction == Direction_Down ) )
         {
            Sprite_SetDirection( sprite, Direction_Left );
         }

         if ( upIsDown || downIsDown )
         {
            player->velocity.x *= DIAGONAL_SCALAR;
         }
      }
      else if ( rightIsDown && !leftIsDown )
      {
         player->velocity.x = PLAYER_MAX_VELOCITY;

         if ( !( upIsDown && sprite->direction == Direction_Up ) &&
              !( downIsDown && sprite->direction == Direction_Down ) )
         {
            Sprite_SetDirection( sprite, Direction_Right );
         }

         if ( upIsDown || downIsDown )
         {
            player->velocity.x *= DIAGONAL_SCALAR;
         }
      }

      if ( upIsDown && !downIsDown )
      {
         player->velocity.y = -PLAYER_MAX_VELOCITY;

         if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
              !( rightIsDown && sprite->direction == Direction_Right ) )
         {
            Sprite_SetDirection( sprite, Direction_Up );
         }

         if ( leftIsDown || rightIsDown )
         {
            player->velocity.y *= DIAGONAL_SCALAR;
         }
      }
      else if ( downIsDown && !upIsDown )
      {
         player->velocity.y = PLAYER_MAX_VELOCITY;

         if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
              !( rightIsDown && sprite->direction == Direction_Right ) )
         {
            Sprite_SetDirection( sprite, Direction_Down );
         }

         if ( leftIsDown || rightIsDown )
         {
            player->velocity.y *= DIAGONAL_SCALAR;
         }
      }
   }
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

internal void Game_DrawSprite( Game_t* game, Sprite_t* sprite, int32_t x, int32_t y )
{
   uint8_t textureIndex = (uint8_t)( sprite->direction * SPRITE_FRAMES ) + sprite->currentFrame;
   uint32_t tx = ( x < 0 ) ? (uint32_t)( -x ) : 0;
   uint32_t ty = ( y < 0 ) ? (uint32_t)( -y ) : 0;
   uint32_t tw = ( ( x + SPRITE_TEXTURE_SIZE ) >= SCREEN_BUFFER_WIDTH ) ? ( SCREEN_BUFFER_WIDTH - x ) : ( SPRITE_TEXTURE_SIZE - tx );
   uint32_t th = ( ( y + SPRITE_TEXTURE_SIZE ) >= SCREEN_BUFFER_HEIGHT ) ? ( SCREEN_BUFFER_HEIGHT - y ) : ( SPRITE_TEXTURE_SIZE - ty );
   uint32_t sx = ( x < 0 ) ? 0 : x;
   uint32_t sy = ( y < 0 ) ? 0 : y;

   Game_DrawTextureSection( game, sprite->textures[textureIndex].memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th, sx, sy, True );
}

internal void Game_DrawTextureSection( Game_t* game, uint8_t* memory, uint32_t stride,
                                       uint32_t tx, uint32_t ty, uint32_t tw, uint32_t th,
                                       uint32_t sx, uint32_t sy, Bool_t transparency )
{
   uint8_t x, y;
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
