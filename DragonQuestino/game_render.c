#include "game.h"
#include "battle.h"
#include "enemy.h"
#include "math.h"

internal void Game_DrawPlayer( Game_t* game );
internal void Game_DrawNonUseableItems( Game_t* game, Bool_t hasUseableItems );
internal void Game_DrawNonPlayerCharacters( Game_t* game );
internal void Game_DrawPlayerNameEntry( Game_t* game );
internal void Game_DrawPasswordEntry( Game_t* game );

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
               Dialog_Draw( &( game->dialog ) );
            }
         }
      }
      else if ( game->mainState == MainState_Battle )
      {
         if ( activeAnimationId == AnimationId_Battle_EnemyFadeIn ||
              activeAnimationId == AnimationId_Battle_EnemySlowFadeIn ||
              activeAnimationId == AnimationId_Battle_EnemyFadeOut )
         {
            Game_DrawEnemy( game );
         }
      }

      return;
   }

   switch ( game->mainState )
   {
      case MainState_Startup:
         Menu_Draw( game->activeMenu );
         break;
      case MainState_EnterName:
         AlphaPicker_Draw( &( game->alphaPicker ) );
         Game_DrawPlayerNameEntry( game );
         break;
      case MainState_EnterPassword:
         AlphaPicker_Draw( &( game->alphaPicker ) );
         Game_DrawPasswordEntry( game );
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
               Game_WipeEnemy( game );
               Game_DrawEnemy( game );
               Menu_Draw( game->activeMenu );
               Dialog_Draw( &( game->dialog ) );
               break;
            case SubState_Dialog:
               Game_DrawQuickStatus( game );
               Game_WipeEnemy( game );
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
   uint32_t memSize;
   int32_t xOffset = ( game->mainState == MainState_Battle ) ? -8 : 0;
   uint8_t level = game->player.level + 1;
   char line[9];

   memSize = Math_Min32u( (uint32_t)( strlen( game->player.name ) ), 4 );
   memcpy( line, game->player.name, sizeof( char ) * memSize );
   line[memSize] = '\0';
   Screen_DrawTextWindowWithTitle( &( game->screen ), 16 + xOffset, 16, 8, 12, line );

   sprintf( line, level < 10 ? "%s   %u" : "%s  %u", STRING_QUICKSTATS_LEVEL, level );
   Screen_DrawText( &( game->screen ), line, 24 + xOffset, 32 );

   sprintf( line, game->player.stats.hitPoints < 10 ? "%s   %u" : game->player.stats.hitPoints < 100 ? "%s  %u" : "%s %u", STRING_QUICKSTATS_HP, game->player.stats.hitPoints );
   Screen_DrawText( &( game->screen ), line, 24 + xOffset, 48 );

   sprintf( line, game->player.stats.magicPoints < 10 ? "%s   %u" : game->player.stats.magicPoints < 100 ? "%s  %u" : "%s %u", STRING_QUICKSTATS_MP, game->player.stats.magicPoints );
   Screen_DrawText( &( game->screen ), line, 24 + xOffset, 64 );

   sprintf( line, game->player.gold < 10 ? "%s    %u" : game->player.gold < 100 ? "%s   %u" : game->player.gold < 1000 ? "%s  %u" : game->player.gold < 10000 ? "%s %u" : "%s%u", STRING_QUICKSTATS_GOLD, game->player.gold );
   Screen_DrawText( &( game->screen ), line, 24 + xOffset, 80 );

   sprintf( line, game->player.experience < 10 ? "%s    %u" : game->player.experience < 100 ? "%s   %u" : game->player.experience < 1000 ? "%s  %u" : game->player.experience < 10000 ? "%s %u" : "%s%u", STRING_QUICKSTATS_EXP, game->player.experience );
   Screen_DrawText( &( game->screen ), line, 24 + xOffset, 96 );
}

void Game_DrawOverworldDeepStatus( Game_t* game )
{
   Screen_DrawTextWindow( &( game->screen ), 80, 16, 20, 18 );
   char line[18];

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_NAME, game->player.name );
   Screen_DrawText( &( game->screen ), line, 104 + ( ( 4 - ( (uint32_t)( ( strlen( game->player.name ) + 1 ) / 2 ) ) ) * TEXT_TILE_SIZE ), 24 );

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
}

void Game_DrawOverworldItemMenu( Game_t* game )
{
   uint32_t useableCount = ITEM_GET_MAPUSEABLECOUNT( game->player.items );

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
   uint8_t x, y, i;
   uint16_t screenOffsetX, screenOffsetY;
   Enemy_t* enemy = &( game->battle.enemy );

   if ( !game->battle.isOver )
   {
      x = 112;
      y = 68;

      for ( i = 0; i < ENEMY_TILE_COUNT; i++ )
      {
         if ( enemy->tileTextureIndexes[i] >= 0 )
         {
            uint8_t* texture = enemy->tileTextures[enemy->tileTextureIndexes[i]];

            screenOffsetX = ( i % ENEMY_TILES_X ) * ENEMY_TILE_SIZE;
            screenOffsetY = ( i / ENEMY_TILES_X ) * ENEMY_TILE_SIZE;

            Screen_DrawMemorySection( &( game->screen ), texture, ENEMY_TILE_SIZE,
                                      0, 0,
                                      ENEMY_TILE_SIZE, ENEMY_TILE_SIZE,
                                      x + screenOffsetX, y + screenOffsetY, False );
         }
      }
   }
}

void Game_WipeEnemy( Game_t* game )
{
   if ( game->tileMap.isDark )
   {
      Screen_DrawRectColor( &( game->screen ), 72, 56, 112, 112, COLOR_BLACK );
   }
   else
   {
      Screen_DrawRectColor( &( game->screen ), 96, 52, 112, 112, COLOR_BLACK );
   }
}

void Game_UpdateTextColor( Game_t* game )
{
   float percentage;
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
      percentage = (float)( player->stats.hitPoints ) / player->stats.maxHitPoints;
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

internal void Game_DrawPlayer( Game_t* game )
{
   ActiveSprite_t* sprite = &( game->player.sprite );
   int32_t wx = (int32_t)( sprite->position.x ) + game->player.sprite.offset.x;
   int32_t wy = (int32_t)( sprite->position.y ) + game->player.sprite.offset.y;
   int32_t sx = wx - game->tileMap.viewport.x;
   int32_t sy = wy - game->tileMap.viewport.y;
   uint32_t textureIndex = ( (uint32_t)( sprite->direction ) * ACTIVE_SPRITE_FRAMES ) + sprite->currentFrame;
   uint32_t tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
   uint32_t ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
   uint32_t tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > game->tileMap.viewport.w ) ? ( game->tileMap.viewport.w - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
   uint32_t th = ( ( sy + SPRITE_TEXTURE_SIZE ) > game->tileMap.viewport.h ) ? ( game->tileMap.viewport.h - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
   uint32_t sxu = ( sx < 0 ) ? 0 : sx;
   uint32_t syu = ( sy < 0 ) ? 0 : sy;

   Screen_DrawMemorySection( &( game->screen ), sprite->textures[textureIndex].memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th,
                             sxu + game->tileMap.viewportScreenPos.x, syu + game->tileMap.viewportScreenPos.y, True );

#if defined( VISUAL_STUDIO_DEV )
   if ( g_debugFlags.showHitBoxes )
   {
      Screen_DrawRectColor( &( game->screen ),
                            sx - sprite->offset.x, sy - sprite->offset.y,
                            (uint32_t)( sprite->hitBoxSize.x ), (uint32_t)( sprite->hitBoxSize.y ),
                            COLOR_RED );
   }
#endif
}

internal void Game_DrawNonUseableItems( Game_t* game, Bool_t hasUseableItems )
{
   uint32_t x, y;
   Player_t* player = &( game->player );
   uint32_t items = player->items;
   uint32_t itemCount = ITEM_GET_MAPNONUSEABLECOUNT( items );
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
   uint32_t i, tx, ty, tw, th, sxu, syu, textureIndex;
   int32_t sx, sy;
   ActiveSprite_t* sprite;
   Vector4i32_t* viewport = &( game->tileMap.viewport );

   for ( i = 0; i < game->tileMap.npcCount; i++ )
   {
      sprite = &( game->tileMap.npcs[i].sprite );
      sx = (int32_t)( sprite->position.x - viewport->x + sprite->offset.x );
      sy = (int32_t)( sprite->position.y - viewport->y + sprite->offset.y );

      if ( Math_RectsIntersect32i( sx, sy, SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, 0, 0, viewport->w, viewport->h ) )
      {
         tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
         ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
         tw = ( ( sx + SPRITE_TEXTURE_SIZE ) > viewport->w ) ? ( viewport->w - sx ) : ( SPRITE_TEXTURE_SIZE - tx );
         th = ( ( sy + SPRITE_TEXTURE_SIZE ) > viewport->h ) ? ( viewport->h - sy ) : ( SPRITE_TEXTURE_SIZE - ty );
         sxu = ( sx < 0 ) ? 0 : sx;
         syu = ( sy < 0 ) ? 0 : sy;
         textureIndex = ( (uint32_t)( sprite->direction ) * ACTIVE_SPRITE_FRAMES ) + sprite->currentFrame;

         Screen_DrawMemorySection( &( game->screen ), sprite->textures[textureIndex].memory, SPRITE_TEXTURE_SIZE, tx, ty, tw, th,
                                   sxu + game->tileMap.viewportScreenPos.x, syu + game->tileMap.viewportScreenPos.y, True );

#if defined( VISUAL_STUDIO_DEV )
         if ( g_debugFlags.showHitBoxes )
         {
            sx = (int32_t)( sprite->position.x - viewport->x );
            sy = (int32_t)( sprite->position.y - viewport->y );
            if ( Math_RectsIntersect32i( sx, sy, sprite->hitBoxSize.x, sprite->hitBoxSize.y, 0, 0, viewport->w, viewport->h ) )
            {
               tx = ( sx < 0 ) ? (uint32_t)( -sx ) : 0;
               ty = ( sy < 0 ) ? (uint32_t)( -sy ) : 0;
               tw = ( ( sx + (int32_t)( sprite->hitBoxSize.x ) ) > viewport->w ) ? ( viewport->w - sx ) : ( (int32_t)( sprite->hitBoxSize.x ) - tx );
               th = ( ( sy + (int32_t)( sprite->hitBoxSize.y ) ) > viewport->h ) ? ( viewport->h - sy ) : ( (int32_t)( sprite->hitBoxSize.y ) - ty );
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
   uint32_t i;
   uint32_t x = 80;
   uint32_t y = 146;
   uint32_t textX = x + ( TEXT_TILE_SIZE * 2 );
   uint32_t textY = y + ( TEXT_TILE_SIZE * 2 );
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
   uint32_t i;
   uint32_t x = 52;
   uint32_t y = 146;
   uint32_t textX = x + ( TEXT_TILE_SIZE * 2 );
   uint32_t textY = y + ( TEXT_TILE_SIZE * 2 );
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
