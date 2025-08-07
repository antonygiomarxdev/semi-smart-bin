// StateMachine.h
#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "DistanceSensor.h"
#include "ColorSensor.h"
#include "DisplayManager.h"
#include "Config.h"
#include "Types.h"

class StateMachine {
public:
  StateMachine(DistanceSensor &ds,
               ColorSensor    &cs,
               DisplayManager &dm,
               Servo          &servo);

  /// Inicializa timestamps y pantalla. Llamar en setup().
  void begin();

  /// Llamar en loop(): regula cadencia e invoca update().
  void tick();

private:
  /// Lógica FSM pura (sin delays, sin debug)
  void update();

  DistanceSensor  &_dist;
  ColorSensor     &_color;
  DisplayManager  &_disp;
  Servo           &_servo;

  EstadoSistema    _state;
  unsigned long    _tsState;
  unsigned long    _tsDetect;
  unsigned long    _tsClassified;

  // Debounce de presencia
  bool   _prevPresent   = false;
  int    _presenceCount = 0;
  int    _absenceCount  = 0;
  static constexpr int PRESENCE_COUNT_THRESHOLD = 3;

  // Transiciones
  void _toWaiting();
  void _toDetecting();
  void _toAnalyzing();
  void _toClassified();
  void _toInactive();

  // Clasificación de color
  ColorCodigo _classifyColor(float r, float g, float b);
};
