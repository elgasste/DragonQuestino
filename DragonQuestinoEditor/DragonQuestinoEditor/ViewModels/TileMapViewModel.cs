using DragonQuestinoEditor.FileOps;
using System.Collections.ObjectModel;

namespace DragonQuestinoEditor.ViewModels
{
   internal class TileMapViewModel : ViewModelBase
   {
      public ObservableCollection<TileViewModel> Tiles { get; } = [];

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

         foreach ( var tileSaveData in saveData.Tiles )
         {
            Tiles.Add( new( tileSaveData ) );
         }
      }
   }
}
