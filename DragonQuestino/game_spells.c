#include "game.h"
#include "random.h"

internal void Game_ApplyHealingSpell( Game_t* game, uint8_t mp, uint8_t minHp, uint8_t maxHp, DialogMessageId_t msg1, DialogMessageId_t msg2 );

void Game_CastHeal( Game_t* game )
{
   if ( game->player.stats.magicPoints < SPELL_HEAL_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else if ( game->player.stats.hitPoints == UINT8_MAX )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_FullyHealed );
   }
   else
   {
      Game_ApplyHealingSpell( game, SPELL_HEAL_MP,
                              SPELL_HEAL_MINEFFECT, SPELL_HEAL_MAXEFFECT,
                              DialogMessageId_Spell_OverworldCastHeal1, DialogMessageId_Spell_OverworldCastHeal2 );
   }
}

void Game_CastGlow( Game_t* game )
{
   if ( game->player.stats.magicPoints < SPELL_GLOW_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else
   {
      if ( game->tileMap.glowDiameter <= GLOW_SPELL_DIAMETER )
      {
         TileMap_SetTargetGlowDiameter( &( game->tileMap ), GLOW_SPELL_DIAMETER );
         game->tileMap.glowTileCount = 0;
         game->glowExpandSeconds = GLOW_EXPAND_FRAME_SECONDS; // push one frame immediately
      }

      game->player.stats.magicPoints -= SPELL_GLOW_MP;
      Game_DrawOverworldQuickStatus( game );
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_OverworldCastGlow );
   }
}

void Game_CastEvac( Game_t* game )
{
   if ( game->player.stats.magicPoints < SPELL_EVAC_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else
   {
      // TODO
   }
}

void Game_CastZoom( Game_t* game )
{
   if ( game->player.stats.magicPoints < SPELL_ZOOM_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else
   {
      // TODO
   }
}

void Game_CastRepel( Game_t* game )
{
   if ( game->player.stats.magicPoints < SPELL_REPEL_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else
   {
      // TODO
   }
}

void Game_CastMidheal( Game_t* game )
{
   if ( game->player.stats.magicPoints < SPELL_MIDHEAL_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else if ( game->player.stats.hitPoints == UINT8_MAX )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_FullyHealed );
   }
   else
   {
      Game_ApplyHealingSpell( game, SPELL_MIDHEAL_MP,
                              SPELL_MIDHEAL_MINEFFECT, SPELL_MIDHEAL_MAXEFFECT,
                              DialogMessageId_Spell_OverworldCastMidheal1, DialogMessageId_Spell_OverworldCastMidheal2 );
   }
}

internal void Game_ApplyHealingSpell( Game_t* game, uint8_t mp, uint8_t minHp, uint8_t maxHp, DialogMessageId_t msg1, DialogMessageId_t msg2 )
{
   uint8_t amount;
   char str[16];

   game->player.stats.magicPoints -= mp;
   amount = Player_RestoreHitPoints( &( game->player ), Random_u8( minHp, maxHp ) );
   sprintf( str, "%u", amount );
   Game_DrawOverworldQuickStatus( game );
   ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), str );
   Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, ( amount == 1 ) ? msg1: msg2 );
}
