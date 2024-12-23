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

      public ObservableCollection<TileTextureViewModel> TileTextureViewModels { get; } = [];

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

         if ( !SaveDataFileOps.LoadData( Constants.EditorSaveDataFilePath, _tileMaps ) )
         {
            MessageBox.Show( "Could not load editor save file!" );
         }

         foreach ( var tileMap in _tileMaps )
         {
            foreach( var tile in tileMap.Tiles )
            {
               tile.SetTileTextureProvider( _tileSet );
            }
         }

         SelectedTileMap = _tileMaps[0];
      }

      private void SaveTileMap()
      {
         SaveDataFileOps.SaveData( Constants.EditorSaveDataFilePath, _tileMaps );
         MessageBox.Show( "Editor data has been saved!" );
      }

      private void NewTileMap()
      {
         var window = new CreateTileMapWindow( _tileMaps.Select( m => m.Name ) );
         var result = window.ShowDialog();

         if ( result.HasValue && result.Value )
         {
            // TODO: add a new TileMapViewModel, fill out the textures with a default value, and set SelectedTileMap
         }
      }

      private void DeleteTileMap()
      {
         // TODO
      }

      private void WriteGameData()
      {
         var writer = new DataSourceCodeWriter( _palette, _tileSet, _tileMaps, _playerSpriteSheet );
         writer.WriteFile( Constants.DataSourceFilePath );
         MessageBox.Show( "Game data file has been written!" );
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
