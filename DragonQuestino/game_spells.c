#include "game.h"
#include "random.h"
#include "math.h"

#define CHECK_CAST_ABILITY( m, n ) if ( !Game_CanCastSpell( game, m, n ) ) return;

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName );
internal void Game_SpellBlockedCallback( Game_t* game );
internal void Game_SpellBlockedMessageCallback( Game_t* game );
internal void Game_SpellFizzledCallback( Game_t* game );
internal void Game_SpellFizzledMessageCallback( Game_t* game );
internal void Game_CastSpellCallback( Game_t* game );
internal void Game_SpellHealCallback( Game_t* game );
internal void Game_SpellHurtCallback( Game_t* game );
internal void Game_SpellSleepCallback( Game_t* game );
internal void Game_SpellSleepSuccessCallback( Game_t* game );
internal void Game_SpellFizzleCallback( Game_t* game );
internal void Game_SpellFizzleSuccessCallback( Game_t* game );
internal void Game_SpellZoomCallback( Game_t* game );
internal void Game_SpellRepelCallback( Game_t* game );
internal void Game_SpellGlowCallback( Game_t* game );
internal void Game_SpellEvacCallback( Game_t* game );
internal void Game_SpellAnimateNoEffect( Game_t* game );
internal void Game_SpellNoEffectCallback( Game_t* game );

void Game_CastHeal( Game_t* game )
{
   uint8_t maxEffect;
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_HEAL_MP, STRING_SPELL_HEAL );

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog_Reset( &( game->dialog ) );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Dialog_PushSectionWithCallback( &( game->dialog ),
                                       ( game->mainState == MainState_Overworld ) ? STRING_FULLYHEALED : STRING_BATTLE_FULLYHEALED,
                                       ( game->mainState == MainState_Overworld ) ? 0 : Game_ResetBattleMenu,
                                       ( game->mainState == MainState_Overworld ) ? 0 : game );
   }
   else
   {
      game->pendingSpell = Spell_Heal;
      maxEffect = game->player.isCursed ? ( SPELL_HEAL_MAXEFFECT / 2 ) : SPELL_HEAL_MAXEFFECT;
      game->pendingPayload8u = MATH_MIN( Random_u8( SPELL_HEAL_MINEFFECT, maxEffect ), game->player.stats.maxHitPoints - game->player.stats.hitPoints );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, STRING_SPELL_HEAL );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
   }

   Game_OpenDialog( game );
}

void Game_CastSizz( Game_t* game )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_SIZZ_MP, STRING_SPELL_SIZZ );

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_BATTLE_SPELLCAST, STRING_SPELL_SIZZ );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
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

   Game_OpenDialog( game );
}

void Game_CastSleep( Game_t* game )
{
   char msg[64];
   
   CHECK_CAST_ABILITY( SPELL_SLEEP_MP, STRING_SPELL_SLEEP );

   if ( game->battle.enemy.stats.isAsleep )
   {
      game->screen.needsRedraw = True;
      Dialog_Reset( &( game->dialog ) );
      sprintf( msg, STRING_BATTLE_SPELL_SLEEP_ALREADYASLEEP, game->battle.enemy.name );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
      Game_OpenDialog( game );
      return;
   }

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_BATTLE_SPELLCAST, STRING_SPELL_SLEEP );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
   game->pendingSpell = Spell_Sleep;
   game->player.stats.magicPoints -= SPELL_SLEEP_MP;

   if ( game->battle.specialEnemy != SpecialEnemy_None )
   {
      game->pendingPayload8u = 0;
   }
   else
   {
      game->pendingPayload8u = ( Random_u8( 0, 15 ) <= game->battle.enemy.stats.sleepResist ) ? 0 : 1;
   }

   Game_OpenDialog( game );
}

void Game_CastGlow( Game_t* game )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_GLOW_MP, STRING_SPELL_GLOW );

   if ( game->tileMap.isDark )
   {
      game->screen.needsRedraw = True;
      Dialog_Reset( &( game->dialog ) );
      game->pendingSpell = Spell_Glow;
      sprintf( msg, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_GLOW );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
      Game_OpenDialog( game );
   }
}

void Game_CastFizzle( Game_t* game )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_FIZZLE_MP, STRING_SPELL_FIZZLE );

   if ( game->battle.enemy.stats.isFizzled )
   {
      game->screen.needsRedraw = True;
      Dialog_Reset( &( game->dialog ) );
      sprintf( msg, STRING_BATTLE_SPELL_FIZZLE_ALREADYFIZZLED, game->battle.enemy.name );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
      Game_OpenDialog( game );
      return;
   }

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_BATTLE_SPELLCAST, STRING_SPELL_FIZZLE );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
   game->pendingSpell = Spell_Fizzle;
   game->player.stats.magicPoints -= SPELL_FIZZLE_MP;
   game->pendingPayload8u = ( Random_u8( 0, 15 ) <= game->battle.enemy.stats.stopSpellResist ) ? 0 : 1;
   Game_OpenDialog( game );
}

void Game_CastEvac( Game_t* game )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_EVAC_MP, STRING_SPELL_EVAC );

   if ( game->tileMap.isDungeon )
   {
      game->screen.needsRedraw = True;
      Dialog_Reset( &( game->dialog ) );
      game->pendingSpell = Spell_Evac;
      sprintf( msg, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_EVAC );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
      Game_OpenDialog( game );
   }
}

void Game_CastZoom( Game_t* game, uint32_t townId )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_ZOOM_MP, STRING_SPELL_ZOOM );

   game->screen.needsRedraw = True;
   Dialog_Reset( &( game->dialog ) );
   game->pendingSpell = Spell_Zoom;
   game->targetPortal = &( game->zoomPortals[townId] );
   sprintf( msg, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_ZOOM );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
   Game_OpenDialog( game );
}

void Game_CastRepel( Game_t* game )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_REPEL_MP, STRING_SPELL_REPEL );

   game->screen.needsRedraw = True;
   Dialog_Reset( &( game->dialog ) );
   game->pendingSpell = Spell_Repel;
   sprintf( msg, STRING_DIALOG_SPELLS_OVERWORLD_CAST, STRING_SPELL_REPEL );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
   Game_OpenDialog( game );
}

void Game_CastMidheal( Game_t* game )
{
   uint8_t maxEffect;
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_MIDHEAL_MP, STRING_SPELL_MIDHEAL );

   Game_ResetBattleMenu( game );
   Dialog_Reset( &( game->dialog ) );

   if ( game->player.stats.hitPoints == game->player.stats.maxHitPoints )
   {
      Dialog_PushSectionWithCallback( &( game->dialog ),
                                       ( game->mainState == MainState_Overworld ) ? STRING_FULLYHEALED : STRING_BATTLE_FULLYHEALED,
                                       ( game->mainState == MainState_Overworld ) ? 0 : Game_ResetBattleMenu,
                                       ( game->mainState == MainState_Overworld ) ? 0 : game );
   }
   else
   {
      game->player.stats.magicPoints -= SPELL_MIDHEAL_MP;
      game->pendingSpell = Spell_Midheal;
      maxEffect = game->player.isCursed ? ( SPELL_MIDHEAL_MAXEFFECT / 2 ) : SPELL_MIDHEAL_MAXEFFECT;
      game->pendingPayload8u = MATH_MIN( Random_u8( SPELL_MIDHEAL_MINEFFECT, maxEffect ), game->player.stats.maxHitPoints - game->player.stats.hitPoints );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, STRING_SPELL_MIDHEAL );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
   }

   Game_OpenDialog( game );
}

void Game_CastSizzle( Game_t* game )
{
   char msg[64];

   CHECK_CAST_ABILITY( SPELL_SIZZLE_MP, STRING_SPELL_SIZZLE );

   game->screen.needsRedraw = True;
   Game_ResetBattleMenu( game );
   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_BATTLE_SPELLCAST, STRING_SPELL_SIZZLE );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_CastSpellCallback, game );
   game->pendingSpell = Spell_Sizzle;
   game->player.stats.magicPoints -= SPELL_SIZZLE_MP;

   if ( Random_u8( 0, 15 ) <= game->battle.enemy.stats.hurtResist )
   {
      game->pendingPayload8u = 0;
   }
   else
   {
      game->pendingPayload8u = Random_u8( SPELL_SIZZLE_MINEFFECT, SPELL_SIZZLE_MAXEFFECT );
   }

   Game_OpenDialog( game );
}

internal Bool_t Game_CanCastSpell( Game_t* game, uint8_t requiredMp, const char* spellName )
{
   char msg[64];

   if ( game->player.stats.magicPoints < requiredMp )
   {
      game->screen.needsRedraw = True;
      Dialog_Reset( &( game->dialog ) );
      Dialog_PushSectionWithCallback( &( game->dialog ),
                                       ( game->mainState == MainState_Overworld ) ? STRING_NOTENOUGHMP : STRING_BATTLE_NOTENOUGHMP,
                                       ( game->mainState == MainState_Overworld ) ? 0 : Game_ResetBattleMenu,
                                       ( game->mainState == MainState_Overworld ) ? 0 : game );
      Game_OpenDialog( game );
      return False;
   }

   if ( game->tileMap.blocksMagic )
   {
      game->screen.needsRedraw = True;
      game->player.stats.magicPoints -= requiredMp;
      Dialog_Reset( &( game->dialog ) );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, spellName );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_SpellBlockedCallback, game );
      Game_OpenDialog( game );
      return False;
   }

   if ( game->mainState == MainState_Battle && game->player.stats.isFizzled )
   {
      game->screen.needsRedraw = True;
      game->player.stats.magicPoints -= requiredMp;
      Dialog_Reset( &( game->dialog ) );
      sprintf( msg, ( game->mainState == MainState_Overworld ) ? STRING_DIALOG_SPELLS_OVERWORLD_CAST : STRING_BATTLE_SPELLCAST, spellName );
      Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_SpellFizzledCallback, game );
      Game_OpenDialog( game );
      return False;
   }

   return True;
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
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_SPELLBLOCKED,
                                    ( game->mainState == MainState_Overworld ) ? 0 : Game_ResetBattleMenu,
                                    ( game->mainState == MainState_Overworld ) ? 0 : game );
   Game_OpenDialog( game );
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
   Dialog_Reset( &( game->dialog ) );
   Dialog_PushSectionWithCallback( &( game->dialog ), STRING_BATTLE_SPELLFIZZLED,
                                    ( game->mainState == MainState_Overworld ) ? 0 : Game_ResetBattleMenu,
                                    ( game->mainState == MainState_Overworld ) ? 0 : game );
   Game_OpenDialog( game );
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
      case Spell_Fizzle: AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellFizzleCallback, game ); break;
      case Spell_Sleep: AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellSleepCallback, game ); break;
      case Spell_Zoom: AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellZoomCallback, game ); break;
      case Spell_Repel: AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellRepelCallback, game ); break;
      case Spell_Glow: AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellGlowCallback, game ); break;
      case Spell_Evac: AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellEvacCallback, game ); break;
   }

   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_SpellHealCallback( Game_t* game )
{
   char msg[64];

   if ( game->pendingSpell == Spell_Heal )
   {
      game->player.stats.magicPoints -= SPELL_HEAL_MP;
   }
   else
   {
      game->player.stats.magicPoints -= SPELL_MIDHEAL_MP;
   }

   Game_DrawQuickStatus( game );
   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_DIALOG_HEAL_RESULT, game->pendingPayload8u, ( game->pendingPayload8u == 1 ) ? STRING_POINT : STRING_POINTS );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_RestoredHitPointsCallback, game );
   Game_OpenDialog( game );
}

internal void Game_SpellHurtCallback( Game_t* game )
{
   if ( game->pendingPayload8u == 0 )
   {
      Game_SpellAnimateNoEffect( game );
   }
   else
   {
      game->battle.pendingPayload8u = ( game->battle.enemy.stats.hitPoints > game->pendingPayload8u )
                                      ? game->pendingPayload8u
                                      : game->battle.enemy.stats.hitPoints;
      AnimationChain_Reset( &( game->animationChain ) );
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Battle_EnemyDamage, Battle_AttackSucceededCallback, &( game->battle ) );
      AnimationChain_Start( &( game->animationChain ) );
   }
}

internal void Game_SpellSleepCallback( Game_t* game )
{
   if ( game->pendingPayload8u == 0 )
   {
      Game_SpellAnimateNoEffect( game );
   }
   else
   {
      AnimationChain_Reset( &( game->animationChain ) );
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellSleepSuccessCallback, game );
      AnimationChain_Start( &( game->animationChain ) );
   }
}

internal void Game_SpellSleepSuccessCallback( Game_t* game )
{
   char msg[64];

   game->battle.enemy.stats.isAsleep = True;
   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMYASLEEP, game->battle.enemy.name );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
   Game_OpenDialog( game );
}

internal void Game_SpellFizzleCallback( Game_t* game )
{
   if ( game->pendingPayload8u == 0 )
   {
      Game_SpellAnimateNoEffect( game );
   }
   else
   {
      AnimationChain_Reset( &( game->animationChain ) );
      AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellFizzleSuccessCallback, game );
      AnimationChain_Start( &( game->animationChain ) );
   }
}

internal void Game_SpellFizzleSuccessCallback( Game_t* game )
{
   char msg[64];

   game->battle.enemy.stats.isFizzled = True;
   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_BATTLE_ENEMYFIZZLED, game->battle.enemy.name );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
   Game_OpenDialog( game );
}

internal void Game_SpellZoomCallback( Game_t* game )
{
   game->player.stats.magicPoints -= SPELL_ZOOM_MP;
   Game_ChangeToOverworldState( game );
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_WhiteOut, Game_EnterTargetPortal, game );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_ChangeToOverworldState, game );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_Pause );
   AnimationChain_PushAnimation( &( game->animationChain ), AnimationId_WhiteIn );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_SpellRepelCallback( Game_t* game )
{
   game->player.stats.magicPoints -= SPELL_ZOOM_MP;
   Game_DrawQuickStatus( game );
   Dialog_Reset( &( game->dialog ) );

   if ( game->player.isCursed )
   {
      Dialog_PushSection( &( game->dialog ), STRING_HOLYPROTECTION_CURSED );
   }
   else
   {
      Player_SetHolyProtection( &( game->player ), True );
      Dialog_PushSection( &( game->dialog ), STRING_HOLYPROTECTION_ON );
   }

   Game_OpenDialog( game );
}

internal void Game_SpellGlowCallback( Game_t* game )
{
   game->player.stats.magicPoints -= SPELL_GLOW_MP;
   Game_DrawQuickStatus( game );

   if ( game->player.isCursed )
   {
      Dialog_Reset( &( game->dialog ) );
      game->tileMap.torchIsLit = False;
      TileMap_SetTargetGlowDiameter( &( game->tileMap ), 1 );
      Dialog_PushSection( &( game->dialog ), STRING_GLOW_CURSED );
      Game_OpenDialog( game );
   }
   else
   {
      if ( game->tileMap.glowDiameter <= GLOW_SPELL_DIAMETER )
      {
         TileMap_SetTargetGlowDiameter( &( game->tileMap ), GLOW_SPELL_DIAMETER );
         game->tileMap.glowTileCount = 0;
      }
   }
}

internal void Game_SpellEvacCallback( Game_t* game )
{
   game->player.stats.magicPoints -= SPELL_EVAC_MP;
   Game_DrawQuickStatus( game );

   if ( game->player.isCursed )
   {
      Dialog_Reset( &( game->dialog ) );
      game->tileMap.torchIsLit = False;
      TileMap_SetTargetGlowDiameter( &( game->tileMap ), 1 );
      Dialog_PushSection( &( game->dialog ), STRING_EVAC_CURSED );
      Game_OpenDialog( game );
   }
   else
   {
      Game_AnimatePortalEntrance( game, &( game->tileMap.evacPortal ) );
   }
}

internal void Game_SpellAnimateNoEffect( Game_t* game )
{
   AnimationChain_Reset( &( game->animationChain ) );
   AnimationChain_PushAnimationWithCallback( &( game->animationChain ), AnimationId_Pause, Game_SpellNoEffectCallback, game );
   AnimationChain_Start( &( game->animationChain ) );
}

internal void Game_SpellNoEffectCallback( Game_t* game )
{
   char msg[64];

   Dialog_Reset( &( game->dialog ) );
   sprintf( msg, STRING_BATTLE_SPELL_NOEFFECT, game->battle.enemy.name );
   Dialog_PushSectionWithCallback( &( game->dialog ), msg, Game_ResetBattleMenu, game );
   Game_OpenDialog( game );
}
