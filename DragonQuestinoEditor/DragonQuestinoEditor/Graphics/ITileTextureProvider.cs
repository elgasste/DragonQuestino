using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Graphics
{
   public interface ITileTextureProvider
   {
      public BitmapSource GetImageFromIndex( int index );
   }
}
