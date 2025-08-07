#pragma once
#include <Arduino.h>
#include "Config.h"

class DistanceSensor {
public:
  DistanceSensor(uint8_t trigPin, uint8_t echoPin);
  void begin();
  /// Devuelve distancia en cm
  float readCm();

private:
  uint8_t _trig, _echo;
};
