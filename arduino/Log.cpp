#include "Log.h"
#include "Storage.h"

Log::Log() {

}

void Log::print(int n) {
  print(String(n).c_str());
}

void Log::print(long n) {
  print(String(n).c_str());
}

void Log::print(const char *str) {
  // NOTE: Comment out to disable Serial or storage.
  Serial.print(str);
  storage.log(str);
}

void Log::print(const __FlashStringHelper *ifsh) {
  byte size = 16, length = 0;
  char buffer[size];

  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  while(true) {    
    char c = pgm_read_byte(p++);
    buffer[length++] = c;

    if (c == 0 || length == (size - 1)) {
      buffer[length] = 0;
      print(buffer);
      length = 0;

      if (c == 0) break;
    }
  }
}

void Log::println(int n) {
  print(n);
  print("\n");
}

void Log::println(long n) {
  print(n);
  print("\n");
}

void Log::println(const char *str) {
  print(str);
  print("\n");
}

void Log::println(const __FlashStringHelper *ifsh) {
  print(ifsh);
  print("\n");
}

Log logger = Log();
