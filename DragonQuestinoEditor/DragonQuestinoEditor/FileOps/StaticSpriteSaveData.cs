using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class StaticSpriteSaveData
   {
      public int TextureIndex { get; set; }
      public int TileIndex { get; set; }

      public StaticSpriteSaveData() { }

      public StaticSpriteSaveData( StaticSpriteViewModel sprite )
      {
         TextureIndex = sprite.TextureIndex;
         TileIndex = sprite.TileIndex;
      }
   }
}
