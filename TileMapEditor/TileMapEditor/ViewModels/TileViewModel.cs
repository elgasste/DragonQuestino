using DeviceFrameEditor.ViewModels;
using System.Windows.Media.Imaging;

namespace TileMapEditor.ViewModels
{
   internal class TileViewModel( int index, BitmapImage image ) : ViewModelBase
   {
      private int _index = index;
      public int Index
      {
         get => _index;
         private set => SetProperty( ref _index, value );
      }

      private BitmapImage _image = image;
      public BitmapImage Image
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

      public void Copy( TileViewModel vm )
      {
         Index = vm.Index;
         Image = vm.Image;
      }
   }
}
