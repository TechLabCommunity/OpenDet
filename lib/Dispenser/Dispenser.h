/*
 * FlowSensor.h - Library for XXXX flow sensors
 * Created by Michele Brunelli on 12 dec 2017
 * Ask the creator if you want to use this library, email me at brunellim94@gmail.com
 */

#ifndef DISPENSER_H
#define DISPENSER_H

#include <Controllino.h>
#include <MsTimer2.h>

//defined in millilitres.
enum FIXED_QUANTITY{
  LT = 1000, HALFLT = 500, QTYLT = 250
};

class Dispenser{

  public:
    Dispenser(unsigned int, unsigned int, unsigned int, unsigned int, String, unsigned int);
    int getPulses();
    String getDetName();
    int getPrice();
    void dispense(FIXED_QUANTITY);

  protected:
    void dispense(unsigned int = 1);

  private:
    unsigned int _pulsesPerLiter, _btnPin, _flowPin, _pumpPin, _detPrice;
    String _detName;
};

#endif
