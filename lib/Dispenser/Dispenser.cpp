#include "Dispenser.h"

Dispenser::Dispenser(uint btnPin, uint flowPin, uint pumpPin,
                     uint pulsesPerLiter, String detName, uint detPrice) {
  if (pulsesPerLiter < 1) SYSERR("_pulsesPerLiter mustn't be less than 1.");
  // It's enough because detPrice is in cents.
  if (detPrice < 1) SYSERR("_detPrice mustn't be less than 1 cent.");
  _detPrice = detPrice;
  _pulsesPerLiter = pulsesPerLiter;
  _btnPin = btnPin;
  _flowPin = flowPin;
  _pumpPin = pumpPin;
  _detName = detName;
  pinMode(btnPin, INPUT);
  pinMode(flowPin, INPUT);
  pinMode(pumpPin, OUTPUT);
}

uint Dispenser::getPulses() { return _pulsesPerLiter; }

String Dispenser::getDetName() { return _detName; }

uint Dispenser::getPrice() { return _detPrice; }

void Dispenser::dispense(FIXED_QUANTITY qty) { dispense((uint)qty); }

void Dispenser::dispense(uint milliliters) {
  // check valid millimiters input
  if (milliliters < 1) SYSERR(_detName + ": millilitres mustn't be 0.");

  int pulses = ((float)milliliters * _pulsesPerLiter) / 1000., counter = 0,
      prevState = 0, currState = 0;

  // turn on pump
  digitalWrite(_pumpPin, HIGH);
  // TODO start a timer and in case of overflow turn off pump and display
  // error message in the home screen. Then update the error.log file

  // disable overflow interrupt
  // bitClear(TIMSK1, TOIE1);

  // count pulses
  while (counter <= pulses) {
    currState = digitalRead(_flowPin);

    if (currState != prevState) {
      Serial.println(counter);
      counter++;
      prevState = currState;
    }
  }

  // turn off pump
  digitalWrite(_pumpPin, LOW);
}

/*
ISR(TIM1_OVF_vect) {  // interrupt overflow routine
  // preload timer
  TCNT1 = 65380;
  digitalWrite(LED, digitalRead(LED) ^ 1);
}
*/
