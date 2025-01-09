using System.Collections.ObjectModel;

namespace DragonQuestinoEditor.ViewModels
{
   public class CreateTilePortalWindowViewModel : ViewModelBase
   {
      public ObservableCollection<TileMapViewModel> TileMaps { get; }

      public TileMapViewModel? SelectedTileMap { get; set; }

      public CreateTilePortalWindowViewModel( ObservableCollection<TileMapViewModel> tileMaps )
      {
         TileMaps = tileMaps;

         if ( tileMaps is not null )
         {
            SelectedTileMap = tileMaps.FirstOrDefault();
         }
      }
   }
}
