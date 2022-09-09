// http://donalmorrissey.blogspot.com/2010/04/sleeping-arduino-part-5-wake-up-via.html

// 1. setup wdt in `setup`.
// 2. define `ISR` function, which will be called when watchdog timed out.
// 3. enter sleep in some case, e.g., idle for some time.

#include "WDT.h"

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

const int idleDuration = 10000; // 10s
long nextSleepTime = 0;
const int sleepTime = 8000; // 8s
volatile long totalSleepTime = 0;
bool isSleeping = false;

// Watchdog Interrupt Service. 
// This is executed when watchdog timed out.
ISR(WDT_vect) {
  totalSleepTime += sleepTime;
}

WDT::WDT() {

}

void WDT::begin() {

  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */ // For how to set different time, check the url on top.
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);

  updateTimeGate(millis());
}

bool WDT::shouldSleep(long now) {
  return now > nextSleepTime;
}

void WDT::sleep() {
  if (!isSleeping) {
    isSleeping = true;

    totalSleepTime += sleepTime / 2; // Add half of sleep time to currect offset.
  }

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // could also use other mode.
  sleep_enable();
  sleep_mode(); // enter sleep mode.
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); // First thing to do is disable sleep.
  power_all_enable(); // Re-enable the peripherals.
}

void WDT::updateTimeGate(long now) {
  isSleeping = false;
  nextSleepTime = now + idleDuration;
}

long WDT::getTotalSleepTime() {
  return totalSleepTime;
}

void WDT::resetTotalSleepTime() {
  totalSleepTime = 0;
}

void WDT::reset() {
  wdt_reset();
}
