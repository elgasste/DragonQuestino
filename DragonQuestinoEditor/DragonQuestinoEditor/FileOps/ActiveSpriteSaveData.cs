using DragonQuestinoEditor.ViewModels;
using System.Windows;

namespace DragonQuestinoEditor.FileOps
{
   public class ActiveSpriteSaveData
   {
      public int SpriteSheetIndex { get; set; }
      public Point Offset { get; set; }
      public Point HitBoxSize { get; set; }

      public ActiveSpriteSaveData() { }

      public ActiveSpriteSaveData( ActiveSpriteViewModel sprite )
      {
         SpriteSheetIndex = sprite.SpriteSheetIndex;
         Offset = sprite.Offset;
         HitBoxSize = sprite.HitBoxSize;
      }
   }
}
