#include "Dispenser.h"
#include "Arduino.h"

Dispenser::Dispenser(int btnPin, int flowPin, int pumpPin, int pulsesPerLiter, String detName){

  pinMode(btnPin, INPUT);
  pinMode(flowPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  _btnPin = btnPin;
  _flowPin = flowPin;
  _pulsesPerLiter = pulsesPerLiter;
  _pumpPin = pumpPin;
  _detName = detName;

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

//think about return type, void is not the best solution
void Dispenser::dispense(double liters){
  int pulses = liters * _pulsesPerLiter, counter = 0, prevState = 0, currState = 0;
  //eventually subtract error due to detergent inertia
  //when i stop the pump some detergent can flow, going over the required quantity
  //adding this error requires another class variable and relative contrustor param
  digitalWrite(_pumpPin, HIGH);
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
