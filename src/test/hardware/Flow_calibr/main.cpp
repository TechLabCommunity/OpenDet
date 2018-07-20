#include <DetScreen.h>
#include <Dispenser.h>
#include <Global.h>
#include "config.h"

int btnArr[DISPENSER_N] = {BTN_PIN_1, BTN_PIN_2, BTN_PIN_3,
                           BTN_PIN_4, BTN_PIN_5, BTN_PIN_6};

int ledArr[DISPENSER_N] = {BTN_LED_1, BTN_LED_2, BTN_LED_3,
                           BTN_LED_4, BTN_LED_5, BTN_LED_6};

int pumpArr[DISPENSER_N] = {PUMP_PIN_1, PUMP_PIN_2, PUMP_PIN_3,
                            PUMP_PIN_4, PUMP_PIN_5, PUMP_PIN_6};

int flowArr[DISPENSER_N] = {FLOW_PIN_1, FLOW_PIN_2, FLOW_PIN_3,
                            FLOW_PIN_4, FLOW_PIN_5, FLOW_PIN_6};

bool firstTime = true;

DetScreen lcd(0x27, 20, 4);

uint btn_pressed = 0;

void setup() {
  DEBUGBEGIN(115200);

  for (int i = 0; i < DISPENSER_N; i++) {
    pinMode(btnArr[i], INPUT);
    pinMode(ledArr[i], OUTPUT);
  }
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  delay(500);

  for (int i = 0; i < DISPENSER_N; i++) {
    digitalWrite(ledArr[i], HIGH);
  }

  DEBUG("Setup done!\n");
}

void loop() {
  firstTime = true;
  // check all pins
  for (int i = 0; i < DISPENSER_N; i++) {
    int counter = 0, prevState = 0, currState = 0;

    while (digitalRead(btnArr[i])) {
      if (firstTime) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("POMPA: " + (String)(i + 1));
        lcd.setCursor(0, 1);
        lcd.print("IMPULSI:");
        firstTime = false;
      }

      digitalWrite(pumpArr[i], HIGH);
      Serial.println("Accensione pompa");

      currState = digitalRead(flowArr[i]);
      if (currState != prevState) {
        Serial.println(counter);
        counter++;
        prevState = currState;
        lcd.setCursor(0, 3);
        lcd.print(counter);
      }
    }
    digitalWrite(pumpArr[i], LOW);
  }
}