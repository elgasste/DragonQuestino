using System.Windows;
using TileMapEditor.ViewModels;

namespace TileMapEditor
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