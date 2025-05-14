using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class TileSaveData
   {
      public int Id { get; set; }
      public int TextureIndex { get; set; }
      public int TreasureFlag { get; set; }
      public int DoorFlag { get; set; }
      public bool IsPassable { get; set; }
      public int EnemyPoolIndex { get; set; }

      public TileSaveData() { }

      public TileSaveData( TileViewModel tile )
      {
         Id = tile.Id;
         TextureIndex = tile.TextureIndex;
         TreasureFlag = tile.TreasureFlag;
         DoorFlag = tile.DoorFlag;
         IsPassable = tile.IsPassable;
         EnemyPoolIndex = tile.EnemyPoolIndex;
      }
   }
}
