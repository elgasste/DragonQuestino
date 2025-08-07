using DragonQuestinoPasswordGenerator.ViewModels;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace DragonQuestinoPasswordGenerator
{
   public partial class MainWindow : Window
   {
      public MainWindow()
      {
         InitializeComponent();

         DataContext = new MainWindowViewModel();
      }

      private void TextBox_PreviewTextInput_NumericOnly( object sender, TextCompositionEventArgs e )
      {
         Regex regex = new Regex( "^\\d+$" );
         e.Handled = !regex.IsMatch( e.Text );
      }

      private void TextBox_PreviewKeyDown_NoSpaces( object sender, KeyEventArgs e )
      {
         e.Handled = e.Key == Key.Space;
      }

      private void TextBox_TextChanged_MaxUint16( object sender, TextChangedEventArgs e )
      {
         if ( sender is TextBox textBox )
         {
            if ( int.TryParse( textBox.Text, out int value ) )
            {
               if ( value > UInt16.MaxValue )
               {
                  textBox.Text = UInt16.MaxValue.ToString(); // Set to max if exceeded
               }
            }
         }
      }

      private void TextBox_TextChanged_Max7( object sender, TextChangedEventArgs e )
      {
         if ( sender is TextBox textBox )
         {
            if ( int.TryParse( textBox.Text, out int value ) )
            {
               if ( value > 7 )
               {
                  textBox.Text = "7"; // Set to max if exceeded
               }
            }
         }
      }

      private void CopyPasswordButton_Click( object sender, RoutedEventArgs e )
      {
         Clipboard.SetText( PasswordTextBox.Text );
      }
   }
}