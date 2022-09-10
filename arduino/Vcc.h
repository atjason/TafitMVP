// https://github.com/Yveaux/Arduino_Vcc

#ifndef VCC_H
#define VCC_H

#include "Arduino.h"

class Vcc
{
  public:
    Vcc();

    byte checkVoltPercentIfNecessary(long);
    byte checkVoltPercent();

    bool isLowVolt();

    float getVolts();
};

extern Vcc vcc;

#endif
