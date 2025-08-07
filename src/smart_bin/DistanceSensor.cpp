#include "DistanceSensor.h"
#include "Config.h"

// Opcional, pon al inicio para usar los macros de log
#if DEBUG
#define LOGD(msg) Serial.println(msg)
#else
#define LOGD(msg) \
  do { \
  } while (0)
#endif
#if DEEP_DEBUG
#define LOGDD(msg) Serial.println(msg)
#else
#define LOGDD(msg) \
  do { \
  } while (0)
#endif

DistanceSensor::DistanceSensor(uint8_t trigPin, uint8_t echoPin)
  : _trig(trigPin), _echo(echoPin) {}

void DistanceSensor::begin() {
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
}

float DistanceSensor::readCm() {
  digitalWrite(_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);

  // Leemos duración en microsegundos
  long duration = pulseIn(_echo, HIGH, 30000);

  LOGDD(String("[DEEP] US raw duration = ") + duration + " µs");

  float cm = (duration == 0) ? 999.0f : (duration * 0.034f) / 2.0f;
  LOGD(String("[DBG] Distance = ") + cm + " cm");

  return cm;
}
