using DragonQuestinoEditor.FileOps;
using System.Windows;

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

      private Point _offset;
      public Point Offset
      {
         get => _offset;
         set => SetProperty( ref _offset, value );
      }

      private Point _hitBoxSize;
      public Point HitBoxSize
      {
         get => _hitBoxSize;
         set => SetProperty( ref _hitBoxSize, value );
      }

      public ActiveSpriteViewModel( int spriteSheetIndex, int tileIndex, Direction direction, Point offset, Point hitBoxSize )
      {
         _spriteSheetIndex = spriteSheetIndex;
         _tileIndex = tileIndex;
         _direction = direction;
         _offset = offset;
         _hitBoxSize = hitBoxSize;
      }

      public ActiveSpriteViewModel( ActiveSpriteSaveData saveData )
      {
         SpriteSheetIndex = saveData.SpriteSheetIndex;
         TileIndex = saveData.TileIndex;
         Direction = saveData.Direction;
         Offset = saveData.Offset;
         HitBoxSize = saveData.HitBoxSize;
      }
   }
}
