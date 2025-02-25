using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class TileSaveData
   {
      public int TextureIndex { get; set; }
      public int TreasureFlag { get; set; }
      public int DoorFlag { get; set; }
      public bool IsPassable { get; set; }

      public TileSaveData() { }

      public TileSaveData( TileViewModel tile )
      {
         TextureIndex = tile.TextureIndex;
         TreasureFlag = tile.TreasureFlag;
         DoorFlag = tile.DoorFlag;
         IsPassable = tile.IsPassable;
      }
   }
}
