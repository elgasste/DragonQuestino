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
               if ( _selectedTileMap is not null )
               {
                  foreach ( var tile in _selectedTileMap.Tiles )
                  {
                     tile.IsSelected = false;
                  }
               }

               SelectedTile = null;

               OnPropertyChanged( nameof( TileMapTextureListViewWidth ) );
               OnPropertyChanged( nameof( TileMapTextureListViewHeight ) );
            }
         }
      }

      private TileViewModel? _selectedTile;
      public TileViewModel? SelectedTile
      {
         get => _selectedTile;
         set
         {
            SetProperty( ref _selectedTile, value );
            OnPropertyChanged( nameof( TileIsSelected ) );
         }
      }

      private int _selectedTileIndex = -1;

      public bool TileIsSelected => SelectedTile != null;

      public int TileMapTextureListViewWidth => _selectedTileMap is null
         ? Constants.TileMapTextureListViewMaxWidth
         : Math.Min( Constants.TileMapTextureListViewMaxWidth, _selectedTileMap.TilesX * Constants.TileMapListViewItemSize );

      public int TileMapTextureListViewHeight => _selectedTileMap is null
         ? Constants.TileMapTextureListViewMaxHeight
         : Math.Min( Constants.TileMapTextureListViewMaxHeight, _selectedTileMap.TilesY * Constants.TileMapListViewItemSize );

      public int TileMapPortalListViewWidth => _selectedTileMap is null
         ? Constants.TileMapPortalListViewMaxWidth
         : Math.Min( Constants.TileMapPortalListViewMaxWidth, _selectedTileMap.TilesX * Constants.TileMapListViewItemSize );

      public int TileMapPortalListViewHeight => _selectedTileMap is null
         ? Constants.TileMapPortalListViewMaxHeight
         : Math.Min( Constants.TileMapPortalListViewMaxHeight, _selectedTileMap.TilesY * Constants.TileMapListViewItemSize );

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

      public void SelectTile( int tileIndex )
      {
         if ( SelectedTileMap is not null )
         {
            foreach ( var tile in SelectedTileMap.Tiles )
            {
               tile.IsSelected = false;
            }

            var selectedTile = SelectedTileMap.Tiles[tileIndex];
            selectedTile.IsSelected = true;
            SelectedTile = selectedTile;
            _selectedTileIndex = tileIndex;
         }
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

      private void AddPortal()
      {
         if ( SelectedTileMap is not null && SelectedTile is not null && !SelectedTile.HasPortal )
         {
            // TODO: show a window where we can select the destination info
            int destinationTileMapIndex = 7;  // Garinham
            int destinationTileIndex = 331;
            Direction arrivalDirection = Direction.Right;

            SelectedTile.Portal = new( _selectedTileIndex, destinationTileMapIndex, destinationTileIndex, arrivalDirection );
            SelectedTileMap.Portals.Add( SelectedTile.Portal );
         }         
      }

      private void DeleteSelectedPortal()
      {
         if ( SelectedTileMap is not null && SelectedTile?.Portal is not null )
         {
            SelectedTileMap.Portals.Remove( SelectedTile.Portal );
            SelectedTile.Portal = null;
         }
      }

      private ICommand? _newTileMapCommand;
      public ICommand? NewTileMapCommand => _newTileMapCommand ??= new RelayCommand( NewTileMap, () => true );

      private ICommand? _deleteTileMapCommand;
      public ICommand? DeleteTileMapCommand => _deleteTileMapCommand ??= new RelayCommand( DeleteTileMap, () => true );

      private ICommand? _saveTileMapsCommand;
      public ICommand? SaveTileMapsCommand => _saveTileMapsCommand ??= new RelayCommand( SaveTileMaps, () => true ) ;

      private ICommand? _writeGameDataCommand;
      public ICommand? WriteGameDataCommand => _writeGameDataCommand ??= new RelayCommand( WriteGameData, () => true );

      private ICommand? _addPortalCommand;
      public ICommand? AddPortalCommand => _addPortalCommand ??= new RelayCommand( AddPortal, () => true );

      private ICommand? _deleteSelectedPortalCommand;
      public ICommand? DeleteSelectedPortalCommand => _deleteSelectedPortalCommand ??= new RelayCommand( DeleteSelectedPortal, () => true );
   }
}
