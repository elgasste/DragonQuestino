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

      public ObservableCollection<TileViewModel> TileSelectionViewModels { get; } = [];
      public ObservableCollection<TileViewModel> TileMapViewModels { get; } = [];

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

            TileSelectionViewModels.Add( new( i, image ) );
         }

         for ( int i = 0; i < Constants.TileMapTileCount; i++ )
         {
            TileMapViewModels.Add( new( 0, TileSelectionViewModels[0].Image ) );
         }
      }
   }
}
