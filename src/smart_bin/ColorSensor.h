#pragma once

#include <Arduino.h>
#include "Config.h"

class ColorSensor {
public:
  ColorSensor(uint8_t s0, uint8_t s1,
              uint8_t s2, uint8_t s3,
              uint8_t outPin,
              uint8_t ledPin);
  void begin();
  int measure(bool s2Level, bool s3Level);
  void ledOn();
  void ledOff();
private:
  uint8_t _s0, _s1, _s2, _s3, _out, _led;
};
