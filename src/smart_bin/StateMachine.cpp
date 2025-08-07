// StateMachine.cpp
#include "StateMachine.h"
#include "Config.h"
#include <float.h>  // FLT_MAX

// Tabla de referencia de colores
static const float colorRefs[][3] = {
  { 0.150f, 0.442f, 0.407f },  // ORG_ROJO
  { 0.256f, 0.264f, 0.479f },  // ORG_VERDE
  { 0.262f, 0.293f, 0.446f },  // ORG_AMARILLO
  { 0.375f, 0.386f, 0.239f },  // ORG_VIOLETA
  { 0.220f, 0.391f, 0.389f },  // ORG_NARANJA
  { 0.423f, 0.350f, 0.227f },  // INORG_AZUL
  { 0.365f, 0.346f, 0.288f },  // INORG_BLANCO
  { 0.356f, 0.340f, 0.303f },  // INORG_NEGRO
  { 0.260f, 0.337f, 0.403f }   // INORG_CAFE
};
static const int nColorRefs = sizeof(colorRefs) / sizeof(colorRefs[0]);

StateMachine::StateMachine(DistanceSensor &ds,
                           ColorSensor &cs,
                           DisplayManager &dm,
                           Servo &servo)
  : _dist(ds), _color(cs), _disp(dm), _servo(servo), _state(ESPERANDO), _tsState(0), _tsDetect(0), _tsClassified(0), _prevPresent(false) {}

void StateMachine::begin() {
  _tsState = millis();
  _disp.show("SISTEMA LISTO", "");
}

void StateMachine::tick() {
  static unsigned long _lastTick = 0;
  unsigned long now = millis();
  if (now - _lastTick < UPDATE_INTERVAL_MS) return;
  _lastTick = now;
  update();
}

void StateMachine::update() {
  // 1) Leer distancia y tiempo
  float d = _dist.readCm();
  unsigned long now = millis();

  // 2) Debounce de presencia
  bool rawPresent = (d <= DIST_THRESHOLD_CM);
  if (rawPresent) {
    _presenceCount++;
    _absenceCount = 0;
  } else {
    _absenceCount++;
    _presenceCount = 0;
  }
  bool present = _prevPresent;
  if (_presenceCount >= PRESENCE_COUNT_THRESHOLD) {
    present = true;
  } else if (_absenceCount >= PRESENCE_COUNT_THRESHOLD) {
    present = false;
  }
  bool flank = (present && !_prevPresent);

  // 3) Inactividad
  if (_state == ESPERANDO && now - _tsState >= INACTIVITY_TIMEOUT_MS) {
    _toInactive();
    _prevPresent = present;
    return;
  }

  // 4) FSM
  switch (_state) {
    case ESPERANDO:
      if (flank) {
        if (DEBUG) Serial.println("DBG ▶ Transición a DETECTANDO");
        _toDetecting();
      }
      break;

    case DETECTANDO:
      if (!present) {
        _toWaiting();
      } else if (now - _tsDetect >= ANALYSIS_TIME_MS) {
        _toAnalyzing();
      }
      break;

    case ANALIZANDO:
      {
        int r = _color.measure(LOW, LOW);
        int g = _color.measure(HIGH, HIGH);
        int b = _color.measure(LOW, HIGH);
        float sum = r + g + b;
        if (sum <= 0) {
          _toWaiting();
          break;
        }
        ColorCodigo cod = _classifyColor(r / sum, g / sum, b / sum);
        const char *name = (cod <= ORG_NARANJA) ? "ORGANICO" : "INORGANICO";
        _disp.show("DETECTADO:", name);
      }
      _toClassified();
      break;

    case CLASIFICADO:
      unsigned long dt = now - _tsClassified;
      if (dt < RESULT_TIME_MS) {
        break;
      }


      if (!present) _toWaiting();
      break;

    case INACTIVO:
      if (present) _toWaiting();
      break;
  }

  // 5) Guardar estado de presencia
  _prevPresent = present;
}

void StateMachine::_toWaiting() {
  _disp.backlightOn();
  _servo.write(0);
  _disp.show("ESPERANDO", "OBJETO...");
  _state = ESPERANDO;
  _tsState = millis();
}

void StateMachine::_toDetecting() {
  _disp.backlightOn();
  _disp.show("DETECTANDO...", "");
  _state = DETECTANDO;
  _tsDetect = millis();
}

void StateMachine::_toAnalyzing() {
  _disp.backlightOn();
  _disp.show("ANALIZANDO...", "");
  _state = ANALIZANDO;
}

void StateMachine::_toClassified() {
  _disp.backlightOn();
  _state = CLASIFICADO;
  _tsClassified = millis();
}

void StateMachine::_toInactive() {
  _servo.write(0);

  // 1) Mostrar mensaje (enciende backlight)
  _disp.show("INACTIVO", "ACERCARSE");

// 2) Esperar un momento para que el usuario vea el mensaje
#if !PRODUCTION_MODE
  delay(INACTIVE_DISPLAY_MS);
#endif

  // 3) Apagar la luz de fondo
  _disp.backlightOff();

  // 4) Fija el estado y timestamp
  _state = INACTIVO;
  _tsState = millis();
}

ColorCodigo StateMachine::_classifyColor(float r, float g, float b) {
  float best2 = FLT_MAX;
  int bestI = -1;
  for (int i = 0; i < nColorRefs; ++i) {
    float dr = r - colorRefs[i][0];
    float dg = g - colorRefs[i][1];
    float db = b - colorRefs[i][2];
    float d2 = dr * dr + dg * dg + db * db;
    if (d2 < best2) {
      best2 = d2;
      bestI = i;
    }
  }
  return (best2 < COLOR_DIST_THRESHOLD * COLOR_DIST_THRESHOLD)
           ? static_cast<ColorCodigo>(bestI)
           : COLOR_INDEFINIDO;
}
