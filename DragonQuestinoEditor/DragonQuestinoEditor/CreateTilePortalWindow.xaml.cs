using DragonQuestinoEditor.ViewModels;
using System.Collections.ObjectModel;
using System.Windows;

namespace DragonQuestinoEditor
{
   public partial class CreateTilePortalWindow : Window
   {
      private readonly CreateTilePortalWindowViewModel _viewModel;

      public int DestinationTileMapIndex { get; private set; }
      public Direction ArrivalDirection { get; private set; }

      public CreateTilePortalWindow( ObservableCollection<TileMapViewModel> tileMaps )
      {
         InitializeComponent();

         _viewModel = new( tileMaps );
         DataContext = _viewModel;
      }

      private void CancelButton_Click( object sender, RoutedEventArgs e )
      {
         DialogResult = false;
         Close();
      }

      private void CreateButton_Click( object sender, RoutedEventArgs e )
      {
         if ( _viewModel.SelectedTileMap is null )
         {
            MessageBox.Show( "You must select a destination tile map." );
            return;
         }

         if ( string.IsNullOrEmpty( ArrivalDirectionComboBox.Text ) )
         {
            MessageBox.Show( "You must select an arrival direction." );
            return;
         }

         DestinationTileMapIndex = _viewModel.SelectedTileMap.Id;
         ArrivalDirection = Enum.Parse<Direction>( ArrivalDirectionComboBox.Text );
         DialogResult = true;
         Close();
      }
   }
}
