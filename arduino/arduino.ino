
#include <Wire.h>
#include "Log.h"
#include "Vcc.h"
#include "WDT.h"
#include "Utils.h"
#include "Storage.h"
#include "Bluetooth.h"

const byte ledPin = 9; // Original is 13 of built-in led.
const byte magneticPin = 2;
const byte btStatePin = 7;
int times = 0;
String lastTimes = "";
volatile bool hasNewTimes = false;

unsigned long lastTriggerTime = 0;
unsigned long lastValidTriggerTime = 0;

Vcc vcc = Vcc();

void magneticTriggered() {
  int state = digitalRead(magneticPin);
//  Serial.println(state);
  if (state == LOW) {
    hasNewTimes = true;    
  }
}

void print2(const char *str) { // TODO Remove it.
  Serial.print(str);
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  while (!Serial)
    delay(100);

  BT.begin();

  wdt.begin();

  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 4);
  
  pinMode(magneticPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(magneticPin), magneticTriggered, FALLING);

  storage.begin();
  // storage.setTime(1662291921, 8 * 3600);

  // float v = vcc.Read_Volts();
  // Serial.print("VCC: "); Serial.println(v);

  // Serial.println(utils.freeMemory(), DEC);
  
  // TODO Remove following test code.
  // storage.add();
  // storage.loadFile("4", print2);

  // logger.println(1);
  // logger.println(1234567890L);
  // logger.println(F("123456"));

  // storage.log("test");
  // storage.loadFile("log");
  // storage.listFiles();
  // storage.eraseAll();

  // long timestamp = 1662291921;
  // tm *t = utils.timestamp2tm(timestamp);
  // Serial.println(t->tm_year);
  // Serial.println(t->tm_mon);
  // Serial.println(t->tm_mday);
}
 
void loop() {
  const long now = millis();

  BT.monitor();

  if (hasNewTimes) {
    hasNewTimes = false;
    wdt.updateTimeGate(now);
    BT.wakeUp(); // TODO Wait up when BT connected.
    analogWrite(ledPin, 4);
    
    const int now = millis();
    if ((now - lastTriggerTime) > 50) { // 50ms for bounce.
      lastTriggerTime = now;

      if ((now - lastValidTriggerTime) > 500) { // 500ms for press interval.
        lastValidTriggerTime = now;

        analogWrite(ledPin, 128);
        delay(200);
        analogWrite(ledPin, 4);

        times += 1;
        Serial.println(times);
        BT.print(String(times).c_str());

        String str = String("Times: ");
        str.concat(times);

        storage.add();

        long totalSleepTime = wdt.getTotalSleepTime();
        if (totalSleepTime) {
          wdt.resetTotalSleepTime();
          storage.setTime(storage.m_timeOffset + totalSleepTime);
        }
        
        // if ((times % 5) == 0) { // TODO Remove it.
        //   storage.loadFile("4", print2);
        // }
      }
    } 
  }

  if (wdt.shouldSleep(now)) {
    digitalWrite(ledPin, LOW);
    BT.sleep();
    delay(100);
    wdt.sleep();
  } else {
    wdt.reset();
  }
}
