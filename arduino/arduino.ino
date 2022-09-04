
#include <Wire.h>
#include "Log.h"
#include "Vcc.h"
#include "Utils.h"
#include "Storage.h"
#include "Bluetooth.h"

int ledPin = 8; // Original is 13 of built-in led.
int magneticPin = 2;
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

  pinMode(ledPin, OUTPUT);
  
  pinMode(magneticPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(magneticPin), magneticTriggered, FALLING);

  storage.begin();
  // storage.setTimeOffset(1661933405);

  // float v = vcc.Read_Volts();
  // Serial.print("VCC: "); Serial.println(v);

  // Serial.println(utils.freeMemory(), DEC);
  
  // TODO Remove following test code.
  // storage.add();
  storage.loadFile("1", print2);

  // logger.println(1);
  // logger.println(1234567890L);
  // logger.println(F("123456"));

  // storage.log("test");
  // storage.loadFile("log");
  // storage.listFiles();
  // storage.eraseAll();
}
 
void loop() {
  BT.monitor();

  if (hasNewTimes) {
    hasNewTimes = false;
    
    const int now = millis();
    if ((now - lastTriggerTime) > 50) { // 50ms for bounce.
      lastTriggerTime = now;

      if ((now - lastValidTriggerTime) > 500) { // 500ms for press interval.
        lastValidTriggerTime = now;

        times += 1;
        Serial.println(times);
        BT.print(String(times).c_str());

        String str = String("Times: ");
        str.concat(times);

        storage.add();
        
        if ((times % 5) == 0) { // TODO Remove it.
          storage.loadFile("1", print2);
        }
      }
    }    
  }
}
