using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Graphics;

namespace DragonQuestinoEditor.ViewModels
{
   public class TileTextureViewModel : ViewModelBase
   {
      private TileSet _tileSet;

      private int _index;
      public int Index
      {
         get => _index;
         private set => SetProperty( ref _index, value );
      }

      private BitmapSource? _image;
      public BitmapSource? Image
      {
         get => _image;
         private set => SetProperty( ref _image, value );
      }

      public TileTextureViewModel( TileSet tileSet, int index )
      {
         _tileSet = tileSet;
         SetIndex( index );
      }

      public void SetIndex( int index )
      {
         Index = index;
         Image = _tileSet.TileBitmaps[index];
      }
   }
}
