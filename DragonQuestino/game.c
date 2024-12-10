#include "game.h"

internal void Game_DrawTileMap( Game_t* game );

void Game_Init( Game_t* game )
{
   Screen_Init( &( game->screen ) );
   TileMap_Init( &( game->tileMap ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), &( game->screen ), 0 );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_DrawTileMap( game );
   Screen_RenderBuffer( &( game->screen ) );
}

internal void Game_DrawTileMap( Game_t* game )
{
   uint8_t tileX, tileY, textureIndex, *textureBufferPos, *screenBufferPos, y;
   TileMap_t* tileMap = &( game->tileMap );

   for ( tileY = 0; tileY < 15; tileY++ )
   {
      for ( tileX = 0; tileX < 20; tileX++ )
      {
         textureIndex = GET_TILETEXTUREINDEX( tileMap->tiles[( tileY * TILE_COUNT_X ) + tileX] );
         textureBufferPos = tileMap->textures[textureIndex].memory;
         screenBufferPos = game->screen.buffer + ( ( tileY * TILE_SIZE ) * SCREEN_BUFFER_BYTES_X ) + ( ( tileX * TILE_SIZE ) / 2 );

         for ( y = 0; y < TILE_SIZE; y++ )
         {
            memcpy( screenBufferPos, textureBufferPos, TILE_BYTES_X );
            textureBufferPos += TILE_BYTES_X;
            screenBufferPos += SCREEN_BUFFER_BYTES_X;
         }
      }
   }
}
