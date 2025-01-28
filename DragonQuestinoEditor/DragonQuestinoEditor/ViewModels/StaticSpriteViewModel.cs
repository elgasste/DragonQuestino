using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.ViewModels
{
   public class StaticSpriteViewModel : ViewModelBase
   {
      private IStaticSpriteTextureProvider? _textureProvider = null;

      private int _textureIndex;
      public int TextureIndex
      {
         get => _textureIndex;
         private set => SetProperty( ref _textureIndex, value );
      }

      private int _tileIndex;
      public int TileIndex
      {
         get => _tileIndex;
         private set => SetProperty( ref _tileIndex, value );
      }

      public BitmapSource? Image => _textureProvider?.GetImageFromIndex( TextureIndex );

      public StaticSpriteViewModel( int index )
      {
         SetTextureIndex( index );
      }

      public StaticSpriteViewModel( StaticSpriteSaveData saveData )
      {
         SetTextureIndex( saveData.TextureIndex );
         _tileIndex = saveData.TileIndex;
      }

      public void SetTextureProvider( IStaticSpriteTextureProvider? provider ) => _textureProvider = provider;

      public void SetTextureIndex( int index )
      {
         TextureIndex = index;
         OnPropertyChanged( nameof( Image ) );
      }
   }
}
