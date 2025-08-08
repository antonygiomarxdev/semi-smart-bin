#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "Config.h"
#include "Types.h"
#include "DistanceSensor.h"
#include "ColorSensor.h"
#include "DisplayManager.h"
#include "StateMachine.h"

#define PIN_TRIG 9
#define PIN_ECHO 10
#define PIN_S0 4
#define PIN_S1 5
#define PIN_S2 6
#define PIN_S3 7
#define PIN_SENSOR_OUT 8
#define PIN_SERVO 2
#define PIN_SENSOR_LED 12

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
DistanceSensor ds(PIN_TRIG, PIN_ECHO);
ColorSensor cs(PIN_S0, PIN_S1, PIN_S2, PIN_S3, PIN_SENSOR_OUT, PIN_SENSOR_LED);
DisplayManager dm(lcd);
StateMachine fsm(ds, cs, dm, servo);

void setup() {
  // Inicializa siempre el Serial
  Serial.begin(SERIAL_BAUD);
  while (!Serial) {}
  Serial.println(F(">> SERIAL INICIADO"));

  servo.attach(PIN_SERVO);
  ds.begin();
  cs.begin();
  dm.begin();
  fsm.begin();
}

void loop() {
  fsm.tick();
}