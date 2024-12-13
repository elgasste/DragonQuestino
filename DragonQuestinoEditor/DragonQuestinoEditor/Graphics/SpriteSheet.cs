using DragonQuestinoEditor.Utilities;
using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Graphics
{
   internal class SpriteSheet
   {
      private readonly List<List<WriteableBitmap>> _frameBitmaps = new( Constants.SpriteFrameCount );

      public List<List<WriteableBitmap>> FrameBitmaps => _frameBitmaps;

      public SpriteSheet( string imagePath )
      {
         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapUtils.CheckSpriteSheetBitmapFormat( bitmapSource );
         // TODO: make sure this works correctly
         ReadFrameBitmaps( bitmapSource );

         // TODO: update the palette here
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
   }
}
