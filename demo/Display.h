#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SLC -> A5;
// SDA -> A4;

class Display
{
  public:
    Display(void);

    void begin(void);

    void setBaseContent(void);

    void updateContent(const String &);

  protected:
    String m_lastStr;
    Adafruit_SSD1306 m_display;
};

#endif
