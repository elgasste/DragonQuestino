using System.IO;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace TileMapEditor.Tiles
{
   internal class TileSet
   {
      private readonly BitmapSource _tileSetBitmap;

      public TileSet( string imagePath )
      {
         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         _tileSetBitmap = textDecoder.Frames[0];
         BitmapSanityCheck( _tileSetBitmap );
      }

      private static void BitmapSanityCheck( BitmapSource bitmap )
      {
         if ( bitmap.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Expecting tileset image pixel format to be Indexed8" );
         }
         else if ( bitmap.PixelWidth != ( Constants.TileSize * Constants.TileCount ) )
         {
            throw new Exception( string.Format( "Tileset image width should be {0}", Constants.TileSize * Constants.TileCount ) );
         }
         else if ( bitmap.PixelHeight != Constants.TileSize )
         {
            throw new Exception( string.Format( "Tileset image height should be {0}", Constants.TileSize ) );
         }
      }
   }
}
