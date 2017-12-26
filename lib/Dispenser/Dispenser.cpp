#include "Dispenser.h"

Dispenser::Dispenser(uint btnPin, uint flowPin, uint pumpPin, uint pulsesPerLiter, String detName, uint detPrice){
  if (_pulsesPerLiter < 1)
    SYSERR("_pulsesPerLiter mustn't be less than 1.");
  //It's enough because detPrice is in cents.
  if (_detPrice < 1)
    SYSERR("_detPrice mustn't be less than 1 cent.");
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

uint Dispenser::getPulses(){
  return _pulsesPerLiter;
}

String Dispenser::getDetName(){
  return _detName;
}

uint Dispenser::getPrice(){
  return _detPrice;
}

void Dispenser::dispense(FIXED_QUANTITY qty){
  dispense(qty);
}

//I can return detPrice, so I can subtract it to balance (ask Giusti)
void Dispenser::dispense(uint millilitres){
  if (millilitres < 1)
    SYSERR(_detName+": millilitres mustn't be 0.");
  int pulses = ((float)millilitres * _pulsesPerLiter) / 1000., counter = 0, prevState = 0, currState = 0;
  digitalWrite(_pumpPin, HIGH);
  while (counter <= pulses){
    currState = digitalRead(_flowPin);
    if (currState != prevState) {
      counter++;
      prevState = currState;
    }
  }
  digitalWrite(_pumpPin, LOW);
}
