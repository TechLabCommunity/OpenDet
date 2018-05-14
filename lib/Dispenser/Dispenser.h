/*
 * FlowSensor.h - Library for XXXX flow sensors
 * Created by Michele Brunelli on 12 dec 2017
 * Ask the creator if you want to use this library, email me at
 * brunellim94@gmail.com
 */

#ifndef DISPENSER_H
#define DISPENSER_H

#include <Global.h>

// defined in millilitres.
enum FIXED_QUANTITY { LT = 1000, HALFLT = 500, QTYLT = 250 };

class Dispenser {
 public:
  Dispenser(uint, uint, uint, uint, String, uint);
  uint getPulses();
  String getDetName();
  uint getPrice();
  void dispense(FIXED_QUANTITY);
  void dispense(uint = 1000);

 protected:
 private:
  uint _pulsesPerLiter, _btnPin, _flowPin, _pumpPin, _detPrice;
  String _detName;
};

#endif
