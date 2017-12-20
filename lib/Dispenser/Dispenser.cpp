#include "Dispenser.h"
#include "Arduino.h"

Dispenser::Dispenser(int btnPin, int flowPin, int pumpPin, int pulsesPerLiter, String detName, int detPrice){

  pinMode(btnPin, INPUT);
  pinMode(flowPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  _btnPin = btnPin;
  _flowPin = flowPin;
  _pulsesPerLiter = pulsesPerLiter;
  _pumpPin = pumpPin;
  _detName = detName;
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
