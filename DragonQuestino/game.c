#include "game.h"
#include "physics.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_DrawTileMap( Game_t* game );
internal void Game_HandleInput( Game_t* game );
internal void Game_DrawTileTextureSection( Game_t* game, uint8_t index,
                                           uint16_t tx, uint16_t ty, uint16_t tw, uint16_t th,
                                           uint16_t sx, uint16_t sy );
internal void Game_DrawPlayer( Game_t* game );
internal void Game_DrawSprite( Game_t* game, Sprite_t* sprite, uint16_t x, uint16_t y );

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
   game->player.hitBoxSize.x = TILE_SIZE;
   game->player.hitBoxSize.y = TILE_SIZE;
   game->player.spriteOffset.x = 0;
   game->player.spriteOffset.y = 0;
   game->playerSprite.direction = Direction_Down;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_HandleInput( game );
   Game_DrawTileMap( game );
   Physics_Tic( game );
   Sprite_Tic( &( game->playerSprite ) );
   Game_DrawPlayer( game );
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
   uint8_t firstTileX, firstTileY, lastTileX, lastTileY, tileX, tileY, textureIndex;
   uint16_t tileOffsetX, tileOffsetY, tileWidth, tileHeight, screenX, screenY;

   firstTileX = (uint8_t)( game->tileMapViewport.x / TILE_SIZE );
   firstTileY = (uint8_t)( game->tileMapViewport.y / TILE_SIZE );
   lastTileX = (uint8_t)( ( game->tileMapViewport.x + SCREEN_BUFFER_WIDTH ) / TILE_SIZE );
   lastTileY = (uint8_t)( ( game->tileMapViewport.y + SCREEN_BUFFER_HEIGHT ) / TILE_SIZE );
   tileOffsetX = (uint16_t)( game->tileMapViewport.x % TILE_SIZE );
   tileOffsetY = (uint16_t)( game->tileMapViewport.y % TILE_SIZE );

   for ( tileY = firstTileY, screenY = 0; tileY <= lastTileY; tileY++ )
   {
      tileHeight = ( tileY == firstTileY ) ? TILE_SIZE - tileOffsetY : ( tileY == lastTileY ) ? ( game->tileMapViewport.y + SCREEN_BUFFER_HEIGHT ) % TILE_SIZE : TILE_SIZE;

      for ( tileX = firstTileX, screenX = 0; tileX <= lastTileX; tileX++ )
      {
         textureIndex = GET_TILETEXTUREINDEX( game->tileMap.tiles[( tileY * TILE_COUNT_X ) + tileX] );
         tileWidth = ( tileX == firstTileX ) ? TILE_SIZE - tileOffsetX : ( tileX == lastTileX ) ? ( game->tileMapViewport.x + SCREEN_BUFFER_WIDTH ) % TILE_SIZE : TILE_SIZE;

         Game_DrawTileTextureSection( game, textureIndex,
                                      tileX == firstTileX ? tileOffsetX : 0, tileY == firstTileY ? tileOffsetY : 0,
                                      tileWidth, tileHeight,
                                      screenX, screenY );

         screenX += tileWidth;
      }

      screenY += tileHeight;
   }
}

internal void Game_DrawTileTextureSection( Game_t* game, uint8_t index,
                                           uint16_t tx, uint16_t ty, uint16_t tw, uint16_t th,
                                           uint16_t sx, uint16_t sy )
{
   uint8_t y;
   uint8_t* textureBufferPos = game->tileMap.textures[index].memory + ( ty * TILE_SIZE ) + tx;
   uint8_t* screenBufferPos = game->screen.buffer + ( sy * SCREEN_BUFFER_WIDTH ) + sx;

   for ( y = 0; y < th; y++ )
   {
      memcpy( screenBufferPos, textureBufferPos, tw );
      textureBufferPos += tw + tx + ( TILE_SIZE - ( tx + tw ) );
      screenBufferPos += SCREEN_BUFFER_WIDTH;
   }
}

internal void Game_DrawPlayer( Game_t* game )
{
   // TODO: take the sprite offset into consideration
   Game_DrawSprite( game, &( game->playerSprite ), (uint16_t)( game->player.position.x ), (uint16_t)( game->player.position.y ) );
}

internal void Game_DrawSprite( Game_t* game, Sprite_t* sprite, uint16_t x, uint16_t y )
{
   uint8_t i, j;
   uint8_t textureIndex = (uint8_t)( sprite->direction * SPRITE_FRAMES ) + sprite->currentFrame;
   uint8_t* textureBufferPos = sprite->textures[textureIndex].memory;
   uint8_t* screenBufferPos = game->screen.buffer + ( y * SCREEN_BUFFER_WIDTH ) + x;

   for ( i = 0; i < SPRITE_TEXTURE_SIZE; i++ )
   {
      for ( j = 0; j < SPRITE_TEXTURE_SIZE; j++ )
      {
         if ( *textureBufferPos != TRANSPARENT_COLOR_INDEX )
         {
            *screenBufferPos = *textureBufferPos;
         }

         textureBufferPos++;
         screenBufferPos++;
      }

      screenBufferPos += ( SCREEN_BUFFER_WIDTH - SPRITE_TEXTURE_SIZE );
   }
}
