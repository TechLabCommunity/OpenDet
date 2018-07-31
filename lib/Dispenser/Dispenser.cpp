#include "Dispenser.h"

// pump error flag
bool pumpError_flag = false;

// raise error for flow meter/pump
void pumpError() { pumpError_flag = true; }

Dispenser::Dispenser(uint btnPin, uint flowPin, uint pumpPin,
                     uint pulsesPerLiter, String detName, uint detPrice,
                     uint tankCap, uint flowTimeout) {
  if (pulsesPerLiter < 1) SYSERR("_pulsesPerLiter mustn't be less than 1.");
  // It's enough because detPrice is in cents.
  if (detPrice < 1) SYSERR("_detPrice mustn't be less than 1 cent.");
  _detPrice = detPrice;
  _pulsesPerLiter = pulsesPerLiter;
  _btnPin = btnPin;
  _flowPin = flowPin;
  _pumpPin = pumpPin;
  _detName = detName;
  // save quantity in milliliters
  _detCnt = tankCap * 1000;
  _detCntMax = _detCnt;

  // 0 = no error
  _error = OK;
  _flowTimeout = flowTimeout;

  pinMode(btnPin, INPUT);
  pinMode(flowPin, INPUT);
  pinMode(pumpPin, OUTPUT);
}

uint Dispenser::getPulses() { return _pulsesPerLiter; }

void Dispenser::setPulses(uint pulses) { _pulsesPerLiter = pulses; }

String Dispenser::getDetName() { return _detName; }

void Dispenser::setName(String name) { _detName = name; }

uint Dispenser::getPrice() { return _detPrice; }

DISP_ERR Dispenser::dispense(FIXED_QUANTITY qty) { return dispense((uint)qty); }

/**
 * @brief dispense a certain amount of milliliters
 *
 * @param milliliters milliliters to dispense
 * @return   0 in case of success
 *          -2 in case of pump or flowmeter error
 */
DISP_ERR Dispenser::dispense(uint milliliters) {
  // if there is pump error do not dispense anything
  if (_error <= PUMP_ERR) {
    return _error;
  }
  if (milliliters > _detCnt) {
    return QTY_LOW;
  }

  // check valid millimiters input
  if (milliliters < 1) SYSERR(_detName + ": millilitres mustn't be 0.");

  uint pulses = ((float)milliliters * _pulsesPerLiter) / 1000., counter = 0,
       prevState = 0, currState = 0;

  // subtract dispensed quantity from counter
  _detCnt -= milliliters;

  // turn on pump
  digitalWrite(_pumpPin, HIGH);

  MsTimer2::set(_flowTimeout, pumpError);
  // start timer to check the dispensing process
  MsTimer2::start();

  // count pulses
  // if error flag is raised during dispensing terminate immediately
  while (counter <= pulses && !pumpError_flag) {
    currState = digitalRead(_flowPin);

    if (currState != prevState) {
      // DEBUG.print((String)counter + "\n");
      counter++;
      prevState = currState;

      // reset timer
      MsTimer2::stop();
      MsTimer2::start();
    }
  }

  // stop the timer
  MsTimer2::stop();

  // turn off pump
  digitalWrite(_pumpPin, LOW);

  if (_detCnt == 0) {
    _error = TANK_EMPTY;
  }
  if (pumpError_flag) {
    _error = (DISP_ERR)(_error + PUMP_ERR);
  }

  // reset error flag - it's a static variable
  pumpError_flag = false;

  return _error;
}

DISP_ERR Dispenser::pumpErr_reset() {
  if (_error == PUMP_ERR || _error == PUMP_AND_EMPTY) {
    _error = (DISP_ERR)(_error - PUMP_ERR);
  }

  return _error;
}

DISP_ERR Dispenser::fillTank() {
  _detCnt = _detCntMax;

  if (_error == TANK_EMPTY || _error == PUMP_AND_EMPTY) {
    _error = (DISP_ERR)(_error - TANK_EMPTY);
  }

  return _error;
}

// TODO maybe add parameter for calibration quantity, so I can calibrate with
// half a liter, two liters...
uint Dispenser::calibrate(uint8_t pin, uint8_t btnPressed) {
  unsigned long startTime = millis();

  uint counter = 0, prevState = 0, currState = 0;

  while (digitalRead(pin) == btnPressed) {
    digitalWrite(_pumpPin, HIGH);

    currState = digitalRead(_flowPin);
    if (currState != prevState) {
      counter++;
      prevState = currState;
    }
  }
  // turn off the pump
  digitalWrite(_pumpPin, LOW);

  // set new value only if it count at least one pulse (only one will cause bad
  // calibration)
  if (counter != 0) {
    // calculate duration for 1 liter dispensing
    unsigned long duration = millis() - startTime;

    // calculate timeout for flow meters, this time correspond to 10 times the
    // pulse duration
    _flowTimeout = duration / counter * 10;

    // set new value for pulsesPerLiter
    setPulses(counter);
  }

  return counter;
}
