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

      private bool _playerIsCursed = false;
      public bool PlayerIsCursed
      {
         get => _playerIsCursed;
         set
         {
            if ( SetProperty( ref _playerIsCursed, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _visitedTantegel = false;
      public bool VisitedTantegel
      {
         get => _visitedTantegel;
         set
         {
            if ( SetProperty( ref _visitedTantegel, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _visitedBrecconary = false;
      public bool VisitedBrecconary
      {
         get => _visitedBrecconary;
         set
         {
            if ( SetProperty( ref _visitedBrecconary, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _visitedGarinham = false;
      public bool VisitedGarinham
      {
         get => _visitedGarinham;
         set
         {
            if ( SetProperty( ref _visitedGarinham, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _visitedKol = false;
      public bool VisitedKol
      {
         get => _visitedKol;
         set
         {
            if ( SetProperty( ref _visitedKol, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _visitedCantlin = false;
      public bool VisitedCantlin
      {
         get => _visitedCantlin;
         set
         {
            if ( SetProperty( ref _visitedCantlin, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _visitedRimuldar = false;
      public bool VisitedRimuldar
      {
         get => _visitedRimuldar;
         set
         {
            if ( SetProperty( ref _visitedRimuldar, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _defeatedGreenDragon = false;
      public bool DefeatedGreenDragon
      {
         get => _defeatedGreenDragon;
         set
         {
            if ( SetProperty( ref _defeatedGreenDragon, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _defeatedAxeKnight = false;
      public bool DefeatedAxeKnight
      {
         get => _defeatedAxeKnight;
         set
         {
            if ( SetProperty( ref _defeatedAxeKnight, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _defeatedGolem = false;
      public bool DefeatedGolem
      {
         get => _defeatedGolem;
         set
         {
            if ( SetProperty( ref _defeatedGolem, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _rescuedGwaelin = false;
      public bool RescuedGwaelin
      {
         get => _rescuedGwaelin;
         set
         {
            if ( SetProperty( ref _rescuedGwaelin, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _gotStaffOfRain = false;
      public bool GotStaffOfRain
      {
         get => _gotStaffOfRain;
         set
         {
            if ( SetProperty( ref _gotStaffOfRain, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _gotRainbowDrop = false;
      public bool GotRainbowDrop
      {
         get => _gotRainbowDrop;
         set
         {
            if ( SetProperty( ref _gotRainbowDrop, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _usedRainbowDrop = false;
      public bool UsedRainbowDrop
      {
         get => _usedRainbowDrop;
         set
         {
            if ( SetProperty( ref _usedRainbowDrop, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _foundHiddenStairs = false;
      public bool FoundHiddenStairs
      {
         get => _foundHiddenStairs;
         set
         {
            if ( SetProperty( ref _foundHiddenStairs, value ) )
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

      private bool _hasDragonScale = false;
      public bool HasDragonScale
      {
         get => _hasDragonScale;
         set
         {
            if ( SetProperty( ref _hasDragonScale, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasCursedBelt = false;
      public bool HasCursedBelt
      {
         get => _hasCursedBelt;
         set
         {
            if ( SetProperty( ref _hasCursedBelt, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasFairyFlute = false;
      public bool HasFairyFlute
      {
         get => _hasFairyFlute;
         set
         {
            if ( SetProperty( ref _hasFairyFlute, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasSilverHarp = false;
      public bool HasSilverHarp
      {
         get => _hasSilverHarp;
         set
         {
            if ( SetProperty( ref _hasSilverHarp, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasGwaelinsLove = false;
      public bool HasGwaelinsLove
      {
         get => _hasGwaelinsLove;
         set
         {
            if ( SetProperty( ref _hasGwaelinsLove, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasStoneOfSunlight = false;
      public bool HasStoneOfSunlight
      {
         get => _hasStoneOfSunlight;
         set
         {
            if ( SetProperty( ref _hasStoneOfSunlight, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasStaffOfRain = false;
      public bool HasStaffOfRain
      {
         get => _hasStaffOfRain;
         set
         {
            if ( SetProperty( ref _hasStaffOfRain, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasErdricksToken = false;
      public bool HasErdricksToken
      {
         get => _hasErdricksToken;
         set
         {
            if ( SetProperty( ref _hasErdricksToken, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _hasRainbowDrop = false;
      public bool HasRainbowDrop
      {
         get => _hasRainbowDrop;
         set
         {
            if ( SetProperty( ref _hasRainbowDrop, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor1 = false;
      public bool OpenedDoor1
      {
         get => _openedDoor1;
         set
         {
            if ( SetProperty( ref _openedDoor1, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor2 = false;
      public bool OpenedDoor2
      {
         get => _openedDoor2;
         set
         {
            if ( SetProperty( ref _openedDoor2, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor3 = false;
      public bool OpenedDoor3
      {
         get => _openedDoor3;
         set
         {
            if ( SetProperty( ref _openedDoor3, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor4 = false;
      public bool OpenedDoor4
      {
         get => _openedDoor4;
         set
         {
            if ( SetProperty( ref _openedDoor4, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor5 = false;
      public bool OpenedDoor5
      {
         get => _openedDoor5;
         set
         {
            if ( SetProperty( ref _openedDoor5, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor6 = false;
      public bool OpenedDoor6
      {
         get => _openedDoor6;
         set
         {
            if ( SetProperty( ref _openedDoor6, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor7 = false;
      public bool OpenedDoor7
      {
         get => _openedDoor7;
         set
         {
            if ( SetProperty( ref _openedDoor7, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor8 = false;
      public bool OpenedDoor8
      {
         get => _openedDoor8;
         set
         {
            if ( SetProperty( ref _openedDoor8, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _openedDoor9 = false;
      public bool OpenedDoor9
      {
         get => _openedDoor9;
         set
         {
            if ( SetProperty( ref _openedDoor9, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected1 = false;
      public bool TreasureCollected1
      {
         get => _treasureCollected1;
         set
         {
            if ( SetProperty( ref _treasureCollected1, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected2 = false;
      public bool TreasureCollected2
      {
         get => _treasureCollected2;
         set
         {
            if ( SetProperty( ref _treasureCollected2, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected3 = false;
      public bool TreasureCollected3
      {
         get => _treasureCollected3;
         set
         {
            if ( SetProperty( ref _treasureCollected3, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected4 = false;
      public bool TreasureCollected4
      {
         get => _treasureCollected4;
         set
         {
            if ( SetProperty( ref _treasureCollected4, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected5 = false;
      public bool TreasureCollected5
      {
         get => _treasureCollected5;
         set
         {
            if ( SetProperty( ref _treasureCollected5, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected6 = false;
      public bool TreasureCollected6
      {
         get => _treasureCollected6;
         set
         {
            if ( SetProperty( ref _treasureCollected6, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected7 = false;
      public bool TreasureCollected7
      {
         get => _treasureCollected7;
         set
         {
            if ( SetProperty( ref _treasureCollected7, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected8 = false;
      public bool TreasureCollected8
      {
         get => _treasureCollected8;
         set
         {
            if ( SetProperty( ref _treasureCollected8, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected9 = false;
      public bool TreasureCollected9
      {
         get => _treasureCollected9;
         set
         {
            if ( SetProperty( ref _treasureCollected9, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected10 = false;
      public bool TreasureCollected10
      {
         get => _treasureCollected10;
         set
         {
            if ( SetProperty( ref _treasureCollected10, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected11 = false;
      public bool TreasureCollected11
      {
         get => _treasureCollected11;
         set
         {
            if ( SetProperty( ref _treasureCollected11, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected12 = false;
      public bool TreasureCollected12
      {
         get => _treasureCollected12;
         set
         {
            if ( SetProperty( ref _treasureCollected12, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected13 = false;
      public bool TreasureCollected13
      {
         get => _treasureCollected13;
         set
         {
            if ( SetProperty( ref _treasureCollected13, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected14 = false;
      public bool TreasureCollected14
      {
         get => _treasureCollected14;
         set
         {
            if ( SetProperty( ref _treasureCollected14, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected15 = false;
      public bool TreasureCollected15
      {
         get => _treasureCollected15;
         set
         {
            if ( SetProperty( ref _treasureCollected15, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected16 = false;
      public bool TreasureCollected16
      {
         get => _treasureCollected16;
         set
         {
            if ( SetProperty( ref _treasureCollected16, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected17 = false;
      public bool TreasureCollected17
      {
         get => _treasureCollected17;
         set
         {
            if ( SetProperty( ref _treasureCollected17, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected18 = false;
      public bool TreasureCollected18
      {
         get => _treasureCollected18;
         set
         {
            if ( SetProperty( ref _treasureCollected18, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected19 = false;
      public bool TreasureCollected19
      {
         get => _treasureCollected19;
         set
         {
            if ( SetProperty( ref _treasureCollected19, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected20 = false;
      public bool TreasureCollected20
      {
         get => _treasureCollected20;
         set
         {
            if ( SetProperty( ref _treasureCollected20, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected21 = false;
      public bool TreasureCollected21
      {
         get => _treasureCollected21;
         set
         {
            if ( SetProperty( ref _treasureCollected21, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected22 = false;
      public bool TreasureCollected22
      {
         get => _treasureCollected22;
         set
         {
            if ( SetProperty( ref _treasureCollected22, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected23 = false;
      public bool TreasureCollected23
      {
         get => _treasureCollected23;
         set
         {
            if ( SetProperty( ref _treasureCollected23, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected24 = false;
      public bool TreasureCollected24
      {
         get => _treasureCollected24;
         set
         {
            if ( SetProperty( ref _treasureCollected24, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected25 = false;
      public bool TreasureCollected25
      {
         get => _treasureCollected25;
         set
         {
            if ( SetProperty( ref _treasureCollected25, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected26 = false;
      public bool TreasureCollected26
      {
         get => _treasureCollected26;
         set
         {
            if ( SetProperty( ref _treasureCollected26, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected27 = false;
      public bool TreasureCollected27
      {
         get => _treasureCollected27;
         set
         {
            if ( SetProperty( ref _treasureCollected27, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected28 = false;
      public bool TreasureCollected28
      {
         get => _treasureCollected28;
         set
         {
            if ( SetProperty( ref _treasureCollected28, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected29 = false;
      public bool TreasureCollected29
      {
         get => _treasureCollected29;
         set
         {
            if ( SetProperty( ref _treasureCollected29, value ) )
            {
               UpdatePassword();
            }
         }
      }

      private bool _treasureCollected30 = false;
      public bool TreasureCollected30
      {
         get => _treasureCollected30;
         set
         {
            if ( SetProperty( ref _treasureCollected30, value ) )
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
