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
   CHECK_CAST_ABILITY( SPELL_SIZZ_MP, STRING_SPELL_SIZZ );
   Game_ResetBattleMenu( game );

   game->player.stats.magicPoints -= SPELL_SIZZ_MP;
   Game_DrawQuickStatus( game );

   if ( Random_u8( 0, 15 ) <= game->battle.enemy.stats.hurtResist )
   {
      game->pendingPayload8u = 0;
   }
   else
   {
      game->pendingPayload8u = Random_u8( SPELL_SIZZ_MINEFFECT, SPELL_SIZZ_MAXEFFECT );
   }

   Dialog_SetInsertionText( &( game->dialog ), STRING_SPELL_SIZZ );
   Game_OpenDialog( game, DialogId_Battle_Spell_Sizz );
}

void Game_ApplySizz( Game_t* game )
{
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

void Game_ApplySizzle( Game_t* game )
{
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
