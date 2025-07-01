using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class NonPlayerCharacterSaveData
   {
      public int Id { get; set; }
      public int TileIndex { get; set; }
      public int ActiveSpriteSheetIndex { get; set; }

      public NonPlayerCharacterSaveData() { }

      public NonPlayerCharacterSaveData( NonPlayerCharacterViewModel viewModel )
      {
         Id = viewModel.Id;
         TileIndex = viewModel.TileIndex;
         ActiveSpriteSheetIndex = viewModel.ActiveSpriteSheetIndex;
      }
   }
}
