#include "game.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleOverworldInput( Game_t* game );
internal void Game_HandleOverworldWaitingInput( Game_t* game );
internal void Game_HandleOverworldScrollingDialogInput( Game_t* game );
internal void Game_HandleMenuInput( Game_t* game );
internal void Game_OpenOverworldSpellMenu( Game_t* game );
internal void Game_OpenOverworldItemMenu( Game_t* game );
internal void Game_OpenZoomMenu( Game_t* game );

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
      case GameState_Overworld_SpellMenu:
      case GameState_Overworld_ItemMenu:
      case GameState_Overworld_ZoomMenu:
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
      Game_DrawOverworldQuickStatus( game );
      Game_OpenMenu( game, MenuId_Overworld );
   }
   else if ( leftIsDown || upIsDown || rightIsDown || downIsDown )
   {
      game->overworldInactivitySeconds = 0.0f;

      if ( leftIsDown && !rightIsDown )
      {
         player->velocity.x = -( player->maxVelocity ) * game->input.analogIntensity[Direction_Left];

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
         player->velocity.x = player->maxVelocity * game->input.analogIntensity[Direction_Right];

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
         player->velocity.y = -( player->maxVelocity )* game->input.analogIntensity[Direction_Up];

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
         player->velocity.y = player->maxVelocity* game->input.analogIntensity[Direction_Down];

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
      Game_StartAnimation( game, Animation_Overworld_Pause );
   }
}

internal void Game_HandleOverworldScrollingDialogInput( Game_t* game )
{
   uint32_t e;

   if ( ScrollingDialog_IsDone( &( game->scrollingDialog ) ) )
   {
      if ( Input_AnyButtonPressed( &( game->input ) ) )
      {
         if ( game->scrollingDialog.messageId == DialogMessageId_Use_RainbowDrop )
         {
            Game_StartAnimation( game, Animation_RainbowBridge_Trippy );
         }
         else
         {
            Game_StartAnimation( game, Animation_Overworld_Pause );
         }
      }
   }
   else if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
   {
      if ( ScrollingDialog_Next( &( game->scrollingDialog ) ) )
      {
         switch ( game->scrollingDialog.section )
         {
            case 1:
               switch ( game->scrollingDialog.messageId )
               {
                  case DialogMessageId_Use_Herb1:
                  case DialogMessageId_Use_Herb2:
                  case DialogMessageId_Spell_OverworldCastHeal1:
                  case DialogMessageId_Spell_OverworldCastHeal2:
                  case DialogMessageId_Spell_OverworldCastMidheal1:
                  case DialogMessageId_Spell_OverworldCastMidheal2:
                     Game_DrawOverworldQuickStatus( game );
                     break;
                  case DialogMessageId_Use_CursedBelt:
                  case DialogMessageId_Chest_DeathNecklace:
                     Player_SetCursed( &( game->player ), True );
                     TileMap_StartGlowTransition( &( game->tileMap ) );
                     break;
                  case DialogMessageId_Use_GwaelynsLove:
                     e = Player_GetExperienceRemaining( &( game->player ) );
                     if ( e == 0 )
                     {
                        ScrollingDialog_Skip( &( game->scrollingDialog ) );
                     }
                     break;
               }
               break;
         }
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
         case MenuCommand_OverworldMain_Talk: Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Talk_NobodyThere ); break;
         case MenuCommand_OverworldMain_Status:
            Game_DrawOverworldDeepStatus( game );
            Game_ChangeState( game, GameState_Overworld_Waiting );
            break;
         case MenuCommand_OverworldMain_Search: Game_Search( game ); break;
         case MenuCommand_OverworldMain_Spell: Game_OpenOverworldSpellMenu( game ); break;
         case MenuCommand_OverworldMain_Item: Game_OpenOverworldItemMenu( game ); break;
         case MenuCommand_OverworldMain_Door: Game_OpenDoor( game ); break;

         case MenuCommand_Spell_Heal: Game_CastHeal( game ); break;
         case MenuCommand_Spell_Glow: Game_CastGlow( game ); break;
         case MenuCommand_Spell_Evac: Game_CastEvac( game ); break;
         case MenuCommand_Spell_Zoom: Game_OpenZoomMenu( game ); break;
         case MenuCommand_Spell_Repel: Game_CastRepel( game ); break;
         case MenuCommand_Spell_Midheal: Game_CastMidheal( game ); break;

         case MenuCommand_Item_Herb: Game_UseHerb( game ); break;
         case MenuCommand_Item_Wing: Game_UseWing( game ); break;
         case MenuCommand_Item_FairyWater: Game_UseFairyWater( game ); break;
         case MenuCommand_Item_Torch: Game_UseTorch( game ); break;
         case MenuCommand_Item_SilverHarp: Game_UseSilverHarp( game ); break;
         case MenuCommand_Item_FairyFlute: Game_UseFairyFlute( game ); break;
         case MenuCommand_Item_GwaelynsLove: Game_UseGwaelynsLove( game ); break;
         case MenuCommand_Item_RainbowDrop: Game_UseRainbowDrop( game ); break;
         case MenuCommand_Item_CursedBelt: Game_UseCursedBelt( game ); break;

         case MenuCommand_Zoom_Tantegel: Game_CastZoom( game, TILEMAP_TANTEGEL_TOWN_ID ); break;
         case MenuCommand_Zoom_Brecconary: Game_CastZoom( game, TILEMAP_BRECCONARY_TOWN_ID ); break;
         case MenuCommand_Zoom_Garinham: Game_CastZoom( game, TILEMAP_GARINHAM_TOWN_ID ); break;
         case MenuCommand_Zoom_Kol: Game_CastZoom( game, TILEMAP_KOL_TOWN_ID ); break;
         case MenuCommand_Zoom_Cantlin: Game_CastZoom( game, TILEMAP_CANTLIN_TOWN_ID ); break;
         case MenuCommand_Zoom_Rimuldar: Game_CastZoom( game, TILEMAP_RIMULDAR_TOWN_ID ); break;
      }
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      // TODO: some of these should probably return to their parent menus
      switch ( game->state )
      {
         case GameState_Overworld_MainMenu:
         case GameState_Overworld_SpellMenu:
         case GameState_Overworld_ItemMenu:
         case GameState_Overworld_ZoomMenu:
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

internal void Game_OpenOverworldSpellMenu( Game_t* game )
{
   if ( !game->player.spells )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_None );
   }
   else if ( SPELL_GET_MAPUSEABLECOUNT( game->player.spells, game->tileMap.isDungeon, game->tileMap.isDark ) )
   {
      Game_OpenMenu( game, MenuId_OverworldSpell );
   }
   else
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_OverworldCantCast );
   }
}

internal void Game_OpenOverworldItemMenu( Game_t* game )
{
   uint32_t useableCount = ITEM_GET_MAPUSEABLECOUNT( game->player.items );
   uint32_t nonUseableCount = ITEM_GET_MAPNONUSEABLECOUNT( game->player.items );

   if ( useableCount == 0 && nonUseableCount == 0 )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Item_None );
   }
   else
   {
      if ( nonUseableCount > 0 )
      {
         Game_DrawNonUseableItems( game, ( useableCount > 0 ) ? True : False );
      }

      if ( useableCount > 0 )
      {
         Game_OpenMenu( game, MenuId_OverworldItem );
      }
      else
      {
         Game_ChangeState( game, GameState_Overworld_Waiting );
      }
   }
}

internal void Game_OpenZoomMenu( Game_t* game )
{
   uint32_t townCount = HAS_VISITED_COUNT( game->player.townsVisited );

   if ( game->player.stats.magicPoints < SPELL_ZOOM_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else if ( townCount > 0 )
   {
      Game_OpenMenu( game, MenuId_Zoom );
   }
}