#include "ColorSensor.h"

ColorSensor::ColorSensor(uint8_t s0, uint8_t s1,
                         uint8_t s2, uint8_t s3,
                         uint8_t outPin)
  : _s0(s0), _s1(s1), _s2(s2), _s3(s3), _out(outPin) {}

void ColorSensor::begin() {
  pinMode(_s0, OUTPUT);
  pinMode(_s1, OUTPUT);
  pinMode(_s2, OUTPUT);
  pinMode(_s3, OUTPUT);
  pinMode(_out, INPUT);
  digitalWrite(_s0, HIGH);
  digitalWrite(_s1, LOW);
}

int ColorSensor::measure(bool s2Level, bool s3Level) {
  digitalWrite(_s2, s2Level);
  digitalWrite(_s3, s3Level);
  delayMicroseconds(50);

  long sum = 0;
  for (int i = 0; i < COLOR_SAMPLE_COUNT; ++i) {
    long pulse = pulseIn(_out, LOW);
    sum += pulse;
#if DEEP_DEBUG
    Serial.println(String("[DEEP] CLRp=") + pulse);
#endif
    delayMicroseconds(5);
  }
  int avg = sum / COLOR_SAMPLE_COUNT;
#if DEBUG
  Serial.println(String("[DBG] CLRavg=") + avg);
#endif
  return avg;
}
