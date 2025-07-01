using DragonQuestinoEditor.FileOps;

namespace DragonQuestinoEditor.ViewModels
{
   public class NonPlayerCharacterViewModel : ViewModelBase
   {
      private int _id;
      public int Id
      {
         get => _id;
         set => SetProperty( ref _id, value );
      }

      private int _tileIndex;
      public int TileIndex
      {
         get => _tileIndex;
         set => SetProperty( ref _tileIndex, value );
      }

      private int _activeSpriteSheetIndex;
      public int ActiveSpriteSheetIndex
      {
         get => _activeSpriteSheetIndex;
         set => SetProperty( ref _activeSpriteSheetIndex, value );
      }

      public NonPlayerCharacterViewModel( int id, int tileIndex, int activeSpriteIndex )
      {
         Id = id;
         TileIndex = tileIndex;
         ActiveSpriteSheetIndex = activeSpriteIndex;
      }

      public NonPlayerCharacterViewModel( NonPlayerCharacterSaveData saveData )
      {
         Id = saveData.Id;
         TileIndex = saveData.TileIndex;
         ActiveSpriteSheetIndex= saveData.ActiveSpriteSheetIndex;
      }
   }
}
