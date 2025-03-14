﻿using DragonQuestinoEditor.FileOps;
using System.Security.RightsManagement;

namespace DragonQuestinoEditor.ViewModels
{
   public class TilePortalViewModel : ViewModelBase
   {
      private int _sourceTileIndex;
      public int SourceTileIndex
      {
         get => _sourceTileIndex;
         set => SetProperty( ref _sourceTileIndex, value );
      }

      private int _destinationTileMapIndex;
      public int DestinationTileMapIndex
      {
         get => _destinationTileMapIndex;
         set => SetProperty( ref _destinationTileMapIndex, value );
      }

      private int _destinationTileIndex;
      public int DestinationTileIndex
      {
         get => _destinationTileIndex;
         set => SetProperty( ref _destinationTileIndex, value );
      }

      private Direction _arrivalDirection;
      public Direction ArrivalDirection
      {
         get => _arrivalDirection;
         set => SetProperty( ref _arrivalDirection, value );
      }

      public TilePortalViewModel( int sourceTileIndex, int destinationTileMapIndex, int destinationTileIndex, Direction arrivalDirection )
      {
         _sourceTileIndex = sourceTileIndex;
         _destinationTileMapIndex = destinationTileMapIndex;
         _destinationTileIndex = destinationTileIndex;
         _arrivalDirection = arrivalDirection;
      }

      public TilePortalViewModel( TilePortalSaveData saveData )
      {
         _sourceTileIndex = saveData.SourceTileIndex;
         _destinationTileMapIndex= saveData.DestinationTileMapIndex;
         _destinationTileIndex = saveData.DestinationTileIndex;
         _arrivalDirection = saveData.ArrivalDirection;
      }
   }
}
