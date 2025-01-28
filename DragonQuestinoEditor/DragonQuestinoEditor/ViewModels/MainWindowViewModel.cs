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
   public class MainWindowViewModel : ViewModelBase
   {
      private readonly Palette _palette;
      private readonly TileSet _tileSet;
      private readonly ActiveSpriteSheet _playerSpriteSheet;
      private readonly StaticSpriteSheet _staticSpriteSheet;

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
               OnPropertyChanged( nameof( TileMapPortalListViewWidth ) );
               OnPropertyChanged( nameof( TileMapPortalListViewHeight ) );
            }
         }
      }

      private TileMapViewModel? _portalDestinationTileMap;
      public TileMapViewModel? PortalDestinationTileMap
      {
         get => _portalDestinationTileMap;
         set
         {
            SetProperty( ref _portalDestinationTileMap, value );
            OnPropertyChanged( nameof( PortalIsSelected ) );
            OnPropertyChanged( nameof( TileMapPortalDestinationListViewWidth ) );
            OnPropertyChanged( nameof( TileMapPortalDestinationListViewHeight ) );
         }
      }

      private TileViewModel? _selectedTile;
      public TileViewModel? SelectedTile
      {
         get => _selectedTile;
         set
         {
            if ( SetProperty( ref _selectedTile, value ) )
            {
               OnPropertyChanged( nameof( TileIsSelected ) );

               if ( _selectedTile is not null )
               {
                  if ( SelectedTileMap is not null )
                  {
                     foreach ( var tile in SelectedTileMap.Tiles )
                     {
                        tile.IsPortalDestination = false;
                     }
                  }

                  if ( _selectedTile.Portal is not null && SelectedTileMap is not null && SelectedTileMap.Portals.Contains( _selectedTile.Portal ) )
                  {
                     foreach ( var portal in SelectedTileMap.Portals )
                     {
                        if ( portal == _selectedTile.Portal )
                        {
                           PortalDestinationTileMap = TileMaps[portal.DestinationTileMapIndex];
                           PortalDestinationTileMap.Tiles[portal.DestinationTileIndex].IsPortalDestination = true;
                           SelectedPortalArrivalDirection = portal.ArrivalDirection.ToString();
                           break;
                        }
                     }
                  }
                  else
                  {
                     PortalDestinationTileMap = null;
                  }
               }
               else
               {
                  PortalDestinationTileMap = null;
               }
            }
         }
      }

      public static ObservableCollection<string> PortalArrivalDirections => [ "Left", "Up", "Right", "Down" ];

      private string? _selectedPortalArrivalDirection;
      public string? SelectedPortalArrivalDirection
      {
         get => _selectedPortalArrivalDirection;
         set
         {
            if ( SetProperty( ref _selectedPortalArrivalDirection, value ) )
            {
               if ( SelectedTile?.Portal is not null && _selectedPortalArrivalDirection is not null )
               {
                  SelectedTile.Portal.ArrivalDirection = Enum.Parse<Direction>( _selectedPortalArrivalDirection );
               }
            }
         }
      }

      private int _selectedTileIndex = -1;

      public bool TileIsSelected => SelectedTile != null;
      public bool PortalIsSelected => PortalDestinationTileMap != null;

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

      public int TileMapPortalDestinationListViewWidth => _portalDestinationTileMap is null
         ? Constants.TileMapPortalListViewMaxWidth
         : Math.Min(Constants.TileMapPortalListViewMaxWidth, _portalDestinationTileMap.TilesX* Constants.TileMapListViewItemSize );

      public int TileMapPortalDestinationListViewHeight => _portalDestinationTileMap is null
         ? Constants.TileMapPortalListViewMaxHeight
         : Math.Min( Constants.TileMapPortalListViewMaxHeight, _portalDestinationTileMap.TilesY * Constants.TileMapListViewItemSize );

      public MainWindowViewModel()
      {
         _palette = new Palette();
         _tileSet = new TileSet( Constants.TileTexturesFilePath, _palette );
         _playerSpriteSheet = new ActiveSpriteSheet( Constants.PlayerSpriteFilePath, _palette );
         _staticSpriteSheet = new StaticSpriteSheet( Constants.StaticSpriteSheetFilePath, _palette );

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
               tile.SetTextureProvider( _tileSet );
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

            if ( PortalDestinationTileMap is not null )
            {
               foreach ( var tile in PortalDestinationTileMap.Tiles )
               {
                  tile.IsPortalDestination = false;
               }
            }

            if ( tileIndex >= 0 )
            {
               var selectedTile = SelectedTileMap.Tiles[tileIndex];
               selectedTile.IsSelected = true;
               SelectedTile = selectedTile;
            }
            else
            {
               SelectedTile = null;
            }

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
               tile.SetTextureProvider( _tileSet );
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
         var writer = new DataSourceCodeWriter( _palette, _tileSet, TileMaps, _playerSpriteSheet, _staticSpriteSheet );
         writer.WriteFile( Constants.DataSourceFilePath );
         MessageBox.Show( "Game data file has been written!" );
      }

      private void AddPortal()
      {
         // TODO: don't allow more than the maximum amount of portals for any given map
         if ( SelectedTileMap is not null && SelectedTile is not null && !SelectedTile.HasPortal )
         {
            var window = new CreateTilePortalWindow( TileMaps );
            var result = window.ShowDialog();

            if ( result.HasValue && result.Value )
            {
               SelectedTile.Portal = new( _selectedTileIndex, window.DestinationTileMapIndex, 0, window.ArrivalDirection );
               SelectedTileMap.Portals.Add( SelectedTile.Portal );
               PortalDestinationTileMap = TileMaps[window.DestinationTileMapIndex];
               SelectedPortalArrivalDirection = window.ArrivalDirection.ToString();

               foreach ( var tile in PortalDestinationTileMap.Tiles )
               {
                  tile.IsPortalDestination = false;
               }

               PortalDestinationTileMap.Tiles[0].IsPortalDestination = true;
            }
         }         
      }

      private void DeleteSelectedPortal()
      {
         if ( SelectedTileMap is not null && SelectedTile?.Portal is not null )
         {
            SelectedTileMap.Portals.Remove( SelectedTile.Portal );
            SelectedTile.Portal = null;

            if ( PortalDestinationTileMap is not null )
            {
               foreach ( var tile in PortalDestinationTileMap.Tiles )
               {
                  tile.IsPortalDestination = false;
               }
            }

            PortalDestinationTileMap = null;
         }
      }

      public void SelectPortalDestination( int tileIndex )
      {
         if ( tileIndex >= 0 && SelectedTile?.Portal is not null && PortalDestinationTileMap is not null )
         {
            foreach ( var tile in PortalDestinationTileMap.Tiles )
            {
               tile.IsPortalDestination = false;
            }

            SelectedTile.Portal.DestinationTileIndex = tileIndex;
            PortalDestinationTileMap.Tiles[tileIndex].IsPortalDestination = true;
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
