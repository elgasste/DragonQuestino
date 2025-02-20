using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;
using System.Collections.ObjectModel;

namespace DragonQuestinoEditor.ViewModels
{
   public class TileMapViewModel : ViewModelBase
   {
      public ObservableCollection<TileViewModel> Tiles { get; } = [];
      public ObservableCollection<StaticSpriteViewModel> StaticSprites { get; } = [];

      public ObservableCollection<TilePortalViewModel> Portals { get; } = [];

      private int _tilesX;
      public int TilesX
      {
         get => _tilesX;
         set => SetProperty( ref _tilesX, value );
      }

      private int _tilesY;
      public int TilesY
      {
         get => _tilesY;
         set => SetProperty( ref _tilesY, value );
      }

      private int _id;
      public int Id
      {
         get => _id;
         set => SetProperty( ref _id, value );
      }

      private string? _name;
      public string? Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      private bool _isDark;
      public bool IsDark
      {
         get => _isDark;
         set => SetProperty( ref _isDark, value );
      }

      public TileMapViewModel( TileSet tileSet, int id, string? name, bool isDark, int tilesX, int tilesY, int defaultTileTextureIndex )
      {
         _id = id;
         _name = name;
         _isDark = isDark;
         _tilesX = tilesX;
         _tilesY = tilesY;

         for ( int i = 0; i < tilesX * tilesY; i++ )
         {
            Tiles.Add( new TileViewModel( tileSet, defaultTileTextureIndex ) );
         }
      }

      public TileMapViewModel( TileSet tileSet, TileMapSaveData saveData )
      {
         _id = saveData.Id;
         _name = saveData.Name;
         _isDark = saveData.IsDark;
         _tilesX = saveData.TilesX;
         _tilesY = saveData.TilesY;

         foreach ( var tileSaveData in saveData.Tiles )
         {
            Tiles.Add( new( tileSet, tileSaveData ) );
         }

         foreach ( var portal in saveData.Portals )
         {
            TilePortalViewModel p = new( portal );
            Tiles[portal.SourceTileIndex].Portal = p;
            Portals.Add( p );
         }

         foreach ( var staticSprite in saveData.StaticSprites )
         {
            StaticSprites.Add( new( staticSprite ) );
         }
      }
   }
}
