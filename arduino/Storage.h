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
    const char *m_version = "0.1";

  public:
    Storage();

    void begin();

    void setTime(long timeOffset, long tzSeconds = 0);

    void add();

    void loadFile(const char *, void (*fn)(const char *) = NULL, String *str = NULL);

    void loadHistory(byte fromDay, byte toDay, long fromTime, void (*fn)(const char *));

    void log(const char *str);

    void listFiles();

    void removeFile(const char *filename);

    void eraseAll();

  public:
    long m_timeOffset;
    int m_tzSeconds;

  private:
    char *m_configFile;

    void loadConfig();
    void saveConfig();

    void saveStr(long timestamp, String str);
};

extern Storage storage;

#endif
