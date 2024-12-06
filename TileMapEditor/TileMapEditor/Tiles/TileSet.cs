using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace TileMapEditor.Tiles
{
   internal class TileSet
   {
      private readonly List<WriteableBitmap> _tileBitmaps = new( Constants.TileCount );

      public List<WriteableBitmap> TileBitmaps => _tileBitmaps;

      public TileSet( string imagePath )
      {
         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapSanityCheck( bitmapSource );

         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
            var data = new byte[stride * bitmapSource.PixelHeight];
            bitmapSource.CopyPixels( data, stride, 0 );
            _tileBitmaps.Add( new WriteableBitmap( Constants.TileSize, bitmapSource.PixelHeight, bitmapSource.DpiX, bitmapSource.DpiY, bitmapSource.Format, bitmapSource.Palette ) );
            _tileBitmaps[i].WritePixels( new Int32Rect( 0, 0, Constants.TileSize, Constants.TileSize ), data, stride, Constants.TileSize * i );
         }
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
