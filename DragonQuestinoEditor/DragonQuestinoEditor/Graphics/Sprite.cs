using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Graphics
{
   internal class Sprite
   {
      private readonly Color[,] _pixels;

      public int Width { get; }
      public int Height { get; }

      public Sprite( int width, int height )
      {
         Width = width;
         Height = height;

         _pixels = new Color[width, height];
      }

      public static Sprite LoadFromFile( string path )
      {
         using var fileStream = new FileStream( path, FileMode.Open, FileAccess.Read );
         var decoder = new PngBitmapDecoder( fileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapFrame = decoder.Frames[0];

         int stride = bitmapFrame.PixelWidth * bitmapFrame.Format.BitsPerPixel / 8;

         var data = new byte[stride * bitmapFrame.PixelHeight];
         bitmapFrame.CopyPixels( data, stride, 0 );

         var sprite = new Sprite( bitmapFrame.PixelWidth, bitmapFrame.PixelHeight );
         int sourceOffset = 0;

         for (int y = 0; y < bitmapFrame.PixelHeight; y++)
         {
            for (int x = 0; x < bitmapFrame.PixelWidth; x++)
            {
               int pixelIndex = data[sourceOffset++];
               var color = bitmapFrame.Palette.Colors[pixelIndex];
               sprite.SetPixel( x, y, color );
            }
         }

         return sprite;
      }

      public void SetPixel( int x, int y, Color color )
      {
         _pixels[x, y] = color;
      }

      public Color GetPixel( int x, int y )
      {
         return _pixels[x, y];
      }

      public Sprite Extract( int x, int y, int width, int height )
      {
         var newSprite = new Sprite( width, height );

         for (int row = 0; row < height; row++)
         {
            for (int col = 0; col < width; col++)
            {
               var srcPixel = GetPixel( col + x, row + y );
               newSprite.SetPixel( col, row, srcPixel );
            }
         }

         return newSprite;
      }

      public void DrawToBuffer( byte[] buffer, int bufferWidth, int x, int y )
      {
         for (int row = 0; row < Height; row++)
         {
            for (int col = 0; col < Width; col++)
            {
               var pixel = GetPixel( col, row );

               int destX = x + col * 4;
               int destY = y + row;
               int offset = destY * bufferWidth + destX;

               buffer[offset + 0] = pixel.B;
               buffer[offset + 1] = pixel.G;
               buffer[offset + 2] = pixel.R;
               buffer[offset + 3] = pixel.A;
            }
         }
      }

      private byte[] ConvertToLinearBuffer()
      {
         var linearPixelBuffer = new byte[_pixels.Length * 4];
         int offset = 0;

         for (int y = 0; y < Height; y++)
         {
            for (int x = 0; x < Width; x++)
            {
               linearPixelBuffer[offset++] = _pixels[x, y].B;
               linearPixelBuffer[offset++] = _pixels[x, y].G;
               linearPixelBuffer[offset++] = _pixels[x, y].R;
               linearPixelBuffer[offset++] = _pixels[x, y].A;
            }
         }

         return linearPixelBuffer;
      }

      public void SaveAsPng( string path )
      {
         var linearPixelBuffer = ConvertToLinearBuffer();

         var writableBitmap = new WriteableBitmap( Width, Height, 96, 96, PixelFormats.Bgra32, null );
         writableBitmap.WritePixels( new Int32Rect( 0, 0, Width, Height ), linearPixelBuffer, Width * 4, 0 );

         using var fileStream = new FileStream( path, FileMode.Create, FileAccess.Write );

         var encoder = new PngBitmapEncoder();
         encoder.Frames.Add( BitmapFrame.Create( writableBitmap ) );
         encoder.Save( fileStream );
      }
   }
}
