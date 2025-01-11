using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor
{
   internal class MapPanel : FrameworkElement
   {
      private readonly SolidColorBrush _background = new SolidColorBrush( Color.FromRgb( 64, 64, 64 ) );

      private bool _isLeftButtonDown;
      private Point _dragAnchorPoint;

      private WriteableBitmap? _bitmap;
      private byte[]? _rawBuffer;

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
         new PropertyMetadata( OnTileMapChanged ) );

      private static void OnTileMapChanged( DependencyObject obj, DependencyPropertyChangedEventArgs e )
      {
         if (obj is not MapPanel sender || e.NewValue is not Collection<TileViewModel> tileMap)
         {
            return;
         }

         sender.PrepareBitmap();
      }

      public Collection<TileViewModel> TileMap
      {
         get => (Collection<TileViewModel>)GetValue( TileMapProperty );
         set => SetValue( TileMapProperty, value );
      }

      public MapPanel()
      {
         ClipToBounds = true;
      }

      private void PrepareBitmap()
      {
         const int tilesPerRow = 140;
         const int tileWidthInPixels = 16;
         const int tileHeightInPixels = 16;
         const int bytesPerPixel = 4;

         int width = tilesPerRow * tileWidthInPixels * bytesPerPixel;
         int height = tilesPerRow * tileHeightInPixels * bytesPerPixel;

         _bitmap = new WriteableBitmap( width, height, 96, 96, PixelFormats.Bgra32, null );
         _rawBuffer = new byte[width * height];

         for (int i = 0; i < TileMap.Count; i++)
         {
            int cellX = i % tilesPerRow;
            int cellY = i / tilesPerRow;

            int destX = cellX * tileWidthInPixels * bytesPerPixel;
            int destY = cellY * tileWidthInPixels;

            int tileIndex = TileMap[i].Index;
            var tile = TileMap[i].TileSet.Tiles[tileIndex];

            tile.DrawToBuffer( _rawBuffer, width, destX, destY );
         }

         // I don't know why I have to divide by 4 on the width, height
         _bitmap.WritePixels( new Int32Rect( 0, 0, _bitmap.PixelWidth / 4, _bitmap.PixelHeight / 4 ), _rawBuffer, _bitmap.PixelWidth, 0 );
      }

      protected override void OnMouseLeftButtonDown( MouseButtonEventArgs e )
      {
         base.OnMouseLeftButtonDown( e );

         _isLeftButtonDown = true;
         CaptureMouse();

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
         ReleaseMouseCapture();

         _isLeftButtonDown = false;
      }

      protected override void OnRender( DrawingContext dc )
      {
         dc.DrawRectangle( _background, null, new Rect( 0, 0, ActualWidth, ActualHeight ) );

         if (_bitmap is not null)
         {
            dc.PushTransform( new ScaleTransform( _zoom, _zoom ) );
            dc.DrawImage( _bitmap, new Rect( Offset.X, Offset.Y, _bitmap.Width, _bitmap.Height ) );
            dc.Pop();
         }
      }
   }
}
