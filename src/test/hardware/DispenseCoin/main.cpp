#include <CH926.h>
#include <Dispenser.h>
#include <Global.h>

#define DISPENSER_N 6
#define FLOW_PIN CONTROLLINO_A0
#define BTN_PIN CONTROLLINO_A6
#define PUMP_PIN CONTROLLINO_R0
#define PULSESXLITER 1101
#define DETPRICE 250  // cents of euro
#define COIN_SIGPIN CONTROLLINO_A12
#define COIN_PWRPIN CONTROLLINO_A13

Dispenser disp1(BTN_PIN, FLOW_PIN, PUMP_PIN, PULSESXLITER, "Test", DETPRICE);
CH926 coin(COIN_SIGPIN, COIN_PWRPIN, NO, ACTIVE_HIGH);

void setup() {
  // DEBUGBEGIN(9600);
  // DEBUG("SETUP COMPLETE\n");
  Serial.begin(115200);
  Serial.println("Setup Complete");
  coin.start();
}

unsigned long startTime = 0;
unsigned long finishTime = 0;

int balance = 0;

void loop() {
  // control button pression
  if (digitalRead(BTN_PIN)) {
    int detPrice = disp1.getPrice();
    if (balance >= detPrice) {
      balance -= detPrice;
      disp1.dispense(LT);
      Serial.println("Finish dispensing");
      Serial.println("Credit: " + (String)balance);

    } else {
      Serial.println("Insufficient Credit");
      Serial.println("Credit: " + (String)balance);
    }
  }

  // control coin insertion
  // better implement a method like "coinInserted()"
  if (!digitalRead(COIN_SIGPIN)) {
    // if no signal after 400 ms stop counting
    int pulses = coin.countPulses(400);
    // pulses counted with no errors
    if (pulses != -1) {
      // 1 pulse = 5 cent
      balance += pulses * 5;
      Serial.println("Credit: " + (String)balance);
    }
    // error in countPulses()
    else {
      Serial.println("Coin signal error");
    }
  }
}
