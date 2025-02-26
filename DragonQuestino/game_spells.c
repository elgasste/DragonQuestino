#include "game.h"

void Game_CastHeal( Game_t* game )
{
   if ( game->player.stats.magicPoints < SPELL_HEAL_MP )
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Spell_NotEnoughMp );
   }
   else
   {
      // TODO
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
      // TODO
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
   else
   {
      // TODO
   }
}
