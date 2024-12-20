using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   internal class TileMapSaveData
   {
      public string? Name { get; set; }
      public List<TileSaveData> Tiles { get; set; } = [];
      public int TilesX { get; set; }
      public int TilesY { get; set; }

      public TileMapSaveData() { }

      public TileMapSaveData( TileMapViewModel tileMap )
      {
         Name = tileMap.Name;
         TilesX = tileMap.TilesX;
         TilesY = tileMap.TilesY;

         foreach ( var tile in tileMap.Tiles )
         {
            Tiles.Add( new( tile ) );
         }
      }
   }
}
