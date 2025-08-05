# Componentes del Sistema Smart Bin Semi-Inteligente

## Componentes Principales

### 1. Arduino Uno/Nano/Mega
- **Función**: Microcontrolador principal
- **Especificaciones**: 5V, 16MHz
- **Pines utilizados**: 3, 4, 5, 6, 7, 8, 9, 10, 12

### 2. Sensor de Color TCS3200
- **Función**: Detección y clasificación semi-inteligente de colores
- **Pines**: S0(4), S1(5), S2(6), S3(7), sensorOut(8)
- **Configuración**: 20% de frecuencia (S0=HIGH, S1=LOW)
- **Filtros**: Rojo(LOW,LOW), Verde(HIGH,HIGH), Azul(LOW,HIGH)
- **Limitación**: Solo detecta colores, no materiales específicos

### 3. Sensor Ultrasónico HC-SR04
- **Función**: Detección de proximidad
- **Pines**: TRIG(9), ECHO(10)
- **Rango**: 2cm - 400cm
- **Precisión**: ±3mm

### 4. Servomotor SG90
- **Función**: Apertura/cierre del contenedor
- **Pin**: SERVO_PIN(3)
- **Rango**: 0° (cerrado) - 90° (abierto)
- **Voltaje**: 4.8V - 6V

### 5. Display LCD I2C
- **Función**: Interfaz de usuario
- **Dirección**: 0x27
- **Dimensiones**: 16x2 caracteres
- **Conexión**: I2C (SDA, SCL)

### 6. LED de Iluminación
- **Función**: Iluminación del área de detección
- **Pin**: LED_ILUMINACION(12)
- **Tipo**: LED de alta intensidad

## Conexiones Eléctricas

### Alimentación
- **Arduino**: 5V desde fuente de alimentación
- **Servomotor**: 5V independiente (recomendado)
- **Sensor de color**: 5V desde Arduino
- **Sensor ultrasónico**: 5V desde Arduino

### Conexiones de Señal
```
Arduino Pin 3  → Servomotor (señal)
Arduino Pin 4  → TCS3200 S0
Arduino Pin 5  → TCS3200 S1
Arduino Pin 6  → TCS3200 S2
Arduino Pin 7  → TCS3200 S3
Arduino Pin 8  → TCS3200 OUT
Arduino Pin 9  → HC-SR04 TRIG
Arduino Pin 10 → HC-SR04 ECHO
Arduino Pin 12 → LED de iluminación
```

## Consideraciones de Diseño Semi-Inteligente

### Calibración de Colores
Los valores de color están calibrados para el entorno específico. Para recalibrar:
1. Usar objetos de referencia conocidos
2. Medir valores RGB normalizados
3. Actualizar array `colores[]` en el código

### Limitaciones del Sistema
- **Clasificación limitada**: Solo por color, no por material
- **Dependencia de iluminación**: Requiere condiciones de luz consistentes
- **Calibración manual**: Necesita ajuste periódico
- **Falsos positivos**: Puede clasificar incorrectamente objetos similares

### Umbral de Distancia
- **Valor actual**: 10cm
- **Ajustable**: Modificar condición en `loop()`
- **Consideraciones**: Balancear sensibilidad vs falsos positivos

### Configuración del Servo
- **Posición cerrada**: 0°
- **Posición abierta**: 90°
- **Velocidad**: Ajustable en código 