using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor
{
   public partial class MainWindow : Window
   {
      private readonly MainWindowViewModel _viewModel;
      private int _tileIndexCache = 0;

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
               var data = new DataObject();
               data.SetData( "Object", tileViewData );

               DragDrop.DoDragDrop( this, data, DragDropEffects.Copy | DragDropEffects.Move );
            }
         }
      }

      private void TileMapListViewImage_OnDragEnter( object sender, DragEventArgs e )
      {
         var tileVM = FindTileViewModelAtPoint( e.GetPosition( this ) );

         if ( tileVM != null )
         {
            tileVM.ShouldHighlight = true;
         }
      }

      private void TileMapListViewImage_OnDragLeave( object sender, DragEventArgs e )
      {
         var tileVM = FindTileViewModelFromObject( sender );

         if ( tileVM != null )
         {
            tileVM.ShouldHighlight = false;
         }
      }

      private void TileMapListView_OnPreviewMouseDown( object sender, MouseButtonEventArgs e )
      {
         if ( Mouse.LeftButton == MouseButtonState.Pressed )
         {
            var tileVM = FindTileViewModelAtPoint( Mouse.GetPosition( this ) );

            if ( tileVM != null )
            {
               _tileIndexCache = tileVM.Index;
            }
         }
      }

      private void TileMapListView_OnMouseMove( object sender, MouseEventArgs e )
      {
         base.OnMouseMove( e );

         if ( Mouse.LeftButton == MouseButtonState.Pressed && ( Keyboard.IsKeyDown( Key.LeftShift ) || Keyboard.IsKeyDown( Key.RightShift ) ) )
         {
            var tileVM = FindTileViewModelAtPoint( e.GetPosition( this ) );

            if ( tileVM != null && tileVM.Index != _tileIndexCache )
            {
               tileVM.Index = _tileIndexCache;
            }
         }
      }

      private void TileMapListView_OnDrop( object sender, DragEventArgs e )
      {
         if ( e.Data.GetData( "Object" ) is TileViewModel droppedTileVM )
         {
            var mapTileVM = FindTileViewModelAtPoint( e.GetPosition( this ) );

            if ( mapTileVM != null )
            {
               mapTileVM.Index = droppedTileVM.Index ;
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