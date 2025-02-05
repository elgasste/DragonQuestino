﻿using System.Windows;
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

      private void TileTextureListView_OnMouseMove( object sender, MouseEventArgs e )
      {
         base.OnMouseMove( e );

         if ( e.LeftButton == MouseButtonState.Pressed )
         {
            var tileTextureViewData = FindTileTextureViewModelAtPoint( e.GetPosition( this ) );

            if ( tileTextureViewData is not null )
            {
               var data = new DataObject();
               data.SetData( "Object", tileTextureViewData );

               DragDrop.DoDragDrop( this, data, DragDropEffects.Copy | DragDropEffects.Move );
            }
         }
      }

      private void TileMapTextureListViewImage_OnDragEnter( object sender, DragEventArgs e )
      {
         var tileVM = FindTileViewModelAtPoint( e.GetPosition( this ) );

         if ( tileVM is not null )
         {
            tileVM.IsDraggingTextureOver = true;
         }
      }

      private void TileMapTextureListViewImage_OnDragLeave( object sender, DragEventArgs e )
      {
         var tileVM = FindTileViewModelFromObject( sender );

         if ( tileVM is not null )
         {
            tileVM.IsDraggingTextureOver = false;
         }
      }

      private void TileMapTextureListView_OnPreviewMouseDown( object sender, MouseButtonEventArgs e )
      {
         if ( Mouse.LeftButton == MouseButtonState.Pressed )
         {
            var tileVM = FindTileViewModelAtPoint( Mouse.GetPosition( this ) );

            if ( tileVM is not null )
            {
               _tileIndexCache = tileVM.TextureIndex;
            }
         }
      }

      private void TileMapTextureListView_OnMouseMove( object sender, MouseEventArgs e )
      {
         base.OnMouseMove( e );

         if ( Mouse.LeftButton == MouseButtonState.Pressed && ( Keyboard.IsKeyDown( Key.LeftShift ) || Keyboard.IsKeyDown( Key.RightShift ) ) )
         {
            var tileVM = FindTileViewModelAtPoint( e.GetPosition( this ) );

            if ( tileVM is not null && tileVM.TextureIndex != _tileIndexCache )
            {
               tileVM.TextureIndex = _tileIndexCache;
            }
         }
      }

      private void TileMapTextureListView_OnDrop( object sender, DragEventArgs e )
      {
         if ( e.Data.GetData( "Object" ) is TileTextureViewModel droppedTileTextureVM )
         {
            var mapTileVM = FindTileViewModelAtPoint( e.GetPosition( this ) );

            if ( mapTileVM is not null )
            {
               mapTileVM.TextureIndex = droppedTileTextureVM.Index;
               mapTileVM.IsDraggingTextureOver = false;
            }
         }
      }

      private static TileViewModel? FindTileViewModelFromObject( object obj )
      {
         if ( obj is not null )
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

      private TileTextureViewModel? FindTileTextureViewModelAtPoint( Point p )
      {
         var hitResult = VisualTreeHelper.HitTest( this, p );
         return hitResult is null ? null : FindTileTextureViewModelInTree( hitResult.VisualHit );
      }

      private static TileTextureViewModel? FindTileTextureViewModelInTree( DependencyObject? obj )
      {
         if ( obj is null )
         {
            return null;
         }

         var visual = VisualTreeHelper.GetParent( obj );

         while ( visual is not null && visual.GetType().Name != "ListViewItem" )
         {
            visual = VisualTreeHelper.GetParent( visual );
         }

         if ( visual is not null )
         {
            return ( visual is not ListViewItem item || item.Content is not TileTextureViewModel vm ) ? null : vm;
         }
         else
         {
            return null;
         }
      }

      private TileViewModel? FindTileViewModelAtPoint( Point p )
      {
         var hitResult = VisualTreeHelper.HitTest( this, p );
         return hitResult is null ? null : FindTileViewModelInTree( hitResult.VisualHit );
      }

      private static TileViewModel? FindTileViewModelInTree( DependencyObject? obj )
      {
         if ( obj is null )
         {
            return null;
         }

         var visual = VisualTreeHelper.GetParent( obj );

         while ( visual is not null && visual.GetType().Name != "ListViewItem" )
         {
            visual = VisualTreeHelper.GetParent( visual );
         }

         if ( visual is not null )
         {
            return ( visual is not ListViewItem item || item.Content is not TileViewModel vm ) ? null : vm;
         }
         else
         {
            return null;
         }
      }

      private void TileMapPortalSourceListView_OnSelectionChanged( object sender, SelectionChangedEventArgs e )
      {
         var portalSourceListView = e.OriginalSource as ListView;
         var selectedTileIndex = portalSourceListView?.SelectedIndex;

         if ( selectedTileIndex is not null )
         {
            _viewModel.SelectTile( selectedTileIndex.Value );
         }
      }

      private void TileMapPortalDestinationListView_OnSelectionChanged( object sender, SelectionChangedEventArgs e )
      {
         var portalDestinationListView = e.OriginalSource as ListView;
         var selectedTileIndex = portalDestinationListView?.SelectedIndex;

         if ( selectedTileIndex is not null )
         {
            _viewModel.SelectPortalDestination( selectedTileIndex.Value );
         }
      }
   }
}