#include "Dispenser.h"
#include "Arduino.h"

Dispenser::Dispenser(int btnPin, int flowPin, int pumpPin, int pulsesPerLiter){

  pinMode(btnPin, INPUT);
  pinMode(flowPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);

  _btnPin = btnPin;
  _flowPin = flowPin;
  _pulsesPerLiter = pulsesPerLiter;
  _pumpPin = pumpPin;

}

int Dispenser::getPin(){
  return _flowPin;
}

int Dispenser::getPulses(){
  return _pulsesPerLiter;
}

int Dispenser::getPump(){
  return _pumpPin;
}

//think about return type, void is not the best solution
void Dispenser::dispense(double liters){
  int pulses = liters * _pulsesPerLiter;
  int counter = 0;
  digitalWrite(_pumpPin, HIGH);
  int prevState = 0;
  int currState = 0;
  //eventually subtract error due to detergent inertia
  //when i stop the pump some detergent can flow, going over the required quantity
  //adding this error requires another class variable and relative contrustor param
  while (counter <= pulses /*- inertiaError*/){

    currState = digitalRead(_flowPin);
    if (currState != prevState) {
      counter++;
      prevState = currState;
    }

    //function to updateLCD
  }
  digitalWrite(_pumpPin, LOW);
}
