using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.Utilities
{
   public static class BitmapUtils
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

      public static void CheckActiveSpriteSheetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Sprite image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.SpriteFrameSize * Constants.ActiveSpriteFrameCount ) )
         {
            throw new Exception( string.Format( "Active sprite image width should be {0}", Constants.SpriteFrameSize * Constants.ActiveSpriteFrameCount ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.SpriteFrameSize * Constants.ActiveSpritePositionCount )
         {
            throw new Exception( string.Format( "Active sprite image height should be {0}", Constants.SpriteFrameSize * Constants.ActiveSpritePositionCount ) );
         }
      }

      public static void CheckStaticSpriteSheetBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Sprite image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth % Constants.SpriteFrameSize != 0 )
         {
            throw new Exception( string.Format( "Static sprite image width should be a multiple of {0}", Constants.SpriteFrameSize ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.SpriteFrameSize )
         {
            throw new Exception( string.Format( "Sprite image height should be {0}", Constants.SpriteFrameSize ) );
         }
      }
   }
}
