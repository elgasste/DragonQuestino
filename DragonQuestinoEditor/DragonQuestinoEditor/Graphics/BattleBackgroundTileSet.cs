using DragonQuestinoEditor.Utilities;
using System.IO;
using System.Windows.Media.Imaging;
using System.Windows;

namespace DragonQuestinoEditor.Graphics
{
   public class BattleBackgroundTileSet
   {
      private readonly List<WriteableBitmap> _tileBitmaps = new( Constants.BattleBackgroundTileTextureCount );
      private readonly Palette _palette;
      private readonly Sprite[] _tileTextures = new Sprite[Constants.BattleBackgroundTileTextureCount];

      public List<List<int>> TilePaletteIndexes = new( Constants.BattleBackgroundTileTextureCount );

      public List<WriteableBitmap> TileBitmaps => _tileBitmaps;
      public Sprite[] TileTextures => _tileTextures;

      public BattleBackgroundTileSet( string imagePath, Palette palette )
      {
         _palette = palette;

         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];

         BitmapUtils.CheckBattleBackgroundTileSetBitmapFormat( bitmapSource );
         ReadTileBitmaps( bitmapSource );
         UpdatePalette();

         // Extract the tiles as sprites

         var tileSheet = Sprite.LoadFromFile( imagePath );
         int tileCount = tileSheet.Width / Constants.TileSize;

         for ( int textureIndex = 0; textureIndex < tileCount; textureIndex++ )
         {
            int srcX = textureIndex * Constants.TileSize;
            int srcY = 0;

            var tileSprite = tileSheet.Extract( srcX, srcY, Constants.TileSize, Constants.TileSize );
            _tileTextures[textureIndex] = tileSprite;
         }
      }

      private void ReadTileBitmaps( BitmapSource bitmapSource )
      {
         for ( int i = 0; i < Constants.BattleBackgroundTileTextureCount; i++ )
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
         for ( int i = 0; i < Constants.BattleBackgroundTileTextureCount; i++ )
         {
            TilePaletteIndexes.Add( new( Constants.TilePixels ) );

            for ( int j = 0; j < Constants.TilePixels; j++ )
            {
               TilePaletteIndexes[i].Add( 0 );
            }
         }

         for ( int i = 0; i < Constants.BattleBackgroundTileTextureCount; i++ )
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
