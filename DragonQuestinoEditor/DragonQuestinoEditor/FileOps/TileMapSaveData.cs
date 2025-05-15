using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class TileMapSaveData
   {
      public int Id { get; set; }
      public string? Name { get; set; }
      public bool HasEncounters { get; set; }
      public bool BlocksMagic { get; set; }
      public bool IsDungeon { get; set; }
      public bool IsDark { get; set; }
      public List<TileSaveData> Tiles { get; set; } = [];
      public int TilesX { get; set; }
      public int TilesY { get; set; }
      public List<TilePortalSaveData> Portals { get; set; } = [];
      public TilePortalSaveData EvacPortal { get; set; } = new();
      public List<StaticSpriteSaveData> StaticSprites { get; set; } = [];

      public TileMapSaveData() { }

      public TileMapSaveData( TileMapViewModel tileMap )
      {
         Id = tileMap.Id;
         Name = tileMap.Name;
         HasEncounters = tileMap.HasEncounters;
         BlocksMagic = tileMap.BlocksMagic;
         IsDungeon = tileMap.IsDungeon;
         IsDark = tileMap.IsDark;
         TilesX = tileMap.TilesX;
         TilesY = tileMap.TilesY;
         EvacPortal = new( tileMap.EvacPortal );

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

         // NOTE: this is how I originally initialized the enemy pool indexes for the Overworld,
         // I think we should keep it around in case we need it again.

         //if ( Name == "Overworld" )
         //{
         //   List<int> poolIndexes = [
         //      3 , 3 , 2 , 2 , 3 , 5 , 4,  5,
         //      3 , 2 , 1 , 2 , 3 , 3 , 4,  5,
         //      4 , 1 , 0 , 0 , 2 , 3 , 4,  5,
         //      5 , 1 , 1 , 12, 6 , 6 , 6,  6,
         //      5 , 5 , 4 , 12, 7 , 7 , 7,  7,
         //      10, 9 , 8 , 12, 12, 12, 8,  7,
         //      10, 10, 11, 12, 13, 13, 9,  8,
         //      11, 11, 12, 13, 13, 12, 9,  9
         //   ];

         //   for ( int regionIndex = 0; regionIndex < 64; regionIndex++ )
         //   {
         //      int regionRow = regionIndex / 8;
         //      int regionCol = regionIndex % 8;
         //      int startTileIndex = 990 + ( 140 * regionRow * 15 ) + ( regionCol * 15 );

         //      for ( int i = 0; i < 225; i++ )
         //      {
         //         int tileRow = i / 15;
         //         int tileCol = i % 15;
         //         int tileIndex = startTileIndex + ( 140 * tileRow ) + tileCol;
         //         Tiles[tileIndex].EnemyPoolIndex = poolIndexes[regionIndex];
         //      }
         //   }
         //}
      }
   }
}
