#include "StateMachine.h"
#include "Config.h"
#include <float.h>  // FLT_MAX

// — Tabla de colores normalizados —
static const float colorRefs[][3] = {
  { 0.150f, 0.442f, 0.407f },
  { 0.256f, 0.264f, 0.479f },
  { 0.262f, 0.293f, 0.446f },
  { 0.375f, 0.386f, 0.239f },
  { 0.220f, 0.391f, 0.389f },
  { 0.423f, 0.350f, 0.227f },
  { 0.365f, 0.346f, 0.288f },
  { 0.356f, 0.340f, 0.303f },
  { 0.260f, 0.337f, 0.403f }
};
static const int nColorRefs = sizeof(colorRefs) / sizeof(colorRefs[0]);

StateMachine::StateMachine(DistanceSensor &ds,
                           ColorSensor &cs,
                           DisplayManager &dm,
                           Servo &servo)
  : _dist(ds), _color(cs), _disp(dm), _servo(servo), _state(ESPERANDO), _tsState(0), _tsDetect(0), _tsClassified(0), _prevPresent(false), _presenceCount(0), _absenceCount(0), _lastR(0), _lastG(0), _lastB(0) {}

void StateMachine::begin() {
  Serial.begin(SERIAL_BAUD);
  _tsState = millis();
  _disp.begin();
  _disp.show("SISTEMA LISTO", "");
}

void StateMachine::tick() {
  static unsigned long lastTick = 0;
  unsigned long now = millis();
  if (now - lastTick < UPDATE_INTERVAL_MS) return;
  lastTick = now;

  LOGDD(String("[DEEP] FSM.tick @") + now);
  update();
}

void StateMachine::update() {
  unsigned long now = millis();
  float d = _dist.readCm();

  // — Debounce de presencia —
  bool rawPres = (d <= DIST_THRESHOLD_CM);
  if (rawPres) {
    _presenceCount++;
    _absenceCount = 0;
  } else {
    _absenceCount++;
    _presenceCount = 0;
  }
  bool present = _prevPresent;
  if (_presenceCount >= PRESENCE_COUNT_THRESHOLD) present = true;
  else if (_absenceCount >= PRESENCE_COUNT_THRESHOLD) present = false;

  bool flank = present && !_prevPresent;

  LOGDD(String("[DEEP] upd st=") + _state
        + " dist=" + String(d, 1)
        + " pres=" + (present ? "1" : "0")
        + " dtSt=" + (now - _tsState));

  // — Timeout inactividad —
  if (_state == ESPERANDO && now - _tsState >= INACTIVITY_TIMEOUT_MS) {
    LOGD("[DBG] → INACTIVO (timeout)");
    _toInactive();
    _prevPresent = present;
    return;
  }

  switch (_state) {
    case ESPERANDO:
      if (flank) {
        LOGD("[DBG] ESPERANDO → DETECTANDO");
        _toDetecting();
      }
      break;

    case DETECTANDO:
      if (!present) {
        LOGD("[DBG] DETECTANDO → ESPERANDO (objeto retirado)");
        _toWaiting();
      } else if (now - _tsDetect >= ANALYSIS_TIME_MS) {
        LOGD("[DBG] DETECTANDO → ANALIZANDO");
        _toAnalyzing();
      }
      break;

    case ANALIZANDO:
      {
        LOGD("[DBG] ANALIZANDO color");
        int r = _color.measure(LOW, LOW);
        int g = _color.measure(HIGH, HIGH);
        int b = _color.measure(LOW, HIGH);
        float sum = r + g + b;
        if (sum <= 0) {
          LOGD("[DBG] Error sensor color");
          _toWaiting();
          break;
        }
        ColorCodigo cod = _classifyColor(r / sum, g / sum, b / sum);
        const char *label = (cod <= ORG_NARANJA) ? "ORGANICO" : "INORGANICO";
        LOGD(String("[DBG] Resultado: ") + label);
        _disp.show("DETECTADO:", label);
      }
      _toClassified();
      break;

    case CLASIFICADO:
      _handleClassified(now, present);
      break;

    case INACTIVO:
      if (present) {
        LOGD("[DBG] INACTIVO → ESPERANDO (reactivado)");
        _toWaiting();
      }
      break;
  }

  _prevPresent = present;
}

void StateMachine::_handleClassified(unsigned long now, bool present) {
  // 1) Reanálisis inmediato si cambia color
  if (_hasColorChanged()) {
    LOGD("[DBG] CLASIFICADO: cambio de color → ANALIZANDO");
    _toAnalyzing();
    return;
  }

  // 2) Mantener resultado fijo al menos RESULT_TIME_MS
  if (now - _tsClassified < RESULT_TIME_MS) {
    LOGDD(String("[DEEP] CLASIFICADO fijo dt=") + (now - _tsClassified));
    return;
  }

  // 3) Tras 7s, si el objeto se retiró → ESPERANDO
  if (!present) {
    LOGD("[DBG] CLASIFICADO → ESPERANDO (objeto retirado tras 7s)");
    _toWaiting();
    return;
  }

  // 4) Si sigue presente sin cambio, permanecer en CLASIFICADO
  LOGDD("[DEEP] CLASIFICADO: persistir");
}

bool StateMachine::_hasColorChanged() {
  int rr = _color.measure(LOW, LOW),
      gg = _color.measure(HIGH, HIGH),
      bb = _color.measure(LOW, HIGH);
  float sum = float(rr + gg + bb);
  if (sum <= 0) return false;

  float nr = rr / sum, ng = gg / sum, nb = bb / sum;
  float dr = nr - _lastR, dg = ng - _lastG, db = nb - _lastB;
  return (dr * dr + dg * dg + db * db) > COLOR_CHANGE_THRESHOLD2;
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
  _color.ledOn();
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
  _color.ledOff();
  _state = CLASIFICADO;
  _tsClassified = millis();
  // Guardar normalización
  int r = _color.measure(LOW, LOW),
      g = _color.measure(HIGH, HIGH),
      b = _color.measure(LOW, HIGH);
  float s = float(r + g + b);
  if (s > 0) {
    _lastR = r / s;
    _lastG = g / s;
    _lastB = b / s;
  }
}

void StateMachine::_toInactive() {
  _disp.show("INACTIVO", "ACERCARSE");
#if !PRODUCTION_MODE
  delay(INACTIVE_DISPLAY_MS);
#endif
  _disp.backlightOff();
  _servo.write(0);
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
