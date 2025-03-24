#include "game.h"
#include "random.h"

#define CHECK_CAST_ABILITY( m, n ) if ( !Game_CanCastSpell( game, m, n ) ) return;

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName );

void Game_CastHeal( Game_t* game )
{
   uint8_t maxEffect;

   CHECK_CAST_ABILITY( SPELL_HEAL_MP, STRING_SPELLMENU_HEAL );

   if ( game->player.stats.hitPoints == UINT8_MAX )
   {
      Game_OpenScrollingDialog( game, DialogMessageId_FullyHealed );
   }
   else
   {
      maxEffect = game->player.isCursed ? ( SPELL_HEAL_MAXEFFECT / 2 ) : SPELL_HEAL_MAXEFFECT;
      game->player.stats.magicPoints -= SPELL_HEAL_MP;
      Game_DrawOverworldQuickStatus( game );
      Game_ApplyHealing( game, SPELL_HEAL_MINEFFECT, maxEffect,
                         DialogMessageId_Spell_OverworldCastHeal1, DialogMessageId_Spell_OverworldCastHeal2 );
   }
}

void Game_CastGlow( Game_t* game )
{
   CHECK_CAST_ABILITY( SPELL_GLOW_MP, STRING_SPELLMENU_GLOW );

   if ( game->tileMap.isDark )
   {
      game->player.stats.magicPoints -= SPELL_GLOW_MP;
      Game_DrawOverworldQuickStatus( game );

      if ( game->player.isCursed )
      {
         TileMap_SetTargetGlowDiameter( &( game->tileMap ), 1 );
         Game_OpenScrollingDialog( game, DialogMessageId_Spell_OverworldCastGlowCursed );
      }
      else
      {
         if ( game->tileMap.glowDiameter <= GLOW_SPELL_DIAMETER )
         {
            TileMap_SetTargetGlowDiameter( &( game->tileMap ), GLOW_SPELL_DIAMETER );
            game->tileMap.glowTileCount = 0;
            TileMap_StartGlowTransition( &( game->tileMap ) );
         }

         Game_OpenScrollingDialog( game, DialogMessageId_Spell_OverworldCastGlow );
      }
   }
}

void Game_CastEvac( Game_t* game )
{
   CHECK_CAST_ABILITY( SPELL_EVAC_MP, STRING_SPELLMENU_EVAC );

   if ( game->tileMap.isDungeon )
   {
      game->player.stats.magicPoints -= SPELL_EVAC_MP;
      Game_DrawOverworldQuickStatus( game );

      if ( game->player.isCursed )
      {
         Game_OpenScrollingDialog( game, DialogMessageId_Spell_CastEvacCursed );
      }
      else
      {
         Game_OpenScrollingDialog( game, DialogMessageId_Spell_CastEvac );
      }
   }
}

void Game_CastZoom( Game_t* game, uint32_t townId )
{
   CHECK_CAST_ABILITY( SPELL_ZOOM_MP, STRING_SPELLMENU_ZOOM );

   game->player.stats.magicPoints -= SPELL_ZOOM_MP;
   game->targetPortal = &( game->zoomPortals[townId] );
   Game_DrawOverworldQuickStatus( game );
   Game_OpenScrollingDialog( game, DialogMessageId_Spell_CastZoom );
}

void Game_CastRepel( Game_t* game )
{
   CHECK_CAST_ABILITY( SPELL_REPEL_MP, STRING_SPELLMENU_REPEL );

   game->player.stats.magicPoints -= SPELL_REPEL_MP;
   Game_DrawOverworldQuickStatus( game );

   if ( game->player.isCursed )
   {
      Game_OpenScrollingDialog( game, DialogMessageId_Spell_CastRepelCursed );
   }
   else
   {
      Player_SetHolyProtection( &( game->player ), True );
      Game_OpenScrollingDialog( game, DialogMessageId_Spell_CastRepel );
   }
}

void Game_CastMidheal( Game_t* game )
{
   uint8_t maxEffect;

   CHECK_CAST_ABILITY( SPELL_MIDHEAL_MP, STRING_SPELLMENU_MIDHEAL );

   if ( game->player.stats.hitPoints == UINT8_MAX )
   {
      Game_OpenScrollingDialog( game, DialogMessageId_FullyHealed );
   }
   else
   {
      maxEffect = game->player.isCursed ? ( SPELL_MIDHEAL_MAXEFFECT / 2 ) : SPELL_MIDHEAL_MAXEFFECT;
      game->player.stats.magicPoints -= SPELL_MIDHEAL_MP;
      Game_DrawOverworldQuickStatus( game );
      Game_ApplyHealing( game, SPELL_MIDHEAL_MINEFFECT, maxEffect,
                         DialogMessageId_Spell_OverworldCastMidheal1, DialogMessageId_Spell_OverworldCastMidheal2 );
   }
}

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName )
{
   if ( game->player.stats.magicPoints < requiredMp )
   {
      Game_OpenScrollingDialog( game, DialogMessageId_Spell_NotEnoughMp );
      return False;
   }
   else if ( game->tileMap.blocksMagic )
   {
      game->player.stats.magicPoints -= requiredMp;
      Game_DrawOverworldQuickStatus( game );
      ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), spellName );
      Game_OpenScrollingDialog( game, DialogMessageId_Spell_Blocked );
      return False;
   }

   return True;
}
