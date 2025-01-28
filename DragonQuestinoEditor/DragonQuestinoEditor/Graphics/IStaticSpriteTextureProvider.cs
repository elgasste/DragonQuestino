using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Graphics
{
   public interface IStaticSpriteTextureProvider
   {
      public BitmapSource GetImageFromIndex( int index );
   }
}
