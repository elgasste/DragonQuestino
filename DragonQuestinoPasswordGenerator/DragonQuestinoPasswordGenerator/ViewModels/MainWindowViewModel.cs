using DragonQuestinoPasswordGenerator.Commands;
using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Input;

namespace DragonQuestinoPasswordGenerator.ViewModels
{
   public class BattleItem( string displayText, int flag)
   {
      public string DisplayText { get; set; } = displayText;
      public int Flag { get; } = flag;
   }

   public class MainWindowViewModel : ViewModelBase
   {
      private static readonly int[] _checksumTable =
      {
         11,
         29,
         7,
         5,
         16,
         4,
         9,
         28,
         19,
         15,
         13,
         12,
         8,
         6,
         0,
         3,
         24,
         10,
         23,
         25,
         31,
         30,
         22,
         1,
         27,
         21,
         18,
         26,
         14,
         20,
         17,
         2
      };

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

      private string _playerExperience = "0";
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

      private string _playerGold = "0";
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

      private string _keyCount = "0";
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

      private string _herbCount = "0";
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

      private string _torchCount = "0";
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

      private string _wingCount = "0";
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

      private string _fairyWaterCount = "0";
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

      private string _password = string.Empty;
      public string Password
      {
         get => _password;
         set => SetProperty( ref _password, value );
      }

      private ICommand? _enterPasswordCommand;
      public ICommand? EnterPasswordCommand => _enterPasswordCommand ??= new RelayCommand( EnterPassword, () => true );
      private ICommand? _clearAllFieldsCommand;
      public ICommand? ClearAllFieldsCommand => _clearAllFieldsCommand ??= new RelayCommand( ClearAllFields, () => true );

      public MainWindowViewModel()
      {
         _selectedWeapon = Weapons[0];
         _selectedArmor = Armor[0];
         _selectedShield = Shields[0];

         UpdatePassword();
      }

      private void UpdatePassword()
      {
         string name = ( _playerName == string.Empty ) ? " " : 
            _playerName.Length > 8 ? _playerName.Substring( 0, 8 ) : _playerName;
         UInt32 experience = (UInt32)GetRangedNumberFromString( _playerExperience, UInt16.MaxValue );
         UInt32 gold = (UInt32)GetRangedNumberFromString( _playerGold, UInt16.MaxValue );
         UInt32 weaponFlag = (UInt32)SelectedWeapon.Flag;
         UInt32 armorFlag = (UInt32)SelectedArmor.Flag;
         UInt32 shieldFlag = (UInt32)SelectedShield.Flag;
         UInt32 openedDoorFlags = GetOpenedDoorFlags();
         UInt32 treasureCollectedFlags = GetTreasureCollectedFlags();
         UInt32 itemFlags = GetItemFlags();
         UInt32 townVisitedFlags = GetTownVisitedFlags();
         UInt32 specialEnemyDefeatedFlags = GetSpecialEnemyDefeatedFlags();
         UInt32[] encodedNameChars = new UInt32[8];

         for ( int i = 0; i < 8; i++ )
         {
            // pad unused chars with spaces
            encodedNameChars[i] = ( i < name.Length ) ? GetEncodedBitsFromChar( name[i] ) : (UInt32)0x3F;
         }

         List<UInt32> encodedBits = [0, 0, 0, 0, 0, 0];

         // 16 bits: doors that haven't been opened
         // 16 bits: player's experience
         encodedBits[0] = ( ( 0xFFFF ^ openedDoorFlags ) << 16 ) | experience;

         // 32 bits: treasures that haven't been collected
         encodedBits[1] = 0xFFFFFFFF ^ treasureCollectedFlags;

         // 25 bits: player's items
         // 6 bits: towns visited
         // 1 bit: reserved
         encodedBits[2] = ( itemFlags << 7 ) | ( townVisitedFlags << 1 ) | (UInt32)0x1;

         // 16 bits: gold
         // 3 bits: weapon
         // 3 bits: armor
         // 2 bits: shield
         // 3 bits: special enemies remaining
         // 1 bit: rescued Gwaelin
         // 1 bit: found hidden stairs
         // 1 bit: used Rainbow Drop
         // 1 bit: Got Staff of Rain
         // 1 bit: Got Rainbow Drop
         encodedBits[3] = ( gold << 16 )
                          | ( weaponFlag << 13 )
                          | ( armorFlag << 10 )
                          | ( shieldFlag << 8 )
                          | ( ( (UInt32)0x7 ^ specialEnemyDefeatedFlags ) << 5 )
                          | ( ( _rescuedGwaelin ? (UInt32)0x1 : 0 ) << 4 )
                          | ( ( _foundHiddenStairs ? (UInt32)0x1 : 0 ) << 3 )
                          | ( ( _usedRainbowDrop ? (UInt32)0x1 : 0 ) << 2 )
                          | ( ( _gotStaffOfRain ? (UInt32)0x1 : 0 ) << 1 )
                          | ( _gotRainbowDrop ? (UInt32)0x1 : 0 );

         // write out all 8 characters in 6-bit increments, followed by the length of the name
         encodedBits[4] = ( encodedNameChars[0] << 26 ) |
                          ( encodedNameChars[1] << 20 ) |
                          ( encodedNameChars[2] << 14 ) |
                          ( encodedNameChars[3] << 8 ) |
                          ( encodedNameChars[4] << 2 ) |
                          ( encodedNameChars[5] >> 4 );
         encodedBits[5] = ( ( encodedNameChars[5] & (UInt32)0xF ) << 28 ) |
                          ( encodedNameChars[6] << 22 ) |
                          ( encodedNameChars[7] << 16 ) |
                          ( ( (UInt32)( name.Length - 1 ) & (UInt32)0x7 ) << 13 );

         // inject the checksum
         Random random = new Random();
         UInt32 tableIndex = (UInt32)random.Next( 32 );
         UInt32 tableValue = (UInt32)_checksumTable[tableIndex];

         encodedBits[0] &= ~( (UInt32)0x7F << 25 );
         encodedBits[0] |= tableIndex << 27;
         encodedBits[0] |= ( tableValue >> 3 ) << 25;

         encodedBits[1] &= ~( (UInt32)0x3 << 30 );
         encodedBits[1] |= ( ( tableValue >> 1 ) & 0x3 ) << 30;

         encodedBits[5] &= ~( (UInt32)0x1 << 12 );
         encodedBits[5] |= ( tableValue & 0x1 ) << 12;

         // since the last 13 bits are unused, we only need 30 characters
         char[] passwordChars =
         [
            GetEncodedCharFromBits( encodedBits[0] >> 26 ),
            GetEncodedCharFromBits( ( encodedBits[0] >> 20 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[0] >> 14 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[0] >> 8 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[0] >> 2 ) & 0x3F ),
            GetEncodedCharFromBits( ( ( encodedBits[0] & 0x3 ) << 4 ) | ( encodedBits[1] >> 28 ) ),
            GetEncodedCharFromBits( ( encodedBits[1] >> 22 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[1] >> 16 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[1] >> 10 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[1] >> 4 ) & 0x3F ),
            GetEncodedCharFromBits( ( ( encodedBits[1] & 0xF ) << 2 ) | ( encodedBits[2] >> 30 ) ),
            GetEncodedCharFromBits( ( encodedBits[2] >> 24 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[2] >> 18 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[2] >> 12 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[2] >> 6 ) & 0x3F ),
            GetEncodedCharFromBits( encodedBits[2] & 0x3F ),
            GetEncodedCharFromBits( encodedBits[3] >> 26 ),
            GetEncodedCharFromBits( ( encodedBits[3] >> 20 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[3] >> 14 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[3] >> 8 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[3] >> 2 ) & 0x3F ),
            GetEncodedCharFromBits( ( ( encodedBits[3] & 0x3 ) << 4 ) | ( encodedBits[4] >> 28 ) ),
            GetEncodedCharFromBits( ( encodedBits[4] >> 22 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[4] >> 16 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[4] >> 10 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[4] >> 4 ) & 0x3F ),
            GetEncodedCharFromBits( ( ( encodedBits[4] & 0xF ) << 2 ) | ( encodedBits[5] >> 30 ) ),
            GetEncodedCharFromBits( ( encodedBits[5] >> 24 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[5] >> 18 ) & 0x3F ),
            GetEncodedCharFromBits( ( encodedBits[5] >> 12 ) & 0x3F )
         ];

         Password = new( passwordChars );
      }


      private void EnterPassword()
      {
         EnterPasswordWindow window = new EnterPasswordWindow();
         bool? dialogResult = window.ShowDialog();

         if ( dialogResult == false )
         {
            return;
         }

         ApplyPassword( window.Password );
      }

      private void ClearAllFields()
      {
         ApplyPassword( "Y.8AAD....8AAAAAAAAA4P.......w" );
      }

      private void ApplyPassword( string password )
      {
         if ( password.Length != 30 )
         {
            MessageBox.Show( password.Length < 30 ? "Password is too short." : "Password is too long.", "Invalid Password", MessageBoxButton.OK, MessageBoxImage.Error );
            return;
         }

         for ( int i = 0; i < 30; i++ )
         {
            char c = password[i];

            // A-Z, a-z, 0-9, dot, and dash are allowed
            if ( !( ( c >= 65 && c <= 90 ) || ( c >= 97 && c <= 122 ) || ( c >= 48 && c <= 57 ) || c == 45 || c == 46 ) )
            {
               MessageBox.Show( "Password contains at least one invalid character.", "Invalid Password", MessageBoxButton.OK, MessageBoxImage.Error );
               return;
            }
         }

         UInt32[] encodedBits = [0, 0, 0, 0, 0, 0];

         encodedBits[0] = ( GetEncodedBitsFromChar( password[0] ) << 26 ) |
                          ( GetEncodedBitsFromChar( password[1] ) << 20 ) |
                          ( GetEncodedBitsFromChar( password[2] ) << 14 ) |
                          ( GetEncodedBitsFromChar( password[3] ) << 8 ) |
                          ( GetEncodedBitsFromChar( password[4] ) << 2 ) |
                          ( GetEncodedBitsFromChar( password[5] ) >> 4 );
         encodedBits[1] = ( GetEncodedBitsFromChar( password[5] ) << 28 ) |
                          ( GetEncodedBitsFromChar( password[6] ) << 22 ) |
                          ( GetEncodedBitsFromChar( password[7] ) << 16 ) |
                          ( GetEncodedBitsFromChar( password[8] ) << 10 ) |
                          ( GetEncodedBitsFromChar( password[9] ) << 4 ) |
                          ( GetEncodedBitsFromChar( password[10] ) >> 2 );
         encodedBits[2] = ( GetEncodedBitsFromChar( password[10] ) << 30 ) |
                          ( GetEncodedBitsFromChar( password[11] ) << 24 ) |
                          ( GetEncodedBitsFromChar( password[12] ) << 18 ) |
                          ( GetEncodedBitsFromChar( password[13] ) << 12 ) |
                          ( GetEncodedBitsFromChar( password[14] ) << 6 ) |
                          ( GetEncodedBitsFromChar( password[15] ) );
         encodedBits[3] = ( GetEncodedBitsFromChar( password[16] ) << 26 ) |
                          ( GetEncodedBitsFromChar( password[17] ) << 20 ) |
                          ( GetEncodedBitsFromChar( password[18] ) << 14 ) |
                          ( GetEncodedBitsFromChar( password[19] ) << 8 ) |
                          ( GetEncodedBitsFromChar( password[20] ) << 2 ) |
                          ( GetEncodedBitsFromChar( password[21] ) >> 4 );
         encodedBits[4] = ( GetEncodedBitsFromChar( password[21] ) << 28 ) |
                          ( GetEncodedBitsFromChar( password[22] ) << 22 ) |
                          ( GetEncodedBitsFromChar( password[23] ) << 16 ) |
                          ( GetEncodedBitsFromChar( password[24] ) << 10 ) |
                          ( GetEncodedBitsFromChar( password[25] ) << 4 ) |
                          ( GetEncodedBitsFromChar( password[26] ) >> 2 );
         encodedBits[5] = ( GetEncodedBitsFromChar( password[26] ) << 30 ) |
                          ( GetEncodedBitsFromChar( password[27] ) << 24 ) |
                          ( GetEncodedBitsFromChar( password[28] ) << 18 ) |
                          ( GetEncodedBitsFromChar( password[29] ) << 12 );

         if ( !ValidateChecksum( encodedBits[0], encodedBits[1], encodedBits[5] ) )
         {
            MessageBox.Show( "Password checksum is invalid.", "Invalid Password", MessageBoxButton.OK, MessageBoxImage.Error );
            return;
         }

         PlayerExperience = ( encodedBits[0] & 0xFFFF ).ToString();
         SetDoorsOpenedFromFlags( encodedBits[0] >> 16 );
         SetTreasuresCollectedFromFlags( encodedBits[1] );
         SetItemsFromFlags( ( encodedBits[2] >> 7 ) & 0x1FFFFFF );
         SetTownsVisitedFromFlags( ( encodedBits[2] >> 1 ) & 0x3F );
         PlayerGold = ( encodedBits[3] >> 16 ).ToString();
         SetSpecialEnemiesDefeatedFromFlags( ( encodedBits[3] >> 5 ) & 0x7 );
         RescuedGwaelin = ( ( encodedBits[3] >> 4 ) & 0x1 ) != 0;
         FoundHiddenStairs = ( ( encodedBits[3] >> 3 ) & 0x1 ) != 0;
         UsedRainbowDrop = ( ( encodedBits[3] >> 2 ) & 0x1 ) != 0;
         GotStaffOfRain = ( ( encodedBits[3] >> 1 ) & 0x1 ) != 0;
         GotRainbowDrop = ( encodedBits[3] & 0x1 ) != 0;
         ExtractPlayerName( encodedBits[4], encodedBits[5] );
         SetBattleItemsFromFlags( encodedBits[3] >> 8 );
      }

      private int GetRangedNumberFromString( string str, int max )
      {
         if ( int.TryParse( str, out int result ) )
         {
            return Math.Max( 0, Math.Min( max, result ) );
         }

         return 0;
      }

      private UInt32 GetOpenedDoorFlags()
      {
         UInt32 openedDoors = 0;
         openedDoors |= _openedDoor1 ? (UInt32)0x1 : 0;
         openedDoors |= _openedDoor2 ? (UInt32)0x1 << 1 : 0;
         openedDoors |= _openedDoor3 ? (UInt32)0x1 << 2 : 0;
         openedDoors |= _openedDoor4 ? (UInt32)0x1 << 3 : 0;
         openedDoors |= _openedDoor5 ? (UInt32)0x1 << 4 : 0;
         openedDoors |= _openedDoor6 ? (UInt32)0x1 << 5 : 0;
         openedDoors |= _openedDoor7 ? (UInt32)0x1 << 6 : 0;
         openedDoors |= _openedDoor8 ? (UInt32)0x1 << 7 : 0;
         openedDoors |= _openedDoor9 ? (UInt32)0x1 << 8 : 0;
         return openedDoors;
      }

      private void SetDoorsOpenedFromFlags( UInt32 flags )
      {
         OpenedDoor1 = ( flags & 0x1 ) == 0;
         OpenedDoor2 = ( flags & 0x2 ) == 0;
         OpenedDoor3 = ( flags & 0x4 ) == 0;
         OpenedDoor4 = ( flags & 0x8 ) == 0;
         OpenedDoor5 = ( flags & 0x10 ) == 0;
         OpenedDoor6 = ( flags & 0x20 ) == 0;
         OpenedDoor7 = ( flags & 0x40 ) == 0;
         OpenedDoor8 = ( flags & 0x80 ) == 0;
         OpenedDoor9 = ( flags & 0x100 ) == 0;
      }

      private UInt32 GetTreasureCollectedFlags()
      {
         UInt32 treasuresCollected = 0;
         treasuresCollected |= _treasureCollected1 ? (UInt32)0x1 : 0;
         treasuresCollected |= _treasureCollected2 ? (UInt32)0x1 << 1 : 0;
         treasuresCollected |= _treasureCollected3 ? (UInt32)0x1 << 2 : 0;
         treasuresCollected |= _treasureCollected4 ? (UInt32)0x1 << 3 : 0;
         treasuresCollected |= _treasureCollected5 ? (UInt32)0x1 << 4 : 0;
         treasuresCollected |= _treasureCollected6 ? (UInt32)0x1 << 5 : 0;
         treasuresCollected |= _treasureCollected7 ? (UInt32)0x1 << 6 : 0;
         treasuresCollected |= _treasureCollected8 ? (UInt32)0x1 << 7 : 0;
         treasuresCollected |= _treasureCollected9 ? (UInt32)0x1 << 8 : 0;
         treasuresCollected |= _treasureCollected10 ? (UInt32)0x1 << 9 : 0;
         treasuresCollected |= _treasureCollected11 ? (UInt32)0x1 << 10 : 0;
         treasuresCollected |= _treasureCollected12 ? (UInt32)0x1 << 11 : 0;
         treasuresCollected |= _treasureCollected13 ? (UInt32)0x1 << 12 : 0;
         treasuresCollected |= _treasureCollected14 ? (UInt32)0x1 << 13 : 0;
         treasuresCollected |= _treasureCollected15 ? (UInt32)0x1 << 14 : 0;
         treasuresCollected |= _treasureCollected16 ? (UInt32)0x1 << 15 : 0;
         treasuresCollected |= _treasureCollected17 ? (UInt32)0x1 << 16 : 0;
         treasuresCollected |= _treasureCollected18 ? (UInt32)0x1 << 17 : 0;
         treasuresCollected |= _treasureCollected19 ? (UInt32)0x1 << 18 : 0;
         treasuresCollected |= _treasureCollected20 ? (UInt32)0x1 << 19 : 0;
         treasuresCollected |= _treasureCollected21 ? (UInt32)0x1 << 20 : 0;
         treasuresCollected |= _treasureCollected22 ? (UInt32)0x1 << 21 : 0;
         treasuresCollected |= _treasureCollected23 ? (UInt32)0x1 << 22 : 0;
         treasuresCollected |= _treasureCollected24 ? (UInt32)0x1 << 23 : 0;
         treasuresCollected |= _treasureCollected25 ? (UInt32)0x1 << 24 : 0;
         treasuresCollected |= _treasureCollected26 ? (UInt32)0x1 << 25 : 0;
         treasuresCollected |= _treasureCollected27 ? (UInt32)0x1 << 26 : 0;
         treasuresCollected |= _treasureCollected28 ? (UInt32)0x1 << 27 : 0;
         treasuresCollected |= _treasureCollected29 ? (UInt32)0x1 << 28 : 0;
         treasuresCollected |= _treasureCollected30 ? (UInt32)0x1 << 29 : 0;
         return treasuresCollected;
      }

      private void SetTreasuresCollectedFromFlags( UInt32 flags )
      {
         TreasureCollected1 = ( flags & 0x1 ) == 0;
         TreasureCollected2 = ( flags & 0x2 ) == 0;
         TreasureCollected3 = ( flags & 0x4 ) == 0;
         TreasureCollected4 = ( flags & 0x8 ) == 0;
         TreasureCollected5 = ( flags & 0x10 ) == 0;
         TreasureCollected6 = ( flags & 0x20 ) == 0;
         TreasureCollected7 = ( flags & 0x40 ) == 0;
         TreasureCollected8 = ( flags & 0x80 ) == 0;
         TreasureCollected9 = ( flags & 0x100 ) == 0;
         TreasureCollected10 = ( flags & 0x200 ) == 0;
         TreasureCollected11 = ( flags & 0x400 ) == 0;
         TreasureCollected12 = ( flags & 0x800 ) == 0;
         TreasureCollected13 = ( flags & 0x1000 ) == 0;
         TreasureCollected14 = ( flags & 0x2000 ) == 0;
         TreasureCollected15 = ( flags & 0x4000 ) == 0;
         TreasureCollected16 = ( flags & 0x8000 ) == 0;
         TreasureCollected17 = ( flags & 0x10000 ) == 0;
         TreasureCollected18 = ( flags & 0x20000 ) == 0;
         TreasureCollected19 = ( flags & 0x40000 ) == 0;
         TreasureCollected20 = ( flags & 0x80000 ) == 0;
         TreasureCollected21 = ( flags & 0x100000 ) == 0;
         TreasureCollected22 = ( flags & 0x200000 ) == 0;
         TreasureCollected23 = ( flags & 0x400000 ) == 0;
         TreasureCollected24 = ( flags & 0x800000 ) == 0;
         TreasureCollected25 = ( flags & 0x1000000 ) == 0;
         TreasureCollected26 = ( flags & 0x2000000 ) == 0;
         TreasureCollected27 = ( flags & 0x4000000 ) == 0;
         TreasureCollected28 = ( flags & 0x8000000 ) == 0;
         TreasureCollected29 = ( flags & 0x10000000 ) == 0;
         TreasureCollected30 = ( flags & 0x20000000 ) == 0;
      }

      private UInt32 GetItemFlags()
      {
         int keys = GetRangedNumberFromString( _keyCount, 7 );
         int herbs = GetRangedNumberFromString( _keyCount, 7 );
         int torches = GetRangedNumberFromString( _torchCount, 7 );
         int wings = GetRangedNumberFromString( _wingCount, 7 );
         int fairyWaters = GetRangedNumberFromString( _fairyWaterCount, 7 );

         return (UInt32)keys
                | ( (UInt32)herbs << 3 )
                | ( (UInt32)wings << 6 )
                | ( (UInt32)fairyWaters << 9 )
                | ( (UInt32)torches << 12 )
                | ( (UInt32)( _hasFairyFlute ? 0x1 : 0x0 ) << 15 )
                | ( (UInt32)( _hasSilverHarp ? 0x1 : 0x0 ) << 16 )
                | ( (UInt32)( _hasGwaelinsLove ? 0x1 : 0x0 ) << 17 )
                | ( (UInt32)( _hasStoneOfSunlight ? 0x1 : 0x0 ) << 18 )
                | ( (UInt32)( _hasStaffOfRain ? 0x1 : 0x0 ) << 19 )
                | ( (UInt32)( _hasErdricksToken ? 0x1 : 0x0 ) << 20 )
                | ( (UInt32)( _hasRainbowDrop ? 0x1 : 0x0 ) << 21 )
                | ( (UInt32)( _hasDragonScale ? 0x1 : 0x0 ) << 23 )
                | ( (UInt32)( _hasCursedBelt ? 0x1 : 0x0 ) << 24 );
      }

      private void SetItemsFromFlags( UInt32 flags )
      {
         KeyCount = ( flags & 0x7 ).ToString();
         HerbCount = ( ( flags >> 3 ) & 0x7 ).ToString();
         TorchCount = ( ( flags >> 6 ) & 0x7 ).ToString();
         WingCount = ( ( flags >> 9 ) & 0x7 ).ToString();
         FairyWaterCount = ( ( flags >> 12 ) & 0x7 ).ToString();
      }

      private UInt32 GetTownVisitedFlags()
      {
         UInt32 townVisitedFlags = 0;
         townVisitedFlags |= _visitedTantegel ? (UInt32)0x1 : 0;
         townVisitedFlags |= _visitedBrecconary ? (UInt32)0x1 << 1 : 0;
         townVisitedFlags |= _visitedGarinham ? (UInt32)0x1 << 2 : 0;
         townVisitedFlags |= _visitedKol ? (UInt32)0x1 << 3 : 0;
         townVisitedFlags |= _visitedCantlin ? (UInt32)0x1 << 4 : 0;
         townVisitedFlags |= _visitedRimuldar ? (UInt32)0x1 << 5 : 0;
         return townVisitedFlags;
      }

      private void SetTownsVisitedFromFlags( UInt32 flags )
      {
         VisitedTantegel = ( flags & 0x1 ) != 0;
         VisitedBrecconary = ( flags & 0x2 ) != 0;
         VisitedGarinham = ( flags & 0x4 ) != 0;
         VisitedKol = ( flags & 0x8 ) != 0;
         VisitedCantlin = ( flags & 0x10 ) != 0;
         VisitedRimuldar = ( flags & 0x20 ) != 0;
      }

      private UInt32 GetSpecialEnemyDefeatedFlags()
      {
         UInt32 specialEnemyDefeatedFlags = 0;
         specialEnemyDefeatedFlags |= _defeatedGreenDragon ? (UInt32)0x1 : 0;
         specialEnemyDefeatedFlags |= _defeatedAxeKnight ? (UInt32)0x1 << 1 : 0;
         specialEnemyDefeatedFlags |= _defeatedGolem ? (UInt32)0x1 << 2 : 0;
         return specialEnemyDefeatedFlags;
      }

      private void SetSpecialEnemiesDefeatedFromFlags( UInt32 flags )
      {
         DefeatedGreenDragon = ( flags & 0x1 ) == 0;
         DefeatedAxeKnight = ( flags & 0x2 ) == 0;
         DefeatedGolem = ( flags & 0x4 ) == 0;
      }

      private UInt32 GetEncodedBitsFromChar( char c )
      {
         return ( c >= 65 && c <= 90 ) ? (UInt32)( c - 65 ) :        // A-Z
                ( c >= 97 && c <= 122 ) ? (UInt32)( c - 97 + 26 ) :  // a-z
                ( c >= 48 && c <= 57 ) ? (UInt32)( c - 48 + 52 ) :   // 0-9
                ( c == '-' ) ? (UInt32)62 :                          // dash
                (UInt32)63;                                          // dot
      }

      private char GetEncodedCharFromBits( UInt32 bits )
      {
         return ( bits < 26 ) ? (char)( bits + 65 ) :        // A-Z
                ( bits < 52 ) ? (char)( bits - 26 + 97 ) :   // a-z
                ( bits < 62 ) ? (char)( bits - 52 + 48 ) :   // 0-9
                ( bits == 62 ) ? '-' :                       // dash
                '.';                                         // dot
      }

      private void ExtractPlayerName( UInt32 encodedBits1, UInt32 encodedBits2 )
      {
         int length = (int)( ( encodedBits2 >> 13 ) & 0x7 ) + 1;
         char[] name = ['\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'];

         if ( length > 0 )
         {
            name[0] = GetEncodedCharFromBits( ( encodedBits1 >> 26 ) & 0x3F );
         }
         if ( length > 1 )
         {
            name[1] = GetEncodedCharFromBits( ( encodedBits1 >> 20 ) & 0x3F );
         }
         if ( length > 2 )
         {
            name[2] = GetEncodedCharFromBits( ( encodedBits1 >> 14 ) & 0x3F );
         }
         if ( length > 3 )
         {
            name[3] = GetEncodedCharFromBits( ( encodedBits1 >> 8 ) & 0x3F );
         }
         if ( length > 4 )
         {
            name[4] = GetEncodedCharFromBits( ( encodedBits1 >> 2 ) & 0x3F );
         }
         if ( length > 5 )
         {
            name[5] = GetEncodedCharFromBits( ( ( encodedBits1 & 0x3 ) << 4 ) | ( ( encodedBits2 >> 28 ) & 0xF ) );
         }
         if ( length > 6 )
         {
            name[6] = GetEncodedCharFromBits( ( encodedBits2 >> 22 ) & 0x3F );
         }
         if ( length > 7 )
         {
            name[7] = GetEncodedCharFromBits( ( encodedBits2 >> 16 ) & 0x3F );
         }

         // spaces are stored as dots in the encoded password, translate them back here
         for ( int i = 0; i < length; i++ )
         {
            if ( name[i] == '.' )
            {
               name[i] = ' ';
            }
         }

         string result = new string( name );

         // strip out trailing \0 chars
         PlayerName = result.Substring( 0, length );
      }

      private void SetBattleItemsFromFlags( UInt32 flags )
      {
         UInt32 weaponFlag = ( flags >> 5 ) & 0x7;
         UInt32 armorFlag = ( flags >> 2 ) & 0x7;
         UInt32 shieldFlag = flags & 0x3;

         SelectedWeapon = Weapons[(int)weaponFlag];
         SelectedArmor = Armor[(int)armorFlag];
         SelectedShield = Shields[(int)shieldFlag];
      }

      private bool ValidateChecksum( UInt32 encodedBits0, UInt32 encodedBits1, UInt32 encodedBits5 )
      {
         UInt32 tableIndex = encodedBits0 >> 27;
         UInt32 tableValue = ( ( encodedBits5 >> 12 ) & 0x1 ) |
                             ( ( encodedBits1 >> 29 ) & 0x6 ) |
                             ( ( encodedBits0 >> 22 ) & 0x18 );

         return ( tableValue == _checksumTable[tableIndex] );
      }
   }
}
