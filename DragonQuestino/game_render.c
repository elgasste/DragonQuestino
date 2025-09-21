#include "game.h"
#include "battle.h"
#include "enemy.h"
#include "math.h"

internal void Game_DrawPlayer( Game_t* game );
internal void Game_DrawNonUseableItems( Game_t* game, Bool_t hasUseableItems );
internal void Game_DrawNonPlayerCharacters( Game_t* game );
internal void Game_DrawPlayerNameEntry( Game_t* game );
internal void Game_DrawPasswordEntry( Game_t* game );
internal void Game_DrawEnding1( Game_t* game );

global u32 g_battleBackgroundIndexTable[49] =
{
   0,  1,  2,  3,  3,  4,  5,
   6,  7,  8,  9,  10, 11, 12,
   13, 14, 15, 16, 17, 18, 19,
   20, 21, 22, 23, 24, 25, 26,
   27, 28, 23, 23, 23, 29, 30,
   31, 32, 23, 23, 23, 23, 30,
   33, 34, 34, 34, 34, 34, 35
};

void Game_Draw( Game_t* game )
{
   AnimationId_t activeAnimationId;

   Game_UpdateTextColor( game );

   if ( game->doAnimation )
   {
      activeAnimationId = AnimationChain_GetActiveAnimationId( &( game->animationChain ) );

      if ( game->mainState == MainState_Overworld )
      {
         if ( activeAnimationId == AnimationId_CastSpell )
         {
            Game_DrawOverworld( game );
            Game_DrawQuickStatus( game );
            Dialog_Draw( &( game->dialog ) );
         }
         else if ( activeAnimationId != AnimationId_Pause )
         {
            Game_DrawOverworld( game );

            if ( game->subState == SubState_Dialog )
            {
               // TODO: this fixes the "blip" when casting heal spells in the overworld,
               // but I don't really know the full ramifications elsewhere. keep an eye on this.
               Game_DrawQuickStatus( game );
               Dialog_Draw( &( game->dialog ) );
            }
         }
      }
      else if ( game->mainState == MainState_Battle )
      {
         if ( activeAnimationId == AnimationId_Battle_EnemyFadeIn ||
              activeAnimationId == AnimationId_Battle_EnemySlowFadeIn ||
              activeAnimationId == AnimationId_Battle_EnemyFadeOut ||
              activeAnimationId == AnimationId_Battle_EnemySlowFadeOut )
         {
            Game_DrawBattleBackground( game );
            Game_DrawEnemy( game );
         }
      }
      else if ( game->mainState == MainState_Startup || game->mainState == MainState_Ending_2 )
      {
         Game_DrawTileMap( game );
      }

      return;
   }

   switch ( game->mainState )
   {
      case MainState_Startup:
         Game_DrawTileMap( game );
         Game_DrawTitleScreenFlash( game );
         if ( game->subState == SubState_Menu )
         {
            Menu_Draw( game->activeMenu );
         }
         break;
      case MainState_EnterName:
         if ( game->subState == SubState_Menu )
         {
            AlphaPicker_Draw( &( game->alphaPicker ) );
            Game_DrawPlayerNameEntry( game );
         }
         else
         {
            Screen_WipeColor( &( game->screen ), COLOR_BLACK );
         }
         break;
      case MainState_EnterPassword:
         if ( game->subState == SubState_Menu )
         {
            AlphaPicker_Draw( &( game->alphaPicker ) );
            Game_DrawPasswordEntry( game );
         }
         else
         {
            Screen_WipeColor( &( game->screen ), COLOR_BLACK );
         }
         break;
      case MainState_Ending_1:
         Game_DrawEnding1( game );
         break;
      case MainState_Ending_2:
         Game_DrawTileMap( game );
         break;
      case MainState_Overworld:
         Game_DrawOverworld( game );
         switch ( game->subState )
         {
            case SubState_Menu:
               Game_DrawQuickStatus( game );
               Menu_Draw( &( game->menus[MenuId_Overworld] ) );
               switch ( game->activeMenu->id )
               {
                  case MenuId_OverworldItem:
                     Game_DrawOverworldItemMenu( game );
                     break;
                  case MenuId_Zoom:
                     Menu_Draw( &( game->menus[MenuId_OverworldSpell] ) );
                     Menu_Draw( game->activeMenu );
                     break;
                  default:
                     Menu_Draw( game->activeMenu );
                     break;
               }
               break;
            case SubState_Dialog:
               Game_DrawQuickStatus( game );
               Dialog_Draw( &( game->dialog ) );
               break;
            case SubState_Status:
               Game_DrawQuickStatus( game );
               Game_DrawOverworldDeepStatus( game );
               break;
            case SubState_NonUseableItems:
               Game_DrawQuickStatus( game );
               Menu_Draw( &( game->menus[MenuId_Overworld] ) );
               Game_DrawNonUseableItems( game, False );
               break;
            case SubState_BinaryChoice:
               Game_DrawQuickStatus( game );
               Dialog_Draw( &( game->dialog ) );
               BinaryPicker_Draw( &( game->binaryPicker ) );
               break;
            case SubState_ShopMenu:
               Game_DrawQuickStatus( game );
               Dialog_Draw( &( game->dialog ) );
               ShopPicker_Draw( &( game->shopPicker ) );
               break;
         }
         break;
      case MainState_Battle:
         if ( game->battle.enemy.id == ENEMY_DRAGONLORDDRAGON_ID )
         {
            Screen_WipeColor( &( game->screen ), COLOR_BLACK );
         }
         else
         {
            Game_DrawTileMap( game );
         }
         switch ( game->subState )
         {
            case SubState_Menu:
               Game_DrawQuickStatus( game );
               Game_DrawBattleBackground( game );
               Game_DrawEnemy( game );
               switch ( game->activeMenu->id )
               {
                  case MenuId_BattleSpell:
                  case MenuId_BattleItem:
                     Menu_Draw( &( game->menus[MenuId_Battle] ) );
                     break;
               }
               Menu_Draw( game->activeMenu );
               Dialog_Draw( &( game->dialog ) );
               break;
            case SubState_Dialog:
               Game_DrawQuickStatus( game );
               Game_DrawBattleBackground( game );
               Game_DrawEnemy( game );
               Dialog_Draw( &( game->dialog ) );
               break;
         }
         break;
   }
}

void Game_DrawOverworld( Game_t* game )
{
   Game_DrawTileMap( game );
   Game_DrawNonPlayerCharacters( game );

   if ( !( game->player.sprite.flickerOff ) )
   {
      Game_DrawPlayer( game );
   }

   if ( game->subState == SubState_None && game->overworldInactivitySeconds > OVERWORLD_INACTIVE_STATUS_SECONDS )
   {
      Game_DrawQuickStatus( game );
   }
}

void Game_DrawQuickStatus( Game_t* game )
{
   u32 memSize;
   i32 xOffset = ( ( game->mainState == MainState_Battle ) ? -8 : 0 ) + game->rumbleOffset.x;
   u8 level = game->player.level + 1;
   char line[9];

   // special case for the game's ending dialogue
   if ( game->gameFlags.defeatedDragonlord &&
        ( ( game->player.tileIndex == TILEMAP_ENDING_TRIGGERINDEX_1 ) || ( game->player.tileIndex == TILEMAP_ENDING_TRIGGERINDEX_2 ) ) )
   {
      return;
   }

   memSize = Math_Min32u( (u32)( strlen( game->player.name ) ), 4 );
   memcpy( line, game->player.name, sizeof( char ) * memSize );
   line[memSize] = '\0';
   Screen_DrawTextWindowWithTitle( &( game->screen ), (u32)( 16 + xOffset ), (u32)( 16 + game->rumbleOffset.y ), 8, 12, line );

   sprintf( line, level < 10 ? "%s   %u" : "%s  %u", STRING_QUICKSTATS_LEVEL, level );
   Screen_DrawText( &( game->screen ), line, (u32)( 24 + xOffset ), (u32)( 32 + game->rumbleOffset.y ) );

   sprintf( line, game->player.stats.hitPoints < 10 ? "%s   %u" : game->player.stats.hitPoints < 100 ? "%s  %u" : "%s %u", STRING_QUICKSTATS_HP, game->player.stats.hitPoints );
   Screen_DrawText( &( game->screen ), line, (u32)( 24 + xOffset ), (u32)( 48 + game->rumbleOffset.y ) );

   sprintf( line, game->player.stats.magicPoints < 10 ? "%s   %u" : game->player.stats.magicPoints < 100 ? "%s  %u" : "%s %u", STRING_QUICKSTATS_MP, game->player.stats.magicPoints );
   Screen_DrawText( &( game->screen ), line, (u32)( 24 + xOffset ), (u32)( 64 + game->rumbleOffset.y ) );

   sprintf( line, game->player.gold < 10 ? "%s    %u" : game->player.gold < 100 ? "%s   %u" : game->player.gold < 1000 ? "%s  %u" : game->player.gold < 10000 ? "%s %u" : "%s%u", STRING_QUICKSTATS_GOLD, game->player.gold );
   Screen_DrawText( &( game->screen ), line, (u32)( 24 + xOffset ), (u32)( 80 + game->rumbleOffset.y ) );

   sprintf( line, game->player.experience < 10 ? "%s    %u" : game->player.experience < 100 ? "%s   %u" : game->player.experience < 1000 ? "%s  %u" : game->player.experience < 10000 ? "%s %u" : "%s%u", STRING_QUICKSTATS_EXP, game->player.experience );
   Screen_DrawText( &( game->screen ), line, (u32)( 24 + xOffset ), (u32)( 96 + game->rumbleOffset.y ) );
}

void Game_DrawOverworldDeepStatus( Game_t* game )
{
   // status window
   Screen_DrawTextWindow( &( game->screen ), 80, 16, 20, 18 );
   char line[18];

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_NAME, game->player.name );
   Screen_DrawText( &( game->screen ), line, 104 + ( ( 4 - ( (u32)( ( strlen( game->player.name ) + 1 ) / 2 ) ) ) * TEXT_TILE_SIZE ), 24 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_STRENGTH, game->player.stats.strength );
   Screen_DrawText( &( game->screen ), line, 96, 40 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_AGILITY, game->player.stats.agility );
   Screen_DrawText( &( game->screen ), line, 104, 56 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXHP, game->player.stats.maxHitPoints );
   Screen_DrawText( &( game->screen ), line, 112, 72 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXMP, game->player.stats.maxMagicPoints );
   Screen_DrawText( &( game->screen ), line, 112, 88 );

   Screen_DrawText( &( game->screen ), STRING_OVERWORLD_DEEPSTATS_WEAPON, 96, 104 );
   Screen_DrawText( &( game->screen ), game->player.weapon.name1, 160, 104 );
   Screen_DrawText( &( game->screen ), game->player.weapon.name2, 168, 112 );

   Screen_DrawText( &( game->screen ), STRING_OVERWORLD_DEEPSTATS_ARMOR, 104, 120 );
   Screen_DrawText( &( game->screen ), game->player.armor.name1, 160, 120 );
   Screen_DrawText( &( game->screen ), game->player.armor.name2, 168, 128 );

   Screen_DrawText( &( game->screen ), STRING_OVERWORLD_DEEPSTATS_SHIELD, 96, 136 );
   Screen_DrawText( &( game->screen ), game->player.shield.name1, 160, 136 );
   Screen_DrawText( &( game->screen ), game->player.shield.name2, 168, 144 );

   // spells window
   Screen_DrawTextWindowWithTitle( &( game->screen ), 16, 168, 28, 6, STRING_OVERWORLD_DEEPSTATS_SPELLS );

   if ( SPELL_HAS_HEAL( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_HEAL, 28, 180 );
   if ( SPELL_HAS_SIZZ( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_SIZZ, 28, 190 );
   if ( SPELL_HAS_SLEEP( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_SLEEP, 28, 200 );
   if ( SPELL_HAS_GLOW( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_GLOW, 76, 180 );
   if ( SPELL_HAS_FIZZLE( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_FIZZLE, 76, 190 );
   if ( SPELL_HAS_EVAC( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_EVAC, 76, 200 );
   if ( SPELL_HAS_ZOOM( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_ZOOM, 132, 180 );
   if ( SPELL_HAS_REPEL( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_REPEL, 132, 190 );
   if ( SPELL_HAS_MIDHEAL( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_MIDHEAL, 132, 200 );
   if ( SPELL_HAS_SIZZLE( game->player.spells ) ) Screen_DrawText( &( game->screen ), STRING_SPELL_SIZZLE, 180, 180 );
}

void Game_DrawOverworldItemMenu( Game_t* game )
{
   u32 useableCount = ITEM_GET_MAPUSEABLECOUNT( game->player.items );

   if ( ITEM_GET_MAPNONUSEABLECOUNT( game->player.items ) > 0 )
   {
      Game_DrawNonUseableItems( game, ( useableCount > 0 ) ? True : False );
   }

   if ( useableCount > 0 )
   {
      Menu_Draw( &( game->menus[MenuId_OverworldItem] ) );
   }
   else
   {
      Game_ChangeSubState( game, SubState_NonUseableItems );
   }
}

void Game_DrawEnemy( Game_t* game )
{
   u8 i;
   u16 screenOffsetX, screenOffsetY;
   Enemy_t* enemy = &( game->battle.enemy );
   u8 x = 112;
   u8 y = 68;

   for ( i = 0; i < ENEMY_TILE_COUNT; i++ )
   {
      if ( enemy->tileTextureIndexes[i] >= 0 )
      {
         u8* texture = enemy->tileTextures[enemy->tileTextureIndexes[i]];

         screenOffsetX = ( i % ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
         screenOffsetY = ( i / ENEMY_TILES_X ) * ENEMY_TILE_SIZE;

         if ( game->battle.enemyAlpha < 255 )
         {
            Screen_DrawMemorySectionBlended( &( game->screen ), texture, ENEMY_TILE_SIZE,
                                             0, 0,
                                             ENEMY_TILE_SIZE, ENEMY_TILE_SIZE,
                                             x + screenOffsetX, y + screenOffsetY,
                                             game->battle.enemyAlpha );
         }
         else
         {
            Screen_DrawMemorySection( &( game->screen ), texture, ENEMY_TILE_SIZE,
                                       0, 0,
                                       ENEMY_TILE_SIZE, ENEMY_TILE_SIZE,
                                       x + screenOffsetX, y + screenOffsetY, True );
         }
      }
   }
}

void Game_DrawBattleBackground( Game_t* game )
{
   u32 row, col;
   u32 x = (u32)( 96 + game->rumbleOffset.x );
   u32 y = (u32)( 52 + game->rumbleOffset.y );

   if ( game->tileMap.isDark )
   {
      Screen_DrawRectColor( &( game->screen ), 72, 56, 112, 112, COLOR_BLACK );
   }
   else if ( game->tileMap.isDungeon || game->tileMap.id == TILEMAP_HAUKSNESS_ID )
   {
      Screen_DrawRectColor( &( game->screen ), x, y, 112, 112, COLOR_BLACK );
   }
   else
   {
      for ( row = 0; row < 7; row++ )
      {
         for ( col = 0; col < 7; col++ )
         {
            Screen_DrawMemorySection( &( game->screen ), game->screen.battleBackgroundTileTextures[g_battleBackgroundIndexTable[col + ( row * 7 )]].memory,
                                      TILE_SIZE, 0, 0,
                                      TILE_SIZE, TILE_SIZE,
                                      x + ( col * TILE_SIZE ), y + ( row * TILE_SIZE ),
                                      False );
         }
      }
   }
}

void Game_UpdateTextColor( Game_t* game )
{
   r32 percentage;
   Player_t* player = &( game->player );

   if ( game->gameFlags.joinedDragonlord )
   {
      game->screen.textColor = COLOR_DARKRED;
   }
   else if ( player->isCursed )
   {
      game->screen.textColor = COLOR_GROSSYELLOW;
   }
   else
   {
      percentage = (r32)( player->stats.hitPoints ) / player->stats.maxHitPoints;
      game->screen.textColor = ( percentage < PLAYER_LOWHEALTH_PERCENTAGE ) ? COLOR_INJUREDRED : COLOR_WHITE;
   }
}

void Game_DrawTileMap( Game_t* game )
{
   if ( game->tileMap.viewportScreenPos.x != 0 || game->tileMap.viewportScreenPos.y != 0 )
   {
      Screen_WipeColor( &( game->screen ), COLOR_BLACK );
   }

   TileMap_Draw( &( game->tileMap ) );
}

void Game_DrawTitleScreenFlash( Game_t* game )
{
   u32 sx = 181, sy = 58;
   ActiveSprite_t* sprite;

   if ( !( game->titleScreenFlash.isFlashing ) )
   {
      return;
   }

   switch ( game->titleScreenFlash.currentFrame )
   {
      case 0:
      case 6:
         sprite = &( game->tileMap.npcs[0].sprite );
         break;
      case 1:
      case 5:
         sprite = &( game->tileMap.npcs[1].sprite );
         break;
      case 2:
      case 4:
         sprite = &( game->tileMap.npcs[2].sprite );
         break;
      default:
         sprite = &( game->tileMap.npcs[3].sprite );
         break;
   }

   Screen_DrawMemorySection( &( game->screen ), sprite->textures[0].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx, sy, True );
   Screen_DrawMemorySection( &( game->screen ), sprite->textures[1].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx + SPRITE_TEXTURE_SIZE, sy, True );
   Screen_DrawMemorySection( &( game->screen ), sprite->textures[2].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx, sy + SPRITE_TEXTURE_SIZE, True );
   Screen_DrawMemorySection( &( game->screen ), sprite->textures[3].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx + SPRITE_TEXTURE_SIZE, sy + SPRITE_TEXTURE_SIZE, True );
   Screen_DrawMemorySection( &( game->screen ), sprite->textures[4].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx, sy + ( SPRITE_TEXTURE_SIZE * 2 ), True );
   Screen_DrawMemorySection( &( game->screen ), sprite->textures[5].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx + SPRITE_TEXTURE_SIZE, sy + ( SPRITE_TEXTURE_SIZE * 2 ), True );
   Screen_DrawMemorySection( &( game->screen ), sprite->textures[7].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx + ( SPRITE_TEXTURE_SIZE * 2 ), sy + ( SPRITE_TEXTURE_SIZE * 2 ), True );
   Screen_DrawMemorySection( &( game->screen ), sprite->textures[6].memory, SPRITE_TEXTURE_SIZE, 0, 0, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, sx, sy + ( SPRITE_TEXTURE_SIZE * 3 ), True );
}

internal void Game_DrawPlayer( Game_t* game )
{
   ActiveSprite_t* sprite = &( game->player.sprite );
   i32 wx = (i32)( sprite->position.x ) + game->player.sprite.offset.x;
   i32 wy = (i32)( sprite->position.y ) + game->player.sprite.offset.y;
   i32 sx = wx - game->tileMap.viewport.x;
   i32 sy = wy - game->tileMap.viewport.y;
   u32 textureIndex = ( (u32)( sprite->direction ) * ACTIVE_SPRITE_FRAMES ) + sprite->currentFrame;
   u32 tx = ( sx < 0 ) ? (u32)( -sx ) : 0;
   u32 ty = ( sy < 0 ) ? (u32)( -sy ) : 0;
   u32 tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > game->tileMap.viewport.w ) ? ( game->tileMap.viewport.w - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
   u32 th = ( ( sy + SPRITE_TEXTURE_SIZE ) > game->tileMap.viewport.h ) ? ( game->tileMap.viewport.h - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
   u32 sxu = ( sx < 0 ) ? 0 : sx;
   u32 syu = ( sy < 0 ) ? 0 : sy;

   Screen_DrawMemorySection( &( game->screen ), sprite->textures[textureIndex].memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th,
                             sxu + game->tileMap.viewportScreenPos.x, syu + game->tileMap.viewportScreenPos.y, True );

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.showHitBoxes )
   {
      Screen_DrawRectColor( &( game->screen ),
                            sx - sprite->offset.x, sy - sprite->offset.y,
                            (u32)( sprite->hitBoxSize.x ), (u32)( sprite->hitBoxSize.y ),
                            COLOR_RED );
   }
#endif
}

internal void Game_DrawNonUseableItems( Game_t* game, Bool_t hasUseableItems )
{
   u32 x, y;
   Player_t* player = &( game->player );
   u32 items = player->items;
   u32 itemCount = ITEM_GET_MAPNONUSEABLECOUNT( items );
   char line[MENU_LINE_LENGTH];

   if ( hasUseableItems )
   {
      Screen_DrawTextWindow( &( game->screen ), 56, 48, 11, ( itemCount * 2 ) + 2 );
      x = 64;
      y = 56;
   }
   else
   {
      Screen_DrawTextWindowWithTitle( &( game->screen ), 152, 48, 11, ( itemCount * 2 ) + 3, STRING_OVERWORLD_MENU_ITEM );
      x = 160;
      y = 64;
   }

   if ( ITEM_GET_KEYCOUNT( items ) )
   {
      sprintf( line, STRING_OVERWORLD_ITEMMENU_KEY, ITEM_GET_KEYCOUNT( items ) );
      Screen_DrawText( &( game->screen ), line, x, y );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( ITEM_HAS_DRAGONSCALE( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_DRAGONSCALE_1, x, y );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_DRAGONSCALE_2, x, y + 8 );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( ITEM_HAS_TOKEN( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_TOKEN_1, x, y );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_TOKEN_2, x, y + 8 );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( ITEM_HAS_STONEOFSUNLIGHT( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STONEOFSUNLIGHT_1, x, y );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STONEOFSUNLIGHT_2, x, y + TEXT_TILE_SIZE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( ITEM_HAS_STAFFOFRAIN( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STAFFOFRAIN_1, x, y );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_STAFFOFRAIN_2, x, y + TEXT_TILE_SIZE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
   if ( ITEM_HAS_SPHEREOFLIGHT( items ) )
   {
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_SPHEREOFLIGHT_1, x, y );
      Screen_DrawText( &( game->screen ), STRING_OVERWORLD_ITEMMENU_SPHEREOFLIGHT_2, x, y + TEXT_TILE_SIZE );
      y += ( TEXT_TILE_SIZE * 2 );
   }
}

internal void Game_DrawNonPlayerCharacters( Game_t* game )
{
   u32 i, tx, ty, tw, th, sxu, syu, textureIndex;
   i32 sx, sy;
   ActiveSprite_t* sprite;
   Vector4i32_t* viewport = &( game->tileMap.viewport );

   for ( i = 0; i < game->tileMap.npcCount; i++ )
   {
      sprite = &( game->tileMap.npcs[i].sprite );
      sx = (i32)( sprite->position.x - viewport->x + sprite->offset.x );
      sy = (i32)( sprite->position.y - viewport->y + sprite->offset.y );

      if ( Math_RectsIntersect32i( sx, sy, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, 0, 0, viewport->w, viewport->h ) )
      {
         tx = ( sx < 0 ) ? (u32)( -sx ) : 0;
         ty = ( sy < 0 ) ? (u32)( -sy ) : 0;
         tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > viewport->w ) ? ( viewport->w - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
         th = ( ( sy + SPRITE_TEXTURE_SIZE ) > viewport->h ) ? ( viewport->h - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
         sxu = ( sx < 0 ) ? 0 : sx;
         syu = ( sy < 0 ) ? 0 : sy;
         textureIndex = ( (u32)( sprite->direction ) * ACTIVE_SPRITE_FRAMES ) + sprite->currentFrame;

         Screen_DrawMemorySection( &( game->screen ), sprite->textures[textureIndex].memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th,
                                   sxu + game->tileMap.viewportScreenPos.x, syu + game->tileMap.viewportScreenPos.y, True );

#if defined( VISUAL_STUDIO_DEV )
         if ( g_debugFlags.showHitBoxes )
         {
            sx = (i32)( sprite->position.x - viewport->x );
            sy = (i32)( sprite->position.y - viewport->y );
            if ( Math_RectsIntersect32i( sx, sy, sprite->hitBoxSize.x, sprite->hitBoxSize.y, 0, 0, viewport->w, viewport->h ) )
            {
               tx = ( sx < 0 ) ? (u32)( -sx ) : 0;
               ty = ( sy < 0 ) ? (u32)( -sy ) : 0;
               tw = ( ( sx + (i32)( sprite->hitBoxSize.x ) ) > viewport->w ) ? ( viewport->w - sx ) : ( (i32)( sprite->hitBoxSize.x ) - tx );
               th = ( ( sy + (i32)( sprite->hitBoxSize.y ) ) > viewport->h ) ? ( viewport->h - sy ) : ( (i32)( sprite->hitBoxSize.y ) - ty );
               sxu = ( sx < 0 ) ? 0 : sx;
               syu = ( sy < 0 ) ? 0 : sy;
               Screen_DrawRectColor( &( game->screen ), sxu, syu, tw, th, COLOR_RED );
            }
         }
#endif
      }
   }
}

internal void Game_DrawPlayerNameEntry( Game_t* game )
{
   u32 i;
   u32 x = 80;
   u32 y = 146;
   u32 textX = x + ( TEXT_TILE_SIZE * 2 );
   u32 textY = y + ( TEXT_TILE_SIZE * 2 );
   size_t length = strlen( game->player.name );

   Screen_DrawTextWindow( &( game->screen ), x, y, 12, 5 );

   for ( i = 0; i < 8; i++ )
   {
      if ( i < length )
      {
         Screen_DrawChar( &( game->screen ), game->player.name[i], textX + ( i * TEXT_TILE_SIZE ), textY );
      }
      else
      {
         Screen_DrawChar( &( game->screen ), '*', textX + ( i * TEXT_TILE_SIZE ), textY );
      }
   }
}

internal void Game_DrawPasswordEntry( Game_t* game )
{
   u32 i;
   u32 x = 52;
   u32 y = 146;
   u32 textX = x + ( TEXT_TILE_SIZE * 2 );
   u32 textY = y + ( TEXT_TILE_SIZE * 2 );
   size_t length = strlen( game->password );

   Screen_DrawTextWindow( &( game->screen ), x, y, 19, 7 );

   for ( i = 0; i < 15; i++ )
   {
      if ( i < length )
      {
         Screen_DrawChar( &( game->screen ), game->password[i], textX + ( i * TEXT_TILE_SIZE ), textY);
      }
      else
      {
         Screen_DrawChar( &( game->screen ), '*', textX + ( i * TEXT_TILE_SIZE ), textY);
      }
   }

   for ( i = 15; i < 30; i++ )
   {
      if ( i < length )
      {
         Screen_DrawChar( &( game->screen ), game->password[i], textX + ( ( i - 15 ) * TEXT_TILE_SIZE ), textY + 16);
      }
      else
      {
         Screen_DrawChar( &( game->screen ), '*', textX + ( ( i - 15 ) * TEXT_TILE_SIZE ), textY + 16);
      }
   }
}

internal void Game_DrawEnding1( Game_t* game )
{
   char msg[64];

   Screen_WipeColor( &( game->screen ), COLOR_BLACK );

   game->screen.textColor = COLOR_ENDINGYELLOW;
   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_1, 32 );
   sprintf( msg, STRING_ENDING_MESSAGE_2, game->player.name );
   Screen_DrawCenteredText( &( game->screen ), msg, 48 );
   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_3, 64 );
   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_4, 80 );

   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_5, 112 );
   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_6, 128 );
   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_7, 144 );
   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_8, 160 );

   game->screen.textColor = COLOR_ENDINGPURPLE;
   Screen_DrawCenteredText( &( game->screen ), STRING_ENDING_MESSAGE_9, 192 );
}
