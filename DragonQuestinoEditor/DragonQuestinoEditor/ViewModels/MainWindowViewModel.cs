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
      private readonly List<TileMapViewModel> _tileMaps;

      public ObservableCollection<TileViewModel> TileSelectionViewModels { get; } = [];

      private TileMapViewModel? _selectedTileMap;
      public TileMapViewModel? SelectedTileMap
      {
         get => _selectedTileMap;
         set => SetProperty( ref _selectedTileMap, value );
      }

      public MainWindowViewModel()
      {
         _palette = new Palette();
         _tileSet = new TileSet( Constants.TileTexturesFilePath, _palette );
         _playerSpriteSheet = new SpriteSheet( Constants.PlayerSpriteFilePath, _palette );
         _tileMaps = [];

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

         _tileMaps.Add( new( "Overworld" ) );

         for ( int i = 0; i < Constants.TileMapTileCount; i++ )
         {
            _tileMaps[0].Tiles.Add( new( _tileSet, 0 ) );
         }

         if ( !TileMapFileOps.LoadTileMap( Constants.TileMapSaveFilePath, _tileMaps[0].Tiles ) )
         {
            MessageBox.Show( "Could not load tile map save file!" );
         }

         SelectedTileMap = _tileMaps[0];
      }

      private void SaveTileMap()
      {
         TileMapFileOps.SaveTileMap( Constants.TileMapSaveFilePath, _tileMaps[0].Tiles );
         MessageBox.Show( "Tile map has been saved!" );
      }

      private void NewTileMap()
      {
         // TODO
      }

      private void DeleteTileMap()
      {
         // TODO
      }

      private void WriteGameData()
      {
         var writer = new DataSourceCodeWriter( _palette, _tileSet, _tileMaps[0].Tiles, _playerSpriteSheet );
         writer.WriteFile( Constants.DataSourceFilePath );
         MessageBox.Show( "Data file has been written!" );
      }

      private ICommand? _newTileMapCommand;
      public ICommand? NewTileMapCommand => _newTileMapCommand ??= new RelayCommand( NewTileMap, () => true );

      private ICommand? _deleteTileMapCommand;
      public ICommand? DeleteTileMapCommand => _deleteTileMapCommand ??= new RelayCommand( DeleteTileMap, () => true );

      private ICommand? _saveTileMapCommand;
      public ICommand? SaveTileMapCommand => _saveTileMapCommand ??= new RelayCommand( SaveTileMap, () => true ) ;

      private ICommand? _writeGameDataCommand;
      public ICommand? WriteGameDataCommand => _writeGameDataCommand ??= new RelayCommand( WriteGameData, () => true ) ;
   }
}
