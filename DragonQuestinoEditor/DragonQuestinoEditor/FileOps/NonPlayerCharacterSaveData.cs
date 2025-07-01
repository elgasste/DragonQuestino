using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class NonPlayerCharacterSaveData
   {
      public int Id { get; set; }
      public int TileIndex { get; set; }
      public ActiveSpriteSaveData? ActiveSprite { get; set; }
      public Direction Direction { get; set; }

      public NonPlayerCharacterSaveData() { }

      public NonPlayerCharacterSaveData( NonPlayerCharacterViewModel viewModel )
      {
         Id = viewModel.Id;
         TileIndex = viewModel.TileIndex;
         Direction = viewModel.Direction;

         if ( viewModel.ActiveSprite != null )
         {
            ActiveSprite = new( viewModel.ActiveSprite );
         }
      }
   }
}
