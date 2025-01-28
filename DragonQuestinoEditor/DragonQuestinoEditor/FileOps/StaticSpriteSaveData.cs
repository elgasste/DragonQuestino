using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class StaticSpriteSaveData
   {
      public int TextureIndex { get; set; }
      public bool IsPassable { get; set; }

      public StaticSpriteSaveData() { }

      public StaticSpriteSaveData( StaticSpriteViewModel sprite )
      {
         TextureIndex = sprite.TextureIndex;
         IsPassable = sprite.IsPassable;
      }
   }
}
