using DragonQuestinoEditor.Utilities;
using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Graphics
{
   internal class SpriteSheet
   {
      private readonly Palette _palette;
      private readonly List<List<WriteableBitmap>> _frameBitmaps = new( Constants.SpriteFrameCount );

      public List<List<WriteableBitmap>> FrameBitmaps => _frameBitmaps;
      public List<List<List<int>>> FramePaletteIndexes = new( Constants.SpritePositionCount );

      public SpriteSheet( string imagePath, Palette palette )
      {
         _palette = palette;

         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapUtils.CheckSpriteSheetBitmapFormat( bitmapSource );
         // MUFFINS: make sure both of these worked correctly
         ReadFrameBitmaps( bitmapSource );
         UpdatePalette();
      }

      private void ReadFrameBitmaps( BitmapSource bitmapSource )
      {
         for ( int i = 0; i < Constants.SpritePositionCount; i++ )
         {
            _frameBitmaps.Add( [] );

            for ( int j = 0; j < Constants.SpriteFrameCount; j++ )
            {
               int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
               var data = new byte[stride * bitmapSource.PixelHeight];
               bitmapSource.CopyPixels( data, stride, 0 );
               _frameBitmaps[i].Add( new WriteableBitmap( Constants.SpriteFrameSize, Constants.SpriteFrameSize,
                                                          bitmapSource.DpiX, bitmapSource.DpiY,
                                                          bitmapSource.Format, bitmapSource.Palette ) );
               int offset = ( Constants.SpriteFrameCount * Constants.SpriteFrameSize * i ) + j;
               _frameBitmaps[i][j].WritePixels( new Int32Rect( 0, 0, Constants.SpriteFrameSize, Constants.SpriteFrameSize ),
                                                data, stride, offset );
            }
         }
      }

      private void UpdatePalette()
      {
         for ( int i = 0; i < Constants.SpritePositionCount; i++ )
         {
            for ( int j = 0; j < Constants.SpriteFrameCount; j++ )
            {
               FramePaletteIndexes[i].Add( new( Constants.SpriteFramePixels ) );

               for ( int k = 0; k < Constants.SpriteFramePixels; k++ )
               {
                  FramePaletteIndexes[i][j].Add( 0 );
               }
            }

            for ( int j = 0; j < Constants.TileCount; j++ )
            {
               var frameBitmap = _frameBitmaps[i][j];

               for ( int y = 0; y < frameBitmap.PixelHeight; y++ )
               {
                  for ( int x = 0; x < frameBitmap.PixelWidth; x++ )
                  {
                     var pixelColor = ColorUtils.GetPixelColor( frameBitmap, x, y );
                     var pixelColor16 = ColorUtils.ColortoUInt16( pixelColor );
                     _palette.AddColor( pixelColor16 );
                     FramePaletteIndexes[i][j][( y * Constants.TileSize ) + x] = _palette.GetIndexForColor( pixelColor16 );
                  }
               }
            }
         }
      }
   }
}
