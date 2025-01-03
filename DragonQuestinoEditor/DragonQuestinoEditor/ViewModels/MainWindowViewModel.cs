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

      public ObservableCollection<TileMapViewModel> TileMaps { get; } = [];

      public ObservableCollection<TileTextureViewModel> TileTextureViewModels { get; } = [];

      private TileMapViewModel? _selectedTileMap;
      public TileMapViewModel? SelectedTileMap
      {
         get => _selectedTileMap;
         set
         {
            if ( SetProperty( ref _selectedTileMap, value ) )
            {
               OnPropertyChanged( nameof( TileMapListViewWidth ) );
               OnPropertyChanged( nameof( TileMapListViewHeight ) );
            }
         }
      }

      public int TileMapListViewWidth => _selectedTileMap == null
         ? Constants.TileMapListViewMaxWidth
         : Math.Min( Constants.TileMapListViewMaxWidth, _selectedTileMap.TilesX * Constants.TileMapListViewItemSize );

      public int TileMapListViewHeight => _selectedTileMap == null
         ? Constants.TileMapListViewMaxHeight
         : Math.Min( Constants.TileMapListViewMaxHeight, _selectedTileMap.TilesY * Constants.TileMapListViewItemSize );
      
      public MainWindowViewModel()
      {
         _palette = new Palette();
         _tileSet = new TileSet( Constants.TileTexturesFilePath, _palette );
         _playerSpriteSheet = new SpriteSheet( Constants.PlayerSpriteFilePath, _palette );

         for ( int i = 0; i < Constants.TileTextureCount; i++ )
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

            TileTextureViewModels.Add( new( _tileSet, i ) );
         }

         if ( !SaveDataFileOps.LoadData( Constants.EditorSaveDataFilePath, TileMaps ) )
         {
            MessageBox.Show( "Could not load editor save file!" );
         }

         foreach ( var tileMap in TileMaps )
         {
            foreach( var tile in tileMap.Tiles )
            {
               tile.SetTileTextureProvider( _tileSet );
            }
         }

         SelectedTileMap = TileMaps[0];
      }

      private void SaveTileMaps()
      {
         SaveDataFileOps.SaveData( Constants.EditorSaveDataFilePath, TileMaps );
         MessageBox.Show( "Tile maps have been saved!" );
      }

      private void NewTileMap()
      {
         var window = new CreateTileMapWindow( TileMaps.Select( m => m.Name ) );
         var result = window.ShowDialog();

         if ( result.HasValue && result.Value )
         {
            int id = TileMaps[^1].Id + 1;
            var newTileMap = new TileMapViewModel( id, window.NewTileMapName, window.NewTilesX, window.NewTilesY, Constants.TileTextureDefaultIndex );

            foreach ( var tile in newTileMap.Tiles )
            {
               tile.SetTileTextureProvider( _tileSet );
            }

            TileMaps.Add( newTileMap );
            SelectedTileMap = newTileMap;
         }
      }

      private void DeleteTileMap()
      {
         // TODO
      }

      private void WriteGameData()
      {
         var writer = new DataSourceCodeWriter( _palette, _tileSet, TileMaps, _playerSpriteSheet );
         writer.WriteFile( Constants.DataSourceFilePath );
         MessageBox.Show( "Game data file has been written!" );
      }

      private ICommand? _newTileMapCommand;
      public ICommand? NewTileMapCommand => _newTileMapCommand ??= new RelayCommand( NewTileMap, () => true );

      private ICommand? _deleteTileMapCommand;
      public ICommand? DeleteTileMapCommand => _deleteTileMapCommand ??= new RelayCommand( DeleteTileMap, () => true );

      private ICommand? _saveTileMapsCommand;
      public ICommand? SaveTileMapsCommand => _saveTileMapsCommand ??= new RelayCommand( SaveTileMaps, () => true ) ;

      private ICommand? _writeGameDataCommand;
      public ICommand? WriteGameDataCommand => _writeGameDataCommand ??= new RelayCommand( WriteGameData, () => true ) ;
   }
}
