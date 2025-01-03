using System.Collections.ObjectModel;
using System.IO;
using System.Text;
using DragonQuestinoEditor.Graphics;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   internal class DataSourceCodeWriter( Palette palette,
                                        TileSet tileSet,
                                        ObservableCollection<TileMapViewModel> tileMaps,
                                        SpriteSheet spriteSheet )
   {
      private readonly Palette _palette = palette;
      private readonly TileSet _tileSet = tileSet;
      private readonly ObservableCollection<TileMapViewModel> _tileMaps = tileMaps;
      private readonly SpriteSheet _spriteSheet = spriteSheet;

      public void WriteFile( string filePath )
      {
         using FileStream fs = File.Create( filePath );
         WriteHeaderSection( fs );
         WritePaletteFunction( fs );
         WriteTileTexturesFunction( fs );
         WriteTileMapFunction( fs );
         WriteSpriteFunctions( fs );
      }

      private static void WriteHeaderSection( FileStream fs )
      {
         WriteText( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteText( fs, "#include \"screen.h\"\n" );
         WriteText( fs, "#include \"tile_map.h\"\n" );
         WriteText( fs, "#include \"sprite.h\"\n" );
      }

      private void WritePaletteFunction( FileStream fs )
      {
         WriteText( fs, "\nvoid Screen_LoadPalette( Screen_t* screen )\n" );
         WriteText( fs, "{\n" );
         WriteText( fs, "   uint16_t i;\n\n" );
         WriteText( fs, string.Format( "   for ( i = 0; i < {0}; i++ ) {{ screen->palette[i] = 0; }}\n\n", Constants.PaletteSize ) );

         for ( int i = 0; i < _palette.ColorCount; i++ )
         {
            WriteText( fs, string.Format( "   screen->palette[{0}] = 0x{1};\n", i, _palette.Colors[i].ToString( "X4" ) ) );
         }

         WriteText( fs, "}\n" );
      }

      private void WriteTileTexturesFunction( FileStream fs )
      {
         WriteText( fs, "\nvoid TileMap_LoadTextures( TileMap_t* tileMap )\n" );
         WriteText( fs, "{\n" );
         WriteText( fs, "   uint32_t* mem32;\n\n" );

         // TODO: try compressing this, it only ever gets called once
         for ( int i = 0; i < Constants.TileTextureCount; i++ )
         {
            WriteText( fs, string.Format( "   mem32 = (uint32_t*)( tileMap->textures[{0}].memory );\n", i ) );

            var pixelIndexes = _tileSet.TilePaletteIndexes[i];

            for ( int j = 0, memoryIndex = 0; j < Constants.TilePixels; j += 4, memoryIndex++ )
            {
               var index0 = (UInt32)( pixelIndexes[j + 0] );
               var index1 = (UInt32)( pixelIndexes[j + 1] );
               var index2 = (UInt32)( pixelIndexes[j + 2] );
               var index3 = (UInt32)( pixelIndexes[j + 3] );

               var packed = ( index3 << 24 ) | ( index2 << 16  ) | ( index1 << 8  ) | ( index0 << 0  );

               WriteText( fs, string.Format( "   mem32[{0}] = 0x{1};\n", memoryIndex, packed.ToString( "X8" ) ) );
            }
         }

         WriteText( fs, "}\n" );
      }

      private void WriteTileMapFunction( FileStream fs )
      {
         WriteText( fs, "\nvoid TileMap_Load( TileMap_t* tileMap, uint32_t index )\n" );
         WriteText( fs, "{\n" );
         WriteText( fs, "   int32_t i;\n" );
         WriteText( fs, "   uint32_t* tiles32 = (uint32_t*)( tileMap->tiles );\n\n" );
         WriteText( fs, "   switch( index )\n" );
         WriteText( fs, "   {\n" );

         for ( int i = 0; i < _tileMaps.Count; i++ )
         {
            var tiles = _tileMaps[i].Tiles;

            WriteText( fs, string.Format( "      case {0}:\n", _tileMaps[i].Id ) );
            WriteText( fs, string.Format( "         tileMap->tilesX = {0};\n", _tileMaps[i].TilesX ) );
            WriteText( fs, string.Format( "         tileMap->tilesY = {0};\n", _tileMaps[i].TilesY ) );
            WriteText( fs, "         for ( i = 0; i < TILEMAP_MAX_PORTALS; i++ ) {{ tileMap->portals[i].sourceTileIndex = -1; }}\n" );
            WriteText( fs, string.Format( "         tileMap->spriteCount = 0;\n" ) );

            var packedTiles = new List<UInt32>( tiles.Count / 2 );
            var indexCounts = new Dictionary<UInt32, int>();

            for ( int j = 0; j < tiles.Count; j += 2 )
            {
               var index0 = (UInt32)( tiles[j].Index )
                  | ( tiles[j].IsPassable ? (UInt32)0x20 : 0 )
                  | Constants.TileSetIndexWalkSpeeds[tiles[j].Index]
                  | 0x100  // is encounterable
                  | Constants.TileSetIndexEncounterRates[tiles[j].Index]
                  | Constants.TileSetIndexDamageRates[tiles[j].Index];
               var index1 = (UInt32)( tiles[j + 1].Index )
                  | ( tiles[j + 1].IsPassable ? (UInt32)0x20 : 0 )
                  | Constants.TileSetIndexWalkSpeeds[tiles[j + 1].Index]
                  | 0x100  // is encounterable
                  | Constants.TileSetIndexEncounterRates[tiles[j + 1].Index]
                  | Constants.TileSetIndexDamageRates[tiles[j + 1].Index];

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

            WriteText( fs, string.Format( "         for ( i = 0; i < ( TILE_COUNT / 2 ); i++ ) {{ tiles32[i] = 0x{0}; }}\n", mostCommonValue.ToString( "X8" ) ) );

            for ( int j = 0, packedTileIndex = 0; j < tiles.Count; j += 2, packedTileIndex++ )
            {
               if ( packedTiles[packedTileIndex] != mostCommonValue )
               {
                  WriteText( fs, string.Format( "         tiles32[{0}] = 0x{1};\n", packedTileIndex, packedTiles[packedTileIndex].ToString( "X8" ) ) );
               }
            }

            WriteText( fs, "         break;\n" );
         }
         WriteText( fs, "   }\n" );
         WriteText( fs, "}\n" );
      }

      private void WriteSpriteFunctions( FileStream fs )
      {
         WriteText( fs, "\nvoid Sprite_LoadPlayer( Sprite_t* sprite )\n" );
         WriteText( fs, "{\n" );
         WriteText( fs, "   int32_t i;\n" );
         WriteText( fs, "   uint32_t* mem32 = (uint32_t*)( sprite->textures[0].memory );\n\n" );

         var indexCounts = new Dictionary<UInt32, int>();
         var packedIndexes = new List<UInt32>();

         for ( int i = 0; i < Constants.SpritePositionCount; i++ )
         {
            for ( int j = 0; j < Constants.SpriteFrameCount; j++ )
            {
               var pixelIndexes = _spriteSheet.FramePaletteIndexes[i][j];

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

         WriteText( fs, string.Format( "   for ( i = 0; i < ( SPRITE_TEXTURE_BYTES / 4 ) * SPRITE_TEXTURES; i++ ) {{ mem32[i] = 0x{0}; }}\n", mostCommonValue.ToString( "X8" ) ) );

         for ( int i = 0, packedIndex = 0; i < Constants.SpritePositionCount; i++ )
         {
            for ( int j = 0; j < Constants.SpriteFrameCount; j++ )
            {
               WriteText( fs, string.Format( "   mem32 = (uint32_t*)( sprite->textures[{0}].memory );\n", ( i * Constants.SpriteFrameCount ) + j ) );

               var pixelIndexes = _spriteSheet.FramePaletteIndexes[i][j];

               for ( int k = 0, memoryIndex = 0; k < Constants.SpriteFramePixels; k += 4, memoryIndex++, packedIndex++ )
               {
                  if ( packedIndexes[packedIndex] != mostCommonValue )
                  {
                     WriteText( fs, string.Format( "   mem32[{0}] = 0x{1};\n", memoryIndex, packedIndexes[packedIndex].ToString( "X8" ) ) );
                  }
               }
            }
         }

         WriteText( fs, "}\n\n" );

         // TODO
         WriteText( fs, "\nvoid Sprite_LoadGeneric( Sprite_t* sprite, uint32_t index )\n" );
         WriteText( fs, "{\n" );
         WriteText( fs, "   UNUSED_PARAM( sprite );\n" );
         WriteText( fs, "   UNUSED_PARAM( index );\n" );
         WriteText( fs, "}\n" );
      }

      private static void WriteText( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
