using DeviceFrameEditor.ViewModels;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Media.Imaging;
using TileMapEditor.Tiles;

namespace TileMapEditor.ViewModels
{
   internal class MainWindowViewModel : ViewModelBase
   {
      private readonly TileSet _tileSet;

      public ObservableCollection<TileViewData> TileViewDatas { get; } = [];

      public MainWindowViewModel()
      {
         _tileSet = new TileSet( "..\\..\\..\\..\\..\\Assets\\tile_textures.png" );

         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            var image = new BitmapImage();

            using var stream = new MemoryStream();
            {
               var encoder = new PngBitmapEncoder();
               encoder.Frames.Add( BitmapFrame.Create( _tileSet.TileBitmaps[i] ) );
               encoder.Save( stream );
               image.BeginInit();
               image.CacheOption = BitmapCacheOption.OnLoad;
               image.StreamSource = stream;
               image.EndInit();
               image.Freeze();
            }

            TileViewDatas.Add( new( i, image ) );
         }
      }
   }
}
