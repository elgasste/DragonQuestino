
#if !defined( GIGA_SHIELD_H )
#define GIGA_SHIELD_H

#include "Adafruit_GFX.h"
#include "Arduino_H7_Video.h"

typedef struct Screen_t Screen_t;

class GigaShield : public Adafruit_GFX {
   public:
      GigaShield();
      ~GigaShield();

      void begin();
      void setScreen( Screen_t* screen ) { _screen = screen; };
      // we have to implement this, even if it does nothing
      void drawPixel( int16_t x, int16_t y, uint16_t color ) { }
      void drawScreen();
      uint16_t* getBuffer() { return _buffer; }

   private:
      void refreshThreadWorker();

   private:
      Arduino_H7_Video* _display;
      uint16_t* _buffer = nullptr;
      Screen_t* _screen;
      rtos::Thread* _refreshThread;
};

#endif // GIGA_SHIELD_H
