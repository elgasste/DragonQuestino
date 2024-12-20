using DragonQuestinoEditor.FileOps;
using System.Collections.ObjectModel;

namespace DragonQuestinoEditor.ViewModels
{
   internal class TileMapViewModel : ViewModelBase
   {
      public ObservableCollection<TileViewModel> Tiles { get; } = [];

      private int _tilesX;
      public int TilesX
      {
         get => _tilesX;
         set => SetProperty( ref _tilesX, value );
      }

      private int _tilesY;
      public int TilesY
      {
         get => _tilesY;
         set => SetProperty( ref _tilesY, value );
      }

      private string? _name;
      public string? Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      public TileMapViewModel( string? name )
      {
         _name = name;
      }

      public TileMapViewModel( TileMapSaveData saveData )
      {
         Name = saveData.Name;
         TilesX = saveData.TilesX;
         TilesY = saveData.TilesY;

         foreach ( var tileSaveData in saveData.Tiles )
         {
            Tiles.Add( new( tileSaveData ) );
         }
      }
   }
}
