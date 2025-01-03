using System.Collections.ObjectModel;
using System.IO;
using System.Text.Json;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   internal static class SaveDataFileOps
   {
      public static void SaveData( string filePath, IEnumerable<TileMapViewModel> tileMaps )
      {
         SaveData saveData = new( tileMaps );
         File.WriteAllText( filePath, JsonSerializer.Serialize( saveData ) );
      }

      public static bool LoadData( string filePath, ObservableCollection<TileMapViewModel> tileMaps )
      {
         var contents = File.ReadAllText( filePath );
         var saveData = JsonSerializer.Deserialize<SaveData>( contents );

         if ( saveData == null )
         {
            return false;
         }

         foreach ( var tileMapSaveData in saveData.TileMaps )
         {
            tileMaps.Add( new( tileMapSaveData ) );
         }

         return true;
      }
   }
}
