using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace TileMapEditor.Utilities
{
   internal static class ColorUtils
   {
      public static UInt16 ColortoUInt16( Color color )
      {
         var r16 = (UInt16)( ( color.R / 255.0 ) * 31 );
         var g16 = (UInt16)( ( color.G / 255.0 ) * 63 );
         var b16 = (UInt16)( ( color.B / 255.0 ) * 31 );

         return (UInt16)( ( r16 << 11 ) | ( g16 << 5 ) | b16 );
      }

      public static Color GetPixelColor( BitmapSource bitmap, int x, int y )
      {
         var bytesPerPixel = (int)Math.Truncate( ( bitmap.Format.BitsPerPixel + 7.0 ) / 8.0 );
         int stride = bitmap.PixelWidth * bytesPerPixel;
         byte[] pixel = new byte[1];
         var rect = new Int32Rect( x, y, 1, 1 );

         bitmap.CopyPixels( rect, pixel, stride, 0 );
         return bitmap.Palette.Colors[pixel[0]];
      }
   }
}
