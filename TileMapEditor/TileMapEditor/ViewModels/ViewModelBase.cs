using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace DeviceFrameEditor.ViewModels
{
   public abstract class ViewModelBase : INotifyPropertyChanged
   {
      public event PropertyChangedEventHandler? PropertyChanged;

      protected void OnPropertyChanged( string? propertyName )
      {
         PropertyChangedEventHandler? eventHandler = PropertyChanged;

         if ( eventHandler != null )
         {
            var args = new PropertyChangedEventArgs( propertyName );
            eventHandler( this, args );
         }
      }

      protected bool SetProperty<T>( ref T property, T newValue, [CallerMemberName] string? propertyName = null )
      {
         if ( Equals( property, newValue ) )
         {
            return false;
         }
         property = newValue;
         OnPropertyChanged( propertyName );
         return true;
      }
   }
}
