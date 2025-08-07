using DragonQuestinoPasswordGenerator.ViewModels;
using System.Windows;

namespace DragonQuestinoPasswordGenerator
{
   public partial class MainWindow : Window
   {
      public MainWindow()
      {
         InitializeComponent();

         DataContext = new MainWindowViewModel();
      }
   }
}