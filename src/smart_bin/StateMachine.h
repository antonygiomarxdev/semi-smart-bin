#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "DistanceSensor.h"
#include "ColorSensor.h"
#include "DisplayManager.h"
#include "Config.h"
#include "Types.h"

// — Runtime‐checked logging macros —
// DEBUG-level (state transitions, key events)
#define LOGD(msg) \
  do { \
    if (DEBUG) Serial.println(msg); \
  } while (0)
// DEEP_DEBUG-level (per-tick, raw sensor values…)
#define LOGDD(msg) \
  do { \
    if (DEEP_DEBUG) Serial.println(msg); \
  } while (0)

class StateMachine {
public:
  StateMachine(DistanceSensor &ds,
               ColorSensor &cs,
               DisplayManager &dm,
               Servo &servo);

  /// Llamar en setup()
  void begin();

  /// Llamar en loop()
  void tick();

private:
  /// Lógica interna, sin retardos
  void update();

  DistanceSensor &_dist;
  ColorSensor &_color;
  DisplayManager &_disp;
  Servo &_servo;

  EstadoSistema _state;
  unsigned long _tsState;
  unsigned long _tsDetect;
  unsigned long _tsClassified;

  // — Debounce de presencia —
  bool _prevPresent;
  int _presenceCount;
  int _absenceCount;

  // — Cambio de color para re-análisis —
  float _lastR, _lastG, _lastB;

  // Manejo de CLASIFICADO
  void _handleClassified(unsigned long now, bool present);
  bool _hasColorChanged();

  // Transiciones
  void _toWaiting();
  void _toDetecting();
  void _toAnalyzing();
  void _toClassified();
  void _toInactive();

  // Clasificación por distancia Euclídea²
  ColorCodigo _classifyColor(float r, float g, float b);
};
