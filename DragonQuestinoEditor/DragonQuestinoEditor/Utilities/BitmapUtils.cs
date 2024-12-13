using System.Windows.Media;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor;

namespace DragonQuestinoEditor.Utilities
{
   internal static class BitmapUtils
   {
      public static void CheckBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Expecting tileset image pixel format to be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.TileSize * Constants.TileCount ) )
         {
            throw new Exception( string.Format( "Tileset image width should be {0}", Constants.TileSize * Constants.TileCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.TileSize )
         {
            throw new Exception( string.Format( "Tileset image height should be {0}", Constants.TileSize ) );
         }
      }
   }
}
