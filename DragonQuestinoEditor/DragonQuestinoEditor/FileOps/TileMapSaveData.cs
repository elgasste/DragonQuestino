using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   internal class TileMapSaveData
   {
      public string? Name { get; set; }
      public List<TileSaveData> Tiles { get; set; } = [];

      public TileMapSaveData() { }

      public TileMapSaveData( TileMapViewModel tileMap )
      {
         Name = tileMap.Name;

         foreach ( var tile in tileMap.Tiles )
         {
            Tiles.Add( new( tile) );
         }
      }
   }
}
