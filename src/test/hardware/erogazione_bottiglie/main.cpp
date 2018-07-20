#include <CH926.h>
#include <DetScreen.h>
#include <Dispenser.h>
#include <Global.h>

#define BOTTLE_PIN CONTROLLINO_R14
#define BOTTLE_ENDSTOP CONTROLLINO_A15
#define BUTTON_PIN CONTROLLINO_A13

void setup() {
  DEBUGBEGIN(115200);
  pinMode(CONTROLLINO_D6, OUTPUT);
  digitalWrite(CONTROLLINO_D6, HIGH);
  pinMode(BOTTLE_PIN, OUTPUT);
  pinMode(BOTTLE_ENDSTOP, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  DEBUG("Setup end\n");
}

void loop() {
  if (digitalRead(BUTTON_PIN)) {
    DEBUG("Button pressed\n");

    digitalWrite(BOTTLE_PIN, HIGH);
    DEBUG("Motor on\n");
    DEBUG("Waiting for endstop\n");
    // wait for the endstop to change state two times
    while (digitalRead(BOTTLE_ENDSTOP))
      ;
    while (!digitalRead(BOTTLE_ENDSTOP))
      ;

    digitalWrite(BOTTLE_PIN, LOW);
    DEBUG("Motor off\n");
  }
}