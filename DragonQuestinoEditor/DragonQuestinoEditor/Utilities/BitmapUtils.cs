using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Utilities
{
   public static class BitmapUtils
   {
      public static ushort ColortoUInt16( Color color )
      {
         var r16 = (ushort)( ( color.R / 255.0 ) * 31 );
         var g16 = (ushort)( ( color.G / 255.0 ) * 63 );
         var b16 = (ushort)( ( color.B / 255.0 ) * 31 );

         return (ushort)( ( r16 << 11 ) | ( g16 << 5 ) | b16 );
      }

      public static Color GetBitmapPixelColor( BitmapSource bitmap, int x, int y )
      {
         var bytesPerPixel = (int)Math.Truncate( ( bitmap.Format.BitsPerPixel + 7.0 ) / 8.0 );
         int stride = bitmap.PixelWidth * bytesPerPixel;
         byte[] pixel = new byte[1];
         var rect = new Int32Rect( x, y, 1, 1 );

         bitmap.CopyPixels( rect, pixel, stride, 0 );
         return bitmap.Palette.Colors[pixel[0]];
      }

      public static ushort GetPixelColor16( BitmapSource bitmap, int x, int y ) => ColortoUInt16( GetBitmapPixelColor( bitmap, x, y ) );

      public static void CheckMapTileSetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Tileset image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.MapTileSize * Constants.MapTileTextureCount ) )
         {
            throw new Exception( string.Format( "Tileset image width should be {0}", Constants.MapTileSize * Constants.MapTileTextureCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.MapTileSize )
         {
            throw new Exception( string.Format( "Tileset image height should be {0}", Constants.MapTileSize ) );
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

      public static void CheckEnemyTileSetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Enemy tileset image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.EnemyTileStride * Constants.EnemyTileSize ) )
         {
            throw new Exception( string.Format( "Enemy tileset image width should be {0}", Constants.EnemyTileStride * Constants.EnemyTileSize ) );
         }
         else if ( bitmapSource.PixelHeight != ( Constants.EnemyTileCount / Constants.EnemyTileStride ) * Constants.EnemyTileSize )
         {
            throw new Exception( string.Format( "Enemy tileset image height should be {0}", ( Constants.EnemyTileCount / Constants.EnemyTileStride ) * Constants.EnemyTileSize ) );
         }
      }
   }
}
