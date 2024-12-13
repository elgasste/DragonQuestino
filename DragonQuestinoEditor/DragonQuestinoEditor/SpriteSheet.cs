using DragonQuestinoEditor.Utilities;
using System.IO;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor
{
   internal class SpriteSheet
   {
      public SpriteSheet( string imagePath )
      {
         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapUtils.CheckBitmapFormat( bitmapSource );

         // TODO: check the image size, and read all the frames
      }
   }
}
