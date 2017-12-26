#ifndef DETSCREEN_H
#define DETSCREEN_H

#include <Global.h>
#include <LiquidCrystal_I2C.h>

class DetScreen : public LiquidCrystal_I2C{

  private:

    unsigned int address, cols, rows;
    unsigned long timer = 0;
    uint slider = 0;

  protected:

    unsigned int inline calcol(const String&);

    void inline smartprint(const String&, uint);

    static String cents_to_string(const uint&);

    void slideprint(const String&, uint);

  public:

    DetScreen(uint, uint, uint);

    void start_system(const String& version);

    void main_screen(uint, uint);

    void clear();

};

#endif
