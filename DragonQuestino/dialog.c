#include "game.h"
#include "tables.h"

internal void Dialog_ResetScroll( Dialog_t* dialog );
internal void Dialog_LoadMessage( Dialog_t* dialog );
internal uint32_t Dialog_GetMessageSectionCount( Dialog_t* dialog );
internal void Dialog_GetMessageText( Dialog_t* dialog, char* text );
internal void Dialog_FinishSection( Dialog_t* dialog );
internal void Dialog_LoadBattleVictorySpoils( Dialog_t* dialog, char* text );
internal void Dialog_LoadBattleStrengthAndAgilityUp( Dialog_t* dialog, char* text );
internal void Dialog_LoadBattleHitPointsAndMagicPointsUp( Dialog_t* dialog, char* text );
internal void Dialog_LoadSpellsLearned( Dialog_t* dialog, char* text );

internal void Dialog2_LoadActiveSectionLines( Dialog2_t* dialog );
internal void Dialog2_ResetScroll( Dialog2_t* dialog );
internal void Dialog2_FinishSection( Dialog2_t* dialog );

//void Dialog_Init( Dialog_t* dialog, Game_t* game )
//{
//   dialog->game = game;
//}

//void Dialog_Load( Dialog_t* dialog, DialogId_t id )
//{
//   dialog->id = id;
//   dialog->section = 0;
//
//   if ( dialog->game->mainState == MainState_Overworld )
//   {
//      dialog->position.x = 32;
//      dialog->position.y = 144;
//      dialog->size.x = 24;
//      dialog->size.y = 8;
//      dialog->lineWidth = 22;
//   }
//   else
//   {
//      dialog->position.x = 24;
//      dialog->position.y = 168;
//      dialog->size.x = 26;
//      dialog->size.y = 6;
//      dialog->lineWidth = 24;
//   }
//
//   Dialog_LoadMessage( dialog );
//   Dialog_ResetScroll( dialog );
//}

void Dialog_SetInsertionText( Dialog_t* dialog, const char* text )
{
   strcpy( dialog->insertionText, text );
}

//void Dialog_Draw( Dialog_t* dialog )
//{
//   uint32_t i, x, y, stopCharIndex, len;
//   char substr[DIALOG_MAX_LINE_CHARS];
//   Screen_t* screen = &( dialog->game->screen );
//
//   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y );
//
//   x = dialog->position.x + TEXT_TILE_SIZE;
//   y = dialog->position.y + TEXT_TILE_SIZE;
//
//   if ( dialog->isScrolling )
//   {
//      stopCharIndex = (uint32_t)( dialog->scrollSeconds / DIALOG_SCROLL_CHAR_SECONDS );
//
//      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
//      {
//         len = (uint32_t)( strlen( dialog->lines[i] ) );
//
//         if ( len < stopCharIndex )
//         {
//            Screen_DrawText( screen, dialog->lines[i], x, y );
//            stopCharIndex -= len;
//         }
//         else
//         {
//            strcpy( substr, dialog->lines[i] );
//            substr[stopCharIndex] = '\0';
//            Screen_DrawText( screen, substr, x, y );
//            break;
//         }
//      }
//   }
//   else
//   {
//      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
//      {
//         Screen_DrawText( screen, dialog->lines[i], x, y );
//      }
//
//      if ( dialog->showCarat && !Dialog_IsDone( dialog ) )
//      {
//         Screen_DrawChar( screen, DOWNWARD_CARAT, x + ( ( dialog->lineWidth / 2 ) * TEXT_TILE_SIZE ), y );
//      }
//   }
//}

//Bool_t Dialog_StepAhead( Dialog_t* dialog )
//{
//   if ( dialog->isScrolling )
//   {
//      Dialog_FinishSection( dialog );
//   }
//   else if ( dialog->section < ( dialog->sectionCount - 1 ) )
//   {
//      Dialog_NextSection( dialog );
//      return True;
//   }
//
//   return False;
//}

//void Dialog_NextSection( Dialog_t* dialog )
//{
//   if ( dialog->section == 0 )
//   {
//      switch ( dialog->id )
//      {
//         case DialogId_Battle_FleeAttemptSucceeded:
//            Animation_Start( &( dialog->game->animation ), AnimationId_Battle_EnemyFadeOut );
//            break;
//         case DialogId_Spell_CastHeal1:
//         case DialogId_Spell_CastHeal2:
//         case DialogId_Spell_CastMidheal1:
//         case DialogId_Spell_CastMidheal2:
//            Player_RestoreHitPoints( &( dialog->game->player ), dialog->game->pendingPayload8u );
//            Game_DrawQuickStatus( dialog->game );
//            break;
//      }
//   }
//
//   dialog->section++;
//   Dialog_LoadMessage( dialog );
//   Dialog_ResetScroll( dialog );
//}

//void Dialog_Tic( Dialog_t* dialog )
//{
//   if ( dialog->isScrolling )
//   {
//      dialog->scrollSeconds += CLOCK_FRAME_SECONDS;
//
//      if ( dialog->scrollSeconds > dialog->scrollTotalSeconds )
//      {
//         Dialog_FinishSection( dialog );
//      }
//   }
//   else if ( !Dialog_IsDone( dialog ) )
//   {
//      dialog->blinkSeconds += CLOCK_FRAME_SECONDS;
//
//      while ( dialog->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
//      {
//         TOGGLE_BOOL( dialog->showCarat );
//         dialog->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
//      }
//   }
//}

//Bool_t Dialog_IsDone( Dialog_t* dialog )
//{
//   return ( !dialog->isScrolling && ( dialog->section >= ( dialog->sectionCount - 1 ) ) ) ? True : False;
//}

internal void Dialog_ResetScroll( Dialog_t* dialog )
{
   dialog->isScrolling = True;
   dialog->scrollSeconds = 0.0f;
   dialog->scrollTotalSeconds = dialog->charCount * DIALOG_SCROLL_CHAR_SECONDS;
   dialog->showCarat = True;
   dialog->blinkSeconds = 0.0f;
}

internal void Dialog_LoadMessage( Dialog_t* dialog )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen;
   Bool_t endOfLine, endOfText;
   char curChar, message[DIALOG_MAX_MESSAGE_CHARS];

   dialog->lineCount = 0;
   dialog->charCount = 0;
   dialog->sectionCount = Dialog_GetMessageSectionCount( dialog );

   Dialog_GetMessageText( dialog, message );
   strLen = (uint16_t)strlen( message );

   for ( textIndex = 0, lineIndex = 0, lastSpaceIndex = 0, currentLine = 0; textIndex < strLen; textIndex++ )
   {
      curChar = message[textIndex];
      endOfLine = ( lineIndex == ( dialog->lineWidth - 1 ) );
      endOfText = ( textIndex == ( strLen - 1 ) ) ? True : False;

      if ( endOfLine || endOfText )
      {
         if ( ( lastSpaceIndex > 0 ) && !endOfText )
         {
            if ( curChar == ' ' )
            {
               dialog->lines[currentLine][lineIndex] = '\0';
            }
            else if ( message[textIndex + 1] == ' ' )
            {
               dialog->lines[currentLine][lineIndex] = curChar;
               dialog->lines[currentLine][lineIndex + 1] = '\0';
            }
            else
            {
               dialog->lines[currentLine][lastSpaceIndex - 1] = '\0';
               textIndex -= ( ( lineIndex - lastSpaceIndex ) + 1 );
            }
         }
         else if ( curChar == ' ' )
         {
            dialog->lines[currentLine][lineIndex] = '\0';
         }
         else
         {
            dialog->lines[currentLine][lineIndex] = curChar;
            dialog->lines[currentLine][lineIndex + 1] = '\0';
         }

         lineIndex = 0;
         lastSpaceIndex = 0;
         dialog->lineCount++;
         dialog->charCount += (uint32_t)( strlen( dialog->lines[currentLine] ) );
         currentLine++;
      }
      else if ( curChar != ' ' )
      {
         dialog->lines[currentLine][lineIndex++] = curChar;
      }
      else if ( lineIndex > 0 && lastSpaceIndex != lineIndex )
      {
         dialog->lines[currentLine][lineIndex++] = curChar;
         lastSpaceIndex = lineIndex;
      }
   }
}

internal uint32_t Dialog_GetMessageSectionCount( Dialog_t* dialog )
{
   switch ( dialog->id )
   {
      case DialogId_Spell_None:
      case DialogId_Spell_OverworldCantCast:
      case DialogId_Spell_OverworldCastGlow:
      case DialogId_Spell_CastEvac:
      case DialogId_Spell_CastZoom:
      case DialogId_Spell_NotEnoughMp:
      case DialogId_Item_None:
      case DialogId_FullyHealed:
      case DialogId_HolyProtection_Off:
      case DialogId_Use_TorchCantUse:
      case DialogId_Use_TorchAlreadyUsed:
      case DialogId_Use_Torch:
      case DialogId_Use_GwaelynsLoveCantUse:
      case DialogId_Use_RainbowDropCantUse:
      case DialogId_Use_RainbowDrop:
      case DialogId_Battle_EnemyApproaches:
      case DialogId_Battle_Victory:
      case DialogId_Battle_Spell_None:
      case DialogId_Battle_Spell_CantCast:
      case DialogId_Battle_Spell_NotEnoughMp:
      case DialogId_Battle_FullyHealed:
      case DialogId_Battle_Spell_NoEffect:
      case DialogId_Battle_Spell_AttackSucceeded:
      case DialogId_Battle_Spell_Sizz:
      case DialogId_Battle_Spell_Sizzle:
         return 1;
      case DialogId_Use_FairyWaterCursed:
      case DialogId_Use_FairyWater:
      case DialogId_Use_FairyFlute:
      case DialogId_Use_SilverHarp:
      case DialogId_Use_CursedBelt:
      case DialogId_Spell_OverworldCastGlowCursed:
      case DialogId_Spell_CastRepelCursed:
      case DialogId_Spell_CastRepel:
      case DialogId_Spell_CastEvacCursed:
      case DialogId_Chest_DeathNecklace:
      case DialogId_Spell_CastHeal1:
      case DialogId_Spell_CastHeal2:
      case DialogId_Spell_CastMidheal1:
      case DialogId_Spell_CastMidheal2:
      case DialogId_Spell_Blocked:
      case DialogId_Battle_AttackAttemptSucceeded:
      case DialogId_Battle_AttackAttemptFailed:
      case DialogId_Battle_FleeAttemptSucceeded:
      case DialogId_Battle_FleeAttemptFailed:
      case DialogId_Battle_VictoryWithSpoils:
      case DialogId_Battle_Spell_Blocked:
      case DialogId_Battle_Spell_Fizzled:
         return 2;
      case DialogId_Use_GwaelynsLove:
         return 4;
      case DialogId_Battle_VictoryWithLevelUp:
         return ( dialog->game->battle.previousSpells < dialog->game->player.spells ) ? 6 : 5;
   }

   return 0;
}

internal void Dialog_GetMessageText( Dialog_t* dialog, char* text )
{
   uint32_t e;
   Player_t* player = &( dialog->game->player );
   Battle_t* battle = &( dialog->game->battle );

   switch ( dialog->id )
   {
      case DialogId_Spell_None: strcpy( text, STRING_DIALOG_NO_SPELLS ); return;
      case DialogId_Spell_OverworldCantCast: strcpy( text, STRING_DIALOG_SPELLS_OVERWORLD_CANT_CAST ); return;
      case DialogId_Spell_OverworldCastGlow: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_GLOW ); return;
      case DialogId_Spell_OverworldCastGlowCursed:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_GLOW ); return;
            case 1: strcpy( text, STRING_GLOW_CURSED ); return;
         }
      case DialogId_Spell_CastRepelCursed:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_REPEL ); return;
            case 1: strcpy( text, STRING_HOLYPROTECTION_CURSED ); return;
         }
      case DialogId_Spell_CastEvacCursed:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_EVAC ); return;
            case 1: strcpy( text, STRING_EVAC_CURSED ); return;
         }
      case DialogId_Spell_CastRepel:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_REPEL ); return;
            case 1: strcpy( text, STRING_HOLYPROTECTION_ON ); return;
         }
      case DialogId_Spell_CastEvac: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_EVAC ); return;
      case DialogId_Spell_CastZoom: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_ZOOM ); return;
      case DialogId_Spell_NotEnoughMp: strcpy( text, STRING_NOTENOUGHMP ); return;
      case DialogId_Item_None: strcpy( text, STRING_DIALOG_NO_ITEMS ); return;
      case DialogId_FullyHealed: strcpy( text, STRING_FULLYHEALED ); return;
      case DialogId_HolyProtection_Off: strcpy( text, STRING_HOLYPROTECTION_OFF ); return;
      case DialogId_Use_FairyWaterCursed:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_FAIRYWATER ); return;
            case 1: strcpy( text, STRING_HOLYPROTECTION_CURSED ); return;
         }
      case DialogId_Use_FairyWater:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_FAIRYWATER ); return;
            case 1: strcpy( text, STRING_HOLYPROTECTION_ON ); return;
         }
      case DialogId_Use_TorchCantUse: strcpy( text, STRING_ITEMUSE_TORCH_CANTUSE ); return;
      case DialogId_Use_TorchAlreadyUsed: strcpy( text, STRING_ITEMUSE_TORCH_ALREADYUSED ); return;
      case DialogId_Use_Torch: strcpy( text, STRING_ITEMUSE_TORCH ); return;
      case DialogId_Use_FairyFlute:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_FAIRYFLUTE_1 ); return;
            case 1: strcpy( text, STRING_ITEMUSE_FAIRYFLUTE_2 ); return;
         }
      case DialogId_Use_SilverHarp:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_SILVERHARP_1 ); return;
            case 1: strcpy( text, STRING_ITEMUSE_SILVERHARP_2 ); return;
         }
      case DialogId_Use_GwaelynsLoveCantUse: strcpy( text, STRING_ITEMUSE_GWAELINSLOVE_CANTUSE ); return;
      case DialogId_Use_GwaelynsLove:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_ITEMUSE_GWAELINSLOVE_1, player->name ); return;
            case 1:
               e = Player_GetExperienceRemaining( player );
               sprintf( text, STRING_ITEMUSE_GWAELINSLOVE_2, e );
               return;
            case 2: sprintf( text, STRING_ITEMUSE_GWAELINSLOVE_3, dialog->insertionText ); return;
            case 3: sprintf( text, STRING_ITEMUSE_GWAELINSLOVE_4, player->name ); return;
         }
      case DialogId_Use_RainbowDropCantUse: strcpy( text, STRING_ITEMUSE_RAINBOWDROP_CANTUSE ); return;
      case DialogId_Use_RainbowDrop: strcpy( text, STRING_ITEMUSE_RAINBOWDROP ); return;
      case DialogId_Use_CursedBelt:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_CURSEDBELT ); return;
            case 1: strcpy( text, STRING_CURSED ); return;
         }
      case DialogId_Spell_CastHeal1:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_HEAL ); return;
            //case 1: sprintf( text, STRING_DIALOG_HEAL_RESULT, dialog->insertionText ); return;
         }
      case DialogId_Spell_CastHeal2:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_HEAL ); return;
            //case 1: sprintf( text, STRING_DIALOG_HEAL_RESULT, dialog->insertionText ); return;
         }
      case DialogId_Spell_CastMidheal1:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_MIDHEAL ); return;
            //case 1: sprintf( text, STRING_DIALOG_HEAL_RESULT, dialog->insertionText ); return;
         }
      case DialogId_Spell_CastMidheal2:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_MIDHEAL ); return;
            //case 1: sprintf( text, STRING_DIALOG_HEAL_RESULT, dialog->insertionText ); return;
         }
      case DialogId_Spell_Blocked:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_DIALOG_SPELLS_OVERWORLD_CAST, dialog->insertionText ); return;
            case 1: strcpy( text, STRING_SPELLBLOCKED ); return;
         }
      case DialogId_Battle_EnemyApproaches:
         sprintf( text, STRING_BATTLE_ENEMYAPPROACHES, dialog->insertionText ); return;
      case DialogId_Battle_AttackAttemptSucceeded:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_BATTLE_ATTACKATTEMPT ); return;
            case 1: strcpy( text, dialog->insertionText ); return;
         }
      case DialogId_Battle_AttackAttemptFailed:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_BATTLE_ATTACKATTEMPT ); return;
            case 1: sprintf( text, STRING_BATTLE_ATTACKATTEMPTFAILED, dialog->insertionText ); return;
         }
      case DialogId_Battle_FleeAttemptSucceeded:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_BATTLE_FLEEATTEMPT ); return;
            case 1: sprintf( text, STRING_BATTLE_FLEEATTEMPTSUCCEEDED, dialog->insertionText ); return;
         }
      case DialogId_Battle_FleeAttemptFailed:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_BATTLE_FLEEATTEMPT ); return;
            case 1: sprintf( text, STRING_BATTLE_FLEEATTEMPTFAILED, dialog->insertionText ); return;
         }
      case DialogId_Battle_Victory:
         sprintf( text, STRING_BATTLE_VICTORY, battle->enemy.name ); return;
      case DialogId_Battle_VictoryWithSpoils:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_BATTLE_VICTORY, battle->enemy.name ); return;
            case 1: Dialog_LoadBattleVictorySpoils( dialog, text ); return;
         }
      case DialogId_Battle_VictoryWithLevelUp:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_BATTLE_VICTORY, battle->enemy.name ); return;
            case 1: Dialog_LoadBattleVictorySpoils( dialog, text ); return;
            case 2: strcpy( text, STRING_BATTLE_LEVELUP ); return;
            case 3: Dialog_LoadBattleStrengthAndAgilityUp( dialog, text ); return;
            case 4: Dialog_LoadBattleHitPointsAndMagicPointsUp( dialog, text ); return;
            case 5: Dialog_LoadSpellsLearned( dialog, text ); return;
         }
      case DialogId_Battle_Spell_None: strcpy( text, STRING_BATTLE_NOSPELLS ); return;
      case DialogId_Battle_Spell_CantCast: strcpy( text, STRING_BATTLE_CANTCASTSPELL ); return;
      case DialogId_Battle_Spell_NotEnoughMp: strcpy( text, STRING_BATTLE_NOTENOUGHMP ); return;
      case DialogId_Battle_Spell_Blocked:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_BATTLE_SPELLCAST, dialog->insertionText ); return;
            case 1: strcpy( text, STRING_SPELLBLOCKED ); return;
         }
      case DialogId_Battle_Spell_Fizzled:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_BATTLE_SPELLCAST, dialog->insertionText ); return;
            case 1: strcpy( text, STRING_BATTLE_SPELLFIZZLED ); return;
         }
      case DialogId_Battle_FullyHealed: strcpy( text, STRING_BATTLE_FULLYHEALED ); return;
      case DialogId_Battle_Spell_NoEffect: sprintf( text, STRING_BATTLE_SPELL_NOEFFECT, dialog->insertionText ); return;
      case DialogId_Battle_Spell_AttackSucceeded: strcpy( text, dialog->insertionText ); return;
      case DialogId_Battle_Spell_Sizz:
      case DialogId_Battle_Spell_Sizzle:
         sprintf( text, STRING_BATTLE_SPELLCAST, dialog->insertionText ); return;
   }
}

internal void Dialog_FinishSection( Dialog_t* dialog )
{
   dialog->isScrolling = False;

   if ( dialog->section == 0 )
   {
      switch ( dialog->id )
      {
         case DialogId_Spell_CastHeal1:
         case DialogId_Spell_CastHeal2:
         case DialogId_Spell_CastMidheal1:
         case DialogId_Spell_CastMidheal2:
         case DialogId_Spell_OverworldCastGlowCursed:
         case DialogId_Spell_OverworldCastGlow:
         case DialogId_Spell_CastRepelCursed:
         case DialogId_Spell_CastRepel:
         case DialogId_Spell_CastEvacCursed:
         case DialogId_Spell_CastEvac:
         case DialogId_Spell_CastZoom:
         case DialogId_Spell_Blocked:
         case DialogId_Battle_Spell_Blocked:
         case DialogId_Battle_Spell_Fizzled:
         case DialogId_Battle_Spell_Sizz:
         case DialogId_Battle_Spell_Sizzle:
            Animation_Start( &( dialog->game->animation ), AnimationId_CastSpell );
            break;
         case DialogId_Battle_EnemyApproaches:
            Animation_Start( &( dialog->game->animation ), AnimationId_Battle_EnemyFadeInPause );
            break;
         case DialogId_Battle_AttackAttemptSucceeded:
            Animation_Start( &( dialog->game->animation ), AnimationId_Battle_EnemyDamage );
            break;
         case DialogId_Battle_AttackAttemptFailed:
            Animation_Start( &( dialog->game->animation ), AnimationId_Battle_EnemyDodge );
            break;
         case DialogId_Battle_Spell_None:
         case DialogId_Battle_Spell_CantCast:
         case DialogId_Battle_Spell_NotEnoughMp:
         case DialogId_Battle_FullyHealed:
         case DialogId_Battle_Spell_NoEffect:
            //Dialog_Draw( dialog );
            Menu_Reset( dialog->game->activeMenu );
            Game_ChangeSubState( dialog->game, SubState_Menu );
            break;
      }
   }
   else if ( dialog->section == 1 )
   {
      switch ( dialog->id )
      {
         case DialogId_Battle_AttackAttemptSucceeded:
            //Dialog_Draw( dialog );
            if ( dialog->game->battle.enemy.stats.hitPoints == 0 )
            {
               Animation_Start( &( dialog->game->animation ), AnimationId_Battle_VictoryPause );
            }
            else
            {
               Menu_Reset( dialog->game->activeMenu );
               Game_ChangeSubState( dialog->game, SubState_Menu );
            }
            break;
         case DialogId_Battle_AttackAttemptFailed:
         case DialogId_Battle_FleeAttemptFailed:
         case DialogId_Battle_Spell_Blocked:
         case DialogId_Battle_Spell_Fizzled:
         case DialogId_Battle_Spell_AttackSucceeded:
            //Dialog_Draw( dialog );
            if ( dialog->game->battle.enemy.stats.hitPoints == 0 )
            {
               Animation_Start( &( dialog->game->animation ), AnimationId_Battle_VictoryPause );
            }
            else
            {
               Menu_Reset( dialog->game->activeMenu );
               Game_ChangeSubState( dialog->game, SubState_Menu );
            }
            break;
         case DialogId_Battle_VictoryWithSpoils:
         case DialogId_Battle_VictoryWithLevelUp:
            Game_DrawQuickStatus( dialog->game );
            break;
         case DialogId_Spell_CastHeal1:
         case DialogId_Spell_CastHeal2:
         case DialogId_Spell_CastMidheal1:
         case DialogId_Spell_CastMidheal2:
            if ( dialog->game->mainState == MainState_Battle )
            {
               dialog->game->activeMenu = &( dialog->game->menus[MenuId_Battle] );
               Menu_Reset( dialog->game->activeMenu );
               dialog->game->screen.needsRedraw = True;
               Game_ChangeSubState( dialog->game, SubState_Menu );
            }
            break;
      }
   }
   else if ( dialog->section == 2 )
   {
      switch ( dialog->id )
      {
         case DialogId_Battle_VictoryWithLevelUp:
            dialog->game->player.level = Player_GetLevelFromExperience( &( dialog->game->player ) );
            Game_DrawQuickStatus( dialog->game );
            break;
      }
   }
   else if ( dialog->section == 4 )
   {
      switch ( dialog->id )
      {
         case DialogId_Battle_VictoryWithLevelUp:
            dialog->game->player.stats.maxHitPoints += dialog->game->battle.hitPointsGained;
            dialog->game->player.stats.hitPoints += dialog->game->battle.hitPointsGained;
            dialog->game->player.stats.maxMagicPoints += dialog->game->battle.magicPointsGained;
            dialog->game->player.stats.magicPoints += dialog->game->battle.magicPointsGained;
            Game_DrawQuickStatus( dialog->game );
            break;
      }
   }
}

internal void Dialog_LoadBattleVictorySpoils( Dialog_t* dialog, char* text )
{
   Battle_t* battle = &( dialog->game->battle );

   if ( battle->experienceGained > 0 && battle->goldGained > 0 )
   {
      sprintf( text, STRING_BATTLE_EXPERIENCEANDGOLD, battle->experienceGained, ( battle->experienceGained == 1 ) ? STRING_POINT : STRING_POINTS, battle->goldGained );
   }
   else if ( battle->experienceGained > 0 && battle->goldGained == 0 )
   {
      sprintf( text, STRING_BATTLE_EXPERIENCEONLY, battle->experienceGained, ( battle->experienceGained == 1 ) ? STRING_POINT : STRING_POINTS );
   }
   else if ( battle->experienceGained == 0 && battle->goldGained > 0 )
   {
      sprintf( text, STRING_BATTLE_GOLDONLY, battle->goldGained );
   }
}

internal void Dialog_LoadBattleStrengthAndAgilityUp( Dialog_t* dialog, char* text )
{
   Battle_t* battle = &( dialog->game->battle );

   if ( battle->strengthGained > 0 && battle->agilityGained > 0 )
   {
      sprintf( text, STRING_BATTLE_STRENGTHANDAGILITYGAIN, battle->strengthGained, battle->agilityGained );
   }
   else if ( battle->strengthGained > 0 && battle->agilityGained == 0 )
   {
      sprintf( text, STRING_BATTLE_STRENGTHGAIN, battle->strengthGained );
   }
   else if ( battle->strengthGained == 0 && battle->agilityGained > 0 )
   {
      sprintf( text, STRING_BATTLE_AGILITYGAIN, battle->agilityGained );
   }
}

internal void Dialog_LoadBattleHitPointsAndMagicPointsUp( Dialog_t* dialog, char* text )
{
   Battle_t* battle = &( dialog->game->battle );

   if ( battle->hitPointsGained > 0 && battle->magicPointsGained > 0 )
   {
      sprintf( text, STRING_BATTLE_HITPOINTSANDMAGICPOINTSGAIN, battle->hitPointsGained, battle->magicPointsGained );
   }
   else if ( battle->hitPointsGained > 0 && battle->magicPointsGained == 0 )
   {
      sprintf( text, STRING_BATTLE_HITPOINTSGAIN, battle->hitPointsGained );
   }
   else if ( battle->hitPointsGained == 0 && battle->magicPointsGained > 0 )
   {
      sprintf( text, STRING_BATTLE_MAGICPOINTSGAIN, battle->magicPointsGained );
   }
}

internal void Dialog_LoadSpellsLearned( Dialog_t* dialog, char* text )
{
   uint16_t i, learnedSpell = 0;

   for ( i = 0; i < SPELL_TABLE_SIZE; i++ )
   {
      if ( ( dialog->game->battle.previousSpells & ( 0x1 << i ) ) != ( dialog->game->player.spells & ( 0x1 << i ) ) )
      {
         learnedSpell = i;
         break;
      }
   }

   switch ( i )
   {
      case 0: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_HEAL ); break;
      case 1: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_SIZZ ); break;
      case 2: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_SLEEP ); break;
      case 3: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_GLOW ); break;
      case 4: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_FIZZLE ); break;
      case 5: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_EVAC ); break;
      case 6: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_ZOOM ); break;
      case 7: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_REPEL ); break;
      case 8: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_MIDHEAL ); break;
      case 9: sprintf( text, STRING_BATTLE_LEARNEDSPELL, STRING_SPELL_SIZZLE ); break;
   }
}

void Dialog2_Init( Dialog2_t* dialog, Screen_t* screen )
{
   dialog->screen = screen;
}

void Dialog2_Reset( Dialog2_t* dialog, MainState_t mainState )
{
   dialog->sectionCount = 0;
   dialog->activeSection = 0;

   switch ( mainState )
   {
      case MainState_Overworld:
         dialog->position.x = 32;
         dialog->position.y = 144;
         dialog->size.x = 24;
         dialog->size.y = 8;
         dialog->lineWidth = 22;
         break;
      case MainState_Battle:
         dialog->position.x = 24;
         dialog->position.y = 168;
         dialog->size.x = 26;
         dialog->size.y = 6;
         dialog->lineWidth = 24;
         break;
   }
}

void Dialog2_Start( Dialog2_t* dialog )
{
   Dialog2_LoadActiveSectionLines( dialog );
   Dialog2_ResetScroll( dialog );
}

void Dialog2_PushSection( Dialog2_t* dialog, const char* text, void ( *callback )( void* ), void* callbackData )
{
   strcpy( dialog->sectionTexts[dialog->sectionCount], text );
   dialog->sectionCallbacks[dialog->sectionCount] = callback;
   dialog->sectionCallbackData[dialog->sectionCount] = callbackData;
   dialog->sectionCount++;
}

void Dialog2_NextSection( Dialog2_t* dialog )
{
   dialog->activeSection++;
   Dialog2_LoadActiveSectionLines( dialog );
   Dialog2_ResetScroll( dialog );
}

Bool_t Dialog2_StepAhead( Dialog2_t* dialog )
{
   if ( dialog->isScrolling )
   {
      Dialog2_FinishSection( dialog );
      return False;
   }

   Dialog2_NextSection( dialog );
   return True;
}

Bool_t Dialog2_IsDone( Dialog2_t* dialog )
{
   return ( !dialog->isScrolling && ( dialog->activeSection >= ( dialog->sectionCount - 1 ) ) ) ? True : False;
}

void Dialog2_Tic( Dialog2_t* dialog )
{
   if ( dialog->isScrolling )
   {
      dialog->scrollSeconds += CLOCK_FRAME_SECONDS;

      if ( dialog->scrollSeconds > dialog->scrollTotalSeconds )
      {
         Dialog2_FinishSection( dialog );
      }
   }
   else if ( !Dialog2_IsDone( dialog ) )
   {
      dialog->blinkSeconds += CLOCK_FRAME_SECONDS;

      while ( dialog->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
      {
         TOGGLE_BOOL( dialog->showCarat );
         dialog->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      }
   }
}

void Dialog2_Draw( Dialog2_t* dialog )
{
   uint32_t i, x, y, stopCharIndex, len;
   char substr[DIALOG2_LINE_TEXT_SIZE];
   Screen_t* screen = dialog->screen;

   Screen_DrawTextWindow( screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y );

   x = dialog->position.x + TEXT_TILE_SIZE;
   y = dialog->position.y + TEXT_TILE_SIZE;

   if ( dialog->isScrolling )
   {
      stopCharIndex = (uint32_t)( dialog->scrollSeconds / DIALOG2_SCROLL_CHAR_SECONDS );

      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         len = (uint32_t)( strlen( dialog->lines[i] ) );

         if ( len < stopCharIndex )
         {
            Screen_DrawText( screen, dialog->lines[i], x, y );
            stopCharIndex -= len;
         }
         else
         {
            strcpy( substr, dialog->lines[i] );
            substr[stopCharIndex] = '\0';
            Screen_DrawText( screen, substr, x, y );
            break;
         }
      }
   }
   else
   {
      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         Screen_DrawText( screen, dialog->lines[i], x, y );
      }

      if ( dialog->showCarat && !Dialog2_IsDone( dialog ) )
      {
         Screen_DrawChar( screen, DOWNWARD_CARAT, x + ( ( dialog->lineWidth / 2 ) * TEXT_TILE_SIZE ), y );
      }
   }
}

internal void Dialog2_LoadActiveSectionLines( Dialog2_t* dialog )
{
   uint32_t textIndex, lineIndex, lastSpaceIndex, currentLine;
   uint32_t strLen;
   Bool_t endOfLine, endOfText;
   char curChar;
   char* sectionText = dialog->sectionTexts[dialog->activeSection];

   dialog->lineCount = 0;
   dialog->charCount = 0;

   strLen = (uint16_t)strlen( sectionText );

   for ( textIndex = 0, lineIndex = 0, lastSpaceIndex = 0, currentLine = 0; textIndex < strLen; textIndex++ )
   {
      curChar = sectionText[textIndex];
      endOfLine = ( lineIndex == ( dialog->lineWidth - 1 ) );
      endOfText = ( textIndex == ( strLen - 1 ) ) ? True : False;

      if ( endOfLine || endOfText )
      {
         if ( ( lastSpaceIndex > 0 ) && !endOfText )
         {
            if ( curChar == ' ' )
            {
               dialog->lines[currentLine][lineIndex] = '\0';
            }
            else if ( sectionText[textIndex + 1] == ' ' )
            {
               dialog->lines[currentLine][lineIndex] = curChar;
               dialog->lines[currentLine][lineIndex + 1] = '\0';
            }
            else
            {
               dialog->lines[currentLine][lastSpaceIndex - 1] = '\0';
               textIndex -= ( ( lineIndex - lastSpaceIndex ) + 1 );
            }
         }
         else if ( curChar == ' ' )
         {
            dialog->lines[currentLine][lineIndex] = '\0';
         }
         else
         {
            dialog->lines[currentLine][lineIndex] = curChar;
            dialog->lines[currentLine][lineIndex + 1] = '\0';
         }

         lineIndex = 0;
         lastSpaceIndex = 0;
         dialog->lineCount++;
         dialog->charCount += (uint32_t)( strlen( dialog->lines[currentLine] ) );
         currentLine++;
      }
      else if ( curChar != ' ' )
      {
         dialog->lines[currentLine][lineIndex++] = curChar;
      }
      else if ( lineIndex > 0 && lastSpaceIndex != lineIndex )
      {
         dialog->lines[currentLine][lineIndex++] = curChar;
         lastSpaceIndex = lineIndex;
      }
   }
}

internal void Dialog2_ResetScroll( Dialog2_t* dialog )
{
   dialog->isScrolling = True;
   dialog->scrollSeconds = 0.0f;
   dialog->scrollTotalSeconds = dialog->charCount * DIALOG2_SCROLL_CHAR_SECONDS;
   dialog->showCarat = True;
   dialog->blinkSeconds = 0.0f;
}

internal void Dialog2_FinishSection( Dialog2_t* dialog )
{
   dialog->isScrolling = False;

   if ( dialog->sectionCallbacks[dialog->activeSection] )
   {
      dialog->sectionCallbacks[dialog->activeSection]( dialog->sectionCallbackData[dialog->activeSection] );
   }
}
