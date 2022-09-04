#include <Wire.h>
#include <RTClib.h>

// SLC -> A5;
// SDA -> A4;
RTC_DS1307 rtc;
 
void setup()
{
  Wire.begin();
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find rtc");
    // Serial.flush();
    while (1) delay(10);
  }
  
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(__DATE__, __TIME__));
  //  rtc.adjust(DateTime(2022, 8, 31, 15, 2, 0));    
  }

  DateTime now = rtc.now();

  Serial.print(now.year());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second());
  Serial.println();
}
 
void loop() {

}
