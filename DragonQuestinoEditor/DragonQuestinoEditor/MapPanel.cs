using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor
{
   internal class MapPanel : FrameworkElement
   {
      private bool _isLeftButtonDown;
      private Point _dragAnchorPoint;

      private static readonly DependencyProperty OffsetProperty = DependencyProperty.Register(
         nameof( Offset ),
         typeof( Vector ),
         typeof( MapPanel ),
         new FrameworkPropertyMetadata( new Vector( 0, 0 ), FrameworkPropertyMetadataOptions.AffectsRender ) );

      public Vector Offset
      {
         get => (Vector)GetValue( OffsetProperty );
         set => SetValue( OffsetProperty, value );
      }

      public static readonly DependencyProperty TileMapProperty = DependencyProperty.Register(
         nameof( TileMap ),
         typeof( Collection<TileViewModel> ),
         typeof( MapPanel ),
         new FrameworkPropertyMetadata( null, FrameworkPropertyMetadataOptions.AffectsMeasure | FrameworkPropertyMetadataOptions.AffectsArrange ) );

      public Collection<TileViewModel> TileMap
      {
         get => (Collection<TileViewModel>)GetValue( TileMapProperty );
         set => SetValue( TileMapProperty, value );
      }

      protected override void OnMouseLeftButtonDown( MouseButtonEventArgs e )
      {
         base.OnMouseLeftButtonDown( e );

         _isLeftButtonDown = true;
         _dragAnchorPoint = e.GetPosition( this ) - Offset;
      }

      protected override void OnMouseMove( MouseEventArgs e )
      {
         base.OnMouseMove( e );

         if (_isLeftButtonDown)
         {
            Offset = e.GetPosition( this ) - _dragAnchorPoint;
         }
      }

      protected override void OnMouseLeftButtonUp( MouseButtonEventArgs e )
      {
         base.OnMouseLeftButtonUp( e );
         _isLeftButtonDown = false;
      }

      protected override void OnRender( DrawingContext dc )
      {
         if (TileMap is null || TileMap.Count == 0)
         {
            return;
         }

         dc.DrawRectangle( Brushes.DarkSlateGray, null, new Rect( 0, 0, ActualWidth, ActualHeight ) );

         for (int i = 0; i < TileMap.Count; i++)
         {
            var tile = TileMap[i];

            int width = (int)tile.Image.Width;
            int height = (int)tile.Image.Height;

            var offset = Offset;

            int x = (int)offset.X + (i % 140) * width;
            int y = (int)offset.Y + (i / 140) * height;

            dc.DrawImage( tile.Image, new Rect( x, y, width, height ) );
         }
      }
   }
}
