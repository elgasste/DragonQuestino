#include "game.h"
#include "random.h"
#include "math.h"
#include "tables.h"

internal void Game_BattleIntroMessageCallback( Game_t* game );
internal void Game_TicActiveSprites( Game_t* game );
internal void Game_DeathFadeOutCallback( Game_t* game );
internal void Game_DeathPostFadeCallback( Game_t* game );
internal void Game_CursedExpelCallback( Game_t* game );
internal void Game_CursedExpelMessageCallback( Game_t* game );
internal void Game_ResetTitleScreenFlash( Game_t* game );
internal void Game_TicTitleScreenFlash( Game_t* game );
internal void Game_KingQuestionCallback( Game_t* game );
internal void Game_KingQuestionPostDialogCallback( Game_t* game );
internal void Game_KingQuestionPauseCallback( Game_t* game );
internal void Game_GwaelinAccompanyCallback( Game_t* game );
internal void Game_GwaelinAccompanyPostDialogCallback( Game_t* game );
internal void Game_GwaelinAccompanyPauseCallback( Game_t* game );
internal void Game_QueenGwaelinCallback( Game_t* game );
internal void Game_QueenGwaelinPostDialogCallback( Game_t* game );
internal void Game_HappyEndingCallback( Game_t* game );
internal void Game_GoFindGwaelinCallback( Game_t* game );
internal void Game_GoFindGwaelinPostDialogCallback( Game_t* game );
internal void Game_GoFindGwaelinPostDialogPauseCallback( Game_t* game );
internal void Game_StartPostIntroFadeIn( Game_t* game );
internal void Game_PostIntroPauseCallback( Game_t* game );
internal void Game_TitleScreenFadeInCallback( Game_t* game );
internal void Game_EnterNameFadeOutCallback( Game_t* game );
internal void Game_EnterPasswordFadeOutCallback( Game_t* game );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   u32 i;

   Random_Seed();
   Screen_Init( &( game->screen ), screenBuffer );
   Screen_LoadBattleBackgroundTileTextures( &( game->screen ) );
   TileMap_Init( &( game->tileMap ), &( game->screen ), &( game->gameFlags ), &( game->player ) );
   AnimationChain_Init( &( game->animationChain ), &( game->screen ), &( game->tileMap ), game );
   Sprite_LoadActive( &( game->player.sprite ), ACTIVE_SPRITE_PLAYER_ID );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ), &( game->tileMap ) );
   Battle_Init( &( game->battle ), game );
   Game_UpdateTextColor( game );

   for ( i = 0; i < MenuId_Count; i++ )
   {
      Menu_Init( &( game->menus[(MenuId_t)i] ), (MenuId_t)( i ), &( game->screen ), &( game->player ), &( game->tileMap ) );
   }

   AlphaPicker_Init( &( game->alphaPicker ), &( game->screen ) );
   BinaryPicker_Init( &( game->binaryPicker ), &( game->screen ) );
   ShopPicker_Init( &( game->shopPicker ), &( game->screen ), &( game->tileMap ) );
   Dialog_Init( &( game->dialog ), &( game->screen ), &( game->mainState ) );

   for ( i = 0; i < TILEMAP_TOWN_COUNT; i++ )
   {
      game->zoomPortals[i].destinationTileMapIndex = TILEMAP_OVERWORLD_ID;
      game->zoomPortals[i].arrivalDirection = Direction_Down;
   }

   game->zoomPortals[TILEMAP_TANTEGEL_TOWN_ID].destinationTileIndex = TILEMAP_TANTEGEL_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_BRECCONARY_TOWN_ID].destinationTileIndex = TILEMAP_BRECCONARY_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_GARINHAM_TOWN_ID].destinationTileIndex = TILEMAP_GARINHAM_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_KOL_TOWN_ID].destinationTileIndex = TILEMAP_KOL_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_CANTLIN_TOWN_ID].destinationTileIndex = TILEMAP_CANTLIN_ZOOM_INDEX;
   game->zoomPortals[TILEMAP_RIMULDAR_TOWN_ID].destinationTileIndex = TILEMAP_RIMULDAR_ZOOM_INDEX;

   Game_Reset( game );
}

void Game_Reset( Game_t* game )
{
   Player_t* player = &( game->player );

   game->postRenderCallback = 0;
   game->activeMenu = 0;
   game->mainState = MainState_Startup;
   game->subState = SubState_None;
   game->doAnimation = False;
   game->targetPortal = 0;
   game->overworldInactivitySeconds = 0.0f;

   Player_SetName( player, "" );
   player->sprite.position.x = (r32)( TILE_SIZE * 8 ) + 2.0f;
   player->sprite.position.y = (r32)( TILE_SIZE * 6 ) + 4.0f;
   player->sprite.direction = Direction_Up;
   player->velocity.x = 0.0f;
   player->velocity.y = 0.0f;
   player->sprite.isFlickering = False;
   player->hasHolyProtection = False;
   player->holyProtectionSteps = 0;
   player->townsVisited = 0;
   player->experience = 0;
   player->level = 0;
   player->gold = 0;
   player->items = 0;
   player->spells = 0;
   player->stats.sleepResist = 0;
   player->stats.stopSpellResist = 0;
   player->stats.hurtResist = 0;
   player->stats.dodge = 1;
   player->isCursed = False;

   Player_LoadWeapon( player, WEAPON_NONE_ID );
   Player_LoadArmor( player, ARMOR_NONE_ID );
   Player_LoadShield( player, SHIELD_NONE_ID );

   TileMap_LoadTextures( &( game->tileMap ), TileTextureType_Title );
   TileMap_Load( &( game->tileMap ), TILEMAP_TITLESCREEN_ID );

   Screen_BackupPaletteAndWipeColor( &( game->screen ), COLOR_BLACK );

   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_MidFadeIn );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause);
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause);
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_TitleScreenFadeInCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

void Game_Load( Game_t* game, const char* password )
{
   Player_t* player = &( game->player );

   if ( strlen( password ) > 0 )
   {
      if ( Game_LoadFromPassword( game, password ) )
      {
         game->gameFlags.leftThroneRoom = True;
      }
      else
      {
         return;
      }
   }
   else
   {
      game->gameFlags.treasures = 0xFFFFFFFF;
      game->gameFlags.doors = 0xFFFFFFFF;
      game->gameFlags.specialEnemies = 0xFF;
      game->gameFlags.leftThroneRoom = False;
      game->gameFlags.gotStaffOfRain = False;
      game->gameFlags.gotRainbowDrop = False;
      game->gameFlags.usedRainbowDrop = False;
      game->gameFlags.foundHiddenStairs = False;
      game->gameFlags.rescuedPrincess = False;
      game->gameFlags.carryingPrincess = False;
      game->gameFlags.joinedDragonlord = False;
      game->gameFlags.defeatedDragonlord = False;
   }

   player->stats.strength = Player_GetStrengthFromLevel( player, player->level );
   player->stats.agility = Player_GetAgilityFromLevel( player, player->level );
   player->stats.maxHitPoints = Player_GetMaxHitPointsFromLevel( player, player->level );
   player->stats.hitPoints = player->stats.maxHitPoints;
   player->stats.maxMagicPoints = Player_GetMaxMagicPointsFromLevel( player, player->level );
   player->stats.magicPoints = player->stats.maxMagicPoints;

   TileMap_LoadTextures( &( game->tileMap ), TileTextureType_Map );
   TileMap_Load( &( game->tileMap ), TILEMAP_TANTEGEL_THRONEROOM_ID );
   game->tileMap.previousId = game->tileMap.id;
   game->tileMap.lastPortalTileIndex = 0;
   player->tileIndex = 128; // in front of King Lorik
   Player_SetCanonicalTileIndex( player );

   game->subState = SubState_None;

   Screen_BackupPaletteAndWipeColor( &( game->screen ), COLOR_BLACK );
   Game_StartPostIntroFadeIn( game );
}

void Game_Tic( Game_t* game )
{
   Bool_t runningCallback;

   game->doAnimation = game->animationChain.isRunning;
   Input_Read( &( game->input ) );

   runningCallback = ( game->animationChain.pendingCallback != 0 ) || ( game->dialog.pendingCallback != 0 );

   if ( game->animationChain.pendingCallback )
   {
      game->animationChain.pendingCallback( game->animationChain.pendingCallbackData );
      game->animationChain.pendingCallback = 0;
   }
   else if ( game->dialog.pendingCallback )
   {
      game->dialog.pendingCallback( game->dialog.pendingCallbackData );
      game->dialog.pendingCallback = 0;
   }

   if ( game->animationChain.isRunning )
   {
      AnimationChain_Tic( &( game->animationChain ) );

      if ( ( ( game->animationChain.animationIds[game->animationChain.activeAnimation] == AnimationId_ActivePause ) ||
             ( game->animationChain.animationIds[game->animationChain.activeAnimation] == AnimationId_Ending_WalkFade ) ||
             ( game->animationChain.animationIds[game->animationChain.activeAnimation] == AnimationId_ActiveMidFadeIn ) ) &&
           game->mainState == MainState_Overworld )
      {
         Game_TicPhysics( game );
         Game_TicActiveSprites( game );
      }
   }
   else if ( !runningCallback )
   {
      Game_HandleInput( game );

      switch ( game->mainState )
      {
         case MainState_Startup:
            Game_TicTitleScreenFlash( game );
            switch ( game->subState )
            {
               case SubState_Menu:
                  Menu_Tic( game->activeMenu );
                  break;
            }
            break;
         case MainState_EnterName:
         case MainState_EnterPassword:
               AlphaPicker_Tic( &( game->alphaPicker ) );
               break;
         case MainState_Overworld:
            switch ( game->subState )
            {
               case SubState_None:
                  Game_TicPhysics( game );
                  break;
               case SubState_Menu:
                  Menu_Tic( game->activeMenu );
                  break;
               case SubState_Dialog:
                  Dialog_Tic( &( game->dialog ) );
                  break;
               case SubState_BinaryChoice:
                  BinaryPicker_Tic( &( game->binaryPicker ) );
                  break;
               case SubState_ShopMenu:
                  ShopPicker_Tic( &( game->shopPicker ) );
                  break;
            }
            TileMap_Tic( &( game->tileMap ) );
            Game_TicActiveSprites( game );
            break;
         case MainState_Battle:
            switch ( game->subState )
            {
               case SubState_Menu:
                  Menu_Tic( game->activeMenu );
                  break;
               case SubState_Dialog:
                  Dialog_Tic( &( game->dialog ) );
                  break;
            }
            break;
      }
   }

#if defined( VISUAL_STUDIO_DEV )
   if ( game->tileMap.isDark )
   {
      if ( g_debugFlags.noDark || game->gameFlags.defeatedDragonlord )
      {
         TileMap_ChangeViewportSize( &( game->tileMap ), SCREEN_WIDTH, SCREEN_HEIGHT );
      }
      else
      {
         TileMap_ChangeViewportSize( &( game->tileMap ),
                                     TILE_SIZE * (u16)( game->tileMap.glowDiameter ),
                                     TILE_SIZE * (u16)( game->tileMap.glowDiameter ) );
      }
   }
#endif

   Game_Draw( game );
   Screen_RenderBuffer( &( game->screen ) );

   if ( game->postRenderCallback )
   {
      game->postRenderCallback( game->postRenderCallbackData );
      game->postRenderCallback = 0;
   }
}

void Game_ChangeToOverworldState( Game_t* game )
{
   game->mainState = MainState_Overworld;
   game->subState = SubState_None;
   game->overworldInactivitySeconds = 0.0f;
}

void Game_ChangeToStartupState( Game_t* game )
{
   game->mainState = MainState_Startup;
   Screen_WipeColor( &( game->screen ), COLOR_BLACK );
}

void Game_ChangeToEnterNameState( Game_t* game )
{
   game->subState = SubState_None;

   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeOut );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_EnterNameFadeOutCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

void Game_ChangeToEnterPasswordState( Game_t* game )
{
   game->subState = SubState_None;

   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeOut );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_EnterPasswordFadeOutCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

void Game_ChangeToBattleState( Game_t* game )
{
   game->mainState = MainState_Battle;
   game->subState = SubState_None;
   AnimationChain_Reset( &( game->animationChain ) );

   if ( game->battle.specialEnemy == SpecialEnemy_DragonlordDragon )
   {
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Battle_EnemySlowFadeIn, Game_DrawEnemy, game );
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_BattleIntroMessageCallback, game );
   }
   else
   {
      TileMap_UpdateViewport( &( game->tileMap ) );
      Game_DrawTileMap( game );
      AnimationChain_Reset( &( game->animationChain ) );
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Battle_Checkerboard );
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Battle_EnemyFadeIn, Game_DrawEnemy, game );
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_BattleIntroMessageCallback, game );
   }

   AnimationChain_Start( &( game->animationChain ) );
}

void Game_ChangeSubState( Game_t* game, SubState_t newState )
{
   game->subState = newState;
}

void Game_AnimatePortalEntrance( Game_t* game, TilePortal_t* portal )
{
   game->targetPortal = portal;
   Game_ChangeToOverworldState( game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeOut );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_EnterTargetPortal, game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_FadeIn );
}

void Game_EnterTargetPortal( Game_t* game )
{
   u32 destinationTileIndex = game->targetPortal->destinationTileIndex;
   Direction_t arrivalDirection = game->targetPortal->arrivalDirection;

   switch ( game->targetPortal->destinationTileMapIndex )
   {
      case TILEMAP_TANTEGEL_ID: SET_VISITED_TANTEGEL( game->player.townsVisited ); break;
      case TILEMAP_BRECCONARY_ID: SET_VISITED_BRECCONARY( game->player.townsVisited ); break;
      case TILEMAP_GARINHAM_ID: SET_VISITED_GARINHAM( game->player.townsVisited ); break;
      case TILEMAP_KOL_ID: SET_VISITED_KOL( game->player.townsVisited ); break;
      case TILEMAP_CANTLIN_ID: SET_VISITED_CANTLIN( game->player.townsVisited ); break;
      case TILEMAP_RIMULDAR_ID: SET_VISITED_RIMULDAR( game->player.townsVisited ); break;
   }

   game->tileMap.previousId = game->tileMap.id;
   game->tileMap.lastPortalTileIndex = game->player.tileIndex;
   TileMap_Load( &( game->tileMap ), game->targetPortal->destinationTileMapIndex );

   game->player.sprite.position.x = (r32)( ( i32 )( TILE_SIZE * ( destinationTileIndex % game->tileMap.tilesX ) ) - game->player.sprite.offset.x ) + COLLISION_THETA;
   // the player sprite gets caught on unpassable tiles unless we use COLLISION_THETA here, but for some reason the x-axis has no problems
   game->player.sprite.position.y = (r32)( ( i32 )( TILE_SIZE * ( destinationTileIndex / game->tileMap.tilesX ) ) - game->player.sprite.offset.y ) - COLLISION_THETA;
   game->player.tileIndex = destinationTileIndex;
   Player_SetCanonicalTileIndex( &( game->player ) );
   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), destinationTileIndex );
   game->targetPortal = 0;

   ActiveSprite_SetDirection( &( game->player.sprite ), arrivalDirection );

   if ( game->tileMap.isDark && !game->gameFlags.defeatedDragonlord )
   {
      TileMap_ChangeViewportSize( &( game->tileMap ),
                                  (u16)( game->tileMap.glowDiameter * TILE_SIZE ),
                                  (u16)( game->tileMap.glowDiameter * TILE_SIZE ) );
   }
   else
   {
      TileMap_ResetViewport( &( game->tileMap ) );
   }
}

void Game_OpenMenu( Game_t* game, MenuId_t id )
{
   game->activeMenu = &( game->menus[(i32)id] );
   Menu_Reset( game->activeMenu );
   Game_ChangeSubState( game, SubState_Menu );
}

void Game_OpenDialog( Game_t* game )
{
   Dialog_Start( &( game->dialog ) );
   Game_ChangeSubState( game, SubState_Dialog );
}

void Game_RestoredHitPointsCallback( Game_t* game )
{
   Player_RestoreHitPoints( &( game->player ), game->pendingPayload8u );

   if ( game->mainState == MainState_Battle )
   {
      Battle_SwitchTurn( &( game->battle ) );
   }
}

void Game_CursedCallback( Game_t* game )
{
   Player_SetCursed( &( game->player ), True );
}

void Game_ResetBattleMenu( Game_t* game )
{
   if ( game->mainState == MainState_Battle )
   {
      game->activeMenu = &( game->menus[MenuId_Battle] );
      Menu_Reset( game->activeMenu );
      Game_ChangeSubState( game, SubState_Menu );
   }
}

void Game_HandleDeath( Game_t* game )
{
   u32 i;

   game->mainState = MainState_Overworld;
   game->subState = SubState_None;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 9; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_MidFadeOut, Game_DeathFadeOutCallback, game );

   for ( i = 0; i < 9; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_MidFadeIn, Screen_RestorePalette, &( game->screen ) );

   for ( i = 0; i < 4; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_DeathPostFadeCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

void Game_ExpelCursedPlayer( Game_t* game )
{
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_DIALOG_EXPEL_1 );
   Dialog_PushSection( &( game->dialog ), STRING_DIALOG_EXPEL_2 );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_DIALOG_EXPEL_3, Game_CursedExpelCallback, game );
   Game_OpenDialog( game );
}

void Game_TriggerEnding( Game_t* game )
{
   char msg[128];

   Dialog_Reset( &( game->dialog ) );

   sprintf( msg, STRING_NPC_ENDING_1, game->player.name );
   Dialog_PushSection( &( game->dialog ), msg );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_2 );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_3 );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_4 );
   sprintf( msg, STRING_NPC_ENDING_5, game->player.name );
   Dialog_PushSection( &( game->dialog ), msg );

   if ( !game->gameFlags.rescuedPrincess )
   {
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_5_1 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_5_2 );
   }

   Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_6 );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_7 );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_8 );
   sprintf( msg, STRING_NPC_ENDING_9, game->player.name );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_KingQuestionCallback, game );

   Game_OpenDialog( game );
}

internal void Game_BattleIntroMessageCallback( Game_t* game )
{
   char enemyName[24];
   char msg[64];

   Dialog_Reset( &( game->dialog ) );

   if ( game->battle.specialEnemy == SpecialEnemy_DragonlordWizard )
   {
      sprintf( msg, STRING_BATTLE_DRAGONLORD_WIZARDINTRO );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
   }
   else if ( game->battle.specialEnemy == SpecialEnemy_DragonlordDragon )
   {
      sprintf( msg, STRING_BATTLE_DRAGONLORD_DRAGONINTRO );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
   }
   else
   {
      sprintf( enemyName, "%s %s", game->battle.enemy.indefiniteArticle, game->battle.enemy.name );

      if ( Battle_RollEnemyFlee( &( game->battle ) ) )
      {
         sprintf( msg, STRING_BATTLE_ENEMYAPPROACHES, enemyName );
         Dialog_PushSectionWithCallback( &( game->dialog ), msg, Battle_EnemyInitiativeFlee, &( game->battle ) );
      }
      else if ( game->battle.turn == BattleTurn_Player )
      {
         sprintf( msg, STRING_BATTLE_ENEMYAPPROACHESINITIATIVE, enemyName );
         Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
      }
      else
      {
         sprintf( msg, STRING_BATTLE_ENEMYAPPROACHES, enemyName );
         Dialog_PushSectionWithCallback( &( game->dialog ), msg, Battle_EnemyInitiative, &( game->battle ) );
      }
   }
   
   Game_OpenDialog( game );
}

internal void Game_TicActiveSprites( Game_t* game )
{
   u32 i;

   ActiveSprite_Tic( &( game->player.sprite ) );

   for ( i = 0; i < game->tileMap.npcCount; i++ )
   {
      ActiveSprite_Tic( &( game->tileMap.npcs[i].sprite ) );
   }
}

internal void Game_DeathFadeOutCallback( Game_t* game )
{
   if ( game->player.isCursed )
   {
      game->player.gold = 0;
      game->player.stats.hitPoints = 1;
      game->player.stats.magicPoints = 0;
   }
   else
   {
      game->player.gold /= 2;
      game->player.stats.hitPoints = game->player.stats.maxHitPoints;
      game->player.stats.magicPoints = game->player.stats.maxMagicPoints;
   }

   if ( game->gameFlags.carryingPrincess )
   {
      game->gameFlags.carryingPrincess = False;
      game->gameFlags.rescuedPrincess = False;
      Sprite_LoadActive( &( game->player.sprite ), ACTIVE_SPRITE_PLAYER_ID );
   }

   TileMap_Load( &( game->tileMap ), TILEMAP_TANTEGEL_THRONEROOM_ID );
   game->player.tileIndex = 128;
   Player_SetCanonicalTileIndex( &( game->player ) );
   Player_CenterOnTile( &( game->player ) );
   ActiveSprite_SetDirection( &( game->player.sprite ), Direction_Up );
   TileMap_UpdateViewport( &( game->tileMap ) );
}

internal void Game_DeathPostFadeCallback( Game_t* game )
{
   char msg[128];

   Dialog_Reset( &( game->dialog ) );

   if ( game->player.isCursed )
   {
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_CURSED_1 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_CURSED_2 );
      sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_CURSED_3, game->player.name );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CursedExpelCallback, game );
   }
   else
   {
      sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_DEATH_1, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_DEATH_2 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_DEATH_3 );
   }

   Game_OpenDialog( game );
}

internal void Game_CursedExpelCallback( Game_t* game )
{
   u32 i;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 11; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_CursedExpelMessageCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_CursedExpelMessageCallback( Game_t* game )
{
   game->tileMap.evacPortal.arrivalDirection = Direction_Down;
   game->tileMap.evacPortal.destinationTileMapIndex = TILEMAP_OVERWORLD_ID;
   game->tileMap.evacPortal.destinationTileIndex = 7193;

   AnimationChain_Reset( &( game->animationChain ) );
   Game_AnimatePortalEntrance( game, &( game->tileMap.evacPortal ) );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_ResetTitleScreenFlash( Game_t* game )
{
   game->titleScreenFlash.isFlashing = False;
   game->titleScreenFlash.slowFlash = Random_u32( 0, 1 );
   game->titleScreenFlash.elapsedSeconds = 0.0f;
   game->titleScreenFlash.pauseSeconds = (r32)Random_u32( TITLESCREEN_FLASH_MINPAUSE, TITLESCREEN_FLASH_MAXPAUSE );
   game->titleScreenFlash.currentFrame = 0;
}

internal void Game_TicTitleScreenFlash( Game_t* game )
{
   TitleScreenFlash_t* flash = &( game->titleScreenFlash );
   r32 frameDuration;

   flash->elapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( flash->isFlashing )
   {
      frameDuration = flash->slowFlash ? TITLESCREEN_FLASH_SLOWFRAMEDURATION : TITLESCREEN_FLASH_FASTFRAMEDURATION;

      while ( flash->elapsedSeconds > frameDuration )
      {
         flash->elapsedSeconds -= frameDuration;
         flash->currentFrame++;

         if ( flash->currentFrame >= TITLESCREEN_FLASH_NUMFRAMES )
         {
            flash->isFlashing = False;
            flash->elapsedSeconds = 0.0f;
            flash->pauseSeconds = (r32)Random_u32( TITLESCREEN_FLASH_MINPAUSE, TITLESCREEN_FLASH_MAXPAUSE );
            break;
         }
      }
   }
   else if ( flash->elapsedSeconds > flash->pauseSeconds )
   {
      flash->isFlashing = True;
      flash->elapsedSeconds = 0.0f;
      flash->slowFlash = Random_u32( 0, 1 );
      flash->currentFrame = 0;
   }
}

internal void Game_KingQuestionCallback( Game_t* game )
{
   game->postDialogCallback = Game_KingQuestionPostDialogCallback;
   game->postDialogCallbackData = game;
}

internal void Game_KingQuestionPostDialogCallback( Game_t* game )
{
   u32 i;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 12; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_ActivePause );
   }
   
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_ActivePause, Game_KingQuestionPauseCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_KingQuestionPauseCallback( Game_t* game )
{
   char msg[128];

   Dialog_Reset( &( game->dialog ) );

   if ( game->gameFlags.rescuedPrincess )
   {
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_1 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_2 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_3 );
      sprintf( msg, STRING_NPC_ENDING_RESCUED_4, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_5 );
      sprintf( msg, STRING_NPC_ENDING_RESCUED_6, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_7 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_8 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_9 );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_ENDING_RESCUED_10, Game_GwaelinAccompanyCallback, game );
   }
   else
   {
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_NOTRESCUED_1 );
      sprintf( msg, STRING_NPC_ENDING_NOTRESCUED_2, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_NOTRESCUED_3 );
      Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_ENDING_NOTRESCUED_4, Game_GoFindGwaelinCallback, game );
   }

   Game_OpenDialog( game );
}

internal void Game_GwaelinAccompanyCallback( Game_t* game )
{
   game->postDialogCallback = Game_GwaelinAccompanyPostDialogCallback;
   game->postDialogCallbackData = game;
}

internal void Game_GwaelinAccompanyPostDialogCallback( Game_t* game )
{
   u32 i;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 12; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_ActivePause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_ActivePause, Game_GwaelinAccompanyPauseCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_GwaelinAccompanyPauseCallback( Game_t* game )
{
   char msg[128];

   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSection( &( game->dialog ), STRING_NPC_ENDING_RESCUED_11 );
   sprintf( msg, STRING_NPC_ENDING_RESCUED_12, game->player.name );
   Dialog_PushSection( &( game->dialog ), msg );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_NPC_ENDING_RESCUED_13, Game_QueenGwaelinCallback, game );
   Game_OpenDialog( game );
}

internal void Game_QueenGwaelinCallback( Game_t* game )
{
   game->tileMap.npcCount--;
   Sprite_LoadActive( &( game->player.sprite ), ACTIVE_SPRITE_PLAYER_CARRY_ID );
   game->player.sprite.direction = Direction_Down;
   game->postDialogCallback = Game_QueenGwaelinPostDialogCallback;
   game->postDialogCallbackData = game;
}

internal void Game_QueenGwaelinPostDialogCallback( Game_t* game )
{
   u32 i;

   for ( i = 0; i < game->tileMap.npcCount; i++ )
   {
      if ( game->tileMap.npcs[i].id == 88 )
      {
         Sprite_LoadActive( &( game->tileMap.npcs[i].sprite ), 16 );
      }
   }

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 16; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_ActivePause );
   }

   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Ending_WalkFade );

   for ( i = 0; i < 10; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_HappyEndingCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_HappyEndingCallback( Game_t* game )
{
   game->mainState = MainState_Ending_1;
   Screen_RestorePalette( &( game->screen ) );
   Screen_WipeColor( &( game->screen ), COLOR_BLACK );
}

internal void Game_GoFindGwaelinCallback( Game_t* game )
{
   game->postDialogCallback = Game_GoFindGwaelinPostDialogCallback;
   game->postDialogCallbackData = game;
}

internal void Game_GoFindGwaelinPostDialogCallback( Game_t* game )
{
   u32 i;

   game->player.sprite.direction = Direction_Down;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 10; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_ActivePause );
   }

   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Ending_WalkFade );

   for ( i = 0; i < 10; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_GoFindGwaelinPostDialogPauseCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_GoFindGwaelinPostDialogPauseCallback( Game_t* game )
{
   Screen_RestorePalette( &( game->screen ) );
   Game_Reset( game );
}

internal void Game_StartPostIntroFadeIn( Game_t* game )
{
   u32 i;

   game->mainState = MainState_Overworld;
   game->subState = SubState_None;

   AnimationChain_Reset( &( game->animationChain ) );

   for ( i = 0; i < 3; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   }

   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_ActiveMidFadeIn );

   for ( i = 0; i < 4; i++ )
   {
      AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_ActivePause );
   }

   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_ActivePause, Game_PostIntroPauseCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_PostIntroPauseCallback( Game_t* game )
{
   char msg[128];

   Dialog_Reset( &( game->dialog ) );

   if ( game->gameFlags.leftThroneRoom )
   {
      sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_3_1, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_3_2 );
   }
   else
   {
      sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_1, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_2 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_3 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_4 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_5 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_6 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_7 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_8 );
      Dialog_PushSection( &( game->dialog ), STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_9 );
      sprintf( msg, STRING_NPC_TANTEGEL_THRONEROOM_KING_INTRO_1_10, game->player.name );
      Dialog_PushSection( &( game->dialog ), msg );
   }

   Game_OpenDialog( game );
}

internal void Game_TitleScreenFadeInCallback( Game_t* game )
{
   Game_ResetTitleScreenFlash( game );
   Game_OpenMenu( game, MenuId_Startup );
}

internal void Game_EnterNameFadeOutCallback( Game_t* game )
{
   Screen_RestorePalette( &( game->screen ) );
   Screen_WipeColor( &( game->screen ), COLOR_BLACK );

   game->mainState = MainState_EnterName;
   game->subState = SubState_Menu;

   game->alphaPicker.position.x = 28;
   game->alphaPicker.position.y = 28;
   AlphaPicker_Reset( &( game->alphaPicker ), STRING_ALPHAPICKER_NAME_TITLE, False );
}

internal void Game_EnterPasswordFadeOutCallback( Game_t* game )
{
   Screen_RestorePalette( &( game->screen ) );
   Screen_WipeColor( &( game->screen ), COLOR_BLACK );

   game->mainState = MainState_EnterPassword;
   game->subState = SubState_Menu;

   // MUFFINS: this gives us some goodies for testing (level 30 with everything except a few treasures).
   // to use it, uncomment this line and comment out all the lines below it.
   //Game_Load( game, "UCz..xAgIwBJ........HxHdtPf..4" );

   game->alphaPicker.position.x = 28;
   game->alphaPicker.position.y = 28;
   AlphaPicker_Reset( &( game->alphaPicker ), STRING_ALPHAPICKER_PASSWORD_TITLE, True );
}
