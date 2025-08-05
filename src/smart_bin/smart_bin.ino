#include <Servo.h>
#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === CONFIGURACIÓN GENERAL ===
#define DEBUG true
#define THRESHOLD 0.35
#define MUESTRAS 10

// === DEFINICIÓN DE PINES ===
#define TRIG 9
#define ECHO 10
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#define LED_ILUMINACION 12
#define SERVO_PIN 3

// === OBJETOS ===
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === ENUM PARA COLORES ===
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

// === ESTRUCTURA COLOR ===
struct ColorReferencia {
  float r, g, b;
  ColorCodigo codigo;
};

// === COLORES CALIBRADOS (ajustar según pruebas) ===
ColorReferencia colores[] = {
  {0.150, 0.442, 0.407, ORG_ROJO},
  {0.256, 0.264, 0.479, ORG_VERDE},
  {0.262, 0.293, 0.446, ORG_AMARILLO},
  {0.375, 0.386, 0.239, ORG_VIOLETA},
  {0.220, 0.391, 0.389, ORG_NARANJA},
  {0.423, 0.350, 0.227, INORG_AZUL},
  {0.365, 0.346, 0.288, INORG_BLANCO},
  {0.356, 0.340, 0.303, INORG_NEGRO},
  {0.260, 0.337, 0.403, INORG_CAFE}
};

// === FUNCIONES AUXILIARES ===
void log(String msg) {
  if (DEBUG) Serial.println(msg);
}

String colorTexto(ColorCodigo cod) {
  switch (cod) {
    case ORG_ROJO: return "ROJO ORGÁNICO";
    case ORG_VERDE: return "VERDE ORGÁNICO";
    case ORG_AMARILLO: return "AMARILLO ORGÁNICO";
    case ORG_VIOLETA: return "VIOLETA ORGÁNICO";
    case ORG_NARANJA: return "NARANJA ORGÁNICO";
    case INORG_AZUL: return "AZUL INORGÁNICO";
    case INORG_BLANCO: return "BLANCO INORGÁNICO";
    case INORG_NEGRO: return "NEGRO INORGÁNICO";
    case INORG_CAFE: return "CAFÉ INORGÁNICO";
    default: return "INDEFINIDO";
  }
}

int medirColorPromedio(bool s2, bool s3) {
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  delay(50);
  long suma = 0;
  for (int i = 0; i < MUESTRAS; i++) {
    suma += pulseIn(sensorOut, LOW);
    delay(5);
  }
  return suma / MUESTRAS;
}

ColorCodigo clasificarColorPorDistancia(float r, float g, float b) {
  float minDist = 999.0;
  ColorCodigo mejor = COLOR_INDEFINIDO;

  for (int i = 0; i < sizeof(colores) / sizeof(colores[0]); i++) {
    float dr = r - colores[i].r;
    float dg = g - colores[i].g;
    float db = b - colores[i].b;
    float dist = sqrt(dr * dr + dg * dg + db * db);
    if (dist < minDist) {
      minDist = dist;
      mejor = colores[i].codigo;
    }
  }

  log("📐 Distancia mínima al color: " + String(minDist, 3));
  return (minDist < THRESHOLD) ? mejor : COLOR_INDEFINIDO;
}

float medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracion = pulseIn(ECHO, HIGH, 30000);
  if (duracion == 0) return 999;
  return duracion * 0.034 / 2;
}

// === SETUP ===
void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(LED_ILUMINACION, OUTPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(LED_ILUMINACION, LOW);
  
  servo.attach(SERVO_PIN);
  servo.write(0);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema listo");
}

// === LOOP PRINCIPAL ===
void loop() {
  float distancia = medirDistancia();

  if (distancia <= 10) {
    log("🟢 Objeto detectado cerca");
    digitalWrite(LED_ILUMINACION, HIGH);
    servo.write(90);

    int red = medirColorPromedio(LOW, LOW);
    int green = medirColorPromedio(HIGH, HIGH);
    int blue = medirColorPromedio(LOW, HIGH);

    log("📊 Crudos - R: " + String(red) + " | G: " + String(green) + " | B: " + String(blue));

    float total = red + green + blue;
    if (total == 0) {
      log("⚠ Error: Total de color es 0. Revisa el sensor.");
      return;
    }

    float rNorm = red / total;
    float gNorm = green / total;
    float bNorm = blue / total;

    log("🎯 Normalizado - R: " + String(rNorm, 3) + 
        " | G: " + String(gNorm, 3) + 
        " | B: " + String(bNorm, 3));

    ColorCodigo cod = clasificarColorPorDistancia(rNorm, gNorm, bNorm);
    String color = colorTexto(cod);

    log("🎨 Color detectado: " + color);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detectado:");
    lcd.setCursor(0, 1);
    if (cod == COLOR_INDEFINIDO) {
      lcd.print("INDEFINIDO");
    } else if (cod <= ORG_NARANJA) {
      lcd.print("ORGÁNICO");
    } else {
      lcd.print("INORGÁNICO");
    }

  } else {
    digitalWrite(LED_ILUMINACION, LOW);
    servo.write(0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Esperando objeto");
    log("🔍 Sin objeto cerca (Distancia: " + String(distancia, 1) + " cm)");
  }

  delay(500);
} 