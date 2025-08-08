using System.Windows;
using System.Windows.Input;

namespace DragonQuestinoPasswordGenerator
{
   public partial class EnterPasswordWindow : Window
   {
      public string Password { get; set; } = string.Empty;

      public EnterPasswordWindow()
      {
         InitializeComponent();
      }

      private void Window_Loaded( object sender, RoutedEventArgs e )
      {
         PasswordTextBox.Focus();
      }

      private void OkButton_Click( object sender, RoutedEventArgs e )
      {
         CloseWithResult();
      }

      private void Window_KeyDown( object sender, KeyEventArgs e )
      {
         if ( e.Key == Key.Enter )
         {
            CloseWithResult();
         }
      }

      private void CloseWithResult()
      {
         Password = PasswordTextBox.Text;
         DialogResult = true;
         Close();
      }
   }
}
