/*
 * Dispenser.h - Library for pump dispenser with flow sensors
 * Created by Michele Brunelli on 12 dec 2017
 * Ask the creator if you want to use this library, email me at
 * brunellim94@gmail.com
 */

#ifndef DISPENSER_H
#define DISPENSER_H

#include <Global.h>

// defined in millilitres.
enum FIXED_QUANTITY { LT = 1000, HALFLT = 500, QTYLT = 250 };

enum DISP_ERR {
  OK = 0,
  TANK_EMPTY = 5,
  QTY_LOW = -2,
  PUMP_ERR = -3,
  PUMP_AND_EMPTY = -4
};

class Dispenser {
 public:
  Dispenser(uint, uint, uint, uint, String, uint, uint, uint);
  uint getPulses();
  void setPulses(const uint);
  String getDetName();
  void setName(const String);
  uint getPrice();
  void setPrice(const uint);
  DISP_ERR dispense(FIXED_QUANTITY);
  DISP_ERR dispense(uint = 1000);
  DISP_ERR pumpErr_reset();
  DISP_ERR fillTank();

  uint calibrate(uint8_t, uint8_t);

 protected:
 private:
  uint _pulsesPerLiter, _btnPin, _flowPin, _pumpPin, _detPrice, _detCnt,
      _detCntMax, _flowTimeout;
  DISP_ERR _error;
  String _detName;
};

#endif
