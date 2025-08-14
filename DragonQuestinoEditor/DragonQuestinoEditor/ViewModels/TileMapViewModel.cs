using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;
using System.Collections.ObjectModel;

namespace DragonQuestinoEditor.ViewModels
{
   public class TileMapViewModel : ViewModelBase
   {
      public ObservableCollection<TileViewModel> Tiles { get; } = [];
      public ObservableCollection<StaticSpriteViewModel> StaticSprites { get; } = [];
      public ObservableCollection<NonPlayerCharacterViewModel> NonPlayerCharacters { get; } = [];
      public ObservableCollection<BoothViewModel> Booths { get; } = [];

      public ObservableCollection<TilePortalViewModel> Portals { get; } = [];

      public TilePortalViewModel EvacPortal { get; }

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

      private bool _hasEncounters;
      public bool HasEncounters
      {
         get => _hasEncounters;
         set => SetProperty( ref _hasEncounters, value );
      }

      private bool _blocksMagic;
      public bool BlocksMagic
      {
         get => _blocksMagic;
         set => SetProperty( ref _blocksMagic, value );
      }

      private bool _isDungeon;
      public bool IsDungeon
      {
         get => _isDungeon;
         set => SetProperty( ref _isDungeon, value );
      }

      private bool _isDark;
      public bool IsDark
      {
         get => _isDark;
         set => SetProperty( ref _isDark, value );
      }

      private int _innTileIndex;
      public int InnTileIndex
      {
         get => _innTileIndex;
         set => SetProperty( ref _innTileIndex, value );
      }

      private int _innPrice;
      public int InnPrice
      {
         get => _innPrice;
         set => SetProperty( ref _innPrice, value );
      }

      public TileMapViewModel( TileSet tileSet,
                               int id,
                               string? name,
                               bool hasEncounters,
                               bool blocksMagic,
                               bool isDungeon,
                               bool isDark,
                               int tilesX,
                               int tilesY,
                               int innTileIndex,
                               int innPrice,
                               int defaultTileTextureIndex )
      {
         _id = id;
         _name = name;
         _hasEncounters = hasEncounters;
         _blocksMagic = blocksMagic;
         _isDungeon = isDungeon;
         _isDark = isDark;
         _tilesX = tilesX;
         _tilesY = tilesY;
         _innTileIndex = innTileIndex;
         _innPrice = innPrice;
         EvacPortal = new( 0, 0, 0, Direction.Left );

         for ( int i = 0; i < tilesX * tilesY; i++ )
         {
            Tiles.Add( new TileViewModel( tileSet, i, defaultTileTextureIndex ) );
         }
      }

      public TileMapViewModel( TileSet tileSet, TileMapSaveData saveData )
      {
         _id = saveData.Id;
         _name = saveData.Name;
         _hasEncounters = saveData.HasEncounters;
         _blocksMagic = saveData.BlocksMagic;
         _isDungeon = saveData.IsDungeon;
         _isDark = saveData.IsDark;
         _tilesX = saveData.TilesX;
         _tilesY = saveData.TilesY;
         _innTileIndex = saveData.InnTileIndex;
         _innPrice = saveData.InnPrice;
         EvacPortal = new( saveData.EvacPortal );

         for ( int i = 0; i < saveData.Tiles.Count; i++ )
         {
            Tiles.Add( new( tileSet, i, saveData.Tiles[i] ) );
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

         foreach ( var npc in saveData.NonPlayerCharacters )
         {
            NonPlayerCharacters.Add( new( npc ) );
         }

         foreach ( var booth in saveData.Booths )
         {
            Booths.Add( new( booth ) );
         }
      }
   }
}
