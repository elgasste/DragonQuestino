using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.ViewModels
{
   public class TileViewModel : ViewModelBase
   {
      private ITileTextureProvider? _textureProvider = null;

      // TODO: allow updating this with the tile editor
      private readonly int[] _passableTextureIndexes = [
         0,    // grass
         1,    // trees
         2,    // hills
         3,    // desert
         4,    // swamp
         7,    // brick path
         9,    // barrier
         13    // bridge
      ];

      public TileSet TileSet { get; }

      private int _textureIndex = -1;
      public int TextureIndex
      {
         get => _textureIndex;
         set
         {
            if ( SetProperty( ref _textureIndex, value ) )
            {
               Image = TileSet.TileBitmaps[value];
               IsPassable = _passableTextureIndexes.Contains( value );
            }
         }
      }

      public BitmapSource? Image => _textureProvider?.GetImageFromIndex( TextureIndex );

      private bool _isPassable;
      public bool IsPassable
      {
         get => _isPassable;
         private set => SetProperty( ref _isPassable, value );
      }

      private bool _isDraggingTextureOver = false;
      public bool IsDraggingTextureOver
      {
         get => _isDraggingTextureOver;
         set => SetProperty( ref _isDraggingTextureOver, value );
      }

      private bool _isSelected = false;
      public bool IsSelected
      {
         TileSet = tileSet;
         Index = index;

         _isPassable = _passableIndexes.Contains( index );
      }

      private bool _isPortalDestination = false;
      public bool IsPortalDestination
      {
         get => _isPortalDestination;
         set => SetProperty( ref _isPortalDestination, value );
      }

      private TilePortalViewModel? _portal = null;
      public TilePortalViewModel? Portal
      {
         get => _portal;
         set
         {
            SetProperty( ref _portal, value );
            OnPropertyChanged( nameof( HasPortal ) );
         }
      }

      public bool HasPortal => _portal is not null;

      public TileViewModel( int textureIndex )
      {
         TextureIndex = textureIndex;
         _isPassable = _passableTextureIndexes.Contains( textureIndex );
      }

      public TileViewModel( TileSaveData saveData )
      {
         SetTextureIndex( saveData.TextureIndex );
         _isPassable = saveData.IsPassable;
      }

      public void SetTextureProvider( ITileTextureProvider? provider ) => _textureProvider = provider;
   }
}
