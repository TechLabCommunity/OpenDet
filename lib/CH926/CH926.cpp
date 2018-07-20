#include "CH926.h"
#include <Controllino.h>
#include <Global.h>

// Pin 18 correspond to pin PD3 (pin 46, INT3) of ATMega2560

// fast mode has pulse freq of 7.7 Hz

CH926::CH926(int sigPin, int pwrPin, int sigLogic, int pwrLogic) {
  (sigLogic == NO) ? pinMode(sigPin, INPUT_PULLUP) : pinMode(sigPin, INPUT);
  // pinMode(sigPin, INPUT);
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
    return counter / 2;
  }

  return -1;
}

void CH926::ISR_counter() { _counter++; }

// TODO
/* After attachInterrupt() the ISR is called (1,2 or 3 times) without a falling
 * edge, in fact the EIFR register is 0. A workaround for this is to reset the
 * counter 500ms after attachInterrupt(), but this strange behaviour need to be
 * solved
 */
void CH926::startInterrupt() {
  start();

  // clear interrupt flag for pin 18 (INT3 for the ATMega 2560)
  // EIFR |= 0x01 << INT3;

  int mode;
  if (_sigLogic == NO) {
    mode = FALLING;
  } else if (_sigLogic == NC) {
    mode = RISING;
  }

  attachInterrupt(digitalPinToInterrupt(_sigPin), ISR_counter, mode);

  delay(500);
  _counter = 0;
}

// TODO
/* As for startInterrupt, the ISR is called after detachInterrupt and this make
 * no sense. Fortunately the counter is reset after the next calling to
 * startInterrupt, but this strange behaviour nedd some investigation
 */
void CH926::stopInterrupt() {
  detachInterrupt(_sigPin);

  stop();
}

uint32_t CH926::counter() { return _counter; }