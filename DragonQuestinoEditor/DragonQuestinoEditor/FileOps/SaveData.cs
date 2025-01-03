using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   internal class SaveData
   {
      public List<TileMapSaveData> TileMaps { get; set; } = [];

      public SaveData() { }

      public SaveData( IEnumerable<TileMapViewModel> tileMaps )
      {
         foreach ( var tileMap in tileMaps )
         {
            TileMaps.Add( new( tileMap ) );
         }
      }
   }
}
