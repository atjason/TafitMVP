#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"

class Utils
{
  public:
    Utils();

    int freeMemory();
};

extern Utils utils;

#endif
