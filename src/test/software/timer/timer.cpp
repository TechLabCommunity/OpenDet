#include <Arduino.h>
#include "MsTimer2.h"

void timerfunc() { Serial.println(millis()); }

void setup() {
  Serial.begin(115200);
  // call timerfunc every second
  MsTimer2::set(1000, timerfunc);
  Serial.println(
      "you should see 3 timestamp with delay 1s, and then the 4th timestamp "
      "after about 1.5s");
  MsTimer2::start();
  delay(3500);

  // reset timer
  MsTimer2::stop();
  MsTimer2::start();
}

void loop() {}
