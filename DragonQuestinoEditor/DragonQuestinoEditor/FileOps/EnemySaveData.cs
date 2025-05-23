using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class EnemySaveData
   {
      public int Index { get; set; }
      public string Name { get; set; } = "";
      public string NameMacro { get; set; } = "";
      public string IndefiniteArticle { get; set; } = "";
      public int Strength { get; set; }
      public int Agility { get; set; }
      public int MinHitPoints { get; set; }
      public int MaxHitPoints { get; set; }
      public int SleepResist { get; set; }
      public int StopSpellResist { get; set; }
      public int HurtResist { get; set; }
      public int Dodge { get; set; }
      public int Experience { get; set; }
      public int MinGold { get; set; }
      public int MaxGold { get; set; }

      public EnemySaveData() { }

      public EnemySaveData( EnemyViewModel enemy )
      {
         Index = enemy.Index;
         Name = enemy.Name;
         NameMacro = enemy.NameMacro;
         IndefiniteArticle = enemy.IndefiniteArticle;
         Strength = enemy.Strength;
         Agility = enemy.Agility;
         MinHitPoints = enemy.MinHitPoints;
         MaxHitPoints = enemy.MaxHitPoints;
         SleepResist = enemy.SleepResist;
         StopSpellResist = enemy.StopSpellResist;
         HurtResist = enemy.HurtResist;
         Dodge = enemy.Dodge;
         Experience = enemy.Experience;
         MinGold = enemy.MinGold;
         MaxGold = enemy.MaxGold;
      }
   }
}
