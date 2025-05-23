﻿using DragonQuestinoEditor.FileOps;
using DragonQuestinoEditor.Graphics;
using System.Windows.Media.Imaging;

namespace DragonQuestinoEditor.ViewModels
{
   public class TileViewModel : ViewModelBase
   {
      private ITileTextureProvider? _textureProvider = null;

      // TODO: allow updating this with the tile editor
      private readonly int[] _passableTextureIndexes = [
         0,    // grass
         1,    // trees
         2,    // hills
         3,    // desert
         4,    // swamp
         7,    // brick path
         9,    // barrier
         13    // bridge
      ];

      public TileSet TileSet { get; }

      private int _id = 0;
      public int Id
      {
         get => _id;
         set => SetProperty( ref _id, value );
      }

      private int _textureIndex = -1;
      public int TextureIndex
      {
         get => _textureIndex;
         set
         {
            if ( SetProperty( ref _textureIndex, value ) )
            {
               Image = TileSet.TileBitmaps[value];
               IsPassable = _passableTextureIndexes.Contains( value );
            }
         }
      }

      private int _treasureFlag = 0;
      public int TreasureFlag
      {
         get => _treasureFlag;
         set => SetProperty( ref _treasureFlag, value );
      }

      private int _doorFlag = 0;
      public int DoorFlag
      {
         get => _doorFlag;
         set => SetProperty( ref _doorFlag, value );
      }

      private BitmapSource? _image;
      public BitmapSource? Image
      {
         get => _image;
         private set => SetProperty( ref _image, value );
      }

      private bool _isPassable;
      public bool IsPassable
      {
         get => _isPassable;
         private set => SetProperty( ref _isPassable, value );
      }

      private bool _isDraggingTextureOver = false;
      public bool IsDraggingTextureOver
      {
         get => _isDraggingTextureOver;
         set => SetProperty( ref _isDraggingTextureOver, value );
      }

      private bool _isSelected = false;
      public bool IsSelected
      {
         get => _isSelected;
         set => SetProperty( ref _isSelected, value );
      }

      private bool _isPortalDestination = false;
      public bool IsPortalDestination
      {
         get => _isPortalDestination;
         set => SetProperty( ref _isPortalDestination, value );
      }

      private int _enemyPoolIndex = 0;
      public int EnemyPoolIndex
      {
         get => _enemyPoolIndex;
         set => SetProperty( ref _enemyPoolIndex, value );
      }

      private TilePortalViewModel? _portal = null;
      public TilePortalViewModel? Portal
      {
         get => _portal;
         set
         {
            SetProperty( ref _portal, value );
            OnPropertyChanged( nameof( HasPortal ) );
         }
      }

      public bool HasPortal => _portal is not null;

      public TileViewModel( TileSet tileSet, int id, int textureIndex )
      {
         Id = id;
         TileSet = tileSet;
         TextureIndex = textureIndex;
         _isPassable = _passableTextureIndexes.Contains( textureIndex );
      }

      public TileViewModel( TileSet tileSet, int id, TileSaveData saveData )
      {
         TileSet = tileSet;
         Id = id;
         TextureIndex = saveData.TextureIndex;
         TreasureFlag = saveData.TreasureFlag;
         DoorFlag = saveData.DoorFlag;
         _isPassable = saveData.IsPassable;
         _enemyPoolIndex = saveData.EnemyPoolIndex;
      }

      public void SetTextureProvider( ITileTextureProvider? provider ) => _textureProvider = provider;
   }
}
