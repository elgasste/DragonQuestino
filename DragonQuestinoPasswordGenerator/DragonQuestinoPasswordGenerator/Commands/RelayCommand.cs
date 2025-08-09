﻿using System.Windows.Input;

namespace DragonQuestinoPasswordGenerator.Commands
{
   public class RelayCommand : ICommand
   {
      readonly Action _execute;
      readonly Func<bool> _canExecute;

      public RelayCommand( Action execute )
         : this( execute, null! )
      {
      }

      public RelayCommand( Action execute, Func<bool> canExecute )
      {
         _execute = execute ?? throw new ArgumentNullException( nameof( execute ) );
         _canExecute = canExecute;
      }

      public bool CanExecute( object? parameter ) => _canExecute is null || _canExecute();

      public event EventHandler? CanExecuteChanged
      {
         add
         {
            if ( _canExecute is not null )
            {
               CommandManager.RequerySuggested += value;
            }
         }
         remove
         {
            if ( _canExecute is not null )
            {
               CommandManager.RequerySuggested -= value;
            }
         }
      }

      public void Execute( object? parameter ) => _execute();
   }
}
