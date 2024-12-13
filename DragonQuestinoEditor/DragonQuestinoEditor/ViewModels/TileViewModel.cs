using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Tiles;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.ViewModels
{
   internal class TileViewModel : ViewModelBase
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

      private bool _shouldHighlight = false;
      public bool ShouldHighlight
      {
         get => _shouldHighlight;
         set => SetProperty( ref _shouldHighlight, value );
      }

      public TileViewModel( TileSet tileSet, int index )
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
