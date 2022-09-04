#include <SoftwareSerial.h>

#include "Bluetooth.h"
#include "Storage.h"

// Pin 10 为 RX，接蓝牙 TXD;
// Pin 11 为 TX，接蓝牙 RXD;
SoftwareSerial _BT(6, 7);

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
  _BT.begin(38400);
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

  if (btOp != "LD") {
    String result = "`OPE&" + btOp + "#";
    Serial.println(result);
    _BT.println(result);
    return;
  }

  byte paramCount = 0;
  if (btOp == "LD") paramCount = 3;
  
  String params[paramCount];
  const char separator = '_';
  bool paramOk;

  if (btOp == "LD") paramOk = splitParamString(btParam, separator, paramCount, params);
  
  if (!paramOk) {
    String result = "`PAE&" + btParam + "#";
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
  }
}

bool Bluetooth::splitParamString(String str, char separator, byte count, String result[]) {

  int start = 0;

  for (byte i = 0; i < count; i++) {
    int to = str.indexOf(separator, start);
    if (to > 0) {
      result[i] = str.substring(start, to);
      start = to + 1;
    } else {
      result[i] = str.substring(start);
      break;
    }
  }

  return result[count - 1].length() > 0;
}

Bluetooth BT = Bluetooth();
