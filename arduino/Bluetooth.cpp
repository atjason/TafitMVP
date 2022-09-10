#include <SoftwareSerial.h>

#include "Bluetooth.h"
#include "Vcc.h"
#include "Utils.h"
#include "Storage.h"

// Pin 4 为 RX，接蓝牙 TXD;
// Pin 5 为 TX，接蓝牙 RXD;
SoftwareSerial _BT(4, 5);

const byte enablePin = 8;

String btOp = "";
String btParam = "";
String *btCurrent;
String btLoadLastPostfix = "";

void writeData(const char *buffer) {
  String str = btLoadLastPostfix + buffer;
  const char separator = '_';
  int index = str.lastIndexOf(separator);
  if (index < 0) {
    btLoadLastPostfix = "";
  } else {
    index += 1;
    btLoadLastPostfix = str.substring(index);
    str = str.substring(0, index);
  }
  if (!str.length()) return;

  String command = "`LDR&" + str + "#";
  _BT.print(command.c_str());
}

/////////////////////////////////////////////////////////////

Bluetooth::Bluetooth() {

}

void Bluetooth::begin() {
  _BT.begin(115200); // TODO Check state and wait.
  pinMode(enablePin, OUTPUT);
  sleep();
}

void Bluetooth::monitor() {
  while (_BT.available()) {
    const char c = _BT.read();
    if (c == '`') {
      btOp = ""; // op start;
      btCurrent = &btOp;
    } else if (c == '&') {
      btParam = ""; // op end, param start;
      btCurrent = &btParam;

    } else if (c == '#') { // param end;
      Serial.print(F("op: ")); Serial.println(btOp);
      Serial.print(F("param: ")); Serial.println(btParam);

      btParam.trim();
      parseBTCommand(btOp, btParam);

    } else {
      btCurrent->concat(c);
    }
  }  
}

void Bluetooth::print(const char *buffer) {
  _BT.print(buffer);
}

void Bluetooth::parseBTCommand(String &btOp, String &btParam) {

  if (btOp != "LD" && btOp != "VCC" && btOp != "STT" && btOp != "LDT" && btOp != "RMF" && btOp != "RMA") {
    String result = "`OPE&" + btOp + "#";
    Serial.println(result);
    _BT.println(result);
    return;
  }

  byte paramCount = 0;
  if (btOp == "LD") paramCount = 3;
  if (btOp == "STT") paramCount = 2;
  if (btOp == "RMF") paramCount = 1;
  
  String params[paramCount];
  const char separator = '_';

  bool paramOk = utils.splitString(btParam, separator, paramCount, params);
  
  if (!paramOk) {
    String result = "`PAE&" + btOp + "_" + btParam + "#";
    Serial.println(result);
    _BT.println(result);
    return;
  }

  if (btOp == "LD") {
    Serial.println("Will load data.");
    byte fromDay = params[0].toInt();
    byte toDay = params[1].toInt();
    long fromTime = params[2].toInt();
    storage.loadHistory(fromDay, toDay, fromTime, writeData);
    
  } else if (btOp == "VCC") {
    String str = "`VCCR&" + String(vcc.getVolts()) + "#";
    print(str.c_str());
  
  } else if (btOp == "STT") {
    long timeOffset = params[0].toInt();
    long tzSeconds = params[1].toInt();
    storage.setTime(timeOffset, tzSeconds);
    print("OK");
  
  } else if (btOp == "LDT") {
    String str = "`LDT&" + String(storage.m_timeOffset) + "_" + String(storage.m_tzSeconds) + "#";
    print(str.c_str());
  
  } else if (btOp == "RMF") {
    storage.removeFile(params[0].c_str());
    print("OK");
  
  } else if (btOp == "RMA") {
    storage.eraseAll();
    print("OK");
  }
}

void Bluetooth::sleep() {
  digitalWrite(enablePin, HIGH);
}

void Bluetooth::wakeUp() {
  digitalWrite(enablePin, LOW);
}

Bluetooth BT = Bluetooth();
