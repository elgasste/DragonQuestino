using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using TileMapEditor.ViewModels;

namespace TileMapEditor
{
   public partial class MainWindow : Window
   {
      private MainWindowViewModel viewModel;

      public MainWindow()
      {
         InitializeComponent();
         viewModel = new MainWindowViewModel();
         DataContext = viewModel;
      }

      private void TileSelectionListView_OnMouseMove( object sender, MouseEventArgs e )
      {
         base.OnMouseMove( e );

         if ( e.LeftButton == MouseButtonState.Pressed )
         {
            var tileViewData = FindTileViewModelAtPoint( e.GetPosition( this ) );

            if ( tileViewData != null )
            {
               DataObject data = new DataObject();
               data.SetData( "Object", tileViewData );

               DragDrop.DoDragDrop( this, data, DragDropEffects.Copy | DragDropEffects.Move );
            }
         }
      }

      private void TileMapListView_OnDrop( object sender, DragEventArgs e )
      {
         var droppedTileViewData = e.Data.GetData( "Object" ) as TileViewModel;

         if ( droppedTileViewData != null )
         {
            var mapTileViewData = FindTileViewModelAtPoint( e.GetPosition( this ) );

            if ( mapTileViewData != null )
            {
               mapTileViewData.Copy( droppedTileViewData );
            }
         }

         e.Handled = true;
      }

      private TileViewModel? FindTileViewModelAtPoint( Point p )
      {
         var hitResult = VisualTreeHelper.HitTest( this, p );

         if ( hitResult.VisualHit == null )
         {
            return null;
         }

         var visual = VisualTreeHelper.GetParent( hitResult.VisualHit );

         while ( visual != null && visual.GetType().Name != "ListViewItem" )
         {
            visual = VisualTreeHelper.GetParent( visual );
         }

         if ( visual != null )
         {
            ListViewItem? item = visual as ListViewItem;
            return item == null ? null : item.Content as TileViewModel;
         }
         else
         {
            return null;
         }
      }
   }
}