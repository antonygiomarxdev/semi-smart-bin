# Smart Bin - Diagrama de Conexiones

## Componentes del Sistema

### 1. Arduino Uno (Placa Principal)
- **Función**: Controlador principal del sistema
- **Alimentación**: 5V DC

### 2. Sensor de Color TCS3200
- **Función**: Detectar colores de objetos
- **Alimentación**: 5V
- **Comunicación**: Digital

### 3. Sensor Ultrasónico HC-SR04
- **Función**: Medir distancia a objetos
- **Alimentación**: 5V
- **Comunicación**: Digital

### 4. Servomotor SG90
- **Función**: Abrir/cerrar tapa del contenedor
- **Alimentación**: 5V
- **Control**: PWM

### 5. LCD I2C 16x2
- **Función**: Mostrar información del sistema
- **Alimentación**: 5V
- **Comunicación**: I2C

### 6. LED de Iluminación
- **Función**: Iluminar objetos para mejor detección
- **Alimentación**: 5V
- **Control**: Digital

## Conexiones Detalladas

### Arduino Uno ↔ Sensor TCS3200
```
Arduino Pin 4  → TCS3200 S0
Arduino Pin 5  → TCS3200 S1
Arduino Pin 6  → TCS3200 S2
Arduino Pin 7  → TCS3200 S3
Arduino Pin 8  → TCS3200 OUT
Arduino 5V     → TCS3200 VCC
Arduino GND    → TCS3200 GND
```

### Arduino Uno ↔ Sensor HC-SR04
```
Arduino Pin 9  → HC-SR04 TRIG
Arduino Pin 10 → HC-SR04 ECHO
Arduino 5V     → HC-SR04 VCC
Arduino GND    → HC-SR04 GND
```

### Arduino Uno ↔ Servomotor SG90
```
Arduino Pin 3  → Servo SIGNAL
Arduino 5V     → Servo VCC
Arduino GND    → Servo GND
```

### Arduino Uno ↔ LCD I2C
```
Arduino A4 (SDA) → LCD SDA
Arduino A5 (SCL) → LCD SCL
Arduino 5V       → LCD VCC
Arduino GND      → LCD GND
```

### Arduino Uno ↔ LED de Iluminación
```
Arduino Pin 12 → LED +
Arduino GND    → LED -
```

## Diagrama de Pines Arduino

```
Arduino Uno Pinout:
┌─────────────────────────────────┐
│  ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ │
│  │0│ │1│ │2│ │3│ │4│ │5│ │6│ │7│ │ ← Digital Pins
│  └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ │
│  ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ │
│  │8│ │9│ │10││11││12││13││GND││VIN│ │
│  └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ │
│                                     │
│  ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ ┌─┐ │
│  │A0││A1││A2││A3││A4││A5││5V││3V3│ │ ← Analog Pins
│  └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ └─┘ │
└─────────────────────────────────┘
```

## Asignación de Pines por Función

| Pin Arduino | Componente | Función |
|-------------|------------|---------|
| 3 | Servomotor | Control PWM |
| 4 | TCS3200 S0 | Configuración sensor |
| 5 | TCS3200 S1 | Configuración sensor |
| 6 | TCS3200 S2 | Selección filtro |
| 7 | TCS3200 S3 | Selección filtro |
| 8 | TCS3200 OUT | Lectura de color |
| 9 | HC-SR04 TRIG | Trigger ultrasónico |
| 10 | HC-SR04 ECHO | Echo ultrasónico |
| 12 | LED | Iluminación |
| A4 | LCD SDA | I2C Data |
| A5 | LCD SCL | I2C Clock |

## Notas Importantes

### Alimentación
- Todos los componentes funcionan a 5V
- El Arduino puede suministrar hasta 500mA en total
- Para el servomotor, considerar alimentación externa si es necesario

### Conexiones I2C
- Los pines A4 (SDA) y A5 (SCL) están dedicados a I2C
- No usar estos pines para otros componentes
- El LCD I2C tiene dirección 0x27 por defecto

### Consideraciones de Diseño
- Mantener cables cortos para señales digitales
- Usar resistencias pull-up si es necesario
- Verificar polaridad del LED
- El sensor TCS3200 requiere buena iluminación

## Código de Referencia

```cpp
// Definición de pines en el código
#define TRIG 9
#define ECHO 10
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8
#define LED_ILUMINACION 12
#define SERVO_PIN 3
```

## Solución de Problemas

### Sensor TCS3200 no funciona
- Verificar conexiones S0-S3
- Comprobar alimentación 5V
- Asegurar buena iluminación

### Sensor HC-SR04 no detecta
- Verificar pines TRIG y ECHO
- Comprobar que ECHO esté en modo INPUT
- Verificar alimentación

### Servomotor no responde
- Verificar pin de señal
- Comprobar alimentación 5V
- Verificar librería Servo.h

### LCD no muestra nada
- Verificar conexiones I2C (A4, A5)
- Comprobar dirección I2C (0x27)
- Verificar alimentación 