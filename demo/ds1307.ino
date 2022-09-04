// https://adafruit.github.io/RTClib/html/index.html

#include <Wire.h>
#include <RTClib.h>

// SLC -> A5;
// SDA -> A4;
RTC_DS1307 rtc;

void setup () {
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find rtc");
    // Serial.flush();
    while (1) delay(10);
  }
  
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(__DATE__, __TIME__));
  //  rtc.adjust(DateTime(2022, 8, 31, 15, 2, 0));    
  }
}

void loop () {

  // https://adafruit.github.io/rtclib/html/class_date_time.html
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(3000);
}