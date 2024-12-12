#include "game.h"

internal void Game_DrawTileMap( Game_t* game );
internal void Game_HandleInput( Game_t* game );
internal void Game_DrawTileTextureSection( Game_t* game, uint8_t index,
                                           uint16_t tx, uint16_t ty, uint16_t tw, uint16_t th,
                                           uint16_t sx, uint16_t sy );

void Game_Init( Game_t* game )
{
   Screen_Init( &( game->screen ) );
   TileMap_Init( &( game->tileMap ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), &( game->screen ), 0 );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );

   game->tileMapViewport.x = 0;
   game->tileMapViewport.y = 0;
   game->tileMapViewport.w = SCREEN_BUFFER_WIDTH;
   game->tileMapViewport.h = SCREEN_BUFFER_HEIGHT;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_HandleInput( game );
   Game_DrawTileMap( game );
   Screen_RenderBuffer( &( game->screen ) );
}

internal void Game_HandleInput( Game_t* game )
{
   Vector4i32_t* viewport = &( game->tileMapViewport );
   Bool_t leftIsDown = game->input.buttonStates[Button_Left].down;
   Bool_t upIsDown = game->input.buttonStates[Button_Up].down;
   Bool_t rightIsDown = game->input.buttonStates[Button_Right].down;
   Bool_t downIsDown = game->input.buttonStates[Button_Down].down;

   if ( leftIsDown )
   {
      viewport->x -= 5;
      if ( viewport->x < 0 )
         viewport->x = 0;
   }
   if ( upIsDown )
   {
      viewport->y -= 5;
      if ( viewport->y < 0 ) viewport->y = 0;
   }
   if ( rightIsDown )
   {
      viewport->x += 5;
      if ( ( ( TILE_COUNT_X * TILE_SIZE ) - viewport->x ) < SCREEN_BUFFER_WIDTH ) viewport->x = ( TILE_COUNT_X * TILE_SIZE ) - SCREEN_BUFFER_WIDTH;
   }
   if ( downIsDown )
   {
      viewport->y += 5;
      if ( ( ( TILE_COUNT_Y * TILE_SIZE ) - viewport->y ) < SCREEN_BUFFER_HEIGHT ) viewport->y = ( TILE_COUNT_Y * TILE_SIZE ) - SCREEN_BUFFER_HEIGHT;
   }
}

internal void Game_DrawTileMap( Game_t* game )
{
   uint8_t tileX, tileY, textureIndex;
   TileMap_t* tileMap = &( game->tileMap );

   for ( tileY = 0; tileY < 15; tileY++ )
   {
      for ( tileX = 0; tileX < 20; tileX++ )
      {
         textureIndex = GET_TILETEXTUREINDEX( tileMap->tiles[( tileY * TILE_COUNT_X ) + tileX] );
         Game_DrawTileTextureSection( game, textureIndex, 0, 0, TILE_SIZE, TILE_SIZE, tileX * TILE_SIZE, tileY * TILE_SIZE );
      }
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
      screenBufferPos += ( SCREEN_BUFFER_WIDTH );
   }
}
