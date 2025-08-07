// DistanceSensor.cpp
#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(uint8_t trigPin, uint8_t echoPin)
 : _trigPin(trigPin), _echoPin(echoPin) {}

void DistanceSensor::begin() {
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

float DistanceSensor::readCm() {
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  long dur = pulseIn(_echoPin, HIGH, _timeout);
  if (dur == 0) return 999.0f;
  return (dur * 0.034f) / 2.0f;
}
