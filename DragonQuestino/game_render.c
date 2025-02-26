#include "game.h"
#include "math.h"

internal void Game_DrawOverworld( Game_t* game );
internal void Game_DrawPlayer( Game_t* game );

void Game_Draw( Game_t* game )
{
   if ( game->isAnimating )
   {
      switch ( game->animation )
      {
         case Animation_Overworld_Wash:
         case Animation_RainbowBridge_Trippy:
         case Animation_RainbowBridge_Whiteout:
         case Animation_RainbowBridge_FadeIn:
         case Animation_RainbowBridge_Pause:
            Game_DrawOverworld( game );
            break;
      }
   }
   else
   {
      switch ( game->state )
      {
         case GameState_Overworld:
            Game_DrawOverworld( game );
            break;
         case GameState_Overworld_MainMenu:
         case GameState_Overworld_SpellMenu:
         case GameState_Overworld_ItemMenu:
            Menu_Draw( &( game->menu ) );
            break;
         case GameState_Overworld_ScrollingDialog:
            ScrollingDialog_Draw( &( game->scrollingDialog ) );
            break;
         case GameState_TileMapTransition:
            Screen_WipeColor( &( game->screen ), COLOR_BLACK );
            break;
      }
   }
}

void Game_DrawOverworldQuickStatus( Game_t* game )
{
   uint16_t lvl = Player_GetLevel( &( game->player ) );
   uint32_t memSize;
   char line[9];

   memSize = MATH_MIN( (uint32_t)( strlen( game->player.name ) ), 4 );
   memcpy( line, game->player.name, sizeof( char ) * memSize );
   line[memSize] = '\0';
   Screen_DrawTextWindowWithTitle( &( game->screen ), 16, 16, 8, 12, line );

   sprintf( line, lvl < 10 ? "%s   %u" : "%s  %u", STRING_OVERWORLD_QUICKSTATS_LEVEL, lvl);
   Screen_DrawText( &( game->screen ), line, 24, 32 );

   sprintf( line, game->player.stats.hitPoints < 10 ? "%s   %u" : game->player.stats.hitPoints < 100 ? "%s  %u" : "%s %u", STRING_OVERWORLD_QUICKSTATS_HP, game->player.stats.hitPoints );
   Screen_DrawText( &( game->screen ), line, 24, 48 );

   sprintf( line, game->player.stats.magicPoints < 10 ? "%s   %u" : game->player.stats.magicPoints < 100 ? "%s  %u" : "%s %u", STRING_OVERWORLD_QUICKSTATS_MP, game->player.stats.magicPoints );
   Screen_DrawText( &( game->screen ), line, 24, 64 );

   sprintf( line, game->player.gold < 10 ? "%s    %u" : game->player.gold < 100 ? "%s   %u" : game->player.gold < 1000 ? "%s  %u" : game->player.gold < 10000 ? "%s %u" : "%s%u", STRING_OVERWORLD_QUICKSTATS_GOLD, game->player.gold );
   Screen_DrawText( &( game->screen ), line, 24, 80 );

   sprintf( line, game->player.experience < 10 ? "%s    %u" : game->player.experience < 100 ? "%s   %u" : game->player.experience < 1000 ? "%s  %u" : game->player.experience < 10000 ? "%s %u" : "%s%u", STRING_OVERWORLD_QUICKSTATS_EXP, game->player.experience );
   Screen_DrawText( &( game->screen ), line, 24, 96 );
}

void Game_DrawOverworldDeepStatus( Game_t* game )
{
   Screen_DrawTextWindow( &( game->screen ), 80, 32, 20, 22 );
   char line[18];

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_NAME, game->player.name );
   Screen_DrawText( &( game->screen ), line, 104 + ( ( 4 - ( (uint32_t)( ( strlen( game->player.name ) + 1 ) / 2 ) ) ) * TEXT_TILE_SIZE ), 40 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_STRENGTH, game->player.stats.strength );
   Screen_DrawText( &( game->screen ), line, 96, 56 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_AGILITY, game->player.stats.agility );
   Screen_DrawText( &( game->screen ), line, 104, 72 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXHP, game->player.stats.maxHitPoints );
   Screen_DrawText( &( game->screen ), line, 112, 88 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_MAXHP, game->player.stats.maxMagicPoints );
   Screen_DrawText( &( game->screen ), line, 112, 104 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_ATTACK, game->player.stats.attackPower );
   Screen_DrawText( &( game->screen ), line, 112, 120 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_DEFENSE, game->player.stats.defensePower );
   Screen_DrawText( &( game->screen ), line, 104, 136 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_WEAPON, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 96, 152 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_ARMOR, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 104, 168 );

   sprintf( line, STRING_OVERWORLD_DEEPSTATS_SHIELD, STRING_OVERWORLD_DEEPSTATS_NONE );
   Screen_DrawText( &( game->screen ), line, 96, 184 );
}

void Game_DrawNonUseableItems( Game_t* game, Bool_t hasUseableItems )
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

internal void Game_DrawOverworld( Game_t* game )
{
   if ( game->tileMap.viewportScreenPos.x != 0 || game->tileMap.viewportScreenPos.y != 0 )
   {
      Screen_WipeColor( &( game->screen ), COLOR_BLACK );
   }

   TileMap_Draw( &( game->tileMap ) );
   Game_DrawPlayer( game );

   if ( game->state == GameState_Overworld && game->overworldInactivitySeconds > OVERWORLD_INACTIVE_STATUS_SECONDS )
   {
      Game_DrawOverworldQuickStatus( game );
   }
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
