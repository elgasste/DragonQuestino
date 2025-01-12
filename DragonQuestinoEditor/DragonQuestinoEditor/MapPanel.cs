using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor
{
   internal class MapPanel : FrameworkElement
   {
      private const int _tilesPerRow = 140;
      private const int _defaultTileSize = 16;

      private static readonly TimeSpan _zoomAnimationDuration = TimeSpan.FromMilliseconds( 500 );
      private static readonly EasingFunctionBase _zoomAnimationEase = new CircleEase
      {
         EasingMode = EasingMode.EaseOut
      };

      private readonly SolidColorBrush _background = new SolidColorBrush( Color.FromRgb( 64, 64, 64 ) );
      private readonly SolidColorBrush _highlight = new SolidColorBrush( Color.FromArgb( 128, 255, 0, 0 ) );

      private bool _isLeftButtonDown;
      private Point _dragAnchorPoint;

      private WriteableBitmap? _bitmap;
      private byte[]? _rawBuffer;

      private bool _isAnimatingZooming;

      private int _cellX;
      private int _cellY;

      private InputMode _inputMode;

      private static readonly DependencyProperty ZoomProperty = DependencyProperty.Register(
         nameof( Zoom ),
         typeof( double ),
         typeof( MapPanel ),
         new FrameworkPropertyMetadata( 1.0, FrameworkPropertyMetadataOptions.AffectsRender ) );

      private double Zoom
      {
         get => (double)GetValue( ZoomProperty );
         set => SetValue( ZoomProperty, value );
      }

      private static readonly DependencyProperty TileHighlightProperty = DependencyProperty.Register(
         nameof( TileHighlight ),
         typeof( Rect ),
         typeof( MapPanel ),
         new FrameworkPropertyMetadata( Rect.Empty, FrameworkPropertyMetadataOptions.AffectsRender ) );

      public Rect TileHighlight
      {
         get => (Rect)GetValue( TileHighlightProperty );
         set => SetValue( TileHighlightProperty, value );
      }

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
         Focusable = true;
      }

      /// <summary>
      /// Gets the "current" size of the tiles in the UI. This accounts for the various UI states,
      /// such as pan and zoom.
      /// </summary>
      /// <returns>A size instances that describes the pixel size of the tiles in the UI.</returns>
      private Size GetCurrentTileSize()
      {
         return new Size( _defaultTileSize * Zoom, _defaultTileSize * Zoom );
      }

      private void PrepareBitmap()
      {
         const int bytesPerPixel = 4;

         int width = _tilesPerRow * _defaultTileSize * bytesPerPixel;
         int height = _tilesPerRow * _defaultTileSize * bytesPerPixel;

         _bitmap = new WriteableBitmap( width, height, 96, 96, PixelFormats.Bgra32, null );
         _rawBuffer = new byte[width * height];

         for (int i = 0; i < TileMap.Count; i++)
         {
            int cellX = i % _tilesPerRow;
            int cellY = i / _tilesPerRow;

            int destX = cellX * _defaultTileSize * bytesPerPixel;
            int destY = cellY * _defaultTileSize;

            int tileIndex = TileMap[i].Index;
            var tile = TileMap[i].TileSet.Tiles[tileIndex];

            tile.DrawToBuffer( _rawBuffer, width, destX, destY );
         }

         // I don't know why I have to divide by 4 on the width, height
         _bitmap.WritePixels( new Int32Rect( 0, 0, _bitmap.PixelWidth / 4, _bitmap.PixelHeight / 4 ), _rawBuffer, _bitmap.PixelWidth, 0 );
      }

      private void SetZoomLevel( double zoomFactorDelta )
      {
         if (_isAnimatingZooming)
         {
            return;
         }

         _isAnimatingZooming = true;
         double zoomFactor = Math.Clamp( Zoom + zoomFactorDelta, 0.2, 5 );

         var zoomLevelAnimation = new DoubleAnimation( zoomFactor, _zoomAnimationDuration )
         {
            EasingFunction = _zoomAnimationEase
         };

         zoomLevelAnimation.Completed += OnAnimationComplete;
         BeginAnimation( ZoomProperty, zoomLevelAnimation );

         void OnAnimationComplete( object? sender, EventArgs e )
         {
            _isAnimatingZooming = false;
         }
      }

      protected override void OnPreviewKeyDown( KeyEventArgs e )
      {
         if (e.Key == Key.Space)
         {
            _inputMode = InputMode.Pan;
            Cursor = Cursors.ScrollAll;
         }
      }

      protected override void OnPreviewKeyUp( KeyEventArgs e )
      {
         base.OnPreviewKeyUp( e );

         _inputMode = InputMode.Draw;
         Cursor = Cursors.Arrow;
      }

      protected override void OnMouseWheel( MouseWheelEventArgs e )
      {
         base.OnMouseWheel( e );

         double delta = e.Delta > 0 ? 1 : -1;
         SetZoomLevel( delta );
      }

      protected override void OnMouseLeftButtonDown( MouseButtonEventArgs e )
      {
         base.OnMouseLeftButtonDown( e );

         switch (_inputMode)
         {
            case InputMode.Draw:
            {
               if (_bitmap is null)
               {
                  return;
               }

               int offset = _cellY * _tilesPerRow + _cellX;
               var tileViewModel = TileMap[offset];
               tileViewModel.SetIndex( 1 );

               var byteBuffer = new byte[_defaultTileSize * _defaultTileSize * 4];
               var tileSprite = TileMap[tileViewModel.Index].TileSet.Tiles[tileViewModel.Index];
               tileSprite.DrawToBuffer( byteBuffer, _defaultTileSize * 4, 0, 0 );

               int destX = _cellX * 16;
               int destY = _cellY * 16;

               _bitmap.WritePixels( new Int32Rect( destX, destY, _defaultTileSize, _defaultTileSize ), byteBuffer, _defaultTileSize * 4, 0 );
               InvalidateVisual();

               break;
            }
            case InputMode.Pan:
            {
               _isLeftButtonDown = true;
               CaptureMouse();
               _dragAnchorPoint = e.GetPosition( this ) - Offset;

               break;
            }
         }
      }

      protected override void OnMouseMove( MouseEventArgs e )
      {
         base.OnMouseMove( e );

         if (_bitmap is null)
         {
            return;
         }

         var mousePos = e.GetPosition( this );
         var mapRect = new Rect( Offset.X * Zoom, Offset.Y * Zoom, _bitmap.Width, _bitmap.Height );

         if (mapRect.Contains( mousePos ))
         {
            var tileSize = GetCurrentTileSize();
            _cellX = (int)((mousePos.X - Offset.X * Zoom) / tileSize.Width);
            _cellY = (int)((mousePos.Y - Offset.Y * Zoom) / tileSize.Height);

            TileHighlight = new Rect( _cellX * _defaultTileSize, _cellY * _defaultTileSize, _defaultTileSize, _defaultTileSize );
         }
         else
         {
            TileHighlight = Rect.Empty;
         }

         if (_isLeftButtonDown)
         {
            Offset = mousePos - _dragAnchorPoint;
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
            var transform = new TransformGroup();
            transform.Children.Add( new TranslateTransform( Offset.X, Offset.Y ) );
            transform.Children.Add( new ScaleTransform( Zoom, Zoom ) );

            dc.PushTransform( transform );
            dc.DrawImage( _bitmap, new Rect( 0, 0, _bitmap.Width, _bitmap.Height ) );

            if (TileHighlight != Rect.Empty && _inputMode == InputMode.Draw)
            {
               dc.DrawRectangle( _highlight, null, TileHighlight );
            }

            dc.Pop();
         }
      }
   }
}
