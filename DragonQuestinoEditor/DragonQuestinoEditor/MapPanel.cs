using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Media;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor
{
   internal class MapPanel : FrameworkElement
   {
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

            int x = (i % 140) * width;
            int y = (i / 140) * height;

            dc.DrawImage( tile.Image, new Rect( x, y, width, height ) );
         }
      }
   }
}
