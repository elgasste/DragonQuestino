using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Utilities;

namespace DragonQuestinoEditor.Graphics
{
   public class TileSet : ITileTextureProvider
   {
      private readonly List<WriteableBitmap> _tileBitmaps = new( Constants.TileTextureCount );
      private readonly Palette _palette;

      public List<List<int>> TilePaletteIndexes = new ( Constants.TileTextureCount );

      public List<WriteableBitmap> TileBitmaps => _tileBitmaps;

      public TileSet( string imagePath, Palette palette )
      {
         _palette = palette;

         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapUtils.CheckTileSetBitmapFormat( bitmapSource );
         ReadTileBitmaps( bitmapSource );
         UpdatePalette();
      }

      private void ReadTileBitmaps( BitmapSource bitmapSource )
      {
         for ( int i = 0; i < Constants.TileTextureCount; i++ )
         {
            int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
            var data = new byte[stride * bitmapSource.PixelHeight];
            bitmapSource.CopyPixels( data, stride, 0 );
            _tileBitmaps.Add( new WriteableBitmap( Constants.TileSize, bitmapSource.PixelHeight, bitmapSource.DpiX, bitmapSource.DpiY, bitmapSource.Format, bitmapSource.Palette ) );
            _tileBitmaps[i].WritePixels( new Int32Rect( 0, 0, Constants.TileSize, Constants.TileSize ), data, stride, Constants.TileSize * i );
         }
      }

      private void UpdatePalette()
      {
         for ( int i = 0; i < Constants.TileTextureCount; i++ )
         {
            TilePaletteIndexes.Add( new( Constants.TilePixels ) );

            for ( int j = 0; j < Constants.TilePixels; j++ )
            {
               TilePaletteIndexes[i].Add( 0 );
            }
         }

         for ( int i = 0; i < Constants.TileTextureCount; i++ )
         {
            var tileBitmap = _tileBitmaps[i];

            for ( int y = 0; y < tileBitmap.PixelHeight; y++ )
            {
               for ( int x = 0; x < tileBitmap.PixelWidth; x++ )
               {
                  var pixelColor = ColorUtils.GetPixelColor( tileBitmap, x, y );
                  var pixelColor16 = ColorUtils.ColorToUInt16( pixelColor );
                  _palette.AddColor( pixelColor16 );
                  TilePaletteIndexes[i][( y * Constants.TileSize ) + x] = _palette.GetIndexForColor( pixelColor16 );
               }
            }
         }
      }

      public BitmapSource GetImageFromIndex( int index ) => _tileBitmaps[index];
   }
}
