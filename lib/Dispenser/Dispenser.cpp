#include "Dispenser.h"
Dispenser Dispenser::listDispenser[MAX_DISPENSER];
uint Dispenser::indexList = 0;

Dispenser::Dispenser(){

}

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
  _numberId = indexList; //NumberId con indice di classe.
  pinMode(btnPin, INPUT);
  pinMode(flowPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  listDispenser[indexList++] = (*this);
}

uint Dispenser::getPulses() { return _pulsesPerLiter; }

String Dispenser::getDetName() { return _detName; }

uint Dispenser::getPrice() { return _detPrice; }

uint Dispenser::getButton() { return _btnPin;}

uint Dispenser::getNumberId() {return _numberId;}

void Dispenser::dispense(FIXED_QUANTITY qty) { dispense((uint)qty); }

void Dispenser::dispense(uint milliliters) {
  Serial.println("Dispensing ml: " + (String)milliliters);
  if (milliliters < 1) SYSERR(_detName + ": millilitres mustn't be 0.");
  int pulses = ((float)milliliters * _pulsesPerLiter) / 1000., counter = 0,
      prevState = 0, currState = 0;
  digitalWrite(_pumpPin, HIGH);
  Serial.println("Accensione pompa");
  while (counter <= pulses) {
    currState = digitalRead(_flowPin);
    if (currState != prevState) {
      Serial.println(counter);
      counter++;
      prevState = currState;
    }
  }
  digitalWrite(_pumpPin, LOW);
}

Dispenser& Dispenser::getDispFromButton(uint button){
  for (uint i=0; i<indexList; i++){
    if (listDispenser[i].getButton() == button){
      return listDispenser[i];
    }
  }
  Dispenser* disp = NULL;
  return *disp;
}