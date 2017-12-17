#include <Arduino.h>
#include "Controllino.h"
#include "Dispenser.h"

#define DISPENSER_N 6
#define FLOW_PIN CONTROLLINO_D1
#define BTN_PIN CONTROLLINO_D2
#define PUMP_PIN CONTROLLINO_R0
#define PULSESXLITER 200


Dispenser disp1(FLOW_PIN, BTN_PIN, PUMP_PIN, PULSESXLITER, "Main");


void setup() {
}

void loop() {
}
