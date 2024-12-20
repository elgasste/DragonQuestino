using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Utilities
{
   internal static class BitmapUtils
   {
      public static void CheckTileSetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Tileset image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.TileSize * Constants.TileTextureCount ) )
         {
            throw new Exception( string.Format( "Tileset image width should be {0}", Constants.TileSize * Constants.TileTextureCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.TileSize )
         {
            throw new Exception( string.Format( "Tileset image height should be {0}", Constants.TileSize ) );
         }
      }

      public static void CheckSpriteSheetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Sprite image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.SpriteFrameSize * Constants.SpriteFrameCount ) )
         {
            throw new Exception( string.Format( "Sprite image width should be {0}", Constants.SpriteFrameSize * Constants.SpriteFrameCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.SpriteFrameSize * Constants.SpritePositionCount )
         {
            throw new Exception( string.Format( "Sprite image height should be {0}", Constants.SpriteFrameSize * Constants.SpritePositionCount ) );
         }
      }
   }
}
