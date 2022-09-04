#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <time.h>

class Utils
{
  public:
    Utils();

    int freeMemory();

    tm* timestamp2tm(long, int tzSeconds = 8 * 3600);

    bool splitString(String, char, byte, String[]);
};

extern Utils utils;

#endif
