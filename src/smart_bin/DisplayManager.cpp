// DisplayManager.cpp
#include "DisplayManager.h"
#include <string.h>

DisplayManager::DisplayManager(LiquidCrystal_I2C &lcd)
  : _lcd(lcd)
{
  _prev1[0] = _prev2[0] = '\0';
}

void DisplayManager::begin() {
  _lcd.init();
  _lcd.backlight();
}

void DisplayManager::_clearIfNeeded(const char *l1, const char *l2) {
  if (strcmp(_prev1, l1) != 0 || strcmp(_prev2, l2) != 0) {
    _lcd.clear();
    strncpy(_prev1, l1, 16); _prev1[16] = '\0';
    strncpy(_prev2, l2, 16); _prev2[16] = '\0';
  }
}

void DisplayManager::show(const char *l1, const char *l2) {
  _clearIfNeeded(l1, l2);
  backlightOn();
  _lcd.setCursor(0,0);
  _lcd.print(l1);
  _lcd.setCursor(0,1);
  _lcd.print(l2);
}

void DisplayManager::backlightOn() {
  _lcd.backlight();
}

void DisplayManager::backlightOff() {
  _lcd.noBacklight();
}
