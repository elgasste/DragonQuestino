using DragonQuestinoEditor.FileOps;

namespace DragonQuestinoEditor.ViewModels
{
   public class BoothViewModel : ViewModelBase
   {
      private int _id;
      public int Id
      {
         get => _id;
         set => SetProperty( ref _id, value );
      }

      private int _tileIndex;
      public int TileIndex
      {
         get => _tileIndex;
         set => SetProperty( ref _tileIndex, value );
      }

      public BoothViewModel( int id, int tileIndex )
      {
         _id = id;
         _tileIndex = tileIndex;
      }

      public BoothViewModel( BoothSaveData saveData )
      {
         _id = saveData.Id;
         _tileIndex = saveData.TileIndex;
      }
   }
}
