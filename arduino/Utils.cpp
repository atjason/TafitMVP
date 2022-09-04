#include "Utils.h"

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int Utils::freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

tm* Utils::timestamp2tm(long timestamp, int tzSeconds) {
  // https://www.nongnu.org/avr-libc/user-manual/group__avr__time.html
  // struct tm {
  //   int tm_sec;    /* seconds 0-59 */
  //   int tm_min;    /* minutes 0-59 */
  //   int tm_hour;   /* hours 0-23 */
  //   int tm_mday;   /* day of the month 1-31 */
  //   int tm_mon;    /* month 0-11 */
  //   int tm_year;   /* year *距离1990的年数 */
  //   int tm_wday;   /* day of the week 0-6 */
  //   int tm_yday;   /* day in the year 0-365 */
  //   int tm_isdst;  /* daylight saving time */
  // };
  time_t _timestamp = timestamp - UNIX_OFFSET + tzSeconds;   
  struct tm *t = gmtime(&_timestamp);
  t->tm_year += 1900;
  t->tm_mon += 1;
  return t;
}
