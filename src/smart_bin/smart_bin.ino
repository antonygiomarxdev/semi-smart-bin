#include "Config.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "DistanceSensor.h"
#include "ColorSensor.h"
#include "DisplayManager.h"
#include "StateMachine.h"

// Definición de pines
#define PIN_TRIG 9
#define PIN_ECHO 10
#define PIN_S0 4
#define PIN_S1 5
#define PIN_S2 6
#define PIN_S3 7
#define PIN_SENSOR_OUT 8
#define PIN_LED 12
#define PIN_SERVO 2


LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
DistanceSensor ds(PIN_TRIG, PIN_ECHO);
ColorSensor cs(PIN_S0, PIN_S1, PIN_S2, PIN_S3, PIN_SENSOR_OUT);
DisplayManager dm(lcd);
StateMachine fsm(ds, cs, dm, servo);

void setup() {
  if (DEBUG || DEEP_DEBUG) {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) {}
  }

  servo.attach(PIN_SERVO);
  ds.begin();
  cs.begin();
  dm.begin();
  fsm.begin();
}

void loop() {
  fsm.tick();
}