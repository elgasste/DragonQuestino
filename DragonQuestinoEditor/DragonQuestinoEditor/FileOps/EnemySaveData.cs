using DragonQuestinoEditor.ViewModels;

namespace DragonQuestinoEditor.FileOps
{
   public class EnemySaveData
   {
      public int Index { get; set; }
      public string Name { get; set; } = "";
      public string NameMacro { get; set; } = "";
      public string IndefiniteArticle { get; set; } = "";
      public int HitPoints { get; set; }
      public int AttackPower { get; set; }
      public int DefensePower { get; set; }
      public int Agility { get; set; }
      public int Experience { get; set; }
      public int Gold { get; set; }

      public EnemySaveData() { }

      public EnemySaveData( EnemyViewModel enemy )
      {
         Index = enemy.Index;
         Name = enemy.Name;
         NameMacro = enemy.NameMacro;
         IndefiniteArticle = enemy.IndefiniteArticle;
         HitPoints = enemy.HitPoints;
         AttackPower = enemy.AttackPower;
         DefensePower = enemy.DefensePower;
         Agility = enemy.Agility;
         Experience = enemy.Experience;
         Gold = enemy.Gold;
      }
   }
}
