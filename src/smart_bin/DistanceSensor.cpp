#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(uint8_t trigPin, uint8_t echoPin)
  : _trig(trigPin), _echo(echoPin) {}

void DistanceSensor::begin() {
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
}

float DistanceSensor::readCm() {
  // Trigger
  digitalWrite(_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);

  // Leer pulso
  long duration = pulseIn(_echo, HIGH, 30000);
#if DEEP_DEBUG
  Serial.println(String("[DEEP] USµs=") + duration);
#endif

  float cm = (duration == 0) ? 999.0f : (duration * 0.034f) / 2.0f;
#if DEBUG
  Serial.println(String("[DBG] Dist=") + cm + " cm");
#endif

  return cm;
}
