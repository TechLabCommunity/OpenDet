#include "Dispenser.h"

// THIS LIBRARY COULD USE MSTIMER2, SO BE SURE TO AVOID CONFLICT WITH DETSCREEN
// LIBRARY

// if no pulse arrive from flow meters within this time an error is raised
// TODO about 30 s to dispense pump 5, see pulse for this pump to see time for a
// single pulse
// pump 6 35s
#define FLOW_TIMER_MS 100

bool flowError_flag = false;

// raise error for flow meter/pump
void flowError() { flowError_flag = true; }

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
  pinMode(btnPin, INPUT);
  pinMode(flowPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  MsTimer2::set(FLOW_TIMER_MS, flowError);
}

uint Dispenser::getPulses() { return _pulsesPerLiter; }

String Dispenser::getDetName() { return _detName; }

uint Dispenser::getPrice() { return _detPrice; }

int Dispenser::dispense(FIXED_QUANTITY qty) { return dispense((uint)qty); }

/**
 * @brief dispense a certain amount of milliliters
 *
 * @param milliliters milliliters to dispense
 * @return   0 in case of success
 *          -2 in case of pump or flowmeter error
 */
int Dispenser::dispense(uint milliliters) {
  // check valid millimiters input
  if (milliliters < 1) SYSERR(_detName + ": millilitres mustn't be 0.");

  int pulses = ((float)milliliters * _pulsesPerLiter) / 1000., counter = 0,
      prevState = 0, currState = 0;

  // turn on pump
  digitalWrite(_pumpPin, HIGH);

  // TODO start a timer and in case of overflow turn off pump and display
  // error message in the home screen. Then update the error.log file

  // start timer to check the dispensing process
  MsTimer2::start();

  // count pulses
  while (counter <= pulses && !flowError_flag) {
    currState = digitalRead(_flowPin);

    if (currState != prevState) {
      // TODO doesn't see Global.h
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

  return (flowError_flag) ? (-2) : (0);
}
