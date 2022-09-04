/*
W25Q32 32Mbit FLASH外扩存储模块（SPI接口）

Arduino 的 uno 引脚：
CS----------10
MOSI（DI）---11
MISO（DO）---12
CLK---------13
VCC 3.3V
*/

#include <SerialFlash.h>
#include <SPI.h>

// 文件大小
#define FSIZE 256

// 文件名
const char *filename = "test.txt";

// 文件内容
const char *contents = "Hello World.";

// 片选引脚
const int FlashChipSelect = 10;

void listFiles() {
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

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100);

  // 初始化SPI Flash芯片
  SerialFlash.begin(FlashChipSelect);
  while (!SerialFlash.ready()) delay(10);

  listFiles();

  // SerialFlash.eraseAll();
  // while (SerialFlash.ready() == false) {
  //   delay(500);  // wait, 30 seconds to 2 minutes for most chips
  // }

  // 如果文件已存在，则输出提示
  // 如果文件不存在，则创建文件并写入

  if(SerialFlash.exists(filename)) {
    Serial.println("File " + String(filename) + " already exists");

    char buffer[256];
    SerialFlashFile file = SerialFlash.open(filename);
    file.read(buffer, 256);
    for (int i = 0; i < 256; i++) {
      if (buffer[i] == -1) {
        buffer[i] = 0;
        break;
      }
    }
    Serial.print("File content:");
    Serial.println(buffer);

  } else {
    Serial.println("Creating file " + String(filename));

    SerialFlash.createErasable(filename, FSIZE);
    SerialFlashFile file = SerialFlash.open(filename);

    file.write(contents, strlen(contents) + 1);
    Serial.println("String \"" + String(contents) + "\" written to file " + String(filename));
  }
}

void loop() {

}
