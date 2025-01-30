using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Utilities
{
   public static class ColorUtils
   {
      public static UInt16 ColorToUInt16( Color color )
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

      public static UInt16 BytesToRgb565( byte r, byte g, byte b )
      {
         var ri = (float)r / 0xFF;
         var gi = (float)g / 0xFF;
         var bi = (float)b / 0xFF;

         var r5 = (UInt16)( 0x1F * ri );
         var g6 = (UInt16)( 0x3F * gi );
         var b5 = (UInt16)( 0x1F * bi );

         return (UInt16)( ( r5 << 11 ) | ( g6 << 5 ) | b5 );
      }
   }
}
