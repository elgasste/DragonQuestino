#include "game.h"
#include "physics.h"
#include "math.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleInput( Game_t* game );
internal void Game_TicOverworld( Game_t* game );
internal void Game_TicTileMapTransition( Game_t* game );
internal void Game_HandleOverworldInput( Game_t* game );
internal void Game_HandleOverworldScrollingDialogInput( Game_t* game );
internal void Game_HandleMenuInput( Game_t* game );
internal void Game_Draw( Game_t* game );
internal void Game_DrawOverworld( Game_t* game );
internal void Game_DrawStaticSprites( Game_t* game );
internal void Game_DrawPlayer( Game_t* game );
internal void Game_DrawOverworldStatus( Game_t* game );

void Game_Init( Game_t* game, uint16_t* screenBuffer )
{
   Screen_Init( &( game->screen ), screenBuffer );
   TileMap_Init( &( game->tileMap ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 0 );
   Sprite_LoadPlayer( &( game->player.sprite ) );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ) );
   ScrollingDialog_Init( &( game->scrollingDialog ), game->player.name );

   game->overworldInactivitySeconds = 0.0f;

   game->state = GameState_Overworld;
   game->swapPortal = 0;
}

void Game_ChangeState( Game_t* game, GameState_t newState )
{
   game->state = newState;

   if ( newState == GameState_Overworld )
   {
      game->overworldInactivitySeconds = 0.0f;
   }
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );
   Game_HandleInput( game );

   switch ( game->state )
   {
      case GameState_Overworld:
         Game_TicOverworld( game );
         break;
      case GameState_Overworld_ScrollingDialog:
         ScrollingDialog_Tic( &( game->scrollingDialog ) );
         break;
      case GameState_Overworld_MainMenu:
         Menu_Tic( &( game->menu ) );
         break;
      case GameState_TileMapTransition:
         Game_TicTileMapTransition( game );
         break;
   }

   Game_Draw( game );
   Screen_RenderBuffer( &( game->screen ) );
}

void Game_PlayerSteppedOnTile( Game_t* game, uint32_t tileIndex )
{
   TilePortal_t* portal;

   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), tileIndex );
   game->player.tileIndex = tileIndex;
   portal = TileMap_GetPortalForTileIndex( &( game->tileMap ), tileIndex );

   if ( portal )
   {
      game->swapPortal = portal;
      Game_ChangeState( game, GameState_TileMapTransition );
   }
}

internal void Game_HandleInput( Game_t* game )
{
   switch ( game->state )
   {
      case GameState_Overworld:
         Game_HandleOverworldInput( game );
         break;
      case GameState_Overworld_MainMenu:
         Game_HandleMenuInput( game );
         break;
      case GameState_Overworld_ScrollingDialog:
         Game_HandleOverworldScrollingDialogInput( game );
         break;
   }
}

internal void Game_TicOverworld( Game_t* game )
{
   Physics_Tic( game );
   Sprite_Tic( &( game->player.sprite ) );
   TileMap_UpdateViewport( &( game->tileMap ),
                           (int32_t)( game->player.sprite.position.x ), (int32_t)( game->player.sprite.position.y ),
                           game->player.hitBoxSize.x, game->player.hitBoxSize.y );
}

internal void Game_TicTileMapTransition( Game_t* game )
{
   uint32_t destinationTileIndex;
   Direction_t arrivalDirection;

   if ( game->swapPortal )
   {
      destinationTileIndex = game->swapPortal->destinationTileIndex;
      arrivalDirection = game->swapPortal->arrivalDirection;

      TileMap_Load( &( game->tileMap ), game->swapPortal->destinationTileMapIndex );

      game->player.sprite.position.x = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex % game->tileMap.tilesX ) ) - game->player.spriteOffset.x ) + COLLISION_THETA;
      // the player sprite gets caught on unpassable tiles unless we use COLLISION_THETA here, but for some reason the x-axis has no problems
      game->player.sprite.position.y = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex / game->tileMap.tilesX ) ) - game->player.spriteOffset.y ) - COLLISION_THETA;
      game->player.tileIndex = destinationTileIndex;
      game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), destinationTileIndex );
      game->tileMapSwapSecondsElapsed = 0.0f;
      game->swapPortal = 0;

      Sprite_SetDirection( &( game->player.sprite ), arrivalDirection );
      TileMap_UpdateViewport( &( game->tileMap ),
                              (int32_t)( game->player.sprite.position.x ), (int32_t)( game->player.sprite.position.y ),
                              game->player.hitBoxSize.x, game->player.hitBoxSize.y );
   }
   else
   {
      game->tileMapSwapSecondsElapsed += CLOCK_FRAME_SECONDS;

      if ( game->tileMapSwapSecondsElapsed > TILEMAP_SWAP_SECONDS )
      {
         Game_ChangeState( game, GameState_Overworld );
      }
   }
}

internal void Game_HandleOverworldInput( Game_t* game )
{
   Player_t* player = &( game->player );
   ActiveSprite_t* playerSprite = &( game->player.sprite );
   Bool_t leftIsDown = game->input.buttonStates[Button_Left].down;
   Bool_t upIsDown = game->input.buttonStates[Button_Up].down;
   Bool_t rightIsDown = game->input.buttonStates[Button_Right].down;
   Bool_t downIsDown = game->input.buttonStates[Button_Down].down;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      game->overworldInactivitySeconds = 0.0f;
      Menu_Load( &( game->menu ), MenuId_Overworld );
      Game_ChangeState( game, GameState_Overworld_MainMenu );
   }
   else if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
   {
      game->overworldInactivitySeconds = 0.0f;

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
   else if ( !game->input.buttonStates[Button_A].down && !game->input.buttonStates[Button_B].down )
   {
      game->overworldInactivitySeconds += CLOCK_FRAME_SECONDS;
   }
}

internal void Game_HandleOverworldScrollingDialogInput( Game_t* game )
{
   if ( Input_AnyButtonPressed( &( game->input ) ) )
   {
      if ( !ScrollingDialog_IsDone( &( game->scrollingDialog ) ) )
      {
         ScrollingDialog_Next( &( game->scrollingDialog ) );
      }
      else
      {
         Game_ChangeState( game, GameState_Overworld );
      }
   }
}

internal void Game_HandleMenuInput( Game_t* game )
{
   uint32_t i;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      Menu_ResetCursor( &( game->menu ) );

      switch ( game->menu.items[game->menu.selectedIndex].command )
      {
         case MenuCommand_Overworld_Talk:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Talk_NobodyThere );
            break;
         case MenuCommand_Overworld_Status:
            break;
         case MenuCommand_Overworld_Search:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Search_NothingFound );
            break;
         case MenuCommand_Overworld_Spell:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Spell_None );
            break;
         case MenuCommand_Overworld_Item:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Item_None );
            break;
         case MenuCommand_Overworld_Door:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Door_None );
            break;
      }
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      switch ( game->state )
      {
         case GameState_Overworld_MainMenu:
            Game_ChangeState( game, GameState_Overworld );
            break;
      }
   }
   else
   {
      for ( i = 0; i < Direction_Count; i++ )
      {
         if ( game->input.buttonStates[i].pressed )
         {
            Menu_MoveSelection( &( game->menu ), (Direction_t)i );
         }
      }
   }
}

internal void Game_Draw( Game_t* game )
{
   switch ( game->state )
   {
      case GameState_Overworld:
         Game_DrawOverworld( game );
         break;
      case GameState_Overworld_MainMenu:
         Game_DrawOverworld( game );
         Game_DrawOverworldStatus( game );
         Menu_Draw( &( game->menu ), &( game->screen ) );
         break;
      case GameState_Overworld_ScrollingDialog:
         Game_DrawOverworld( game );
         Game_DrawOverworldStatus( game );
         Menu_Draw( &( game->menu ), &( game->screen ) );
         ScrollingDialog_Draw( &( game->scrollingDialog ), &( game->screen ) );
         break;
      case GameState_TileMapTransition:
         Screen_WipeColor( &( game->screen ), COLOR_BLACK );
         break;
   }
}

internal void Game_DrawOverworld( Game_t* game )
{
   TileMap_Draw( &( game->tileMap ), &( game->screen ) );
   Game_DrawStaticSprites( game );
   Game_DrawPlayer( game );

   if ( game->overworldInactivitySeconds > OVERWORLD_INACTIVE_STATUS_SECONDS )
   {
      Game_DrawOverworldStatus( game );
   }
}

internal void Game_DrawStaticSprites( Game_t* game )
{
   uint32_t i, tx, ty, tw, th, sxu, syu;
   int32_t sx, sy;
   StaticSprite_t* sprite;
   Vector4i32_t* viewport = &( game->tileMap.viewport );

   for ( i = 0; i < game->tileMap.staticSpriteCount; i++ )
   {
      sprite = &( game->tileMap.staticSprites[i] );
      sx = sprite->position.x - viewport->x;
      sy = sprite->position.y - viewport->y;

      if ( Math_RectsIntersect32i( sprite->position.x, sprite->position.y, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE,
                                   viewport->x, viewport->y, viewport->w, viewport->h ) )
      {
         tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
         ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
         tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_WIDTH ) ? ( TILEMAP_VIEWPORT_WIDTH - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
         th = ( ( sy + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_HEIGHT ) ? ( TILEMAP_VIEWPORT_HEIGHT - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
         sxu = ( sx < 0 ) ? 0 : sx;
         syu = ( sy < 0 ) ? 0 : sy;

         Screen_DrawMemorySection( &( game->screen ), sprite->texture.memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th, sxu, syu, True );
      }
   }
}

internal void Game_DrawPlayer( Game_t* game )
{
   ActiveSprite_t* sprite = &( game->player.sprite );
   int32_t wx = (int32_t)( sprite->position.x ) + game->player.spriteOffset.x;
   int32_t wy = (int32_t)( sprite->position.y ) + game->player.spriteOffset.y;
   int32_t sx = wx - game->tileMap.viewport.x;
   int32_t sy = wy - game->tileMap.viewport.y;
   uint32_t textureIndex = ( (uint32_t)( sprite->direction ) * ACTIVE_SPRITE_FRAMES ) + sprite->currentFrame;
   uint32_t tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
   uint32_t ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
   uint32_t tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_WIDTH ) ? ( TILEMAP_VIEWPORT_WIDTH - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
   uint32_t th = ( ( sy + SPRITE_TEXTURE_SIZE ) > TILEMAP_VIEWPORT_HEIGHT ) ? ( TILEMAP_VIEWPORT_HEIGHT - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
   uint32_t sxu = ( sx < 0 ) ? 0 : sx;
   uint32_t syu = ( sy < 0 ) ? 0 : sy;

   Screen_DrawMemorySection( &( game->screen ), sprite->textures[textureIndex].memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th, sxu, syu, True );
}

internal void Game_DrawOverworldStatus( Game_t* game )
{
   uint8_t lvl = Player_GetLevel( &( game->player ) );
   uint32_t memSize;
   char line[9];

   memSize = MATH_MINU32( (uint32_t)( strlen( game->player.name ) ), 4 );
   memcpy( line, game->player.name, sizeof( char ) * memSize );
   line[memSize] = '\0';
   Screen_DrawTextWindowWithTitle( &( game->screen ), 16, 16, 8, 12, line, COLOR_WHITE );

   sprintf( line, lvl < 10 ? "LV   %u" : "LV  %u", lvl);
   Screen_DrawText( &( game->screen ), line, 24, 32, COLOR_WHITE );

   sprintf( line, game->player.stats.hitPoints < 10 ? "HP   %u" : game->player.stats.hitPoints < 100 ? "HP  %u" : "HP %u", game->player.stats.hitPoints );
   Screen_DrawText( &( game->screen ), line, 24, 48, COLOR_WHITE );

   sprintf( line, game->player.stats.magicPoints < 10 ? "MP   %u" : game->player.stats.magicPoints < 100 ? "MP  %u" : "MP %u", game->player.stats.magicPoints );
   Screen_DrawText( &( game->screen ), line, 24, 64, COLOR_WHITE );

   sprintf( line, game->player.gold < 10 ? "G    %u" : game->player.gold < 100 ? "G   %u" : game->player.gold < 1000 ? "G  %u" : game->player.gold < 10000 ? "G %u" : "G%u", game->player.gold );
   Screen_DrawText( &( game->screen ), line, 24, 80, COLOR_WHITE );

   sprintf( line, game->player.experience < 10 ? "E    %u" : game->player.experience < 100 ? "E   %u" : game->player.experience < 1000 ? "E  %u" : game->player.experience < 10000 ? "E %u" : "E%u", game->player.experience );
   Screen_DrawText( &( game->screen ), line, 24, 96, COLOR_WHITE );
}
