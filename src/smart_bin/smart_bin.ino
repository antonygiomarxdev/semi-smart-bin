#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <float.h>  // Para FLT_MAX

// ——————————————— MODOS DE EJECUCIÓN ———————————————
// Versión de producción: quita logs y minimiza retardos
static constexpr bool PRODUCTION_MODE = false;
// Debug profundo: registra cada iteración de loop
static constexpr bool DEEP_DEBUG = false && !PRODUCTION_MODE;
// Logs generales de cambio de estado y errores
static constexpr bool DEBUG = !PRODUCTION_MODE;

// ————————————— CONFIGURACIONES —————————————
static const int SERIAL_BAUD = 9600;
static const int BT_BAUD = 9600;

static const unsigned long ANALYSIS_TIME_MS = 1000;
static const unsigned long RESULT_TIME_MS = 7000;
static const unsigned long INACTIVITY_TIMEOUT_MS = 15000;

static const int DIST_THRESHOLD_CM = 10;
static const float COLOR_DIST_THRESHOLD = 0.35f;
static const int COLOR_SAMPLE_COUNT = 10;

// Pines
static const int PIN_TRIG = 9;
static const int PIN_ECHO = 10;
static const int PIN_S0 = 4;
static const int PIN_S1 = 5;
static const int PIN_S2 = 6;
static const int PIN_S3 = 7;
static const int PIN_SENSOR_OUT = 8;
static const int PIN_LED = 12;
static const int PIN_SERVO = 2;
static const int PIN_BT_RX = 3;
static const int PIN_BT_TX = 11;

// ————————————— ENUM Y ESTRUCTURAS —————————————
enum ColorCodigo {
  ORG_ROJO,
  ORG_VERDE,
  ORG_AMARILLO,
  ORG_VIOLETA,
  ORG_NARANJA,
  INORG_AZUL,
  INORG_BLANCO,
  INORG_NEGRO,
  INORG_CAFE,
  COLOR_INDEFINIDO
};

enum EstadoSistema {
  ESPERANDO = 0,
  DETECTANDO = 1,
  ANALIZANDO = 2,
  CLASIFICADO = 3,
  INACTIVO = 4
};

struct ColorReferencia {
  float r, g, b;
  ColorCodigo codigo;
};

// Tabla de referencia normalizada
static const ColorReferencia referencias[] = {
  { 0.150f, 0.442f, 0.407f, ORG_ROJO },
  { 0.256f, 0.264f, 0.479f, ORG_VERDE },
  { 0.262f, 0.293f, 0.446f, ORG_AMARILLO },
  { 0.375f, 0.386f, 0.239f, ORG_VIOLETA },
  { 0.220f, 0.391f, 0.389f, ORG_NARANJA },
  { 0.423f, 0.350f, 0.227f, INORG_AZUL },
  { 0.365f, 0.346f, 0.288f, INORG_BLANCO },
  { 0.356f, 0.340f, 0.303f, INORG_NEGRO },
  { 0.260f, 0.337f, 0.403f, INORG_CAFE }
};

// ————————————— GLOBALES —————————————
Servo servoMotor;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial bluetooth(PIN_BT_RX, PIN_BT_TX);

EstadoSistema estado = ESPERANDO;
unsigned long timestampEstado = 0;
unsigned long timestampDetect = 0;
unsigned long timestampClasificado = 0;

bool objetoPresentePrev = false;
String ultimaLinea1 = "";
String ultimaLinea2 = "";

// ————————————— PROTOTIPOS —————————————
void cambiarEstado(EstadoSistema nuevo);
void logDebug(const String &msg);
void mostrarLCD(const String &l1, const String &l2);
void moverServo(int angulo);
int leerColorPromedio(bool s2, bool s3);
ColorCodigo clasificarColor(float r, float g, float b);
float medirDistanciaCM();

// ————————————— SETUP —————————————
void setup() {
  if (DEBUG) {
    Serial.begin(SERIAL_BAUD);
    bluetooth.begin(BT_BAUD);
  }

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_S0, OUTPUT);
  pinMode(PIN_S1, OUTPUT);
  pinMode(PIN_S2, OUTPUT);
  pinMode(PIN_S3, OUTPUT);
  pinMode(PIN_SENSOR_OUT, INPUT);
  pinMode(PIN_LED, OUTPUT);

  // Configuracion sensor de color
  digitalWrite(PIN_S0, HIGH);
  digitalWrite(PIN_S1, LOW);

  servoMotor.attach(PIN_SERVO);
  moverServo(0);

  lcd.init();
  lcd.backlight();
  mostrarLCD("SISTEMA LISTO", "");
  cambiarEstado(ESPERANDO);
}

// ————————————— LOOP —————————————
void loop() {
  float distancia = medirDistanciaCM();
  bool presenteNow = (distancia <= DIST_THRESHOLD_CM);

  if (DEEP_DEBUG) {
    logDebug("LOOP est=" + String(estado)
             + " dist=" + String(distancia, 1)
             + " pres=" + (presenteNow ? "1" : "0")
             + " dt_cla=" + ((estado == CLASIFICADO) ? String(millis() - timestampClasificado) : "-"));
  }

  // Salida forzada de CLASIFICADO tras RESULT_TIME_MS
  if (estado == CLASIFICADO) {
    if (millis() - timestampClasificado >= RESULT_TIME_MS) {
      digitalWrite(PIN_LED, LOW);
      moverServo(0);
      mostrarLCD("ESPERANDO", "OBJETO...");
      cambiarEstado(ESPERANDO);
    }
  }

  // Reactivacion desde INACTIVO
  if (estado == INACTIVO && presenteNow) {
    lcd.backlight();
    mostrarLCD("ACTIVADO", "ESPERANDO OBJETO");
    moverServo(0);
    cambiarEstado(ESPERANDO);
    objetoPresentePrev = true;
    delay(500);
    return;
  }

  // Maquina de estados
  switch (estado) {
    case ESPERANDO:
      if (presenteNow && !objetoPresentePrev) {
        mostrarLCD("ANALIZANDO...", "");
        cambiarEstado(DETECTANDO);
        timestampDetect = millis();
      }
      break;

    case DETECTANDO:
      if (!presenteNow) {
        mostrarLCD("ESPERANDO", "OBJETO...");
        moverServo(0);
        cambiarEstado(ESPERANDO);
      } else if (millis() - timestampDetect >= ANALYSIS_TIME_MS) {
        cambiarEstado(ANALIZANDO);
      }
      break;

    case ANALIZANDO:
      if (!presenteNow) {
        digitalWrite(PIN_LED, LOW);
        moverServo(0);
        mostrarLCD("ESPERANDO", "OBJETO...");
        cambiarEstado(ESPERANDO);
        break;
      }
      digitalWrite(PIN_LED, HIGH);
      moverServo(90);
      logDebug("ANALIZANDO COLOR...");
      {
        int r = leerColorPromedio(LOW, LOW);
        int g = leerColorPromedio(HIGH, HIGH);
        int b = leerColorPromedio(LOW, HIGH);
        float total = float(r + g + b);
        if (total <= 0.0f) {
          logDebug("ERROR DE LECTURA");
          mostrarLCD("ERROR SENSOR", "");
          if (!PRODUCTION_MODE) delay(500);
          cambiarEstado(ESPERANDO);
          break;
        }
        ColorCodigo cod = clasificarColor(r / total, g / total, b / total);
        String tipo = (cod == COLOR_INDEFINIDO) ? "INDEFINIDO"
                      : (cod <= ORG_NARANJA)    ? "ORGANICO"
                                                : "INORGANICO";
        mostrarLCD("DETECTADO:", tipo);
        logDebug("COLOR: " + tipo);
      }
      timestampClasificado = millis();
      cambiarEstado(CLASIFICADO);
      break;

    case CLASIFICADO:
      // salida gestionada arriba
      break;

    case INACTIVO:
      // gestionado al inicio
      break;
  }

  // Inactividad en ESPERANDO
  if (estado == ESPERANDO
      && millis() - timestampEstado >= INACTIVITY_TIMEOUT_MS) {
    mostrarLCD("INACTIVO", "ACERCARSE");
    if (!PRODUCTION_MODE) delay(1500);
    lcd.noBacklight();
    cambiarEstado(INACTIVO);
  }

  objetoPresentePrev = presenteNow;
  if (!PRODUCTION_MODE) delay(50);
}

// ————————————— FUNCIONES —————————————
void cambiarEstado(EstadoSistema nuevo) {
  if (estado != nuevo) {
    estado = nuevo;
    timestampEstado = millis();
    logDebug("ESTADO -> " + String(nuevo));
  }
}

void logDebug(const String &msg) {
  if (!DEBUG) return;
  Serial.println(msg);
  if (!PRODUCTION_MODE) delay(5);
}

void mostrarLCD(const String &l1, const String &l2) {
  if (l1 == ultimaLinea1 && l2 == ultimaLinea2) return;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(l1);
  lcd.setCursor(0, 1);
  lcd.print(l2);
  ultimaLinea1 = l1;
  ultimaLinea2 = l2;
}

void moverServo(int angulo) {
  servoMotor.write(angulo);
}

int leerColorPromedio(bool s2, bool s3) {
  digitalWrite(PIN_S2, s2);
  digitalWrite(PIN_S3, s3);
  if (!PRODUCTION_MODE) delay(50);
  long sum = 0;
  for (int i = 0; i < COLOR_SAMPLE_COUNT; i++) {
    sum += pulseIn(PIN_SENSOR_OUT, LOW);
    if (!PRODUCTION_MODE) delay(5);
  }
  return sum / COLOR_SAMPLE_COUNT;
}

ColorCodigo clasificarColor(float r, float g, float b) {
  float minDist = FLT_MAX;
  ColorCodigo mejor = COLOR_INDEFINIDO;
  for (auto &ref : referencias) {
    float dr = r - ref.r, dg = g - ref.g, db = b - ref.b;
    float d = sqrt(dr * dr + dg * dg + db * db);
    if (d < minDist) {
      minDist = d;
      mejor = ref.codigo;
    }
  }
  logDebug("DIST_MIN = " + String(minDist, 3));
  return (minDist < COLOR_DIST_THRESHOLD) ? mejor : COLOR_INDEFINIDO;
}

float medirDistanciaCM() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  long dur = pulseIn(PIN_ECHO, HIGH, PRODUCTION_MODE ? 10000 : 30000);
  return (dur == 0) ? 999.0f : (dur * 0.034f) / 2.0f;
}
