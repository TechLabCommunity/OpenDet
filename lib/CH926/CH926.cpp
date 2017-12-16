#include "CH926.h"
#include "Arduino.h"

CH926::CH926(int sigPin, int pwrPin, int logic){

  pinMode(sigPin, INPUT);
  pinMode(pwrPin, OUTPUT);
  digitalWrite(pwrPin, !logic);

  _sigPin = sigPin;
  _pwrPin = pwrPin;
  _logic = logic;
}

void CH926::start(){
  digitalWrite(_pwrPin, _logic);
}

void CH926::stop(){
  digitalWrite(_pwrPin, !_logic);
}
