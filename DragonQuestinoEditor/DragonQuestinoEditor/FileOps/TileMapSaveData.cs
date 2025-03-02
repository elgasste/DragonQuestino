using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class TileMapSaveData
   {
      public int Id { get; set; }
      public string? Name { get; set; }
      public bool IsDungeon { get; set; }
      public bool IsDark { get; set; }
      public List<TileSaveData> Tiles { get; set; } = [];
      public int TilesX { get; set; }
      public int TilesY { get; set; }
      public List<TilePortalSaveData> Portals { get; set; } = [];
      public List<StaticSpriteSaveData> StaticSprites { get; set; } = [];

      public TileMapSaveData() { }

      public TileMapSaveData( TileMapViewModel tileMap )
      {
         Id = tileMap.Id;
         Name = tileMap.Name;
         IsDungeon = tileMap.IsDungeon;
         IsDark = tileMap.IsDark;
         TilesX = tileMap.TilesX;
         TilesY = tileMap.TilesY;

         foreach ( var tile in tileMap.Tiles )
         {
            Tiles.Add( new( tile ) );
         }

         foreach ( var portal in tileMap.Portals )
         {
            Portals.Add( new( portal ) );
         }

         foreach ( var staticSprite in tileMap.StaticSprites )
         {
            StaticSprites.Add( new( staticSprite ) );
         }
      }
   }
}
