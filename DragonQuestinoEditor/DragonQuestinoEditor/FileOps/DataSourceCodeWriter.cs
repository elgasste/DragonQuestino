using System.Collections.ObjectModel;
using System.IO;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Graphics;
using DragonQuestinoEditor.Utilities;
using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class DataSourceCodeWriter( Palette palette,
                                      TileSet tileSet,
                                      ObservableCollection<TileMapViewModel> tileMaps,
                                      ActiveSpriteSheet activeSpriteSheet,
                                      StaticSpriteSheet staticSpriteSheet )
   {
      private readonly Palette _palette = palette;
      private readonly TileSet _tileSet = tileSet;
      private readonly ObservableCollection<TileMapViewModel> _tileMaps = tileMaps;
      private readonly ActiveSpriteSheet _activeSpriteSheet = activeSpriteSheet;
      private readonly StaticSpriteSheet _staticSpriteSheet = staticSpriteSheet;

      public void WriteFiles()
      {
         WriteGameDataFile();
         WriteShieldBackgroundDataFile();
      }

      private void WriteGameDataFile()
      {
         using FileStream fs = File.Create( Constants.GameDataSourceFilePath );
         WriteHeaderSection( fs );
         WritePaletteFunction( fs );
         WriteTextTilesFunction( fs );
         WriteTileTexturesFunction( fs );
         WriteTileMapFunction( fs );
         WriteActiveSpritesFunctions( fs );
         WriteStaticSpritesFunction( fs );
      }

      private void WriteShieldBackgroundDataFile()
      {
         using FileStream fs = File.Create( Constants.ShieldBackgroundSourceFilePath );
         WriteShieldBackgroundFunction( fs );
      }


      private static void WriteHeaderSection( FileStream fs )
      {
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#include \"screen.h\"\n" );
         WriteToFileStream( fs, "#include \"tile_map.h\"\n" );
      }

      private void WritePaletteFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Screen_LoadPalette( Screen_t* screen )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint16_t i;\n\n" );
         WriteToFileStream( fs, string.Format( "   for ( i = 0; i < {0}; i++ ) {{ screen->palette[i] = 0; }}\n\n", Constants.PaletteSize ) );

         for ( int i = 0; i < _palette.ColorCount; i++ )
         {
            WriteToFileStream( fs, string.Format( "   screen->palette[{0}] = 0x{1};\n", i, _palette.Colors[i].ToString( "X4" ) ) );
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileTexturesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadTextures( TileMap_t* tileMap )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t* mem32;\n\n" );

         // TODO: try compressing this, it only ever gets called once
         for ( int i = 0; i < Constants.TileTextureCount; i++ )
         {
            WriteToFileStream( fs, string.Format( "   mem32 = (uint32_t*)( tileMap->textures[{0}].memory );\n", i ) );

            var pixelIndexes = _tileSet.TilePaletteIndexes[i];

            for ( int j = 0, memoryIndex = 0; j < Constants.TilePixels; j += 4, memoryIndex++ )
            {
               var index0 = (UInt32)( pixelIndexes[j + 0] );
               var index1 = (UInt32)( pixelIndexes[j + 1] );
               var index2 = (UInt32)( pixelIndexes[j + 2] );
               var index3 = (UInt32)( pixelIndexes[j + 3] );

               var packed = ( index3 << 24 ) | ( index2 << 16  ) | ( index1 << 8  ) | ( index0 << 0  );

               WriteToFileStream( fs, string.Format( "   mem32[{0}] = 0x{1};\n", memoryIndex, packed.ToString( "X8" ) ) );
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileMapFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_Load( TileMap_t* tileMap, uint32_t index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   int32_t i;\n" );
         WriteToFileStream( fs, "   uint32_t* tiles32 = (uint32_t*)( tileMap->tiles );\n\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         foreach ( var tileMap in _tileMaps )
         {
            var tiles = tileMap.Tiles;

            WriteToFileStream( fs, string.Format( "      case {0}:\n", tileMap.Id ) );
            WriteToFileStream( fs, string.Format( "         tileMap->tilesX = {0};\n", tileMap.TilesX ) );
            WriteToFileStream( fs, string.Format( "         tileMap->tilesY = {0};\n", tileMap.TilesY ) );
            WriteToFileStream( fs, string.Format( "         tileMap->portalCount = {0};\n", tileMap.Portals.Count ) );

            if ( tileMap.Portals.Count > 0 )
            {
               for ( int i = 0; i < tileMap.Portals.Count; i++ )
               {
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].sourceTileIndex = {1};\n", i, tileMap.Portals[i].SourceTileIndex ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].destinationTileMapIndex = {1};\n", i, tileMap.Portals[i].DestinationTileMapIndex ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].destinationTileIndex = {1};\n", i, tileMap.Portals[i].DestinationTileIndex ) );
                  WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].arrivalDirection = (Direction_t){1};\n", i, (int)( tileMap.Portals[i].ArrivalDirection ) ) );
               }
            }

            WriteToFileStream( fs, "         tileMap->activeSpriteCount = 0;\n" );
            WriteToFileStream( fs, string.Format( "         tileMap->staticSpriteCount = {0};\n", tileMap.StaticSprites.Count ) );

            for ( int i = 0; i < tileMap.StaticSprites.Count; i++ )
            {
               var sprite = tileMap.StaticSprites[i];
               int xPos = ( sprite.TileIndex % tileMap.TilesX ) * Constants.SpriteFrameSize;
               int yPos = ( sprite.TileIndex / tileMap.TilesX ) * Constants.SpriteFrameSize;

               WriteToFileStream( fs, string.Format( "         Sprite_LoadStatic( &( tileMap->staticSprites[{0}] ), {1} );\n", i, sprite.TextureIndex ) );
               WriteToFileStream( fs, string.Format( "         tileMap->staticSprites[{0}].position.x = {1};\n", i, xPos ) );
               WriteToFileStream( fs, string.Format( "         tileMap->staticSprites[{0}].position.y = {1};\n", i, yPos ) );
            }

            var packedTiles = new List<UInt32>( ( tileMap.TilesX * tileMap.TilesY ) / 2 );
            var indexCounts = new Dictionary<UInt32, int>();

            for ( int row = 0; row < tileMap.TilesY; row++ )
            {
               for ( int col = 0; col < tileMap.TilesX; col += 2 )
               {
                  var tileIndex = ( row * tileMap.TilesX ) + col;

                  var index0 = (UInt32)( tiles[tileIndex].TextureIndex )
                     | ( tiles[tileIndex].IsPassable ? (UInt32)0x20 : 0 )
                     | Constants.TileSetIndexWalkSpeeds[tiles[tileIndex].TextureIndex]
                     | 0x100  // is encounterable
                     | Constants.TileSetIndexEncounterRates[tiles[tileIndex].TextureIndex]
                     | Constants.TileSetIndexDamageRates[tiles[tileIndex].TextureIndex];
                  var index1 = (UInt32)( tiles[tileIndex + 1].TextureIndex )
                     | ( tiles[tileIndex + 1].IsPassable ? (UInt32)0x20 : 0 )
                     | Constants.TileSetIndexWalkSpeeds[tiles[tileIndex + 1].TextureIndex]
                     | 0x100  // is encounterable
                     | Constants.TileSetIndexEncounterRates[tiles[tileIndex + 1].TextureIndex]
                     | Constants.TileSetIndexDamageRates[tiles[tileIndex + 1].TextureIndex];

                  var packed = ( index1 << 16 ) | index0;
                  packedTiles.Add( packed );

                  if ( indexCounts.TryGetValue( packed, out int value ) )
                  {
                     indexCounts[packed] = ++value;
                  }
                  else
                  {
                     indexCounts[packed] = 1;
                  }
               }
            }

            int highestCount = 0;
            UInt32 mostCommonValue = 0;

            foreach ( var pair in indexCounts )
            {
               if ( pair.Value > highestCount )
               {
                  highestCount = pair.Value;
                  mostCommonValue = pair.Key;
               }
            }

            // TODO: this can be optimized, we should only fill in the area of TilesX and TilesY
            WriteToFileStream( fs, string.Format( "         for ( i = 0; i < ( TILE_COUNT / 2 ); i++ ) {{ tiles32[i] = 0x{0}; }}\n", mostCommonValue.ToString( "X8" ) ) );

            for ( int row = 0, packedTileIndex = 0; row < tileMap.TilesY; row++ )
            {
               for ( int col = 0; col < tileMap.TilesX; packedTileIndex++, col += 2 )
               {
                  var tileIndex = ( row * Constants.TileMapMaxTilesX ) + col;

                  if ( packedTiles[packedTileIndex] != mostCommonValue )
                  {
                     WriteToFileStream( fs, string.Format( "         tiles32[{0}] = 0x{1};\n", tileIndex / 2, packedTiles[packedTileIndex].ToString( "X8" ) ) );
                  }
               }
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteActiveSpritesFunctions( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Sprite_LoadPlayer( ActiveSprite_t* sprite )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   int32_t i;\n" );
         WriteToFileStream( fs, "   uint32_t* mem32 = (uint32_t*)( sprite->textures[0].memory );\n\n" );

         var indexCounts = new Dictionary<UInt32, int>();
         var packedIndexes = new List<UInt32>();

         for ( int i = 0; i < Constants.ActiveSpritePositionCount; i++ )
         {
            for ( int j = 0; j < Constants.ActiveSpriteFrameCount; j++ )
            {
               var pixelIndexes = _activeSpriteSheet.FramePaletteIndexes[i][j];

               for ( int k = 0; k < Constants.SpriteFramePixels; k += 4 )
               {
                  var index0 = (UInt32)( pixelIndexes[k + 0] );
                  var index1 = (UInt32)( pixelIndexes[k + 1] );
                  var index2 = (UInt32)( pixelIndexes[k + 2] );
                  var index3 = (UInt32)( pixelIndexes[k + 3] );
                  var packed = ( index3 << 24 ) | ( index2 << 16 ) | ( index1 << 8 ) | ( index0 << 0 );

                  packedIndexes.Add( packed );

                  if ( indexCounts.TryGetValue( packed, out int value ) )
                  {
                     indexCounts[packed] = ++value;
                  }
                  else
                  {
                     indexCounts[packed] = 1;
                  }
               }
            }
         }

         int highestCount = 0;
         UInt32 mostCommonValue = 0;

         foreach ( var pair in indexCounts )
         {
            if ( pair.Value > highestCount )
            {
               highestCount = pair.Value;
               mostCommonValue = pair.Key;
            }
         }

         WriteToFileStream( fs, string.Format( "   for ( i = 0; i < ( SPRITE_TEXTURE_BYTES / 4 ) * ACTIVE_SPRITE_TEXTURES; i++ ) {{ mem32[i] = 0x{0}; }}\n", mostCommonValue.ToString( "X8" ) ) );

         for ( int i = 0, packedIndex = 0; i < Constants.ActiveSpritePositionCount; i++ )
         {
            for ( int j = 0; j < Constants.ActiveSpriteFrameCount; j++ )
            {
               WriteToFileStream( fs, string.Format( "   mem32 = (uint32_t*)( sprite->textures[{0}].memory );\n", ( i * Constants.ActiveSpriteFrameCount ) + j ) );

               for ( int k = 0, memoryIndex = 0; k < Constants.SpriteFramePixels; k += 4, memoryIndex++, packedIndex++ )
               {
                  if ( packedIndexes[packedIndex] != mostCommonValue )
                  {
                     WriteToFileStream( fs, string.Format( "   mem32[{0}] = 0x{1};\n", memoryIndex, packedIndexes[packedIndex].ToString( "X8" ) ) );
                  }
               }
            }
         }

         WriteToFileStream( fs, "}\n" );

         // TODO
         WriteToFileStream( fs, "\nvoid Sprite_LoadActive( ActiveSprite_t* sprite, uint32_t index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   UNUSED_PARAM( sprite );\n" );
         WriteToFileStream( fs, "   UNUSED_PARAM( index );\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteStaticSpritesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Sprite_LoadStatic( StaticSprite_t* sprite, uint32_t index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t* mem32 = (uint32_t*)( sprite->texture.memory );\n\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _staticSpriteSheet.Bitmaps.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );
            
            var pixelIndexes = _staticSpriteSheet.PaletteIndexes[i];

            for ( int k = 0, memoryIndex = 0; k < Constants.SpriteFramePixels; k += 4, memoryIndex++ )
            {
               var index0 = (UInt32)( pixelIndexes[k + 0] );
               var index1 = (UInt32)( pixelIndexes[k + 1] );
               var index2 = (UInt32)( pixelIndexes[k + 2] );
               var index3 = (UInt32)( pixelIndexes[k + 3] );
               var packed = ( index3 << 24 ) | ( index2 << 16 ) | ( index1 << 8 ) | ( index0 << 0 );

               WriteToFileStream( fs, string.Format( "         mem32[{0}] = 0x{1};\n", memoryIndex, packed.ToString( "X8" ) ) );
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private static void WriteShieldBackgroundFunction( FileStream fs )
      {
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#include \"giga_shield_background_data.h\"\n\n" );
         WriteToFileStream( fs, "void Giga_LoadShieldBackground( uint32_t* buffer )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t i;\n\n" );

         var textFileStream = new FileStream( Constants.ShieldBackgroundFilePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];
         BitmapUtils.CheckBackgroundBitmapFormat( bitmapSource );

         int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
         var data = new byte[stride * bitmapSource.PixelHeight];
         bitmapSource.CopyPixels( data, stride, 0 );

         var packedPixelCounts = new Dictionary<UInt32, int>();
         List<UInt32> packedPixels = new( ( Constants.GigaShieldWidth / 2 ) * Constants.GigaShieldHeight );

         for ( int i = 0, j = 0, dataIndex = 0; i < Constants.GigaShieldWidth * Constants.GigaShieldHeight; i += 2, j++ )
         {
            var color16 = ColorUtils.BytesToRgb565( data[dataIndex + 2], data[dataIndex + 1], data[dataIndex + 1] );
            dataIndex += 3;
            var packedPixel = (UInt32)color16 << 16;

            color16 = ColorUtils.BytesToRgb565( data[dataIndex + 2], data[dataIndex + 1], data[dataIndex + 1] );
            dataIndex += 3;
            packedPixel |= (UInt32)color16;

            packedPixels.Add( packedPixel );

            if ( packedPixelCounts.TryGetValue( packedPixel, out int value ) )
            {
               packedPixelCounts[packedPixel] = ++value;
            }
            else
            {
               packedPixelCounts[packedPixel] = 1;
            }
         }

         int highestCount = 0;
         UInt32 mostCommonValue = 0;

         foreach ( var c in packedPixelCounts )
         {
            if ( c.Value > highestCount )
            {
               highestCount = c.Value;
               mostCommonValue = c.Key;
            }
         }

         WriteToFileStream( fs, "   for ( i = 0; i < ( GIGA_SHIELD_WIDTH / 2 ) * GIGA_SHIELD_HEIGHT; i++ )\n" );
         WriteToFileStream( fs, "   {\n" );
         WriteToFileStream( fs, string.Format( "      buffer[i] = 0x{0};\n", mostCommonValue.ToString( "X8" ) ) );
         WriteToFileStream( fs, "   }\n\n" );

         for ( int i = 0; i < packedPixels.Count; )
         {
            int firstIndex = i;
            int lastIndex = i;
            var currentPixel = packedPixels[i];
            i++;

            if ( currentPixel != mostCommonValue )
            {
               while ( i < packedPixels.Count )
               {
                  var nextPixel = packedPixels[i];
                  lastIndex = i;
                  i++;

                  if ( nextPixel != currentPixel )
                  {
                     break;
                  }
               }

               if ( ( lastIndex - firstIndex ) > 1 )
               {
                  WriteToFileStream( fs, string.Format( "   for ( i = {0}; i <= {1}; i++ ) buffer[i] = 0x{2};\n", firstIndex, lastIndex, packedPixels[firstIndex].ToString( "X8" ) ) );
               }
               else
               {
                  WriteToFileStream( fs, string.Format( "   buffer[{0}] = 0x{1};\n", firstIndex, packedPixels[firstIndex].ToString( "X8" ) ) );

                  if ( lastIndex - firstIndex > 0 )
                  {
                     i--;
                  }
               }
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private static void WriteTextTilesFunction( FileStream fs )
      {
         var textTextureMap = new List<byte>();

         var textFileStream = new FileStream( Constants.TextTileSetFilePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         BitmapSource bitmap = textDecoder.Frames[0];
         BitmapUtils.CheckTextTileSetBitmapFormat( bitmap );

         for ( int row = 0; row < bitmap.PixelHeight; row++ )
         {
            for ( int col = 0; col < bitmap.PixelWidth; col += 8 )
            {
               textTextureMap.Add( 0x00 );

               for ( int i = 0; i < Constants.TextTileSize; i++ )
               {
                  var pixelColor = BitmapUtils.GetBitmapPixelColor( bitmap, col + i, row );

                  if ( !Color.AreClose( pixelColor, Color.FromArgb( 255, 0, 0, 0 ) ) )
                  {
                     textTextureMap[^1] |= (byte)( 1 << ( Constants.TextTileSize - i - 1 ) );
                  }
               }
            }
         }

         WriteToFileStream( fs, "\nvoid Screen_LoadTextBitFields( Screen_t* screen )\n" );
         WriteToFileStream( fs, "{\n" );

         for ( int i = 0; i < Constants.TextTileCount; i++ )
         {
            for ( int j = 0; j < Constants.TextTileSize; j++ )
            {
               byte b = textTextureMap[i + ( j * Constants.TextTileCount )];
               WriteToFileStream( fs, string.Format( "   screen->textBitFields[{0}][{1}] = 0x{2};\n", i, j, b.ToString( "X2" ) ) );
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private static void WriteToFileStream( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
