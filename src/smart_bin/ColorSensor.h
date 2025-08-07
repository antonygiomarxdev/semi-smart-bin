// ColorSensor.h
#pragma once
#include <Arduino.h>

class ColorSensor {
public:
  ColorSensor(uint8_t s0, uint8_t s1,
              uint8_t s2, uint8_t s3,
              uint8_t outPin);

  /// Inicializa pines y configura frecuencia S0/S1.
  void begin();

  /// Mide intensidad en el canal dado (LOW/LOW, HIGH/HIGH, LOW/HIGH).
  int measure(bool s2Level, bool s3Level);

private:
  const uint8_t _s0, _s1, _s2, _s3, _outPin;
  static constexpr int _samples = 10;
};
