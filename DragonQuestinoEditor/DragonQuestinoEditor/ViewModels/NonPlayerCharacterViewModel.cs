﻿using DragonQuestinoEditor.FileOps;

namespace DragonQuestinoEditor.ViewModels
{
   public class NonPlayerCharacterViewModel : ViewModelBase
   {
      private int _id;
      public int Id
      {
         get => _id;
         set => SetProperty( ref _id, value );
      }

      private int _tileIndex;
      public int TileIndex
      {
         get => _tileIndex;
         set => SetProperty( ref _tileIndex, value );
      }

      private ActiveSpriteViewModel? _activeSprite;
      public ActiveSpriteViewModel? ActiveSprite
      {
         get => _activeSprite;
         set => SetProperty( ref _activeSprite, value );
      }

      private Direction _direction;
      public Direction Direction
      {
         get => _direction;
         set => SetProperty( ref _direction, value );
      }

      private bool _wanders;
      public bool Wanders
      {
         get => _wanders;
         set => SetProperty( ref _wanders, value );
      }

      private SimpleRect? _wanderBounds;
      public SimpleRect? WanderBounds
      {
         get => _wanderBounds;
         set => SetProperty( ref _wanderBounds, value );
      }

      public NonPlayerCharacterViewModel( int id,
                                          int tileIndex,
                                          ActiveSpriteViewModel activeSprite,
                                          Direction direction,
                                          bool wanders,
                                          SimpleRect wanderBounds )
      {
         Id = id;
         TileIndex = tileIndex;
         ActiveSprite = activeSprite;
         Direction = direction;
         Wanders = wanders;
         WanderBounds = wanderBounds;
      }

      public NonPlayerCharacterViewModel( NonPlayerCharacterSaveData saveData )
      {
         Id = saveData.Id;
         TileIndex = saveData.TileIndex;
         Direction = saveData.Direction;
         Wanders = saveData.Wanders;
         WanderBounds = saveData.WanderBounds;

         if ( saveData.ActiveSprite != null )
         {
            ActiveSprite = new( saveData.ActiveSprite );
         }
      }
   }
}
