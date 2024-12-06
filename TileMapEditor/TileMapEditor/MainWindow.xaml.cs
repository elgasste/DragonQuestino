using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using TileMapEditor.ViewModels;

namespace TileMapEditor
{
   public partial class MainWindow : Window
   {
      private readonly MainWindowViewModel _viewModel;

      public MainWindow()
      {
         InitializeComponent();
         _viewModel = new MainWindowViewModel();
         DataContext = _viewModel;
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

      private void TileMapListViewImage_OnDragEnter( object sender, DragEventArgs e )
      {
         var tileVm = FindTileViewModelAtPoint( e.GetPosition( this ) );

         if ( tileVm != null )
         {
            tileVm.ShouldHighlight = true;
         }
      }

      private void TileMapListViewImage_OnDragLeave( object sender, DragEventArgs e )
      {
         var tileVm = FindTileViewModelFromObject( sender );

         if ( tileVm != null )
         {
            tileVm.ShouldHighlight = false;
         }
      }

      private void TileMapListView_OnDrop( object sender, DragEventArgs e )
      {
         if ( e.Data.GetData( "Object" ) is TileViewModel droppedTileVM )
         {
            var mapTileVM = FindTileViewModelAtPoint( e.GetPosition( this ) );

            if ( mapTileVM != null )
            {
               mapTileVM.Copy( droppedTileVM );
               mapTileVM.ShouldHighlight = false;
            }
         }
      }

      private TileViewModel? FindTileViewModelFromObject( object obj )
      {
         if ( obj != null )
         {
            if ( obj is TileViewModel )
            {
               return obj as TileViewModel;
            }
            else if ( obj is FrameworkElement )
            {
               var element = obj as FrameworkElement;
               return FindTileViewModelInTree( element );
            }
         }

         return null;
      }

      private TileViewModel? FindTileViewModelAtPoint( Point p )
      {
         var hitResult = VisualTreeHelper.HitTest( this, p );
         return hitResult == null ? null : FindTileViewModelInTree( hitResult.VisualHit );
      }

      private static TileViewModel? FindTileViewModelInTree( DependencyObject? obj )
      {
         if ( obj == null )
         {
            return null;
         }

         var visual = VisualTreeHelper.GetParent( obj );

         while ( visual != null && visual.GetType().Name != "ListViewItem" )
         {
            visual = VisualTreeHelper.GetParent( visual );
         }

         if ( visual != null )
         {
            return ( visual is not ListViewItem item || item.Content is not TileViewModel vm ) ? null : vm;
         }
         else
         {
            return null;
         }
      }
   }
}