using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using DragonQuestinoEditor.Commands;
using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Tiles;

namespace DragonQuestinoEditor.ViewModels
{
   internal class MainWindowViewModel : ViewModelBase
   {
      private readonly TileSet _tileSet;
      private readonly SpriteSheet _playerSpriteSheet;

      public ObservableCollection<TileViewModel> TileSelectionViewModels { get; } = [];
      public ObservableCollection<TileViewModel> TileMapViewModels { get; } = [];

      public MainWindowViewModel()
      {
         _tileSet = new TileSet( Constants.TileTexturesFilePath );
         _playerSpriteSheet = new SpriteSheet( Constants.PlayerSpriteFilePath );

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
            TileMapViewModels.Add( new( _tileSet, 0 ) );
         }

         if ( !TileMapFileOps.LoadTileMap( Constants.TileMapSaveFilePath, TileMapViewModels ) )
         {
            MessageBox.Show( "Could not load tile map save file!" );
         }
      }

      private void SaveTileMap()
      {
         TileMapFileOps.SaveTileMap( Constants.TileMapSaveFilePath, TileMapViewModels );
         MessageBox.Show( "Tile map has been saved!" );
      }

      private void WriteFile()
      {
         var writer = new DataSourceCodeWriter( _tileSet, TileMapViewModels );
         writer.WriteFile( Constants.DataSourceFilePath );
         MessageBox.Show( "Data file has been written!" );
      }

      private ICommand? _saveTileMapCommand;
      public ICommand? SaveTileMapCommand => _saveTileMapCommand ?? ( _saveTileMapCommand = new RelayCommand( SaveTileMap, () => true ) );

      private ICommand? _writeFileCommand;
      public ICommand? WriteFileCommand => _writeFileCommand ?? ( _writeFileCommand = new RelayCommand( WriteFile, () => true ) );
   }
}
