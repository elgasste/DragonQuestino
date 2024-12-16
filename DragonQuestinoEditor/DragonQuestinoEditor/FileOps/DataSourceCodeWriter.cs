using System.Collections.ObjectModel;
using System.IO;
using DragonQuestinoEditor.Graphics;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   internal class DataSourceCodeWriter( Palette palette,
                                        TileSet tileSet,
                                        ObservableCollection<TileViewModel> mapTiles,
                                        SpriteSheet spriteSheet )
   {
      private readonly Palette _palette = palette;
      private readonly TileSet _tileSet = tileSet;
      private readonly ObservableCollection<TileViewModel> _tiles = mapTiles;
      private readonly SpriteSheet _spriteSheet = spriteSheet;
      private string _fileContents = string.Empty;

      public void WriteFile( string filePath )
      {
         BuildHeaderSection();
         BuildPaletteFunction();
         BuildTileTexturesFunction();
         BuildTileMapFunction();
         BuildSpriteFunction();

         File.WriteAllText( filePath, _fileContents );
      }

      private void BuildHeaderSection()
      {
         _fileContents = "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n";
         _fileContents += "#include \"screen.h\"\n";
         _fileContents += "#include \"tile_map.h\"\n";
         _fileContents += "#include \"sprite.h\"\n";
      }

      private void BuildPaletteFunction()
      {
         _fileContents += "\nvoid Screen_LoadPalette( Screen_t* screen )\n";
         _fileContents += "{\n";
         _fileContents += "   uint16_t i;\n\n";
         _fileContents += string.Format( "   for ( i = 0; i < {0}; i++ ) {{ screen->palette[i] = 0; }}\n\n", Constants.PaletteSize );

         for ( int i = 0; i < _palette.ColorCount; i++ )
         {
            _fileContents += string.Format( "   screen->palette[{0}] = 0x{1};\n", i, _palette.Colors[i].ToString( "X4" ) );
         }

         _fileContents += "}\n";
      }

      private void BuildTileTexturesFunction()
      {
         _fileContents += "\nvoid TileMap_LoadTextures( TileMap_t* tileMap )\n";
         _fileContents += "{\n";
         _fileContents += "   uint32_t* mem32;\n\n";

         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            _fileContents += string.Format( "   mem32 = (uint32_t*)( tileMap->textures[{0}].memory );\n", i );

            var pixelIndexes = _tileSet.TilePaletteIndexes[i];

            for ( int j = 0, memoryIndex = 0; j < Constants.TilePixels; j += 4, memoryIndex++ )
            {
               var index0 = (UInt32)( pixelIndexes[j + 0] );
               var index1 = (UInt32)( pixelIndexes[j + 1] );
               var index2 = (UInt32)( pixelIndexes[j + 2] );
               var index3 = (UInt32)( pixelIndexes[j + 3] );

               var packed = ( index3 << 24 ) | ( index2 << 16  ) | ( index1 << 8  ) | ( index0 << 0  );

               _fileContents += string.Format( "   mem32[{0}] = 0x{1};\n", memoryIndex, packed.ToString( "X8" ) );
            }
         }

         _fileContents += "}\n";
      }

      private void BuildTileMapFunction()
      {
         _fileContents += "\nvoid TileMap_Load( TileMap_t* tileMap, uint32_t index )\n";
         _fileContents += "{\n";
         _fileContents += "   int32_t i;\n";
         _fileContents += "   uint32_t* tiles32 = (uint32_t*)( tileMap->tiles );\n\n";
         _fileContents += "   switch( index )\n";
         _fileContents += "   {\n";
         _fileContents += "      case 0:\n";
         _fileContents += string.Format( "         tileMap->tilesX = {0};\n", Constants.TileMapTileCountX );
         _fileContents += string.Format( "         tileMap->tilesY = {0};\n", Constants.TileMapTileCountY );

         var indexCounts = new Dictionary<UInt32, int>();

         for ( int i = 0; i < _tiles.Count; i += 2 )
         {
            var index0 = (UInt32)_tiles[i].Index;
            var index1 = (UInt32)_tiles[i + 1].Index;
            var packed = ( index1 << 16 ) | index0;

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

         _fileContents += string.Format( "         for ( i = 0; i < ( TILE_COUNT / 2 ); i++ ) {{ tiles32[i] = 0x{0}; }}\n", mostCommonValue.ToString( "X8" ) );

         for ( int i = 0, tileIndex = 0; i < _tiles.Count; i += 2, tileIndex++ )
         {
            var index0 = (UInt32)_tiles[i].Index;
            var index1 = (UInt32)_tiles[i + 1].Index;
            var packed = ( index1 << 16 ) | index0;

            if ( packed != mostCommonValue )
            {
               _fileContents += string.Format( "         tiles32[{0}] = 0x{1};\n", tileIndex, packed.ToString( "X8" ) );
            }
         }

         _fileContents += "         break;\n";
         _fileContents += "   }\n";
         _fileContents += "}\n";
      }

      private void BuildSpriteFunction()
      {
         _fileContents += "\nvoid Sprite_Load( Sprite_t* sprite, uint32_t index )\n";
         _fileContents += "{\n";
         _fileContents += "   uint32_t* mem32;\n\n";
         _fileContents += "   switch( index )\n";
         _fileContents += "   {\n";
         _fileContents += "      case 0:\n";

         for ( int i = 0; i < Constants.SpritePositionCount; i++ )
         {
            // MUFFINS: I think we should compress these, probably
            for ( int j = 0; j < Constants.SpriteFrameCount; j++ )
            {
               _fileContents += string.Format( "         mem32 = (uint32_t*)( sprite->textures[{0}].memory );\n", ( i * Constants.SpriteFrameCount ) + j );

               var pixelIndexes = _spriteSheet.FramePaletteIndexes[i][j];

               for ( int k = 0, memoryIndex = 0; k < Constants.SpriteFramePixels; k += 4, memoryIndex++ )
               {
                  var index0 = (UInt32)( pixelIndexes[k + 0] );
                  var index1 = (UInt32)( pixelIndexes[k + 1] );
                  var index2 = (UInt32)( pixelIndexes[k + 2] );
                  var index3 = (UInt32)( pixelIndexes[k + 3] );

                  var packed = ( index3 << 24 ) | ( index2 << 16 ) | ( index1 << 8 ) | ( index0 << 0 );

                  _fileContents += string.Format( "         mem32[{0}] = 0x{1};\n", memoryIndex, packed.ToString( "X8" ) );
               }
            }
         }

         _fileContents += "         break;\n";
         _fileContents += "   }\n";
         _fileContents += "}\n";
      }
   }
}
