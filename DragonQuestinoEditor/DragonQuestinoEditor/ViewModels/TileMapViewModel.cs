using System.Collections.ObjectModel;

namespace DragonQuestinoEditor.ViewModels
{
   internal class TileMapViewModel( string name ) : ViewModelBase
   {
      public ObservableCollection<TileViewModel> Tiles { get; } = [];

      private string? _name = name;
      public string? Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }
   }
}
