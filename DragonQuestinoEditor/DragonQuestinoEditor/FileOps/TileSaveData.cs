using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   internal class TileSaveData
   {
      public int Index { get; set; }
      public bool IsPassable { get; set; }

      public TileSaveData() { }

      public TileSaveData( TileViewModel tile )
      {
         Index = tile.Index;
         IsPassable = tile.IsPassable;
      }
   }
}
