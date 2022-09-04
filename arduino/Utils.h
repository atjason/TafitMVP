#ifndef UTILS_H
#define UTILS_H

#include <time.h>

class Utils
{
  public:
    Utils();

    int freeMemory();

    tm* timestamp2tm(long, int tzSeconds = 8 * 3600);
};

extern Utils utils;

#endif
