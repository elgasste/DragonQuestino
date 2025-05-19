using System.Collections.ObjectModel;
using System.IO;
using System.Text.Json;
using DragonQuestinoEditor.Graphics;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public static class SaveDataFileOps
   {
      public static void SaveData( string filePath, IEnumerable<TileMapViewModel> tileMaps, IEnumerable<EnemyViewModel> enemies )
      {
         SaveData saveData = new( tileMaps, enemies );
         File.WriteAllText( filePath, JsonSerializer.Serialize( saveData ) );
      }

      public static bool LoadData( string filePath,
                                   TileSet tileSet,
                                   Palette palette,
                                   ObservableCollection<TileMapViewModel> tileMaps,
                                   ObservableCollection<EnemyViewModel> enemies )
      {
         var contents = File.ReadAllText( filePath );
         var saveData = JsonSerializer.Deserialize<SaveData>( contents );

         if ( saveData is null )
         {
            return false;
         }

         foreach ( var tileMapSaveData in saveData.TileMaps )
         {
            tileMaps.Add( new( tileSet, tileMapSaveData ) );
         }

         foreach ( var enemySaveData in saveData.Enemies )
         {
            enemies.Add( new( palette, enemySaveData ) );
         }

         return true;
      }
   }
}
