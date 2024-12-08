using System.IO;
using System.Text.Json;
using TileMapEditor.ViewModels;

namespace TileMapEditor.FileOps
{
   internal static class TileMapFileOps
   {
      public static void SaveTileMap( string filePath, ICollection<TileViewModel> mapTiles )
      {
         var tileIndexes = new List<int>();

         foreach ( var tile in mapTiles )
         {
            tileIndexes.Add( tile.Index );
         }

         File.WriteAllText( filePath, JsonSerializer.Serialize( tileIndexes ) );
      }
   }
}
