using System.Windows;

namespace DragonQuestinoPasswordGenerator
{
   public partial class EnterPasswordWindow : Window
   {
      public string Password { get; set; } = string.Empty;

      public EnterPasswordWindow()
      {
         InitializeComponent();
      }

      private void OkButton_Click( object sender, RoutedEventArgs e )
      {
         Password = PasswordTextBox.Text;
         Close();
      }
   }
}
