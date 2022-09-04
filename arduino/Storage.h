/*
  W25Q32 32Mbit FLASH外扩存储模块（SPI接口）

  Arduino 的 uno 引脚：
  CS----------10
  MOSI（DI）---11
  MISO（DO）---12
  CLK---------13
  VCC 3.3V

  // SPI.setSCK(13);
  // SPI.setMISO(12);
  // SPI.setMOSI(11);
  // //SPI.begin;
  // //SPISettings(8000000, MSBFIRST, SPI_MODE0);
*/

#ifndef STORAGE_H
#define STORAGE_H

#include "Arduino.h"

class Storage
{
  public:
    Storage();

    void begin();

    void setTimeOffset(long);

    void add();

    void loadFile(const char *, void (*fn)(const char *) = NULL);

    void loadHistory(byte fromDay, byte toDay, long fromTime, void (*fn)(const char *));

    void log(const char *str);

    void listFiles();

    void eraseAll();

  private:
    char *m_configFile;
    long m_timeOffset;

    void loadConfig();
    void saveConfig();

    void saveStr(long timestamp, String str);
};

extern Storage storage;

#endif
