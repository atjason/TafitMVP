#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "Arduino.h"

class Bluetooth
{
  public:
    Bluetooth();

    void begin();

    void monitor();

    void print(const char *);

    void sleep();
    void wakeUp();

  private:
    void parseBTCommand(String &, String &);

    bool splitParamString(String, char, byte, String[]);
};

extern Bluetooth BT;

#endif