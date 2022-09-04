#ifndef LOG_H
#define LOG_H

#include "Arduino.h"

class Log
{
  public:
    Log();

    void print(int);
    void print(long);
    void print(const char *);
    void print(const __FlashStringHelper *ifsh);

    void println(int);
    void println(long);
    void println(const char *);
    void println(const __FlashStringHelper *ifsh);

  protected:
    // float m_correction;
};

extern Log logger;

#endif
