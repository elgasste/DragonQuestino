using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Graphics;

namespace DragonQuestinoEditor.ViewModels
{
   internal class TileViewModel : ViewModelBase
   {
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

      public TileSet TileSet { get; }

      private int _index = -1;
      public int Index
      {
         get => _index;
         set
         {
            if (SetProperty( ref _index, value ))
            {
               Image = TileSet.TileBitmaps[value];
               IsPassable = _passableIndexes.Contains( value );
            }
         }
      }

      private BitmapSource? _image;
      public BitmapSource? Image
      {
         get => _image;
         private set => SetProperty( ref _image, value );
      }

      private bool _isPassable;
      public bool IsPassable
      {
         get => _isPassable;
         private set => SetProperty( ref _isPassable, value );
      }

      private bool _shouldHighlight = false;
      public bool ShouldHighlight
      {
         get => _shouldHighlight;
         set => SetProperty( ref _shouldHighlight, value );
      }

      public TileViewModel( TileSet tileSet, int index )
      {
         TileSet = tileSet;
         Index = index;

         _isPassable = _passableIndexes.Contains( index );
      }
   }
}
