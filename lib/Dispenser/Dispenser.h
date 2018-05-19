/*
 * FlowSensor.h - Library for XXXX flow sensors
 * Created by Michele Brunelli on 12 dec 2017
 * Ask the creator if you want to use this library, email me at
 * brunellim94@gmail.com
 */

#ifndef DISPENSER_H
#define DISPENSER_H

#include <Global.h>
#define MAX_DISPENSER 20
// defined in millilitres.
enum FIXED_QUANTITY { LT = 1000, HALFLT = 500, QTYLT = 250 };

class Dispenser {
 private:
  static Dispenser listDispenser[MAX_DISPENSER];
  static uint indexList;

 public:
  Dispenser();
  Dispenser(uint, uint, uint, uint, String, uint);
  uint getPulses();
  String getDetName();
  uint getPrice();
  uint getButton();
  uint getNumberId();
  void dispense(FIXED_QUANTITY);
  void dispense(uint = 1000);
  static Dispenser& getDispFromButton(uint);

 protected:
  uint _pulsesPerLiter, _btnPin, _flowPin, _pumpPin, _detPrice, _numberId;
  String _detName;

};

#endif
