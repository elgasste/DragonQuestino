using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class BoothSaveData
   {
      public int Id { get; set; }
      public int TileIndex { get; set; }

      public BoothSaveData() { }

      public BoothSaveData( BoothViewModel viewModel )
      {
         Id = viewModel.Id;
         TileIndex = viewModel.TileIndex;
      }
   }
}
