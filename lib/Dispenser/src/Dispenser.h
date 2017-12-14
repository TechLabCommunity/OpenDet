/*
 * FlowSensor.h - Library for XXXX flow sensors
 * Created by Michele Brunelli on 12 dec 2017
 * Ask the creator if you want to use this library, email me at brunellim94@gmail.com
 */

#ifndef DISPENSER_H
#define DISPENSER_H

class Dispenser{
  public:

    Dispenser(int, int, int, int);
    int getPin();
    int getPulses();
    int getPump();
    void dispense(double);

  private:

    int _btnPin;
    int _flowPin;
    int _pulsesPerLiter;
    int _pumpPin;
};

#endif
