#ifndef BLUETOOTH_H
#define BLUETOOTH_H

class Bluetooth
{
  public:
    Bluetooth();

    void begin();

    void monitor();

    void print(const char *);

  private:
    void parseBTCommand(String &, String &);

    bool splitParamString(String, char, byte, String[]);
};

extern Bluetooth BT;

#endif