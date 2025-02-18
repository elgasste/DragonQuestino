#include "game.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleOverworldInput( Game_t* game );
internal void Game_HandleOverworldWaitingInput( Game_t* game );
internal void Game_HandleOverworldScrollingDialogInput( Game_t* game );
internal void Game_HandleMenuInput( Game_t* game );
internal void Game_OpenOverworldItemMenu( Game_t* game );

void Game_HandleInput( Game_t* game )
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
            Game_Search( game );
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

