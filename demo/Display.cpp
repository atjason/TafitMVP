
#include "Display.h"

Display::Display() {
  const uint8_t SCREEN_WIDTH = 128; // 设置OLED宽度, 单位:像素
  const uint8_t SCREEN_HEIGHT = 64; // 设置OLED高度, 单位:像素
  m_display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
  m_lastStr = "";
}

void Display::begin() {
  // 初始化OLED并设置其IIC地址为 0x3C
  m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // TODO May fail.
}

void Display::setBaseContent() {
  // 清除屏幕
  m_display.clearDisplay();
 
  // 设置字体颜色,白色可见
  m_display.setTextColor(WHITE);
 
  //设置字体大小
  m_display.setTextSize(2);
 
  //设置光标位置
  m_display.setCursor(0, 0);
  m_display.print("Tafit");

  m_display.display();
}

void Display::updateContent(const String &str) {
  
  m_display.setCursor(0, 20);
  m_display.setTextColor(BLACK);
  m_display.print(m_lastStr);

  m_display.setCursor(0, 20);
  m_display.setTextColor(WHITE);
  m_display.print(str);
  m_lastStr = str;  

  m_display.display();
}
