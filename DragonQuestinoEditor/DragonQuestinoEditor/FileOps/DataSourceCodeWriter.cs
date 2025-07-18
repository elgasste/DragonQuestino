﻿using System.Collections.ObjectModel;
using System.IO;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Graphics;
using DragonQuestinoEditor.Utilities;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class DataSourceCodeWriter( Palette palette,
                                      TileSet tileSet,
                                      ObservableCollection<TileMapViewModel> tileMaps,
                                      ObservableCollection<EnemyViewModel> enemies,
                                      ObservableCollection<ActiveSpriteSheet> activeSpriteSheets,
                                      StaticSpriteSheet staticSpriteSheet )
   {
      private readonly Palette _palette = palette;
      private readonly TileSet _tileSet = tileSet;
      private readonly ObservableCollection<TileMapViewModel> _tileMaps = tileMaps;
      private readonly ObservableCollection<EnemyViewModel> _enemies = enemies;
      private readonly ObservableCollection<ActiveSpriteSheet> _activeSpriteSheets = activeSpriteSheets;
      private readonly StaticSpriteSheet _staticSpriteSheet = staticSpriteSheet;

      private readonly List<List<int>> _overworldEnemyIndexPools =
      [
         [0, 1, 0, 1, 0],
         [1, 0, 1, 2, 1],
         [0, 3, 2, 3, 1],
         [1, 1, 2, 3, 4],
         [3, 4, 5, 5, 6],
         [3, 4, 5, 6, 11],
         [5, 6, 11, 12, 14],
         [11, 12, 13, 14, 14],
         [13, 15, 18, 18, 25],
         [15, 21, 18, 21, 25],
         [21, 22, 23, 26, 28],
         [23, 26, 27, 27, 16],
         [26, 27, 28, 29, 31],
         [29, 30, 31, 31, 32]
      ];

      private readonly List<List<int>> _dungeonEnemyIndexPools =
      [
         [3, 4, 6, 7, 7],
         [8, 9, 10, 11, 12],
         [11, 12, 13, 14, 14],
         [17, 18, 19, 20, 23],
         [29, 30, 31, 32, 33],
         [32, 33, 34, 34, 35],
         [32, 35, 36, 36, 37]
      ];

      public void WriteFiles()
      {
         WriteGeneratedHeaderFile();
         WriteGameDataFile();
         WriteShieldBackgroundDataFile();
      }

      private void WriteGeneratedHeaderFile()
      {
         using FileStream fs = File.Create( Constants.GeneratedDefinesSourceFilePath );

         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#if !defined( GENERATED_DEFINES_H )\n" );
         WriteToFileStream( fs, "#define GENERATED_DEFINES_H\n\n" );
         WriteToFileStream( fs, string.Format( "#define PALETTE_COLORS {0}\n", _palette.ColorCount ) );

         int highestCount = 0;
         foreach( var pool in _overworldEnemyIndexPools )
         {
            if ( pool.Count > highestCount )
            {
               highestCount = pool.Count;
            }
         }
         foreach ( var pool in _dungeonEnemyIndexPools )
         {
            if ( pool.Count > highestCount )
            {
               highestCount = pool.Count;
            }
         }
         WriteToFileStream( fs, string.Format( "#define TILE_MAX_ENEMY_INDEX_POOL_ENEMIES {0}\n", highestCount ) );

         WriteToFileStream( fs, string.Format( "#define TILE_OVERWORLD_ENEMY_INDEX_POOLS {0}\n", _overworldEnemyIndexPools.Count ) );
         WriteToFileStream( fs, string.Format( "#define TILE_DUNGEON_ENEMY_INDEX_POOLS {0}\n", _dungeonEnemyIndexPools.Count ) );
         WriteToFileStream( fs, string.Format( "#define ACTIVE_SPRITE_PLAYER_ID {0}\n\n", Constants.PlayerActiveSpriteIndex ) );

         WriteToFileStream( fs, "#endif // GENERATED_DEFINES_H\n" );
      }

      private void WriteGameDataFile()
      {
         using FileStream fs = File.Create( Constants.GameDataSourceFilePath );
         WriteHeaderSection( fs );
         WritePaletteFunction( fs );
         WriteTextTilesFunction( fs );
         WriteTileTexturesFunction( fs );
         WriteEnemyIndexPoolsFunction( fs );
         WriteEnemyLoadFunction( fs );
         WriteTileMapFunction( fs );
         WriteTileMapHiddenStairsFunction( fs );
         WriteActiveSpritesFunctions( fs );
         WriteStaticSpritesFunction( fs );
         WriteTreasureFlagsFunction( fs );
         WriteDoorFlagsFunction( fs );
      }

      private void WriteShieldBackgroundDataFile()
      {
         using FileStream fs = File.Create( Constants.ShieldBackgroundSourceFilePath );
         WriteShieldBackgroundFunction( fs );
      }


      private static void WriteHeaderSection( FileStream fs )
      {
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#include \"game.h\"\n" );
         WriteToFileStream( fs, "#include \"random.h\"\n" );
      }

      private void WritePaletteFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Screen_LoadPalette( Screen_t* screen )\n" );
         WriteToFileStream( fs, "{\n" );

         for ( int i = 0; i < _palette.ColorCount; i++ )
         {
            WriteToFileStream( fs, string.Format( "   screen->palette[{0}] = 0x{1};\n", i, _palette.Colors[i].ToString( "X4" ) ) );
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileTexturesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadTextures( TileMap_t* tileMap )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t* mem32;\n\n" );

         // TODO: try compressing this, it only ever gets called once
         for ( int i = 0; i < Constants.MapTileTextureCount; i++ )
         {
            WriteToFileStream( fs, string.Format( "   mem32 = (uint32_t*)( tileMap->textures[{0}].memory );\n", i ) );

            var pixelIndexes = _tileSet.TilePaletteIndexes[i];

            for ( int j = 0, memoryIndex = 0; j < Constants.MapTilePixels; j += 4, memoryIndex++ )
            {
               var index0 = (UInt32)( pixelIndexes[j + 0] );
               var index1 = (UInt32)( pixelIndexes[j + 1] );
               var index2 = (UInt32)( pixelIndexes[j + 2] );
               var index3 = (UInt32)( pixelIndexes[j + 3] );

               var packed = ( index3 << 24 ) | ( index2 << 16  ) | ( index1 << 8  ) | ( index0 << 0  );

               WriteToFileStream( fs, string.Format( "   mem32[{0}] = 0x{1};\n", memoryIndex, packed.ToString( "X8" ) ) );
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteEnemyIndexPoolsFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadEnemyIndexPools( TileMap_t* tileMap )\n" );
         WriteToFileStream( fs, "{\n" );

         for ( int i = 0; i < _overworldEnemyIndexPools.Count; i++ )
         {
            for ( int j = 0; j < _overworldEnemyIndexPools[i].Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "   tileMap->overworldEnemyIndexPools[{0}].enemyIndexes[{1}] = {2};\n", i, j, _overworldEnemyIndexPools[i][j] ) );
            }
         }

         for ( int i = 0; i < _dungeonEnemyIndexPools.Count; i++ )
         {
            for ( int j = 0; j < _dungeonEnemyIndexPools[i].Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "   tileMap->dungeonEnemyIndexPools[{0}].enemyIndexes[{1}] = {2};\n", i, j, _dungeonEnemyIndexPools[i][j] ) );
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteEnemyLoadFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Enemy_Load( Enemy_t* enemy, uint32_t index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t i, j;\n" );
         WriteToFileStream( fs, "   uint32_t* mem32;" );

         var blankPaletteIndex = (ushort)_palette.GetIndexForColor( 0 );
         var packedBlankPaletteIndex = ( blankPaletteIndex << 24 ) | ( blankPaletteIndex << 16 ) | ( blankPaletteIndex << 8 ) | ( blankPaletteIndex << 0 );

         WriteToFileStream( fs, string.Format( "   for ( i = 0; i < 78; i++ ) {{ mem32 = (uint32_t*)( enemy->tileTextures[i] ); for ( j = 0; j < 16; j++ ) {{ mem32[j] = 0x{0}; }} }}\n", packedBlankPaletteIndex.ToString( "X8" ) ) );
         WriteToFileStream( fs, "   for ( i = 0; i < 120; i++ ) {{ enemy->tileTextureIndexes[i] = -1; }}\n\n" );

         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         var sortedEnemies = _enemies.OrderBy( e => e.Index ).ToList();

         foreach ( var enemy in sortedEnemies )
         {
            int groupId = enemy.Index < 20 ? 0 : enemy.Index < 30 ? 1 : enemy.Index < 35 ? 2 : 3;
            WriteToFileStream( fs, string.Format( "   case {0}:\n", enemy.Index ) );
            WriteToFileStream( fs, string.Format( "      enemy->id = {0};\n", enemy.Index ) );
            WriteToFileStream( fs, string.Format( "      enemy->groupId = {0};\n", groupId ) );
            WriteToFileStream( fs, string.Format( "      strcpy( enemy->name, {0} );\n", enemy.NameMacro ) );
            WriteToFileStream( fs, string.Format( "      strcpy( enemy->indefiniteArticle, {0} );\n", enemy.IndefiniteArticle == "An" ? "STRING_INDEFINITE_ARTICLE_AN" : "STRING_INDEFINITE_ARTICLE_A" ) );
            WriteToFileStream( fs, string.Format( "      enemy->minHitPoints = {0};\n", enemy.MinHitPoints ) );
            WriteToFileStream( fs, string.Format( "      enemy->maxHitPoints = {0};\n", enemy.MaxHitPoints ) );
            WriteToFileStream( fs, string.Format( "      enemy->experience = {0};\n", enemy.Experience ) );
            WriteToFileStream( fs, string.Format( "      enemy->minGold = {0};\n", enemy.MinGold ) );
            WriteToFileStream( fs, string.Format( "      enemy->maxGold = {0};\n", enemy.MaxGold ) );
            WriteToFileStream( fs, string.Format( "      enemy->stats.strength = {0};\n", enemy.Strength ) );
            WriteToFileStream( fs, string.Format( "      enemy->stats.agility = {0};\n", enemy.Agility ) );
            WriteToFileStream( fs, string.Format( "      enemy->stats.sleepResist = {0};\n", enemy.SleepResist ) );
            WriteToFileStream( fs, string.Format( "      enemy->stats.stopSpellResist = {0};\n", enemy.StopSpellResist ) );
            WriteToFileStream( fs, string.Format( "      enemy->stats.hurtResist = {0};\n", enemy.HurtResist ) );
            WriteToFileStream( fs, string.Format( "      enemy->stats.dodge = {0};\n", enemy.Dodge ) );

            for ( int i = 0; i < enemy.TextureIndexes.Count; i++ )
            {
               if ( enemy.TextureIndexes[i] >= 0 )
               {
                  WriteToFileStream( fs, string.Format( "      enemy->tileTextureIndexes[{0}] = {1};\n", i, enemy.TextureIndexes[i] ) );
               }
            }

            for ( int i = 0; i < enemy.TileTextures.Count; i++ )
            {
               WriteToFileStream( fs, string.Format( "      mem32 = (uint32_t*)( enemy->tileTextures[{0}] );\n", i ) );

               for ( int j = 0, idx = 0; j < enemy.TileTextures[i].Count; j++, idx++ )
               {
                  var index0 = (ushort)enemy.TileTextures[i][j++];
                  var index1 = (ushort)enemy.TileTextures[i][j++];
                  var index2 = (ushort)enemy.TileTextures[i][j++];
                  var index3 = (ushort)enemy.TileTextures[i][j];
                  var packedIndexes = ( index3 << 24 ) | ( index2 << 16 ) | ( index1 << 8 ) | ( index0 << 0 );

                  if ( packedIndexes != packedBlankPaletteIndex )
                  {
                     WriteToFileStream( fs, string.Format( "      mem32[{0}] = 0x{1};\n", idx, packedIndexes.ToString( "X8" ) ) );
                  }
               }
            }

            WriteToFileStream( fs, "      break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileMapFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_Load( TileMap_t* tileMap, uint32_t id )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   int32_t i, j;\n" );
         WriteToFileStream( fs, "   uint32_t* tiles32 = (uint32_t*)( tileMap->tiles );\n\n" );
         WriteToFileStream( fs, "   if ( id == TILEMAP_OVERWORLD_ID ) tileMap->gameFlags->doors = ( 0xFFFF0000 | ( tileMap->gameFlags->doors & 0xFFFF ) );\n\n" );
         WriteToFileStream( fs, "   Random_Seed();\n\n" );
         WriteToFileStream( fs, "   switch( id )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int tm = 0; tm < _tileMaps.Count; tm++ )
         {
            var tileMap = _tileMaps[ tm ];

            var tiles = tileMap.Tiles;

            WriteToFileStream( fs, string.Format( "      case {0}:\n", tileMap.Id ) );
            WriteToFileStream( fs, string.Format( "         tileMap->id = {0};\n", tm ) );
            WriteToFileStream( fs, string.Format( "         tileMap->hasEncounters = {0};\n", tileMap.HasEncounters ? "True" : "False" ) );
            WriteToFileStream( fs, string.Format( "         tileMap->blocksMagic = {0};\n", tileMap.BlocksMagic ? "True" : "False" ) );
            WriteToFileStream( fs, string.Format( "         tileMap->isDungeon = {0};\n", tileMap.IsDungeon ? "True" : "False" ) );
            WriteToFileStream( fs, string.Format( "         tileMap->isDark = {0};\n", tileMap.IsDark ? "True" : "False" ) );
            WriteToFileStream( fs, string.Format( "         tileMap->tilesX = {0};\n", tileMap.TilesX ) );
            WriteToFileStream( fs, string.Format( "         tileMap->tilesY = {0};\n", tileMap.TilesY ) );
            WriteToFileStream( fs, string.Format( "         tileMap->portalCount = {0};\n", tileMap.Portals.Count ) );

            if ( tileMap.Portals.Count > 0 )
            {
               for ( int i = 0; i < tileMap.Portals.Count; i++ )
               {
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].sourceTileIndex = {1};\n", i, tileMap.Portals[i].SourceTileIndex ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].destinationTileMapIndex = {1};\n", i, tileMap.Portals[i].DestinationTileMapIndex ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].destinationTileIndex = {1};\n", i, tileMap.Portals[i].DestinationTileIndex ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].arrivalDirection = (Direction_t){1};\n", i, (int)( tileMap.Portals[i].ArrivalDirection ) ) );
               }
            }

            WriteToFileStream( fs, string.Format( "         tileMap->evacPortal.destinationTileMapIndex = {0};\n", tileMap.EvacPortal.DestinationTileMapIndex ) );
            WriteToFileStream( fs, string.Format( "         tileMap->evacPortal.destinationTileIndex = {0};\n", tileMap.EvacPortal.DestinationTileIndex ) );
            WriteToFileStream( fs, string.Format( "         tileMap->evacPortal.arrivalDirection = (Direction_t){0};\n", (int)( tileMap.EvacPortal.ArrivalDirection ) ) );

            WriteToFileStream( fs, string.Format( "         tileMap->staticSpriteCount = {0};\n", tileMap.StaticSprites.Count ) );

            for ( int i = 0; i < tileMap.StaticSprites.Count; i++ )
            {
               var sprite = tileMap.StaticSprites[i];
               int xPos = ( sprite.TileIndex % tileMap.TilesX ) * Constants.SpriteFrameSize;
               int yPos = ( sprite.TileIndex / tileMap.TilesX ) * Constants.SpriteFrameSize;

               WriteToFileStream( fs, string.Format( "         Sprite_LoadStatic( &( tileMap->staticSprites[{0}] ), {1} );\n", i, sprite.TextureIndex ) );
               WriteToFileStream( fs, string.Format( "         tileMap->staticSprites[{0}].position.x = {1};\n", i, xPos ) );
               WriteToFileStream( fs, string.Format( "         tileMap->staticSprites[{0}].position.y = {1};\n", i, yPos ) );
            }

            WriteToFileStream( fs, string.Format( "         tileMap->npcCount = {0};\n", tileMap.NonPlayerCharacters.Count ) );

            if ( tileMap.NonPlayerCharacters.Count > 0 )
            {
               WriteToFileStream( fs, "         for ( i = 0; i < (int32_t)( tileMap->npcCount ); i++ ) ActiveSprite_Reset( &( tileMap->npcs[i].sprite ) );\n" );

               for ( int i = 0; i < tileMap.NonPlayerCharacters.Count; i++ )
               {
                  var npc = tileMap.NonPlayerCharacters[i];
                  int xPos = ( npc.TileIndex % tileMap.TilesX ) * Constants.SpriteFrameSize;
                  int yPos = ( npc.TileIndex / tileMap.TilesX ) * Constants.SpriteFrameSize;

                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].id = {1};\n", i, npc.Id ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].tileIndex = {1};\n", i, npc.TileIndex ) );
                  WriteToFileStream( fs, string.Format( "         Sprite_LoadActive( &( tileMap->npcs[{0}].sprite ), {1} );\n", i, npc.ActiveSprite?.SpriteSheetIndex ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].sprite.position.x = {1};\n", i, xPos - npc.ActiveSprite?.Offset.X ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].sprite.position.y = {1};\n", i, yPos - npc.ActiveSprite?.Offset.Y ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].sprite.direction = (Direction_t){1};\n", i, (int)npc.Direction ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].sprite.offset.x = {1};\n", i, npc.ActiveSprite?.Offset.X ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].sprite.offset.y = {1};\n", i, npc.ActiveSprite?.Offset.Y ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].sprite.hitBoxSize.x = {1};\n", i, npc.ActiveSprite?.HitBoxSize.X ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].sprite.hitBoxSize.y = {1};\n", i, npc.ActiveSprite?.HitBoxSize.Y ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].wanders = {1};\n", i, npc.Wanders ) );

                  if ( npc.Wanders )
                  {
                     WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].isWandering = False;\n", i ) );
                     WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].wanderBounds.x = {1};\n", i, npc.WanderBounds?.X ) );
                     WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].wanderBounds.y = {1};\n", i, npc.WanderBounds?.Y ) );
                     WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].wanderBounds.w = {1};\n", i, npc.WanderBounds?.W ) );
                     WriteToFileStream( fs, string.Format( "         tileMap->npcs[{0}].wanderBounds.h = {1};\n", i, npc.WanderBounds?.H ) );
                  }
               }
            }

            var packedTiles = new List<UInt32>( ( tileMap.TilesX / 2 ) * tileMap.TilesY );
            var indexCounts = new Dictionary<UInt32, int>();

            for ( int row = 0; row < tileMap.TilesY; row++ )
            {
               for ( int col = 0; col < tileMap.TilesX; col += 2 )
               {
                  var tileIndex = ( row * tileMap.TilesX ) + col;

                  var index0 = (UInt32)( tiles[tileIndex].TextureIndex )
                     | ( tiles[tileIndex].IsPassable ? (UInt32)0x20 : 0 )
                     | Constants.TileSetIndexWalkSpeeds[tiles[tileIndex].TextureIndex]
                     | Constants.TileSetIndexEncounterRates[tiles[tileIndex].TextureIndex]
                     | Constants.TileSetIndexDamageRates[tiles[tileIndex].TextureIndex]
                     | ( ( (UInt32)tiles[tileIndex].EnemyPoolIndex & 0xF ) << 12 );
                  var index1 = (UInt32)( tiles[tileIndex + 1].TextureIndex )
                     | ( tiles[tileIndex + 1].IsPassable ? (UInt32)0x20 : 0 )
                     | Constants.TileSetIndexWalkSpeeds[tiles[tileIndex + 1].TextureIndex]
                     | Constants.TileSetIndexEncounterRates[tiles[tileIndex + 1].TextureIndex]
                     | Constants.TileSetIndexDamageRates[tiles[tileIndex + 1].TextureIndex]
                     | ( ( (UInt32)tiles[tileIndex + 1].EnemyPoolIndex & 0xF ) << 12 );

                  var packed = ( index1 << 16 ) | index0;
                  packedTiles.Add( packed );

                  if ( indexCounts.TryGetValue( packed, out int value ) )
                  {
                     indexCounts[packed] = ++value;
                  }
                  else
                  {
                     indexCounts[packed] = 1;
                  }
               }
            }

            int highestCount = 0;
            UInt32 mostCommonValue = 0;

            foreach ( var pair in indexCounts )
            {
               if ( pair.Value > highestCount )
               {
                  highestCount = pair.Value;
                  mostCommonValue = pair.Key;
               }
            }

            WriteToFileStream( fs, string.Format( "         for ( i = 0; i < {0}; i++ ) for ( j = 0; j < {1}; j++ ) tiles32[(i * {2}) + j] = 0x{3};\n",
               tileMap.TilesY, tileMap.TilesX / 2, Constants.TileMapMaxTilesX / 2, mostCommonValue.ToString( "X8" ) ) );

            for ( int i = 0; i < packedTiles.Count; )
            {
               int firstIndex = i;
               int lastIndex = i;
               var currentTile = packedTiles[i];
               i++;

               if ( currentTile != mostCommonValue )
               {
                  while ( i < packedTiles.Count )
                  {
                     var nextTile = packedTiles[i];
                     lastIndex = i;
                     i++;

                     if ( nextTile != currentTile )
                     {
                        break;
                     }
                  }

                  int row = firstIndex / ( tileMap.TilesX / 2 );
                  int col = firstIndex % ( tileMap.TilesX / 2 );
                  int firstTileIndex32 = ( row * ( Constants.TileMapMaxTilesX / 2 ) ) + col;

                  row = lastIndex / ( tileMap.TilesX / 2 );
                  col = lastIndex % ( tileMap.TilesX / 2 );
                  int lastTileIndex32 = ( row * ( Constants.TileMapMaxTilesX / 2 ) ) + col;

                  if ( lastIndex == firstIndex )
                  {
                     WriteToFileStream( fs, string.Format( "         tiles32[{0}] = 0x{1};\n", firstTileIndex32, packedTiles[firstIndex].ToString( "X8" ) ) );
                  }
                  else
                  {
                     if ( ( lastIndex - firstIndex ) > 1 )
                     {
                        WriteToFileStream( fs, string.Format( "         for ( i = {0}; i < {1}; i++ ) tiles32[i] = 0x{2};\n", firstTileIndex32, lastTileIndex32, packedTiles[firstIndex].ToString( "X8" ) ) );
                     }
                     else
                     {
                        WriteToFileStream( fs, string.Format( "         tiles32[{0}] = 0x{1};\n", firstTileIndex32, packedTiles[firstIndex].ToString( "X8" ) ) );  
                     }

                     i--;
                  }
               }
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n\n" );

         WriteToFileStream( fs, "   if ( id == TILEMAP_OVERWORLD_ID && tileMap->gameFlags->usedRainbowDrop )\n" );
         WriteToFileStream( fs, "   {\n" );
         WriteToFileStream( fs, "      TILE_SET_TEXTUREINDEX( tileMap->tiles[TILEMAP_RAINBOWBRIDGE_INDEX], 13 );\n" );
         WriteToFileStream( fs, "      TILE_TOGGLE_PASSABLE( tileMap->tiles[TILEMAP_RAINBOWBRIDGE_INDEX] );\n" );
         WriteToFileStream( fs, "   }\n\n" );

         WriteToFileStream( fs, "   if ( id == TILEMAP_CHARLOCK_ID && tileMap->gameFlags->foundHiddenStairs )\n" );
         WriteToFileStream( fs, "   {\n" );
         WriteToFileStream( fs, "      TileMap_LoadHiddenStairs( tileMap );\n" );
         WriteToFileStream( fs, "   }\n\n" );

         WriteToFileStream( fs, "   TileMap_ResetNpcs( tileMap );\n" );

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileMapHiddenStairsFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadHiddenStairs( TileMap_t* tileMap )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   Sprite_LoadStatic( &( tileMap->staticSprites[tileMap->staticSpriteCount] ), 3 );\n" );
         WriteToFileStream( fs, "   tileMap->staticSprites[tileMap->staticSpriteCount].position.x = ( TILEMAP_HIDDENSTAIRS_INDEX % tileMap->tilesX ) * TILE_SIZE;\n" );
         WriteToFileStream( fs, "   tileMap->staticSprites[tileMap->staticSpriteCount].position.y = ( TILEMAP_HIDDENSTAIRS_INDEX / tileMap->tilesX ) * TILE_SIZE;\n" );
         WriteToFileStream( fs, "   tileMap->staticSpriteCount++;\n" );
         WriteToFileStream( fs, "   tileMap->portals[tileMap->portalCount].sourceTileIndex = TILEMAP_HIDDENSTAIRS_INDEX;\n" );
         WriteToFileStream( fs, "   tileMap->portals[tileMap->portalCount].destinationTileMapIndex = TILEMAP_HIDDENSTAIRS_DESTINATION_ID;\n" );
         WriteToFileStream( fs, "   tileMap->portals[tileMap->portalCount].destinationTileIndex = TILEMAP_HIDDENSTAIRS_DESTINATION_INDEX;\n" );
         WriteToFileStream( fs, "   tileMap->portals[tileMap->portalCount].arrivalDirection = TILEMAP_HIDDENSTAIRS_DESTINATION_DIR;\n" );
         WriteToFileStream( fs, "   tileMap->portalCount++;\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteActiveSpritesFunctions( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Sprite_LoadActive( ActiveSprite_t* sprite, uint32_t index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   int32_t i;\n" );
         WriteToFileStream( fs, "   uint32_t* mem32 = (uint32_t*)( sprite->textures[0].memory );\n\n" );

         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _activeSpriteSheets.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );
            WriteActiveSpriteData( fs, _activeSpriteSheets[i] );
            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteActiveSpriteData( FileStream fs, ActiveSpriteSheet spriteSheet )
      {
         var indexCounts = new Dictionary<UInt32, int>();
         var packedIndexes = new List<UInt32>();

         for ( int i = 0; i < Constants.ActiveSpritePositionCount; i++ )
         {
            for ( int j = 0; j < Constants.ActiveSpriteFrameCount; j++ )
            {
               var pixelIndexes = spriteSheet.FramePaletteIndexes[i][j];

               for ( int k = 0; k < Constants.SpriteFramePixels; k += 4 )
               {
                  var index0 = (UInt32)( pixelIndexes[k + 0] );
                  var index1 = (UInt32)( pixelIndexes[k + 1] );
                  var index2 = (UInt32)( pixelIndexes[k + 2] );
                  var index3 = (UInt32)( pixelIndexes[k + 3] );
                  var packed = ( index3 << 24 ) | ( index2 << 16 ) | ( index1 << 8 ) | ( index0 << 0 );

                  packedIndexes.Add( packed );

                  if ( indexCounts.TryGetValue( packed, out int value ) )
                  {
                     indexCounts[packed] = ++value;
                  }
                  else
                  {
                     indexCounts[packed] = 1;
                  }
               }
            }
         }

         int highestCount = 0;
         UInt32 mostCommonValue = 0;

         foreach ( var pair in indexCounts )
         {
            if ( pair.Value > highestCount )
            {
               highestCount = pair.Value;
               mostCommonValue = pair.Key;
            }
         }

         WriteToFileStream( fs, string.Format( "         for ( i = 0; i < ( SPRITE_TEXTURE_BYTES / 4 ) * ACTIVE_SPRITE_TEXTURES; i++ ) {{ mem32[i] = 0x{0}; }}\n", mostCommonValue.ToString( "X8" ) ) );

         for ( int i = 0, packedIndex = 0; i < Constants.ActiveSpritePositionCount; i++ )
         {
            for ( int j = 0; j < Constants.ActiveSpriteFrameCount; j++ )
            {
               WriteToFileStream( fs, string.Format( "         mem32 = (uint32_t*)( sprite->textures[{0}].memory );\n", ( i * Constants.ActiveSpriteFrameCount ) + j ) );

               for ( int k = 0, memoryIndex = 0; k < Constants.SpriteFramePixels; k += 4, memoryIndex++, packedIndex++ )
               {
                  if ( packedIndexes[packedIndex] != mostCommonValue )
                  {
                     WriteToFileStream( fs, string.Format( "         mem32[{0}] = 0x{1};\n", memoryIndex, packedIndexes[packedIndex].ToString( "X8" ) ) );
                  }
               }
            }
         }
      }

      private void WriteStaticSpritesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Sprite_LoadStatic( StaticSprite_t* sprite, uint32_t index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t* mem32 = (uint32_t*)( sprite->texture.memory );\n\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _staticSpriteSheet.Bitmaps.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );
            
            var pixelIndexes = _staticSpriteSheet.PaletteIndexes[i];

            for ( int k = 0, memoryIndex = 0; k < Constants.SpriteFramePixels; k += 4, memoryIndex++ )
            {
               var index0 = (UInt32)( pixelIndexes[k + 0] );
               var index1 = (UInt32)( pixelIndexes[k + 1] );
               var index2 = (UInt32)( pixelIndexes[k + 2] );
               var index3 = (UInt32)( pixelIndexes[k + 3] );
               var packed = ( index3 << 24 ) | ( index2 << 16 ) | ( index1 << 8 ) | ( index0 << 0 );

               WriteToFileStream( fs, string.Format( "         mem32[{0}] = 0x{1};\n", memoryIndex, packed.ToString( "X8" ) ) );
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTreasureFlagsFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nuint32_t TileMap_GetTreasureFlag( uint32_t tileMapId, uint32_t tileIndex )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   switch( tileMapId )\n" );
         WriteToFileStream( fs, "   {\n" );

         bool hasTreasure = false;

         for ( int i = 0; i < _tileMaps.Count; i++ )
         {
            for ( int j = 0; j < _tileMaps[i].Tiles.Count; j++ )
            {
               var tile = _tileMaps[i].Tiles[j];
               
               if ( tile.TreasureFlag != 0 )
               {
                  if ( !hasTreasure )
                  {
                     WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );
                     hasTreasure = true;
                  }

                  WriteToFileStream( fs, string.Format( "         if ( tileIndex == {0} ) return 0x{1};\n", j, tile.TreasureFlag.ToString( "X8" ) ) );
               }
            }

            if ( hasTreasure )
            {
               WriteToFileStream( fs, "         break;\n" );
               hasTreasure = false;
            }
         }

         WriteToFileStream( fs, "   }\n\n" );
         WriteToFileStream( fs, "   return 0;\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteDoorFlagsFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nuint32_t TileMap_GetDoorFlag( uint32_t tileMapId, uint32_t tileIndex )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   switch( tileMapId )\n" );
         WriteToFileStream( fs, "   {\n" );

         bool hasDoor = false;

         for ( int i = 0; i < _tileMaps.Count; i++ )
         {
            for ( int j = 0; j < _tileMaps[i].Tiles.Count; j++ )
            {
               var tile = _tileMaps[i].Tiles[j];

               if ( tile.DoorFlag != 0 )
               {
                  if ( !hasDoor )
                  {
                     WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );
                     hasDoor = true;
                  }

                  WriteToFileStream( fs, string.Format( "         if ( tileIndex == {0} ) return 0x{1};\n", j, tile.DoorFlag.ToString( "X8" ) ) );
               }
            }

            if ( hasDoor )
            {
               WriteToFileStream( fs, "         break;\n" );
               hasDoor = false;
            }
         }

         WriteToFileStream( fs, "   }\n\n" );
         WriteToFileStream( fs, "   return 0;\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private static void WriteShieldBackgroundFunction( FileStream fs )
      {
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#include \"giga_shield_background_data.h\"\n\n" );
         WriteToFileStream( fs, "void Giga_LoadShieldBackground( uint32_t* buffer )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t i;\n\n" );

         var textFileStream = new FileStream( Constants.ShieldBackgroundFilePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapUtils.CheckBackgroundBitmapFormat( bitmapSource );

         int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
         var data = new byte[stride * bitmapSource.PixelHeight];
         bitmapSource.CopyPixels( data, stride, 0 );

         var packedPixelCounts = new Dictionary<UInt32, int>();
         List<UInt32> packedPixels = new( ( Constants.GigaShieldWidth / 2 ) * Constants.GigaShieldHeight );

         for ( int i = 0, j = 0, dataIndex = 0; i < Constants.GigaShieldWidth * Constants.GigaShieldHeight; i += 2, j++ )
         {
            var color16 = ColorUtils.BytesToRgb565( data[dataIndex + 2], data[dataIndex + 1], data[dataIndex + 1] );
            dataIndex += 3;
            var packedPixel = (UInt32)color16 << 16;

            color16 = ColorUtils.BytesToRgb565( data[dataIndex + 2], data[dataIndex + 1], data[dataIndex + 1] );
            dataIndex += 3;
            packedPixel |= (UInt32)color16;

            packedPixels.Add( packedPixel );

            if ( packedPixelCounts.TryGetValue( packedPixel, out int value ) )
            {
               packedPixelCounts[packedPixel] = ++value;
            }
            else
            {
               packedPixelCounts[packedPixel] = 1;
            }
         }

         int highestCount = 0;
         UInt32 mostCommonValue = 0;

         foreach ( var c in packedPixelCounts )
         {
            if ( c.Value > highestCount )
            {
               highestCount = c.Value;
               mostCommonValue = c.Key;
            }
         }

         WriteToFileStream( fs, "   for ( i = 0; i < ( GIGA_SHIELD_WIDTH / 2 ) * GIGA_SHIELD_HEIGHT; i++ )\n" );
         WriteToFileStream( fs, "   {\n" );
         WriteToFileStream( fs, string.Format( "      buffer[i] = 0x{0};\n", mostCommonValue.ToString( "X8" ) ) );
         WriteToFileStream( fs, "   }\n\n" );

         for ( int i = 0; i < packedPixels.Count; )
         {
            int firstIndex = i;
            int lastIndex = i;
            var currentPixel = packedPixels[i];
            i++;

            if ( currentPixel != mostCommonValue )
            {
               while ( i < packedPixels.Count )
               {
                  var nextPixel = packedPixels[i];
                  lastIndex = i;
                  i++;

                  if ( nextPixel != currentPixel )
                  {
                     break;
                  }
               }

               if ( lastIndex == firstIndex )
               {
                  WriteToFileStream( fs, string.Format( "   buffer[{0}] = 0x{1};\n", firstIndex, packedPixels[firstIndex].ToString( "X8" ) ) );
               }
               else
               {
                  if ( ( lastIndex - firstIndex ) > 1 )
                  {
                     WriteToFileStream( fs, string.Format( "   for ( i = {0}; i <= {1}; i++ ) buffer[i] = 0x{2};\n", firstIndex, lastIndex, packedPixels[firstIndex].ToString( "X8" ) ) );
                  }
                  else
                  {
                     WriteToFileStream( fs, string.Format( "   buffer[{0}] = 0x{1};\n", firstIndex, packedPixels[firstIndex].ToString( "X8" ) ) );
                  }

                  i--;
               }
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private static void WriteTextTilesFunction( FileStream fs )
      {
         var textTextureMap = new List<byte>();

         var textFileStream = new FileStream( Constants.TextTileSetFilePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         BitmapSource bitmap = textDecoder.Frames[0];
         BitmapUtils.CheckTextTileSetBitmapFormat( bitmap );

         for ( int row = 0; row < bitmap.PixelHeight; row++ )
         {
            for ( int col = 0; col < bitmap.PixelWidth; col += 8 )
            {
               textTextureMap.Add( 0x00 );

               for ( int i = 0; i < Constants.TextTileSize; i++ )
               {
                  var pixelColor = BitmapUtils.GetBitmapPixelColor( bitmap, col + i, row );

                  if ( !Color.AreClose( pixelColor, Color.FromArgb( 255, 0, 0, 0 ) ) )
                  {
                     textTextureMap[^1] |= (byte)( 1 << ( Constants.TextTileSize - i - 1 ) );
                  }
               }
            }
         }

         WriteToFileStream( fs, "\nvoid Screen_LoadTextBitFields( Screen_t* screen )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t i, j;\n\n" );

         var byteCounts = new Dictionary<byte, int>();

         for ( int i = 0; i < Constants.TextTileCount; i++ )
         {
            for ( int j = 0; j < Constants.TextTileSize; j++ )
            {
               byte b = textTextureMap[i + ( j * Constants.TextTileCount )];

               if ( byteCounts.TryGetValue( b, out int value ) )
               {
                  byteCounts[b] = ++value;
               }
               else
               {
                  byteCounts[b] = 1;
               }
            }
         }

         int highestCount = 0;
         byte mostCommonValue = 0;

         foreach ( var pair in byteCounts )
         {
            if ( pair.Value > highestCount )
            {
               highestCount = pair.Value;
               mostCommonValue = pair.Key;
            }
         }

         WriteToFileStream( fs, string.Format(
            "   for ( i = 0; i < TEXT_TILE_COUNT; i++ ) for ( j = 0; j < TEXT_TILE_SIZE; j++ ) screen->textBitFields[i][j] = 0x{0};\n",
            mostCommonValue.ToString( "X2" ) ) );

         for ( int i = 0; i < Constants.TextTileCount; i++ )
         {
            for ( int j = 0; j < Constants.TextTileSize; j++ )
            {
               byte b = textTextureMap[i + ( j * Constants.TextTileCount )];

               if ( b != mostCommonValue )
               {
                  WriteToFileStream( fs, string.Format( "   screen->textBitFields[{0}][{1}] = 0x{2};\n", i, j, b.ToString( "X2" ) ) );
               }
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private static void WriteToFileStream( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
