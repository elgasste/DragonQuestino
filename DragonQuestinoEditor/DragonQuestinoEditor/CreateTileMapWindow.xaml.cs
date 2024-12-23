using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

namespace DragonQuestinoEditor
{
   public partial class CreateTileMapWindow : Window
   {
      private readonly IEnumerable<string?> _existingTileMapNames;

      public string NewTileMapName { get; private set; } = string.Empty;
      public int NewTilesX { get; private set; } = 0;
      public int NewTilesY { get; private set; } = 0;

      public CreateTileMapWindow( IEnumerable<string?> existingTileMapNames )
      {
         _existingTileMapNames = existingTileMapNames;
         InitializeComponent();
      }

      [GeneratedRegex( "[^0-9]+" )]
      private static partial Regex NonNumericRegX();

      private void TileCount_PreviewTextInput( object sender, TextCompositionEventArgs e )
      {
         var regex = NonNumericRegX();
         e.Handled = regex.IsMatch( e.Text );
      }

      private void CancelButton_Click( object sender, RoutedEventArgs e )
      {
         DialogResult = false;
         Close();
      }

      private void CreateButton_Click( object sender, RoutedEventArgs e )
      {
         if ( string.IsNullOrEmpty( TileMapName.Text ) )
         {
            MessageBox.Show( "The tile map must have a name." );
            return;
         }
         else if ( _existingTileMapNames.Contains( TileMapName.Text ) )
         {
            MessageBox.Show( "A tile map with this name already exists." );
            return;
         }
         else if ( !int.TryParse( HorizontalTilesTextBox.Text, out int tilesX ) )
         {
            MessageBox.Show( "Invalid value entered for horizontal tiles." );
            return;
         }
         else if ( tilesX < Constants.TileMapMinTilesX || tilesX > Constants.TileMapMaxTilesX )
         {
            MessageBox.Show( string.Format( "Horizontal tiles must be between {0} and {1}.", Constants.TileMapMinTilesX, Constants.TileMapMaxTilesX ) );
            return;
         }
         else if ( !int.TryParse( VerticalTilesTextBox.Text, out int tilesY ) )
         {
            MessageBox.Show( "Invalid value entered for vertical tiles." );
            return;
         }
         else if ( tilesY < Constants.TileMapMinTilesY || tilesY > Constants.TileMapMaxTilesY )
         {
            MessageBox.Show( string.Format( "Vertical tiles must be between {0} and {1}.", Constants.TileMapMinTilesY, Constants.TileMapMaxTilesY ) );
            return;
         }

         NewTileMapName = TileMapName.Text;
         NewTilesX = int.Parse( HorizontalTilesTextBox.Text );
         NewTilesY = int.Parse( VerticalTilesTextBox.Text );
         DialogResult = true;
         Close();
      }
   }
}
