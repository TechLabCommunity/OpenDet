#include "CH926.h"
#include <Global.h>

CH926::CH926(int sigPin, int pwrPin, int sigLogic, int pwrLogic) {
  pinMode(sigPin, INPUT);
  pinMode(pwrPin, OUTPUT);
  digitalWrite(pwrPin, !pwrLogic);

  _sigPin = sigPin;
  _pwrPin = pwrPin;
  _pwrLogic = pwrLogic;
  _sigLogic = sigLogic;
  _state = 0;
}

void CH926::start() {
  digitalWrite(_pwrPin, _pwrLogic);
  _state = 1;
}

void CH926::stop() {
  digitalWrite(_pwrPin, !_pwrLogic);
  _state = 0;
}

int CH926::countPulses(unsigned long endTime) {
  int currState = _sigLogic;
  int prevState = _sigLogic;
  int counter = 0;
  unsigned long prevTime = millis();

  if (_state) {
    // if millis() overflow reset prevTime
    if (millis() - prevTime < 0) {
      prevTime = millis();
    }

    while ((millis() - prevTime) < endTime) {
      currState = digitalRead(_sigPin);

      if (currState != prevState) {
        prevState = currState;
        counter++;
        prevTime = millis();
      }
    }
    return counter;
  }

  return -1;
}
