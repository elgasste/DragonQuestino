#include "game.h"
#include "physics.h"
#include "math.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleInput( Game_t* game );
internal void Game_TicOverworld( Game_t* game );
internal void Game_TicTileMapTransition( Game_t* game );
internal void Game_HandleOverworldInput( Game_t* game );
internal void Game_HandleOverworldWaitingInput( Game_t* game );
internal void Game_HandleOverworldScrollingDialogInput( Game_t* game );
internal void Game_HandleMenuInput( Game_t* game );
internal void Game_OpenOverworldItemMenu( Game_t* game );
internal void Game_UseHerb( Game_t* game );
internal void Game_UseWing( Game_t* game );
internal void Game_UseFairyWater( Game_t* game );
internal void Game_UseSilverHarp( Game_t* game );
internal void Game_UseFairyFlute( Game_t* game );
internal void Game_UseGwaelynsLove( Game_t* game );
internal void Game_UseRainbowDrop( Game_t* game );
internal void Game_Draw( Game_t* game );
internal void Game_DrawOverworld( Game_t* game );
internal void Game_DrawStaticSprites( Game_t* game );
internal void Game_DrawPlayer( Game_t* game );
internal void Game_DrawOverworldQuickStatus( Game_t* game );
internal void Game_DrawOverworldDeepStatus( Game_t* game );
internal void Game_DrawNonUseableItems( Game_t* game );

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
   Menu_Init( &( game->menu ), &( game->screen ), &( game->player ) );
   ScrollingDialog_Init( &( game->scrollingDialog ), &( game->screen ), &( game->player ) );

   game->state = GameState_Overworld;
   game->swapPortal = 0;
   game->overworldInactivitySeconds = 0.0f;
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
   if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
   {
      Game_ChangeState( game, GameState_Overworld );
   }
}

internal void Game_HandleOverworldScrollingDialogInput( Game_t* game )
{
   if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
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

internal void Game_UseHerb( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_Herb );
}

internal void Game_UseWing( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_Wing );
}

internal void Game_UseFairyWater( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_FairyWater );
}

internal void Game_UseSilverHarp( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_SilverHarp );
}

internal void Game_UseFairyFlute( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_FairyFlute );
}

internal void Game_UseGwaelynsLove( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_GwaelynsLove );
}

internal void Game_UseRainbowDrop( Game_t* game )
{
   Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
   ScrollingDialog_Load( &( game->scrollingDialog ), ScrollingDialogType_Overworld, DialogMessageId_Use_RainbowDrop );
}

internal void Game_Draw( Game_t* game )
{
   switch ( game->state )
   {
      case GameState_Overworld:
         Game_DrawOverworld( game );
         break;
      case GameState_Overworld_MainMenu:
      case GameState_Overworld_ItemMenu:
         Menu_Draw( &( game->menu ) );
         break;
      case GameState_Overworld_ScrollingDialog:
         ScrollingDialog_Draw( &( game->scrollingDialog ) );
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

   if ( game->state == GameState_Overworld && game->overworldInactivitySeconds > OVERWORLD_INACTIVE_STATUS_SECONDS )
   {
      Game_DrawOverworldQuickStatus( game );
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

internal void Game_DrawOverworldQuickStatus( Game_t* game )
{
   uint8_t lvl = Player_GetLevel( &( game->player ) );
   uint32_t memSize;
   char line[9];

   memSize = MATH_MINU32( (uint32_t)( strlen( game->player.name ) ), 4 );
   memcpy( line, game->player.name, sizeof( char ) * memSize );
   line[memSize] = '\0';
   Screen_DrawTextWindowWithTitle( &( game->screen ), 16, 16, 8, 12, line, COLOR_WHITE );

   sprintf( line, lvl < 10 ? "%s   %u" : "%s  %u", STRING_OVERWORLD_QUICKSTATS_LEVEL, lvl);
   Screen_DrawText( &( game->screen ), line, 24, 32, COLOR_WHITE );

   sprintf( line, game->player.stats.hitPoints < 10 ? "%s   %u" : game->player.stats.hitPoints < 100 ? "%s  %u" : "%s %u", STRING_OVERWORLD_QUICKSTATS_HP, game->player.stats.hitPoints );
   Screen_DrawText( &( game->screen ), line, 24, 48, COLOR_WHITE );

   sprintf( line, game->player.stats.magicPoints < 10 ? "%s   %u" : game->player.stats.magicPoints < 100 ? "%s  %u" : "%s %u", STRING_OVERWORLD_QUICKSTATS_MP, game->player.stats.magicPoints );
   Screen_DrawText( &( game->screen ), line, 24, 64, COLOR_WHITE );

   sprintf( line, game->player.gold < 10 ? "%s    %u" : game->player.gold < 100 ? "%s   %u" : game->player.gold < 1000 ? "%s  %u" : game->player.gold < 10000 ? "%s %u" : "%s%u", STRING_OVERWORLD_QUICKSTATS_GOLD, game->player.gold );
   Screen_DrawText( &( game->screen ), line, 24, 80, COLOR_WHITE );

   sprintf( line, game->player.experience < 10 ? "%s    %u" : game->player.experience < 100 ? "%s   %u" : game->player.experience < 1000 ? "%s  %u" : game->player.experience < 10000 ? "%s %u" : "%s%u", STRING_OVERWORLD_QUICKSTATS_EXP, game->player.experience );
   Screen_DrawText( &( game->screen ), line, 24, 96, COLOR_WHITE );
}

internal void Game_DrawOverworldDeepStatus( Game_t* game )
{
   Screen_DrawTextWindow( &( game->screen ), 80, 32, 20, 22, COLOR_WHITE );
   char line[18];

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_NAME, game->player.name );
   Screen_DrawText( &( game->screen ), line, 104 + ( ( 4 - ( (uint32_t)( ( strlen( game->player.name ) + 1 ) / 2 ) ) ) * TEXT_TILE_SIZE ), 40, COLOR_WHITE);

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_STRENGTH, game->player.stats.strength );
   Screen_DrawText( &( game->screen ), line, 96, 56, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_AGILITY, game->player.stats.agility );
   Screen_DrawText( &( game->screen ), line, 104, 72, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXHP, game->player.stats.maxHitPoints );
   Screen_DrawText( &( game->screen ), line, 112, 88, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXHP, game->player.stats.maxMagicPoints );
   Screen_DrawText( &( game->screen ), line, 112, 104, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_ATTACK, game->player.stats.attackPower );
   Screen_DrawText( &( game->screen ), line, 112, 120, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_DEFENSE, game->player.stats.defensePower );
   Screen_DrawText( &( game->screen ), line, 104, 136, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_WEAPON, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 96, 152, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_ARMOR, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 104, 168, COLOR_WHITE );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_SHIELD, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 96, 184, COLOR_WHITE );
}

internal void Game_DrawNonUseableItems( Game_t* game )
{
   uint32_t x, y;
   Player_t* player = &( game->player );
   uint32_t items = player->items;
   uint32_t itemCount = PLAYER_GET_MAPNONUSEABLEITEMCOUNT( items );
   char line[MENU_LINE_LENGTH];

   if ( game->state == GameState_Overworld_ItemMenu )
   {
      Screen_DrawTextWindow( &( game->screen ), 16, 120, 11, ( itemCount * 2 ) + 2, COLOR_WHITE );
      x = 24;
      y = 128;
   }
   else
   {
      Screen_DrawTextWindowWithTitle( &( game->screen ), 152, 48, 11, ( itemCount * 2 ) + 3, STRING_OVERWORLD_MENU_ITEM, COLOR_WHITE );
      x = 160;
      y = 64;
   }

   if ( PLAYER_GET_KEYCOUNT( items ) )
   {
      sprintf( line, STRING_OVERWORLD_ITEMMENU_KEY, PLAYER_GET_KEYCOUNT( items ) );
      Screen_DrawText( &( game->screen ), line, x, y, COLOR_WHITE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( PLAYER_HAS_TOKEN( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_TOKEN_1, x, y, COLOR_WHITE );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_TOKEN_2, x, y + 8, COLOR_WHITE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( PLAYER_HAS_STONEOFSUNLIGHT( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STONEOFSUNLIGHT_1, x, y, COLOR_WHITE );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STONEOFSUNLIGHT_2, x, y + TEXT_TILE_SIZE, COLOR_WHITE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( PLAYER_HAS_STAFFOFRAIN( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STAFFOFRAIN_1, x, y, COLOR_WHITE );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STAFFOFRAIN_2, x, y + TEXT_TILE_SIZE, COLOR_WHITE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( PLAYER_HAS_SPHEREOFLIGHT( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_SPHEREOFLIGHT_1, x, y, COLOR_WHITE );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_SPHEREOFLIGHT_2, x, y + TEXT_TILE_SIZE, COLOR_WHITE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
}
