using DragonQuestinoEditor.FileOps;
using System.IO;
using System;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Utilities;
using DragonQuestinoEditor.Graphics;
using System.Windows;

namespace DragonQuestinoEditor.ViewModels
{
   public class EnemyViewModel : ViewModelBase
   {
      private readonly Palette _palette;

      public List<int> TextureIndexes { get; } = [];

      public List<List<byte>> TileTextures { get; } = [];

      private int _index;
      public int Index
      {
         get => _index;
         set => SetProperty( ref _index, value );
      }

      private string _name = "";
      public string Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      private string _nameMacro = "";
      public string NameMacro
      {
         get => _nameMacro;
         set => SetProperty( ref _nameMacro, value );
      }

      private string _indefiniteArticle = "";
      public string IndefiniteArticle
      {
         get => _indefiniteArticle;
         set => SetProperty( ref _indefiniteArticle, value );
      }

      private int _hitPoints;
      public int HitPoints
      {
         get => _hitPoints;
         set => SetProperty( ref _hitPoints, value );
      }

      private int _attackPower;
      public int AttackPower
      {
         get => _attackPower;
         set => SetProperty( ref _attackPower, value );
      }

      private int _defensePower;
      public int DefensePower
      {
         get => _defensePower;
         set => SetProperty( ref _defensePower, value );
      }

      private int _agility;
      public int Agility
      {
         get => _agility;
         set => SetProperty( ref _agility, value );
      }

      private int _experience;
      public int Experience
      {
         get => _experience;
         set => SetProperty( ref _experience, value );
      }

      private int _gold;
      public int Gold
      {
         get => _gold;
         set => SetProperty( ref _gold, value );
      }

      public EnemyViewModel( Palette palette, EnemySaveData saveData )
      {
         _palette = palette;

         Index = saveData.Index;
         Name = saveData.Name;
         NameMacro = saveData.NameMacro;
         IndefiniteArticle = saveData.IndefiniteArticle;
         HitPoints = saveData.HitPoints;
         AttackPower = saveData.AttackPower;
         DefensePower = saveData.DefensePower;
         Agility = saveData.Agility;
         Experience = saveData.Experience;
         Gold = saveData.Gold;

         var enemyFileStream = new FileStream( Constants.AssetsBasePath + Name + ".png", FileMode.Open, FileAccess.Read, FileShare.Read );
         var enemyDecoder = new PngBitmapDecoder( enemyFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         BitmapSource bitmapSource = enemyDecoder.Frames[0];
         BitmapUtils.CheckEnemyTileSetBitmapFormat( bitmapSource );
         ReadTileBitmaps( bitmapSource );
      }

      private int PaletteIndexFromColor( ushort color )
      {
         int paletteIndex = _palette.GetIndexForColor( color );

         if ( paletteIndex < 0 )
         {
            _palette.AddColor( color );
            paletteIndex = _palette.ColorCount - 1;
         }

         return paletteIndex;
      }

      private void ReadTileBitmaps( BitmapSource bitmapSource )
      {
         var textureMapBytes = new List<byte>();
         byte blankPaletteIndex = (byte)_palette.GetIndexForColor( 0 );

         for ( int row = 0; row < bitmapSource.PixelHeight; row++ )
         {
            for ( int col = 0; col < bitmapSource.PixelWidth; col++ )
            {
               var pixelColor = BitmapUtils.GetPixelColor16( bitmapSource, col, row );
               textureMapBytes.Add( (byte)PaletteIndexFromColor( pixelColor ) );
            }
         }

         // TODO: magic numbers!
         for ( int tileRow = 0; tileRow < 12; tileRow++ )
         {
            for ( int tileCol = 0; tileCol < 10; tileCol++ )
            {
               bool blankTexture = true;
               var tileTextureBytes = new List<byte>();

               for ( int pixelRow = 0; pixelRow < 8; pixelRow++ )
               {
                  for ( int pixelCol = 0; pixelCol < 8; pixelCol++ )
                  {
                     int pixelIndex = ( tileRow * 10 * 8 * 8 ) + ( pixelRow * 10 * 8 ) + ( tileCol * 8 ) + pixelCol;
                     tileTextureBytes.Add( textureMapBytes[pixelIndex] );
                  }
               }

               foreach ( var textureByte in tileTextureBytes )
               {
                  if ( textureByte != blankPaletteIndex )
                  {
                     blankTexture = false;
                     break;
                  }
               }

               if ( blankTexture )
               {
                  TextureIndexes.Add( -1 );
               }
               else
               {
                  // TODO: what if this tile texture already exists for this enemy? should we check for that?
                  TileTextures.Add( tileTextureBytes );
                  TextureIndexes.Add( TileTextures.Count - 1 );
               }
            }
         }
      }
   }
}
