using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.ViewModels
{
   internal class TileViewModel : ViewModelBase
   {
      private ITileTextureProvider? _tileTextureProvider = null;

      // TODO: allow updating this with the tile editor
      private readonly int[] _passableIndexes = [
         0,    // grass
         1,    // trees
         2,    // hills
         3,    // desert
         4,    // swamp
         7,    // brick path
         9,    // barrier
         13    // bridge
      ];

      private int _index;
      public int Index
      {
         get => _index;
         private set => SetProperty( ref _index, value );
      }

      public BitmapSource? Image => _tileTextureProvider?.GetImageFromIndex( Index );

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

      private bool _isSelectedForPortal = false;
      public bool IsSelectedForPortal
      {
         get => _isSelectedForPortal;
         set => SetProperty( ref _isSelectedForPortal, value );
      }

      public TileViewModel( int index )
      {
         SetIndex( index );
         _isPassable = _passableIndexes.Contains( index );
      }

      public TileViewModel( TileSaveData saveData )
      {
         SetIndex( saveData.Index );
         _isPassable = saveData.IsPassable;
      }

      public void SetTileTextureProvider( ITileTextureProvider? provider ) => _tileTextureProvider = provider;

      public void SetIndex( int index )
      {
         Index = index;
         IsPassable = _passableIndexes.Contains( index );
         OnPropertyChanged( nameof( Image ) );
      }
   }
}
