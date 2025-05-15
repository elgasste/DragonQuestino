using DragonQuestinoEditor.FileOps;

namespace DragonQuestinoEditor.ViewModels
{
   public class EnemyViewModel : ViewModelBase
   {
      private int _index;
      public int Index
      {
         get => _index;
         set => SetProperty( ref _index, value );
      }

      private string _name = "";
      public string Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      private string _nameMacro = "";
      public string NameMacro
      {
         get => _nameMacro;
         set => SetProperty( ref _nameMacro, value );
      }

      private string _indefiniteArticle = "";
      public string IndefiniteArticle
      {
         get => _indefiniteArticle;
         set => SetProperty( ref _indefiniteArticle, value );
      }

      private int _hitPoints;
      public int HitPoints
      {
         get => _hitPoints;
         set => SetProperty( ref _hitPoints, value );
      }

      private int _attackPower;
      public int AttackPower
      {
         get => _attackPower;
         set => SetProperty( ref _attackPower, value );
      }

      private int _defensePower;
      public int DefensePower
      {
         get => _defensePower;
         set => SetProperty( ref _defensePower, value );
      }

      private int _agility;
      public int Agility
      {
         get => _agility;
         set => SetProperty( ref _agility, value );
      }

      private int _experience;
      public int Experience
      {
         get => _experience;
         set => SetProperty( ref _experience, value );
      }

      private int _gold;
      public int Gold
      {
         get => _gold;
         set => SetProperty( ref _gold, value );
      }

      public EnemyViewModel( EnemySaveData saveData )
      {
         Index = saveData.Index;
         Name = saveData.Name;
         NameMacro = saveData.NameMacro;
         IndefiniteArticle = saveData.IndefiniteArticle;
         HitPoints = saveData.HitPoints;
         AttackPower = saveData.AttackPower;
         DefensePower = saveData.DefensePower;
         Agility = saveData.Agility;
         Experience = saveData.Experience;
         Gold = saveData.Gold;
      }
   }
}
