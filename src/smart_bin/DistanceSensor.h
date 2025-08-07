// DistanceSensor.h
#pragma once
#include <Arduino.h>

class DistanceSensor {
public:
  DistanceSensor(uint8_t trigPin, uint8_t echoPin);

  /// Configura los pines. Llamar en setup().
  void begin();

  /// Devuelve distancia en cm (o 999 si timeout).
  float readCm();

private:
  const uint8_t _trigPin, _echoPin;
  static constexpr unsigned long _timeout = 30000;  // µs
};
