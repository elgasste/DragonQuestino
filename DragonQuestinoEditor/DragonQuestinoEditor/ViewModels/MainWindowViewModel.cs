using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Commands;
using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;

namespace DragonQuestinoEditor.ViewModels
{
   internal class MainWindowViewModel : ViewModelBase
   {
      private readonly Palette _palette;
      private readonly TileSet _tileSet;
      private readonly SpriteSheet _playerSpriteSheet;

      public ObservableCollection<TileViewModel> TileSelectionViewModels { get; } = [];
      public ObservableCollection<TileViewModel> TileViewModels { get; } = [];

      public MainWindowViewModel()
      {
         _palette = new Palette();
         _tileSet = new TileSet( Constants.TileTexturesFilePath, _palette );
         _playerSpriteSheet = new SpriteSheet( Constants.PlayerSpriteFilePath, _palette );

         for ( int i = 0; i < Constants.TileCount; i++ )
         {
            var image = new BitmapImage();

            using var stream = new MemoryStream();
            {
               var encoder = new PngBitmapEncoder();
               encoder.Frames.Add( BitmapFrame.Create( _tileSet.TileBitmaps[i] ) );
               encoder.Save( stream );
               image.BeginInit();
               image.CacheOption = BitmapCacheOption.OnLoad;
               image.StreamSource = stream;
               image.EndInit();
               image.Freeze();
            }

            TileSelectionViewModels.Add( new( _tileSet, i ) );
         }

         for ( int i = 0; i < Constants.TileMapTileCount; i++ )
         {
            TileViewModels.Add( new( _tileSet, 0 ) );
         }

         if ( !TileMapFileOps.LoadTileMap( Constants.TileMapSaveFilePath, TileViewModels ) )
         {
            MessageBox.Show( "Could not load tile map save file!" );
         }
      }

      private void SaveTileMap()
      {
         TileMapFileOps.SaveTileMap( Constants.TileMapSaveFilePath, TileViewModels );
         MessageBox.Show( "Tile map has been saved!" );
      }

      private void WriteFile()
      {
         var writer = new DataSourceCodeWriter( _palette, _tileSet, TileViewModels, _playerSpriteSheet );
         writer.WriteFile( Constants.DataSourceFilePath );
         MessageBox.Show( "Data file has been written!" );
      }

      private ICommand? _saveTileMapCommand;
      public ICommand? SaveTileMapCommand => _saveTileMapCommand ?? ( _saveTileMapCommand = new RelayCommand( SaveTileMap, () => true ) );

      private ICommand? _writeFileCommand;
      public ICommand? WriteFileCommand => _writeFileCommand ?? ( _writeFileCommand = new RelayCommand( WriteFile, () => true ) );
   }
}
