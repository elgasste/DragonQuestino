#include "dialog.h"
#include "screen.h"
#include "player.h"
#include "clock.h"
#include "animation.h"

internal void Dialog_ResetScroll( Dialog_t* dialog );
internal void Dialog_LoadMessage( Dialog_t* dialog );
internal uint32_t Dialog_GetMessageSectionCount( DialogId_t id );
internal void Dialog_GetMessageText( Dialog_t* dialog, char* text );
internal void Dialog_FinishSection( Dialog_t* dialog );

void Dialog_Init( Dialog_t* dialog, Screen_t* screen, Player_t* player, Animation_t* animation )
{
   dialog->screen = screen;
   dialog->player = player;
   dialog->animation = animation;
}

void Dialog_Load( Dialog_t* dialog, DialogId_t id )
{
   dialog->id = id;
   dialog->section = 0;
   dialog->position.x = 32;
   dialog->position.y = 144;
   dialog->size.x = 24;
   dialog->size.y = 8;
   dialog->lineWidth = 22;

   Dialog_LoadMessage( dialog );
   Dialog_ResetScroll( dialog );
}

void Dialog_SetInsertionText( Dialog_t* dialog, const char* text )
{
   strcpy( dialog->insertionText, text );
}

void Dialog_Draw( Dialog_t* dialog )
{
   uint32_t i, x, y, stopCharIndex, len;
   char substr[DIALOG_MAX_LINE_CHARS];

   Screen_DrawTextWindow( dialog->screen, dialog->position.x, dialog->position.y, dialog->size.x, dialog->size.y );

   x = dialog->position.x + TEXT_TILE_SIZE;
   y = dialog->position.y + TEXT_TILE_SIZE;

   if ( dialog->isScrolling )
   {
      stopCharIndex = (uint32_t)( dialog->scrollSeconds / DIALOG_SCROLL_CHAR_SECONDS );

      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         len = (uint32_t)( strlen( dialog->lines[i] ) );

         if ( len < stopCharIndex )
         {
            Screen_DrawText( dialog->screen, dialog->lines[i], x, y );
            stopCharIndex -= len;
         }
         else
         {
            strcpy( substr, dialog->lines[i] );
            substr[stopCharIndex] = '\0';
            Screen_DrawText( dialog->screen, substr, x, y );
            break;
         }
      }
   }
   else
   {
      for ( i = 0; i < dialog->lineCount; i++, y += TEXT_TILE_SIZE )
      {
         Screen_DrawText( dialog->screen, dialog->lines[i], x, y );
      }

      if ( dialog->showCarat && !Dialog_IsDone( dialog ) )
      {
         Screen_DrawChar( dialog->screen, DOWNWARD_CARAT, x + ( ( dialog->lineWidth / 2 ) * TEXT_TILE_SIZE ), y );
      }
   }
}

Bool_t Dialog_StepAhead( Dialog_t* dialog )
{
   if ( dialog->isScrolling )
   {
      Dialog_FinishSection( dialog );
   }
   else if ( dialog->section < ( dialog->sectionCount - 1 ) )
   {
      Dialog_NextSection( dialog );
      return True;
   }

   return False;
}

void Dialog_NextSection( Dialog_t* dialog )
{
   dialog->section++;
   Dialog_LoadMessage( dialog );
   Dialog_ResetScroll( dialog );
}

void Dialog_Tic( Dialog_t* dialog )
{
   if ( dialog->isScrolling )
   {
      dialog->scrollSeconds += CLOCK_FRAME_SECONDS;

      if ( dialog->scrollSeconds > dialog->scrollTotalSeconds )
      {
         Dialog_FinishSection( dialog );
      }
   }
   else if ( !Dialog_IsDone( dialog ) )
   {
      dialog->blinkSeconds += CLOCK_FRAME_SECONDS;

      while ( dialog->blinkSeconds > CARAT_BLINK_RATE_SECONDS )
      {
         TOGGLE_BOOL( dialog->showCarat );
         dialog->blinkSeconds -= CARAT_BLINK_RATE_SECONDS;
      }
   }
}

Bool_t Dialog_IsDone( Dialog_t* dialog )
{
   return ( !dialog->isScrolling && ( dialog->section >= ( dialog->sectionCount - 1 ) ) ) ? True : False;
}

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
   dialog->sectionCount = Dialog_GetMessageSectionCount( dialog->id );

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

internal uint32_t Dialog_GetMessageSectionCount( DialogId_t id )
{
   switch ( id )
   {
      case DialogId_Talk_NobodyThere:
      case DialogId_Spell_None:
      case DialogId_Spell_OverworldCantCast:
      case DialogId_Spell_OverworldCastGlow:
      case DialogId_Spell_CastEvac:
      case DialogId_Spell_CastZoom:
      case DialogId_Spell_NotEnoughMp:
      case DialogId_Item_None:
      case DialogId_Door_None:
      case DialogId_Door_NoKeys:
      case DialogId_FullyHealed:
      case DialogId_HolyProtection_Off:
      case DialogId_Use_WingCantUse:
      case DialogId_Use_Wing:
      case DialogId_Use_TorchCantUse:
      case DialogId_Use_Torch:
      case DialogId_Use_GwaelynsLoveCantUse:
      case DialogId_Use_RainbowDropCantUse:
      case DialogId_Use_RainbowDrop:
      case DialogId_Chest_ItemCollected:
      case DialogId_Chest_GoldCollected:
      case DialogId_Battle_EnemyApproaches:
         return 1;
      case DialogId_Search_NothingFound:
      case DialogId_Search_FoundItem:
      case DialogId_Search_FoundHiddenStairs:
      case DialogId_Use_Herb1:
      case DialogId_Use_Herb2:
      case DialogId_Use_FairyWaterCursed:
      case DialogId_Use_FairyWater:
      case DialogId_Use_FairyFlute:
      case DialogId_Use_SilverHarp:
      case DialogId_Use_CursedBelt:
      case DialogId_Spell_OverworldCastGlowCursed:
      case DialogId_Spell_CastRepelCursed:
      case DialogId_Spell_CastRepel:
      case DialogId_Spell_CastEvacCursed:
      case DialogId_Chest_ItemNoSpace:
      case DialogId_Chest_GoldNoSpace:
      case DialogId_Chest_DeathNecklace:
      case DialogId_Spell_OverworldCastHeal1:
      case DialogId_Spell_OverworldCastHeal2:
      case DialogId_Spell_OverworldCastMidheal1:
      case DialogId_Spell_OverworldCastMidheal2:
      case DialogId_Spell_Blocked:
         return 2;
      case DialogId_Use_GwaelynsLove:
         return 4;
      case DialogId_Chest_Tablet:
         return 8;
   }

   return 0;
}

internal void Dialog_GetMessageText( Dialog_t* dialog, char* text )
{
   uint32_t e;
   Player_t* player = dialog->player;

   switch ( dialog->id )
   {
      case DialogId_Talk_NobodyThere: strcpy( text, STRING_OVERWORLD_DIALOG_NOBODY_THERE ); return;
      case DialogId_Search_NothingFound:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_OVERWORLD_DIALOG_SEARCH ); return;
            case 1: strcpy( text, STRING_OVERWORLD_DIALOG_SEARCH_NOT_FOUND ); return;
         }
      case DialogId_Search_FoundItem:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_OVERWORLD_DIALOG_SEARCH ); return;
            case 1: sprintf( text, STRING_OVERWORLD_DIALOG_SEARCH_FOUND, dialog->insertionText ); return;
         }
      case DialogId_Search_FoundHiddenStairs:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_OVERWORLD_DIALOG_SEARCH ); return;
            case 1: strcpy( text, STRING_FOUND_HIDDENSTAIRS ); return;
         }
      case DialogId_Spell_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_SPELLS ); return;
      case DialogId_Spell_OverworldCantCast: strcpy( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CANT_CAST ); return;
      case DialogId_Spell_OverworldCastGlow: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_GLOW ); return;
      case DialogId_Spell_OverworldCastGlowCursed:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_GLOW ); return;
            case 1: strcpy( text, STRING_GLOW_CURSED ); return;
         }
      case DialogId_Spell_CastRepelCursed:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_REPEL ); return;
            case 1: strcpy( text, STRING_HOLYPROTECTION_CURSED ); return;
         }
      case DialogId_Spell_CastEvacCursed:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_EVAC ); return;
            case 1: strcpy( text, STRING_EVAC_CURSED ); return;
         }
      case DialogId_Spell_CastRepel:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_REPEL ); return;
            case 1: strcpy( text, STRING_HOLYPROTECTION_ON ); return;
         }
      case DialogId_Spell_CastEvac: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_EVAC ); return;
      case DialogId_Spell_CastZoom: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_ZOOM ); return;
      case DialogId_Spell_NotEnoughMp: strcpy( text, STRING_NOTENOUGHMP ); return;
      case DialogId_Item_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_ITEMS ); return;
      case DialogId_Door_None: strcpy( text, STRING_OVERWORLD_DIALOG_NO_DOOR ); return;
      case DialogId_Door_NoKeys: strcpy( text, STRING_OVERWORLD_DIALOG_NO_KEYS ); return;
      case DialogId_FullyHealed: strcpy( text, STRING_FULLYHEALED ); return;
      case DialogId_HolyProtection_Off: strcpy( text, STRING_HOLYPROTECTION_OFF ); return;
      case DialogId_Use_Herb1:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_HERB ); return;
            case 1: sprintf( text, STRING_OVERWORLD_DIALOG_HEAL_RESULT_1, dialog->insertionText ); return;
         }
      case DialogId_Use_Herb2:
         switch ( dialog->section )
         {
            case 0: strcpy( text, STRING_ITEMUSE_HERB ); return;
            case 1: sprintf( text, STRING_OVERWORLD_DIALOG_HEAL_RESULT_2, dialog->insertionText ); return;
         }
      case DialogId_Use_Wing: strcpy( text, STRING_ITEMUSE_WING ); return;
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
      case DialogId_Use_WingCantUse: strcpy( text, STRING_ITEMUSE_WING_CANTUSE ); return;
      case DialogId_Use_TorchCantUse: strcpy( text, STRING_ITEMUSE_TORCH_CANTUSE ); return;
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
      case DialogId_Chest_ItemCollected: sprintf( text, STRING_CHEST_ITEMFOUND, dialog->insertionText ); return;
      case DialogId_Chest_ItemNoSpace:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_CHEST_ITEMFOUND, dialog->insertionText ); return;
            case 1: strcpy( text, STRING_CHEST_ITEMNOSPACE ); return;
         }
      case DialogId_Chest_GoldCollected: sprintf( text, STRING_CHEST_GOLDFOUND, dialog->insertionText ); return;
      case DialogId_Chest_GoldNoSpace:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_CHEST_GOLDFOUND, dialog->insertionText ); return;
            case 1: strcpy( text, STRING_CHEST_GOLDNOSPACE ); return;
         }
      case DialogId_Chest_Tablet:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_TABLET ); return;
            case 1: strcpy( text, STRING_CHEST_TABLET_1 ); return;
            case 2: strcpy( text, STRING_CHEST_TABLET_2 ); return;
            case 3: strcpy( text, STRING_CHEST_TABLET_3 ); return;
            case 4: strcpy( text, STRING_CHEST_TABLET_4 ); return;
            case 5: strcpy( text, STRING_CHEST_TABLET_5 ); return;
            case 6: strcpy( text, STRING_CHEST_TABLET_6 ); return;
            case 7: strcpy( text, STRING_CHEST_TABLET_7 ); return;
         }
      case DialogId_Chest_DeathNecklace:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_CHEST_ITEMFOUND, STRING_CHESTCOLLECT_DEATHNECKLACE ); return;
            case 1: strcpy( text, STRING_CURSED ); return;
         }
      case DialogId_Spell_OverworldCastHeal1:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_HEAL ); return;
            case 1: sprintf( text, STRING_OVERWORLD_DIALOG_HEAL_RESULT_1, dialog->insertionText ); return;
         }
      case DialogId_Spell_OverworldCastHeal2:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_HEAL ); return;
            case 1: sprintf( text, STRING_OVERWORLD_DIALOG_HEAL_RESULT_2, dialog->insertionText ); return;
         }
      case DialogId_Spell_OverworldCastMidheal1:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_MIDHEAL ); return;
            case 1: sprintf( text, STRING_OVERWORLD_DIALOG_HEAL_RESULT_1, dialog->insertionText ); return;
         }
      case DialogId_Spell_OverworldCastMidheal2:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELLMENU_MIDHEAL ); return;
            case 1: sprintf( text, STRING_OVERWORLD_DIALOG_HEAL_RESULT_2, dialog->insertionText ); return;
         }
      case DialogId_Spell_Blocked:
         switch ( dialog->section )
         {
            case 0: sprintf( text, STRING_OVERWORLD_DIALOG_SPELLS_OVERWORLD_CAST, dialog->insertionText ); return;
            case 1: strcpy( text, STRING_SPELLBLOCKED ); return;
         }
      case DialogId_Battle_EnemyApproaches:
         sprintf( text, STRING_BATTLE_ENEMYAPPROACHES, dialog->insertionText ); return;
   }
}

internal void Dialog_FinishSection( Dialog_t* dialog )
{
   dialog->isScrolling = False;

   if ( dialog->section == 0 )
   {
      switch ( dialog->id )
      {
         case DialogId_Spell_OverworldCastHeal1:
         case DialogId_Spell_OverworldCastHeal2:
         case DialogId_Spell_OverworldCastMidheal1:
         case DialogId_Spell_OverworldCastMidheal2:
         case DialogId_Spell_OverworldCastGlowCursed:
         case DialogId_Spell_OverworldCastGlow:
         case DialogId_Spell_CastRepelCursed:
         case DialogId_Spell_CastRepel:
         case DialogId_Spell_CastEvacCursed:
         case DialogId_Spell_CastEvac:
         case DialogId_Spell_CastZoom:
         case DialogId_Spell_Blocked:
            Animation_Start( dialog->animation, AnimationId_CastSpell );
            break;
      }
   }
}
