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

      public ActiveSpriteViewModel( int spriteSheetIndex, Point offset, Point hitBoxSize )
      {
         _spriteSheetIndex = spriteSheetIndex;
         _offset = offset;
         _hitBoxSize = hitBoxSize;
      }

      public ActiveSpriteViewModel( ActiveSpriteSaveData saveData )
      {
         SpriteSheetIndex = saveData.SpriteSheetIndex;
         Offset = saveData.Offset;
         HitBoxSize = saveData.HitBoxSize;
      }
   }
}
