#include "game.h"
#include "random.h"

internal void Game_TicOverworld( Game_t* game );
internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag );

void Game_Init( Game_t* game, uint16_t* screenBuffer )
{
   Random_Seed();
   Screen_Init( &( game->screen ), screenBuffer );
   TileMap_Init( &( game->tileMap ), &( game->screen ) );
   TileMap_LoadTextures( &( game->tileMap ) );
   TileMap_Load( &( game->tileMap ), 1 );
   Sprite_LoadPlayer( &( game->player.sprite ) );
   Clock_Init( &( game->clock ) );
   Input_Init( &( game->input ) );
   Player_Init( &( game->player ), &( game->screen ), &( game->tileMap ) );
   Menu_Init( &( game->menu ), &( game->screen ), &( game->player ) );
   ScrollingDialog_Init( &( game->scrollingDialog ), &( game->screen ), &( game->player ) );

   game->state = GameState_Overworld;
   game->targetPortal = 0;
   game->overworldInactivitySeconds = 0.0f;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &( game->input ) );   

   if ( game->isAnimating )
   {
      Game_TicAnimation( game );
   }
   else
   {
      Game_HandleInput( game );

      switch ( game->state )
      {
         case GameState_Overworld:
            Game_TicOverworld( game );
            break;
         case GameState_Overworld_ScrollingDialog:
            ScrollingDialog_Tic( &( game->scrollingDialog ) );
            break;
         case GameState_Overworld_MainMenu:
         case GameState_Overworld_SpellMenu:
         case GameState_Overworld_ItemMenu:
            Menu_Tic( &( game->menu ) );
            break;
      }
   }

   Game_Draw( game );
   Screen_RenderBuffer( &( game->screen ) );
}

void Game_ChangeState( Game_t* game, GameState_t newState )
{
   game->state = newState;

   switch ( newState )
   {
      case GameState_Overworld:
         game->overworldInactivitySeconds = 0.0f;
         break;
   }
}

void Game_EnterTargetPortal( Game_t* game )
{
   uint32_t destinationTileIndex = game->targetPortal->destinationTileIndex;
   Direction_t arrivalDirection = game->targetPortal->arrivalDirection;

   TileMap_Load( &( game->tileMap ), game->targetPortal->destinationTileMapIndex );

   game->player.sprite.position.x = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex % game->tileMap.tilesX ) ) - game->player.spriteOffset.x ) + COLLISION_THETA;
   // the player sprite gets caught on unpassable tiles unless we use COLLISION_THETA here, but for some reason the x-axis has no problems
   game->player.sprite.position.y = (float)( ( int32_t )( TILE_SIZE * ( destinationTileIndex / game->tileMap.tilesX ) ) - game->player.spriteOffset.y ) - COLLISION_THETA;
   game->player.tileIndex = destinationTileIndex;
   game->player.maxVelocity = TileMap_GetWalkSpeedForTileIndex( &( game->tileMap ), destinationTileIndex );
   game->targetPortal = 0;

   Sprite_SetDirection( &( game->player.sprite ), arrivalDirection );

   if ( game->tileMap.isDark )
   {
      TileMap_ChangeViewportSize( &( game->tileMap ),
                                  (uint16_t)( game->tileMap.glowDiameter * TILE_SIZE ),
                                  (uint16_t)( game->tileMap.glowDiameter * TILE_SIZE ) );
   }
   else
   {
      TileMap_ResetViewport( &( game->tileMap ) );
   }

   TileMap_UpdateViewport( &( game->tileMap ),
                           (int32_t)( game->player.sprite.position.x ), (int32_t)( game->player.sprite.position.y ),
                           game->player.hitBoxSize.x, game->player.hitBoxSize.y );
}

void Game_OpenMenu( Game_t* game, MenuId_t id )
{
   Menu_Load( &( game->menu ), id );

   switch ( id )
   {
      case MenuId_Overworld: Game_ChangeState( game, GameState_Overworld_MainMenu ); break;
      case MenuId_OverworldSpell: Game_ChangeState( game, GameState_Overworld_SpellMenu ); break;
      case MenuId_OverworldItem: Game_ChangeState( game, GameState_Overworld_ItemMenu ); break;
   }
}

void Game_OpenScrollingDialog( Game_t* game, ScrollingDialogType_t type, DialogMessageId_t messageId )
{
   ScrollingDialog_Load( &( game->scrollingDialog ), type, messageId );

   switch ( type )
   {
      case ScrollingDialogType_Overworld:
         Game_ChangeState( game, GameState_Overworld_ScrollingDialog );
         break;
   }
}

void Game_Search( Game_t* game )
{
   uint32_t treasureFlag;

   if ( game->tileMap.id == TILEMAP_OVERWORLD_ID && game->player.tileIndex == TILEMAP_TOKEN_INDEX && !ITEM_HAS_TOKEN( game->player.items ) )
   {
      Player_CollectItem( &( game->player ), Item_Token );
      ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_FOUNDITEM_TOKEN );
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Search_FoundItem );
   }
   else if ( game->tileMap.id == TILEMAP_KOL_ID && game->player.tileIndex == TILEMAP_FAIRYFLUTE_INDEX && !ITEM_HAS_FAIRYFLUTE( game->player.items ) )
   {
      Player_CollectItem( &( game->player ), Item_FairyFlute );
      ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_FOUNDITEM_FAIRYFLUTE );
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Search_FoundItem );
   }
   else if ( game->tileMap.id == TILEMAP_CHARLOCK_ID && game->player.tileIndex == TILEMAP_HIDDENSTAIRS_INDEX && !game->tileMap.foundHiddenStairs )
   {
      game->tileMap.foundHiddenStairs = True;
      TileMap_LoadHiddenStairs( &( game->tileMap ) );
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Search_FoundHiddenStairs );
   }
   else
   {
      treasureFlag = TileMap_GetTreasureFlag( game->tileMap.id, game->player.tileIndex );

      if ( treasureFlag && ( game->tileMap.treasureFlags & treasureFlag ) )
      {
         Game_CollectTreasure( game, treasureFlag );
      }
      else
      {
         Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Search_NothingFound );
      }
   }
}

void Game_OpenDoor( Game_t* game )
{
   uint32_t doorTileIndex = TileMap_GetFacingTileIndex( &( game->tileMap ), game->player.tileIndex, game->player.sprite.direction );
   uint32_t doorFlag = TileMap_GetDoorFlag( game->tileMap.id, doorTileIndex );

   if ( doorFlag && ( game->tileMap.doorFlags & doorFlag ) )
   {
      if ( !ITEM_GET_KEYCOUNT( game->player.items ) )
      {
         Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Door_NoKeys );
      }
      else
      {
         ITEM_SET_KEYCOUNT( game->player.items, ITEM_GET_KEYCOUNT( game->player.items ) - 1 );
         game->tileMap.doorFlags ^= doorFlag;
         Game_ChangeState( game, GameState_Overworld );
      }
   }
   else
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Door_None );
   }
}

internal void Game_TicOverworld( Game_t* game )
{
   Game_TicPhysics( game );
   Sprite_Tic( &( game->player.sprite ) );

#if defined( VISUAL_STUDIO_DEV )
   if ( game->tileMap.isDark )
   {
      if ( g_debugFlags.noDark )
      {
         TileMap_ChangeViewportSize( &( game->tileMap ), SCREEN_WIDTH, SCREEN_HEIGHT );
      }
      else
      {
         TileMap_ChangeViewportSize( &( game->tileMap ),
                                     TILE_SIZE * (uint16_t)( game->tileMap.glowDiameter ),
                                     TILE_SIZE * (uint16_t)( game->tileMap.glowDiameter ) );
      }
   }
#endif

   if ( game->tileMap.isDark && game->tileMap.glowDiameter < game->tileMap.targetGlowDiameter )
   {
      game->glowExpandSeconds += CLOCK_FRAME_SECONDS;

      while ( game->glowExpandSeconds > GLOW_EXPAND_FRAME_SECONDS )
      {
         game->glowExpandSeconds -= GLOW_EXPAND_FRAME_SECONDS;
         TileMap_IncreaseGlowDiameter( &( game->tileMap ) );
      }
   }

   TileMap_UpdateViewport( &( game->tileMap ),
                           (int32_t)( game->player.sprite.position.x ), (int32_t)( game->player.sprite.position.y ),
                           game->player.hitBoxSize.x, game->player.hitBoxSize.y );
}

internal void Game_CollectTreasure( Game_t* game, uint32_t treasureFlag )
{
   uint16_t gold = 0;
   Bool_t collected = False;
   char msg[6];

   switch ( treasureFlag )
   {
      case 0x1:         // Tantegel throne room, upper-right chest
         collected = Player_CollectItem( &( game->player ), Item_Key );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_KEY );
         break;
      case 0x2:         // Tantegel throne room, lower-left chest
         gold = 120; break;
      case 0x4:         // Tantegel throne room, lower-right chest
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_HERB );
         break;
      case 0x8:         // Tantegel ground floor, left room, upper-left chest
         gold = Random_u16( 6, 13 ); break;
      case 0x10:        // Tantegel ground floor, left room, middle chest
         gold = Random_u16( 6, 13 ); break;
      case 0x20:        // Tantegel ground floor, left room, bottom-left chest
         gold = Random_u16( 6, 13 ); break;
      case 0x40:        // Tantegel ground floor, left room, bottom-right chest
         gold = Random_u16( 6, 13 ); break;
      case 0x80:        // Tantegel basement
         collected = Player_CollectItem( &( game->player ), Item_StoneOfSunlight );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_STONEOFSUNLIGHT );
         break;
      case 0x100:       // Erdrick's Cave, the tablet. this is not an item that can be collected.
         Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Chest_Tablet );
         game->tileMap.treasureFlags ^= treasureFlag;
         return;
      case 0x200:       // Rimuldar Inn
         collected = Player_CollectItem( &( game->player ), Item_Wing );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_WING );
         break;
      case 0x400:       // Rocky Mountain Cave B1
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_HERB );
         break;
      case 0x800:       // Rocky Mountain Cave B2, upper-left area, left chest
         collected = Player_CollectItem( &( game->player ), Item_DragonScale );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_DRAGONSCALE );
         break;
      case 0x1000:      // Rocky Mountain Cave B2, upper-left area, right chest
         collected = Player_CollectItem( &( game->player ), Item_Torch );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_TORCH );
         break;
      case 0x2000:      // Rocky Mountain Cave B2, center-left chest
         if ( Random_Percent() <= 5 )
         {
            game->tileMap.treasureFlags ^= treasureFlag;
            Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, DialogMessageId_Chest_DeathNecklace );
            return;
         }
         else
         {
            gold = Random_u16( 100, 131 ); break;
         }
         break;
      case 0x4000:      // Rocky Mountain Cave B2, center-right chest
         gold = Random_u16( 10, 17 ); break;
      case 0x8000:      // Garinham, top-left chest
         gold = Random_u16( 10, 17 ); break;
      case 0x10000:     // Garinham, top-right chest
         collected = Player_CollectItem( &( game->player ), Item_Torch );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_TORCH );
         break;
      case 0x20000:     // Garinham, bottom-left chest
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_HERB );
         break;
      case 0x40000:     // Garin's Grave B1, left chest
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_HERB );
         break;
      case 0x80000:     // Garin's Grave B1, center chest
         gold = Random_u16( 5, 20 ); break;
      case 0x100000:    // Garin's Grave B1, right chest
         gold = Random_u16( 6, 13 ); break;
      case 0x200000:    // Garin's Grave B3, upper-left chest
         collected = Player_CollectItem( &( game->player ), Item_CursedBelt );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_CURSEDBELT );
         break;
      case 0x400000:    // Garin's Grave B3, right chest
         collected = Player_CollectItem( &( game->player ), Item_SilverHarp );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_SILVERHARP );
         break;
      case 0x800000:    // Charlock B2
         // TODO: should be Erdrick's Sword
         gold = 1000; break;
      case 0x1000000:   // Charlock B7, top chest
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_HERB );
         break;
      case 0x2000000:   // Charlock B7, center-left chest
         gold = Random_u16( 500, 755 ); break;
      case 0x4000000:   // Charlock B7, center-right chest
         collected = Player_CollectItem( &( game->player ), Item_Key );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_KEY );
         break;
      case 0x8000000:   // Charlock B7, bottom-left chest
         collected = Player_CollectItem( &( game->player ), Item_Wing );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_WING );
         break;
      case 0x10000000:  // Charlock B7, bottom-center chest
         collected = Player_CollectItem( &( game->player ), Item_CursedBelt );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_CURSEDBELT );
         break;
      case 0x20000000:  // Charlock B7, bottom-right chest
         collected = Player_CollectItem( &( game->player ), Item_Herb );
         ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), STRING_CHESTCOLLECT_HERB );
         break;
   }

   if ( gold > 0 )
   {
      collected = ( Player_CollectGold( &( game->player ), gold ) > 0 ) ? True : False;
      sprintf( msg, "%u", gold );
      ScrollingDialog_SetInsertionText( &( game->scrollingDialog ), msg );
      Game_DrawOverworldQuickStatus( game );
   }

   if ( collected )
   {
      game->tileMap.treasureFlags ^= treasureFlag;
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, ( gold > 0 ) ? DialogMessageId_Chest_GoldCollected : DialogMessageId_Chest_ItemCollected );
   }
   else
   {
      Game_OpenScrollingDialog( game, ScrollingDialogType_Overworld, ( gold > 0 ) ? DialogMessageId_Chest_GoldNoSpace : DialogMessageId_Chest_ItemNoSpace );
   }
}
