
#if !defined( GIGA_SHIELD_H )
#define GIGA_SHIELD_H

#include "Adafruit_GFX.h"
#include "Arduino_H7_Video.h"
#include "screen.h"

class GigaShield : public Adafruit_GFX {
   public:
      GigaShield();
      ~GigaShield();

      void begin();
      void setScreen( Screen_t* screen ) { _screen = screen; };
      // we have to implement this, even if it does nothing
      void drawPixel( int16_t x, int16_t y, uint16_t color ) { }
      void drawScreen();
      void wipeScreen( uint16_t color );
      uint16_t* getBuffer() { return _mainBuffer; }

   private:
      void refreshThreadWorker();

   private:
      Arduino_H7_Video* _display;
      uint16_t* _mainBuffer = nullptr;
      uint16_t* _wipeBuffer = nullptr;
      bool _isWiping = false;
      Screen_t* _screen;
      rtos::Thread* _refreshThread;
};

#endif // GIGA_SHIELD_H
