using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class ActiveSpriteSaveData
   {
      public int SpriteSheetIndex { get; set; }
      public int TileIndex { get; set; }
      public Direction Direction { get; set; }

      public ActiveSpriteSaveData() { }

      public ActiveSpriteSaveData( ActiveSpriteViewModel sprite )
      {
         SpriteSheetIndex = sprite.SpriteSheetIndex;
         TileIndex = sprite.TileIndex;
         Direction = sprite.Direction;
      }
   }
}
