using System.Windows.Media.Imaging;

namespace TileMapEditor.Tiles
{
   internal class TileViewData( int index, BitmapImage image )
   {
      public int Index { get; } = index;
      public BitmapImage Image { get; } = image;
   }
}
