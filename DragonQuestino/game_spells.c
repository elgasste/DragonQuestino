#include "game.h"
#include "random.h"

#define CHECK_CAST_ABILITY( m, n ) if ( !Game_CanCastSpell( game, m, n ) ) return;

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName );
internal void Game_ResetBattleMenu( Game_t* game );

void Game_CastHeal( Game_t* game )
{
   uint8_t maxEffect;
   DialogId_t dialogId;

   CHECK_CAST_ABILITY( SPELL_HEAL_MP, STRING_SPELL_HEAL );
   Game_ResetBattleMenu( game );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      dialogId = ( game->mainState == MainState_Overworld ) ? DialogId_FullyHealed : DialogId_Battle_FullyHealed;
      Game_OpenDialog( game, dialogId );
   }
   else
   {
      maxEffect = game->player.isCursed ? ( SPELL_HEAL_MAXEFFECT / 2 ) : SPELL_HEAL_MAXEFFECT;
      game->player.stats.magicPoints -= SPELL_HEAL_MP;
      Game_ApplyHealing( game, SPELL_HEAL_MINEFFECT, maxEffect,
                         DialogId_Spell_CastHeal1, DialogId_Spell_CastHeal2 );
      Game_DrawQuickStatus( game );
   }
}

void Game_CastSizz( Game_t* game )
{
   // TODO
   UNUSED_PARAM( game );
}

void Game_CastSleep( Game_t* game )
{
   // TODO
   UNUSED_PARAM( game );
}

void Game_CastGlow( Game_t* game )
{
   CHECK_CAST_ABILITY( SPELL_GLOW_MP, STRING_SPELL_GLOW );

   if ( game->tileMap.isDark )
   {
      game->player.stats.magicPoints -= SPELL_GLOW_MP;
      Game_DrawQuickStatus( game );

      if ( game->player.isCursed )
      {
         game->tileMap.torchIsLit = False;
         TileMap_SetTargetGlowDiameter( &( game->tileMap ), 1 );
         Game_OpenDialog( game, DialogId_Spell_OverworldCastGlowCursed );
      }
      else
      {
         if ( game->tileMap.glowDiameter <= GLOW_SPELL_DIAMETER )
         {
            TileMap_SetTargetGlowDiameter( &( game->tileMap ), GLOW_SPELL_DIAMETER );
            game->tileMap.glowTileCount = 0;
            TileMap_StartGlowTransition( &( game->tileMap ) );
         }

         Game_OpenDialog( game, DialogId_Spell_OverworldCastGlow );
      }
   }
}

void Game_CastFizzle( Game_t* game )
{
   // TODO
   UNUSED_PARAM( game );
}

void Game_CastEvac( Game_t* game )
{
   CHECK_CAST_ABILITY( SPELL_EVAC_MP, STRING_SPELL_EVAC );

   if ( game->tileMap.isDungeon )
   {
      game->player.stats.magicPoints -= SPELL_EVAC_MP;
      Game_DrawQuickStatus( game );

      if ( game->player.isCursed )
      {
         Game_OpenDialog( game, DialogId_Spell_CastEvacCursed );
      }
      else
      {
         Game_OpenDialog( game, DialogId_Spell_CastEvac );
      }
   }
}

void Game_CastZoom( Game_t* game, uint32_t townId )
{
   CHECK_CAST_ABILITY( SPELL_ZOOM_MP, STRING_SPELL_ZOOM );

   game->player.stats.magicPoints -= SPELL_ZOOM_MP;
   game->targetPortal = &( game->zoomPortals[townId] );
   Game_DrawQuickStatus( game );
   Game_OpenDialog( game, DialogId_Spell_CastZoom );
}

void Game_CastRepel( Game_t* game )
{
   CHECK_CAST_ABILITY( SPELL_REPEL_MP, STRING_SPELL_REPEL );

   game->player.stats.magicPoints -= SPELL_REPEL_MP;
   Game_DrawQuickStatus( game );

   if ( game->player.isCursed )
   {
      Game_OpenDialog( game, DialogId_Spell_CastRepelCursed );
   }
   else
   {
      Player_SetHolyProtection( &( game->player ), True );
      Game_OpenDialog( game, DialogId_Spell_CastRepel );
   }
}

void Game_CastMidheal( Game_t* game )
{
   uint8_t maxEffect;
   DialogId_t dialogId;

   CHECK_CAST_ABILITY( SPELL_MIDHEAL_MP, STRING_SPELL_MIDHEAL );
   Game_ResetBattleMenu( game );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      dialogId = ( game->mainState == MainState_Overworld ) ? DialogId_FullyHealed : DialogId_Battle_FullyHealed;
      Game_OpenDialog( game, dialogId );
   }
   else
   {
      maxEffect = game->player.isCursed ? ( SPELL_MIDHEAL_MAXEFFECT / 2 ) : SPELL_MIDHEAL_MAXEFFECT;
      game->player.stats.magicPoints -= SPELL_MIDHEAL_MP;
      Game_ApplyHealing( game, SPELL_MIDHEAL_MINEFFECT, maxEffect,
                         DialogId_Spell_CastMidheal1, DialogId_Spell_CastMidheal2 );
      Game_DrawQuickStatus( game );
   }
}

void Game_CastSizzle( Game_t* game )
{
   // TODO
   UNUSED_PARAM( game );
}

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName )
{
   DialogId_t dialogId;

   if ( game->player.stats.magicPoints < requiredMp )
   {
      dialogId = ( game->mainState == MainState_Overworld ) ? DialogId_Spell_NotEnoughMp : DialogId_Battle_Spell_NotEnoughMp;
      Game_OpenDialog( game, dialogId );
      return False;
   }

   if ( game->tileMap.blocksMagic )
   {
      game->screen.needsRedraw = True;
      game->player.stats.magicPoints -= requiredMp;
      Game_DrawQuickStatus( game );
      Dialog_SetInsertionText( &( game->dialog ), spellName );
      dialogId = game->mainState == MainState_Overworld ? DialogId_Spell_Blocked : DialogId_Battle_Spell_Blocked;
      Game_OpenDialog( game, dialogId );
      return False;
   }
   else if ( game->mainState == MainState_Battle && game->player.stats.isFizzled )
   {
      game->screen.needsRedraw = True;
      game->player.stats.magicPoints -= requiredMp;
      Game_DrawQuickStatus( game );
      Dialog_SetInsertionText( &( game->dialog ), spellName );
      Game_OpenDialog( game, DialogId_Battle_Spell_Fizzled );
      return False;
   }

   return True;
}

internal void Game_ResetBattleMenu( Game_t* game )
{
   if ( game->mainState == MainState_Battle )
   {
      game->screen.needsRedraw = True;
      game->activeMenu = &( game->menus[MenuId_Battle] );
      Menu_Reset( game->activeMenu );
      game->screen.needsRedraw = True;
      Game_ChangeSubState( game, SubState_Menu );
   }
}
