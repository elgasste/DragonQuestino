using DeviceFrameEditor.ViewModels;
using System.Windows.Media.Imaging;

namespace TileMapEditor.ViewModels
{
   internal class TileViewModel( int index, BitmapImage image ) : ViewModelBase
   {
      public int Index { get; private set; } = index;
      public BitmapImage Image { get; private set; } = image;

      public void Copy( TileViewModel vm )
      {
         Index = vm.Index;
         Image = vm.Image;

         OnPropertyChanged( nameof( Index ) );
         OnPropertyChanged( nameof( Image ) );
      }
   }
}
