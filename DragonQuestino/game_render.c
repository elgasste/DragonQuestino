#include "game.h"
#include "battle.h"
#include "enemy.h"
#include "math.h"

internal void Game_DrawTileMap( Game_t* game );
internal void Game_DrawPlayer( Game_t* game );
internal void Game_DrawNonUseableItems( Game_t* game, Bool_t hasUseableItems );

void Game_Draw( Game_t* game )
{
   uint32_t i;

   if ( game->animation.isRunning )
   {
      if ( game->mainState == MainState_Overworld )
      {
         if ( game->animation.id != AnimationId_CastSpell )
         {
            Game_DrawOverworld( game );
         }
      }
      else if ( game->mainState == MainState_Battle )
      {
         if ( game->screen.needsRedraw )
         {
            game->screen.needsRedraw = False;
            Game_DrawTileMap( game );
         }
         else if ( game->animation.id == AnimationId_Battle_EnemyFadeIn || game->animation.id == AnimationId_Battle_EnemyFadeOut )
         {
            Game_DrawEnemy( game );
         }
      }
   }
   else
   {
      if ( game->mainState == MainState_Overworld )
      {
         if ( game->screen.needsRedraw )
         {
            game->screen.needsRedraw = False;

            for ( i = 0; i < MenuId_Count; i++ )
            {
               game->menus[i].hasDrawn = False;
            }

            Game_DrawOverworld( game );

            switch ( game->subState )
            {
               case SubState_Menu:
                  Game_DrawQuickStatus( game );
                  Menu_Draw( &( game->menus[MenuId_Overworld] ) );
                  if ( game->activeMenu->id == MenuId_OverworldItem )
                  {
                     Game_DrawOverworldItemMenu( game );
                  }
                  else
                  {
                     Menu_Draw( game->activeMenu );
                  }
                  break;
               case SubState_Dialog:
                  Game_DrawQuickStatus( game );
                  switch ( game->activeMenu->id )
                  {
                     case MenuId_Overworld:
                        Menu_Draw( &( game->menus[MenuId_Overworld] ) );
                        break;
                     case MenuId_OverworldItem:
                        Menu_Draw( &( game->menus[MenuId_Overworld] ) );
                        Game_DrawOverworldItemMenu( game );
                        if ( game->dialog.id == DialogId_Use_Herb2 )
                        {
                           Game_DrawQuickStatus( game );
                        }
                        break;
                     case MenuId_OverworldSpell:
                        Menu_Draw( &( game->menus[MenuId_Overworld] ) );
                        Menu_Draw( &( game->menus[MenuId_OverworldSpell] ) );
                        break;
                  }
                  Dialog_Draw( &( game->dialog ) );
                  break;
            }
         }
         else
         {
            switch ( game->subState )
            {
               case SubState_None:
                  Game_DrawOverworld( game );
                  break;
               case SubState_Menu:
                  Menu_Draw( game->activeMenu );
                  break;
               case SubState_Dialog:
                  Dialog_Draw( &( game->dialog ) );
                  break;
            }
         }
      }
      else // main state is battle
      {
         if ( game->screen.needsRedraw )
         {
            Game_DrawTileMap( game );
            Game_WipeEnemy( game );
            Game_DrawEnemy( game );

            switch ( game->subState )
            {
               case SubState_Menu:
                  Game_DrawQuickStatus( game );
                  game->activeMenu->hasDrawn = False;
                  Menu_Draw( game->activeMenu );
                  Dialog_Draw( &( game->dialog ) );
                  break;
               case SubState_None:
               case SubState_Dialog:
                  Game_DrawQuickStatus( game );
                  Dialog_Draw( &( game->dialog ) );
                  break;
            }

            game->screen.needsRedraw = False;
         }
         else
         {
            switch ( game->subState )
            {
               case SubState_Menu:
                  Menu_Draw( game->activeMenu );
                  break;
               case SubState_Dialog:
                  Dialog_Draw( &( game->dialog ) );
                  break;
            }
         }
      }
   }
}

void Game_DrawOverworld( Game_t* game )
{
   Game_DrawTileMap( game );
   Game_DrawPlayer( game );

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

   memSize = MATH_MIN( (uint32_t)( strlen( game->player.name ) ), 4 );
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
   Screen_DrawTextWindow( &( game->screen ), 80, 32, 20, 18 );
   char line[18];

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_NAME, game->player.name );
   Screen_DrawText( &( game->screen ), line, 104 + ( ( 4 - ( (uint32_t)( ( strlen( game->player.name ) + 1 ) / 2 ) ) ) * TEXT_TILE_SIZE ), 40 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_STRENGTH, game->player.stats.strength );
   Screen_DrawText( &( game->screen ), line, 96, 56 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_AGILITY, game->player.stats.agility );
   Screen_DrawText( &( game->screen ), line, 104, 72 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXHP, game->player.stats.maxHitPoints );
   Screen_DrawText( &( game->screen ), line, 112, 88 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXMP, game->player.stats.maxMagicPoints );
   Screen_DrawText( &( game->screen ), line, 112, 104 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_WEAPON, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 96, 120 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_ARMOR, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 104, 136 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_SHIELD, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 96, 152 );
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
      Game_ChangeSubState( game, SubState_Waiting );
   }
}

void Game_DrawEnemy( Game_t* game )
{
   uint8_t x, y, i;
   uint16_t screenOffsetX, screenOffsetY;
   Enemy_t* enemy = &( game->battle.enemy );

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

void Game_WipeEnemy( Game_t* game )
{
   Screen_DrawRectColor( &( game->screen ), 96, 52, 112, 112, COLOR_BLACK );
}

internal void Game_DrawTileMap( Game_t* game )
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
   int32_t wx = (int32_t)( sprite->position.x ) + game->player.spriteOffset.x;
   int32_t wy = (int32_t)( sprite->position.y ) + game->player.spriteOffset.y;
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
