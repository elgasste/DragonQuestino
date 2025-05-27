#include "game.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleOverworldInput( Game_t* game );
internal void Game_HandleOverworldWaitingInput( Game_t* game );
internal void Game_HandleOverworldDialogInput( Game_t* game );
internal void Game_HandleOverworldMenuInput( Game_t* game );
internal void Game_OpenOverworldSpellMenu( Game_t* game );
internal void Game_OpenOverworldItemMenu( Game_t* game );
internal void Game_OpenZoomMenu( Game_t* game );
internal void Game_HandleBattleMenuInput( Game_t* game );
internal void Game_HandleBattleDialogInput( Game_t* game );

void Game_HandleInput( Game_t* game )
{
   if ( game->mainState == MainState_Overworld )
   {
      switch ( game->subState )
      {
         case SubState_None:
            Game_HandleOverworldInput( game );
            break;
         case SubState_Waiting:
            Game_HandleOverworldWaitingInput( game );
            break;
         case SubState_Menu:
            Game_HandleOverworldMenuInput( game );
            break;
         case SubState_Dialog:
            Game_HandleOverworldDialogInput( game );
            break;
      }
   }
   else if ( game->mainState == MainState_Battle )
   {
      switch ( game->subState )
      {
         case SubState_Menu:
            Game_HandleBattleMenuInput( game );
            break;
         case SubState_Dialog:
            Game_HandleBattleDialogInput( game );
            break;
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
      Game_DrawQuickStatus( game );
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
      Animation_Start( &( game->animation ), AnimationId_Overworld_Pause);
   }
}

internal void Game_HandleOverworldDialogInput( Game_t* game )
{
   uint32_t e;

   if ( Dialog_IsDone( &( game->dialog ) ) )
   {
      if ( Input_AnyButtonPressed( &( game->input ) ) )
      {
         if ( game->dialog.id == DialogId_Use_RainbowDrop )
         {
            Animation_Start( &( game->animation ), AnimationId_RainbowBridge_Trippy );
         }
         else
         {
            Animation_Start( &( game->animation ), AnimationId_Overworld_Pause );
         }
      }
   }
   else if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
   {
      if ( Dialog_StepAhead( &( game->dialog ) ) )
      {
         switch ( game->dialog.section )
         {
            case 1:
               switch ( game->dialog.id )
               {
                  case DialogId_Use_Herb1:
                  case DialogId_Use_Herb2:
                  case DialogId_Spell_OverworldCastHeal1:
                  case DialogId_Spell_OverworldCastHeal2:
                  case DialogId_Spell_OverworldCastMidheal1:
                  case DialogId_Spell_OverworldCastMidheal2:
                     Player_RestoreHitPoints( &( game->player ), game->pendingPayload8u );
                     Game_DrawQuickStatus( game );
                     break;
                  case DialogId_Use_CursedBelt:
                  case DialogId_Chest_DeathNecklace:
                     Player_SetCursed( &( game->player ), True );
                     TileMap_StartGlowTransition( &( game->tileMap ) );
                     game->screen.needsRedraw = True;
                     break;
                  case DialogId_Use_GwaelynsLove:
                     e = Player_GetExperienceRemaining( &( game->player ) );
                     if ( e == 0 )
                     {
                        Dialog_NextSection( &( game->dialog ) );
                     }
                     break;
               }
               break;
         }
      }
   }
}

internal void Game_HandleOverworldMenuInput( Game_t* game )
{
   uint32_t i;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      Menu_ResetCarat( game->activeMenu );

      switch ( game->activeMenu->items[game->activeMenu->selectedIndex].command )
      {
         case MenuCommand_Overworld_Talk: Game_OpenDialog( game, DialogId_Talk_NobodyThere ); break;
         case MenuCommand_Overworld_Status:
            Game_DrawOverworldDeepStatus( game );
            Game_ChangeSubState( game, SubState_Waiting );
            break;
         case MenuCommand_Overworld_Search: Game_Search( game ); break;
         case MenuCommand_Overworld_Spell: Game_OpenOverworldSpellMenu( game ); break;
         case MenuCommand_Overworld_Item: Game_OpenOverworldItemMenu( game ); break;
         case MenuCommand_Overworld_Door: Game_OpenDoor( game ); break;

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
      switch ( game->activeMenu->id )
      {
         case MenuId_OverworldItem:
         case MenuId_OverworldSpell:
            game->activeMenu = &( game->menus[MenuId_Overworld] );
            game->screen.needsRedraw = True;
            break;
         default:
            Game_ChangeMainState( game, MainState_Overworld );
            break;
      }

   }
   else if ( game->activeMenu->itemCount )
   {
      for ( i = 0; i < Direction_Count; i++ )
      {
         if ( game->input.buttonStates[i].pressed )
         {
            Menu_MoveSelection( game->activeMenu, (Direction_t)i );
         }
      }
   }
}

internal void Game_OpenOverworldSpellMenu( Game_t* game )
{
   if ( !game->player.spells )
   {
      Game_OpenDialog( game, DialogId_Spell_None );
   }
   else if ( SPELL_GET_MAPUSEABLECOUNT( game->player.spells, game->tileMap.isDungeon, game->tileMap.isDark ) )
   {
      Game_OpenMenu( game, MenuId_OverworldSpell );
   }
   else
   {
      Game_OpenDialog( game, DialogId_Spell_OverworldCantCast );
   }
}

internal void Game_OpenOverworldItemMenu( Game_t* game )
{
   if ( ITEM_GET_MAPUSEABLECOUNT( game->player.items ) == 0 && ITEM_GET_MAPNONUSEABLECOUNT( game->player.items ) == 0 )
   {
      Game_OpenDialog( game, DialogId_Item_None );
   }
   else
   {
      Game_OpenMenu( game, MenuId_OverworldItem );
      Game_DrawOverworldItemMenu( game );
   }
}

internal void Game_OpenZoomMenu( Game_t* game )
{
   uint32_t townCount = HAS_VISITED_COUNT( game->player.townsVisited );

   if ( game->player.stats.magicPoints < SPELL_ZOOM_MP )
   {
      Game_OpenDialog( game, DialogId_Spell_NotEnoughMp );
   }
   else if ( townCount > 0 )
   {
      Game_OpenMenu( game, MenuId_Zoom );
   }
}

internal void Game_HandleBattleMenuInput( Game_t* game )
{
   uint32_t i;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      Menu_ResetCarat( game->activeMenu );

      switch ( game->activeMenu->items[game->activeMenu->selectedIndex].command )
      {
         case MenuCommand_Battle_Attack: Battle_AttemptAttack( &( game->battle ) ); break;
         case MenuCommand_Battle_Flee: Battle_AttemptFlee( &( game->battle ) ); break;
         case MenuCommand_Battle_Spell: Game_ChangeMainState( game, MainState_Overworld ); break;
         case MenuCommand_Battle_Item: Game_ChangeMainState( game, MainState_Overworld ); break;
      }
   }
   else if ( game->activeMenu->itemCount )
   {
      for ( i = 0; i < Direction_Count; i++ )
      {
         if ( game->input.buttonStates[i].pressed )
         {
            Menu_MoveSelection( game->activeMenu, (Direction_t)i );
         }
      }
   }
}

internal void Game_HandleBattleDialogInput( Game_t* game )
{
   if ( Dialog_IsDone( &( game->dialog ) ) )
   {
      if ( Input_AnyButtonPressed( &( game->input ) ) )
      {
         switch ( game->dialog.id )
         {
            case DialogId_Battle_FleeAttemptSucceeded:
               Game_DrawOverworld( game );
               Animation_Start( &( game->animation ), AnimationId_Overworld_Pause );
               break;
            case DialogId_Battle_AttackAttemptSucceeded:
            case DialogId_Battle_AttackAttemptFailed:
            case DialogId_Battle_FleeAttemptFailed:
               Game_ChangeSubState( game, SubState_Menu );
               break;
         }
      }
   }
   else if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
   {
      Dialog_StepAhead( &( game->dialog ) );
   }
}
