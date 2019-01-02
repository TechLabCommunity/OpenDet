#include <Dispenser.h>
#include <Global.h>

#define DEBUGGER

#define DISPENSER_N 6
#define FLOW_PIN CONTROLLINO_A1
#define BTN_PIN CONTROLLINO_A2
#define PUMP_PIN CONTROLLINO_R0
#define PULSESXLITER 1101
#define DETPRICE 250

Dispenser disp1(BTN_PIN, FLOW_PIN, PUMP_PIN, PULSESXLITER, "Test", DETPRICE);

void setup() {
  // DEBUGBEGIN(9600);
  // DEBUG("SETUP COMPLETE\n");
  Serial.begin(9600);
  Serial.println("Setup Complete");
}

int currState = 0;
int prevState = 0;
int counter = 0;
unsigned long startTime = 0;
unsigned long finishTime = 0;

void loop() {
  if (digitalRead(BTN_PIN)) {
    startTime = millis();
    disp1.dispense(1);
    finishTime = millis();
    Serial.println("Start: " + (String)startTime);
    Serial.println("Finish: " + (String)finishTime);
    Serial.println("Elapsed: " + (String)(finishTime - startTime));
    Serial.println("Finish");
  }

  /*
  while(digitalRead(BTN_PIN)) {
    digitalWrite(PUMP_PIN, HIGH);
    currState = digitalRead(FLOW_PIN);
    if (currState != prevState) {
      counter++;
      prevState = currState;
      Serial.println((String)counter);
    }
  }

  digitalWrite(PUMP_PIN, LOW);
*/
}
