// DisplayManager.h
#pragma once
#include <LiquidCrystal_I2C.h>

class DisplayManager {
public:
  explicit DisplayManager(LiquidCrystal_I2C &lcd);

  /// Inicializa la pantalla en setup()
  void begin();

  /// Muestra dos líneas (0–16 chars). Evita refrescar si no cambian.
  void show(const char *line1, const char *line2 = "");

  /// Forzar encendido del backlight
  void backlightOn();

  /// Forzar apagado del backlight
  void backlightOff();

private:
  LiquidCrystal_I2C &_lcd;
  char _prev1[17], _prev2[17];

  void _clearIfNeeded(const char *l1, const char *l2);
};
