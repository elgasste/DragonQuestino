using DeviceFrameEditor.ViewModels;
using TileMapEditor.Tiles;

namespace TileMapEditor.ViewModels
{
   internal class MainWindowViewModel : ViewModelBase
   {
      private TileSet _tileSet;

      public MainWindowViewModel()
      {
         _tileSet = new TileSet( "..\\..\\..\\..\\..\\Assets\\tile_textures.png" );
      }
   }
}
