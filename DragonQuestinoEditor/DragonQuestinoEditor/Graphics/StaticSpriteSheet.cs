using DragonQuestinoEditor.Utilities;
using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Graphics
{
   public class StaticSpriteSheet
   {
      private readonly Palette _palette;
      private readonly List<WriteableBitmap> _bitmaps = [];

      public List<WriteableBitmap> Bitmaps => _bitmaps;
      public List<List<int>> PaletteIndexes = [];

      public StaticSpriteSheet( string imagePath, Palette palette )
      {
         _palette = palette;

         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapUtils.CheckStaticSpriteSheetBitmapFormat( bitmapSource );
         ReadBitmaps( bitmapSource );
         UpdatePalette();
      }

      private void ReadBitmaps( BitmapSource bitmapSource )
      {
         int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
         var data = new byte[stride * bitmapSource.PixelHeight];
         bitmapSource.CopyPixels( data, stride, 0 );
         int spriteCount = bitmapSource.PixelWidth % Constants.SpriteFrameSize;

         for ( int i = 0; i < spriteCount; i++ )
         {
            _bitmaps.Add( new WriteableBitmap( Constants.SpriteFrameSize, Constants.SpriteFrameSize,
                                               bitmapSource.DpiX, bitmapSource.DpiY,
                                               bitmapSource.Format, bitmapSource.Palette ) );
            int offset = i * Constants.SpriteFrameSize;
            _bitmaps[i].WritePixels( new Int32Rect( 0, 0, Constants.SpriteFrameSize, Constants.SpriteFrameSize ), data, stride, offset );
         }
      }

      private void UpdatePalette()
      {
         for ( int i = 0; i < _bitmaps.Count; i++ )
         {
            PaletteIndexes.Add( new( Constants.SpriteFramePixels ) );

            for ( int j = 0; j < Constants.SpriteFramePixels; j++ )
            {
               PaletteIndexes[i].Add( 0 );
            }
         }

         for ( int i = 0; i < _bitmaps.Count; i++ )
         {
            var bitmap = _bitmaps[i];

            for ( int y = 0; y < bitmap.PixelHeight; y++ )
            {
               for ( int x = 0; x < bitmap.PixelWidth; x++ )
               {
                  var pixelColor = ColorUtils.GetPixelColor( bitmap, x, y );
                  var pixelColor16 = ColorUtils.ColortoUInt16( pixelColor );
                  _palette.AddColor( pixelColor16 );
                  PaletteIndexes[i][( y * Constants.SpriteFrameSize ) + x] = _palette.GetIndexForColor( pixelColor16 );
               }
            }
         }
      }
   }
}
