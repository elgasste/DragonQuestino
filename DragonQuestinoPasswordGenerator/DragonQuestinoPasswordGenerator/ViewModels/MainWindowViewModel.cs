using System.Collections.ObjectModel;
using System.Numerics;

namespace DragonQuestinoPasswordGenerator.ViewModels
{
   public class BattleItem( string displayText, int shift)
   {
      public string DisplayText { get; set; } = displayText;
      public int Shift { get; } = shift;
   }

   public class MainWindowViewModel : ViewModelBase
   {
      public ObservableCollection<BattleItem> Weapons { get; set; } =
      [
         new( "none", 0 ),
         new( "bamboo pole", 1 ),
         new( "club", 2 ),
         new( "copper sword", 3 ),
         new( "hand axe", 4 ),
         new( "broad sword", 5 ),
         new( "flame sword", 6 ),
         new( "Erdrick's Sword", 7 )
      ];

      private BattleItem _selectedWeapon;
      public BattleItem SelectedWeapon
      {
         get => _selectedWeapon;
         set
         {
            if ( SetProperty( ref _selectedWeapon, value ) )
            {
               UpdatePassword();
            }
         }
      }

      public ObservableCollection<BattleItem> Armor { get; set; } =
      [
         new( "none", 0 ),
         new( "clothes", 1 ),
         new( "leather armor", 2 ),
         new( "chain mail", 3 ),
         new( "half plate", 4 ),
         new( "full plate", 5 ),
         new( "Magic Armor", 6 ),
         new( "Erdrick's Armor", 7 )
      ];

      private BattleItem _selectedArmor;
      public BattleItem SelectedArmor
      {
         get => _selectedArmor;
         set
         {
            if ( SetProperty( ref _selectedArmor, value ) )
            {
               UpdatePassword();
            }
         }
      }

      public ObservableCollection<BattleItem> Shields { get; set; } =
      [
         new( "none", 0 ),
         new( "small shield", 1 ),
         new( "large shield", 2 ),
         new( "silver shield", 3 )
      ];

      private BattleItem _selectedShield;
      public BattleItem SelectedShield
      {
         get => _selectedShield;
         set
         {
            if ( SetProperty( ref _selectedShield, value ) )
            {
               UpdatePassword();
            }
         }
      }

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

      public MainWindowViewModel()
      {
         _selectedWeapon = Weapons[0];
         _selectedArmor = Armor[0];
         _selectedShield = Shields[0];
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
         int weaponShift = SelectedWeapon.Shift;
         int armorShift = SelectedArmor.Shift;
         int shieldShift = SelectedShield.Shift;
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
