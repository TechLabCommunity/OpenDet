#include <DetScreen.h>
#include <Global.h>

DetScreen scr(0x27, 20, 4);
// LiquidCrystal_I2C scr(0x27, 20, 4);
unsigned long timer1 = 0;
unsigned long timer2 = 0;
uint coin = 0;
bool is_first = true;

void setup() {
  DEBUGBEGIN(115200);

  scr.begin();
  scr.backlight();
  scr.start_system(VERSION_FIRMWARE);
  scr.clear();
  timer1 = timer2 = millis();
}

void loop() {
    if (millis() - timer2 < 20000) {
    if (millis() - timer1 >= 5000) {
      coin += 70;
      timer1 = millis();
    }
    scr.main_screen(0, coin);
  } else {
    if (is_first) {
      scr.clear();
      is_first = false;
    }

    scr.dispense_screen("");
  }
}
