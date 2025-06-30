using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;

namespace DragonQuestinoEditor.ViewModels
{
   public class ActiveSpriteViewModel : ViewModelBase
   {
      private int _spriteSheetIndex;
      public int SpriteSheetIndex
      {
         get => _spriteSheetIndex;
         set => SetProperty( ref _spriteSheetIndex, value );
      }

      private int _tileIndex;
      public int TileIndex
      {
         get => _tileIndex;
         set => SetProperty( ref _tileIndex, value );
      }

      private Direction _direction;
      public Direction Direction
      {
         get => _direction;
         set => SetProperty( ref _direction, value );
      }

      public ActiveSpriteViewModel( int spriteSheetIndex, int tileIndex, Direction direction )
      {
         _spriteSheetIndex = spriteSheetIndex;
         _tileIndex = tileIndex;
         _direction = direction;
      }

      public ActiveSpriteViewModel( ActiveSpriteSaveData saveData )
      {
         SpriteSheetIndex = saveData.SpriteSheetIndex;
         TileIndex = saveData.TileIndex;
         Direction = saveData.Direction;
      }
   }
}
