using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using TileMapEditor.Utilities;

namespace TileMapEditor.Tiles
{
   internal class TileSet
   {
      private readonly List<WriteableBitmap> _tileBitmaps = new( Constants.TileCount );
      public List<List<int>> TilePaletteIndexes = new ( Constants.TileCount );
      public List<UInt16> Palette { get; } = new( Constants.PaletteSize );
      public int PaletteCount { get; private set; } = 0;

      public List<WriteableBitmap> TileBitmaps => _tileBitmaps;

      public TileSet( string imagePath )
      {
         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapSanityCheck( bitmapSource );
         ReadTileBitmaps( bitmapSource );
         LoadPalette();
      }

      private static void BitmapSanityCheck( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Expecting tileset image pixel format to be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.TileSize * Constants.TileCount ) )
         {
            throw new Exception( string.Format( "Tileset image width should be {0}", Constants.TileSize * Constants.TileCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.TileSize )
         {
            throw new Exception( string.Format( "Tileset image height should be {0}", Constants.TileSize ) );
         }
      }

      private void ReadTileBitmaps( BitmapSource bitmapSource )
      {
         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
            var data = new byte[stride * bitmapSource.PixelHeight];
            bitmapSource.CopyPixels( data, stride, 0 );
            _tileBitmaps.Add( new WriteableBitmap( Constants.TileSize, bitmapSource.PixelHeight, bitmapSource.DpiX, bitmapSource.DpiY, bitmapSource.Format, bitmapSource.Palette ) );
            _tileBitmaps[i].WritePixels( new Int32Rect( 0, 0, Constants.TileSize, Constants.TileSize ), data, stride, Constants.TileSize * i );
         }
      }

      private void LoadPalette()
      {
         for ( int i = 0; i < Constants.PaletteSize; i++ )
         {
            Palette.Add( 0 );
         }

         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            TilePaletteIndexes.Add( new( Constants.TilePixels ) );

            for ( int j = 0; j < Constants.TilePixels; j++ )
            {
               TilePaletteIndexes[i].Add( 0 );
            }
         }

         PaletteCount = 0;

         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            var tileBitmap = _tileBitmaps[i];

            for ( int y = 0; y < tileBitmap.PixelHeight; y++ )
            {
               for ( int x = 0; x < tileBitmap.PixelWidth; x++ )
               {
                  var pixelColor = ColorUtils.GetPixelColor( tileBitmap, x, y );
                  var pixelColor16 = ColorUtils.ColortoUInt16( pixelColor );
                  var paletteIndex = GetPaletteIndexForColor( pixelColor16 );

                  if ( paletteIndex < 0 )
                  {
                     PaletteCount++;

                     if ( PaletteCount > Constants.PaletteSize )
                     {
                        throw new Exception( "tile map contains too many colors for the palette" );
                     }

                     paletteIndex = PaletteCount - 1;
                     Palette[paletteIndex] = pixelColor16;
                  }

                  TilePaletteIndexes[i][( y * Constants.TileSize ) + x] = paletteIndex;
               }
            }
         }
      }

      public int GetPaletteIndexForColor( UInt16 color16 )
      {
         for ( int i = 0; i < PaletteCount; i++ )
         {
            if (  Palette[i] == color16 )
            {
               return i;
            }
         }

         return -1;
      }
   }
}
