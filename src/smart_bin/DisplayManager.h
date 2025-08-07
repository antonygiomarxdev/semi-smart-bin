#pragma once

#include <LiquidCrystal_I2C.h>

class DisplayManager {
public:
  explicit DisplayManager(LiquidCrystal_I2C &lcd);
  void begin();
  void show(const char *line1, const char *line2 = "");
  void backlightOn();
  void backlightOff();

private:
  LiquidCrystal_I2C &_lcd;
  char _prev1[17], _prev2[17];
  void _clearIfNeeded(const char *l1, const char *l2);
};
