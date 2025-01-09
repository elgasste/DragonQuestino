using System.Collections.ObjectModel;
using System.IO;
using System.Text;
using DragonQuestinoEditor.Graphics;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class DataSourceCodeWriter( Palette palette,
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

         foreach ( var tileMap in _tileMaps )
         {
            var tiles = tileMap.Tiles;

            WriteText( fs, string.Format( "      case {0}:\n", tileMap.Id ) );
            WriteText( fs, string.Format( "         tileMap->tilesX = {0};\n", tileMap.TilesX ) );
            WriteText( fs, string.Format( "         tileMap->tilesY = {0};\n", tileMap.TilesY ) );
            WriteText( fs, string.Format( "         tileMap->portalCount = {0};\n", tileMap.Portals.Count ) );

            if ( tileMap.Portals.Count > 0 )
            {
               WriteText( fs, string.Format( "         for ( i = 0; i < {0}; i++ )\n", tileMap.Portals.Count ) );
               WriteText( fs, "         {\n" );

               foreach ( var portal in tileMap.Portals )
               {
                  WriteText( fs, string.Format( "            tileMap->portals[i].sourceTileIndex = {0};\n", portal.SourceTileIndex ) );
                  WriteText( fs, string.Format( "            tileMap->portals[i].destinationTileMapIndex = {0};\n", portal.DestinationTileMapIndex ) );
                  WriteText( fs, string.Format( "            tileMap->portals[i].destinationTileIndex = {0};\n", portal.DestinationTileIndex ) );
                  WriteText( fs, string.Format( "            tileMap->portals[i].arrivalDirection = (Direction_t){0};\n", (int)( portal.ArrivalDirection ) ) );
               }

               WriteText( fs, "         }\n" );
            }

            WriteText( fs, "         tileMap->spriteCount = 0;\n" );

            var packedTiles = new List<UInt32>( ( tileMap.TilesX * tileMap.TilesY ) / 2 );
            var indexCounts = new Dictionary<UInt32, int>();

            for ( int row = 0; row < tileMap.TilesY; row++ )
            {
               for ( int col = 0; col < tileMap.TilesX; col += 2 )
               {
                  var tileIndex = ( row * tileMap.TilesX ) + col;

                  var index0 = (UInt32)( tiles[tileIndex].TextureIndex )
                     | ( tiles[tileIndex].IsPassable ? (UInt32)0x20 : 0 )
                     | Constants.TileSetIndexWalkSpeeds[tiles[tileIndex].TextureIndex]
                     | 0x100  // is encounterable
                     | Constants.TileSetIndexEncounterRates[tiles[tileIndex].TextureIndex]
                     | Constants.TileSetIndexDamageRates[tiles[tileIndex].TextureIndex];
                  var index1 = (UInt32)( tiles[tileIndex + 1].TextureIndex )
                     | ( tiles[tileIndex + 1].IsPassable ? (UInt32)0x20 : 0 )
                     | Constants.TileSetIndexWalkSpeeds[tiles[tileIndex + 1].TextureIndex]
                     | 0x100  // is encounterable
                     | Constants.TileSetIndexEncounterRates[tiles[tileIndex + 1].TextureIndex]
                     | Constants.TileSetIndexDamageRates[tiles[tileIndex + 1].TextureIndex];

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

            // TODO: this can be optimized, we should only fill in the area of TilesX and TilesY
            WriteText( fs, string.Format( "         for ( i = 0; i < ( TILE_COUNT / 2 ); i++ ) {{ tiles32[i] = 0x{0}; }}\n", mostCommonValue.ToString( "X8" ) ) );

            for ( int row = 0, packedTileIndex = 0; row < tileMap.TilesY; row++ )
            {
               for ( int col = 0; col < tileMap.TilesX; packedTileIndex++, col += 2 )
               {
                  var tileIndex = ( row * Constants.TileMapMaxTilesX ) + col;

                  if ( packedTiles[packedTileIndex] != mostCommonValue )
                  {
                     WriteText( fs, string.Format( "         tiles32[{0}] = 0x{1};\n", tileIndex / 2, packedTiles[packedTileIndex].ToString( "X8" ) ) );
                  }
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
