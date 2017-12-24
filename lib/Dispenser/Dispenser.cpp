#include "Dispenser.h"

Dispenser::Dispenser(int btnPin, int flowPin, int pumpPin, unsigned int pulsesPerLiter, String detName, unsigned int detPrice){

  pinMode(btnPin, INPUT);
  pinMode(flowPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  _btnPin = btnPin;
  _flowPin = flowPin;
  _pulsesPerLiter = pulsesPerLiter;
  _pumpPin = pumpPin;
  _detName = detName;
  //It's enough because detPrice is in cents.
  if (_detPrice < 1)
    SYSERR("_detPrice mustn't be less then 1 cent.");
  _detPrice = detPrice;

}

int Dispenser::getBtnPin(){
  return _btnPin;
}

int Dispenser::getFlowPin(){
  return _flowPin;
}

int Dispenser::getPulses(){
  return _pulsesPerLiter;
}

int Dispenser::getPumpPin(){
  return _pumpPin;
}

String Dispenser::getDetName(){
  return _detName;
}

int Dispenser::getPrice(){
  return _detPrice;
}

//I can return detPrice, so I can subtract it to balance (ask Giusti)
void Dispenser::dispense(double liters){
  int pulses = liters * _pulsesPerLiter, counter = 0, prevState = 0, currState = 0;

  digitalWrite(_pumpPin, HIGH);
  while (counter <= pulses){
    currState = digitalRead(_flowPin);
    if (currState != prevState) {
      counter++;
      prevState = currState;
    }
    //function to updateLCD
  }
  digitalWrite(_pumpPin, LOW);
}
