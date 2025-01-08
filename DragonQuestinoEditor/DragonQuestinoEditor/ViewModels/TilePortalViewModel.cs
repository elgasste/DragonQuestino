using System.Security.RightsManagement;

namespace DragonQuestinoEditor.ViewModels
{
   internal class TilePortalViewModel( int sourceTileIndex, int destinationTileMapIndex, int destinationTileIndex, Direction arrivalDirection ) : ViewModelBase
   {
      private int _sourceTileIndex = sourceTileIndex;
      public int SourceTileIndex
      {
         get => _sourceTileIndex;
         set => SetProperty( ref _sourceTileIndex, value );
      }

      private int _destinationTileMapIndex = destinationTileMapIndex;
      public int DestinationTileMapIndex
      {
         get => _destinationTileMapIndex;
         set => SetProperty( ref _destinationTileMapIndex, value );
      }

      private int _destinationTileIndex = destinationTileIndex;
      public int DestinationTileIndex
      {
         get => _destinationTileIndex;
         set => SetProperty( ref _destinationTileIndex, value );
      }

      private Direction _arrivalDirection = arrivalDirection;
      public Direction ArrivalDirection
      {
         get => _arrivalDirection;
         set => SetProperty( ref _arrivalDirection, value );
      }
   }
}
