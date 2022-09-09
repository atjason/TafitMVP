#ifndef WDT_H
#define WDT_H

class WDT
{
  public:
    WDT();

    void begin();

    bool shouldSleep(long);

    void sleep();

    void updateTimeGate(long);

    long getTotalSleepTime();
    void resetTotalSleepTime();

    void WDT::reset();
};

extern WDT wdt;

#endif
