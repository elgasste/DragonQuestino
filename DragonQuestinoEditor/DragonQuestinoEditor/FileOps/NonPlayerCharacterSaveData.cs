using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class NonPlayerCharacterSaveData
   {
      public int Id { get; set; }
      public int TileIndex { get; set; }
      public ActiveSpriteSaveData? ActiveSprite { get; set; }
      public Direction Direction { get; set; }
      public bool Wanders { get; set; }
      public SimpleRect? WanderBounds { get; set; }

      public NonPlayerCharacterSaveData() { }

      public NonPlayerCharacterSaveData( NonPlayerCharacterViewModel viewModel )
      {
         Id = viewModel.Id;
         TileIndex = viewModel.TileIndex;
         Direction = viewModel.Direction;
         Wanders = viewModel.Wanders;
         WanderBounds = viewModel.WanderBounds;

         if ( viewModel.ActiveSprite != null )
         {
            ActiveSprite = new( viewModel.ActiveSprite );
         }
      }
   }
}
