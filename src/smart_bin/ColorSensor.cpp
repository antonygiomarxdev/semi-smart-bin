#include "ColorSensor.h"

ColorSensor::ColorSensor(uint8_t s0, uint8_t s1,
                         uint8_t s2, uint8_t s3,
                         uint8_t outPin,
                         uint8_t ledPin)
  : _s0(s0), _s1(s1), _s2(s2), _s3(s3), _out(outPin), _led(ledPin) {}

void ColorSensor::begin() {
  pinMode(_s0, OUTPUT);
  pinMode(_s1, OUTPUT);
  pinMode(_s2, OUTPUT);
  pinMode(_s3, OUTPUT);
  pinMode(_out, INPUT);
  pinMode(_led, OUTPUT);
  digitalWrite(_s0, HIGH);
  digitalWrite(_s1, LOW);
  ledOff();  
}

void ColorSensor::ledOn() {
  digitalWrite(_led, HIGH);
}

void ColorSensor::ledOff() {
  digitalWrite(_led, LOW);
}

int ColorSensor::measure(bool s2Level, bool s3Level) {
  ledOn();
  digitalWrite(_s2, s2Level);
  digitalWrite(_s3, s3Level);
  delayMicroseconds(50);
  long sum = 0;
  for (int i = 0; i < COLOR_SAMPLE_COUNT; ++i) {
    sum += pulseIn(_out, LOW);
    delayMicroseconds(5);
  }
  ledOff();
  return sum / COLOR_SAMPLE_COUNT;
}
