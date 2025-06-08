#include "game.h"
#include "random.h"
#include "math.h"

#define CHECK_CAST_ABILITY( m, n ) if ( !Game_CanCastSpell( game, m, n ) ) return;

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName );
internal void Game_ResetBattleMenu( Game_t* game );
internal void Game_SpellBlockedCallback( Game_t* game );
internal void Game_SpellBlockedMessageCallback( Game_t* game );
internal void Game_SpellFizzledCallback( Game_t* game );
internal void Game_SpellFizzledMessageCallback( Game_t* game );
internal void Game_CastSpellCallback( Game_t* game );
internal void Game_SpellHealCallback( Game_t* game );
internal void Game_SpellHurtCallback( Game_t* game );

void Game_CastHeal( Game_t* game )
{
   uint8_t maxEffect;
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_HEAL_MP, STRING_SPELL_HEAL );

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog2_Reset( &( game->dialog2 ) );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Dialog2_PushSection( &( game->dialog2 ), ( game->mainState == MainState_Overworld ) ? STRING_FULLYHEALED : STRING_BATTLE_FULLYHEALED );
   }
   else
   {
      game->player.stats.magicPoints -= SPELL_HEAL_MP;
      game->pendingSpell = Spell_Heal;
      maxEffect = game->player.isCursed ? ( SPELL_HEAL_MAXEFFECT / 2 ) : SPELL_HEAL_MAXEFFECT;
      game->pendingPayload8u = MATH_MIN( Random_u8( SPELL_HEAL_MINEFFECT, maxEffect ), game->player.stats.maxHitPoints - game->player.stats.hitPoints );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, STRING_SPELL_HEAL );
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_CastSpellCallback, game );
   }

   Game_OpenDialog2( game );
}

void Game_CastSizz( Game_t* game )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_SIZZ_MP, STRING_SPELL_SIZZ );

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog2_Reset( &( game->dialog2 ) );
   sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, STRING_SPELL_SIZZ );
   Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_CastSpellCallback, game );
   game->pendingSpell = Spell_Sizz;
   game->player.stats.magicPoints -= SPELL_SIZZ_MP;

   if ( Random_u8( 0, 15 ) <= game->battle.enemy.stats.hurtResist )
   {
      game->pendingPayload8u = 0;
   }
   else
   {
      game->pendingPayload8u = Random_u8( SPELL_SIZZ_MINEFFECT, SPELL_SIZZ_MAXEFFECT );
   }

   Game_OpenDialog2( game );
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
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_MIDHEAL_MP, STRING_SPELL_MIDHEAL );

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog2_Reset( &( game->dialog2 ) );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Dialog2_PushSection( &( game->dialog2 ), ( game->mainState == MainState_Overworld ) ? STRING_FULLYHEALED : STRING_BATTLE_FULLYHEALED );
   }
   else
   {
      game->player.stats.magicPoints -= SPELL_MIDHEAL_MP;
      game->pendingSpell = Spell_Midheal;
      maxEffect = game->player.isCursed ? ( SPELL_MIDHEAL_MAXEFFECT / 2 ) : SPELL_MIDHEAL_MAXEFFECT;
      game->pendingPayload8u = MATH_MIN( Random_u8( SPELL_MIDHEAL_MINEFFECT, maxEffect ), game->player.stats.maxHitPoints - game->player.stats.hitPoints );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, STRING_SPELL_MIDHEAL );
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_CastSpellCallback, game );
   }

   Game_OpenDialog2( game );
}

void Game_CastSizzle( Game_t* game )
{
   CHECK_CAST_ABILITY( SPELL_SIZZLE_MP, STRING_SPELL_SIZZLE );
   Game_ResetBattleMenu( game );

   game->player.stats.magicPoints -= SPELL_SIZZLE_MP;
   Game_DrawQuickStatus( game );

   if ( Random_u8( 0, 15 ) <= game->battle.enemy.stats.hurtResist )
   {
      game->pendingPayload8u = 0;
   }
   else
   {
      game->pendingPayload8u = Random_u8( SPELL_SIZZLE_MINEFFECT, SPELL_SIZZLE_MAXEFFECT );
   }

   Dialog_SetInsertionText( &( game->dialog ), STRING_SPELL_SIZZLE );
   Game_OpenDialog( game, DialogId_Battle_Spell_Sizzle );
}

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName )
{
   char msg[64];

   if ( game->player.stats.magicPoints < requiredMp )
   {
      Dialog2_Reset( &( game->dialog2 ) );
      Dialog2_PushSection( &( game->dialog2 ), ( game->mainState == MainState_Overworld ) ? STRING_NOTENOUGHMP : STRING_BATTLE_NOTENOUGHMP );
      Game_OpenDialog2( game );
      return False;
   }

   if ( game->tileMap.blocksMagic )
   {
      game->player.stats.magicPoints -= requiredMp;
      Dialog2_Reset( &( game->dialog2 ) );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, spellName );
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_SpellBlockedCallback, game );
      Game_OpenDialog2( game );
      return False;
   }

   if ( game->mainState == MainState_Battle && game->player.stats.isFizzled )
   {
      game->player.stats.magicPoints -= requiredMp;
      Dialog2_Reset( &( game->dialog2 ) );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, spellName );
      Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_SpellFizzledCallback, game );
      Game_OpenDialog2( game );
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

internal void Game_SpellBlockedCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_CastSpell, Game_DrawQuickStatus, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellBlockedMessageCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_SpellBlockedMessageCallback( Game_t* game )
{
   Dialog2_Reset( &( game->dialog2 ) );
   Dialog2_PushSection( &( game->dialog2 ), STRING_SPELLBLOCKED );
   Game_OpenDialog2( game );
}

internal void Game_SpellFizzledCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_CastSpell, Game_DrawQuickStatus, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellFizzledMessageCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_SpellFizzledMessageCallback( Game_t* game )
{
   Dialog2_Reset( &( game->dialog2 ) );
   Dialog2_PushSection( &( game->dialog2 ), STRING_BATTLE_SPELLFIZZLED );
   Game_OpenDialog2( game );
}

internal void Game_CastSpellCallback( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_CastSpell, Game_DrawQuickStatus, game );

   switch ( game->pendingSpell )
   {
      case Spell_Heal:
      case Spell_Midheal:
         AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellHealCallback, game );
         break;
      case Spell_Sizz:
      case Spell_Sizzle:
         AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellHurtCallback, game );
         break;
   }

   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_SpellHealCallback( Game_t* game )
{
   char msg[64];

   Dialog2_Reset( &( game->dialog2 ) );
   sprintf( msg, STRING_DIALOG_HEAL_RESULT, game->pendingPayload8u, ( game->pendingPayload8u == 1 ) ? STRING_POINT : STRING_POINTS );
   Dialog2_PushSectionWithCallback( &( game->dialog2 ), msg, Game_RestoredHitPointsCallback, game );
   Game_OpenDialog2( game );
}

internal void Game_SpellHurtCallback( Game_t* game )
{
   UNUSED_PARAM( game );

   // MUFFINS: this is what ApplySizz did, let's re-visit this when we have battle working

   /*
   char msg[64];
   uint8_t damage;

   if ( game->pendingPayload8u == 0 )
   {
      Dialog_SetInsertionText( &( game->dialog ), game->battle.enemy.name );
      Game_OpenDialog( game, DialogId_Battle_Spell_NoEffect );
   }
   else
   {
      Dialog2_Draw( &( game->dialog2 ) );
      Animation_Start( &( game->animation ), AnimationId_Battle_EnemyDamage );
      damage = ( game->battle.enemy.stats.hitPoints > game->pendingPayload8u ) ? game->pendingPayload8u : game->battle.enemy.stats.hitPoints;
      game->battle.enemy.stats.hitPoints -= damage;
      sprintf( msg, STRING_BATTLE_ATTACKATTEMPTSUCCEEDED, game->battle.enemy.name, damage, ( damage == 1 ) ? STRING_POINT : STRING_POINTS );
      Dialog_SetInsertionText( &( game->dialog ), msg );
      Game_OpenDialog( game, DialogId_Battle_Spell_AttackSucceeded );
   }
   */
}
