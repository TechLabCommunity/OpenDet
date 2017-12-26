#include <Global.h>
#include <DetScreen.h>

DetScreen scr(0x27, 20, 4);
uint coin = 0;
unsigned long timer = 0;

void setup() {
  DEBUGBEGIN(115200);
  scr.begin();
  scr.backlight();
  scr.start_system(VERSION_FIRMWARE);
  scr.clear();
}

void loop() {
  scr.main_screen(0, coin);
  if (millis() - timer >= 10000){
    coin+=70;
    timer = millis();
  }
}
