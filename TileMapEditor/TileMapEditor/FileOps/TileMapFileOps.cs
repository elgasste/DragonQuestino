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

      public static bool LoadTileMap( string filePath, ICollection<TileViewModel> mapTiles )
      {
         var contents = File.ReadAllText( filePath );
         var tileIndexes = JsonSerializer.Deserialize<List<int>>( contents );

         if ( tileIndexes == null || tileIndexes.Count != Constants.TileMapTileCount )
         {
            return false;
         }

         int i = 0;

         foreach ( var tile in mapTiles )
         {
            tile.SetIndex( tileIndexes[i] );
            i++;
         }

         return true;
      }
   }
}
