using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class TilePortalSaveData
   {
      public int SourceTileIndex { get; set; }
      public int DestinationTileMapIndex { get; set; }
      public int DestinationTileIndex { get; set; }
      public Direction ArrivalDirection { get; set; }

      public TilePortalSaveData() { }

      public TilePortalSaveData( TilePortalViewModel portal )
      {
         SourceTileIndex = portal.SourceTileIndex;
         DestinationTileMapIndex = portal.DestinationTileMapIndex;
         DestinationTileIndex = portal.DestinationTileIndex;
         ArrivalDirection = portal.ArrivalDirection;
      }
   }
}
