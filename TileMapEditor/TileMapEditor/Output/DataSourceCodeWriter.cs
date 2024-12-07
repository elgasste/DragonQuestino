using TileMapEditor.Tiles;
using TileMapEditor.ViewModels;

namespace TileMapEditor.Output
{
   internal class DataSourceCodeWriter
   {
      private TileSet _tileSet;
      private ICollection<TileViewModel> _tiles;

      public DataSourceCodeWriter( TileSet tileSet, ICollection<TileViewModel> mapTiles )
      {
         _tileSet = tileSet;
         _tiles = mapTiles;
      }

      public void WriteFile( string filePath )
      {
         // TODO
      }
   }
}
