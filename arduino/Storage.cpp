// https://github.com/PaulStoffregen/SerialFlash

#include "Storage.h"
#include "Arduino.h"

#include <SPI.h>
#include <SerialFlash.h>

/*
  TODO: Use enum to store start and length.
  `config` map:
  0~9: timeOffset, timestamp in seconds, e.g., '1661933405'.
 */
const int Config_TimeOffset_Start = 0;
const int Config_TimeOffset_Length = 10;

const char *logFilename = "log";
SerialFlashFile logFile;

Storage::Storage() {
  m_configFile = "config";
  m_timeOffset = 0;
}

void Storage::begin() {

  const byte FlashChipSelect = 10;
  SerialFlash.begin(FlashChipSelect);
  while (!SerialFlash.ready()) delay(50); // TODO: timeout

  loadConfig();

  if(SerialFlash.exists(logFilename)) {
    logFile = SerialFlash.open(logFilename);
    logFile.erase();
  } else {
    SerialFlash.createErasable(logFilename, 4096);
    logFile = SerialFlash.open(logFilename);
  }
}

void Storage::loadConfig() {

  if(!SerialFlash.exists(m_configFile)) {
    Serial.println("No config.");
    return;
  }

  SerialFlashFile file = SerialFlash.open(m_configFile);

  char str[Config_TimeOffset_Length];
  file.seek(Config_TimeOffset_Start);
  file.read(str, Config_TimeOffset_Length);
  m_timeOffset = atol(str);

  Serial.println("Load config:");
  Serial.print("m_timeOffset: "); Serial.println(m_timeOffset);
}

void Storage::saveConfig() {

  if(!SerialFlash.exists(m_configFile)) {
    // In fact erasable file is at least block size, e.g., 64KB.
    SerialFlash.createErasable(m_configFile, 4096);
    while (!SerialFlash.ready()) delay(50); // TODO: timeout
  }

  SerialFlashFile file = SerialFlash.open(m_configFile);
  file.erase();

  char str[Config_TimeOffset_Length];
  ltoa(m_timeOffset, str, DEC);
  file.seek(Config_TimeOffset_Start);
  file.write(str, Config_TimeOffset_Length);

  Serial.println("Save config:");
  Serial.print("m_timeOffset: "); Serial.println(m_timeOffset);
}

void Storage::loadFile(const char *filename, void (*fn)(const char *), String *str) {

  Serial.print("Load "); Serial.println(filename);

  if(!SerialFlash.exists(filename)) {
    Serial.println("Not exist.");
    return;
  }
  
  SerialFlashFile file = SerialFlash.open(filename);

  const byte length = 16;
  char buffer[length + 1];
  long size = file.size();
  bool done = false;

  while(!done && file.position() < size) {
    file.read(buffer, length);
    
    for (int i = 0; i < length; i++) {
      const char c = buffer[i];
      if (c == -1) {
        buffer[i] = '\0';
        done = true;
        break;
      }
    }

    if (fn) (*fn)(buffer);
    if (str) str->concat(buffer);
  }

  if (fn) (*fn)(""); // Send empty to let `fn` flush.
  Serial.println("\nEnd");
}

void Storage::loadHistory(byte fromDay, byte toDay, long fromTime, void (*fn)(const char *)) {
  
  const byte monthDays = 31;
  for (byte day = fromDay + monthDays; day <= (toDay + monthDays); day++) {
    String filename = String(day % (monthDays + 1) + 1);

    if(!SerialFlash.exists(filename.c_str())) {
      Serial.print(F("No file: ")); Serial.println(filename);
      continue;
    }
    
    const byte length = 10; // TODO 10 to enum.
    char buffer[length];

    SerialFlashFile file = SerialFlash.open(filename.c_str());
    file.seek(0);
    file.read(buffer, length);

    const long startTimestamp = atol(buffer);
    if (startTimestamp < fromTime) continue; // Which should be last month's data.

    loadFile(filename.c_str(), fn);
  }
}

void Storage::setTimeOffset(long timeOffset) {
  m_timeOffset = timeOffset;
  saveConfig();
}

//////////////////////////////////////////////////////////////////

long addLastTime = 0;
String addStr = "";
char *addLastFilename = "";
long addLastPosition = 0;

void Storage::add() {

  if (m_timeOffset < 1660000000) {
    Serial.println(F("No timeOffset."));
    return;
  }

  String addStr2Save = "";

  long now = long(millis() / 1000) + m_timeOffset;
  long offset = now - addLastTime;
  Serial.println("Add.");
  Serial.println("Now: " + String(now));
  Serial.println("Offset: " + String(offset));

  if (offset > 255) { // max of unsigned char: 255. // TODO Save to const param.
    addStr2Save = addStr;
    addStr = String(now) + "_";
    addLastTime = now;

  } else {
    addLastTime = now;
    addStr += String(offset) + "_";
    if (addStr.length() > 16) { // TODO Save to const param.
      addStr2Save = addStr;
      addStr = "";
    }
  }

  if (addStr2Save.length()) {
    saveStr(addLastTime, addStr2Save);
  }
}

void Storage::saveStr(long timestamp, String str) {
  Serial.println("Save");
  Serial.print("timestamp: "); Serial.println(timestamp);
  Serial.print("str: "); Serial.println(str);

  const int day = 0; // TODO from timestamp to day.
  char *filename = "1";

  if (addLastFilename != filename) {
    addLastFilename = filename;

    if(!SerialFlash.exists(filename)) {
      // Create if doesn't exist.
      // In fact erasable file is at least block size, e.g., 64KB.
      Serial.println("Create file:" + String(filename));
      SerialFlash.createErasable(filename, 4096);

    } else {
      // Erease if it's day of last month.
      SerialFlashFile file = SerialFlash.open(filename);

      const byte length = 10;
      char buffer[length];
      file.seek(0);
      file.read(buffer, length);

      const long startTimestamp = atol(buffer);
      if (startTimestamp < (timestamp - 3600 * 24)) {
        Serial.println("Old timestamp:" + startTimestamp);
        Serial.println("Erease file:" + String(filename));
        file.erase();
        addLastPosition = 0;

      } else {
        // Find last position to append.

        const int length = 16;
        char buffer[length];
        long position = 0;
        long size = file.size();

        while(position < size) {
          file.seek(position);
          file.read(buffer, length);
          
          for (int i = 0; i < length; i++) {
            const char c = buffer[i];
            if (c == '\0' || c == 0 || c == -1) {
              addLastPosition = position + i;
              position = size; // to break nested loop.
              Serial.println("Find append position: " + String(addLastPosition));
              break;
            }
          }

          position += length;
        }
      }
    }
  }

  Serial.println("Add to:");
  Serial.println("File: " + String(filename));
  Serial.println("Position: " + String(addLastPosition));

  SerialFlashFile file = SerialFlash.open(filename);
  file.seek(addLastPosition);
  file.write(str.c_str(), str.length());
  addLastPosition = file.position();
}

//////////////////////////////////////////////////////////////////

void Storage::log(const char *str) {
  logFile.write(str, strlen(str));
}

void Storage::listFiles() {
  SerialFlash.opendir();
  while (1) {
    char filename[64];
    uint32_t filesize;

    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
      Serial.print("  ");
      Serial.print(filename);
      // spaces(20 - strlen(filename));
      Serial.print("  ");
      Serial.print(filesize);
      Serial.print(" bytes");
      Serial.println();
    } else {
      break; // no more files
    }
  }
}

void Storage::eraseAll() {
  SerialFlash.eraseAll();
  while (!SerialFlash.ready()) {
    delay(500);  // wait, 30 seconds to 2 minutes for most chips
  }
}

Storage storage = Storage();
