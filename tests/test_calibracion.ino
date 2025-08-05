/*
 * Test de Calibración - Smart Bin Semi-Inteligente
 * 
 * Este archivo contiene funciones de prueba para calibrar
 * el sistema de clasificación de basura semi-inteligente.
 */

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === CONFIGURACIÓN DE PRUEBA ===
#define DEBUG true
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

// === FUNCIONES DE PRUEBA ===

void setup() {
  Serial.begin(9600);
  inicializarPines();
  inicializarComponentes();
  
  Serial.println("=== TEST DE CALIBRACIÓN SMART BIN SEMI-INTELIGENTE ===");
  mostrarMenu();
}

void loop() {
  if (Serial.available()) {
    char comando = Serial.read();
    procesarComando(comando);
  }
}

void inicializarPines() {
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
}

void inicializarComponentes() {
  servo.attach(SERVO_PIN);
  servo.write(0);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Calibracion");
}

void mostrarMenu() {
  Serial.println("\n=== MENÚ DE PRUEBAS ===");
  Serial.println("1. Test Sensor Ultrasónico");
  Serial.println("2. Test Sensor de Color");
  Serial.println("3. Test Servomotor");
  Serial.println("4. Test LCD");
  Serial.println("5. Test LED");
  Serial.println("6. Calibración de Color");
  Serial.println("7. Test Completo");
  Serial.println("0. Salir");
  Serial.println("Ingrese opción: ");
}

void procesarComando(char comando) {
  switch (comando) {
    case '1':
      testSensorUltrasonico();
      break;
    case '2':
      testSensorColor();
      break;
    case '3':
      testServomotor();
      break;
    case '4':
      testLCD();
      break;
    case '5':
      testLED();
      break;
    case '6':
      calibracionColor();
      break;
    case '7':
      testCompleto();
      break;
    case '0':
      Serial.println("Saliendo...");
      break;
    default:
      Serial.println("Opción inválida");
      mostrarMenu();
      break;
  }
}

void testSensorUltrasonico() {
  Serial.println("\n=== TEST SENSOR ULTRASÓNICO ===");
  
  for (int i = 0; i < 10; i++) {
    float distancia = medirDistancia();
    Serial.print("Medición ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(distancia);
    Serial.println(" cm");
    delay(500);
  }
  
  mostrarMenu();
}

void testSensorColor() {
  Serial.println("\n=== TEST SENSOR DE COLOR ===");
  
  digitalWrite(LED_ILUMINACION, HIGH);
  delay(1000);
  
  for (int i = 0; i < 5; i++) {
    Serial.print("Medición ");
    Serial.print(i + 1);
    Serial.println(":");
    
    int red = medirColorPromedio(LOW, LOW);
    int green = medirColorPromedio(HIGH, HIGH);
    int blue = medirColorPromedio(LOW, HIGH);
    
    Serial.print("  Rojo: ");
    Serial.println(red);
    Serial.print("  Verde: ");
    Serial.println(green);
    Serial.print("  Azul: ");
    Serial.println(blue);
    
    float total = red + green + blue;
    if (total > 0) {
      Serial.print("  Normalizado - R:");
      Serial.print(red / total, 3);
      Serial.print(" G:");
      Serial.print(green / total, 3);
      Serial.print(" B:");
      Serial.println(blue / total, 3);
    }
    
    delay(1000);
  }
  
  digitalWrite(LED_ILUMINACION, LOW);
  mostrarMenu();
}

void testServomotor() {
  Serial.println("\n=== TEST SERVOMOTOR ===");
  
  Serial.println("Abriendo...");
  servo.write(90);
  delay(2000);
  
  Serial.println("Cerrando...");
  servo.write(0);
  delay(2000);
  
  Serial.println("Test completado");
  mostrarMenu();
}

void testLCD() {
  Serial.println("\n=== TEST LCD ===");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test LCD");
  lcd.setCursor(0, 1);
  lcd.print("Funcionando OK");
  
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Bin");
  lcd.setCursor(0, 1);
  lcd.print("Sistema Listo");
  
  Serial.println("Test LCD completado");
  mostrarMenu();
}

void testLED() {
  Serial.println("\n=== TEST LED ===");
  
  for (int i = 0; i < 5; i++) {
    Serial.print("LED ON - Iteración ");
    Serial.println(i + 1);
    digitalWrite(LED_ILUMINACION, HIGH);
    delay(500);
    
    Serial.print("LED OFF - Iteración ");
    Serial.println(i + 1);
    digitalWrite(LED_ILUMINACION, LOW);
    delay(500);
  }
  
  Serial.println("Test LED completado");
  mostrarMenu();
}

void calibracionColor() {
  Serial.println("\n=== CALIBRACIÓN DE COLOR SEMI-INTELIGENTE ===");
  Serial.println("Coloca un objeto de color conocido cerca del sensor");
  Serial.println("Presiona cualquier tecla para medir...");
  Serial.println("NOTA: Este sistema solo clasifica por color, no por material");
  
  while (!Serial.available()) {
    digitalWrite(LED_ILUMINACION, HIGH);
    delay(100);
    digitalWrite(LED_ILUMINACION, LOW);
    delay(100);
  }
  
  Serial.read(); // Limpiar buffer
  
  digitalWrite(LED_ILUMINACION, HIGH);
  delay(1000);
  
  int red = medirColorPromedio(LOW, LOW);
  int green = medirColorPromedio(HIGH, HIGH);
  int blue = medirColorPromedio(LOW, HIGH);
  
  Serial.println("Valores medidos:");
  Serial.print("  Rojo: ");
  Serial.println(red);
  Serial.print("  Verde: ");
  Serial.println(green);
  Serial.print("  Azul: ");
  Serial.println(blue);
  
  float total = red + green + blue;
  if (total > 0) {
    float rNorm = red / total;
    float gNorm = green / total;
    float bNorm = blue / total;
    
    Serial.println("Valores normalizados:");
    Serial.print("  R: ");
    Serial.println(rNorm, 3);
    Serial.print("  G: ");
    Serial.println(gNorm, 3);
    Serial.print("  B: ");
    Serial.println(bNorm, 3);
    
    Serial.println("Código para agregar al array:");
    Serial.print("  {");
    Serial.print(rNorm, 3);
    Serial.print(", ");
    Serial.print(gNorm, 3);
    Serial.print(", ");
    Serial.print(bNorm, 3);
    Serial.println(", COLOR_CODIGO},");
  }
  
  digitalWrite(LED_ILUMINACION, LOW);
  mostrarMenu();
}

void testCompleto() {
  Serial.println("\n=== TEST COMPLETO ===");
  
  // Test sensor ultrasónico
  Serial.println("1. Probando sensor ultrasónico...");
  float distancia = medirDistancia();
  Serial.print("   Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  // Test sensor de color
  Serial.println("2. Probando sensor de color...");
  digitalWrite(LED_ILUMINACION, HIGH);
  delay(500);
  
  int red = medirColorPromedio(LOW, LOW);
  int green = medirColorPromedio(HIGH, HIGH);
  int blue = medirColorPromedio(LOW, HIGH);
  
  Serial.print("   Valores RGB: ");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);
  
  digitalWrite(LED_ILUMINACION, LOW);
  
  // Test servomotor
  Serial.println("3. Probando servomotor...");
  servo.write(90);
  delay(1000);
  servo.write(0);
  
  // Test LCD
  Serial.println("4. Probando LCD...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Completo OK");
  lcd.setCursor(0, 1);
  lcd.print("Sistema Listo");
  
  Serial.println("Test completo finalizado");
  mostrarMenu();
}

// === FUNCIONES AUXILIARES ===

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