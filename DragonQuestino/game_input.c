#include "game.h"

#define DIAGONAL_SCALAR    0.707f

internal void Game_HandleStartupMenuInput( Game_t* game );
internal void Game_HandleOverworldInput( Game_t* game );
internal void Game_HandleOverworldWaitingInput( Game_t* game );
internal void Game_HandleOverworldDialogInput( Game_t* game );
internal void Game_HandleOverworldMenuInput( Game_t* game );
internal void Game_ReturnToOverworldWithPause( Game_t* game );
internal void Game_OpenOverworldSpellMenu( Game_t* game );
internal void Game_OpenOverworldItemMenu( Game_t* game );
internal void Game_OpenZoomMenu( Game_t* game );
internal void Game_HandleBattleMenuInput( Game_t* game );
internal void Game_HandleBattleDialogInput( Game_t* game );
internal void Game_OpenBattleSpellMenu( Game_t* game );
internal void Game_OpenBattleItemMenu( Game_t* game );
internal void Game_HandleEnterNameInput( Game_t* game );
internal void Game_HandleEnterPasswordInput( Game_t* game );
internal void Game_HandleOverworldBinaryChoiceInput( Game_t* game );
internal void Game_HandleOverworldShopMenuInput( Game_t* game );

void Game_HandleInput( Game_t* game )
{
   switch ( game->mainState )
   {
      case MainState_Startup:
         Game_HandleStartupMenuInput( game );
         break;
      case MainState_EnterName:
         Game_HandleEnterNameInput( game );
         break;
      case MainState_EnterPassword:
         Game_HandleEnterPasswordInput( game );
         break;
      case MainState_Overworld:
         switch ( game->subState )
         {
            case SubState_None:
               Game_HandleOverworldInput( game );
               break;
            case SubState_NonUseableItems:
            case SubState_Status:
               Game_HandleOverworldWaitingInput( game );
               break;
            case SubState_Menu:
               Game_HandleOverworldMenuInput( game );
               break;
            case SubState_Dialog:
               Game_HandleOverworldDialogInput( game );
               break;
            case SubState_BinaryChoice:
               Game_HandleOverworldBinaryChoiceInput( game );
               break;
            case SubState_ShopMenu:
               Game_HandleOverworldShopMenuInput( game );
               break;
         }
         break;
      case MainState_Battle:
         switch ( game->subState )
         {
            case SubState_Menu:
               Game_HandleBattleMenuInput( game );
               break;
            case SubState_Dialog:
               Game_HandleBattleDialogInput( game );
               break;
         }
         break;
   }
}

internal void Game_HandleStartupMenuInput( Game_t* game )
{
   uint32_t i;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      Menu_ResetCarat( game->activeMenu );

      switch ( game->activeMenu->items[game->activeMenu->selectedIndex].command )
      {
         case MenuCommand_Startup_NewGame:
            Game_ChangeToEnterNameState( game );
            break;
         case MenuCommand_Startup_EnterPassword:
            Game_ChangeToEnterPasswordState( game );
            break;
      }
   }
   else
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
         player->velocity.x = -( player->maxVelocity );

         if ( !( upIsDown && playerSprite->direction == Direction_Up ) &&
              !( downIsDown && playerSprite->direction == Direction_Down ) )
         {
            ActiveSprite_SetDirection( playerSprite, Direction_Left );
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
            ActiveSprite_SetDirection( playerSprite, Direction_Right );
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
            ActiveSprite_SetDirection( playerSprite, Direction_Up );
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
            ActiveSprite_SetDirection( playerSprite, Direction_Down );
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
      Game_ReturnToOverworldWithPause( game );
   }
}

internal void Game_HandleOverworldDialogInput( Game_t* game )
{
   if ( Dialog_IsDone( &( game->dialog ) ) )
   {
      if ( Input_AnyButtonPressed( &( game->input ) ) )
      {
         Game_ReturnToOverworldWithPause( game );
      }
   }
   else if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
   {
      Dialog_StepAhead( &( game->dialog ) );
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
         case MenuCommand_Overworld_Talk: Game_Talk( game ); break;
         case MenuCommand_Overworld_Status: Game_ChangeSubState( game, SubState_Status ); break;
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
            break;
         case MenuId_Zoom:
            game->activeMenu = &( game->menus[MenuId_OverworldSpell] );
            break;
         default:
            Game_ChangeToOverworldState( game );
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

internal void Game_ReturnToOverworldWithPause( Game_t* game )
{
   Game_ChangeToOverworldState( game );
   Game_DrawOverworld( game );
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_OpenOverworldSpellMenu( Game_t* game )
{
   if ( !game->player.spells )
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_NO_SPELLS );
      Game_OpenDialog( game );
   }
   else if ( SPELL_GET_MAPUSEABLECOUNT( game->player.spells, game->tileMap.isDungeon, game->tileMap.isDark ) )
   {
      Game_OpenMenu( game, MenuId_OverworldSpell );
   }
   else
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_SPELLS_OVERWORLD_CANT_CAST );
      Game_OpenDialog( game );
   }
}

internal void Game_OpenOverworldItemMenu( Game_t* game )
{
   if ( ITEM_GET_MAPUSEABLECOUNT( game->player.items ) == 0 && ITEM_GET_MAPNONUSEABLECOUNT( game->player.items ) == 0 )
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSection( &( game->dialog ), STRING_DIALOG_NO_ITEMS );
      Game_OpenDialog( game );
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
      Game_CastZoom( game, 0 );
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
         case MenuCommand_Battle_Attack: Battle_Attack( &( game->battle ) ); break;
         case MenuCommand_Battle_Flee: Battle_Flee( &( game->battle ) ); break;
         case MenuCommand_Battle_Spell: Game_OpenBattleSpellMenu( game ); break;
         case MenuCommand_Battle_Item: Game_OpenBattleItemMenu( game ); break;

         case MenuCommand_Spell_Heal: Game_CastHeal( game ); break;
         case MenuCommand_Spell_Sizz: Game_CastSizz( game ); break;
         case MenuCommand_Spell_Sleep: Game_CastSleep( game ); break;
         case MenuCommand_Spell_Fizzle: Game_CastFizzle( game ); break;
         case MenuCommand_Spell_Midheal: Game_CastMidheal( game ); break;
         case MenuCommand_Spell_Sizzle: Game_CastSizzle( game ); break;

         case MenuCommand_Item_Herb: Game_UseHerb( game ); break;
         case MenuCommand_Item_FairyFlute: Game_UseFairyFlute( game ); break;
      }
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      switch ( game->activeMenu->id )
      {
         case MenuId_BattleSpell:
         case MenuId_BattleItem:
            game->activeMenu = &( game->menus[MenuId_Battle] );
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

internal void Game_HandleBattleDialogInput( Game_t* game )
{
   if ( Dialog_IsDone( &( game->dialog ) ) )
   {
      if ( Input_AnyButtonPressed( &( game->input ) ) )
      {
         if ( game->battle.isOver )
         {
            Game_ReturnToOverworldWithPause( game );
         }
      }
   }
   else if ( game->input.buttonStates[Button_A].pressed || game->input.buttonStates[Button_B].pressed )
   {
      Dialog_StepAhead( &( game->dialog ) );
   }
}

internal void Game_OpenBattleSpellMenu( Game_t* game )
{
   if ( !game->player.spells )
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_BATTLE_NOSPELLS, Game_ResetBattleMenu, game );
      Game_OpenDialog( game );
   }
   else if ( SPELL_GET_BATTLEUSEABLECOUNT( game->player.spells ) )
   {
      Game_OpenMenu( game, MenuId_BattleSpell );
   }
   else
   {
      // this is impossible in normal gameplay, but we'll account for it anyway
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_BATTLE_CANTCASTSPELL, Game_ResetBattleMenu, game );
      Game_OpenDialog( game );
   }
}

internal void Game_OpenBattleItemMenu( Game_t* game )
{
   if ( !game->player.items )
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_BATTLE_NOITEMS, Game_ResetBattleMenu, game );
      Game_OpenDialog( game );
   }
   else if ( ITEM_GET_BATTLEUSEABLECOUNT( game->player.items ) )
   {
      Game_OpenMenu( game, MenuId_BattleItem );
   }
   else
   {
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_BATTLE_CANTUSEITEM, Game_ResetBattleMenu, game );
      Game_OpenDialog( game );
   }
}

internal void Game_HandleEnterNameInput( Game_t* game )
{
   uint32_t i;
   char name[9];
   size_t length = strlen( game->player.name );

   strcpy( name, game->player.name );

   if ( game->input.buttonStates[Button_Start].pressed )
   {
      if ( length > 0 )
      {
         Game_Load( game, "" );
      }
   }
   if ( game->input.buttonStates[Button_A].pressed )
   {
      if ( game->alphaPicker.selectedIndex == 64 )
      {
         if ( length > 0 )
         {
            Game_Load( game, "" );
         }
      }
      else if ( length < 8 )
      {
         name[length] = AlphaPicker_GetSelectedChar( &( game->alphaPicker ) );
         name[length + 1] = 0;
         Player_SetName( &( game->player ), name );
         AlphaPicker_ResetCarat( &( game->alphaPicker ) );
      }
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      if ( length > 0 )
      {
         name[length - 1] = 0;
         Player_SetName( &( game->player ), name );
      }
      else
      {
         Game_ChangeToStartupState( game );
      }
   }
   else
   {
      for ( i = 0; i < Direction_Count; i++ )
      {
         if ( game->input.buttonStates[i].pressed )
         {
            AlphaPicker_MoveSelection( &( game->alphaPicker ), (Direction_t)i );
         }
      }
   }
}

internal void Game_HandleEnterPasswordInput( Game_t* game )
{
   uint32_t i;
   char* password = game->password;
   size_t length = strlen( password );

   if ( game->input.buttonStates[Button_Start].pressed )
   {
      if ( length == PASSWORD_LENGTH )
      {
         Game_Load( game, password );
      }
   }
   if ( game->input.buttonStates[Button_A].pressed )
   {
      if ( game->alphaPicker.selectedIndex == 64 )
      {
         if ( length == PASSWORD_LENGTH )
         {
            Game_Load( game, password );
         }
      }
      else if ( length < PASSWORD_LENGTH )
      {
         password[length] = AlphaPicker_GetSelectedChar( &( game->alphaPicker ) );
         password[length + 1] = 0;
         AlphaPicker_ResetCarat( &( game->alphaPicker ) );
      }
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      if ( length > 0 )
      {
         password[length - 1] = 0;
      }
      else
      {
         Game_ChangeToStartupState( game );
      }
   }
   else
   {
      for ( i = 0; i < Direction_Count; i++ )
      {
         if ( game->input.buttonStates[i].pressed )
         {
            AlphaPicker_MoveSelection( &( game->alphaPicker ), (Direction_t)i );
         }
      }
   }
}

internal void Game_HandleOverworldBinaryChoiceInput( Game_t* game )
{
   uint32_t i;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      BinaryPicker_ResetCarat( &( game->binaryPicker ) );
      BinaryPicker_Select( &( game->binaryPicker ) );
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      BinaryPicker_Select2( &( game->binaryPicker ) );
   }
   else
   {
      for ( i = 0; i < Direction_Count; i++ )
      {
         if ( game->input.buttonStates[i].pressed )
         {
            BinaryPicker_MoveSelection( &( game->binaryPicker ), (Direction_t)i );
         }
      }
   }
}

internal void Game_HandleOverworldShopMenuInput( Game_t* game )
{
   uint32_t i;

   if ( game->input.buttonStates[Button_A].pressed )
   {
      ShopPicker_ResetCarat( &( game->shopPicker ) );
      Game_SelectShopItem( game );
   }
   else if ( game->input.buttonStates[Button_B].pressed )
   {
      Dialog_Reset( &( game->dialog ) );

      switch ( game->tileMap.shopType )
      {
         case ShopType_Weapon:
            Dialog_PushSection( &( game->dialog ), STRING_WEAPONSHOP_LEAVE );
            break;
         case ShopType_Item:
            Dialog_PushSection( &( game->dialog ), STRING_ITEMSHOP_LEAVE );
            break;
      }

      Game_OpenDialog( game );
   }
   else
   {
      for ( i = 0; i < Direction_Count; i++ )
      {
         if ( game->input.buttonStates[i].pressed )
         {
            ShopPicker_MoveSelection( &( game->shopPicker ), (Direction_t)i );
         }
      }
   }
}
