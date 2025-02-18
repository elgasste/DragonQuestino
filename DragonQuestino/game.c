#include "game.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleInput( Game_t* game );
internal void Game_TicOverworld( Game_t* game );
internal void Game_TicOverworldWashing( Game_t* game );
internal void Game_TicPhysics( Game_t* game );
internal void Game_TicTileMapTransition( Game_t* game );
internal void Game_HandleOverworldInput( Game_t* game );
internal void Game_HandleOverworldWaitingInput( Game_t* game );
internal void Game_HandleOverworldScrollingDialogInput( Game_t* game );
internal void Game_HandleMenuInput( Game_t* game );
internal void Game_UpdatePlayerTileIndex( Game_t* game );
internal void Game_OpenOverworldItemMenu( Game_t* game );

void Game_Init( Game_t* game, uint16_t* screenBuffer )
{
   Screen_Init( &( game->screen ), screenBuffer );
   TileMap_Init( &( game->tileMap ), &( game->screen ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 0 );
   Sprite_LoadPlayer( &( game->player.sprite ) );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ) );
   Menu_Init( &( game->menu ), &( game->screen ), &( game->player ) );
   ScrollingDialog_Init( &( game->scrollingDialog ), &( game->screen ), &( game->player ) );

   game->state = GameState_Overworld;
   game->swapPortal = 0;
   game->overworldInactivitySeconds = 0.0f;
}

void Game_ChangeState( Game_t* game, GameState_t newState )
{
   game->state = newState;

   switch ( newState )
   {
      case GameState_Overworld:
         game->overworldInactivitySeconds = 0.0f;
         break;
      case GameState_Overworld_Washing:
         game->overworldWashSeconds = 0.0f;
         break;
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
      case GameState_Overworld_Washing:
         Game_TicOverworldWashing( game );
         break;
      case GameState_Overworld_ScrollingDialog:
         ScrollingDialog_Tic( &( game->scrollingDialog ) );
         break;
      case GameState_Overworld_MainMenu:
      case GameState_Overworld_ItemMenu:
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
      case GameState_Overworld_Waiting:
         Game_HandleOverworldWaitingInput( game );
         break;
      case GameState_Overworld_MainMenu:
      case GameState_Overworld_ItemMenu:
         Game_HandleMenuInput( game );
         break;
      case GameState_Overworld_ScrollingDialog:
         Game_HandleOverworldScrollingDialogInput( game );
         break;
   }
}

internal void Game_TicOverworld( Game_t* game )
{
   Game_TicPhysics( game );
   Sprite_Tic( &( game->player.sprite ) );
   TileMap_UpdateViewport( &( game->tileMap ),
                           (int32_t)( game->player.sprite.position.x ), (int32_t)( game->player.sprite.position.y ),
                           game->player.hitBoxSize.x, game->player.hitBoxSize.y );
}

internal void Game_TicOverworldWashing( Game_t* game )
{
   game->overworldWashSeconds += CLOCK_FRAME_SECONDS;

   if ( game->overworldWashSeconds > OVERWORLD_WASH_TOTAL_SECONDS )
   {
      Game_ChangeState( game, GameState_Overworld );
   }
}

internal void Game_TicPhysics( Game_t* game )
{
   Vector2f_t newPos;
   uint32_t tileRowStartIndex, tileRowEndIndex, tileColStartIndex, tileColEndIndex, row, col, tile, tileIndex;
   Player_t* player = &( game->player );

   if ( player->velocity.x == 0.0f && player->velocity.y == 0.0f )
   {
      return;
   }

   newPos.x = player->sprite.position.x + ( player->velocity.x * CLOCK_FRAME_SECONDS );
   newPos.y = player->sprite.position.y + ( player->velocity.y * CLOCK_FRAME_SECONDS );

   if ( newPos.x < 0 )
   {
      newPos.x = COLLISION_THETA;
   }
   else if ( ( newPos.x + player->hitBoxSize.x ) >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      newPos.x = ( game->tileMap.tilesX * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_THETA;
   }

   if ( newPos.y < 0 )
   {
      newPos.y = COLLISION_THETA;
   }
   else if ( ( newPos.y + player->hitBoxSize.y ) >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      newPos.y = ( game->tileMap.tilesY * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_THETA;
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.noClip == False ) {
#endif

      // clip to unpassable horizontal tiles
      if ( newPos.x != player->sprite.position.x )
      {
         tileRowStartIndex = (uint32_t)( player->sprite.position.y / TILE_SIZE );
         tileRowEndIndex = (uint32_t)( ( player->sprite.position.y + player->hitBoxSize.y ) / TILE_SIZE );

         if ( newPos.x < player->sprite.position.x )
         {
            // moving left, check leftward tiles
            col = (uint32_t)( newPos.x / TILE_SIZE );

            for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.x = (float)( ( ( col + 1 ) * TILE_SIZE ) );
                  break;
               }
            }
         }
         else
         {
            // moving right, check rightward tiles
            col = (uint32_t )( ( newPos.x + player->hitBoxSize.x ) / TILE_SIZE );

            for ( row = tileRowStartIndex; row <= tileRowEndIndex; row++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.x = ( col * TILE_SIZE ) - player->hitBoxSize.x - COLLISION_THETA;
                  break;
               }
            }
         }
      }

      // clip to unpassable vertical tiles
      if ( newPos.y != player->sprite.position.y )
      {
         tileColStartIndex = ( uint32_t )( player->sprite.position.x / TILE_SIZE );
         tileColEndIndex = (uint32_t)( ( player->sprite.position.x + player->hitBoxSize.x ) / TILE_SIZE );

         if ( newPos.y < player->sprite.position.y )
         {
            // moving up, check upward tiles
            row = (uint32_t)( newPos.y / TILE_SIZE );

            for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.y = (float)( ( ( row + 1 ) * TILE_SIZE ) );
                  break;
               }
            }
         }
         else
         {
            // moving down, check downward tiles
            row = (uint32_t)( ( newPos.y + player->hitBoxSize.y ) / TILE_SIZE );

            for ( col = tileColStartIndex; col <= tileColEndIndex; col++ )
            {
               tileIndex = col + ( row * TILE_COUNT_X );
               tile = game->tileMap.tiles[tileIndex];

               if ( !GET_TILEPASSABLE( tile ) )
               {
                  newPos.y = ( row * TILE_SIZE ) - player->hitBoxSize.y - COLLISION_THETA;
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
      Menu_Load( &( game->menu ), MenuId_Overworld);
      Game_ChangeState( game, GameState_Overworld_MainMenu );
      Game_DrawOverworldQuickStatus( game );
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
   else
   {
      game->overworldInactivitySeconds += CLOCK_FRAME_SECONDS;
   }
}

internal void Game_HandleOverworldWaitingInput( Game_t* game )
{
   if ( Input_AnyButtonPressed( &( game->input ) ) )
   {
      Game_ChangeState( game, GameState_Overworld_Washing );
   }
}

internal void Game_HandleOverworldScrollingDialogInput( Game_t* game )
{
   if ( ScrollingDialog_IsDone( &( game->scrollingDialog ) ) )
   {
      if ( Input_AnyButtonPressed( &( game->input ) ) )
      {
         Game_ChangeState( game, GameState_Overworld_Washing );
      }
   }
   else if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
   {
      ScrollingDialog_Next( &( game->scrollingDialog ) );
   }
}

internal void Game_HandleMenuInput( Game_t* game )
{
   uint32_t i;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      Menu_ResetCarat( &( game->menu ) );

      switch ( game->menu.items[game->menu.selectedIndex].command )
      {
         case MenuCommand_OverworldMain_Talk:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Talk_NobodyThere );
            break;
         case MenuCommand_OverworldMain_Status:
            Game_DrawOverworldDeepStatus( game );
            Game_ChangeState( game, GameState_Overworld_Waiting );
            break;
         case MenuCommand_OverworldMain_Search:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Search_NothingFound );
            break;
         case MenuCommand_OverworldMain_Spell:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Spell_None );
            break;
         case MenuCommand_OverworldMain_Item:
            Game_OpenOverworldItemMenu( game );
            break;
         case MenuCommand_OverworldMain_Door:
            Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
            ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Door_None );
            break;

         case MenuCommand_OverworldItem_Herb: Game_UseHerb( game ); break;
         case MenuCommand_OverworldItem_Wing: Game_UseWing( game ); break;
         case MenuCommand_OverworldItem_FairyWater: Game_UseFairyWater( game ); break;
         case MenuCommand_OverworldItem_SilverHarp: Game_UseSilverHarp( game ); break;
         case MenuCommand_OverworldItem_FairyFlute: Game_UseFairyFlute( game ); break;
         case MenuCommand_OverworldItem_GwaelynsLove: Game_UseGwaelynsLove( game ); break;
         case MenuCommand_OverworldItem_RainbowDrop: Game_UseRainbowDrop( game ); break;
      }
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      switch ( game->state )
      {
         case GameState_Overworld_MainMenu:
         case GameState_Overworld_ItemMenu:
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

internal void Game_OpenOverworldItemMenu( Game_t* game )
{
   uint32_t useableCount = PLAYER_GET_MAPUSEABLEITEMCOUNT( game->player.items );
   uint32_t nonUseableCount = PLAYER_GET_MAPNONUSEABLEITEMCOUNT( game->player.items );

   if ( useableCount == 0 && nonUseableCount == 0 )
   {
      Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
      ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Item_None );
   }
   else
   {
      Game_ChangeState( game, ( useableCount > 0 ) ? GameState_Overworld_ItemMenu : GameState_Overworld_Waiting );

      if ( useableCount > 0 )
      {
         Menu_Load( &( game->menu ), MenuId_OverworldItem );
      }

      if ( nonUseableCount > 0 )
      {
         Game_DrawNonUseableItems( game );
      }
   }
}

internal void Game_UpdatePlayerTileIndex( Game_t* game )
{
   uint32_t centerX = (uint32_t)( game->player.sprite.position.x + ( game->player.hitBoxSize.x / 2 ) );
   uint32_t centerY = (uint32_t)( game->player.sprite.position.y + ( game->player.hitBoxSize.y / 2 ) );
   uint32_t newTileIndex = ( ( centerY / TILE_SIZE ) * game->tileMap.tilesX ) + ( centerX / TILE_SIZE );

   if ( newTileIndex != game->player.tileIndex )
   {
      game->player.tileIndex = newTileIndex;
      Game_PlayerSteppedOnTile( game, newTileIndex );
   }
}
