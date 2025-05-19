using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class SaveData
   {
      public List<TileMapSaveData> TileMaps { get; set; } = [];
      public List<EnemySaveData> Enemies { get; set; } = [];

      public SaveData() { }

      public SaveData( IEnumerable<TileMapViewModel> tileMaps, IEnumerable<EnemyViewModel> enemies )
      {
         foreach ( var tileMap in tileMaps )
         {
            TileMaps.Add( new( tileMap ) );
         }

         foreach ( var enemy in enemies )
         {
            Enemies.Add( new( enemy ) );
         }
      }
   }
}
