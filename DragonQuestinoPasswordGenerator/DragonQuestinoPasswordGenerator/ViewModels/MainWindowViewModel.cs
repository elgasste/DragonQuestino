namespace DragonQuestinoPasswordGenerator.ViewModels
{
   public class MainWindowViewModel : ViewModelBase
   {
      private string _playerName = string.Empty;
      public string PlayerName
      {
         get => _playerName;
         set
         {
            if ( SetProperty( ref _playerName, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private string _playerExperience = string.Empty;
      public string PlayerExperience
      {
         get => _playerExperience;
         set
         {
            if ( SetProperty( ref _playerExperience, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private string _playerGold = string.Empty;
      public string PlayerGold
      {
         get => _playerGold;
         set
         {
            if ( SetProperty( ref _playerGold, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private string _keyCount = string.Empty;
      public string KeyCount
      {
         get => _keyCount;
         set
         {
            if ( SetProperty( ref _keyCount, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private string _herbCount = string.Empty;
      public string HerbCount
      {
         get => _herbCount;
         set
         {
            if ( SetProperty( ref _herbCount, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private string _torchCount = string.Empty;
      public string TorchCount
      {
         get => _torchCount;
         set
         {
            if ( SetProperty( ref _torchCount, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private string _wingCount = string.Empty;
      public string WingCount
      {
         get => _wingCount;
         set
         {
            if ( SetProperty( ref _wingCount, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private string _fairyWaterCount = string.Empty;
      public string FairyWaterCount
      {
         get => _fairyWaterCount;
         set
         {
            if ( SetProperty( ref _fairyWaterCount, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private void UpdatePassword()
      {
         string name = ( _playerName == string.Empty ) ? " " : _playerName.Substring( 0, 8 );
         int experience = GetRangedNumberFromString( _playerExperience, UInt16.MaxValue );
         int gold = GetRangedNumberFromString( _playerGold, UInt16.MaxValue );
         int keys = GetRangedNumberFromString( _keyCount, 7 );
         int herbs = GetRangedNumberFromString( _keyCount, 7 );
         int torches = GetRangedNumberFromString( _torchCount, 7 );
         int wings = GetRangedNumberFromString( _wingCount, 7 );
         int fairyWaters = GetRangedNumberFromString( _fairyWaterCount, 7 );
      }

      private int GetRangedNumberFromString( string str, int max )
      {
         if ( int.TryParse( str, out int result ) )
         {
            return Math.Max( 0, Math.Min( max, result ) );
         }

         return 0;
      }
   }
}
