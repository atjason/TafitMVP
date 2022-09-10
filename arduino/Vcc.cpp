
#include "Vcc.h"
#include "Arduino.h"

#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define ADMUX_VCCWRT1V1 (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define ADMUX_VCCWRT1V1 (_BV(MUX5) | _BV(MUX0))
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define ADMUX_VCCWRT1V1 (_BV(MUX3) | _BV(MUX2))
#else
#define ADMUX_VCCWRT1V1 (_BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
#endif  

#define VCC_PIN A1

const float maxVolts = 4.2;
const float lowVolts = 3.68;
const float minVolts = 3.4;

float volts = maxVolts;
byte voltPercent = 100;

long nextCheckTime = 0;
const long checkInterval = 600 * 1000; // 10min

float readVolts() {
  return analogRead(VCC_PIN) / 1024.0 * 3.3 * 2;
}

float readVolts2() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  if (ADMUX != ADMUX_VCCWRT1V1) {
    ADMUX = ADMUX_VCCWRT1V1;

    // Bandgap reference start-up time: max 70us
    // Wait for Vref to settle.
    delayMicroseconds(350); 
  }
  
  // Start conversion and wait for it to finish.
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC)) {};
    
  // Result is now stored in ADC.
  
  // Calculate Vcc (in V)
  return 1.1*1024.0 / ADC;
}

Vcc::Vcc() {

}

void Vcc::begin() {
  pinMode(VCC_PIN, INPUT);
}

byte Vcc::checkVoltPercentIfNecessary(long now) {
  if (now > nextCheckTime) {
    nextCheckTime = now + checkInterval;
    voltPercent = checkVoltPercent();
  }
  Serial.print("Vcc percent: "); Serial.println(voltPercent); // TODO Remove it.
  return voltPercent;
}

byte Vcc::checkVoltPercent() {
  volts = readVolts();
  Serial.print("Vcc: "); Serial.println(volts); // TODO Remove it.

  const byte listLength = 11;
  const byte percentList[] = {100,90,80,70,60,50,40,30,20,10,0};
  const float voltsList[] = {maxVolts, 4.06,3.98,3.92,3.87,3.82,3.79,3.77,3.74,3.68, minVolts};

  if (volts >= maxVolts) {
    return 100;
  } else if (volts <= minVolts) {
    return 0;
  } else {
    for (byte i = 1; i < listLength; i++) {
      if (volts > voltsList[i]) {
        return byte((volts - voltsList[i]) / (voltsList[i-1] - voltsList[i]) * (percentList[i-1] - percentList[i])) + percentList[i];
      }
    }
  }
  return 0;
}

bool Vcc::isLowVolt() {
  return volts < lowVolts;
}

float Vcc::getVolts() {
  checkVoltPercent();
  return volts;
}

Vcc vcc = Vcc();
