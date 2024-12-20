using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Graphics
{
   internal interface ITileTextureProvider
   {
      public BitmapSource GetImageFromIndex( int index );
   }
}
