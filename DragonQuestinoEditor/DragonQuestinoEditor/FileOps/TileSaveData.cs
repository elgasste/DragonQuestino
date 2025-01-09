using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class TileSaveData
   {
      public int TextureIndex { get; set; }
      public bool IsPassable { get; set; }

      public TileSaveData() { }

      public TileSaveData( TileViewModel tile )
      {
         TextureIndex = tile.TextureIndex;
         IsPassable = tile.IsPassable;
      }
   }
}
