using System.IO;
using TileMapEditor.Tiles;
using TileMapEditor.ViewModels;

namespace TileMapEditor.FileOps
{
   internal class DataSourceCodeWriter( TileSet tileSet, ICollection<TileViewModel> mapTiles )
   {
      private readonly TileSet _tileSet = tileSet;
      private readonly ICollection<TileViewModel> _tiles = mapTiles;
      private string _fileContents = string.Empty;

      public void WriteFile( string filePath )
      {
         BuildHeaderSection();
         BuildPaletteFunction();
         BuildTileTexturesFunction();
         BuildTileMapFunction();

         File.WriteAllText( filePath, _fileContents );
      }

      private void BuildHeaderSection()
      {
         _fileContents = "#include \"screen.h\"\n";
         _fileContents += "#include \"tile_map.h\"\n";
      }

      private void BuildPaletteFunction()
      {
         _fileContents += "\nvoid Screen_LoadPalette( Screen_t* screen, uint8_t index )\n";
         _fileContents += "{\n";
         _fileContents += "   switch( index )\n";
         _fileContents += "   {\n";
         _fileContents += "      case 0:\n";

         for ( int i = 0; i < Constants.PaletteSize; i++ )
         {
            _fileContents += string.Format( "         screen->palette[{0}] = 0x{1};\n", i, _tileSet.Palette[i].ToString( "X4" ) );
         }

         _fileContents += "         break;\n";
         _fileContents += "   }\n";
         _fileContents += "}\n";
      }

      private void BuildTileTexturesFunction()
      {
         _fileContents += "\nvoid TileMap_LoadTextures( TileMap_t* tileMap )\n";
         _fileContents += "{\n";

         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            var pixelIndexes = _tileSet.TilePaletteIndexes[i];
            int memoryIndex = 0;

            for ( int j = 0; j < Constants.TilePixels; j++ )
            {
               var index1 = pixelIndexes[j];
               j++;
               var index2 = pixelIndexes[j];
               var packed = ( index1 << 4 ) | index2;

               _fileContents += string.Format( "   tileMap->textures[{0}].memory[{1}] = 0x{2};\n", i, memoryIndex, packed.ToString( "X4" ) );

               memoryIndex++;
            }
         }

         _fileContents += "}\n";
      }

      private void BuildTileMapFunction()
      {
         _fileContents += "\nvoid TileMap_Load( TileMap_t* tileMap, Screen_t* screen, uint8_t index )\n";
         _fileContents += "{\n";
         _fileContents += "   switch( index )\n";
         _fileContents += "   {\n";
         _fileContents += "      case 0:\n";
         _fileContents += "         Screen_LoadPalette( screen, 0 );\n";
         _fileContents += string.Format( "         tileMap->tilesX = {0};\n", Constants.TileMapTileCountX );
         _fileContents += string.Format( "         tileMap->tilesY = {0};\n", Constants.TileMapTileCountY );

         int i = 0;

         foreach ( var tile in _tiles )
         {
            _fileContents += string.Format( "         SET_TILETEXTUREINDEX( tileMap->tiles[{0}], {1} );\n", i, tile.Index );
            i++;
         }

         _fileContents += "         break;\n";
         _fileContents += "   }\n";
         _fileContents += "}\n";
      }
   }
}
