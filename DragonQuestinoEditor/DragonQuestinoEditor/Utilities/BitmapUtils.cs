using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Utilities
{
   public static class BitmapUtils
   {
      public static Color GetBitmapPixelColor( BitmapSource bitmap, int x, int y )
      {
         var bytesPerPixel = (int)Math.Truncate( ( bitmap.Format.BitsPerPixel + 7.0 ) / 8.0 );
         int stride = bitmap.PixelWidth * bytesPerPixel;
         byte[] pixel = new byte[1];
         var rect = new Int32Rect( x, y, 1, 1 );

         bitmap.CopyPixels( rect, pixel, stride, 0 );
         return bitmap.Palette.Colors[pixel[0]];
      }

      public static void CheckTileSetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Tileset image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.TileSize * Constants.TileTextureCount ) )
         {
            throw new Exception( string.Format( "Tileset image width should be {0}", Constants.TileSize * Constants.TileTextureCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.TileSize )
         {
            throw new Exception( string.Format( "Tileset image height should be {0}", Constants.TileSize ) );
         }
      }

      public static void CheckActiveSpriteSheetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Sprite image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.SpriteFrameSize * Constants.ActiveSpriteFrameCount ) )
         {
            throw new Exception( string.Format( "Active sprite image width should be {0}", Constants.SpriteFrameSize * Constants.ActiveSpriteFrameCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.SpriteFrameSize * Constants.ActiveSpritePositionCount )
         {
            throw new Exception( string.Format( "Active sprite image height should be {0}", Constants.SpriteFrameSize * Constants.ActiveSpritePositionCount ) );
         }
      }

      public static void CheckStaticSpriteSheetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Sprite image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth % Constants.SpriteFrameSize != 0 )
         {
            throw new Exception( string.Format( "Static sprite image width should be a multiple of {0}", Constants.SpriteFrameSize ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.SpriteFrameSize )
         {
            throw new Exception( string.Format( "Sprite image height should be {0}", Constants.SpriteFrameSize ) );
         }
      }

      public static void CheckBackgroundBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Bgr24 )
         {
            throw new Exception( "Background image pixel format should be Bgr24" );
         }
         else if ( bitmapSource.PixelWidth != Constants.GigaShieldWidth || bitmapSource.PixelHeight != Constants.GigaShieldHeight )
         {
            throw new Exception( string.Format( "Background image dimensions should be {0}x{1}", Constants.GigaShieldWidth, Constants.GigaShieldHeight ) );
         }
      }

      public static void CheckTextTileSetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Text tileset image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.TextTileCount * Constants.TextTileSize ) )
         {
            throw new Exception( string.Format( "Text tileset image width should be {0}", Constants.TextTileCount * Constants.TextTileSize ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.TextTileSize )
         {
            throw new Exception( string.Format( "Text tileset image height should be {0}", Constants.TextTileSize ) );
         }
      }
   }
}
